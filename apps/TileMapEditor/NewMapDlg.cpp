#include "main.h"

BOOL CALLBACK MainWindow::NewMapDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
   MainWindow * mw = reinterpret_cast< MainWindow *>( ::GetWindowLong( hwnd, GWL_USERDATA ) );
	switch( Message )
	{
	   case WM_INITDIALOG:
      {
      } break;
	   case WM_VSCROLL:
      {
         if( mw )
         {
            if( hwnd = ::GetDlgItem( mw->m_HwndNewMapDlg, ID_NEWMAP_DIALOG_UPWOWN_WIDTH ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, UDM_GETPOS, 0, 0 );
               std::ostringstream width;
               width << dwPos;
               ::SetWindowText( ::GetDlgItem( mw->m_HwndNewMapDlg, ID_NEWMAP_DIALOG_UPWOWN_WIDTH ), width.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndNewMapDlg, ID_NEWMAP_DIALOG_UPWOWN_WIDTH ) );
            }
            if( hwnd = ::GetDlgItem( mw->m_HwndNewMapDlg, ID_NEWMAP_DIALOG_UPWOWN_HEIGHT ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, UDM_GETPOS, 0, 0 );
               std::ostringstream height;
               height << dwPos;
               ::SetWindowText( ::GetDlgItem( mw->m_HwndNewMapDlg, ID_NEWMAP_DIALOG_UPWOWN_HEIGHT ), height.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndNewMapDlg, ID_NEWMAP_DIALOG_UPWOWN_HEIGHT ) );
            }
         }
      } break;

      case WM_COMMAND:
      {
         switch( LOWORD( wParam ) )
         {
            case ID_NEWMAP_DIALOG_BUTTON_OK:
            {
               if( mw )
               {
                  std::string width   = GetDlgItemText( hwnd, ID_NEWMAP_DIALOG_EDIT_WIDTH   );
                  std::string height  = GetDlgItemText( hwnd, ID_NEWMAP_DIALOG_EDIT_HEIGHT  );
                  std::string tileset = GetDlgItemText( hwnd, ID_NEWMAP_DIALOG_EDIT_TILESET );
                  if( ( atoi( width.c_str() ) <= 0 ) || ( atoi( height.c_str() ) <= 0 ) )
                     return -1;

                  try
                  {
                     TileMapLib::C_TileMap map;
                     mw->m_TileMaps.push_back( map );
//                     m_TileMaps.push_back( TileMap() ) ;
                  }
                  catch( ... )
                  {
                     ::EndDialog( hwnd, 0 );
                     Alert( "Could not make a new map." );
                     break;
                  }
                  int index = mw->m_TileMaps.size()-1;
                  if( index < 0 )
                     index = 0;
                  mw->m_TileMaps[ index ].MakeNewMap( atoi( width.c_str() ), atoi( height.c_str() ) );
                  mw->m_TileMaps[ index ].SetMapFileName( "Untitled.tm" );
                  if( tileset.size() )
                  {
                     mw->m_TileMaps[ index ].LoadTileSet( tileset );
                     mw->m_TileMaps[ index ].SetTileSetFileName( tileset );
                  }
                  else
                  {
                     mw->m_TileMaps[ index ].SetTileSetFileName( "Untitled.ts" );
                  }
                  mw->MakeNewTab();
                  ::EndDialog( hwnd, 0 );
                  Alert( "To Add New Tiles Click File->Open Tile" );
               }
            } break;
            case ID_NEWMAP_DIALOG_BUTTON_CANCEL:
            {
               ::EndDialog( hwnd, 0 );
               if( mw )
               {
               }
            } break;
            case ID_NEWMAP_DIALOG_BUTTON_BROWSE:
            {
               if( mw )
               {
                  std::string tilesetfile;
                  mw->m_WinFileHandler.OpenFile( tilesetfile, "Tile Set Files *.ts\0*.ts\0All files *.*\0*.*\0\0",
                                                 ".ts", 0, 0, "Open" );;
                  if( tilesetfile.size() )
                  {
                     ::SetWindowText( ::GetDlgItem( mw->m_HwndNewMapDlg, ID_NEWMAP_DIALOG_EDIT_TILESET  ), tilesetfile.c_str() );
                  }
               }
            } break;
         }
         default:
         {
            return FALSE;
         }
      }
   }
   return TRUE;
}

