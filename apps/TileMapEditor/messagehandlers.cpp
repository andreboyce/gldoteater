#include "main.h"

LRESULT MainWindow::MWBaseOnCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( LOWORD(wparam) )
   {
      case 1:
      {
           mw.ShowNewMapDlg();
      } break;
      case 2:
      {
         mw.ReNameMap();
      } break;
      case 3:
      {
         mw.OpenTileMap();
      } break;
      case 4:
      {
         mw.OpenTile();
      } break;
      case 5:
      {
         mw.ShowGetTilesFromBitmapDlg();
      } break;
      case 6:
      {
         if( mw.m_TileMaps.size() )
         {
            char buffer[MAX_PATH];
            ::getcwd( buffer, MAX_PATH-1 );
            ::chdir( mw.m_TileMaps[ mw.m_CurrentMap ].GetMapWorkingDirectory().c_str() );

            if( !mw.m_TileMaps[ mw.m_CurrentMap ].SaveMap( mw.m_TileMaps[ mw.m_CurrentMap ].GetMapFileName() ) )
            {
               Alert( "Could Not Save Map" + GetLastSystemError() );
            }
            else
            {
               mw.m_StatusBar.EnableSimpleMode( "File " + mw.m_TileMaps[ mw.m_CurrentMap ].GetMapFileName() + " Saved" );
            }

            if( !mw.m_TileMaps[ mw.m_CurrentMap ].SaveTileSet(  mw.m_TileMaps[ mw.m_CurrentMap ].GetTileSetFileName() ) )
            {
               Alert( "Could Not Save TileSet" + GetLastSystemError() );
            }
            else
            {
               mw.m_StatusBar.EnableSimpleMode( "File " + mw.m_TileMaps[ mw.m_CurrentMap ].GetTileSetFileName() + " Saved" );
            }

            ::chdir( buffer );
         }
      } break;
      case 7:
      {
         mw.SaveTileMap();
      } break;
      case 8:
      {
         mw.CloseTileMap( mw.m_CurrentMap );
         mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
         mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
         mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_GetDC );
         mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_GetDC );
      } break;
      case 9:
      {
         while( mw.m_TileMaps.size() )
         {
            mw.CloseTileMap( mw.m_TileMaps.size()-1 );
         }
         mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
         mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
         mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_GetDC );
         mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_GetDC );
      } break;
      case 10:
      {
         ::SendMessage( mw.m_WinBase, WM_CLOSE, 0, 0 );
      } break;
      case 11:
      {
         mw.ShowSolidityDlg();
      };
      case 12:
      {
         if( mw.m_TileMaps.size() )
         {
            mw.m_TileMaps[ mw.m_CurrentMap ].DeSelectedTiles();
         }
      } break;
      case 13:
      {
         if( mw.m_TileMaps.size() )
         {
            Structs::Point starttile                  = mw.m_TileMaps[ mw.m_CurrentMap ].GetStartDrawTile();
            Structs::Point tile                       = mw.GetMapTile( Structs::Point( mw.m_RButtonDownStartPoint.x+starttile.x, mw.m_RButtonDownStartPoint.y+starttile.y)  );
            mw.m_CurrentTileGraphics[ mw.m_CurrentMap ] = (mw.m_TileMaps[mw.m_CurrentMap])[tile.y+starttile.y][tile.x+starttile.x].m_TileGraphic;
            mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_GetDC );
            mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_GetDC );
         }
      } break;
      case 14:
      {
         if( mw.m_TileMaps.size() )
         {
            mw.m_TileMaps[ mw.m_CurrentMap ].ReLoadTile( mw.m_CurrentTileGraphics[ mw.m_CurrentMap ] );
            mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
            mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
            mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_GetDC );
            mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_GetDC );
         }
      } break;
      case 15:
      {
         if( mw.m_TileMaps.size() )
         {
            mw.GetSelectedTiles();
            mw.m_TileMaps[ mw.m_CurrentMap ].CopySelectedTiles();
            mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
            mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
            mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_GetDC );
            mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_GetDC );
         }
      } break;
      case 16:
      {
         if( mw.m_TileMaps.size() )
         {
            mw.GetSelectedTiles();
            mw.m_TileMaps[ mw.m_CurrentMap ].CutSelectedTiles();
         }
      } break;
      case 17:
      {
         if( mw.m_TileMaps.size() )
         {
            Structs::Point starttile = mw.m_TileMaps[ mw.m_CurrentMap ].GetStartDrawTile();
            Structs::Point tile      = mw.GetMapTile( Structs::Point( mw.m_RButtonDownStartPoint.x+starttile.x, mw.m_RButtonDownStartPoint.y+starttile.y)  );
            mw.m_TileMaps[ mw.m_CurrentMap ].PasteSelectedTiles( tile.y+starttile.y, tile.x+starttile.x );
            mw.m_TileMaps[ mw.m_CurrentMap ].DeSelectedTiles();
         }
      } break;
      case 18:
      {
         Alert( "undo" );
      } break;
      case 19:
      {
         Alert( "redo" );
      } break;
      case 20:
      {
         mw.ShowResizeMapDlg();
      } break;
      case 21:
      {
         mw.ShowTilePropertiesDlg();
      } break;
      case 22:
      {
         if( mw.m_TileMaps.size() )
         {
            std::string FileName;
            mw.m_WinFileHandler.SaveFile( FileName, "Bitmap Files *.bmp\0*.bmp\0All files *.*\0*.*\0\0",
                                          ".bmp", 0, 0, "Save Map" );
            (mw.m_TileMaps[ mw.m_CurrentMap ].Bitmaps()[ mw.m_CurrentTileGraphics[ mw.m_CurrentMap ] ]).Save( FileName );
         }
      } break;
      case 23:
      {
         mw.ShowAddSpriteDlg();
      } break;
      case 24:
      {
         mw.ShowDeleteSpriteDlg();
      } break;
      case 25:
      {
         mw.ShowSpriteStatsDlg();
      } break;
      case 26:
      {
         if( mw.m_TileMaps.size() )
         {
            Structs::Point starttile = mw.m_TileMaps[ mw.m_CurrentMap ].GetStartDrawTile();
            Structs::Point tile      = mw.GetMapTile( Structs::Point( mw.m_RButtonDownStartPoint.x, mw.m_RButtonDownStartPoint.y)  );
            tile += starttile;
            mw.m_TileMaps[ mw.m_CurrentMap ].SetAStarStartPos( tile );
            mw.m_TileMaps[ mw.m_CurrentMap ].setshowpath( false );
         }
      } break;
      case 27:
      {
         if( mw.m_TileMaps.size() )
         {
            Structs::Point starttile = mw.m_TileMaps[ mw.m_CurrentMap ].GetStartDrawTile();
            Structs::Point tile      = mw.GetMapTile( Structs::Point( mw.m_RButtonDownStartPoint.x, mw.m_RButtonDownStartPoint.y)  );
            tile += starttile;
            mw.m_TileMaps[ mw.m_CurrentMap ].SetAStarEndPos( tile );
            mw.m_TileMaps[ mw.m_CurrentMap ].setshowpath( false );
         }
      } break;
      case 28:
      {
         if( mw.m_TileMaps.size() > mw.m_CurrentMap )
         {
            if( (mw.m_TileMaps[ mw.m_CurrentMap ].GetAStarStartPos() < 0) && (mw.m_TileMaps[ mw.m_CurrentMap ].GetAStarEndPos() < 0) )
            {
               Alert( "You Must Set A Start Position And An End Position First" );
               break;
            }
            if( (mw.m_TileMaps[ mw.m_CurrentMap ].GetAStarStartPos() < 0) )
            {
               Alert( "You Must Set A Start Position" );
               break;
            }
            if( (mw.m_TileMaps[ mw.m_CurrentMap ].GetAStarEndPos() < 0) )
            {
               Alert( "You Must Set An End Position First" );
               break;
            }
            mw.m_TileMaps[ mw.m_CurrentMap ].CalculateAStarPath();
            mw.m_TileMaps[ mw.m_CurrentMap ].setshowpath( true );
            mw.ShowAStarStatsDlg();
         }
      } break;
      case 29:
      {
         if( mw.m_TileMaps.size() > mw.m_CurrentMap )
         {
            mw.ShowAStarStatsDlg();
         }
      } break;
      case 30:
      {
         if( mw.m_TileMaps.size() )
         {
            std::string FileName;
            mw.m_WinFileHandler.SaveFile( FileName, "Bitmap Files *.bmp\0*.bmp\0All files *.*\0*.*\0\0",
                                          ".bmp", 0, 0, "Save Map" );
            ::UpdateWindow( mw );
            C_MemoryDC hdcmem;
            Structs::Rect rect;
            ::GetWindowRect( mw, &rect );
            hdcmem.CreateBitmap( mw, rect );
            HDC hdc = ::GetDC( mw );

            ::BitBlt( hdcmem, 
                      0, 0,
                      rect.right-rect.left,
                      rect.bottom-rect.top,
                      hdc,
                      0, 0,
                      SRCCOPY );

            ::ReleaseDC( mw, hdc );

            if( !::SaveBitmap( hdcmem, hdcmem, FileName ) )
            {
               Alert( "Could Not Capture Screen Shot" );
            }
         }
      } break;
      case 31:
      {
         mw.ShowAboutDlg();
      } break;
      case 32:
      {
         mw.OpenHelp();
      } break;
      case ID_EDIT_SPRITES_SPECIALDATATYPES:
      {
//         mw.m_SpecialDataTypesDialog = new SpecialDataTypesDialog( NULL );
//         mw.m_SpecialDataTypesDialog->Show();
      } break;
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnMouseMove( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   ::GetCursorPos( &mw.m_MousePosition );
   ::ScreenToClient( mw.m_WinBase, &mw.m_MousePosition );

   Structs::Point tile = mw.GetMapTile();
   mw.HilightMouseOverTile( tile );
   if( mw.m_bRMouseDown )
   {
      mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_MemoryDC );
      mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_MemoryDC );
      mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_GetDC );
      mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_GetDC );
      mw.DrawMouseRightClickRect( mw.m_MapsStaticDCs.m_MemoryDC );
      mw.m_MapsStaticDCs.BltMemoryToGet( mw.m_MapVisableX, mw.m_MapVisableY );
   }

