#include "C_GLTextureManager.h"
/* TODO (#1#): Fix logic error in load textures cauing the last element 
               to be the first */

C_GLTextureManager::C_GLTextureManager( void ) : Textures( 0 )
{
   TexturesIndex    = 0;
   TexturesIterator = Textures.begin();
}

C_GLTextureManager::C_GLTextureManager( int NumberOfTextures ) : Textures( NumberOfTextures, ERROR_CNLT )
{
   TexturesIndex    = 0;
   TexturesIterator = Textures.begin();
}

C_GLTextureManager::~C_GLTextureManager()
{
   Clear();
}

void C_GLTextureManager::FreeTextures( GLsizei n )
{ 
   GLuint tmp = 0;
   if( ( n <= TexturesIndex )  &&  ( n > -1 ) )
   {
      // TODO (#1#): Some error handling ..... geeeeze
      // TODO (#1#): Check this code for logic errors
      short i=0;
       
      ::glDeleteTextures( n, &Textures[0] );
       
      for( i=0; i<n ; i++ )
      {
         Textures.erase( Textures.begin() );
         Textures.push_back(1);         
      }


      TexturesIterator = Textures.begin();
      TexturesIndex   -= n;
    }
}

GLuint C_GLTextureManager::LoadGLBitmapHandle( HBITMAP hBMP )
{
   if( (TexturesIndex+1) >= Textures.size() ) // +1 to account for zero indexing
   {
      Textures.resize( Textures.size()+1 );
   }

   ::glGenTextures( 1, &Textures[TexturesIndex] );   // Generate Textures


   if( hBMP )
   {
      BITMAP BMP;

      ::GetObject( hBMP, sizeof(BMP), &BMP );
      ::glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );  // Pixel Storage Mode (Word Alignment / 4 Bytes)
      ::glBindTexture( GL_TEXTURE_2D, Textures[TexturesIndex] );  // Bind Our Texture
   
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); // Linear Filtering
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); // Mipmap Linear Filtering

      // Generate Mipmapped Texture (3 Bytes, Width, Height And Data From The BMP)
      ::gluBuild2DMipmaps( GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits );
      ::DeleteObject( hBMP );  // Delete The Bitmap Object
      
      GLuint texture = Textures[TexturesIndex];
      TexturesIndex++;    
      
      return texture;
   }
   Textures.pop_back();   
   return ERROR_CNLT;
}

GLuint C_GLTextureManager::LoadGLTextureBitmapResource( DWORD texture )
{
   if( (TexturesIndex+1) >= Textures.size() ) // +1 to account for zero indexing
   {
     Textures.resize( Textures.size()+1 );
   }  

   HBITMAP hBMP;
   BITMAP  BMP;

   ::glGenTextures( 1, &Textures[TexturesIndex] );   // Generate Textures
   
   hBMP = (HBITMAP)::LoadImage( ::GetModuleHandle( NULL ), MAKEINTRESOURCE( texture ), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );
   if( hBMP )
   {
      ::GetObject( hBMP, sizeof(BMP), &BMP );
      ::glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );  // Pixel Storage Mode (Word Alignment / 4 Bytes)
      ::glBindTexture( GL_TEXTURE_2D, Textures[TexturesIndex] );  // Bind Our Texture
   
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); // Linear Filtering
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); // Mipmap Linear Filtering

      // Generate Mipmapped Texture (3 Bytes, Width, Height And Data From The BMP)
      ::gluBuild2DMipmaps( GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits );
      ::DeleteObject( hBMP );  // Delete The Bitmap Object      
      
      GLuint texture = Textures[TexturesIndex];
      TexturesIndex++;
      
      return texture;
   }
   Textures.pop_back();   
   return ERROR_CNLT;
}

GLuint C_GLTextureManager::LoadGLTextureBitmapFile( std::string path )
{
   return LoadGLTextureBitmapFile( (char*) path.c_str() );
}

GLuint C_GLTextureManager::LoadGLTextureBitmapFile( char path[] )
{
   if( (TexturesIndex+1) >= Textures.size() ) // +1 to account for zero indexing
   {
     Textures.resize( Textures.size()+1 );
   }

   HBITMAP hBMP;
   BITMAP  BMP;

   ::glGenTextures( 1, &Textures[TexturesIndex] );   // Generate Textures
   
   hBMP = (HBITMAP)::LoadImage( NULL, path, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );
   if( hBMP )
   {
      ::GetObject( hBMP, sizeof(BMP), &BMP );
      ::glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );  // Pixel Storage Mode (Word Alignment / 4 Bytes)
      ::glBindTexture( GL_TEXTURE_2D, Textures[TexturesIndex] );  // Bind Our Texture
   
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); // Linear Filtering
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); // Mipmap Linear Filtering

      // Generate Mipmapped Texture (3 Bytes, Width, Height And Data From The BMP)
      ::gluBuild2DMipmaps( GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits );
      ::DeleteObject( hBMP );  // Delete The Bitmap Object
      
      GLuint texture = Textures[TexturesIndex];
      TexturesIndex++;    
      
      return texture;
   }
   Textures.pop_back();   
   return ERROR_CNLT;
}

