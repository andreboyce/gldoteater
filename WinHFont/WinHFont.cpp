#include "WinHFont.h"

WinHFont::WinHFont( void ) :
   m_HFont( NULL )
{
}

WinHFont::~WinHFont( void )
{
   Destroy();
}

void WinHFont::Destroy( void )
{
   if( m_HFont )
   {
      ::DeleteObject( m_HFont );
   }
}

void WinHFont::Create( const S_FontProperties & FontProperties )
{
   m_FontProperties = FontProperties;
   Create();
}

void WinHFont::Create( void )
{
   Destroy();
   m_HFont = CreateFont(
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
                           m_FontProperties.lpszFace
                       );	
 
}

void WinHFont::ReCreate( void )
{
   Destroy();
   Create();
}
