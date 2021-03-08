#include "main.h"

BOOL CALLBACK MainWindow::ResizeMapDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
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
            if( hwnd = ::GetDlgItem( mw->m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_UPDOWN_WIDTH ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, UDM_GETPOS, 0, 0 );
               std::ostringstream width;
               width << dwPos;
               ::SetWindowText( ::GetDlgItem( mw->m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_UPDOWN_WIDTH ), width.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_UPDOWN_WIDTH ) );
            }
            if( hwnd = ::GetDlgItem( mw->m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_UPDOWN_HEIGHT ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, UDM_GETPOS, 0, 0 );
               std::ostringstream height;
               height << dwPos;
               ::SetWindowText( ::GetDlgItem( mw->m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_UPDOWN_HEIGHT ), height.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_UPDOWN_HEIGHT ) );
            }
         }
      } break;
	   case WM_KEYDOWN:
      {
         ::EndDialog( hwnd, 0 );
      } break;
      case WM_COMMAND:
      {
         switch( LOWORD( wParam ) )
         {
            case ID_RESIZEMAP_DIALOG_BUTTON_OK:
            {
               if( mw )
               {
                  std::string width  = GetDlgItemText( hwnd, ID_RESIZEMAP_DIALOG_EDIT_WIDTH   );
                  std::string height = GetDlgItemText( hwnd, ID_RESIZEMAP_DIALOG_EDIT_HEIGHT  );
                  mw->m_TileMaps[ mw->m_CurrentMap ].ResizeMap( atoi( width.c_str() ) , atoi( height.c_str() ) );
                  mw->ResetScrollBars();
                  ::EndDialog( hwnd, 0 );
                  mw->DrawMap( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                  mw->DrawAStarPath( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                  mw->DrawTileSet( mw->m_CurrentMap, mw->m_TileSetsStaticDCs.m_GetDC );
               }
            } break;
            case ID_RESIZEMAP_DIALOG_BUTTON_CANCEL:
            {
               ::EndDialog( hwnd, 0 );
               if( mw )
               {
                  mw->DrawMap( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                  mw->DrawAStarPath( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                  mw->DrawTileSet( mw->m_CurrentMap, mw->m_TileSetsStaticDCs.m_GetDC );
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

bool MainWindow::ShowResizeMapDlg( void )
{
   if( m_TileMaps.size() == 0 )
   {
      Alert( "No Map To Resize" );
      return true;
   }
   if( m_TileMaps.size() <= m_CurrentMap )
      return true;
   bool        ret = false;
   m_HwndResizeMapDlg = CreateDialog( ::GetModuleHandle( NULL ), 
                                   MAKEINTRESOURCE( ID_RESIZEMAP_DIALOG ), 
                                   m_WinBase, 
                                   ResizeMapDlgProc );

   if( m_HwndResizeMapDlg != NULL )
   {
      ::ShowWindow(    m_HwndResizeMapDlg, SW_SHOW );
      ::SetWindowLong( m_HwndResizeMapDlg, GWL_USERDATA, (LPARAM)this );

      short max_chars = 3;

      std::ostringstream height;
      height << m_TileMaps[ m_CurrentMap ].height();
      ::SetWindowText( ::GetDlgItem( m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_EDIT_HEIGHT ), height.str().c_str() );

      std::ostringstream width;
      width  << m_TileMaps[ m_CurrentMap ].width();
      ::SetWindowText( ::GetDlgItem( m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_EDIT_WIDTH  ), width.str().c_str()  );

      // updown height
      ::SendMessage( ::GetDlgItem( m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_UPDOWN_WIDTH ), UDM_SETRANGE, 0, (LPARAM) MAKELONG((short) pow( (float)10, (int)max_chars )-1, (short) 0) );
      ::SendMessage( ::GetDlgItem( m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_UPDOWN_WIDTH ), UDM_SETBUDDY, (WPARAM) ::GetDlgItem( m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_EDIT_WIDTH ), 0 );

      // updown width
      ::SendMessage( ::GetDlgItem( m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_UPDOWN_HEIGHT ), UDM_SETRANGE, 0, (LPARAM) MAKELONG((short) pow( (float)10, (int)max_chars )-1, (short) 0) );
      ::SendMessage( ::GetDlgItem( m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_UPDOWN_HEIGHT ), UDM_SETBUDDY, (WPARAM) ::GetDlgItem( m_HwndResizeMapDlg, ID_RESIZEMAP_DIALOG_EDIT_HEIGHT ), 0 );

      ret = true;
   }
   else
   {
      ::Alert( GetLastSystemError() + " CreateDialog failed" );
   }
   return ret;
}

