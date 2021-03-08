#include <windows.h>

#include "C_WinButton.h"
#include "../../C_WinMain/C_WinMain.h"

class MainWindow: public C_WinMain
{
   public:
      MainWindow( void )
      {
         AddMessageHandler( WM_SIZE,        reinterpret_cast<MessageHandler>(MWBaseOnSize)     );
         AddMessageHandler( WM_CREATE,      reinterpret_cast<MessageHandler>(MWBaseOnCreate)   );
         AddMessageHandler( WM_LBUTTONDOWN, reinterpret_cast<MessageHandler>(MWBaseOnLButtonD) );
         AddMessageHandler( WM_COMMAND,     reinterpret_cast<MessageHandler>(MWBaseOnCommand) );
      }
      ~MainWindow( void ){}
      static LRESULT MWBaseOnCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         switch( LOWORD(wparam) )
         {
            case 2:
            {
               Alert( "Button Pressed" );
            } break;
            case 3:
            {
               Alert( "Button2 Pressed" );
            } break;
         }
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
      }

      static LRESULT MWBaseOnLButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDOWN, wparam, lparam );
      }
      static LRESULT MWBaseOnCreate( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.Button.Create ( mw.m_LastHwndPassed, 2, "Title", 10, 10, 500, 100, 1 );
         mw.Button.SetText( "What Title - What--------------------------" );
         mw.Button2.Create( mw.m_LastHwndPassed, 3, "Title...", 1000, 100, 100, 100, 1 );
         mw.Button2 = mw.Button;
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_CREATE, wparam, lparam );
      }
      static LRESULT MWBaseOnSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.Button2.Resize( 100,  100, 1000, 100 );
         mw.Button.Resize ( 500,  100,   10,  10 );
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
      }
   protected:
      C_WinButton Button;
      C_WinButton Button2;
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

/*
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//   static C_WinButton Button;
//   static C_WinButton Button2;
//static C_WinButton Button3;

   switch (message)                 
   {
      case WM_CREATE:
      {
         Button.Create( hwnd, 2, "Title", 10, 10, 100, 100, 1 );
         Button.SetText( "What Title - What--------------------------" );
         Button2.Create( hwnd, 3, "Title...", 1000, 100, 100, 100, 1 );
         Button2 = Button;
try
{
//         Button3.Create( hwnd, 3, "Title...", 1000, 100, 100, 100, 1 );
         C_WinButton Button3 = Button;
         Alert( Button3.GetText() );
}
catch(...)
{
   Alert( " some exception thrown" );
}
      } break;

      case WM_SIZE:
      {
         Button.Resize( 0, 0, 10, 0 );
         Button2.Resize( 0, 0, 100, 100 );
//         Button3.Resize( 0, 0, 100, 200 );
      } break;

      case WM_LBUTTONDOWN:
      {
         Alert( Button2.GetText() );
      } break;

       case WM_DESTROY:
           PostQuitMessage(0);      
           break;
       default:                     
           return DefWindowProc (hwnd, message, wParam, lParam);
   }
   return 0;
}
*/