/*   MSG   msg;
   POINT pt    = { 0, 0 };
   msg.hwnd    = mw.m_LastHwndPassed;
   msg.message = WM_MOUSEMOVE;
   msg.wParam  = wparam;
   msg.lParam  = lparam;
   msg.time    = 0;
   msg.pt      = pt;

   ::SendMessage( mw.m_EnLargedTileToolTip, TTM_RELAYEVENT, 0, (LPARAM)(LPMSG)&msg );
*/
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_MOUSEMOVE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnLDoubleClick( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   Structs::Point tile = mw.GetMapTile();
   if( mw.ValidTileIndex( tile ) && mw.m_TileMaps.size() )
   {
      Structs::Point starttile = mw.m_TileMaps[ mw.m_CurrentMap ].GetStartDrawTile();
      if( (wparam & MK_SHIFT) )
      {
         tile += starttile;
         (mw.m_TileMaps[mw.m_CurrentMap])[tile.y][tile.x].m_TileGraphic++;
         (mw.m_TileMaps[mw.m_CurrentMap])[tile.y][tile.x].m_Solidity = mw.m_Solidity[ mw.m_CurrentMap ];
         if( (mw.m_TileMaps[mw.m_CurrentMap])[tile.y][tile.x].m_TileGraphic >= mw.m_TileMaps[ mw.m_CurrentMap ].Bitmaps().size() )
         {
            (mw.m_TileMaps[mw.m_CurrentMap])[tile.y][tile.x].m_TileGraphic = 0;
         }
         mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
         mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
         return 0;
      }
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDBLCLK, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRDoubleClick(     MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONDBLCLK, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_bRMouseDown = true;
   ::GetCursorPos( &mw.m_RButtonDownStartPoint );
   ::ScreenToClient( mw.m_MapsTabStatic, &mw.m_RButtonDownStartPoint );
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONDOWN, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRButtonU( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   ::GetCursorPos( &mw.m_RButtonUpStartPoint );
   ::ScreenToClient( mw.m_MapsTabStatic, &mw.m_RButtonUpStartPoint );

//   Structs::Rect Rect;
//   ::GetWindowRect( HWND hWnd, Rect );
//   if( CheckCollide( Rect, m_RButtonDownStartPoint ) )
//   if( 1 )
//   {
//      mw.m_TPopupMenu2.Display( mw.m_MousePosition.x, mw.m_MousePosition.y );
//   }
//   if( 1 )
//   {
      mw.m_TPopupMenu.Display( mw.m_MousePosition.x, mw.m_MousePosition.y );
//   }
   mw.m_bRMouseDown = false;
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONUP, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnLButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_bLMouseDown = true;
   Structs::Point tile      = mw.GetMapTile();
   Structs::Point starttile;
   if( mw.m_TileMaps.size() > mw.m_CurrentMap )
   {
      starttile = mw.m_TileMaps[ mw.m_CurrentMap ].GetStartDrawTile();
   }
   if( mw.ValidTileIndex( tile ) && mw.m_TileMaps.size() )
   {
      if( (wparam & MK_CONTROL) )
      {
         mw.m_CurrentTileGraphics[ mw.m_CurrentMap ] = (mw.m_TileMaps[mw.m_CurrentMap])[tile.y+starttile.y][tile.x+starttile.x].m_TileGraphic;
         std::ostringstream index;
         index << mw.m_CurrentTileGraphics[ mw.m_CurrentMap ];
         mw.m_StatusBar.EnableSimpleMode( "Tile Index: " + index.str() );
         mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_GetDC );
         mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_GetDC );
         return 0;
      }
      if( (wparam & MK_SHIFT) )
      {
         tile += starttile;
         (mw.m_TileMaps[mw.m_CurrentMap])[tile.y][tile.x].m_TileGraphic++;
         (mw.m_TileMaps[mw.m_CurrentMap])[tile.y][tile.x].m_Solidity = mw.m_Solidity[ mw.m_CurrentMap ];
         if( (mw.m_TileMaps[mw.m_CurrentMap])[tile.y][tile.x].m_TileGraphic >= mw.m_TileMaps[ mw.m_CurrentMap ].Bitmaps().size() )
         {
            (mw.m_TileMaps[mw.m_CurrentMap])[tile.y][tile.x].m_TileGraphic = 0;
         }
         mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
         mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
         return 0;
      }
   }
   if( mw.m_TileMaps.size() )
   {
      if( mw.ValidTileIndex( tile ) )
      {
         Structs::Point starttile = mw.m_TileMaps[ mw.m_CurrentMap ].GetStartDrawTile();
         mw.m_TileMaps[ mw.m_CurrentMap ].SetTileGraphic( tile.y+starttile.y, tile.x+starttile.x, mw.m_CurrentTileGraphics[ mw.m_CurrentMap ] );
         (mw.m_TileMaps[mw.m_CurrentMap])[tile.y+starttile.y][tile.x+starttile.x].m_Solidity = mw.m_Solidity[ mw.m_CurrentMap ];
         mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
         mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
      }

      Structs::Point tilesettile = mw.GetMapTileSetTile();
      if( tilesettile >= 0 )
      {
         if( mw.GetMapTileSetTileID( tilesettile ) > -1 )
         {
            mw.m_CurrentTileGraphics[ mw.m_CurrentMap ] = mw.GetMapTileSetTileID( tilesettile );
            if( mw.m_CurrentTileGraphics[ mw.m_CurrentMap ] >= mw.m_TileMaps[ mw.m_CurrentMap ].Bitmaps().size() )
            {
               mw.m_CurrentTileGraphics[ mw.m_CurrentMap ] = 0;
            }
            else
            {
               if( mw.m_TileMaps[ mw.m_CurrentMap ].GetSolidity( mw.GetMapTileSetTileID( tilesettile ) ) > -1 )
                  mw.m_Solidity[mw.m_CurrentMap] = mw.m_TileMaps[ mw.m_CurrentMap ].GetSolidity( mw.GetMapTileSetTileID( tilesettile ) );
            }
         }
         std::ostringstream index;
         index << mw.m_CurrentTileGraphics[ mw.m_CurrentMap ];
         mw.m_StatusBar.EnableSimpleMode( "Tile Index: " + index.str() );
         mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_GetDC );
         mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_GetDC );
      }
   }

   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDOWN, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnLButtonU( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_bLMouseDown = false;
   mw.m_ClickedTile = mw.GetMapTile();
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONUP, wparam, lparam );
}


