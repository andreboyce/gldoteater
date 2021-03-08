#include "C_WinTab.h"

C_WinTab::C_WinTab( void ) :
   C_WinControllBase( WC_TABCONTROL, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0 )
{

}

C_WinTab::~C_WinTab( void )
{
   Free();
}

bool C_WinTab::Create( HWND HwndParent, DWORD id, V_String Strings, std::string title, short x, short y, short width, short height )
{
   m_HwndParent = HwndParent;
   TCITEM tie;
   m_Strings = Strings;

   CWinBase::Create( (char*)title.c_str(), HwndParent, x, y, width, height, (HMENU)id, 0 );

   tie.mask = TCIF_TEXT | TCIF_IMAGE;
   tie.iImage = -1;
   
   for( int i=0; i<m_Strings.size() ; i++ )
   {
      tie.pszText = const_cast<char*>(m_Strings.at(i).c_str());
      TabCtrl_InsertItem( m_Hwnd, i, &tie );
   }
   return (bool)m_Hwnd;
}

void C_WinTab::Free( void )
{

}

bool C_WinTab::RenameTab( std::string text, int index )
{
   TCITEM item;
   ::ZeroMemory( &item, sizeof(item) );
   item.mask    = TCIF_TEXT;
   item.pszText = (char*)text.c_str();
   return (bool) TabCtrl_SetItem( m_Hwnd, index, &item );
}

bool C_WinTab::AddTab( std::string text, int index )
{
   TCITEM item;
   ::ZeroMemory( &item, sizeof(item) );
   item.mask    = TCIF_TEXT;
   item.pszText = (char*)text.c_str();
   if( TabCtrl_InsertItem( m_Hwnd, index, &item ) == -1 )
      return false;
   return true;
}

int C_WinTab::NumberOfTabs( void )
{
   return TabCtrl_GetItemCount( m_Hwnd );
}

bool C_WinTab::RemoveTab( int index )
{
   return (bool) TabCtrl_DeleteItem( m_Hwnd, index );
}

void C_WinTab::SelectTab( int index )
{
   TabCtrl_SetCurFocus( m_Hwnd, index );
}

