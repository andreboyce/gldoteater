#include "C_GLTextureManagerHash.h"

#include <ddraw.h>

typedef void ( APIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DARBPROC )( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data );
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB = 0;

struct DDS_IMAGE_DATA
{
    GLsizei  width;
    GLsizei  height;
    GLint    components;
    GLenum   format;
    int      numMipMaps;
    GLubyte *pixels;
};

DDS_IMAGE_DATA* loadDDSTextureFile( std::string filename )
{
    DDS_IMAGE_DATA *pDDSImageData;
    DDSURFACEDESC2 ddsd;
    char filecode[4];
    FILE *pFile;
    int factor;
    int bufferSize;

    // Open the file
    pFile = fopen( filename.c_str(), "rb" );

    if( pFile == NULL )
    {
        char str[255];
        sprintf( str, "loadDDSTextureFile couldn't find, or failed to load \"%s\"", filename.c_str() );
        MessageBox( NULL, str, "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return NULL;
    }

    // Verify the file is a true .dds file
    fread( filecode, 1, 4, pFile );

    if( strncmp( filecode, "DDS ", 4 ) != 0 )
    {
        char str[255];
        sprintf( str, "The file \"%s\" doesn't appear to be a valid .dds file!", filename.c_str() );
        MessageBox( NULL, str, "ERROR", MB_OK|MB_ICONEXCLAMATION );
        fclose( pFile );
        return NULL;
    }

    // Get the surface descriptor
    fread( &ddsd, sizeof(ddsd), 1, pFile );

    pDDSImageData = (DDS_IMAGE_DATA*) malloc(sizeof(DDS_IMAGE_DATA));

    memset( pDDSImageData, 0, sizeof(DDS_IMAGE_DATA) );

    //
    // This .dds loader supports the loading of compressed formats DXT1, DXT3 
    // and DXT5.
    //

    switch( ddsd.ddpfPixelFormat.dwFourCC )
    {
        case FOURCC_DXT1:
            // DXT1's compression ratio is 8:1
            pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            factor = 2;
            break;

        case FOURCC_DXT3:
            // DXT3's compression ratio is 4:1
            pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            factor = 4;
            break;

        case FOURCC_DXT5:
            // DXT5's compression ratio is 4:1
            pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            factor = 4;
            break;

        default:
            char str[255];
            sprintf( str, "The file \"%s\" doesn't appear to be compressed " 
                          "using DXT1, DXT3, or DXT5!", filename.c_str() );
            MessageBox( NULL, str, "ERROR", MB_OK|MB_ICONEXCLAMATION );
            return NULL;
    }

    //
    // How big will the buffer need to be to load all of the pixel data 
    // including mip-maps?
    //

    if( ddsd.dwLinearSize == 0 )
    {
        MessageBox( NULL, "dwLinearSize is 0!","ERROR",
            MB_OK|MB_ICONEXCLAMATION);
    }

    if( ddsd.dwMipMapCount > 1 )
        bufferSize = ddsd.dwLinearSize * factor;
    else
        bufferSize = ddsd.dwLinearSize;

    pDDSImageData->pixels = (unsigned char*)malloc(bufferSize * sizeof(unsigned char));

    fread( pDDSImageData->pixels, 1, bufferSize, pFile );

    // Close the file
    fclose( pFile );

    pDDSImageData->width      = ddsd.dwWidth;
    pDDSImageData->height     = ddsd.dwHeight;
    pDDSImageData->numMipMaps = ddsd.dwMipMapCount;

    if( ddsd.ddpfPixelFormat.dwFourCC == FOURCC_DXT1 )
        pDDSImageData->components = 3;
    else
        pDDSImageData->components = 4;

    return pDDSImageData;
}

bool loadCompressedTexture( char *filename )
{
    // NOTE: Unlike "lena.bmp", "lena.dds" actually contains its own mip-map 
    // levels, which are also compressed.
    DDS_IMAGE_DATA *pDDSImageData = loadDDSTextureFile( filename );

   bool barbcompressionsupport = false;
   if( IsExtensionSupported( "GL_ARB_texture_compression" ) )
   {
       barbcompressionsupport = true;
   }


    if( pDDSImageData != NULL && barbcompressionsupport )
    {
        int nHeight     = pDDSImageData->height;
        int nWidth      = pDDSImageData->width;
        int nNumMipMaps = pDDSImageData->numMipMaps;

        int nBlockSize;

        if( pDDSImageData->format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )
            nBlockSize = 8;
        else
            nBlockSize = 16;

        int nSize;
        int nOffset = 0;

        glCompressedTexImage2DARB = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC) wglGetProcAddress( "glCompressedTexImage2DARB" );
        
        // Load the mip-map levels
        for( int i = 0; i < nNumMipMaps; ++i )
        {
            if( nWidth  == 0 ) nWidth  = 1;
            if( nHeight == 0 ) nHeight = 1;

            nSize = ((nWidth+3)/4) * ((nHeight+3)/4) * nBlockSize;

            glCompressedTexImage2DARB( GL_TEXTURE_2D,
                                       i,
                                       pDDSImageData->format,
                                       nWidth,
                                       nHeight,
                                       0,
                                       nSize,
                                       pDDSImageData->pixels + nOffset );

            nOffset += nSize;

            // Half the image size for the next mip-map level...
            nWidth  = (nWidth  / 2);
            nHeight = (nHeight / 2);
        }
    }

    if( pDDSImageData != NULL )
    {
        if( pDDSImageData->pixels != NULL )
            free( pDDSImageData->pixels );

        free( pDDSImageData );
        if( !barbcompressionsupport )
           return false;
        return true;
    }
    return false;
}

GLuint C_GLTextureManagerHash::LoadGLTextureDDSFile( std::string path )
{
   if( TextureAlreadyLoaded( path ) && ::glIsTexture( m_Textures[ path ] ) )
   {
       return m_Textures[ path ];
   }

   GLuint texture = ERROR_CNLT;
   ::glGenTextures( 1, &texture );

   ::glBindTexture( GL_TEXTURE_2D, texture );
   ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP  );
   ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP  );

   if( loadCompressedTexture( (char*) path.c_str() ) )
   {
      m_Textures[ path ] = texture;
      return texture;
   }

   ::glDeleteTextures( 1, &texture );
   return ERROR_CNLT;

}
