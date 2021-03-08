#include "main.h"

BOOL CALLBACK MainWindow::DeleteSpriteDlgProc(   HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
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
            mw->m_DeleteSpriteDialogStaticDCs.m_PaintDC.Begin();
            mw->DrawOnDeleteSpriteDialog( mw->m_DeleteSpriteDialogStaticDCs.m_PaintDC );
            mw->m_DeleteSpriteDialogStaticDCs.m_PaintDC.End();
         }
      } break;
	   case WM_HSCROLL:
      {
         if( mw )
         {
            HWND hwnd = (HWND) lParam;
            if( hwnd == ::GetDlgItem( mw->m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_TRACKBAR_SPRITE_INDEX ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, TBM_GETPOS, 0, 0 );
               std::ostringstream temp;
               temp << dwPos;
               ::SetWindowText( ::GetDlgItem( mw->m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_EDIT_SPRITE_INDEX ), temp.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_EDIT_SPRITE_INDEX ) );

               std::deque< C_Sprite >& Sprites = mw->m_TileMaps[ mw->m_CurrentMap ].GetSprites();
               int index = dwPos;
               if( mw->m_TileMaps[ mw->m_CurrentMap ].GetSprites().size() > index )
               {
                  HWND hwndTrackSprites = ::GetDlgItem( mw->m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_TRACKBAR_FRAME_INDEX );
                  // range
                  ::SendMessage( hwndTrackSprites, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, Sprites[index].GetFrames().size()-1 ) );
                  // page size
                  ::SendMessage( hwndTrackSprites, TBM_SETPAGESIZE, 0, (LPARAM) (Sprites[index].GetFrames().size()-1/10) );
                  // 
                  ::SendMessage( hwndTrackSprites, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0,Sprites[index].GetFrames().size()-1 ) );
                  // Position
                  ::SendMessage( hwndTrackSprites, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) 0 );
               }
            }
            if( hwnd == ::GetDlgItem( mw->m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_TRACKBAR_FRAME_INDEX ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, TBM_GETPOS, 0, 0 );
               std::ostringstream temp;
               temp << dwPos;
               ::SetWindowText( ::GetDlgItem( mw->m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_EDIT_FRAME_INDEX ), temp.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_EDIT_FRAME_INDEX ) );
            }
            mw->DrawOnDeleteSpriteDialog( mw->m_DeleteSpriteDialogStaticDCs.m_GetDC );
         }
      } break;
      case WM_COMMAND:
      {
         switch( LOWORD( wParam ) )
         {
            case ID_DELETESPRITE_DIALOG_BUTTON_DELETE:
            {
//               ::EndDialog( hwnd, 0 );
               if( mw )
               {
                  DWORD index = ::SendMessage( ::GetDlgItem( mw->m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_TRACKBAR_SPRITE_INDEX ), TBM_GETPOS, 0, 0 );
                  std::deque< C_Sprite >& Sprites = mw->m_TileMaps[ mw->m_CurrentMap ].GetSprites();
                  if( index < Sprites.size() )
                  {
                     Sprites.erase( Sprites.begin()+index );
                  }
                  mw->DrawOnDeleteSpriteDialog( mw->m_DeleteSpriteDialogStaticDCs.m_GetDC );
               }
            } break;
         }
         default:
         {
            return FALSE;
         }
      }
   }
   return FALSE;
}

