#include "C_WinListBox.h"

C_WinListBox::C_WinListBox( void ) :
   C_WinControllBase( WC_LISTBOX, WS_CHILD | WS_VISIBLE, 
                      0 )
{
}

bool C_WinListBox::Create( HWND HwndParent, DWORD id, std::string title, short x, short y, short width, short height )
{
   m_HwndParent = HwndParent;
   bool ret = CWinBase::Create( (char*)title.c_str(), HwndParent, x, y, width, height, (HMENU)id, 0 );

//   SetFont( (HFONT) ::GetStockObject( DEFAULT_GUI_FONT ) );
   return ret;
}

bool C_WinListBox::SetFont( HFONT hFont )
{
   return ::SendMessage( m_Hwnd, WM_SETFONT, (WPARAM)hFont,
                         MAKELPARAM(FALSE, 0) );
}

