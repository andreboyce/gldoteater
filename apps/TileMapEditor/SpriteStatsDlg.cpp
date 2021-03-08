#include "main.h"

BOOL CALLBACK MainWindow::SpriteStatsDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
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
            mw->m_SpriteStatsDlgDCs.m_PaintDC.Begin();
            mw->DrawOnSpriteStatsDialog( mw->m_SpriteStatsDlgDCs.m_PaintDC );
            mw->m_SpriteStatsDlgDCs.m_PaintDC.End();
         }
      } break;
	   case WM_HSCROLL:
      {
         if( mw )
         {
            HWND hwnd = (HWND) lParam;
            if( hwnd == ::GetDlgItem( mw->m_HwndSpriteStatsDlg, ID_SPRITESTATS_DIALOG_TRACK_IMGID ) )
            {
               DWORD dwPos = ::SendMessage( hwnd, TBM_GETPOS, 0, 0 );
               std::ostringstream temp;
               temp << dwPos;
               mw->DrawOnSpriteStatsDialog( mw->m_SpriteStatsDlgDCs.m_GetDC );
               ::SetWindowText( ::GetDlgItem( mw->m_HwndSpriteStatsDlg, ID_SPRITESTATS_DIALOG_EDIT_IMGID ), temp.str().c_str() );
               ::UpdateWindow( ::GetDlgItem( mw->m_HwndSpriteStatsDlg, ID_SPRITESTATS_DIALOG_EDIT_IMGID ) );
            }
         }
      } break;
      case WM_COMMAND:
      {
         switch( LOWORD( wParam ) )
         {
            case ID_SPRITESTATS_DIALOG_BUTTON_OK:
            {
               ::EndDialog( hwnd, 0 );
               if( mw )
               {
               }
            } break;
            case ID_SPRITESTATS_DIALOG_BUTTON_CANCEL:
            {
               ::EndDialog( hwnd, 0 );
               if( mw )
               {
               }
            } break;
            case ID_SPRITESTATS_DIALOG_BUTTON_REPLACEINDEX:
            {
               if( mw )
               {
                  std::string spriteframefilename;
                  mw->m_WinFileHandler.OpenFile( spriteframefilename, "Bitmap Files *.bmp\0*.bmp\0All files *.*\0*.*\0\0",
                                                 ".bmp", 0, 0, "Open" );
                  if( spriteframefilename.size() )
                  {
                     SpriteMap::D_HBitmaps& Images = mw->m_TileMaps[ mw->m_CurrentMap ].GetSpriteBitmaps();
                     int index = ::SendMessage( ::GetDlgItem( mw->m_HwndSpriteStatsDlg, ID_SPRITESTATS_DIALOG_TRACK_IMGID ), 
                                                TBM_GETPOS, 0, 0 );
                     if( index < Images.size() )
                     {
                        Images[index].Load( spriteframefilename );
                        Images[index].SetHwnd( (HWND)mw->m_WinBase );
                     }
                  }
                  ::UpdateWindow( mw->m_HwndSpriteStatsDlg );
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

bool MainWindow::ShowSpriteStatsDlg( void )
{
   if( !(m_TileMaps.size() > m_CurrentMap) )
      return true;
   bool        ret = false;
   m_HwndSpriteStatsDlg = CreateDialog( ::GetModuleHandle( NULL ), 
                                        MAKEINTRESOURCE( ID_SPRITESTATS_DIALOG ), 
                                        m_WinBase, 
                                        SpriteStatsDlgProc );

   if( m_HwndSpriteStatsDlg != NULL )
   {
      ::ShowWindow(    m_HwndSpriteStatsDlg, SW_SHOW );
      ::SetWindowLong( m_HwndSpriteStatsDlg, GWL_USERDATA, (LPARAM)this );

      m_SpriteStatsDlgDCs.SetHwnd( ::GetDlgItem( m_HwndSpriteStatsDlg, ID_SPRITESTATS_DIALOG_STATIC ) );

      SpriteMap::D_HBitmaps& Images = m_TileMaps[ m_CurrentMap ].GetSpriteBitmaps();
      int max_index = Images.size()-1;

      std::ostringstream img_index;
      img_index << 0;
      ::SetWindowText( ::GetDlgItem( m_HwndSpriteStatsDlg, ID_SPRITESTATS_DIALOG_EDIT_IMGID ), img_index.str().c_str() );
      ::UpdateWindow(  ::GetDlgItem( m_HwndSpriteStatsDlg, ID_SPRITESTATS_DIALOG_EDIT_IMGID ) );

      HWND hwndTrackFrame = ::GetDlgItem( m_HwndSpriteStatsDlg, ID_SPRITESTATS_DIALOG_TRACK_IMGID );
      // range
      ::SendMessage( hwndTrackFrame, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( 0, max_index ) );
      // page size
      ::SendMessage( hwndTrackFrame, TBM_SETPAGESIZE, 0, (LPARAM) (max_index/10) );
      // 
      ::SendMessage( hwndTrackFrame, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG( 0, max_index ) );
      // Position
      ::SendMessage( hwndTrackFrame, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) 0 );

      ::UpdateWindow( m_HwndSpriteStatsDlg );
      ::UpdateWindow( ::GetDlgItem( m_HwndSpriteStatsDlg, ID_SPRITESTATS_DIALOG_TRACK_IMGID ) );
      DrawOnSpriteStatsDialog( m_SpriteStatsDlgDCs.m_GetDC );

      ret = true;
   }
   else
   {
      ::Alert( GetLastSystemError() + " CreateDialog failed" );
   }
   return ret;
}

void MainWindow::DrawOnSpriteStatsDialog( C_HDC& hdc )
{
   if( m_TileMaps.size() > m_CurrentMap )
   {
      SpriteMap::D_HBitmaps& Images = m_TileMaps[ m_CurrentMap ].GetSpriteBitmaps();
      DWORD index = ::SendMessage( ::GetDlgItem( m_HwndSpriteStatsDlg, ID_SPRITESTATS_DIALOG_TRACK_IMGID ), 
                                   TBM_GETPOS, 0, 0 );
      if( index < Images.size() )
      {
         int width  = Images[index].Width();
         int height = Images[index].Height();
         Images[index].StretchDisplay( hdc, 0, 0, width, height, SRCCOPY );
      }
   }
}
