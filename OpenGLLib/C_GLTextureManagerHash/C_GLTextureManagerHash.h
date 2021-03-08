#pragma once

#ifndef __C_GLTextureManagerHash_H
#define __C_GLTextureManagerHash_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>

//#include <png.h> add back
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "../../errorutil/errorutil.h"
#include "../C_GLErrorStrings/C_GLErrorStrings.h"
#include "../OpenGLUtility/OpenGLUtility.h"
#include "../../WindowsUtility/WindowsUtility.h"
#include "../../Structs/Structs.h"

#ifndef ERROR_CNLT
#define ERROR_CNLT                 0xFFFFFF
#endif
#define DEFAULT_NUMBER_OF_TEXTURES 0

class C_GLTextureManagerHash
{
   public:

      C_GLTextureManagerHash( void );
      ~C_GLTextureManagerHash();

      struct Texture
      {
         Texture() : textureID(ERROR_CNLT), type(GL_TEXTURE_2D), width(0.0), height(0.0) {}
         ~Texture(){ ::glDeleteTextures( 1, &textureID ); }
         inline operator GLuint () const
         {
            return textureID;
         }
         GLuint operator = ( GLuint _textureID )
         {
            textureID = _textureID;
            return textureID;
         }
         Texture& operator = ( const Texture& rhs )
         {
            textureID = rhs.textureID;
            type      = rhs.type;
            width     = rhs.width; 
            height    = rhs.height;
            return *this;
         }
         inline operator bool() const
         {
            return (bool) ::glIsTexture( textureID );
         }
         GLuint textureID;
         GLenum type;
         float width, height;
      };

      // regular members
      GLuint LoadGLTextureRAW( const char *, int, int = 256, int = 256 );
      GLuint LoadGLTextureRAW( std::string,  int, int = 256, int = 256 );
      GLuint LoadGLTextureBitmapFile( char path[] );
      GLuint LoadGLTextureBitmapFile( char path[], COLORREF transparent_color );
      GLuint LoadGLTextureBitmapFile( std::string path );
      GLuint LoadGLTextureBitmapFile( std::string path, COLORREF transparent_color );
      GLuint LoadGLTextureBitmapResource( DWORD resource, std::string name );
      GLuint LoadGLBitmapHandle( HBITMAP, std::string name );
      GLuint LoadGLTexturePNGFile( std::string path );
      GLuint LoadGLTextureTGAFile( std::string path );
      GLuint LoadGLTextureDDSFile( std::string path );
      GLuint LoadGLTextureFile( std::string path, COLORREF transparent_color = RGB( 0, 0, 0 ) );

      bool BindTexture( std::string name );

      bool TextureAlreadyLoaded( std::string name )
      {
        if( (*this)[name] != ERROR_CNLT )
        {
            return true;
        }
        return false;
      }

      // inline members

      bool inline clear( void )
      {
         m_Textures.clear();
		 return true;
      }

      long inline size( void ) const
      {
         return (long)m_Textures.size(); // size is the number of textures loded
      }

      GLuint operator [] ( std::string name );

      struct SubTexture
      {
         SubTexture() : texture( 0 ), TexCoord() {}
         SubTexture( C_TexCoord _TexCoord[4], GLuint _texture, std::string _parent_name ) : texture( _texture ), parent_name( _parent_name )
         {
            TexCoord[0] = _TexCoord[0];
            TexCoord[1] = _TexCoord[1];
            TexCoord[2] = _TexCoord[2];
            TexCoord[3] = _TexCoord[3];
         }
         SubTexture( const SubTexture& rhs ) : texture( rhs.texture ), parent_name( rhs.parent_name )
         {
            TexCoord[0] = rhs.TexCoord[0];
            TexCoord[1] = rhs.TexCoord[1];
            TexCoord[2] = rhs.TexCoord[2];
            TexCoord[3] = rhs.TexCoord[3];
         }
         SubTexture& operator=( const SubTexture& rhs )
         { 
            texture     = rhs.texture;
            parent_name = rhs.parent_name;
            TexCoord[0] = rhs.TexCoord[0];
            TexCoord[1] = rhs.TexCoord[1];
            TexCoord[2] = rhs.TexCoord[2];
            TexCoord[3] = rhs.TexCoord[3];
            return *this;
         }
         void settexcoords( C_TexCoord _TexCoord[4] )
         {
            TexCoord[0] = _TexCoord[0];
            TexCoord[1] = _TexCoord[1];
            TexCoord[2] = _TexCoord[2];
            TexCoord[3] = _TexCoord[3];
         }
         void settexcoords( C_TexCoord Coord1, C_TexCoord Coord2, C_TexCoord Coord3, C_TexCoord Coord4 )
         {
            TexCoord[0] = Coord1;
            TexCoord[1] = Coord2;
            TexCoord[2] = Coord3;
            TexCoord[3] = Coord4;
         }
         void set( GLuint _texture, std::string _parent_name, C_TexCoord Coord1, C_TexCoord Coord2, C_TexCoord Coord3, C_TexCoord Coord4 )
         {
            TexCoord[0] = Coord1;
            TexCoord[1] = Coord2;
            TexCoord[2] = Coord3;
            TexCoord[3] = Coord4;
            texture     = _texture;
            parent_name = _parent_name;
         }
         ~SubTexture(){}
         C_TexCoord  TexCoord[4];
         GLuint      texture;
         std::string parent_name;
      };

      bool FindSubTexture( std::string name );
      GLuint GetSubTextureID( std::string name );
      bool AddSubTexture( std::string name, C_TexCoord TexCoord[4], GLuint parent_texture, std::string parent_name );
      bool AddSubTexture( std::string name, SubTexture subtexture );
      bool GetSubTextureCoords( std::string name, C_TexCoord TexCoord[4] );
      bool GetSubTextureCoords( std::string name, Structs::fPoint TexCoords[4] );
      bool LoadTAI( std::string path );
     
   protected:
      std::map< std::string, Texture    > m_Textures;
      std::map< std::string, SubTexture > m_SubTextures;

   private:      
};

#endif
