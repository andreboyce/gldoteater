#include "C_WinToolBar.h"

C_WinToolBar::C_WinToolBar( void ):
   C_WinControllBase( TOOLBARCLASSNAME, WS_CHILD | WS_VISIBLE | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS )
{
}

C_WinToolBar::~C_WinToolBar( void )
{
}

bool C_WinToolBar::Create( HWND HwndParent, DWORD id, L_TBItems TBBStrings, short NumberOfButtons, std::string title, short x, short y, short width, short height )
{
   m_HwndParent = HwndParent;
   bool ret = CWinBase::Create( (char*)title.c_str(), HwndParent, x, y, width, height, (HMENU)id, 0 );

   TBADDBITMAP tbab;
   std::vector<TBBUTTON>    tbb(NumberOfButtons);
   UINT        i = 0;
   ::SendMessage( m_Hwnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 
                  0 );

   // Add the bitmap containing button images to the toolbar.
   tbab.hInst = HINST_COMMCTRL;
   tbab.nID   = IDB_STD_SMALL_COLOR;

   ::SendMessage( m_Hwnd, TB_ADDBITMAP, (WPARAM)NumberOfButtons,
                  (LPARAM)&tbab );

   // clean memory before using it
   ::ZeroMemory( &tbb[0], sizeof(tbb) );

   m_ToolBarItems = TBBStrings;

   for( L_TBItems::iterator i=m_ToolBarItems.begin() ; i!=m_ToolBarItems.end() ; i++ )
   {
      i->id = ::SendMessage( m_Hwnd, TB_ADDSTRING, 0, (LPARAM)(LPSTR)i->text.c_str() );
   }

   L_TBItems::iterator c = m_ToolBarItems.begin();

   // Button "New"
   tbb[i].iBitmap   = STD_FILENEW;
   tbb[i].idCommand = IDM_NEW;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString   = c->id;
   i++; c++;

   // Button "Open"
   tbb[i].iBitmap   = STD_FILEOPEN;
   tbb[i].idCommand = IDM_OPEN;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString   = c->id;
   i++; c++;


   tbb[i].iBitmap   = STD_FILESAVE;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   tbb[i].idCommand = IDM_SAVE;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString   = c->id;
   i++; c++;


   // Button "Find"
   tbb[i].iBitmap   = STD_FIND;
   tbb[i].idCommand = IDM_FIND;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString   = c->id;
   i++; c++;


   // Button "Copy"
   tbb[i].iBitmap   = STD_COPY;
   tbb[i].idCommand = IDM_EDITCOPY;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString   = c->id;
   i++; c++;


   // Button "Cut"
   tbb[i].iBitmap   = STD_CUT;
   tbb[i].idCommand = IDM_EDITCUT;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString   = c->id;
   i++; c++;


   // Button "Paste"
   tbb[i].iBitmap   = STD_PASTE;
   tbb[i].idCommand = IDM_EDITPASTE;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString   = c->id;
   i++; c++;


   // Button "Undo"
   tbb[i].iBitmap   = STD_UNDO;
   tbb[i].idCommand = IDM_EDITUNDO;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString   = c->id;
   i++; c++;


   // Button "Redo"
   tbb[i].iBitmap   = STD_REDOW;
   tbb[i].idCommand = IDM_EDITREDO;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString   = c->id;
   i++; c++;


   // Button "Replace"
   tbb[i].iBitmap   = STD_REPLACE;
   tbb[i].idCommand = IDM_EDITREPLACE;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString   = c->id;
   i++; c++;

   // Button "Delete"
   tbb[i].iBitmap   = STD_DELETE;
   tbb[i].idCommand = IDM_EDITDELETE;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString   = c->id;
   i++; c++;

   // Button "Properties"
   tbb[i].iBitmap   = STD_PROPERTIES;
   tbb[i].idCommand = IDM_PROPERTIES;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString = c->id;
   i++; c++;

   // Button "Help"
   tbb[i].iBitmap   = STD_HELP;
   tbb[i].idCommand = IDM_HELP;
   tbb[i].fsState   = TBSTATE_ENABLED;
   tbb[i].fsStyle   = TBSTYLE_BUTTON;
   if( c != m_ToolBarItems.end() )
      tbb[i].iString   = c->id;
   i++; c++;

   ::SendMessage( m_Hwnd, TB_ADDBUTTONS, NumberOfButtons,
                  (LPARAM)&tbb[0] );
   ::SendMessage( m_Hwnd, TB_AUTOSIZE, 0, 0);
   return ret;
}

bool C_WinToolBar::AddButtons( L_TBItems ToolBarItems )
{
	std::vector<TBBUTTON> tbb( ToolBarItems.size() );
   ::ZeroMemory( &tbb[0], sizeof(tbb) );
   return ::SendMessage( m_Hwnd, TB_ADDBUTTONS, ToolBarItems.size(),
                        (LPARAM)&tbb[0] );
}

void C_WinToolBar::GetToolTipText( LPARAM &lParam )
{
   LPTOOLTIPTEXT lpttt;
   lpttt = (LPTOOLTIPTEXT) lParam;
   lpttt->hinst = ::GetModuleHandle( NULL );

   // Specify the resource identifier of the descriptive 
   // text for the given button. 
   int idButton = lpttt->hdr.idFrom;
   switch (idButton)
   {
      case IDM_EDITREDO:
      {
         lpttt->lpszText = "REDO";
      } break;
      case IDM_EDITREPLACE:
      {
         lpttt->lpszText = "REPLACE";
      } break;
      case IDM_HELP:
      {
         lpttt->lpszText = "HELP";
      } break;
      case IDM_PROPERTIES:
      {
         lpttt->lpszText = "PROPERTIES";
      } break;
      case IDM_EDITDELETE:
      {
         lpttt->lpszText = "DELETE";
      } break;
      case IDM_EDITUNDO:
      {
         lpttt->lpszText = "UNDO";
      } break;
      case IDM_NEW:
      {
         lpttt->lpszText = "NEW";
      } break;
      case IDM_OPEN:
      {
         lpttt->lpszText = "OPEN";
      } break;
      case IDM_SAVE:
      {
         lpttt->lpszText = "SAVE";
      } break;
      case IDM_FIND:
      {
         lpttt->lpszText = "FIND";
      } break;
      case IDM_EDITCUT:
      {
         lpttt->lpszText = "CUT";
      } break;
      case IDM_EDITCOPY:
      {
         lpttt->lpszText = "COPY";
      } break;
      case IDM_EDITPASTE:
      {
         lpttt->lpszText = "PASTE";
      } break;
   }
}
