#include "C_WinToolTip.h"

C_WinToolTip::C_WinToolTip( void ) : 
   C_WinControllBase( TOOLTIPS_CLASS, WS_VISIBLE | WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_BALLOON,
                      0 )
{
   ::ZeroMemory( &m_ti, sizeof( TOOLINFO ) );
   m_rect.left   = 0;
   m_rect.right  = 500;
   m_rect.top    = 0;
   m_rect.bottom = 500;
}

C_WinToolTip::~C_WinToolTip( void )
{
}

bool C_WinToolTip::Create( HWND HwndParent, DWORD id, Structs::Rect rect, std::string text, std::string title, short x, short y, short width, short height )
{
   m_Text       = text;
   m_HwndParent = HwndParent;
   bool ret = CWinBase::Create( (char*)title.c_str(), HwndParent, x, y, width, height, (HMENU)id, 0 );

   if( !m_Hwnd )
      Alert( GetLastSystemError() );

   ::SetWindowPos( m_Hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );

   m_ti.cbSize   = sizeof( TOOLINFO );
   m_ti.uFlags   = 0;
   m_ti.hwnd     = HwndParent;
   m_ti.uId      = id;
   std::memcpy( &m_ti.rect, &m_rect, sizeof(RECT) );
   m_ti.hinst    = ::GetModuleHandle( NULL );
   m_ti.lpszText = (char*)m_Text.c_str();

   if( !::SendMessage( m_Hwnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&m_ti ) )
   {
      Alert( GetLastSystemError() );
   }
   ::SendMessage( m_Hwnd, TTM_ACTIVATE, true, 0 );
   return ret;
}

void C_WinToolTip::SetRect( Structs::Rect rect )
{
   m_rect = rect;
   m_ti.cbSize   = sizeof( TOOLINFO );
   m_ti.uFlags   = 0;
//   m_ti.hwnd     = m_HwndParent;
//   m_ti.uId      = 0;
   std::memcpy( &m_ti.rect, &m_rect, sizeof(RECT) );
//   m_ti.hinst    = ::GetModuleHandle( NULL );
//   m_ti.lpszText = (char*)m_Text.c_str();

   ::SendMessage( m_Hwnd, TTM_NEWTOOLRECT, 0, (LPARAM)(LPTOOLINFO)&m_ti );
}


bool C_WinToolTip::Activate( int x, int y )
{
	return false;
}

