#pragma once

#ifndef __C_GLTextureMappedFont_H
#define __C_GLTextureMappedFont_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include <string>
#include <sstream>

#include "../C_GLTextureManagerHash/C_GLTextureManagerHash.h"
#include "../../errorutil/errorutil.h"
#include "../C_GLErrorStrings/C_GLErrorStrings.h"
#include "../OpenGLUtility/OpenGLUtility.h"

class C_GLTextureMappedFont
{
   public:

      C_GLTextureMappedFont( float char_offset_x = 0.8f ) : m_base( 0 ), m_texture_id( ERROR_CNLT ), m_bFontBuilt( false ), 
                             m_char_width( 0.0 ), m_char_height( 0.0 ), m_char_offset_x( char_offset_x )
      {
      }
      ~C_GLTextureMappedFont()
      { 
         Destroy();
      }

      bool BuildFont( std::string filename, COLORREF transparent_color = RGB( 0, 0, 0 ), float char_offset_x = 0.8f, float scale_x=16.5, float scale_y=16.5 );
      //GLvoid KillFont( GLvoid ); // Delete The Font From Memory
      GLvoid KillFont(); // Delete The Font From Memory

      void Print( const char*, bool bshow_transparent = true, int set = 1 );
      void Print( const char*, const float, const float, const float, bool bshow_transparent = true, bool PushMatrix = true, int set = 1 );

      inline void Print( const std::string& text, const float x, const float y, const float z, bool bshow_transparent = true, bool PushMatrix = true, int set = 1 )
      {
         Print( text.c_str(), x, y, z, bshow_transparent, PushMatrix, set );
      }
      inline void Print( const std::ostringstream& text, const float x, const float y, const float z, bool bshow_transparent = true, bool PushMatrix = true, int set = 1 )
      {
         Print( text.str(), x, y, z, bshow_transparent, PushMatrix, set );
      }

      inline float Length( const std::string& text )
      {
         if( !m_bFontBuilt )
            return 0;
         return Length( text.c_str() );
      }

      inline float Height( const char* text )
      {
         if( !m_bFontBuilt )
            return 0;
         float height = m_char_height; // Increase Length By Each Characters Width
         return height;
      }

      inline float Height( const char text )
      {
         if( !m_bFontBuilt )
            return 0;
         float height = m_char_height; // Increase Length By Each Characters Width
         return height;
      }

      float Length( const char* text );
      
      GLuint GetTexture() const
      {
         return m_texture_id;
      }

   protected:
      void Destroy( void );

      GLuint m_base, m_texture_id;
      bool   m_bFontBuilt;
      float  m_char_offset_x;
      float  m_char_width, m_char_height;
      C_GLTextureManagerHash m_TextureManager;
      std::string texturename;

   private:
};

#endif
