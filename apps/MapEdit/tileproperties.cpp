#include "main.h"

BOOL CALLBACK MainWindow::TilePropertiesDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
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
            mw->m_HwndTilePropertiesDlg = NULL;
         }
      } break;
      case WM_VSCROLL:
      {
         if( mw )
         {
            if( hwnd == ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_UPDOWN ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, UDM_GETPOS, 0, 0 );
               std::ostringstream Data;
               Data << dwPos;
               Alert( dwPos );
               ::SetWindowText( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_UPDOWN ), Data.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_UPDOWN ) );
            }
         }
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
               if( mw->m_TileMap.size() )
               {
                  if( mw->m_TilePropertiesSolidity >= mw->m_TileMap.maxsolidity() )
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
               if( mw->m_TileMap.size() )
               {
//                  if( mw->m_TilePropertiesGraphic >= mw->m_TileMap.Bitmaps().size() )
//                     mw->m_TilePropertiesGraphic = 0;
               }
            }
            ::UpdateWindow( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_STATIC ) );

//            ((mw->m_TileMaps)[ mw->m_CurrentMap])[mw->m_TilePropertiesTile.y][mw->m_TilePropertiesTile.x].m_TileGraphic = mw->m_TilePropertiesGraphic;
//            mw->DrawMap( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_MemoryDC );
//            mw->DrawAStarPath( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_MemoryDC );
//            mw->m_MapsStaticDCs.BltMemoryToGet( mw->m_MapVisableX, mw->m_MapVisableY );
//            mw->DrawTileOnPropertiesDialog( mw->m_PropertiesDialogStaticDCs.m_GetDC );            
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
//             mw->m_PropertiesDialogStaticDCs.m_PaintDC.Begin();
//             mw->DrawTileOnPropertiesDialog( mw->m_PropertiesDialogStaticDCs.m_PaintDC );
//             mw->m_PropertiesDialogStaticDCs.m_PaintDC.End();
         }
      } break;
      case WM_COMMAND:
      {
         switch( LOWORD( wParam ) )
         {
            case LBN_SELCHANGE:
            {
            } break;
            case ID_TILEPROPERTIES_DIALOG_BUTTON_OK:
            {
               if( mw )
               {
                  mw->m_TileMap[mw->m_TilePropertiesTile.y][mw->m_TilePropertiesTile.x].m_TileGraphic = mw->m_TilePropertiesGraphic;
                  mw->m_TileMap[mw->m_TilePropertiesTile.y][mw->m_TilePropertiesTile.x].m_Solidity    = mw->m_TilePropertiesSolidity;
                  mw->m_HwndTilePropertiesDlg = NULL;                  
                  ::UpdateWindow( *mw );
               }
               ::EndDialog( hwnd, 0 );
            } break;
            case ID_TILEPROPERTIES_DIALOG_BUTTON_ADD:
            {
               if( mw )
               {
                  int index = ::SendMessage( ::GetDlgItem( mw->m_HwndShowSpecialDataTypesDialog, ID_SDTS_DLG_LISTBOX ), 
                                             LB_GETCURSEL, 0, 0 );
                  mw->m_SelectedDataType = index;

                  if( mw->m_TileMap.m_SpecialDataTypes.size() )
                  {
                     bool break_out = false;
                     for( int i=0; i< mw->m_TileMap.m_SpecialData.size() ; ++i )
                     {
                        if( mw->m_TilePropertiesTile.y == mw->m_TileMap.m_SpecialData[i].y &&
                            mw->m_TilePropertiesTile.x == mw->m_TileMap.m_SpecialData[i].x &&  
                            mw->m_SelectedDataType     == mw->m_TileMap.m_SpecialData[i].value )
                        {
                           break_out = true;
                           break;
                        }
                     }
                     if( break_out )
                        break;
                     if( mw->m_TileMap.m_SpecialDataTypes.size() > index )
                     {
                        mw->m_TileMap.m_SpecialData.push_back( TileMapSpecialData( mw->m_TilePropertiesTile.x, mw->m_TilePropertiesTile.y, mw->m_SelectedDataType ) );
                        ::SendMessage( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_LISTBOX ), 
                                       LB_ADDSTRING, 0, 
                                       (LPARAM) mw->m_TileMap.m_SpecialDataTypes[index].name.c_str() );
                     }
                     ::UpdateWindow( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_LISTBOX ) );

                  }
               }
            } break;
            case ID_TILEPROPERTIES_DIALOG_BUTTON_REMOVE:
            {
               if( mw )
               {
                  int index = ::SendMessage( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_LISTBOX ), 
                                             LB_GETCURSEL, 0, 0 );
                  int item_count = ::SendMessage( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_LISTBOX ), 
                                                  LB_GETCOUNT, 0, 0 );
                  if( mw->m_TileMap.m_SpecialData.size() > index )
                  {
                     mw->m_TileMap.m_SpecialData.erase( mw->m_TileMap.m_SpecialData.begin()+index );
                  }
                  if( item_count > index )
                  {
                     ::SendMessage( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_LISTBOX ), 
                                    LB_DELETESTRING, index, 
                                    (LPARAM) 0 );
                  }
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_LISTBOX ) );
               }
            } break;
            case ID_TILEPROPERTIES_DIALOG_BUTTON_CANCEL:
            {
               if( mw )
               {
//                  ((mw->m_TileMaps)[ mw->m_CurrentMap])[mw->m_TilePropertiesTile.y][mw->m_TilePropertiesTile.x].m_TileGraphic = mw->m_TilePropertiesGraphicOrignal;
//                  ((mw->m_TileMaps)[ mw->m_CurrentMap])[mw->m_TilePropertiesTile.y][mw->m_TilePropertiesTile.x].m_Solidity    = mw->m_TilePropertiesSolidityOrignal;
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
//   m_TilePropertiesTile       = GetMapTile( m_RButtonDownStartPoint );
   if( m_TilePropertiesTile < 0 )
   {
      return true;
   }

   if( m_HwndTilePropertiesDlg != NULL )
      return false;

   m_TilePropertiesTile = GetTile( m_GLRButtonDownStartPoint );


   bool        ret = false;
//   if( m_TileMaps.size()  )
//   {

//      Structs::Point starttile = m_TileMaps[ m_CurrentMap ].GetStartDrawTile();
//      m_TilePropertiesTile      += starttile;
//      m_TilePropertiesGraphicOrignal  = (m_TileMaps[ m_CurrentMap ])[ m_TilePropertiesTile.y ][ m_TilePropertiesTile.x ].m_TileGraphic;
//      m_TilePropertiesSolidityOrignal = (m_TileMaps[ m_CurrentMap ])[ m_TilePropertiesTile.y ][ m_TilePropertiesTile.x ].m_Solidity;

      m_HwndTilePropertiesDlg = CreateDialog( ::GetModuleHandle( NULL ), 
                                              MAKEINTRESOURCE( ID_TILEPROPERTIES_DIALOG ), 
                                              m_WinBase, 
                                              TilePropertiesDlgProc );

      if( m_HwndTilePropertiesDlg != NULL )
      {
         ::ShowWindow(    m_HwndTilePropertiesDlg, SW_SHOW );
         ::SetWindowLong( m_HwndTilePropertiesDlg, GWL_USERDATA, (LPARAM)this );

         HWND hwnd  = ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_STATIC );
//         m_PropertiesDialogStaticDCs.SetHwnd( hwnd );

         // ListBox
         for( int i=0; i<m_TileMap.m_SpecialData.size() ; i++ )
         {
            if( (m_TileMap.m_SpecialData[i].y     == m_TilePropertiesTile.y) &&
                (m_TileMap.m_SpecialData[i].x     == m_TilePropertiesTile.x) )
            {
               if( m_TileMap.m_SpecialDataTypes.size() > m_TileMap.m_SpecialData[i].value )
               {
                  std::string text = m_TileMap.m_SpecialDataTypes[ m_TileMap.m_SpecialData[i].value ].name;
                  ::SendMessage( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_LISTBOX ),
                                 LB_ADDSTRING, 0, (LPARAM) text.c_str() );
               }
            }
         }
         ::SendMessage( ::GetDlgItem(  m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_LISTBOX ), LB_SETCURSEL, 0, 0 );
         ::UpdateWindow( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_LISTBOX ) );
         // End ListBox


         m_TilePropertiesGraphic  = m_TileMap[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_TileGraphic;
         m_TilePropertiesSolidity = m_TileMap[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_Solidity;

         std::ostringstream solidity;
         if( m_TileMap[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_Solidity )
            solidity << m_TileMap[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_Solidity;
         else
            solidity << "0";
         std::ostringstream graphic;
         if( m_TileMap[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_TileGraphic )
            graphic << m_TileMap[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_TileGraphic;
         else
            graphic << "0";
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

         std::ostringstream data;
         if( m_TileMap.m_SpecialDataTypes.size() )
            data << m_SelectedDataType;
         else
            data << "0";
         ::SetWindowText( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_DATA  ), data.str().c_str() );
         //::UpdateWindow( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_DATA  ) );

         short max_chars = 3;
         // updown data
         ::SendMessage( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_UPDOWN ), UDM_SETRANGE, 0, (LPARAM) MAKELONG((short) pow( (double)10, (double)max_chars )-1, (short) 0) );
         ::SendMessage( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_UPDOWN ), UDM_SETBUDDY, (WPARAM) ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_EDIT_DATA ), 0 );
         ::SendMessage( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_UPDOWN ), UDM_SETPOS, (LPARAM) MAKELONG((short) m_SelectedDataType, 0), 0 );


         HWND hwndTrackSolidity = ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_TRACKBAR_SOLIDITY );
         // range
         ::SendMessage( hwndTrackSolidity, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, m_TileMap.maxsolidity() ) );
         // page size
         ::SendMessage( hwndTrackSolidity, TBM_SETPAGESIZE, 0, (LPARAM) (m_TileMap.maxsolidity()/10) );
         // 
         ::SendMessage( hwndTrackSolidity, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0, m_TileMap.maxsolidity() ) );
         // Position
         ::SendMessage( hwndTrackSolidity, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) m_TileMap[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_Solidity );

         //--------

         HWND hwndTrackTileID = ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_TRACKBAR_TILEID );
         // range
         ::SendMessage( hwndTrackTileID, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, m_TileMap.GetTileSet().size()-1 ) );
         // page size
         ::SendMessage( hwndTrackTileID, TBM_SETPAGESIZE, 0, (LPARAM) (m_TileMap.GetTileSet().size()-1/10) );
         // 
         ::SendMessage( hwndTrackTileID, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0, m_TileMap.GetTileSet().size()-1 ) );
         // Position
         ::SendMessage( hwndTrackTileID, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) m_TileMap[m_TilePropertiesTile.y][m_TilePropertiesTile.x].m_TileGraphic );

         ::UpdateWindow( m_HwndTilePropertiesDlg );
         ::UpdateWindow( ::GetDlgItem( m_HwndTilePropertiesDlg, ID_TILEPROPERTIES_DIALOG_STATIC ) );
//         DrawTileOnPropertiesDialog( m_PropertiesDialogStaticDCs.m_GetDC );
         ret = true;
      }
      else
      {
         ::Alert( GetLastSystemError() + " CreateDialog failed" );
      }
//   }
   return ret;
}

void MainWindow::DrawTileOnPropertiesDialog( C_HDC& hdc )
{
   int x = 0;
   int y = 0;
//   if( m_TileMaps.size() )
//   {
//      if( m_TileMap.GetTileSet().size() )
//      {
//         m_TileMap.GetTileSet()[ m_TilePropertiesGraphic ].StretchDisplay( hdc, x, y, m_TileMap.tilewidth()*4, m_TileMap.tilewidth()*4 );
//      }
//   }
}

