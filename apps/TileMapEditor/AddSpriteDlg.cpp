#include "main.h"

BOOL CALLBACK MainWindow::AddSpriteDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
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
            mw->m_AddSpriteDialogStaticDCs.m_PaintDC.Begin();
            mw->DrawOnAddSpriteDialog( mw->m_AddSpriteDialogStaticDCs.m_PaintDC );
            mw->m_AddSpriteDialogStaticDCs.m_PaintDC.End();
         }
      } break;
	   case WM_HSCROLL:
      {
         if( mw )
         {
            HWND hwnd = (HWND) lParam;
            if( hwnd == ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TRACKBAR_FRAME ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, TBM_GETPOS, 0, 0 );
               std::ostringstream temp;
               temp << dwPos;
               TabCtrl_SetCurFocus( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ), dwPos );
               int index = 0;
               std::deque< C_Sprite >& Sprites = mw->m_TileMaps[ mw->m_CurrentMap ].GetSprites();
               if( Sprites.size() )
               {
                  SpriteMap::D_HBitmaps& Images = mw->m_TileMaps[ mw->m_CurrentMap ].GetSpriteBitmaps();
                  std::deque< C_Sprite::S_Frame >& Frames = Sprites.back().GetFrames();
                  if( Frames.size() > dwPos )
                  {
                     index = Frames[dwPos].m_index;
                  }         
               }
               ::SendMessage( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TRACKBAR_AVAILABLEIMAGES ), TBM_SETPOS, true, index );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TRACKBAR_AVAILABLEIMAGES ) );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ) );
               mw->DrawOnAddSpriteDialog( mw->m_AddSpriteDialogStaticDCs.m_GetDC );
            }
            hwnd = (HWND) lParam;
            if( hwnd == ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TRACKBAR_AVAILABLEIMAGES ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, TBM_GETPOS, 0, 0 );
               std::ostringstream temp;
               temp << dwPos;
               TCITEM item;
               ::ZeroMemory( &item, sizeof(item) );
               item.mask    = TCIF_TEXT;
               item.pszText = (char*)temp.str().c_str();
               TabCtrl_SetItem( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ), 
                                TabCtrl_GetCurFocus( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ) ),
                                &item );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ) );
               mw->DrawOnAddSpriteDialog( mw->m_AddSpriteDialogStaticDCs.m_GetDC );
            }
         }
      } break;
	   case WM_VSCROLL:
      {
         if( mw )
         {
            HWND hwnd = (HWND) lParam;
            if( hwnd == ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_UPDOWN_DELAY ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, UDM_GETPOS, 0, 0 );
               std::ostringstream delay;
               delay << dwPos;
               int index = TabCtrl_GetCurSel( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ) );
               if( mw->m_TileMaps.size() > mw->m_CurrentMap )
               {
                  std::deque< C_Sprite >& Sprites = mw->m_TileMaps[ mw->m_CurrentMap ].GetSprites();
                  if( Sprites.size() )
                  {
                     std::deque< C_Sprite::S_Frame >& Frames = Sprites.back().GetFrames();
                     if( Frames.size() > index )
                     {
                        Frames[index].m_duration = dwPos;
                     }
                     int length = 0;
                     for( int i =0; i<Frames.size(); ++i )
                     {
                        length += Frames[i].m_duration;
                     }
                     std::ostringstream cycle_lenght;
                     cycle_lenght << length;
                     ::SetWindowText( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_CYCLELENGTH ), cycle_lenght.str().c_str() );
                     ::UpdateWindow(  ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_CYCLELENGTH ) );
                  }
               }
               ::SetWindowText( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_DELAY ), delay.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_DELAY ) );
               std::ostringstream frame;
               frame << index;
               ::SetWindowText( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_FRAME ), frame.str().c_str() );
               ::UpdateWindow(  ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_FRAME ) );
            }
            if( hwnd == ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_UPDOWN_ZOOM ) )
            {
               DWORD dwPos     = ::SendMessage( hwnd, UDM_GETPOS, 0, 0 );
               mw->m_AddSpriteZoom = dwPos;
               std::ostringstream zoom;
               zoom << dwPos;
               ::SetWindowText( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_ZOOM ), zoom.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_ZOOM ) );
               mw->m_AddSpriteDialogStaticDCs.m_GetDC.Clear( WHITE_BRUSH );
               mw->DrawOnAddSpriteDialog( mw->m_AddSpriteDialogStaticDCs.m_GetDC );
            }
         }
      } break;
      case WM_NOTIFY:
      {
         NMHDR *lpnmhdr = (NMHDR*)(lParam);
         switch( lpnmhdr->code )
         {
            case TCN_SELCHANGE:
            {
               if( mw )
               {
                  int iPage = TabCtrl_GetCurSel( lpnmhdr->hwndFrom );
                  if( lpnmhdr->hwndFrom == ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ) )
                  {
                     mw->DrawOnAddSpriteDialog( mw->m_AddSpriteDialogStaticDCs.m_GetDC );
                     TCITEM item;
                     ::ZeroMemory( &item, sizeof( item ) );
                     item.mask    = TCIF_TEXT;
                     char buffer[ MAX_PATH ];
                     buffer[ MAX_PATH-1 ] = '\0';
                     item.pszText    = buffer;
                     item.cchTextMax = MAX_PATH;
                     TabCtrl_GetItem( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ), iPage, &item );
                     ::SetWindowText( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_BROWSE ), item.pszText );
                     ::UpdateWindow( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_BROWSE ) );
                     std::ostringstream frame;
                     frame << iPage;
                     ::SetWindowText( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_FRAME ), frame.str().c_str() );
                     ::UpdateWindow(  ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_FRAME ) );
                     if( mw->m_TileMaps.size() > mw->m_CurrentMap )
                     {
                        std::deque< C_Sprite >& Sprites = mw->m_TileMaps[ mw->m_CurrentMap ].GetSprites();
                        if( Sprites.size() )
                        {
                           int index = iPage;
                           std::deque< C_Sprite::S_Frame >& Frames = Sprites.back().GetFrames();
                           if( Frames.size() > index )
                           {
                              std::ostringstream x_pos, y_pos;
                              x_pos << Sprites.back().GetPosition().x;
                              y_pos << Sprites.back().GetPosition().y;
                              ::SetWindowText( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_X ), x_pos.str().c_str() );
                              ::UpdateWindow(  ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_X ) );

                              ::SetWindowText( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_Y ), y_pos.str().c_str() );
                              ::UpdateWindow(  ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_Y ) );

                              std::ostringstream duration;
                              duration << Frames[index].m_duration;

                              ::SetWindowText( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_DELAY ), duration.str().c_str() );
                              ::UpdateWindow(  ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_DELAY ) );

                              DWORD dwPos = ::SendMessage( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_TRACKBAR_AVAILABLEIMAGES ), 
                                                           TBM_SETPOS, 0, Frames[index].m_index );
                           }
                        }
                     }
                  }
               }
            } break;
         }
      } break;
      case WM_COMMAND:
      {
         switch( LOWORD( wParam ) )
         {
            case ID_ADDSPRITE_BUTTON_PLAY:
            {
               if( mw )
               {
                  mw->m_AddSpriteDrawTimer.Set( mw->m_WinBase, ADD_SPRITE_DRAW_TIMER, 200, NULL );
               }
            } break;
            case ID_ADDSPRITE_BUTTON_PAUSE:
            {
               if( mw )
               {
                  mw->m_AddSpriteDrawTimer.Kill();
               }
            } break;
            case ID_ADDSPRITE_BUTTON_STOP:
            {
               if( mw )
               {
                  mw->m_AddSpriteDrawTimer.Kill();
               }
            } break;
            case ID_ADDSPRITE_BUTTON_OK:
            {
               ::EndDialog( hwnd, 0 );
               if( mw )
               {
                  if( mw->m_TileMaps.size() > mw->m_CurrentMap )
                  {
                     SpriteMap::D_Sprite& spritemap = mw->m_TileMaps[ mw->m_CurrentMap ].GetSpriteIDs();
                     std::deque< C_Sprite >& Sprites = mw->m_TileMaps[ mw->m_CurrentMap ].GetSprites();
                     spritemap.push_back( SpriteMap::SpriteID( Sprites.size()-1, Sprites.back().GetPosition() ) );
                  }
               }
            } break;
            case ID_ADDSPRITE_BUTTON_CANCEL:
            {
               ::EndDialog( hwnd, 0 );
               if( mw )
               {
                  if( mw->m_TileMaps.size() > mw->m_CurrentMap )
                  {
                     mw->m_TileMaps[ mw->m_CurrentMap ].RemoveSprite();
                  }
               }
            } break;
            case ID_ADDSPRITE_BUTTON_USETHISIMAGE:
            {
               if( mw )
               {
                  mw->AddSpriteDlgMakeNewTab();
               }
            } break;
            case ID_ADDSPRITE_CHECK_LOOP:
            {
               if( mw )
               {
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_CHECK_LOOP ) );
                  mw->m_bLoopSpriteAnimation = ::SendMessage( ::GetDlgItem( mw->m_HwndAddSpriteDlg, ID_ADDSPRITE_CHECK_LOOP ), BM_GETCHECK, 0, 0 );
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

bool MainWindow::AddSpriteDlgMakeNewTab( void )
{
   if( m_TileMaps.size() > m_CurrentMap )
   {
      std::ostringstream temp;
      int img_index = ::SendMessage( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_TRACKBAR_AVAILABLEIMAGES ), 
                                 TBM_GETPOS, 0, 0 );
      temp << img_index;
      int index = TabCtrl_GetItemCount( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ) );
      TCITEM item;
      ::ZeroMemory( &item, sizeof(item) );
      item.mask    = TCIF_TEXT;
      item.pszText = (char*)temp.str().c_str();
      if( TabCtrl_InsertItem( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ), index, &item ) == -1 )
         return false;
      std::deque< C_Sprite >& Sprites = m_TileMaps[ m_CurrentMap ].GetSprites();
      if( Sprites.size() )
      {
         Sprites.back().AddFrame();
         std::deque< C_Sprite::S_Frame >& Frames = Sprites.back().GetFrames();
         if( Frames.size() )
         {
            Sprites.back().SetPosition( m_TileAddSpriteTile );
            Frames.back().m_index = img_index;
            Frames.back().m_duration = 1;
            DrawOnAddSpriteDialog( m_AddSpriteDialogStaticDCs.m_GetDC );

            std::ostringstream duration;
            duration << Frames.back().m_duration;
            ::SetWindowText( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_DELAY ), duration.str().c_str() );
            ::UpdateWindow(  ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_DELAY ) );

            HWND hwndTrackFrame = ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_TRACKBAR_FRAME );
            // range
            ::SendMessage( hwndTrackFrame, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, Frames.size()-1 ) );
            // page size
            ::SendMessage( hwndTrackFrame, TBM_SETPAGESIZE, 0, (LPARAM) (Frames.size()/10) );
            // 
            ::SendMessage( hwndTrackFrame, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0, Frames.size()-1 ) );
            // Position
            ::SendMessage( hwndTrackFrame, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) Frames.size()-1 );

            TabCtrl_SetCurFocus( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ), 
                                 TabCtrl_GetItemCount( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ) )-1 );
            DrawOnAddSpriteDialog( m_AddSpriteDialogStaticDCs.m_GetDC );
         }
      }
   }
   return true;
}

