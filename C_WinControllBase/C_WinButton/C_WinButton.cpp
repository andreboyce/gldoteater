#include "C_WinButton.h"

C_WinButton::C_WinButton( void ) : 
   C_WinControllBase( "BUTTON", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0 ),
   m_bresize_for_text (false)
{
}

C_WinButton::C_WinButton( const C_WinButton & RHS ) :
   C_WinControllBase( RHS ),
   m_bresize_for_text( RHS.m_bresize_for_text )
{
}

C_WinButton::~C_WinButton( void )
{}

bool C_WinButton::Create( HWND HwndParent, DWORD id, std::string title,
                          short x, short y, short width, short height, bool resize_for_text )
{
   m_bresize_for_text = resize_for_text;
   m_HwndParent = HwndParent;
   return (bool)CWinBase::Create( (char*)title.c_str(), HwndParent, x, y, width, height, (HMENU)id, 0 );
}

std::string C_WinButton::GetText( void )
{
   DWORD dwTextLength = ::GetWindowTextLength( m_Hwnd );
   std::string windowtext;
   if( dwTextLength > 0 )
   {
      LPSTR pszText;
      DWORD dwBufferSize = dwTextLength + 1;
      pszText = (LPSTR) ::GlobalAlloc( GPTR, dwBufferSize );
      if( pszText != NULL )
      {
         if( ::GetWindowText( m_Hwnd, pszText, dwBufferSize ) )
         {
            windowtext = pszText;
         }
      }
      ::GlobalFree( pszText );
   }
   return windowtext;
}

bool C_WinButton::Resize( short width, short height, short x, short y )
{
   if( m_bresize_for_text )
   {
      HDC hdc = ::GetDC( m_Hwnd );
      SIZE size;
      std::string temp = GetText();
      ::GetTextExtentPoint32( hdc, (LPCTSTR)temp.c_str(), (int)temp.size(), 
                              &size );
      ::ReleaseDC( m_Hwnd, hdc );
      return ::SetWindowPos( m_Hwnd, NULL, x, y, 
                            ( width < size.cx ) ? size.cx : width, 
                            ( height < size.cy ) ? size.cy : height, 
                            SWP_NOZORDER );
   }
   else
   {
      return ::SetWindowPos( m_Hwnd, NULL, x, y, width, height, SWP_NOZORDER );
   }
}

