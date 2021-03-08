#include "C_WinStatic.h"
C_WinStatic::C_WinStatic( void ) :
   C_WinControllBase( WC_STATIC, WS_CHILD | WS_VISIBLE, 
                      0 ),
   m_bresize_for_text(true)
{

}

bool C_WinStatic::Create( HWND HwndParent, DWORD id, std::string title, short x, short y, short width, short height, bool resize_for_text )
{
   m_bresize_for_text = resize_for_text;
   m_HwndParent = HwndParent;
   return (bool)CWinBase::Create( (char*)title.c_str(), HwndParent, x, y, width, height, (HMENU)id, 0 );

//   SetFont( (HFONT) ::GetStockObject( DEFAULT_GUI_FONT ) );
   return true;
}

bool C_WinStatic::SetFont( HFONT hFont )
{
   return ::SendMessage( m_Hwnd, WM_SETFONT, (WPARAM)hFont,
                         MAKELPARAM(FALSE, 0) );
}

bool C_WinStatic::SetText( std::string text )
{
   return ::SetWindowText( m_Hwnd, text.c_str() );
}

std::string C_WinStatic::GetText( void )
{
	std::vector<char> str( ::GetWindowTextLength( m_Hwnd ) );
   ::GetWindowText( m_Hwnd, &str[0], ::GetWindowTextLength( m_Hwnd ) );
   std::string temp = &str[0];
   return temp;
}

bool C_WinStatic::Resize( short width, short height, short x, short y )
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