bool MainWindow::ShowAddSpriteDlg( void )
{
   m_TileAddSpriteTile       = GetMapTile( m_RButtonDownStartPoint );
   if( m_TileAddSpriteTile < 0 )
   {
      return true;
   }
   bool        ret = false;
   if( m_TileMaps.size() )
   {
      m_HwndAddSpriteDlg = CreateDialog( ::GetModuleHandle( NULL ), 
                                         MAKEINTRESOURCE( ID_ADDSPRITE_DIALOG ), 
                                         m_WinBase, 
                                         AddSpriteDlgProc );
      if( m_HwndAddSpriteDlg != NULL )
      {
         ::ShowWindow(    m_HwndAddSpriteDlg, SW_SHOW );
         ::SetWindowLong( m_HwndAddSpriteDlg, GWL_USERDATA, (LPARAM)this );

         if( m_TileMaps.size() > m_CurrentMap )
         {
            m_TileMaps[ m_CurrentMap ].AddSprite();
            AddSpriteDlgMakeNewTab();
         }
         else
         {
            return true;
         }

         m_AddSpriteDialogStaticDCs.SetHwnd( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_STATIC ) );

         std::ostringstream tile_x, tile_y;
         tile_x << m_TileAddSpriteTile.x;
         tile_y << m_TileAddSpriteTile.y;
         ::SetWindowText( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_X ), tile_x.str().c_str() );
         ::SetWindowText( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_Y ), tile_y.str().c_str() );
         ::UpdateWindow(  ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_X ) );
         ::UpdateWindow(  ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_Y ) );

         std::ostringstream sprite_index;
         sprite_index << m_TileMaps[ m_CurrentMap ].GetSprites().size()-1;
         ::SetWindowText( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_SPRITEINDEX ), sprite_index.str().c_str() );
         ::UpdateWindow(  ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_SPRITEINDEX ) );

         std::ostringstream frame;
         frame << 0;
         ::SetWindowText( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_FRAME ), frame.str().c_str() );
         ::UpdateWindow(  ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_FRAME ) );

         std::ostringstream cycle_length;
         cycle_length << 0;
         ::SetWindowText( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_CYCLELENGTH ), cycle_length.str().c_str() );
         ::UpdateWindow(  ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_CYCLELENGTH ) );

         std::ostringstream delay;
         delay << 0;
         ::SetWindowText( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_DELAY ), delay.str().c_str() );
         ::SendMessage( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_DELAY ), EM_LIMITTEXT, 3, 0 );
         ::UpdateWindow( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_DELAY ) );

         ::SendMessage( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_UPDOWN_DELAY ), UDM_SETRANGE, 0, (LPARAM) MAKELONG((short) 1000, (short) 0) );
         ::SendMessage( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_UPDOWN_DELAY ), UDM_SETBUDDY, (WPARAM)::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_DELAY ), 0 );

         std::ostringstream zoom;
         zoom << m_AddSpriteZoom;
         ::SetWindowText( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_ZOOM ), zoom.str().c_str() );
         ::SendMessage( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_ZOOM ), EM_LIMITTEXT, 2, 0 );
         ::UpdateWindow( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_ZOOM ) );

         ::SendMessage( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_UPDOWN_ZOOM ), UDM_SETRANGE, 0, (LPARAM) MAKELONG((short) 10, (short) 0) );
         ::SendMessage( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_UPDOWN_ZOOM ), UDM_SETBUDDY, (WPARAM)::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_EDIT_ZOOM ), 0 );

         HWND hwndTrackFrame = ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_TRACKBAR_FRAME );
         // range
         ::SendMessage( hwndTrackFrame, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, 0 ) );
         // page size
         ::SendMessage( hwndTrackFrame, TBM_SETPAGESIZE, 0, (LPARAM) 0 );
         // 
         ::SendMessage( hwndTrackFrame, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0, 0 ) );
         // Position
         ::SendMessage( hwndTrackFrame, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) 0 );


         SpriteMap::D_HBitmaps& Images = m_TileMaps[ m_CurrentMap ].GetSpriteBitmaps();
         int img_index = Images.size()-1;
         HWND hwndTrackImage = ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_TRACKBAR_AVAILABLEIMAGES );
         // range
         ::SendMessage( hwndTrackImage, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, img_index ) );
         // page size
         ::SendMessage( hwndTrackImage, TBM_SETPAGESIZE, 0, (LPARAM) (img_index/10) );
         // 
         ::SendMessage( hwndTrackImage, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0, img_index ) );
         // Position
         ::SendMessage( hwndTrackImage, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) 0 );


         ::SendMessage( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_CHECK_LOOP ), BM_SETCHECK, m_bLoopSpriteAnimation, 0 );
         ::UpdateWindow( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_CHECK_LOOP ) );

         ::UpdateWindow( m_HwndAddSpriteDlg );
         DrawOnAddSpriteDialog( m_AddSpriteDialogStaticDCs.m_GetDC );
         ret = true;
      }
      else
      {
         ::Alert( GetLastSystemError() + " CreateDialog failed" );
      }
   }
   return ret;
}

void MainWindow::DrawOnAddSpriteDialog( C_HDC& hdc )
{
   if( m_TileMaps.size() > m_CurrentMap )
   {
      std::deque< C_Sprite >& Sprites = m_TileMaps[ m_CurrentMap ].GetSprites();
      if( Sprites.size() )
      {
         SpriteMap::D_HBitmaps& Images = m_TileMaps[ m_CurrentMap ].GetSpriteBitmaps();
         int index        = ::SendMessage( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_TAB ), TBM_GETPOS, 0, 0 );
         std::deque< C_Sprite::S_Frame >& Frames = Sprites.back().GetFrames();
         int frame_index = ::SendMessage( ::GetDlgItem( m_HwndAddSpriteDlg, ID_ADDSPRITE_TRACKBAR_AVAILABLEIMAGES ),
                                          TBM_GETPOS, 0, 0 );
         if( frame_index < Images.size() )
         {
            int width  = Images[frame_index].Width();
            int height = Images[frame_index].Height();
            Images[frame_index].StretchDisplay( hdc, 0, 0, m_AddSpriteZoom*width, m_AddSpriteZoom*height, SRCCOPY );
         }
      }
   }
}

