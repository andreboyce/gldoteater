#include "C_PaintDC.h"

C_PaintDC::C_PaintDC( void ) : C_HDC( NULL )
{
   ::ZeroMemory( &m_ps, sizeof(PAINTSTRUCT) );
}

C_PaintDC::C_PaintDC( HWND Hwnd ) : C_HDC( NULL )
{
   m_Hwnd = Hwnd;
}

C_PaintDC::~C_PaintDC( void )
{
   if( m_Hdc )
      End();
}