GLuint C_GLTextureManager::LoadGLTextureBitmapFile( std::string path, COLORREF transparent_color )
{
   return LoadGLTextureBitmapFile( (char*) path.c_str(), transparent_color );
}

GLuint C_GLTextureManager::LoadGLTextureBitmapFile( char path[], COLORREF transparent_color )
{
   if( !IsExtensionSupported( "GL_EXT_bgra" ) )
   {
      return ERROR_CNLT;
   }

   if( (TexturesIndex+1) >= Textures.size() ) // +1 to account for zero indexing
   {
     Textures.resize( Textures.size()+1 );
   }

   HBITMAP hBMP;
   BITMAP  BMP;

   ::glGenTextures( 1, &Textures[TexturesIndex] );   // Generate Textures
   
   hBMP = (HBITMAP)::LoadImage( NULL, path, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );
   if( hBMP )
   {
      ::GetObject( hBMP, sizeof(BMP), &BMP );
      ::glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );  // Pixel Storage Mode (Word Alignment / 4 Bytes)
      ::glBindTexture( GL_TEXTURE_2D, Textures[TexturesIndex] );  // Bind Our Texture
   
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); // Linear Filtering
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); // Mipmap Linear Filtering

      unsigned char *data = new unsigned char[ BMP.bmWidth * BMP.bmHeight * 4 ];
      unsigned int  pixels = BMP.bmWidth * BMP.bmHeight;
      unsigned char color[3] = {0,0,0};
      unsigned char * bits = (unsigned char*)BMP.bmBits;
      for( int i=0; i<pixels ; ++i )
      {
         color[0] = *(bits+(i*3));
         color[1] = *(bits+(i*3)+1);
         color[2] = *(bits+(i*3)+2);
         if( color[0] == GetBValue(transparent_color) &&
             color[1] == GetGValue(transparent_color) &&
             color[2] == GetRValue(transparent_color)
           )
         {
            data[ i*4    ] = color[0];
            data[ i*4 +1 ] = color[1];
            data[ i*4 +2 ] = color[2];
            data[ i*4 +3 ] = 0;
         }
         else
         {
            data[ i*4    ] = color[0];
            data[ i*4 +1 ] = color[1];
            data[ i*4 +2 ] = color[2];
            data[ i*4 +3 ] = 255;
         }
      }

      if( IsExtensionSupported( "GL_EXT_texture_compression_s3tc" ) &&
          IsExtensionSupported( "GL_ARB_texture_compression" )
        )
      {
         //::gluBuild2DMipmaps( GL_TEXTURE_2D, 4, BMP.bmWidth, BMP.bmHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data );
         ::gluBuild2DMipmaps( GL_TEXTURE_2D, GL_COMPRESSED_RGBA_ARB, BMP.bmWidth, BMP.bmHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data );
      }
      else
      {
         // Generate Mipmapped Texture (4 Bytes, Width, Height And Data From The BMP)
         ::gluBuild2DMipmaps( GL_TEXTURE_2D, 4, BMP.bmWidth, BMP.bmHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data );
      }

      ::DeleteObject( hBMP );  // Delete The Bitmap Object
      delete[] data;
      
      GLuint texture = Textures[TexturesIndex];
      TexturesIndex++;    
      
      return texture;
   }
   Textures.pop_back();   
   return ERROR_CNLT;
}

GLuint C_GLTextureManager::LoadGLTextureRAW( const char * filename, int wrap, int width, int height )
{
   if( (TexturesIndex+1) >= Textures.size() ) // +1 to account for zero indexing
   {
      Textures.resize( Textures.size()+1 );
   }

   BYTE * data;
   FILE * file;  

   // open texture data
   file = ::fopen( filename, "rb" );
   if ( file == NULL )
   {
      Textures.pop_back();
      return ERROR_CNLT;
   }

   // allocate buffer
   data = (BYTE*) ::malloc( width * height * 3 );

   // read texture data
   ::fread( data, width * height * 3, 1, file );
   ::fclose( file );

   // allocate a texture name
   ::glGenTextures( 1, &Textures[TexturesIndex] );

   // select our current texture
   ::glBindTexture( GL_TEXTURE_2D, Textures[TexturesIndex] );

   // select modulate to mix texture with color for shading
   ::glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

   // when texture area is small, bilinear filter the closest MIP map
   ::glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
   
   // when texture area is large, bilinear filter the first MIP map
   ::glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   // if wrap is true, the texture wraps over at the edges (repeat)
   //       ... false, the texture ends at the edges (clamp)
   ::glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                      wrap ? GL_REPEAT : GL_CLAMP );
   ::glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                      wrap ? GL_REPEAT : GL_CLAMP );

   // build our texture MIP maps
   ::gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );

   ::glFlush();

   // free buffer
   ::free( data );
  
   GLuint texture = Textures[TexturesIndex];
   TexturesIndex++;
     
  return texture;
}

GLuint C_GLTextureManager::LoadGLTextureRAW( std::string filename, int wrap, int width, int height )
{
   return LoadGLTextureRAW( filename.c_str() , wrap, width, height );
}

