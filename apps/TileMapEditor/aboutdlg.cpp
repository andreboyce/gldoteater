#include "main.h"

BOOL CALLBACK MainWindow::AboutDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
   MainWindow * mw = reinterpret_cast< MainWindow *>( ::GetWindowLong( hwnd, GWL_USERDATA ) );
	switch( Message )
	{
	   case WM_INITDIALOG:
      {
      } break;
      case WM_COMMAND:
      {
         switch( LOWORD( wParam ) )
         {
            case ID_ABOUT_DIALOG_BUTTON_OK:
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

bool MainWindow::ShowAboutDlg( void )
{
   bool        ret = false;
   m_HwndAboutDlg = CreateDialog( ::GetModuleHandle( NULL ), 
                                   MAKEINTRESOURCE( ID_ABOUT_DIALOG ), 
                                   m_WinBase, 
                                   AboutDlgProc );

   if( m_HwndAboutDlg != NULL )
   {
      ::ShowWindow(    m_HwndAboutDlg, SW_SHOW );
      ::SetWindowLong( m_HwndAboutDlg, GWL_USERDATA, (LPARAM)this );

      ::SetWindowText( ::GetDlgItem( m_HwndAboutDlg, ID_ABOUT_DIALOG_EDIT_VERSION ), GetFileVersionValue( ::GetModuleHandle(NULL), "ProductVersion" ).c_str() );
      ::UpdateWindow(  ::GetDlgItem( m_HwndAboutDlg, ID_ABOUT_DIALOG_EDIT_VERSION ) );
      ret = true;
   }
   else
   {
      ::Alert( GetLastSystemError() + " CreateDialog failed" );
   }
   return ret;
}

