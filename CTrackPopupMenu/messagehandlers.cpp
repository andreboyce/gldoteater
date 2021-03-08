#include "main.h"

LRESULT MainWindow::MWBaseOnCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
  switch( LOWORD(wparam) )
  {
  }
  return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_TrackPopupMenu.Display( LOWORD(lparam), HIWORD(lparam) );
//   Alert( mw.m_WinBase.m_ErrorHandlers.GetLastSystemError() );
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONDOWN, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnLButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDOWN, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnCreate( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_CREATE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
}

