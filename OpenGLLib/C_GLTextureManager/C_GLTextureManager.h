#pragma once

#ifndef __C_GLTextureManager_H
#define __C_GLTextureManager_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include <string>
#include <vector>
#include <map>
#include "../../errorutil/errorutil.h"
#include "../OpenGLUtility/OpenGLUtility.h"

#define ERROR_CNLT                 0xFFFFFF
#define DEFAULT_NUMBER_OF_TEXTURES 0

class C_GLTextureManager
{
   public:
   
      C_GLTextureManager( void );
      C_GLTextureManager( int );      
      ~C_GLTextureManager();

      // typedefs
      typedef std::vector<GLuint>  V_Textures;
      typedef V_Textures::iterator V_TexturesIterator;
      
      // regular members
      GLuint LoadGLTextureRAW( const char *, int, int = 256, int = 256 );
      GLuint LoadGLTextureRAW( std::string,  int, int = 256, int = 256 );
      GLuint LoadGLTextureBitmapFile( char path[] );
      GLuint LoadGLTextureBitmapFile( char path[], COLORREF transparent_color );
      GLuint LoadGLTextureBitmapFile( std::string path );
      GLuint LoadGLTextureBitmapFile( std::string path, COLORREF transparent_color  );
      GLuint LoadGLTextureBitmapResource( DWORD );
      GLuint LoadGLBitmapHandle( HBITMAP );
      void   FreeTextures(   GLsizei );
      
      // inline members
      
      bool   inline Clear( void )
      {
         FreeTextures( TexturesIndex );
         Textures.clear();
         TexturesIndex    = 0;
         TexturesIterator = Textures.begin();
		 return true;
      }
      
      long   inline Size( void ) const
      {
         return (long)Textures.size(); // size is the number of textures loded
      }
      
      // operators
      
      std::vector<GLuint>::reference operator [] ( int index )
      {
         if( Textures.size() <= index ) // Is out of bounds
         {
            GLuint a = ERROR_CNLT;
            std::vector<GLuint>::reference t = a;
            return t;
         }
         return Textures.at( index );
      }

	  /*
	  std::vector<GLuint> operator [] (int index)
	  {
		  if (Textures.size() <= index) // Is out of bounds
		  {
			  GLuint a = ERROR_CNLT;
			  std::vector<GLuint> t = a;
			  return t;
		  }
		  //return Textures.at(index);
		  return Textures[index];
	  }
	  */

//std::vector<GLuint>::reference
      GLuint LastTexture( void )
      {
         if( Textures.size() <= 0 ) // Is out of bounds
         {
            return ERROR_CNLT;
         }
         return Textures.at( Textures.size()-1 );

//         if( Textures.size() <= 0 ) // Is out of bounds
//         {
//            GLuint a = ERROR_CNLT;
//            std::vector<GLuint>::reference t = a;
//            return t;
//         }
//         return Textures.at( Textures.size()-1 );
      }
     
   protected:
   
      V_Textures          Textures;
      GLsizei             TexturesIndex;
      V_TexturesIterator  TexturesIterator;
      
   private:
      
};


#endif