bool MainWindow::ShowDeleteSpriteDlg( void )
{
   if( !(m_TileMaps.size() > m_CurrentMap) )
   {
      return true;
   }
   bool        ret = false;
   m_HwndDeleteSpriteDlg = CreateDialog( ::GetModuleHandle( NULL ), 
                                         MAKEINTRESOURCE( ID_DELETESPRITE_DIALOG ), 
                                         m_WinBase, 
                                         DeleteSpriteDlgProc );
   if( m_HwndDeleteSpriteDlg != NULL )
   {
      ::ShowWindow(    m_HwndDeleteSpriteDlg, SW_SHOW );
      ::SetWindowLong( m_HwndDeleteSpriteDlg, GWL_USERDATA, (LPARAM)this );
      if( !(m_TileMaps.size() > m_CurrentMap) )
      {
         return true;
      }

      m_DeleteSpriteDialogStaticDCs.SetHwnd( ::GetDlgItem( m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_STATIC ) );

      HWND hwndTrackSprites = ::GetDlgItem( m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_TRACKBAR_SPRITE_INDEX );      
      // range
      ::SendMessage( hwndTrackSprites, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, m_TileMaps[ m_CurrentMap ].GetSprites().size()-1 ) );
      // page size
      ::SendMessage( hwndTrackSprites, TBM_SETPAGESIZE, 0, (LPARAM) (m_TileMaps[ m_CurrentMap ].GetSprites().size()-1/10) );
      // 
      ::SendMessage( hwndTrackSprites, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0, m_TileMaps[ m_CurrentMap ].GetSprites().size()-1 ) );
      // Position
      ::SendMessage( hwndTrackSprites, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) 0 );

      std::deque< C_Sprite >& Sprites = m_TileMaps[ m_CurrentMap ].GetSprites();
      int index = 0;

      if( m_TileMaps[ m_CurrentMap ].GetSprites().size() > index )
      {
         HWND hwndTrackSprites = ::GetDlgItem( m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_TRACKBAR_FRAME_INDEX );
         // range
         ::SendMessage( hwndTrackSprites, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, Sprites[index].GetFrames().size()-1 ) );
         // page size
         ::SendMessage( hwndTrackSprites, TBM_SETPAGESIZE, 0, (LPARAM) (Sprites[index].GetFrames().size()-1/10) );
         // 
         ::SendMessage( hwndTrackSprites, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0,Sprites[index].GetFrames().size()-1 ) );
         // Position
         ::SendMessage( hwndTrackSprites, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) 0 );
      }
      else
      {
         ::EndDialog( m_HwndDeleteSpriteDlg, 0 );
         return true;
      }
      std::ostringstream sprite_index;
      sprite_index << 0;
      ::SetWindowText( ::GetDlgItem( m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_EDIT_SPRITE_INDEX ), sprite_index.str().c_str() );
      ::SendMessage( ::GetDlgItem( m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_EDIT_SPRITE_INDEX ), EM_LIMITTEXT, 2, 0 );
      ::UpdateWindow( ::GetDlgItem( m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_EDIT_SPRITE_INDEX ) );

      std::ostringstream frame_index;
      frame_index << 0;
      ::SetWindowText( ::GetDlgItem( m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_EDIT_FRAME_INDEX ), frame_index.str().c_str() );
      ::SendMessage( ::GetDlgItem( m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_EDIT_FRAME_INDEX ), EM_LIMITTEXT, 2, 0 );
      ::UpdateWindow( ::GetDlgItem( m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_EDIT_FRAME_INDEX ) );

      DrawOnDeleteSpriteDialog( m_DeleteSpriteDialogStaticDCs.m_GetDC );
      ret = true;
   }
   else
   {
      ::Alert( GetLastSystemError() + " CreateDialog failed" );
   }
   return ret;
}

void MainWindow::DrawOnDeleteSpriteDialog( C_HDC& hdc )
{
   if( m_TileMaps.size() > m_CurrentMap )
   {
      std::deque< C_Sprite >& Sprites = m_TileMaps[ m_CurrentMap ].GetSprites();
      int sprite_index = ::SendMessage( ::GetDlgItem( m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_TRACKBAR_SPRITE_INDEX ), TBM_GETPOS, 0, 0 );
      int index        = ::SendMessage( ::GetDlgItem( m_HwndDeleteSpriteDlg, ID_DELETESPRITE_DIALOG_TRACKBAR_FRAME_INDEX ), TBM_GETPOS, 0, 0 );
      std::deque< C_Sprite::S_Frame >& Frames = Sprites[sprite_index].GetFrames();
      if( Sprites.size() )
      {
         SpriteMap::D_HBitmaps& Images           = m_TileMaps[ m_CurrentMap ].GetSpriteBitmaps();
         std::deque< C_Sprite::S_Frame >& Frames = Sprites[sprite_index].GetFrames();
         if( index < Images.size() )
         {
            int width  = Images[index].Width();
            int height = Images[index].Height();
            Images[index].StretchDisplay( hdc, 0, 0, m_AddSpriteZoom*width, m_AddSpriteZoom*height, SRCCOPY );
         }
      }
   }
}

