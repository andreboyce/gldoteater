#include "C_GLBitmapFont.h"

bool C_GLBitmapFont::BuildFont( const HWND& Hwnd )
{
   SetHWND( Hwnd );
   return BuildFont();
}

bool C_GLBitmapFont::BuildFont( void )
{
   if( m_Hdc )
   {
      HFONT font;
      HFONT oldfont;

      m_base = ::glGenLists( 96 );   // Storage For 96 Characters

      font = (HFONT)::CreateFont(
                                  m_FontProperties.nHeight,
                                  m_FontProperties.nWidth,
                                  m_FontProperties.nEscapement,
                                  m_FontProperties.nOrientation,
                                  m_FontProperties.fnWeight,
                                  m_FontProperties.fdwItalic,
                                  m_FontProperties.fdwUnderline,
                                  m_FontProperties.fdwStrikeOut,
                                  m_FontProperties.fdwCharSet,
                                  m_FontProperties.fdwOutputPrecision,
                                  m_FontProperties.fdwClipPrecision,
                                  m_FontProperties.fdwQuality,
                                  m_FontProperties.fdwPitchAndFamily,
                                  m_FontProperties.lpszFace  );
   
      oldfont = (HFONT)::SelectObject( m_Hdc, font ); // Selects The Font We Want
      ::wglUseFontBitmaps( m_Hdc, 32, 96, m_base );   // Builds 96 Characters Starting At Character 32
      ::SelectObject( m_Hdc, oldfont );               // Selects The Font We Want
      ::DeleteObject( font );                         // Delete The Font
      m_bFontBuilt = true;
   }
   return true;
}

bool C_GLBitmapFont::SetFontFace( const char * fontface )
{
   ::glDeleteLists( m_base, 96 ); // Delete All 96 Characters
   m_FontProperties.lpszFace = fontface;
   return BuildFont();
}

bool C_GLBitmapFont::SetFontFace( std::string& fontface )
{
   return SetFontFace( fontface.c_str() );
}

bool C_GLBitmapFont::Print( std::string text )
{
   return Print( text.c_str() );
}

bool C_GLBitmapFont::Print( const char* text )
{
   if( text == NULL )             // If There's No Text
      return false;               // Do Nothing
      
   ::glPushAttrib( GL_LIST_BIT );   // Pushes The Display List Bits
   ::glListBase( m_base - 32 );     // Sets The Base Character to 32
   ::glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text ); // Draws The Display List Text
   ::glPopAttrib();                                         // Pops The Display List Bits
   return true;
}

bool C_GLBitmapFont::Print( std::string text, float x, float y, float z )
{
   return Print( text.c_str(), x, y, z ); 
}

bool C_GLBitmapFont::Print( const char* text, float x, float y, float z )
{
   if( text == NULL )             // If There's No Text
      return false;               // Do Nothing
      
   ::glRasterPos3f( x, y, z );
   ::glPushAttrib( GL_LIST_BIT );   // Pushes The Display List Bits
   ::glListBase( m_base - 32 );     // Sets The Base Character to 32
   ::glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text ); // Draws The Display List Text
   ::glPopAttrib();                                         // Pops The Display List Bits
   return true;
}