LRESULT MainWindow::MWBaseOnCreate( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   std::string popup_menu( "popupmenu.xml" );
   DWORD dwFirstCommandID = 11;
   mw.m_TPopupMenu.Load(  mw.m_LastHwndPassed, popup_menu, dwFirstCommandID );
//   mw.m_TPopupMenu2.Load(  mw.m_LastHwndPassed, "popupmenu2.xml", 26 );
   std::string menu( "menu.xml" );
   DWORD first_command = mw.m_XMLMenuFirstCommand;
//   mw.m_XMLMenu.LoadMenu( menu, first_command, false );
//   mw.m_XMLMenu.LoadMenu( menu, first_command );
   mw.m_XMLMenu.SetMenu(   mw.m_LastHwndPassed );
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_CREATE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_MapsStaticDCs.m_MemoryDC.CreateBitmap( mw.m_MapsTabStatic, GetWindowWidth( mw.m_MapsTabStatic ), GetWindowHeight( mw.m_MapsTabStatic ) );
   mw.m_TileSetsStaticDCs.m_MemoryDC.CreateBitmap( mw.m_TileSetsTabStatic, GetWindowWidth( mw.m_TileSetsTabStatic ), GetWindowHeight( mw.m_TileSetsTabStatic ) );
   mw.ResizeWindows( LOWORD(lparam), HIWORD(lparam) );
   mw.m_StatusBar.InitializeStatusBar();
   mw.ResizeTileSetsIDs();
   mw.ResetScrollBars();
   mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
   mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
   mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_GetDC );
   mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_GetDC );

