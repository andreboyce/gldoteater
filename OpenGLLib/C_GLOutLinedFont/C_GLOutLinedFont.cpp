#include "C_GLOutLinedFont.h"

bool C_GLOutLinedFont::BuildFont( const HWND Hwnd )
{
   SetHWND( Hwnd );
   return BuildFont();
}

bool C_GLOutLinedFont::BuildFont( void )         // Build Our Bitmap Font
{
   if( m_Hdc )
   {
      if( !m_HFont )
      {
         m_HFont.Create();
      }

      m_base = ::glGenLists( characters );     // Storage For 256 Characters
      if( !::glIsList( m_base ) || m_base == 0 )
      {
         Alert( C_GLErrorStrings::GetError() );
         return false;
      }

      ::SelectObject( m_Hdc, m_HFont );        // Selects The Font We Created

      ::wglUseFontOutlines( m_Hdc,             // Select The Current DC
                            0,                 // Starting Character
                            characters-1,      // Number Of Display Lists To Build
                            m_base,            // Starting Display Lists
                            0.0f,              // Deviation From The True Outlines
                            0.2f,              // Font Thickness In The Z Direction
                            WGL_FONT_POLYGONS, // Use Polygons, Not Lines
                            gmf );             // Address Of Buffer To Recieve Data

      return  m_bFontBuilt = true;
   }
   return m_bFontBuilt = false;
}

void C_GLOutLinedFont::Print( const std::string& text, const float x, const float y, const float z, bool PushMatrix )
{
   Print( text.c_str(), x, y, z, PushMatrix );
}

void C_GLOutLinedFont::Print( std::ostringstream text, const float x, const float y, const float z, bool PushMatrix )
{
   Print( text.str(), x, y, z, PushMatrix );
}

void C_GLOutLinedFont::Print( std::ostringstream text, const float x, const float y, const float z, int flags, bool PushMatrix )
{
//         flags
   Print( text.str(), x, y, z, PushMatrix );
}

void C_GLOutLinedFont::Print(  const std::string& text )
{
   return Print( text.c_str() );
}

void C_GLOutLinedFont::Print( const char* text )
{
   if( text != NULL )
   {
      ::glDisable( GL_TEXTURE_2D );
      ::glPushAttrib( GL_LIST_BIT );   // Pushes The Display List Bits
      ::glListBase( m_base );
      ::glCallLists( ::strlen( text ), GL_UNSIGNED_BYTE, text ); // Draws The Display List Text
      ::glPopAttrib();                                         // Pops The Display List Bits
   }
}

void C_GLOutLinedFont::Print( const char* text, const float x, const float y, const float z, bool PushMatrix )
{
   if( text != NULL )
   {
      if( PushMatrix )
      {
         ::glPushMatrix();
         ::glLoadIdentity();
      }

      ::glDisable( GL_TEXTURE_2D );
      ::glTranslatef( x, y, z ); // Center Our Text On The Screen      
      ::glRasterPos3f( x, y, z );
      ::glPushAttrib( GL_LIST_BIT );   // Pushes The Display List Bits
      ::glListBase( m_base );
//      ::glListBase( 257 );
      ::glCallLists( ::strlen( text ), GL_UNSIGNED_BYTE, text ); // Draws The Display List Text
      ::glPopAttrib();                                         // Pops The Display List Bits
      if( PushMatrix )
      {
         ::glPopMatrix();
      }
   }
}

