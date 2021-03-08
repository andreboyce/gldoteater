#include "main.h"

MainWindow::MainWindow( void ) :
   // Initialising variables
   m_bQuit( false ),       // if this is set to true the game loop will exit right away
   m_bisVisible( false ),  // the window starts out vissably
   m_bFullScreen( false ), // we start out in windowed mode
   m_Z( -100.0f ),      // where we do the drawing, we can or out by changing this
   m_TextZ( -50.0 ),    // where we draw the text
   m_Width( 0 ),        // window client width
   m_Height( 0 ),       // window client height
   m_deltaGameTime( 0.0 ),        // time elapsed since last frame
   m_deltaGameTimeSquared( 0.0 ), // time elapsed since last frame squared
   m_deltaPositionAndDirectionUptateTime( 0 ),
   m_CurrentMap( 0 ),
   m_bPauseOnSize( true ),
   m_bDepthEvenFlag( false ),
   m_TotalDataSent( 0 ),
   m_GameSpeed( 1.0 ),
   m_GLRC( 16 ),
   m_bUseVBOs( true ),
   m_deltaTickTime( 0 ),
   m_bRemoteVersionReceived( false ),
   m_RemotePlayerInfo(),
   m_deltaSendPing( 0 ),
   m_Version(  ),
   m_bNetworkThreadActive( false ),
   m_PlayerControlledByMouse( 0 ),
   m_PlayerGoal(),
   m_bGotToGameOverAfterStats( false ),
   m_bShowOnlineGameMode( false ),
   m_bMoveGhosts( true ), 
   m_TimeSinceConnect( 0 ),
   m_PositionAndDirectionToHostID( 0 ),
   m_TimeSinceRoundStarted( 0 ),
   m_MapToLoad( -1 ),
   m_bRenderMousePos( false ),
   m_bGameSavedOnServer( false ),
   m_bContinueWithNextPlayersCollisionDetection( true ),
   m_FieldOfViewY( 45.0f ), // perspective
   m_FieldOfViewX( 0.0f ),  // perspective
   m_bSoundOn( true ),      // sound is on when we start
   m_bGamePaused( false ),  // the game is not paused when we start
   m_LimitFPS( 100 ),  // fps limit, on certain video cards (namely the GForce FX5600) when you disable vsync the graphic card begins to over heat
                       // so I put this in here to stop it from overworking itself
   m_GameMode( TITLE_SCREEN ), //VIEW_STATS ),
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
   m_MapTexturedQuad( 1.0, 1.0 ),
   m_GhostTexturesIndex( 0 ),
   m_BackTexture( 0 ),
   m_Level( 0 ),
   m_deltaKeepAlive( 0 ),
   m_Mutex(),
   m_EnetClientThread( NULL, 0, (LPTHREAD_START_ROUTINE)EnetClientThreadFunc, this, 0 ),
   m_EnetServerThread( NULL, 0, (LPTHREAD_START_ROUTINE)EnetServerThreadFunc, this, 0 ),
   m_Port( 7655 ),
   m_Nick( "" ),
   m_ConsoleInput( GET_NOTHING ),
   m_bIgnoreVK_RETURN( false ),
   m_bShowTheConsole( false ),
   m_bTheConsoleAcceptsTyping( false ),
   m_bNetworkActive( false ),
   m_bShowMessageQueue( false ),
   m_bConnectionFailed( false ),
   m_bServerCreationFailed( false ),
   m_HostPeer( NULL ),
   m_Server( NULL ),
   m_Client( NULL ),
   m_MessageQueue( 750, 3 ),
   m_EatenRandomItemIndex( 0 ),
   m_InventoryTextureIndex( 0 ),
   m_NumberOfInventoryTextures( 0 ),
   m_DeltaClientServerRoundStart( 0 ),
   m_bInitializeOnlineGame( false ),
   m_bRenderRandomItems( true ),//( false ),
   m_bProcessClientThread( true ),
   m_bProcessServerThread( true ),
   m_LastGameStateID( 0 ),
   m_TotalDataSentThisSecond( 0 ),
   m_TotalDataSentThisSecondInterval(),
   m_MasterServerHost( "www.andreboyce.com" ),
   m_MasterServerPath( "/server/" ),
   m_RemoteHostAddress( "127.0.0.1" ),
   m_OFont( "Verdana", 
           WinHFont::S_FontProperties( -24, 10, 0, 0, 0, false, false, false,
                                         DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
                                         ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH ) ),
   m_TitleScreenOptionsListBox(   "Options",                  5, m_Font, 0 ),
   m_ChooseClientOrServerListBox( "Choose Client or Server",  2, m_Font, 0 ),
   m_GamesListBox(                "List of Available Games",  6, m_Font, 0 ),
   m_HighScoresListBox(           "HighScores",              18, m_Font, 0 ),

   m_WhiteGhost(   Vector2D( 0, 0 ), Vector2D( 0, 0 ), C_GLTexQuad(), DOWN, std::vector<AStar::AStar2d::PointAndDirection>(), false, false, 0, C_Flash( false, 250 ), Structs::Point(), Structs::Point() ),
   m_BlueGhost(    Vector2D( 0, 0 ), Vector2D( 0, 0 ), C_GLTexQuad(), LEFT, std::vector<AStar::AStar2d::PointAndDirection>(), false, false, 0, C_Flash( false, 250 ), Structs::Point(), Structs::Point() ),
   m_RedGhost(     Vector2D( 0, 0 ), Vector2D( 0, 0 ), C_GLTexQuad(), LEFT, std::vector<AStar::AStar2d::PointAndDirection>(), false, false, 0, C_Flash( false, 250 ), Structs::Point(), Structs::Point() ),
   m_YellowGhost(  Vector2D( 0, 0 ), Vector2D( 0, 0 ), C_GLTexQuad(), LEFT, std::vector<AStar::AStar2d::PointAndDirection>(), false, false, 0, C_Flash( false, 250 ), Structs::Point(), Structs::Point() )
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
   AddMessageHandler( WM_CHAR,             reinterpret_cast<MessageHandler>(MWBaseOnChar)             );

   // Load the icon
   m_WinBase.SetLargeIcon( ::LoadIcon( ::GetModuleHandle(NULL), MAKEINTRESOURCE( IDI_LARGEICON ) ) );
   m_WinBase.SetSmallIcon( ::LoadIcon( ::GetModuleHandle(NULL), MAKEINTRESOURCE( IDI_SMALLICON ) ) );

   // Load the accelerator key bindings
   m_Haccel = ::LoadAccelerators( m_Hinstance, MAKEINTRESOURCE(ID_ACCELERATORS) );

   InitialiseNewGame();
}

