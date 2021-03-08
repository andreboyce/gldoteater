#include "main.h"

LRESULT MainWindow::MWBaseOnCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( LOWORD(wparam) )
   {
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnMouseMove( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
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
   mw.m_Keys.PressKey( wparam );
   mw.HandleDownKeys( wparam, lparam );
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYDOWN, wparam, lparam );
}     

LRESULT MainWindow::MWBaseOnKeyUp( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_Keys.ReleaseKey( wparam );
   mw.HandleUpKeys( wparam, lparam );
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

LRESULT MainWindow::MWBaseOnMove( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_x_pos = LOWORD(lparam);
   mw.m_y_pos = HIWORD(lparam);
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_MOVE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnKillFocus( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_bHasKeyboardFocus = false;
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KILLFOCUS, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnSetFocus( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_bHasKeyboardFocus = true;
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_SETFOCUS, wparam, lparam );
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
//         mw.m_Console.AddLine( GetLastSystemError() );
      }
      mw.m_bFullScreen = true;
      mw.m_Console.AddLine( "FullScreen On" );
   }
//   mw.BuildP1ConsoleList();
   return 0;
}

LRESULT MainWindow::MWBaseOnClose( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   ::glFlush();
   mw.m_GLRC.DisableOpenGL();
   return ::PostMessage( mw, WM_QUIT, 0, 0 );
}

LRESULT MainWindow::MWBaseOnSysCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( wparam )
   {
      case SC_CLOSE:
      {
         ::glFlush();
         mw.m_GLRC.DisableOpenGL();
      } return ::PostMessage( mw, WM_QUIT, 0, 0 );
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_SYSCOMMAND, wparam, lparam );
}

