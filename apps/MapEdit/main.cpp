#include "main.h"

MainWindow::MainWindow( void ) :
   m_GameMode( RENDER_MAP ),
   m_bQuit( false ), // if this is set to true the game loop will exit right away
   m_bisVisible( false ), // the window starts out vissably
   m_bFullScreen( false ), // we start out in windowed mode
   m_Z( -100.0f ), // where we do the drawing, we can or out by changing this
   m_TextZ( -50.0 ), // where we draw the text
   m_Width( 0 ), // window client width
   m_Height( 0 ), // window client height
   m_deltaGameTime( 0.0 ), // time elapsed since last frame
   m_deltaGameTimeSquared( 0.0 ), // time elapsed since last frame squared
   m_GameSpeed( 1.0 ),
   m_FieldOfViewY( 45.0f ), // perspective
   m_FieldOfViewX( 0.0f ), // perspective
   m_bSoundOn( true ), // sound is on when we start
   m_bGamePaused( false ), // the game is not paused when we start
   m_LimitFPS( 100 ),  // fps limit, on certain video cards (namely the GForce FX5600) when you disable vsync the graphic card begins to over heat
                    // so I put this in here to stop it from overworking itself
   m_MapTexturedQuad( 1.0, 1.0 ),
   m_SelectedDataType( 0 ),
   m_bRenderMousePos( true ),
   m_MapTexturesIndex( 0 ),
   m_BoundingRectWidth( 0.0f ), 
   m_BoundingRectHeight( 0.0f ),
   m_TextBoundingRectWidth( 0.0f ),
   m_TextBoundingRectHeight( 0.0f ),
   m_TextBoundingRectHalfWidth( 0.0f ),
   m_TextBoundingRectHalfHeight( 0.0f ),
   m_BoundingRectHalfWidth( 0.0f ),
   m_BoundingRectHalfHeight( 0.0f ),
   m_InvBoundingRectWidthDivTileMapWidth( 0.0f ),
   m_InvBoundingRectHeightDivTileMapHeight( 0.0f ),
   m_BoundingRectHalfWidthMinMapTexturedQuadhalfwidth( 0.0f ),
   m_BoundingRectHalfHeightMinMapTexturedQuadhalfheight( 0.0f ),
   m_HwndShowSpecialDataTypesDialog( NULL ),
   m_HwndTilePropertiesDlg( NULL ),
   m_TilePropertiesSolidity( 0 ),
   m_TilePropertiesGraphic( 0 )
{
   AddMessageHandler( WM_SIZE,             reinterpret_cast<MessageHandler>(MWBaseOnSize)             );
   AddMessageHandler( WM_CREATE,           reinterpret_cast<MessageHandler>(MWBaseOnCreate)           );
   AddMessageHandler( WM_LBUTTONDOWN,      reinterpret_cast<MessageHandler>(MWBaseOnLButtonD)         );
   AddMessageHandler( WM_RBUTTONDOWN,      reinterpret_cast<MessageHandler>(MWBaseOnRButtonD)         );
   AddMessageHandler( WM_LBUTTONUP,        reinterpret_cast<MessageHandler>(MWBaseOnLButtonU)         );
   AddMessageHandler( WM_RBUTTONUP,        reinterpret_cast<MessageHandler>(MWBaseOnRButtonU)         );
   AddMessageHandler( WM_COMMAND,          reinterpret_cast<MessageHandler>(MWBaseOnCommand)          );
   AddMessageHandler( WM_PAINT,            reinterpret_cast<MessageHandler>(MWBaseOnPaint)            );
   AddMessageHandler( WM_MOUSEMOVE,        reinterpret_cast<MessageHandler>(MWBaseOnMouseMove)        );
   AddMessageHandler( WM_HSCROLL,          reinterpret_cast<MessageHandler>(MWBaseOnHScroll)          );
   AddMessageHandler( WM_VSCROLL,          reinterpret_cast<MessageHandler>(MWBaseOnVScroll)          );
   AddMessageHandler( WM_KEYUP,            reinterpret_cast<MessageHandler>(MWBaseOnKeyUp)            );
   AddMessageHandler( WM_KEYDOWN,          reinterpret_cast<MessageHandler>(MWBaseOnKeyDown)          );
   AddMessageHandler( WM_NOTIFY,           reinterpret_cast<MessageHandler>(MWBaseOnNotify)           );
   AddMessageHandler( WM_TIMER,            reinterpret_cast<MessageHandler>(MWBaseOnTimer)            );
   AddMessageHandler( WM_SETFOCUS,         reinterpret_cast<MessageHandler>(MWBaseOnSetFocus)         );
   AddMessageHandler( WM_KILLFOCUS,        reinterpret_cast<MessageHandler>(MWBaseOnKillFocus)        );
   AddMessageHandler( WM_LBUTTONDBLCLK,    reinterpret_cast<MessageHandler>(MWBaseOnLDoubleClick)     );
   AddMessageHandler( WM_RBUTTONDBLCLK,    reinterpret_cast<MessageHandler>(MWBaseOnRDoubleClick)     );
   AddMessageHandler( WM_CLOSE,            reinterpret_cast<MessageHandler>(MWBaseOnClose)            );
   AddMessageHandler( WM_MOUSEWHEEL,       reinterpret_cast<MessageHandler>(MWBaseOnMouseWheel)       );
   
//   AddMessageHandler( WM_SYSCOMMAND,       reinterpret_cast<MessageHandler>(MWBaseOnSysCommand)       );
   AddMessageHandler( WM_TOGGLEFULLSCREEN, reinterpret_cast<MessageHandler>(MWBaseOnToggleFullScreen) );
}

