#pragma once

#ifndef __C_GLOutLinedFont_H
#define __C_GLOutLinedFont_H

#include <windows.h>
#include <gl/glu.h>
#include "../C_GLErrorStrings/C_GLErrorStrings.h"
#include "../../errorutil/errorutil.h"
#include <string>
#include <sstream>
#include "../../WinHFont/WinHFont.h"

class C_GLOutLinedFont
{
   static const long characters = 256;
   public:

       C_GLOutLinedFont( void ): m_base( 0 ), m_Hdc( NULL ), m_Hwnd( NULL ), m_bFontBuilt( false )
          {
             WinHFont::S_FontProperties fp;
             fp.nHeight            = -24;
             fp.nWidth             = 0;
             fp.nEscapement        = 0;
             fp.nOrientation       = 0;
             fp.fnWeight           = 0;//FW_BOLD;
             fp.fdwItalic          = false;
             fp.fdwUnderline       = false;
             fp.fdwStrikeOut       = false;
             fp.fdwCharSet         = DEFAULT_CHARSET;
             fp.fdwOutputPrecision = OUT_TT_PRECIS;
             fp.fdwClipPrecision   = CLIP_DEFAULT_PRECIS;
             fp.fdwQuality         = ANTIALIASED_QUALITY;
             fp.fdwPitchAndFamily  = FF_DONTCARE|DEFAULT_PITCH;
             fp.lpszFace           = new char[ strlen("Times New Roman")+1 ];
             strncpy( (char*)fp.lpszFace, "Times New Roman\0", strlen("Times New Roman")+1 );
             m_HFont.Create( fp );
          }
       C_GLOutLinedFont( std::string face ): m_base( 0 ), m_Hdc( NULL ), m_Hwnd( NULL ), m_bFontBuilt( false )
          {
             WinHFont::S_FontProperties fp;
             fp.nHeight            = -24;
             fp.nWidth             = 0;
             fp.nEscapement        = 0;
             fp.nOrientation       = 0;
             fp.fnWeight           = 0;//FW_BOLD;
             fp.fdwItalic          = false;
             fp.fdwUnderline       = false;
             fp.fdwStrikeOut       = false;
             fp.fdwCharSet         = DEFAULT_CHARSET;
             fp.fdwOutputPrecision = OUT_TT_PRECIS;
             fp.fdwClipPrecision   = CLIP_DEFAULT_PRECIS;
             fp.fdwQuality         = ANTIALIASED_QUALITY;
             fp.fdwPitchAndFamily  = FF_DONTCARE|DEFAULT_PITCH;
             fp.lpszFace           = new char[ face.length()+1 ];
             strncpy( (char*)fp.lpszFace, face.c_str(), face.length()+1 );
             m_HFont.Create( fp );
          }
       C_GLOutLinedFont( std::string face, WinHFont::S_FontProperties _fp ): m_base( 0 ), m_Hdc( NULL ), m_Hwnd( NULL ), m_bFontBuilt( false )
          {
             WinHFont::S_FontProperties fp = _fp;
             fp.lpszFace           = new char[ face.length()+1 ];
             strncpy( (char*)fp.lpszFace, face.c_str(), face.length()+1 );
             m_HFont.Create( fp );
          }
       C_GLOutLinedFont( const HWND &Hwnd, const WinHFont& Hfont ) :
                       m_Hwnd( Hwnd ), m_HFont( Hfont ), m_bFontBuilt( false )
          {
             m_Hdc = ::GetDC( m_Hwnd );
          }

      ~C_GLOutLinedFont( void )
          {
             DestroyFont();
          }


      bool BuildFont( void );
      bool BuildFont( const HWND );

      void Print( std::ostringstream str ){ Print( str.str() ); }
      void Print( const std::string& );
      void Print( const char* );
      void Print( const char*, const float, const float, const float, bool PushMatrix = true );
      void Print( const std::string& text, const float x, const float y, const float z, bool PushMatrix = true );
      void Print( std::ostringstream text, const float x, const float y, const float z, bool PushMatrix = true );
      void Print( std::ostringstream text, const float x, const float y, const float z, int flags, bool PushMatrix = true );

      void DestroyFont( void )
      {
         if( m_bFontBuilt )
         {
            ::glDeleteLists( m_base, characters ); // Delete All 96 Characters
            ::ReleaseDC( m_Hwnd, m_Hdc );
            m_bFontBuilt = false;
            m_base = 0;
         }
      }

      void inline SetHWND( const HWND& Hwnd )
      {
         DestroyFont();
         m_Hwnd = Hwnd;
         m_Hdc = ::GetDC( m_Hwnd );
      }

      bool SetFontFace( const char * fontface )
      {
         DestroyFont();
//         m_FontProperties.lpszFace = fontface;
         return BuildFont();
      }

      bool SetFontFace( std::string& fontface )
      {
         return SetFontFace( fontface.c_str() );
      }

      inline float Length( const std::string& text )
      {
         return Length( text.c_str() );
      }

      inline float Height( const char* text )
      {
         float height = gmf[ text[0] ].gmfCellIncY; // Increase Length By Each Characters Width
         return height;
      }

      inline float Height( const char text )
      {
         float height = gmf[ text ].gmfCellIncY; // Increase Length By Each Characters Width
         return height;
      }

      float Length( const char* text )
      {
         float length = 0;
         int   len    = ::strlen( text );
         for( unsigned int loop=0; loop < len ; loop++ ) // Loop To Find Text Length
         {
            length += gmf[ text[loop] ].gmfCellIncX; // Increase Length By Each Characters Width
         }
         return length;
      }

      GLuint GetBase() const
      {
         return m_base;
      }

   protected:

      bool             m_bFontBuilt;
      GLuint           m_base;
      HDC              m_Hdc;
      HWND             m_Hwnd;
      WinHFont       m_HFont;

      GLYPHMETRICSFLOAT gmf[characters]; // Storage For Information About Our Outline Font Characters

   private:
};

#endif
