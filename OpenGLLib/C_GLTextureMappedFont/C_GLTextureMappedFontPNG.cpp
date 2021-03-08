#include "C_GLTextureMappedFont.h"
#include <fstream>
#include <png.h>

unsigned long C_GLTextureMappedFont::FileSize( std::string str )
{
   unsigned long l,m;
   std::ifstream s( str.c_str(), std::ios::in | std::ios::binary );
   if( !s.is_open() )
   {
      return 0;
   }
   l = s.tellg();
   s.seekg( 0, std::ios::end );
   m = s.tellg();
   s.close();
   return (m-l);
}

bool C_GLTextureMappedFont::LoadEntireFileIntoMemory( char path[], char **buffer )
{
   unsigned long filesize = fileSize( path );
   try
   {
      *buffer = new char[ filesize ];
      std::memset( *buffer, 0, filesize );
      if( buffer == NULL )
         return false;
      std::ifstream file( path, std::ios::in | std::ios::binary );
      if( !file.is_open() )
      {
         delete[] buffer;
         return false;
      }
      file.read( *buffer, filesize );
      file.close();
   }
   catch( ... )
   {
      *buffer = NULL;
      return false;
   }
   return true;
}

void __cdecl C_GLTextureMappedFont::PngReadFunc( png_struct *Png, png_bytep buf, png_size_t size )
{
   TPngFileBuffer *PngFileBuffer = (TPngFileBuffer*)png_get_io_ptr( Png );
   std::memcpy( buf, PngFileBuffer->Buffer+PngFileBuffer->Pos, size );
   PngFileBuffer->Pos += size;
}

bool LoadPNG( char *name, char **pic, int *width, int *height )
{
   int         i, rowptr;
   png_structp png_ptr;
   png_infop   info_ptr;
   png_infop   end_info;

   unsigned char **row_Structs::Pointers;

   *pic = NULL;
   TPngFileBuffer PngFileBuffer = { NULL, 0 };

   LoadEntireFileIntoMemory( name, (char**)&PngFileBuffer.Buffer );

    if( !PngFileBuffer.Buffer )
      return false;

   if( (png_check_sig( PngFileBuffer.Buffer, 8 )) == 0 )
   {
      delete [] ( PngFileBuffer.Buffer );
      //Com_Printf( PRINT_ALL, "Not a PNG file: %s\n", name );
      return false;
   }

   PngFileBuffer.Pos = 0;

   png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL,  NULL, NULL );

   if( !png_ptr )
   {
      delete [] ( PngFileBuffer.Buffer );
      //Com_Printf (PRINT_ALL, "Bad PNG file: %s\n", name);
      return false;
   }

   info_ptr = png_create_info_struct( png_ptr );
   if( !info_ptr )
   {
      png_destroy_read_struct( &png_ptr, (png_infopp)NULL, (png_infopp)NULL );
      delete [] ( PngFileBuffer.Buffer );
      //Com_Printf( PRINT_ALL, "Bad PNG file: %s\n", name );
      return false;
   }

   end_info = png_create_info_struct( png_ptr );
   if (!end_info)
   {
      png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp)NULL );
      delete [] ( PngFileBuffer.Buffer );
      //Com_Printf( PRINT_ALL, "Bad PNG file: %s\n", name );
      return false;
   }

   png_set_read_fn( png_ptr, (png_voidp)&PngFileBuffer, (png_rw_ptr)PngReadFunc );

   png_read_png( png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL );

   row_Structs::Pointers = png_get_rows( png_ptr, info_ptr );

   rowptr = 0;

   *pic = (char*)new char[ info_ptr->width * info_ptr->height * sizeof(int) ];

   if( info_ptr->channels == 4 )
   {
      for( i = 0; i < info_ptr->height; i++ )
      {
         std::memcpy( *pic + rowptr, row_Structs::Pointers[i], info_ptr->rowbytes );
         rowptr += info_ptr->rowbytes;
      }
   }
   else
   {
      int j, x;
      std::memset( *pic, 255, info_ptr->width * info_ptr->height * sizeof(int) );
      x = 0;
      for( i = 0; i < info_ptr->height; i++ )
      {
         for( j = 0; j < info_ptr->rowbytes; j+=info_ptr->channels )
         {
            std::memcpy( *pic + x, row_Structs::Pointers[i] + j, info_ptr->channels );
            x += sizeof(int);
         }
         rowptr += info_ptr->rowbytes;
      }
   }

   *width  = info_ptr->width;
   *height = info_ptr->height;

   png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
   delete [] ( PngFileBuffer.Buffer );
   return true;
}

GLuint C_GLTextureMappedFont::LoadGLTexturePNGFile( std::string path )
{
   LoadGLTexturePNGFile( path.c_str() );
}

GLuint C_GLTextureMappedFont::LoadGLTexturePNGFile( char path[] )
{
   if( !IsExtensionSupported( "GL_EXT_bgra" ) )
   {
      return ERROR_CNLT;
   }

   GLuint texture = ERROR_CNLT;
   ::glGenTextures( 1, &texture ); // Generate Texture

   char *data = NULL;
   int width  = 0;
   int height = 0;
   if( LoadPNG( path, &data, &width, &height ) )
   {
      ::glBindTexture( GL_TEXTURE_2D, texture );  // Bind Our Texture   
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); // Linear Filtering
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); 


      if( IsExtensionSupported( "GL_EXT_texture_compression_s3tc" ) &&
          IsExtensionSupported( "GL_ARB_texture_compression" )
        )
      {
         ::glTexImage2D( GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data );
      }
      else
      {
         ::glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data );
      }
      
      delete[] data;
      data = NULL;
      return texture;
   }
   ::glDeleteTextures( 1, &texture );
   return ERROR_CNLT;
}
