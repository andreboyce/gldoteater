#include <windows.h>

#include "C_WinControllManager.h"
#include "../../C_WinMain/C_WinMain.h"
C_WinControllManager ControllManager;

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
               mw.ControllManager.GetEdit( 0 ).SetText( "Button Pressed" );
            } break;
            case 3:
            {
               mw.ControllManager.GetEdit( 0 ).SetText( "Button2 Pressed" );
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
         mw.ControllManager.SetHwnd( mw.m_LastHwndPassed );
         mw.ControllManager.AddButton( C_WinControllManager::ControllProperties(   10,  10, 500, 100, 2,  "Button", NULL, NULL, C_Structs::Rect(), 1 ) );         
         mw.ControllManager.AddButton( C_WinControllManager::ControllProperties( 1000, 100, 100, 100, 3, "Button2", NULL, NULL, C_Structs::Rect(), 1 ) );
         mw.ControllManager.AddEdit(   C_WinControllManager::ControllProperties(  200, 200, 500, 500, 4,        "", NULL, NULL, C_Structs::Rect(), 1 ) );
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_CREATE, wparam, lparam );
      }
      static LRESULT MWBaseOnSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.ControllManager.GetButton( 0 ).Resize ( 500,  100,   10,  10 );
         mw.ControllManager.GetButton( 1 ).Resize(  100,  100, 1000, 100 );
         mw.ControllManager.GetEdit  ( 0 ).Resize(  500,  100,  200, 200 );
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
      }
   protected:
      C_WinControllManager ControllManager;
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
LRESULT CALLBACK WindowProcedure( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{   
   static POINT p;
   static bool f = 0;
   switch( message )
   {
      case WM_CREATE:
      {
         ControllManager.SetHwnd( hwnd );
      } break;

      case WM_SIZE:
      {
         if(f)
         {
//            ControllManager.GetButton( 0 ).Resize( 0, 0, p.x, p.y );
         }
      } break;

      case WM_LBUTTONDOWN:
      {
         RECT temp;
         C_WinControllManager::ControllProperties cp( p.x = LOWORD(lParam), p.y = HIWORD(lParam), 200, 100, 3, "text here...", 0, ButtonSizeHandler, temp, 0 );
         ControllManager.AddButton( cp );
         f = 1;
      } break;
      case WM_RBUTTONDOWN:
      {
         RECT temp;
         C_WinControllManager::ControllProperties cp( p.x = LOWORD(lParam), p.y = HIWORD(lParam), 100, 100, 3, "text here...", NULL, 0, temp, 0 );
         ControllManager.AddListBox( cp );
      } break;

      case WM_DESTROY:
      {
         PostQuitMessage(0);
      } break;
      default: return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
*/
