#include "../../C_WinMain/C_WinMain.h"
#include "C_WinProgressBar.h"

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
      virtual ~MainWindow( void ){}
      static LRESULT MWBaseOnCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         switch( LOWORD(wparam) )
         {
         }
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
      }
      static LRESULT MWBaseOnRButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONDOWN, wparam, lparam );
      }
      static LRESULT MWBaseOnLButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.m_WinProgressBar.StepNext();
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDOWN, wparam, lparam );
      }
      static LRESULT MWBaseOnCreate( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.m_WinProgressBar.Create( mw.m_LastHwndPassed, 10, "", 0, 0, 1000, 50 );
         mw.m_WinProgressBar.SetRange( 0, 10 );
         mw.m_WinProgressBar.SetStep( 1 );
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_CREATE, wparam, lparam );
      }
      static LRESULT MWBaseOnSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
      }

      C_WinProgressBar m_WinProgressBar;

   protected:
   private:
};

int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,      int nFunsterStil )
{
   MainWindow App;
   if( App.Create( "Window" ) )
   {
      return App.RunMessageLoop();
   }
   return -1;
}

