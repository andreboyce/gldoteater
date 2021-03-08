#include "C_WinScrollBar.h"

C_WinScrollBar::C_WinScrollBar( void ) :
   C_WinControllBase( "SCROLLBAR", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS )
{
}

C_WinScrollBar::~C_WinScrollBar( void )
{
}

bool C_WinScrollBar::Create( HWND HwndParent, DWORD id, std::string title, short x, short y, short width, short height, bool bIsVertical )
{
   if( bIsVertical )
   {
      m_dwStyle |= SBS_VERT;
   }
   else
   {
      m_dwStyle |= SBS_HORZ;
   }
   
   m_HwndParent = HwndParent;
   return (bool)CWinBase::Create( (char*)title.c_str(), HwndParent, x, y, width, height, (HMENU)id, 0 );
   return true;
}