bool MainWindow::ShowNewMapDlg( void )
{
   bool        ret = false;
   m_HwndNewMapDlg = CreateDialog( ::GetModuleHandle( NULL ), 
                                   MAKEINTRESOURCE( ID_NEWMAP_DIALOG ), 
                                   m_WinBase, 
                                   NewMapDlgProc );

   if( m_HwndNewMapDlg != NULL )
   {
      ::ShowWindow(    m_HwndNewMapDlg, SW_SHOW );
      ::SetWindowLong( m_HwndNewMapDlg, GWL_USERDATA, (LPARAM)this );

      short max_chars = 3;

      std::ostringstream height;
      height << 22;
      ::SendMessage( ::GetDlgItem( m_HwndNewMapDlg, ID_NEWMAP_DIALOG_EDIT_HEIGHT ), EM_LIMITTEXT, max_chars, 0 );
      ::SetWindowText( ::GetDlgItem( m_HwndNewMapDlg, ID_NEWMAP_DIALOG_EDIT_HEIGHT ), height.str().c_str() );

      std::ostringstream width;
      width  << 22;
      ::SetWindowText( ::GetDlgItem( m_HwndNewMapDlg, ID_NEWMAP_DIALOG_EDIT_WIDTH  ), width.str().c_str()  );
      ::SendMessage( ::GetDlgItem( m_HwndNewMapDlg, ID_NEWMAP_DIALOG_EDIT_WIDTH ), EM_LIMITTEXT, max_chars, 0 );

      ::SendMessage(   ::GetDlgItem( m_HwndNewMapDlg, ID_NEWMAP_DIALOG_EDIT_TILESET ), EM_LIMITTEXT, MAX_PATH, 0 );
      if( m_TileMaps.size() > m_CurrentMap )
         ::SetWindowText( ::GetDlgItem( m_HwndNewMapDlg, ID_NEWMAP_DIALOG_EDIT_TILESET  ), m_TileMaps[ m_CurrentMap ].GetTileSetFileName().c_str()  );

      // updown height
      ::SendMessage( ::GetDlgItem( m_HwndNewMapDlg, ID_NEWMAP_DIALOG_UPWOWN_WIDTH ), UDM_SETRANGE, 0, (LPARAM) MAKELONG((short) pow( (float)10, (int)max_chars )-1, (short) 0) );
      ::SendMessage( ::GetDlgItem( m_HwndNewMapDlg, ID_NEWMAP_DIALOG_UPWOWN_WIDTH ), UDM_SETBUDDY, (WPARAM) ::GetDlgItem( m_HwndNewMapDlg, ID_NEWMAP_DIALOG_EDIT_WIDTH ), 0 );

      // updown width
      ::SendMessage( ::GetDlgItem( m_HwndNewMapDlg, ID_NEWMAP_DIALOG_UPWOWN_HEIGHT ), UDM_SETRANGE, 0, (LPARAM) MAKELONG((short) pow( (float)10, (int)max_chars )-1, (short) 0) );
      ::SendMessage( ::GetDlgItem( m_HwndNewMapDlg, ID_NEWMAP_DIALOG_UPWOWN_HEIGHT ), UDM_SETBUDDY, (WPARAM) ::GetDlgItem( m_HwndNewMapDlg, ID_NEWMAP_DIALOG_EDIT_HEIGHT ), 0 );

      ret = true;
   }
   else
   {
      ::Alert( GetLastSystemError() + " CreateDialog failed" );
   }
   return ret;
}

