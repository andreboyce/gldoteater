#include "main.h"

bool MainWindow::AddSpecialDataType( std::string name )
{
   for( int i=0; i<m_TileMap.m_SpecialDataTypes.size() ; i++ )
   {
      if( m_TileMap.m_SpecialDataTypes[i].name == name )
      {
          return false;
      }
   }
   m_TileMap.m_SpecialDataTypes.push_back( TileMapSpecialDataTypes( m_TileMap.m_SpecialDataTypes.size(), name ) );
   return true;
}


BOOL CALLBACK MainWindow::SpecialDataTypesDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
   MainWindow * mw = reinterpret_cast< MainWindow *>( ::GetWindowLong( hwnd, GWL_USERDATA ) );
   switch( Message )
   {
      case WM_INITDIALOG:
      {
      } break;
      case WM_CLOSE:
      {
         ::EndDialog( hwnd, 0 );
         if( mw )
         {
            mw->m_HwndShowSpecialDataTypesDialog = NULL;
         }
      } break;
      case WM_COMMAND:
      {
         switch( LOWORD( wParam ) )
         {
            case ID_SDTS_DLG_BUTTON_OK:
            {
               ::EndDialog( hwnd, 0 );
               if( mw )
               {
                  mw->m_HwndShowSpecialDataTypesDialog = NULL;
               }
            } break;
            case ID_SDTS_DLG_BUTTON_REMOVE:
            {
               if( mw )
               {
                  int index = ::SendMessage( ::GetDlgItem( mw->m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_LISTBOX ), 
                                             LB_GETCURSEL, 0, 0 );
                  ::SendMessage( ::GetDlgItem( mw->m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_LISTBOX ),
                                 LB_DELETESTRING,
                                 (WPARAM)index,
                                 (LPARAM) 0 );
                  if( mw->m_TileMap.m_SpecialDataTypes.size() > index )
                  {
                     mw->m_TileMap.m_SpecialDataTypes.erase( mw->m_TileMap.m_SpecialDataTypes.begin() + index );
                     mw->m_SelectedDataType = index;
                  }
                  if( !mw->m_TileMap.m_SpecialDataTypes.size() )
                  {
                     ::SetWindowText( ::GetDlgItem( mw->m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_EDIT_EDIT ), "" );
                     mw->m_SelectedDataType = 0;
                  }
               }
            } break;
            case ID_SDTS_DLG_BUTTON_ADD:
            {
               if( mw )
               {
                   ::SendMessage( ::GetDlgItem( mw->m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_LISTBOX ), 
                                  LB_ADDSTRING, 0, 
                                  (LPARAM) GetWindowText( GetDlgItem( mw->m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_EDIT_ADD ) ).c_str() );
                   mw->AddSpecialDataType( GetWindowText( GetDlgItem( mw->m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_EDIT_ADD ) ) );
                   mw->m_SelectedDataType = 0;
               }
            } break;
            case ID_SDTS_DLG_BUTTON_EDIT:
            {
               if( mw )
               {
                  // Not implemented
                  // delete
                  std::string text = GetWindowText( GetDlgItem( mw->m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_EDIT_EDIT ) );
                  int index = ::SendMessage( ::GetDlgItem( mw->m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_LISTBOX ), 
                                             LB_GETCURSEL, 0, 0 );
                  ::SendMessage( ::GetDlgItem( mw->m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_LISTBOX ),
                                 LB_DELETESTRING,
                                 (WPARAM)index,
                                 (LPARAM) 0 );
                  //add
                  ::SendMessage( ::GetDlgItem( mw->m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_LISTBOX ), 
                                 LB_INSERTSTRING, index, 
                                 (LPARAM) text.c_str() );
                  ::SendMessage( ::GetDlgItem(  mw->m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_LISTBOX ), LB_SETCURSEL, 0, 0 );
                  mw->m_TileMap.m_SpecialDataTypes[index].name = text;
                  mw->m_SelectedDataType = index;
               }
            } break;
         }
      } break;
      default:
      {
         return FALSE;
      }
   }
   return TRUE;
}

bool MainWindow::ShowSpecialDataTypesDialog( void )
{
   if( m_HwndShowSpecialDataTypesDialog != NULL )
      return false;
   bool ret = false;
   m_HwndShowSpecialDataTypesDialog = CreateDialog( ::GetModuleHandle( NULL ), 
                                                     MAKEINTRESOURCE( ID_SDTS_DLG ), 
                                                     m_WinBase, 
                                                     SpecialDataTypesDlgProc );

   if(  m_HwndShowSpecialDataTypesDialog != NULL )
   {
      ::ShowWindow(     m_HwndShowSpecialDataTypesDialog, SW_SHOW );
      ::SetWindowLong(  m_HwndShowSpecialDataTypesDialog, GWL_USERDATA, (LPARAM)this );

      // ListBox
      for( int i=0; i<m_TileMap.m_SpecialDataTypes.size() ; i++ )
      {
         ::SendMessage( ::GetDlgItem( m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_LISTBOX ), 
                        LB_INSERTSTRING, i, 
                        (LPARAM) m_TileMap.m_SpecialDataTypes[i].name.c_str() );
      }
      ::SendMessage( ::GetDlgItem(  m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_LISTBOX ), LB_SETCURSEL, 0, 0 );
      m_SelectedDataType = 0;
      // End ListBox

      std::ostringstream text;
      if( m_TileMap.m_SpecialDataTypes.size() )
         text << m_TileMap.m_SpecialDataTypes[0].name.c_str();
      int max_chars = 500;
      ::SendMessage( ::GetDlgItem(  m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_EDIT_EDIT ), EM_LIMITTEXT, max_chars, 0 );
      ::SetWindowText( ::GetDlgItem(  m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_EDIT_EDIT ), text.str().c_str() );

      ret = true;
   }
   else
   {
      ::Alert( GetLastSystemError() + " CreateDialog failed" );
   }
   return ret;
}
