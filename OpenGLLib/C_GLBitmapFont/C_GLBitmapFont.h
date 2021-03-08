#ifndef __C_GLBitmapFont_H
#define __C_GLBitmapFont_H

#include <windows.h>
#include <gl/glu.h>
#include <string>

class C_GLBitmapFont
{
   public:
   
   struct S_FontProperties
   {
      public:
      
      S_FontProperties( void )
         {
            nHeight            = -24;
            nWidth             = 0;
            nEscapement        = 0;
            nOrientation       = 0;
            fnWeight           = FW_BOLD;
            fdwItalic          = false;
            fdwUnderline       = false;
            fdwStrikeOut       = false;
            fdwCharSet         = ANSI_CHARSET;
            fdwOutputPrecision = OUT_TT_PRECIS;
            fdwClipPrecision   = CLIP_DEFAULT_PRECIS;
            fdwQuality         = ANTIALIASED_QUALITY;
            fdwPitchAndFamily  = FF_DONTCARE|DEFAULT_PITCH;
            lpszFace           = "Times New Roman";
         };
      
         int     nHeight;             // logical height of font
         int     nWidth;              // logical average character width
         int     nEscapement;         // angle of escapement
         int     nOrientation;        // base-line orientation angle
         int     fnWeight;            // font weight
         DWORD   fdwItalic;           // italic attribute flag
         DWORD   fdwUnderline;        // underline attribute flag
         DWORD   fdwStrikeOut;        // strikeout attribute flag
         DWORD   fdwCharSet;          // character set identifier
         DWORD   fdwOutputPrecision;  // output precision
         DWORD   fdwClipPrecision;    // clipping precision
         DWORD   fdwQuality;          // output quality
         DWORD   fdwPitchAndFamily;   // pitch and family
         LPCTSTR lpszFace;            // Structs::Pointer to typeface name string
   };

       C_GLBitmapFont( void ): m_base( 0 ), m_Hdc( NULL ), m_Hwnd( NULL ), m_bFontBuilt( 0 )
          {   }
          
       C_GLBitmapFont( const HWND &Hwnd, const S_FontProperties& FP ): 
                       m_Hwnd( Hwnd ), m_FontProperties( FP ), m_bFontBuilt( 0 )
          {
             m_Hdc = ::GetDC( m_Hwnd );
          }

      ~C_GLBitmapFont( void )
          {
             DestroyFont();
          }

       bool BuildFont( void );
       bool BuildFont( const HWND& );

       bool SetFontFace( const char * );
       bool SetFontFace( std::string & );

       bool Print( std::string );
       bool Print( const char* );
       bool Print( const char*, float, float, float );
       bool Print( std::string, float, float, float );
       
       void DestroyFont( void )
       {
          if( m_bFontBuilt )
          {
             ::glDeleteLists( m_base, 96 ); // Delete All 96 Characters
             ::ReleaseDC( m_Hwnd, m_Hdc );
             m_bFontBuilt = false;
          }
       }

       void inline SetHWND( const HWND& Hwnd )
          {
             DestroyFont();
             m_Hwnd = Hwnd;
             m_Hdc = ::GetDC( m_Hwnd );
          }

   protected:

      bool             m_bFontBuilt;
      GLuint           m_base;
      HDC              m_Hdc;
      HWND             m_Hwnd;
      S_FontProperties m_FontProperties;

   private:

};

#endif