//   mw.m_TileSetsStaticDCs2.m_GetDC.Clear( WHITE_BRUSH );

//   mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_MemoryDC );
//   mw.m_MapsStaticDCs.BltMemoryToGet( mw.m_MapVisableX, mw.m_MapVisableY );
//   ::UpdateWindow( mw.m_MapsTab );
//   ::UpdateWindow( mw.m_MapsTabStatic );

   return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnNotify( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   NMHDR *lpnmhdr = (NMHDR*)(lparam);
   switch( lpnmhdr->code )
   {
      case TCN_SELCHANGE:
      {
         mw.ResetScrollBars();
         int iPage = TabCtrl_GetCurSel( lpnmhdr->hwndFrom );
         mw.m_CurrentMap = iPage;
         if( lpnmhdr->hwndFrom == mw.m_MapsTab )
         {
            mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
            mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
            mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_GetDC );
            mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_GetDC );
            mw.m_TileSetsTab.SelectTab( mw.m_CurrentMap );
         }
         else if( lpnmhdr->hwndFrom == mw.m_TileSetsTab )
         {
            mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
            mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
            mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_GetDC );
            mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_GetDC );
            mw.m_MapsTab.SelectTab( mw.m_CurrentMap );
         }
      } break;
   }

   return ::DefWindowProc( mw.m_LastHwndPassed, WM_NOTIFY, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnHScroll( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{   
   SCROLLINFO si;
   ::ZeroMemory( &si, sizeof(si) );

   HWND hwnd = (HWND)lparam;

   // Get all the vertial scroll bar information
   si.cbSize = sizeof( si );
   si.fMask  = SIF_ALL;

   // Save the position for comparison later on
   if( !::GetScrollInfo( hwnd, SB_CTL, &si ) )
   {
      Alert( GetLastSystemError() );
      return -1;
   }
   int xPos = si.nPos;

   switch( LOWORD(wparam) )
   {
      // user clicked left arrow
      case SB_LINELEFT:
      {
         si.nPos -= 1;
      } break;
              
      // user clicked right arrow
      case SB_LINERIGHT:
      {
         si.nPos += 1;
      } break;
              
      // user clicked the scroll bar shaft left of the scroll box
      case SB_PAGELEFT:
      {
//        si.nPos -= si.nPage;
      } break;
              
      // user clicked the scroll bar shaft right of the scroll box
      case SB_PAGERIGHT:
      {
//         si.nPos += si.nPage;
      } break;
              
      // user dragged the scroll box
      case SB_THUMBTRACK:
      {
         si.nPos = si.nTrackPos;
      } break;
              
      default :
           break;
   }

   if( hwnd == mw.m_MapsHScroll )
   {
      if( mw.MapsScrolls.size() > mw.m_CurrentMap )
      {
         mw.MapsScrolls[ mw.m_CurrentMap ].xCurrentScroll = si.nPos;
      }
   }
   else if( hwnd == mw.m_TileSetsHScroll )
   {
      if( mw.TileSetsScrolls.size() > mw.m_CurrentMap )
      {
         mw.TileSetsScrolls[ mw.m_CurrentMap ].xCurrentScroll = si.nPos;
      }
   }

   if( mw.m_TileMaps.size() )
   {
      Structs::Point starttile = mw.m_TileMaps[ mw.m_CurrentMap ].GetStartDrawTile();
      if( mw.m_TileMaps[ mw.m_CurrentMap ].tilewidth() )
      {
         starttile.x = si.nPos/mw.m_TileMaps[ mw.m_CurrentMap ].tilewidth();
         mw.m_TileMaps[ mw.m_CurrentMap ].SetStartDrawTile( starttile );
         mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_MemoryDC );
         mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_MemoryDC );
         mw.m_MapsStaticDCs.BltMemoryToGet( mw.m_MapVisableX, mw.m_MapVisableY );
      }
   }

   // Set the position and then retrieve it.  Due to adjustments
   //   by Windows it may not be the same as the value set.
   si.fMask = SIF_POS;
   ::SetScrollInfo( hwnd, SB_CTL, &si, TRUE );
   ::GetScrollInfo( hwnd, SB_CTL, &si );
         
   // If the position has changed, scroll the window 
   if( si.nPos != xPos )
   {
//      ScrollWindow( hwnd, mw.m_TileMap.tilewidth() * (xPos - si.nPos), 0, NULL, NULL );
   }
   
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_HSCROLL, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnVScroll( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{

   SCROLLINFO si;

   HWND hwnd = (HWND)lparam;

   // Get all the vertial scroll bar information
   si.cbSize = sizeof( si );
   si.fMask  = SIF_ALL;

   // Save the position for comparison later on
   if( !GetScrollInfo( hwnd, SB_CTL, &si ) )
   {
      Alert( GetLastSystemError() );
      return -1;
   }

   int yPos = si.nPos;

   switch( LOWORD(wparam) )
   {
      // user clicked left arrow
      case SB_LINELEFT:
      {
         si.nPos -= 1;
      } break;
              
      // user clicked right arrow
      case SB_LINERIGHT:
      {
         si.nPos += 1;
      } break;
              
      // user clicked the scroll bar shaft left of the scroll box
      case SB_PAGELEFT:
      {
//        si.nPos -= si.nPage;
      } break;
              
      // user clicked the scroll bar shaft right of the scroll box
      case SB_PAGERIGHT:
      {
//         si.nPos += si.nPage;
      } break;
              
      // user dragged the scroll box
      case SB_THUMBTRACK:
      {
         si.nPos = si.nTrackPos;
      } break;
              
      default :
           break;
   }

   if( hwnd == mw.m_MapsVScroll )
   {
      if( mw.MapsScrolls.size() > mw.m_CurrentMap )
         mw.MapsScrolls[ mw.m_CurrentMap ].yCurrentScroll = si.nPos;
   }
   else if( hwnd == mw.m_TileSetsVScroll )
   {
      if( mw.TileSetsScrolls.size() > mw.m_CurrentMap )
         mw.TileSetsScrolls[ mw.m_CurrentMap ].yCurrentScroll = si.nPos;
   }

   if( mw.m_TileMaps.size() )
   {
      Structs::Point starttile = mw.m_TileMaps[ mw.m_CurrentMap ].GetStartDrawTile();
      if( mw.m_TileMaps[ mw.m_CurrentMap ].tilewidth() )
      {
         starttile.y = si.nPos/mw.m_TileMaps[ mw.m_CurrentMap ].tileheight();
         mw.m_TileMaps[ mw.m_CurrentMap ].SetStartDrawTile( starttile );
         mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_MemoryDC );
         mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_MemoryDC );
         mw.m_MapsStaticDCs.BltMemoryToGet( mw.m_MapVisableX, mw.m_MapVisableY );
      }
   }

   // Set the position and then retrieve it.  Due to adjustments
   //   by Windows it may not be the same as the value set.
   si.fMask = SIF_POS;
   ::SetScrollInfo( hwnd, SB_CTL, &si, TRUE );
   ::GetScrollInfo( hwnd, SB_CTL, &si );
         
   // If the position has changed, scroll the window 
   if( si.nPos != yPos )
   {
//      ::ScrollWindow( hwnd, mw.m_TileMap.tileheight() * (yPos - si.nPos), 0, NULL, NULL );
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_VSCROLL, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnKeyDown( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYDOWN, wparam, lparam );
}     

LRESULT MainWindow::MWBaseOnKeyUp( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYUP, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnPaint( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_MapsStaticDCs.m_PaintDC.Begin();
   mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_PaintDC );
   mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_PaintDC );
   mw.m_MapsStaticDCs.m_PaintDC.End();

   mw.m_TileSetsStaticDCs.m_PaintDC.Begin();
   mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_PaintDC );
   mw.m_TileSetsStaticDCs.m_PaintDC.End();

   mw.m_TileSetsStaticDCs2.m_PaintDC.Begin();
   mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_PaintDC );
   mw.m_TileSetsStaticDCs2.m_PaintDC.End();

   return ::DefWindowProc( mw.m_LastHwndPassed, WM_PAINT, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnTimer( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( wparam )
   {
      case DRAW_TIMER:
      {
         mw.DrawMap( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
         mw.DrawAStarNodes( mw.m_CurrentMap, mw.m_MapsStaticDCs.m_GetDC );
         mw.DrawTileSet( mw.m_CurrentMap, mw.m_TileSetsStaticDCs.m_GetDC );
         mw.DrawCurrentTile( mw.m_CurrentMap, mw.m_TileSetsStaticDCs2.m_GetDC );
      } break;
      case ADD_SPRITE_DRAW_TIMER:
      {
         int index       = TabCtrl_GetCurSel( ::GetDlgItem( mw.m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ) );
         if( index < 0 )
         {
            mw.m_AddSpriteDrawTimer.Kill();
            break;
         }
         int num_of_tabs = TabCtrl_GetItemCount( ::GetDlgItem( mw.m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ) );
         index++;
         if( index >= num_of_tabs )
         {
            if( mw.m_bLoopSpriteAnimation )
            {
               index = 0;
            }
            else
            {
               mw.m_AddSpriteDrawTimer.Kill();
               break;
            }
         }
         TabCtrl_SetCurFocus( ::GetDlgItem( mw.m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ), index );
         mw.DrawOnAddSpriteDialog( mw.m_AddSpriteDialogStaticDCs.m_GetDC );
      } break;


   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_TIMER, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnKillFocus( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_bHasKeyboardFocus = false;
   mw.m_bLMouseDown       = false;
   mw.m_bRMouseDown       = false;
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KILLFOCUS, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnSetFocus( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_bHasKeyboardFocus = true;
   mw.m_bLMouseDown       = false;
   mw.m_bRMouseDown       = false;
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_SETFOCUS, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnClose( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.SaveConfig();
   while( mw.m_TileMaps.size() )
   {
      mw.CloseTileMap( mw.m_TileMaps.size()-1 );
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_CLOSE, wparam, lparam );
}

