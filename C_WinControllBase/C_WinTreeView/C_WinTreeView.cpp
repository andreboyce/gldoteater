#include "C_WinTreeView.h"

C_WinTreeView::C_WinTreeView( void ) :
    C_WinControllBase( WC_TREEVIEW, WS_CHILD | WS_VISIBLE, 0 )
{
}

C_WinTreeView::~C_WinTreeView( void )
{
}

bool C_WinTreeView::Create( HWND HwndParent, DWORD id, std::string title, short x, short y, short width, short height )
{
  m_HwndParent = HwndParent;
  bool ret = CWinBase::Create( (char*)title.c_str(), HwndParent, x, y, width, height, (HMENU)id, 0 );
  return ret;
}