MainWindow::~MainWindow( void )
{
}

WPARAM MainWindow::RunGameLoop( void )
{
   MSG msg;
   bool isMessagePumpActive = true;
   int  iReturn             = 0;
   TimeInterval Interval;

   while( isMessagePumpActive ) // While The Message Pump Is Active
   {
      if( m_bQuit )
      {
         isMessagePumpActive = false;
      }
      if( ::PeekMessage( &msg, m_WinBase, 0, 0, PM_REMOVE ) )
      {
         // Check For WM_QUIT Message
         if( msg.message != WM_QUIT ) // Is The Message A WM_QUIT Message?
         {
            ::TranslateMessage( &msg );
            ::DispatchMessage(  &msg );
         }
         else // Otherwise ( If Message Is WM_QUIT )
         {
            isMessagePumpActive = false; // Terminate The Message Pump
         }
      }
      else // If There Are No Messages
      {
         if( !m_bisVisible ) // If Window Is Not Visible
         {
            ::WaitMessage(); // Application Is Minimized Wait For A Message
         }
         else // If Window Is Visible
         {
            if( m_LimitFPS ) // operator bool of this class will determine if
                             // the fps limit has been reached
            {
               m_deltaGameTime        = Interval.TimeSinceLastCall()*m_GameSpeed;
               m_deltaGameTimeSquared = m_deltaGameTime*m_deltaGameTime;
               Interval.Begin();  // start recording the time taken to reach the next frame

               // this is the part of the game loop that determines what should be done depending on 
               // what mode the game is in
               switch( m_GameMode )
               {
                  case RENDER_MAP:
                  {
                     RenderMap(); // draw the title screen
                  } break;
               };
               if( !m_bFullScreen )
                  m_FrameRate.ShowFPSinTitle( m_WinBase );
               else
               {
                  ShowFPSinWindow( m_TextBoundingRect.left, m_TextBoundingRect.top, m_Z+40 );
               }
               ::SwapBuffers( m_GLRC ); // switch from back buffer to front buffer

               // Run the message loop. It will run until GetMessage() returns 0 
               MSG messages = {0};
               int iReturn = ::GetMessage( &messages, NULL, 0, 0 );
               if( iReturn != -1 )
               {
                  if ( !::TranslateAccelerator( m_WinBase, m_Haccel, &messages ) )
                  {
                     ::TranslateMessage(&messages);
                     ::DispatchMessage(&messages);
                  }
               }
            }
         }
      }
   }
   return msg.wParam;
}

int MainWindow::RunGetMessageLoop( void )
{
   MSG  messages;
   int  iReturn;

   if( ::IsWindow( m_WinBase ) )
   {
      // Run the message loop. It will run until GetMessage() returns 0 
      while ( iReturn = ::GetMessage( &messages, NULL, 0, 0 ) )
      {
         if( iReturn != -1 )
         {
            if ( !::TranslateAccelerator( m_WinBase, m_Haccel, &messages ) )
            {
               ::TranslateMessage(&messages);
               ::DispatchMessage(&messages);
            }
         }
         else
         {
            return -1;
         }
      }
      return messages.wParam;
   }
   return -1;
}

int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,      int nFunsterStil )
//int main( int argc, char *argv[] )
{
   MainWindow App;
   int x_pos = (int)(::GetSystemMetrics( SM_CXSCREEN )*0.05),
       y_pos = (int)(::GetSystemMetrics( SM_CYSCREEN )*0.05);
   int width  = ::GetSystemMetrics( SM_CXSCREEN )-x_pos*2,
       height = ::GetSystemMetrics( SM_CYSCREEN )-y_pos*2;

   App.m_WinBase.SetMenu( MAKEINTRESOURCE( ID_MAINMENU ) );

   try
   {
      if( App.Create( "MapEdit", NULL ) )
      {
         App.Initialise();
         App.m_WinBase.Show( SW_MAXIMIZE );
         App.m_FrameRate.SetTitle( App.m_WinBase.GetTitle() );

         /*if( argc > 1 )
         {
            std::string spriteframefilename = argv[1];
            App.LoadMap( spriteframefilename );
            App.Reshape( App.m_WinBase.GetClientRect().right-App.m_WinBase.GetClientRect().left, 
                        App.m_WinBase.GetClientRect().bottom-App.m_WinBase.GetClientRect().top );
            App.m_FrameRate.SetTitle( App.m_WinBase.GetTitle() + ": " + spriteframefilename );
         }*/

         int ret = App.RunGameLoop();
         //int ret = App.RunGetMessageLoop();
         return ret;
      }
   }
   catch( ... )
   {
      Alert( "Unknown exception" );
      return -1;
   }
   return -1;
}
