#include "C_HDC.h"

C_HDC::C_HDC( HDC Hdc ) :
   m_Hdc( Hdc ),
   m_Hwnd( NULL ), 
   m_Hbitmap( NULL ), 
   hBitmapOrignal( NULL ),
   m_BitmapWidth( 0 ), 
   m_BitmapHeight( 0 )
{}

C_HDC::C_HDC( void ) :
   m_Hdc( NULL ),
   m_Hwnd( NULL ), 
   m_Hbitmap( NULL ), 
   hBitmapOrignal( NULL ),
   m_BitmapWidth( 0 ), 
   m_BitmapHeight( 0 )
{}

C_HDC::~C_HDC( void )
{
   if( m_Hbitmap )
   {
      ::DeleteObject( SelectObject( hBitmapOrignal ) );
   }
}

void C_HDC::GetTextSize( int & cxChar, int & cyChar )
{
   TEXTMETRIC tm;
   ::GetTextMetrics( m_Hdc, &tm );
   cxChar = tm.tmAveCharWidth;
   cyChar = tm.tmHeight + tm.tmExternalLeading;
}

void C_HDC::Line( const C_Structs::Point &start, const C_Structs::Point &end )
{
   ::MoveToEx( m_Hdc, start.x, start.y, 0 );
   ::LineTo(   m_Hdc, end.x,   end.y      );
}



