#include "main.h"

BOOL CALLBACK MainWindow::TilePropertiesDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
   MainWindow * mw = reinterpret_cast< MainWindow *>( ::GetWindowLong( hwnd, GWL_USERDATA ) );
	switch( Message )
	{
	   case WM_INITDIALOG:
      {
      } break;
	   case WM_HSCROLL:
      {
         if( mw )
         {
            if( hwnd = ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_TRACKBAR_SOLIDITY ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, TBM_GETPOS, 0, 0 );
               std::ostringstream temp;
               temp << dwPos;
               mw->m_TilePropertiesSolidity = dwPos;
               ::SetWindowText( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_SOLIDITY ), temp.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_SOLIDITY ) );
               if( mw->m_TileMaps.size() )
               {
                  if( mw->m_TilePropertiesSolidity >= mw->m_TileMaps[ mw->m_CurrentMap ].maxsolidity() )
                     mw->m_TilePropertiesGraphic = 0;
               }
            }
            if( hwnd = ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_TRACKBAR_TILEID ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, TBM_GETPOS, 0, 0 );
               std::ostringstream temp;
               temp << dwPos;
               mw->m_TilePropertiesGraphic = dwPos;
               ::SetWindowText( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_TILEID ), temp.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_TILEID ) );
               if( mw->m_TileMaps.size() )
               {
                  if( mw->m_TilePropertiesGraphic >= mw->m_TileMaps[ mw->m_CurrentMap ].Bitmaps().size() )
                     mw->m_TilePropertiesGraphic = 0;
               }
            }
            ::UpdateWindow( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_STATIC ) );

            ((mw->m_TileMaps)[ mw->m_CurrentMap])[mw->m_TilePropertiesTile.y][mw->m_TilePropertiesTile.x].m_TileGraphic = mw->m_TilePropertiesGraphic;
            mw->DrawMap( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_MemoryDC );
            mw->DrawAStarPath( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_MemoryDC );
            mw->m_MapsStaticDCs.BltMemoryToGet( mw->m_MapVisableX, mw->m_MapVisableY );
            mw->DrawTileOnPropertiesDialog( mw->m_PropertiesDialogStaticDCs.m_GetDC );            
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
	   case WM_KEYDOWN:
      {
         if( wParam == VK_RETURN )
         {
         }
      } break;
	   case WM_PAINT:
      {
         if( mw )
         {
             mw->m_PropertiesDialogStaticDCs.m_PaintDC.Begin();
             mw->DrawTileOnPropertiesDialog( mw->m_PropertiesDialogStaticDCs.m_PaintDC );
             mw->m_PropertiesDialogStaticDCs.m_PaintDC.End();
         }
      } break;
      case WM_COMMAND:
      {
         switch( LOWORD( wParam ) )
         {
            case ID_TILEPROPERTIES_DIALOG_BUTTON_OK:
            {
               if( mw )
               {
                  ((mw->m_TileMaps)[ mw->m_CurrentMap])[mw->m_TilePropertiesTile.y][mw->m_TilePropertiesTile.x].m_TileGraphic = mw->m_TilePropertiesGraphic;
                  ((mw->m_TileMaps)[ mw->m_CurrentMap])[mw->m_TilePropertiesTile.y][mw->m_TilePropertiesTile.x].m_Solidity    = mw->m_TilePropertiesSolidity;
                  ::UpdateWindow( *mw );
               }
               ::EndDialog( hwnd, 0 );
            } break;
            case ID_TILEPROPERTIES_DIALOG_BUTTON_CANCEL:
            {
               if( mw )
               {
                  ((mw->m_TileMaps)[ mw->m_CurrentMap])[mw->m_TilePropertiesTile.y][mw->m_TilePropertiesTile.x].m_TileGraphic = mw->m_TilePropertiesGraphicOrignal;
                  ((mw->m_TileMaps)[ mw->m_CurrentMap])[mw->m_TilePropertiesTile.y][mw->m_TilePropertiesTile.x].m_Solidity    = mw->m_TilePropertiesSolidityOrignal;
                  ::UpdateWindow( *mw );
               }
               ::EndDialog( hwnd, 0 );
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

bool MainWindow::ShowTilePropertiesDlg( void )
{
   m_TilePropertiesTile       = GetMapTile( m_RButtonDownStartPoint );
   if( m_TilePropertiesTile < 0 )
   {
      return true;
   }

   bool        ret = false;
   if( m_TileMaps.size()  )
   {
      Structs::Point starttile = m_TileMaps[ m_CurrentMap ].GetStartDrawTile();
      m_TilePropertiesTile      += starttile;
      m_TilePropertiesGraphicOrignal  = (m_TileMaps[ m_CurrentMap ])[ m_TilePropertiesTile.y ][ m_TilePropertiesTile.x ].m_TileGraphic;
      m_TilePropertiesSolidityOrignal = (m_TileMaps[ m_CurrentMap ])[ m_TilePropertiesTile.y ][ m_TilePropertiesTile.x ].m_Solidity;

      m_HwndTilePropertiesDlg = CreateDialog( ::GetModuleHandle( NULL ), 
                                              MAKEINTRESOURCE( ID_TILEPROPERTIES_DIALOG ), 
                                              m_WinBase, 
                                              TilePropertiesDlgProc );

      if( m_HwndTilePropertiesDlg != NULL )
      {
         ::ShowWindow(    m_HwndTilePropertiesDlg, SW_SHOW );
         ::SetWindowLong( m_HwndTilePropertiesDlg, GWL_USERDATA, (LPARAM)this );

         HWND hwnd  = ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_STATIC );
         m_PropertiesDialogStaticDCs.SetHwnd( hwnd );

         m_TilePropertiesGraphic  = (m_TileMaps[m_CurrentMap])[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_TileGraphic;
         m_TilePropertiesSolidity = (m_TileMaps[m_CurrentMap])[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_Solidity;

         std::ostringstream solidity;
         solidity << (m_TileMaps[m_CurrentMap])[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_Solidity;
         std::ostringstream graphic;
         graphic << (m_TileMaps[m_CurrentMap])[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_TileGraphic;
         ::SetWindowText( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_SOLIDITY ), solidity.str().c_str() );
         ::SetWindowText( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_TILEID   ), graphic.str().c_str() );
         ::UpdateWindow( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_SOLIDITY ) );
         ::UpdateWindow( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_TILEID ) );

         std::ostringstream tile_x, tile_y;
         tile_x << m_TilePropertiesTile.x;
         tile_y << m_TilePropertiesTile.y;
         ::SetWindowText( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_TILEX ), tile_x.str().c_str() );
         ::SetWindowText( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_TILEY ), tile_y.str().c_str() );
         ::UpdateWindow( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_TILEX ) );
         ::UpdateWindow( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_TILEY ) );

         HWND hwndTrackSolidity = ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_TRACKBAR_SOLIDITY );
         // range
         ::SendMessage( hwndTrackSolidity, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, m_TileMaps[ m_CurrentMap ].maxsolidity() ) );
         // page size
         ::SendMessage( hwndTrackSolidity, TBM_SETPAGESIZE, 0, (LPARAM) (m_TileMaps[ m_CurrentMap ].maxsolidity()/10) );
         // 
         ::SendMessage( hwndTrackSolidity, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0, m_TileMaps[ m_CurrentMap ].maxsolidity() ) );
         // Position
         ::SendMessage( hwndTrackSolidity, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) (m_TileMaps[m_CurrentMap])[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_Solidity );

         //--------

         HWND hwndTrackTileID = ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_TRACKBAR_TILEID );
         // range
         ::SendMessage( hwndTrackTileID, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, m_TileMaps[ m_CurrentMap ].Bitmaps().size()-1 ) );
         // page size
         ::SendMessage( hwndTrackTileID, TBM_SETPAGESIZE, 0, (LPARAM) (m_TileMaps[ m_CurrentMap ].Bitmaps().size()-1/10) );
         // 
         ::SendMessage( hwndTrackTileID, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0, m_TileMaps[ m_CurrentMap ].Bitmaps().size()-1 ) );
         // Position
         ::SendMessage( hwndTrackTileID, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) (m_TileMaps[m_CurrentMap])[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_TileGraphic );

         ::UpdateWindow( m_HwndTilePropertiesDlg );
         ::UpdateWindow( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_STATIC ) );
         DrawTileOnPropertiesDialog( m_PropertiesDialogStaticDCs.m_GetDC );
         ret = true;
      }
      else
      {
         ::Alert( GetLastSystemError() + " CreateDialog failed" );
      }
   }
   return ret;
}

void MainWindow::DrawTileOnPropertiesDialog( C_HDC& hdc )
{
   int x = 0;
   int y = 0;
   if( m_TileMaps.size() )
   {
      if( m_TileMaps[ m_CurrentMap ].Bitmaps().size() )
      {
         m_TileMaps[ m_CurrentMap ].Bitmaps()[ m_TilePropertiesGraphic ].StretchDisplay( hdc, x, y, m_TileMaps[ m_CurrentMap ].tilewidth()*4, m_TileMaps[ m_CurrentMap ].tilewidth()*4 );
      }
   }
}

