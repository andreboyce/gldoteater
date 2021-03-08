#include "C_MemoryDC.h"

C_MemoryDC::C_MemoryDC( void ) :
   C_HDC( NULL ),
   m_HdcGet( NULL )
{
}

C_MemoryDC::C_MemoryDC( HWND Hwnd ) :
   C_HDC( NULL ),
   m_HdcGet( NULL )
{
   m_Hwnd = Hwnd;
}

C_MemoryDC::~C_MemoryDC( void )
{
   if( m_Hdc )
      ::DeleteDC( m_Hdc );
}

bool C_MemoryDC::CreateBitmap( HWND Hwnd, int width, int height )
{
   if( m_Hdc )
   {
      ::SelectObject( (HDC)m_Hdc, (HBITMAP) hBitmapOrignal );
      ::DeleteDC( m_Hdc );
      ::DeleteObject( m_Hbitmap );
   }
   m_Hwnd   = Hwnd;
   m_HdcGet = ::GetDC( Hwnd );
   m_Hdc                  = ::CreateCompatibleDC( m_HdcGet );
   m_Hbitmap              = ::CreateCompatibleBitmap( m_HdcGet, width, height );
   HBITMAP hcurrentbitmap = (HBITMAP)::SelectObject( (HDC)m_Hdc, (HBITMAP) m_Hbitmap );
   ::DeleteObject( hcurrentbitmap );
   ::ReleaseDC( Hwnd, m_HdcGet );
   m_BitmapWidth          = width;
   m_BitmapHeight         = height;
   return (m_Hbitmap) ? true : false;
}

bool C_MemoryDC::CreateBitmap( HWND Hwnd, Structs::Rect rect )
{
   int width  = rect.right-rect.left;
   int height = rect.bottom-rect.top;
   if( m_Hdc )
   {
      ::SelectObject( (HDC)m_Hdc, (HBITMAP) hBitmapOrignal );
      ::DeleteDC( m_Hdc );
      ::DeleteObject( m_Hbitmap );
   }
   m_Hwnd     = Hwnd;
   m_HdcGet   = ::GetDC( Hwnd );
   m_Hdc                  = ::CreateCompatibleDC( m_HdcGet );
   m_Hbitmap              = ::CreateCompatibleBitmap( m_HdcGet, width, height );
   HBITMAP hcurrentbitmap = (HBITMAP) ::SelectObject( (HDC)m_Hdc, (HBITMAP) m_Hbitmap );
   ::DeleteObject( hcurrentbitmap );
   ::ReleaseDC( Hwnd, m_HdcGet );
   m_BitmapWidth          = width;
   m_BitmapHeight         = height;
   return (m_Hbitmap) ? true : false;
}

