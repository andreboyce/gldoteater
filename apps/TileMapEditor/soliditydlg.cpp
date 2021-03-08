#include "main.h"

BOOL CALLBACK MainWindow::SolidityDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
   MainWindow * mw = reinterpret_cast< MainWindow *>( ::GetWindowLong( hwnd, GWL_USERDATA ) );
	switch( Message )
	{
	   case WM_INITDIALOG:
      {
      } break;
      case WM_PAINT:
      {
         if( mw )
         {
            mw->m_SolidityDialogStaticDCs.m_PaintDC.Begin();
            mw->DrawTileOnSolidityDialog( mw->m_SolidityDialogStaticDCs.m_PaintDC );
            mw->m_SolidityDialogStaticDCs.m_PaintDC.End();
         }
      } break;
	   case WM_HSCROLL:
      {
         if( mw )
         {
            if( hwnd = ::GetDlgItem( mw->m_HwndSolidityDlg, ID_SOLIDITY_TRACKBAR_SOLIDITY ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, TBM_GETPOS, 0, 0 );
               std::ostringstream temp;
               temp << dwPos;
               mw->m_Solidity[ mw->m_CurrentMap ] = mw->m_TileSoliditySolidity = dwPos;
               ::SetWindowText( ::GetDlgItem( mw->m_HwndSolidityDlg, ID_SOLIDITY_EDIT_SOLIDITY ), temp.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndSolidityDlg, ID_SOLIDITY_EDIT_SOLIDITY ) );
               if( mw->m_TileMaps.size() )
               {
                  if( mw->m_Solidity[ mw->m_CurrentMap ] > mw->m_TileMaps[ mw->m_CurrentMap ].maxsolidity() )
                  {
                     mw->m_Solidity[ mw->m_CurrentMap ] = mw->m_TileSoliditySolidity = mw->m_TileMaps[ mw->m_CurrentMap ].maxsolidity();
                  }
               }
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndSolidityDlg, ID_SOLIDITY_STATIC ) );
            }
            if( hwnd = ::GetDlgItem( mw->m_HwndSolidityDlg, ID_SOLIDITY_TRACKBAR_TILEID ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, TBM_GETPOS, 0, 0 );
               std::ostringstream temp;
               temp << dwPos;
               mw->m_TileSolidityGraphic = dwPos;
               ::SetWindowText( ::GetDlgItem( mw->m_HwndSolidityDlg, ID_SOLIDITY_EDIT_TILEID ), temp.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndSolidityDlg, ID_SOLIDITY_EDIT_TILEID ) );
               if( mw->m_TileMaps.size() )
               {
                  if( mw->m_TileSolidityGraphic >= mw->m_TileMaps[ mw->m_CurrentMap ].Bitmaps().size() )
                     mw->m_TileSolidityGraphic = 0;
               }
            }
            ::UpdateWindow( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_SOLIDITY_STATIC ) );
            mw->DrawMap( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_MemoryDC );
            mw->DrawAStarPath( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_MemoryDC );
            mw->m_MapsStaticDCs.BltMemoryToGet( mw->m_MapVisableX, mw->m_MapVisableY );
            mw->DrawTileOnSolidityDialog( mw->m_SolidityDialogStaticDCs.m_GetDC );
         }
         switch( LOWORD(wParam) )
         {            
            case TB_ENDTRACK:
            {
            } break; 
            default:
            {
            } break;
         }
      } break;
      case WM_COMMAND:
      {
         switch( LOWORD( wParam ) )
         {
            case ID_SOLIDITY_BUTTON_OK:
            {
               ::EndDialog( hwnd, 0 );
               if( mw )
               {
                  if( mw->m_TileMaps.size() > mw->m_CurrentMap )
                  {
                     mw->m_TileMaps[ mw->m_CurrentMap ].SetSolidityForAllTilesWithThisGraphic( mw->m_TileSolidityGraphic, mw->m_TileSoliditySolidity );
                  }
               }
            } break;
            case ID_SOLIDITY_BUTTON_CANCEL:
            {
               ::EndDialog( hwnd, 0 );
               if( mw )
               {
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

bool MainWindow::ShowSolidityDlg( void )
{
   bool        ret = false;
   m_HwndSolidityDlg = CreateDialog( ::GetModuleHandle( NULL ), 
                                     MAKEINTRESOURCE( ID_SOLIDITY_DIALOG ), 
                                     m_WinBase, 
                                     SolidityDlgProc );

   if( m_HwndSolidityDlg != NULL )
   {
      if( m_TileMaps.size() <= m_CurrentMap )
         return false;
      ::ShowWindow(    m_HwndSolidityDlg, SW_SHOW );
      ::SetWindowLong( m_HwndSolidityDlg, GWL_USERDATA, (LPARAM)this );

      m_SolidityDialogStaticDCs.SetHwnd( ::GetDlgItem( m_HwndSolidityDlg, ID_SOLIDITY_STATIC ) );

      std::ostringstream temp;
      temp << m_Solidity[ m_CurrentMap ];
      ::SetWindowText( ::GetDlgItem( m_HwndSolidityDlg, ID_SOLIDITY_EDIT_SOLIDITY ), temp.str().c_str() );
      ::UpdateWindow( ::GetDlgItem( m_HwndSolidityDlg, ID_SOLIDITY_EDIT_SOLIDITY ) );

      m_TileSolidityGraphic = m_CurrentTileGraphics[ m_CurrentMap ];

      HWND hwndTrackSolidity = ::GetDlgItem( m_HwndSolidityDlg, ID_SOLIDITY_TRACKBAR_SOLIDITY );
      // range
      ::SendMessage( hwndTrackSolidity, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, m_TileMaps[ m_CurrentMap ].maxsolidity() ) );
      // page size
      ::SendMessage( hwndTrackSolidity, TBM_SETPAGESIZE, 0, (LPARAM) (m_TileMaps[ m_CurrentMap ].maxsolidity()/10) );
      // 
      ::SendMessage( hwndTrackSolidity, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0, m_TileMaps[ m_CurrentMap ].maxsolidity() ) );
      // Position
      ::SendMessage( hwndTrackSolidity, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) m_Solidity[ m_CurrentMap ] );

      //--------
      HWND hwndTrackTileID = ::GetDlgItem( m_HwndSolidityDlg, ID_SOLIDITY_TRACKBAR_TILEID );
      // range
      ::SendMessage( hwndTrackTileID, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, m_TileMaps[ m_CurrentMap ].Bitmaps().size()-1 ) );
      // page size
      ::SendMessage( hwndTrackTileID, TBM_SETPAGESIZE, 0, (LPARAM) (m_TileMaps[ m_CurrentMap ].Bitmaps().size()-1/10) );
      // 
      ::SendMessage( hwndTrackTileID, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0, m_TileMaps[ m_CurrentMap ].Bitmaps().size()-1 ) );
      // Position
      ::SendMessage( hwndTrackTileID, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) m_TileSolidityGraphic );

      std::ostringstream temp2;
      temp2 << m_CurrentTileGraphics[ m_CurrentMap ];
      ::SetWindowText( ::GetDlgItem( m_HwndSolidityDlg, ID_SOLIDITY_EDIT_TILEID ), temp2.str().c_str() );
      ::UpdateWindow( ::GetDlgItem( m_HwndSolidityDlg, ID_SOLIDITY_EDIT_TILEID ) );
      DrawTileOnSolidityDialog( m_SolidityDialogStaticDCs.m_GetDC );

      ret = true;
   }
   else
   {
      ::Alert( GetLastSystemError() + " CreateDialog failed" );
   }
   return ret;
}

void MainWindow::DrawTileOnSolidityDialog( C_HDC& hdc )
{
   int x = 0;
   int y = 0;
   if( m_TileMaps.size() )
   {
      if( m_TileMaps[ m_CurrentMap ].Bitmaps().size() )
      {
         m_TileMaps[ m_CurrentMap ].Bitmaps()[ m_TileSolidityGraphic ].StretchDisplay( hdc, x, y, m_TileMaps[ m_CurrentMap ].tilewidth()*4, m_TileMaps[ m_CurrentMap ].tilewidth()*4 );
      }
   }
}

