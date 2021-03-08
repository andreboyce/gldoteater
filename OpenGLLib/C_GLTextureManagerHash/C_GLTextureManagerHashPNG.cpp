#include "C_GLTextureManagerHash.h"

#include <windows.h>
#include <fstream>
#include "glpng/glpng.h"

GLuint C_GLTextureManagerHash::LoadGLTexturePNGFile( std::string path )
{
   if( TextureAlreadyLoaded( path ) && ::glIsTexture( m_Textures[ path ] ) )
   {
       return m_Textures[ path ];
   }

   GLuint texture = ERROR_CNLT;
   ::glGenTextures( 1, &texture ); // Generate Texture

   pngInfo info;
   ::glBindTexture( GL_TEXTURE_2D, texture );
   ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP  );
   ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP  );
   pngSetStandardOrientation( 1 );

   if( pngLoad( path.c_str(), PNG_NOMIPMAP, PNG_ALPHA, &info ) )
   {
      m_Textures[ path ] = texture;
      return texture;
   }

   ::glDeleteTextures( 1, &texture );
   return ERROR_CNLT;
}
