#include "main.h"

LRESULT MainWindow::MWBaseOnCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( LOWORD(wparam) )
   {
      case ID_FILE_OPEN:
      {
         std::string spriteframefilename;
         mw.m_WinFileHandler.OpenFile( spriteframefilename, "Binary TileMap Files *.btm\0*.btm\0All files *.*\0*.*\0\0",
                                       ".btm", 0, 0, "Open" );
         if( spriteframefilename.size() )
         {
            if( mw.LoadMap( spriteframefilename ) )
            {
               mw.Reshape( mw.m_WinBase.GetClientRect().right-mw.m_WinBase.GetClientRect().left, 
                           mw.m_WinBase.GetClientRect().bottom-mw.m_WinBase.GetClientRect().top );
               DestroyWindow( mw.m_HwndShowSpecialDataTypesDialog );
               if( mw.m_HwndShowSpecialDataTypesDialog == NULL )
                  mw.ShowSpecialDataTypesDialog();
            }
         }
      } break;
      case ID_FILE_SAVE:
      {
            mw.SaveMap( mw.m_MapFileName );
      } break;
      case ID_FILE_SAVEAS:
      {
         std::string spriteframefilename;
         mw.m_WinFileHandler.SaveFile( spriteframefilename, "Binary TileMap Files *.btm\0*.btm\0All files *.*\0*.*\0\0",
                                       ".btm", 0, 0, "Open" );
         if( spriteframefilename.size() )
         {
            mw.SaveMap( spriteframefilename );
            mw.Reshape( mw.m_WinBase.GetClientRect().right-mw.m_WinBase.GetClientRect().left, 
                        mw.m_WinBase.GetClientRect().bottom-mw.m_WinBase.GetClientRect().top );
         }
      } break;
      case ID_FILE_EXIT:
      {
           mw.m_bQuit = true;
      } break;
      case ID_SDTS_VIEWTYPES:
      {
         mw.ShowSpecialDataTypesDialog();
      } break;
      case ID_SDTS_ADD:
      {
         mw.ShowSpecialDataTypesDialog();
         Structs::Point tile = mw.GetTile( mw.m_GLMousePosition );
         if( mw.m_TileMap.m_SpecialDataTypes.size() )
         {
            mw.ShowTilePropertiesDlg();
         }
      } break;
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnMouseMove( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   ::GetCursorPos( &mw.m_MousePosition );
   ::ScreenToClient( mw.m_WinBase, &mw.m_MousePosition );
   mw.m_MouseSStream.str( std::string() );
   Vector3D temp = WindowToGLCoordinates( mw.m_MousePosition.x, mw.m_MousePosition.y );

   mw.m_GLMousePosition.m_x = temp[0];
   mw.m_GLMousePosition.m_y = temp[1];
   mw.m_GLMousePosition.m_z = temp[2];
   mw.m_MouseSStream << "{ " << mw.m_GLMousePosition.m_x << ", " << mw.m_GLMousePosition.m_y << ", " << mw.m_GLMousePosition.m_z << " }";

   return ::DefWindowProc( mw.m_LastHwndPassed, WM_MOUSEMOVE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnLDoubleClick( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDBLCLK, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRDoubleClick(     MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONDBLCLK, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_MapEditTrackPopupMenu.Display( mw, mw.m_MousePosition.x, mw.m_MousePosition.y );
   mw.m_GLRButtonDownStartPoint = mw.m_GLMousePosition;
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONDOWN, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRButtonU( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONUP, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnLButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDOWN, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnLButtonU( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONUP, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnCreate( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_CREATE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( wparam )
   {
      case SIZE_MINIMIZED:
      {
         mw.m_bisVisible = false;
      } break;
      case SIZE_MAXIMIZED:
      {
         mw.m_bisVisible        = true;
         mw.Reshape( LOWORD(lparam), HIWORD(lparam) );
      } break;

      case SIZE_RESTORED:
      {
         mw.m_bisVisible        = true;
         mw.Reshape( LOWORD(lparam), HIWORD(lparam) );
      } break;
      default:
      {
         mw.m_bisVisible        = true;
         mw.Reshape( LOWORD(lparam), HIWORD(lparam) );
      } break;
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnNotify( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   NMHDR *lpnmhdr = (NMHDR*)(lparam);
   switch( lpnmhdr->code )
   {
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_NOTIFY, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnHScroll( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{   
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_HSCROLL, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnVScroll( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_VSCROLL, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnKeyDown( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYDOWN, wparam, lparam );
}     

LRESULT MainWindow::MWBaseOnKeyUp( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   if( wparam == VK_F12 )
   {
      ::PostMessage( mw, WM_TOGGLEFULLSCREEN, 0, 0 );
   }
   if( wparam == VK_ESCAPE )
   {
      mw.m_bQuit = true;
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYUP, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnPaint( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_PAINT, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnTimer( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( wparam )
   {
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_TIMER, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnKillFocus( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KILLFOCUS, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnSetFocus( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_SETFOCUS, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnClose( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_bQuit = true;
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_CLOSE, wparam, lparam );
}

//***************************************************************************
//******************************* MainWindow::MWBaseOnToggleFullScreen ******
// Accepts: void
//
// Purpose: switch between fullscreen and windowed mode
//
// Returns: void

LRESULT MainWindow::MWBaseOnToggleFullScreen( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   if( mw.m_bFullScreen ) // go to windowed mode
   {
      int x_offset = (int)(::GetSystemMetrics( SM_CXSCREEN )*0.01),
          y_offset = (int)(::GetSystemMetrics( SM_CYSCREEN )*0.01);
      int width  = ::GetSystemMetrics( SM_CXSCREEN )-x_offset,
          height = ::GetSystemMetrics( SM_CYSCREEN )-y_offset;

      if( !::SetWindowLong( mw.m_WinBase, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE ) )
      {
         mw.m_Console.AddLine( "Call to SetWindowLong Failed" );
      }
      if( !::SetWindowLong( mw.m_WinBase, GWL_EXSTYLE, WS_EX_APPWINDOW ) )
      {
         mw.m_Console.AddLine( "Call to SetWindowLong Failed" );
      }
      if( ::ChangeDisplaySettings ( NULL, 0 ) != DISP_CHANGE_SUCCESSFUL )
      {
         mw.m_Console.AddLine( "Call to ChangeDisplaySettings Failed" );
      }
      if( mw.m_WinBase.Show( SW_NORMAL ) )
      {
      }
      if( ::SetWindowPos( mw.m_WinBase, HWND_TOPMOST, (int)(x_offset/2), (int)(y_offset/2), width, height, 0 ) )
      {
//         mw.m_Console.AddLine( "Call to SetWindowPos Failed" );
      }
      if( !::SetForegroundWindow( mw.m_WinBase ) )
      {
         mw.m_Console.AddLine( "Call to SetForegroundWindow Failed" );
      }
      mw.m_bFullScreen = false;
      mw.m_Console.AddLine( "FullScreen Off" );
   }      
   else // go to full screen mode
   {
      int width  = ::GetSystemMetrics( SM_CXSCREEN ),
          height = ::GetSystemMetrics( SM_CYSCREEN );
      int bits_per_pixel = 32;

      if( !::SetWindowLong( mw.m_WinBase, GWL_STYLE, WS_POPUP | WS_VISIBLE ) )
      {
         mw.m_Console.AddLine( "Call to SetWindowLong Failed" );
      }
      if( !::SetWindowLong( mw.m_WinBase, GWL_EXSTYLE, WS_EX_TOPMOST ) )
      {
         mw.m_Console.AddLine( "Call to SetWindowLong Failed" );
      }
      if( !ChangeScreenResolution( width, height, bits_per_pixel, CDS_FULLSCREEN ) )
      {
         mw.m_Console.AddLine( "Call to ChangeScreenResolution Failed" );
      }
      if( !mw.m_WinBase.Show( SW_NORMAL ) )
      {
      }
      if( ::SetWindowPos( mw.m_WinBase, HWND_TOPMOST, 0, 0, width, height, 0 ) )
      {
//         mw.m_Console.AddLine( "Call to SetWindowPos Failed" );
//         mw.m_Console.AddLine( mw.GetLastSystemError() );
      }
      mw.m_bFullScreen = true;
      mw.m_Console.AddLine( "FullScreen On" );
   }
//   mw.BuildP1ConsoleList();
   return 0;
}

LRESULT MainWindow::MWBaseOnMouseWheel( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   int zDelta = (short) HIWORD(wparam);
   if( zDelta > 0 )
   {
      mw.m_Z += 1;
   }
   else
   {
      mw.m_Z -= 1;
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_MOUSEWHEEL, wparam, lparam );
}