// this is called when the program exits for any reason
// so you can put cleanup code here
// but all the stl container classes I use do their own clean up already :)

MainWindow::~MainWindow( void )
{
   DestroyVBOs();
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
//            if( m_LimitFPS ) // operator bool of this class will determine if
//                             // the fps limit has been reached
//            {
               m_Fmod.Update();
               if( m_bNetworkActive )
               {
                  m_TimeSinceConnect      += Interval.TimeSinceLastCall();
                  if( m_GameMode == GAME_IN_PROGRESS )
                     m_TimeSinceRoundStarted += Interval.TimeSinceLastCall();
                  if( m_bInitializeOnlineGame )
                  {
                     m_bInitializeOnlineGame = false;
                     InitialiseClientOnlineGame();
                  }
                  else if( m_bInitializeNewOnlineRound )
                  {
                     m_bInitializeNewOnlineRound = false;
                     InitialiseNewOnlineRound();
                  }
               }
               m_deltaGameTime        = Interval.TimeSinceLastCall()*m_GameSpeed;
               m_deltaGameTimeSquared = m_deltaGameTime*m_deltaGameTime;
               Interval.Begin();  // start recording the time taken to reach the next frame

               if( m_MapToLoad != -1 )
               {
                  if( m_MapFilenames.size() > m_MapToLoad )
                  {
                     LoadMap( m_MapFilenames[m_MapToLoad] );
                  }
                  m_MapToLoad = -1;
               }


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
                     HandleCollisions();
                     RenderGameScreen(); // draw the game screen
                  } break;
                  case YOU_WIN_MODE:
                  {
                     m_LastGameStateID              = 0;
                     m_DeltaClientServerRoundStart  = 0;
                     m_PositionAndDirectionToHostID = 0;
                     RenderYouWinScreen(); // draw the you screen
                  } break;
                  case GAME_OVER:
                  {
                     m_LastGameStateID              = 0;
                     m_DeltaClientServerRoundStart  = 0;
                     m_PositionAndDirectionToHostID = 0;
                     RenderGameOverScreen(); // draw the game screen
                  } break;
                  case CHOOSE_CLIENT_OR_SERVER_MODE:
                  {
                     RenderChooseClientOrServer(); // draw the title screen
                  } break;
                  case LISTEN_MODE:
                  {
                     RenderListenMode();
                  } break;
                  case GET_SERVER_INFORMATION_FROM_USER:
                  {
                     RenderTitleScreen(); // draw the title screen
                  } break;
                  case VIEW_HIGH_SCORES:
                  {
                     RenderTitleScreen(); // draw the title screen
                  } break;
                  case CONNECT_MODE:
                  {
                     RenderConnectMode();
                  } break;
                  case VIEW_STATS:
                  {
                     m_LastGameStateID              = 0;
                     m_DeltaClientServerRoundStart  = 0;
                     m_PositionAndDirectionToHostID = 0;
                     RenderStatsMode();
                     m_TimeSinceRoundStarted = 0;
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
//                  std::ostringstream tmp;
//                  tmp << m_Font.Height("6");
//                  m_Font.Print( tmp.str(), 0, 10, -60 );
               }
               m_Mutex.Lock();
               ::SwapBuffers( m_GLRC ); // switch from back buffer to front buffer
               m_Mutex.Release();
