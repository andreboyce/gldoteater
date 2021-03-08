#include "main.h"

BOOL CALLBACK MainWindow::AStarStatsDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
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
            if( hwnd = ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_UPDOWN_MAXPERCENTAGETOSEARCH ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, UDM_GETPOS, 0, 0 );
               mw->m_TileMaps[ mw->m_CurrentMap ].SetMaxSearchPercentage( (dwPos/100.0) );
               std::ostringstream number_of_tiles;
               number_of_tiles << (mw->m_TileMaps[ mw->m_CurrentMap ].GetMaxSearchPercentage()) * ( mw->m_TileMaps[ mw->m_CurrentMap ].width()*mw->m_TileMaps[ mw->m_CurrentMap ].height() );
               ::SetWindowText( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_NUMBEROFTILES ), number_of_tiles.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_NUMBEROFTILES ) );
            }
         }
      } break;
      case WM_COMMAND:
      {
         switch( LOWORD( wParam ) )
         {
            case ID_ASTARSTATS_BUTTON_OK:
            {
               ::EndDialog( hwnd, 0 );
               if( mw )
               {
               }
            } break;
            case ID_ASTARSTATS_CHECK_ALLOWDIAGONAL:
            {
               if( mw )
               {
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_ALLOWDIAGONAL ) );
                  if( mw->m_TileMaps.size() > mw->m_CurrentMap )
                  {
                     mw->m_TileMaps[ mw->m_CurrentMap ].SetAllowDiagonal( ::SendMessage( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_ALLOWDIAGONAL ), BM_GETCHECK, 0, 0 ) );
                     mw->DrawMap( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                     mw->DrawAStarNodes( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                     mw->DrawTileSet( mw->m_CurrentMap, mw->m_TileSetsStaticDCs.m_GetDC );
                     mw->DrawCurrentTile( mw->m_CurrentMap, mw->m_TileSetsStaticDCs2.m_GetDC );
                  }
               }
            } break;
            case ID_ASTARSTATS_CHECK_SHOWALLNODESSEARCHED:
            {
               if( mw )
               {
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWALLNODESSEARCHED ) );
                  if( mw->m_TileMaps.size() > mw->m_CurrentMap )
                  {
                     mw->m_TileMaps[ mw->m_CurrentMap ].setshownodessearched( ::SendMessage( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWALLNODESSEARCHED ), BM_GETCHECK, 0, 0 ) );
                     mw->DrawMap( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                     mw->DrawAStarNodes( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                     mw->DrawTileSet( mw->m_CurrentMap, mw->m_TileSetsStaticDCs.m_GetDC );
                     mw->DrawCurrentTile( mw->m_CurrentMap, mw->m_TileSetsStaticDCs2.m_GetDC );
                  }
               }
            } break;
            case ID_ASTARSTATS_CHECK_SHOWNODES:
            {
               if( mw )
               {
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWNODES ) );
                  if( mw->m_TileMaps.size() > mw->m_CurrentMap )
                  {
                     mw->m_TileMaps[ mw->m_CurrentMap ].setshowpath( ::SendMessage( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWNODES ), BM_GETCHECK, 0, 0 ) );
                     mw->DrawMap( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                     mw->DrawAStarNodes( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                     mw->DrawTileSet( mw->m_CurrentMap, mw->m_TileSetsStaticDCs.m_GetDC );
                     mw->DrawCurrentTile( mw->m_CurrentMap, mw->m_TileSetsStaticDCs2.m_GetDC );
                  }
               }
            } break;
            case ID_ASTARSTATS_CHECK_SHOWSTARTANDENDNODES:
            {
               if( mw )
               {
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWSTARTANDENDNODES ) );
                  if( mw->m_TileMaps.size() > mw->m_CurrentMap )
                  {
                     mw->m_TileMaps[ mw->m_CurrentMap ].SetShowStartAndEndNode( ::SendMessage( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWSTARTANDENDNODES ), BM_GETCHECK, 0, 0 ) );
                     mw->DrawMap( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                     mw->DrawAStarNodes( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                     mw->DrawTileSet( mw->m_CurrentMap, mw->m_TileSetsStaticDCs.m_GetDC );
                     mw->DrawCurrentTile( mw->m_CurrentMap, mw->m_TileSetsStaticDCs2.m_GetDC );
                  }
               }
            } break;
            case ID_ASTARSTATS_BUTTON_STARTSEARCH:
            {
               if( mw->m_TileMaps.size() > mw->m_CurrentMap )
               {

                  if( (mw->m_TileMaps[ mw->m_CurrentMap ].GetAStarStartPos() < 0) && (mw->m_TileMaps[ mw->m_CurrentMap ].GetAStarEndPos() < 0) )
                  {
                     Alert( "You Must Set A Start Position And An End Position First" );
                     break;
                  }
                  if( (mw->m_TileMaps[ mw->m_CurrentMap ].GetAStarStartPos() < 0) )
                  {
                     Alert( "You Must Set A Start Position" );
                     break;
                  }
                  if( (mw->m_TileMaps[ mw->m_CurrentMap ].GetAStarEndPos() < 0) )
                  {
                     Alert( "You Must Set An End Position First" );
                     break;
                  }

                  mw->m_TileMaps[ mw->m_CurrentMap ].setshowpath( true );
                  mw->m_TileMaps[ mw->m_CurrentMap ].CalculateAStarPath();

                  AStar::AStar2d::SearchStats AStarStats = mw->m_TileMaps[ mw->m_CurrentMap ].GetAStarSearchStats();
                  std::ostringstream timetaken;
                  timetaken << AStarStats.time_taken;
                  ::SetWindowText( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_TIMETAKEN ), timetaken.str().c_str() );
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_TIMETAKEN ) );

                  std::ostringstream totalnodessearched;
                  totalnodessearched << AStarStats.nodes_searched;
                  ::SetWindowText( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_TOTALNODESSEARCHED ), totalnodessearched.str().c_str() );
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_TOTALNODESSEARCHED ) );

                  std::ostringstream path_length;
                  path_length << AStarStats.path_length;
                  ::SetWindowText( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_SHORTESTPATHLENGTH ), path_length.str().c_str() );
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_SHORTESTPATHLENGTH ) );

                  std::ostringstream max_percentage_to_search;
                  max_percentage_to_search << mw->m_TileMaps[ mw->m_CurrentMap ].GetMaxSearchPercentage()*100;
                  ::SetWindowText( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_MAXPERCENTAGETOSEARCH ), max_percentage_to_search.str().c_str() );
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_MAXPERCENTAGETOSEARCH ) );

                  std::ostringstream number_of_tiles;
                  number_of_tiles << (mw->m_TileMaps[ mw->m_CurrentMap ].GetMaxSearchPercentage()) * ( mw->m_TileMaps[ mw->m_CurrentMap ].width()*mw->m_TileMaps[ mw->m_CurrentMap ].height() );
                  ::SetWindowText( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_NUMBEROFTILES ), number_of_tiles.str().c_str() );
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_NUMBEROFTILES ) );

                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_ALLOWDIAGONAL        ) );
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWNODES            ) );
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWALLNODESSEARCHED ) );
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWSTARTANDENDNODES ) );

                  mw->DrawMap( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                  mw->DrawAStarNodes( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                  mw->DrawTileSet( mw->m_CurrentMap, mw->m_TileSetsStaticDCs.m_GetDC );
                  mw->DrawCurrentTile( mw->m_CurrentMap, mw->m_TileSetsStaticDCs2.m_GetDC );
               }
            } break;
            case ID_ASTARSTATS_UPDOWN_MAXPERCENTAGETOSEARCH:
            {
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

bool MainWindow::ShowAStarStatsDlg( void )
{
   if( m_TileMaps.size() <= m_CurrentMap )
   {
      return true;
   }

   bool        ret = false;
   m_HwndAStarSearchStatsDlg = CreateDialog( ::GetModuleHandle( NULL ), 
                                             MAKEINTRESOURCE( ID_ASTARSTATS_DIALOG ), 
                                             m_WinBase, 
                                             AStarStatsDlgProc );

   if( m_HwndAStarSearchStatsDlg != NULL )
   {
      ::ShowWindow(    m_HwndAStarSearchStatsDlg, SW_SHOW );
      ::SetWindowLong( m_HwndAStarSearchStatsDlg, GWL_USERDATA, (LPARAM)this );

      AStar::AStar2d::SearchStats AStarStats = m_TileMaps[ m_CurrentMap ].GetAStarSearchStats();
      std::ostringstream timetaken;
      timetaken << AStarStats.time_taken;
      ::SetWindowText( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_TIMETAKEN ), timetaken.str().c_str() );
      ::UpdateWindow( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_TIMETAKEN ) );

      std::ostringstream totalnodessearched;
      totalnodessearched << AStarStats.nodes_searched;
      ::SetWindowText( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_TOTALNODESSEARCHED ), totalnodessearched.str().c_str() );
      ::UpdateWindow( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_TOTALNODESSEARCHED ) );

      std::ostringstream path_length;
      path_length << AStarStats.path_length;
      ::SetWindowText( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_SHORTESTPATHLENGTH ), path_length.str().c_str() );
      ::UpdateWindow( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_SHORTESTPATHLENGTH ) );

      std::ostringstream max_percentage_to_search;
      max_percentage_to_search << m_TileMaps[ m_CurrentMap ].GetMaxSearchPercentage()*100;
      ::SetWindowText( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_MAXPERCENTAGETOSEARCH ), max_percentage_to_search.str().c_str() );
      ::SendMessage( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_MAXPERCENTAGETOSEARCH ), EM_LIMITTEXT, 3, 0 );
      ::UpdateWindow( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_MAXPERCENTAGETOSEARCH ) );

      std::ostringstream number_of_tiles;
      number_of_tiles << (m_TileMaps[ m_CurrentMap ].GetMaxSearchPercentage()) * ( m_TileMaps[ m_CurrentMap ].width()*m_TileMaps[ m_CurrentMap ].height() );
      ::SetWindowText( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_NUMBEROFTILES ), number_of_tiles.str().c_str() );
      ::UpdateWindow( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_NUMBEROFTILES ) );

      ::SendMessage( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_UPDOWN_MAXPERCENTAGETOSEARCH ), UDM_SETRANGE, 0, (LPARAM) MAKELONG((short) 100, (short) 0) );
      ::SendMessage( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_UPDOWN_MAXPERCENTAGETOSEARCH ), UDM_SETBUDDY, (WPARAM)::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_EDIT_MAXPERCENTAGETOSEARCH ), 0 );

      ::SendMessage( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_ALLOWDIAGONAL ), BM_SETCHECK, ( m_TileMaps[ m_CurrentMap ].GetAllowDiagonal() ) ? BST_CHECKED : BST_UNCHECKED, 0 );
      ::UpdateWindow( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_ALLOWDIAGONAL ) );

      ::SendMessage( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWNODES ), BM_SETCHECK, ( m_TileMaps[ m_CurrentMap ].showpath() ) ? BST_CHECKED : BST_UNCHECKED, 0 );
      ::UpdateWindow( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWNODES ) );

      ::SendMessage( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWALLNODESSEARCHED ), BM_SETCHECK, ( m_TileMaps[ m_CurrentMap ].shownodessearched() ) ? BST_CHECKED : BST_UNCHECKED, 0 );
      ::UpdateWindow( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWALLNODESSEARCHED ) );

      ::SendMessage( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWSTARTANDENDNODES ), BM_SETCHECK, ( m_TileMaps[ m_CurrentMap ].ShowStartAndEndNode() ) ? BST_CHECKED : BST_UNCHECKED, 0 );
      ::UpdateWindow( ::GetDlgItem( m_HwndAStarSearchStatsDlg, ID_ASTARSTATS_CHECK_SHOWSTARTANDENDNODES ) );

      ret = true;
   }
   else
   {
      ::Alert( GetLastSystemError() + " CreateDialog failed" );
   }
   return ret;
}
