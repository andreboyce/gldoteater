#include "main.h"

MainWindow::MainWindow( void ) :
   // Initialising variables

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
m_GameMode( TITLE_SCREEN ), //BOSS_MODE ),
m_MainTank( 9.0, 9.0, 700 ),
m_BossTankA( 14.0, 14.0, 350, 200 ),
m_RowsOfTanksAdded( 1 ), // you start out with one set of enmies then they are added over time
m_MaxEnmyRows( 10 ), // the amount of enmys that you will face
m_ShellDamage( 10 ), // the amount of life you lose when you are hit by a shell
m_ExplosionTexturedQuad( 5.0, 5.0 ), // fot later use
m_bYouWinMusicPlayed( false ), // 
m_MapTexturedQuad( 4.0, 4.0 ),
m_MapTexturesIndex( 0 )

{
   // you need to register the message handler functions or they will not be called in responce to 
   // messages captured by the window
   // see CWinApplication which handles the capturing of messages
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
   AddMessageHandler( WM_SYSCOMMAND,       reinterpret_cast<MessageHandler>(MWBaseOnSysCommand)       );
   AddMessageHandler( WM_CLOSE,            reinterpret_cast<MessageHandler>(MWBaseOnMove)             );
   AddMessageHandler( WM_TOGGLEFULLSCREEN, reinterpret_cast<MessageHandler>(MWBaseOnToggleFullScreen) );

   // Load the icon
   m_WinBase.SetLargeIcon( ::LoadIcon( ::GetModuleHandle(NULL), MAKEINTRESOURCE( IDI_LARGEICON ) ) );
   m_WinBase.SetSmallIcon( ::LoadIcon( ::GetModuleHandle(NULL), MAKEINTRESOURCE( IDI_SMALLICON ) ) );

   // Load the accelerator key bindings
   m_Haccel = ::LoadAccelerators( m_Hinstance, MAKEINTRESOURCE(ID_ACCELERATORS) );

   m_AddEnmyRowInterval.Begin();
   m_AddEnmyRowInterval.SetInterval( 4500 );
   InitialiseNewGame();
}

// this is called when the program exits for any reason
// so you can put cleanup code here
// but all the stl container classes I use do their own clean up already :)

MainWindow::~MainWindow( void )
{
}

//***************************************************************************
//******************************* MainWindow::RunGameLoop *******************
// Accepts: void
//
// Purpose: update the game objects and render them to the screen
//
// Returns: quit message indicating an error
//          or success

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
               m_Fmod.Update();
               m_deltaGameTime        = Interval.TimeSinceLastCall()*m_GameSpeed;
               m_deltaGameTimeSquared = m_deltaGameTime*m_deltaGameTime;
               Interval.Begin();  // start recording the time taken to reach the next frame

               // this is the part of the game loop that determines what should be done depending on 
               // what mode the game is in
               switch( m_GameMode )
               {
                  case TITLE_SCREEN:
                  {
                     RenderTitleScreen(); // draw the title screen
                  } break;
                  case GAME_IN_PROGRESS:
                  {
                     UpdateObjects();    // update the game objects
                     RenderGameScreen(); // draw the game screen
                     HandleCollisions(); // Collision Detection
                     if( !m_bGamePaused )
                     {
                        if( m_AddEnmyRowInterval.IntervalExceeded() && (m_RowsOfTanksAdded < m_MaxEnmyRows) )
                        {
                           m_AddEnmyRowInterval.Begin();
                           AddNewTanks();
                        }
                     }
                  } break;
                  case BOSS_MODE:
                  {
                     UpdateObjects();    // update the game objects
                     RenderBossScreen(); // draw the screen
                     HandleCollisions(); // Collision Detection
                  } break;
                  case YOU_WIN_MODE:
                  {
//                     if( !m_Fmod.IsPlaying( BOSS_EXPLOSION_CHANNEL ) )
//                     {
//                        if( !m_bYouWinMusicPlayed )
//                        {
//                           m_bYouWinMusicPlayed = true;
//                          PlayClip( 3, YOU_WIN_CHANNEL );
//                        }
                        RenderYouWinScreen(); // draw the you screen
//                     }
                  } break;
                  case GAME_OVER:
                  {
                     RenderGameOverScreen(); // draw the game screen
                  } break;
                  default:
                  {
                  };
               };
               if( !m_bFullScreen )
                  m_FrameRate.ShowFPSinTitle( m_WinBase );
               else
               {
                  ShowFPSinWindow( m_TextBoundingRect.left, m_TextBoundingRect.top, m_Z+40 );
               }
               ::SwapBuffers( m_GLRC ); // switch from back buffer to front buffer
            }
         }
      }
   }
   return msg.wParam;
}

// this is a windows gui application
// so you use winmain instead of main
//

int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,      int nFunsterStil )
{
   MainWindow App;  // our main window class

   // for later use if we decide to make the game fit on a certain portion of the screen
   int x_pos = (int)(::GetSystemMetrics( SM_CXSCREEN )*0.05),
       y_pos = (int)(::GetSystemMetrics( SM_CYSCREEN )*0.05);
   int width  = ::GetSystemMetrics( SM_CXSCREEN )-x_pos*2,
       height = ::GetSystemMetrics( SM_CYSCREEN )-y_pos*2;

   if( App.Create( "Dominators", NULL ) )
   {
      App.Initialise(); // initialise the various subsystems, sound graphics etc, and load textures and sounds
      App.m_WinBase.Show( SW_MAXIMIZE ); // maximize the window
      int ret = App.RunGameLoop();  // enter the game loop, this differes from a regular windows message loop because it is built for speed
      return ret;
   }
   return -1; // error creating window
}

