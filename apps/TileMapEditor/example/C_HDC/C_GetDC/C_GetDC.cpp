#include "C_GetDC.h"

C_GetDC::C_GetDC( void ) : C_HDC( NULL )
{
}

C_GetDC::C_GetDC( HWND Hwnd ) : C_HDC( ::GetDC( Hwnd ) )
{
   m_Hwnd = Hwnd;
}

C_GetDC::~C_GetDC( void )
{
   if( m_Hdc )
   {
      ::ReleaseDC( m_Hwnd, m_Hdc );
   }
}

