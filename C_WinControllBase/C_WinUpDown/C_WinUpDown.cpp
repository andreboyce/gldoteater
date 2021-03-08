#include "C_WinUpDown.h"

C_WinUpDown::C_WinUpDown( void ) :
   C_WinControllBase( UPDOWN_CLASS, WS_CHILD | WS_VISIBLE, 0 ),
   m_HwndBuddy( NULL )
{

}

C_WinUpDown::~C_WinUpDown( void )
{
}

bool C_WinUpDown::Create( HWND HwndParent, HWND HwndBuddy, DWORD id, std::string title, short x, short y, short width, short height )
{
   m_HwndParent = HwndParent;
   m_HwndBuddy  = HwndBuddy;
   CWinBase::Create( (char*)title.c_str(), HwndParent, x, y, width, height, (HMENU)id, 0 );
   return true;
}
