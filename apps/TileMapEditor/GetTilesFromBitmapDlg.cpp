#include "main.h"

BOOL CALLBACK MainWindow::GetTilesFromBitmapDlg( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
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
         if( mw )
         {
            mw->DrawBitmapInGetTilesFromBitmapDlg( mw->m_GetTilesFromBitmapDlgDCs.m_GetDC );
         }
      } break;
	   case WM_VSCROLL:
      {
         if( mw )
         {
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
         if( mw )
         {
            mw->DrawBitmapInGetTilesFromBitmapDlg( mw->m_GetTilesFromBitmapDlgDCs.m_GetDC );
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
            mw->m_GetTilesFromBitmapDlgDCs.m_PaintDC.Begin();
            mw->DrawBitmapInGetTilesFromBitmapDlg( mw->m_GetTilesFromBitmapDlgDCs.m_PaintDC );
            mw->m_GetTilesFromBitmapDlgDCs.m_PaintDC.End();
         }
      } break;
	   case WM_CLOSE:
      {
         ::EndDialog( hwnd, 0 );
      } break;
      case WM_SIZE:
      {
      } break;
      case WM_COMMAND:
      {
         switch( LOWORD( wParam ) )
         {
            case ID_GETTILESFROMBITMAP_CHECK_GRID:
            {
               if( mw )
               {
                  ::UpdateWindow( ::GetDlgItem( mw->m_HwndGetTilesFromBitmapDlg, ID_GETTILESFROMBITMAP_CHECK_GRID ) );
                  if( mw->m_TileMaps.size() > mw->m_CurrentMap )
                  {
//::SendMessage( ::GetDlgItem( mw->m_HwndGetTilesFromBitmapDlg, ID_GETTILESFROMBITMAP_CHECK_GRID ), BM_GETCHECK, 0, 0 ) );
                     mw->DrawMap( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                     mw->DrawAStarNodes( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                     mw->DrawTileSet( mw->m_CurrentMap, mw->m_TileSetsStaticDCs.m_GetDC );
                     mw->DrawCurrentTile( mw->m_CurrentMap, mw->m_TileSetsStaticDCs2.m_GetDC );
                  }
               }
            } break;

            case ID_GETTILESFROMBITMAP_BUTTON_OK:
            {
               ::EndDialog( hwnd, 0 );
               if( mw )
               {
                  mw->RipTiles( Structs::Point(32,32) );
                  mw->ResizeTileSetsIDs();
                  mw->DrawMap( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                  mw->DrawAStarNodes( mw->m_CurrentMap, mw->m_MapsStaticDCs.m_GetDC );
                  mw->DrawTileSet( mw->m_CurrentMap, mw->m_TileSetsStaticDCs.m_GetDC );
                  mw->DrawCurrentTile( mw->m_CurrentMap, mw->m_TileSetsStaticDCs2.m_GetDC );
               }
            } break;
            case ID_GETTILESFROMBITMAP_BUTTON_CANCEL:
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

bool MainWindow::ShowGetTilesFromBitmapDlg( void )
{
   bool        ret = false;
   if( m_TileMaps.size()  )
   {
      std::string FileName;
      m_WinFileHandler.OpenFile( FileName, "Bitmap Files *.bmp\0*.bmp\0All files *.*\0*.*\0\0",
                                 ".bmp", 0, 0, "Open" );
      if( !FileName.size() )
         return false;
      m_HwndGetTilesFromBitmapDlg = ::CreateDialog( ::GetModuleHandle( NULL ), 
                                                    MAKEINTRESOURCE( ID_GETTILESFROMBITMAP_DIALOG ), 
                                                    m_WinBase, 
                                                    GetTilesFromBitmapDlg );
      if( m_HwndGetTilesFromBitmapDlg != NULL )
      {
         ::ShowWindow(    m_HwndGetTilesFromBitmapDlg, SW_SHOW );
         ::SetWindowLong( m_HwndGetTilesFromBitmapDlg, GWL_USERDATA, (LPARAM)this );
         if( !m_GetTilesBitmap.Load( FileName ) )
         {
            Alert( "Could not load file: " + FileName );
         }

         m_GetTilesBitmap.SetHwnd( ::GetDlgItem( m_HwndGetTilesFromBitmapDlg, ID_GETTILESFROMBITMAP_STATIC ) );
         m_GetTilesFromBitmapDlgDCs.SetHwnd( ::GetDlgItem( m_HwndGetTilesFromBitmapDlg, ID_GETTILESFROMBITMAP_STATIC ) );
         DrawBitmapInGetTilesFromBitmapDlg( m_GetTilesFromBitmapDlgDCs.m_GetDC );

         ::SetWindowPos( m_HwndGetTilesFromBitmapDlg, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
         ret = true;
      }
      else
      {
         ::Alert( GetLastSystemError() + " CreateDialog failed" );
      }
   }
   return ret;
}

void MainWindow::DrawBitmapInGetTilesFromBitmapDlg( C_HDC & hdc )
{
   hdc.Clear();
//   m_GetTilesBitmap.Display( hdc, 0, 0 );

//   CHBITMAP bitmap;
//   HWND          Hwnd = ::GetDlgItem( m_HwndGetTilesFromBitmapDlg, ID_GETTILESFROMBITMAP_STATIC );
//   Structs::Rect rect( 0, 0, 32, 32 );
//   bitmap.GetRect( m_GetTilesBitmap, rect, Hwnd );
//   bitmap.Display( hdc, 0, 0 );
   if( ::SendMessage( ::GetDlgItem( m_HwndGetTilesFromBitmapDlg, ID_GETTILESFROMBITMAP_CHECK_GRID ), BM_GETCHECK, 0, 0 ) )
   {
      DrawGridInGetTilesFromBitmapDlg( hdc );
   }
   
   m_GetTilesBitmap.Display( hdc, 0, 0 );
}

void MainWindow::DrawGridInGetTilesFromBitmapDlg( C_HDC & hdc )
{
/*
   int width=;
   int height=;
   int max_rows =;
   int max_cols =;
   for( int r=0 ; r< ; r++ )
   {
   }
   for( int c=0; c< ; c++ )
   {
   }
*/
}

void MainWindow::RipTiles( Structs::Point tile_dimensions )
{
   if( !m_TileMaps.size() )
   {
   }
   int max_width  = m_GetTilesBitmap.Width()/tile_dimensions.x;
   int max_height = m_GetTilesBitmap.Height()/tile_dimensions.y;
   Structs::Rect rect( 0, 0, tile_dimensions.x, tile_dimensions.y );
   HWND          Hwnd = ::GetDlgItem( m_HwndGetTilesFromBitmapDlg, ID_GETTILESFROMBITMAP_STATIC );
   std::string prefix = ::GetDlgItemText( m_HwndGetTilesFromBitmapDlg, ID_GETTILESFROMBITMAP_EDIT_PREFIX );
//   ::GetDlgItemText( m_HwndGetTilesFromBitmapDlg, ID_GETTILESFROMBITMAP_EDIT_INDEX );
   std::ostringstream path;
   int index = m_TileMaps[ m_CurrentMap ].Bitmaps().size()+1;
   CHBITMAP bitmap;
//   char buffer[MAX_PATH];
//   Alert( ::getcwd( buffer, MAX_PATH-1) );
   for( int i=0 ; i<max_width ; ++i )
   {
      for( int c=0 ; c<max_height ; ++c )
      {
         bitmap.GetRect( m_GetTilesBitmap, rect, Hwnd );
         path << m_TileMaps[ m_CurrentMap ].GetTileSetWorkingDirectory().c_str() << prefix << index << ".bmp";
         bitmap.Save( path.str() );
         m_TileMaps[ m_CurrentMap ].AddTile( path.str() );
         index++;
         path.str(std::string());
         rect.top    += tile_dimensions.y;
         rect.bottom += tile_dimensions.y;
      }
      rect.left   += tile_dimensions.x;
      rect.right  += tile_dimensions.x;
      rect.top    = 0;
      rect.bottom = tile_dimensions.y;
   }
}