//            }
         }
      }
   }
   return msg.wParam;
}

bool check_for_data( std::string path )
{
/*
   WIN32_FIND_DATA FileData;
   HANDLE          hSearch = NULL;
   char buffer[MAX_PATH];
   strncpy( buffer, path.c_str(), MAX_PATH );
   lstrcat( buffer, "\\*.*" );	
   ::ZeroMemory( &FileData, sizeof(WIN32_FIND_DATA) );
   hSearch = ::FindFirstFile( buffer, &FileData );
	if( hSearch == INVALID_HANDLE_VALUE )
   { 
      return false;
   }
   bool bAllFilesFound = false;
   bool file_found     = false;
//   unsigned long file_count = 0;
   while( !bAllFilesFound )
   {
      // Error Check
      if( strcmp(FileData.cFileName, ".") == 0 )
      {
         // Acquire the next file in the directory
         if( !FindNextFile(hSearch, &FileData) ){ bAllFilesFound = true; }
         continue;
      }

      // Error Check
      if( strcmp(FileData.cFileName, "..") == 0 )
      {
         // Acquire the next file in the directory
         if( !FindNextFile(hSearch, &FileData) ){ bAllFilesFound = true; }
         continue;
      }
      if( strcmp( FileData.cFileName, ) )

      //file_count++;

      if( !::FindNextFile( hSearch, &FileData ) )
      {
         bAllFilesFound = true;
      }
   }
   // Close the search handle
   FindClose( hSearch );
*/
   return true;
}

// this is a windows gui application
// so you use winmain instead of main
//

int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,      int nFunsterStil )
{
   try
   {
      if( !check_for_data( "data" ) )
      {
          Alert( "Data directory not found, game unplayable" );
          return -1;
      }
      //srand( GetTickCount()%500 );
      MainWindow App;  // our main window class

      // for later use if we decide to make the game fit on a certain portion of the screen
      int x_pos = (int)(::GetSystemMetrics( SM_CXSCREEN )*0.05),
          y_pos = (int)(::GetSystemMetrics( SM_CYSCREEN )*0.05);
      int width  = ::GetSystemMetrics( SM_CXSCREEN )-x_pos*2,
          height = ::GetSystemMetrics( SM_CYSCREEN )-y_pos*2;

      char current_dir[ MAX_PATH ];
      ::ZeroMemory( current_dir, MAX_PATH );
      ::getcwd( (char*)current_dir, MAX_PATH );
      App.m_AppWorkingDirectory = current_dir;
      App.m_DataDirectory       = App.m_AppWorkingDirectory + "/data";

      App.HandleCommandLine( hThisInstance, hPrevInstance,
                             lpszArgument,  nFunsterStil );

      App.InitializeEnet();
      App.m_MasterServerHost = "";
      App.LoadConfig();
      //App.m_bRenderMousePos = true;
      App.m_bUseVBOs = false;
      App.m_Version = GetFileVersionValue( NULL, "ProductVersion" );
      std::string title = "GLDotEater (now in pre-beta testing phase) vesrion: " + GetFileVersionValue( NULL, "ProductVersion" );
      if( App.Create( (char*)title.c_str(), NULL ) )
      {
         App.m_FrameRate.SetTitle( App.m_WinBase.GetTitle() );
         App.Initialise(); // initialise the various subsystems, sound graphics etc, and load textures and sounds
         //SetVSync( 0 );
         App.m_WinBase.Show( SW_MAXIMIZE ); // maximize the window
         int ret = App.RunGameLoop();  // enter the game loop, this differes from a regular windows message loop because it is built for speed
         App.SaveHighScores();
         App.SaveConfig();
         return ret;
      }
      return -1; // error creating window
   }
   catch( std::exception& e )
   {
      Alert( e.what() );
      return -1;
   }
   catch( ... )
   {
      Alert( "Unknown exception" );
      return -1;
   }
}
