#include <windows.h>
#include "C_WinStatusBar.h"

#include "../../C_WinMain/C_WinMain.h"


class MainWindow: public C_WinMain
{
   public:
      MainWindow( void )
      {
         AddMessageHandler( WM_SIZE,        reinterpret_cast<MessageHandler>(MWBaseOnSize)     );
         AddMessageHandler( WM_CREATE,      reinterpret_cast<MessageHandler>(MWBaseOnCreate)   );
         AddMessageHandler( WM_LBUTTONDOWN, reinterpret_cast<MessageHandler>(MWBaseOnLButtonD) );
         AddMessageHandler( WM_RBUTTONDOWN, reinterpret_cast<MessageHandler>(MWBaseOnRButtonD) );
         AddMessageHandler( WM_COMMAND,     reinterpret_cast<MessageHandler>(MWBaseOnCommand)  );
      }
      ~MainWindow( void ){}
      static LRESULT MWBaseOnCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         switch( LOWORD(wparam) )
         {
         }
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
      }
      static LRESULT MWBaseOnRButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.Sbar.DisableSimpleMode();
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONDOWN, wparam, lparam );
      }
      static LRESULT MWBaseOnLButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.Sbar.EnableSimpleMode( "6546521.0354768" );
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDOWN, wparam, lparam );
      }
      static LRESULT MWBaseOnCreate( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         C_WinStatusBar::V_Parts Parts;
         Parts.push_back( C_WinStatusBar::S_Part( "Ready...",    30 ) );
         Parts.push_back( C_WinStatusBar::S_Part( "Still Ready", 30 ) );
         Parts.push_back( C_WinStatusBar::S_Part( "Wating...",   10 ) );
         Parts.push_back( C_WinStatusBar::S_Part( "Test...",     10 ) );
         Parts.push_back( C_WinStatusBar::S_Part( "Testing...",  10 ) );
         Parts.push_back( C_WinStatusBar::S_Part( "...",         10 ) );

         mw.Sbar.Create( mw.m_LastHwndPassed, 2, Parts );
         mw.Sbar.SetSimpleModeText( "1234" );
         mw.Sbar.EnableSimpleMode();

         return ::DefWindowProc( mw.m_LastHwndPassed, WM_CREATE, wparam, lparam );
      }
      static LRESULT MWBaseOnSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.Sbar.Resize( (short)wparam, (short)lparam );
         mw.Sbar.InitializeStatusBar();
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
      }
   protected:
      C_WinStatusBar Sbar;

   private:
};

int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument, int nFunsterStil )

{
    MainWindow WinMain;
    
    if( WinMain.m_WinBase.Create( "Window" ) )
    {
       return WinMain.RunMessageLoop();
    }
    return -1;
}

