#include "main.h"

MainWindow::MainWindow( void )
{
   AddMessageHandler( WM_SIZE,        reinterpret_cast<MessageHandler>(MWBaseOnSize)     );
   AddMessageHandler( WM_CREATE,      reinterpret_cast<MessageHandler>(MWBaseOnCreate)   );
   AddMessageHandler( WM_LBUTTONDOWN, reinterpret_cast<MessageHandler>(MWBaseOnLButtonD) );
   AddMessageHandler( WM_RBUTTONDOWN, reinterpret_cast<MessageHandler>(MWBaseOnRButtonD) );
   AddMessageHandler( WM_COMMAND,     reinterpret_cast<MessageHandler>(MWBaseOnCommand)  );
}

MainWindow::~MainWindow( void )
{
}

int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,      int nFunsterStil )
{
   MainWindow App;
   int x_pos = (int)(::GetSystemMetrics( SM_CXSCREEN )*0.05),
       y_pos = (int)(::GetSystemMetrics( SM_CYSCREEN )*0.05);
   int width  = ::GetSystemMetrics( SM_CXSCREEN )-x_pos*2,
       height = ::GetSystemMetrics( SM_CYSCREEN )-y_pos*2;

   if( App.Create( "WinMain", HWND_DESKTOP, x_pos, y_pos, width, height ) )
   {
//      ::SetWindowPos( App, HWND_TOPMOST, x_pos, y_pos, width, height, SWP_SHOWWINDOW );
      int ret = App.RunMessageLoop();
      return ret;
   }
   return -1;
}
