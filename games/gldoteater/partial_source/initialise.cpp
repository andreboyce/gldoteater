#include "main.h"

void MainWindow::InitialiseServerOnlineGame()
{
   m_Mutex.Lock();
   m_Players.clear();
   if( m_Players.size() == 0 )
   {
      m_Players.push_back( C_DotEater( Vector2D( 0.0, 0.0 ), 
                           Vector2D( 0.08, 0.08 ), 
                           C_GLTexQuad( 1.0, 1.0 ), 
                           LEFT, 
                           std::vector<AStar::AStar2d::PointAndDirection>(), 
                           INITIAL_LIVES,
                           false,
                           0,
                           0,
                           ControllKeys( VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT ),
                           C_Flash( false, 250 ),
                           0,
                           true,
                           0,
                           C_DotEater::ItemInventory(),
                           LEFT,
                           Structs::Point(),
                           C_DotEater::EAT_DOT,
                           m_Players.size(),
                           0 ) );

      m_Players.push_back( C_DotEater( Vector2D( 0.0, 0.0 ), 
                           Vector2D( 0.08, 0.08 ), 
                           C_GLTexQuad( 1.0, 1.0 ), 
                           LEFT, 
                           std::vector<AStar::AStar2d::PointAndDirection>(), 
                           INITIAL_LIVES,
                           false,
                           0,
                           0,
                           ControllKeys( -1, -1, -1, -1 ),
                           C_Flash( false, 250 ),
                           0,
                           true,
                           0,
                           C_DotEater::ItemInventory(),
                           LEFT,
                           Structs::Point(),
                           C_DotEater::EAT_DOT,
                           m_Players.size(),
                           0 ) );
   }
   m_GameMode = GAME_IN_PROGRESS;
   if( m_bShowOnlineGameMode )
      m_Console.AddLine( GetGamemodeString() );
   m_Mutex.Release();

   PlayClip( SOUND_PACMAN, SOUND_PACMAN );
   m_Level = 0;
   InitialiseNewGame();
   m_Mutex.Lock();
   NetorkCommandsQueue.push_back( COMMAND_START );
   m_Mutex.Release();
}

void MainWindow::InitialiseClientOnlineGame()
{
   m_Mutex.Lock();
   m_Players.clear();
   if( m_Players.size() == 0 )
   {
      m_Players.push_back( C_DotEater( Vector2D( 0.0, 0.0 ), 
                           Vector2D( 0.08, 0.08 ), 
                           C_GLTexQuad( 1.0, 1.0 ), 
                           LEFT, 
                           std::vector<AStar::AStar2d::PointAndDirection>(), 
                           INITIAL_LIVES,
                           false,
                           0,
                           0,
                           ControllKeys( -1, -1, -1, -1 ),
                           C_Flash( false, 250 ),
                           0,
                           true,
                           0,
                           C_DotEater::ItemInventory(),
                           LEFT,
                           Structs::Point(),
                           C_DotEater::EAT_DOT,
                           m_Players.size(),
                           0 ) );
      m_Players.push_back( C_DotEater( Vector2D( 0.0, 0.0 ), 
                           Vector2D( 0.08, 0.08 ), 
                           C_GLTexQuad( 1.0, 1.0 ), 
                           LEFT, 
                           std::vector<AStar::AStar2d::PointAndDirection>(), 
                           INITIAL_LIVES,
                           false,
                           0,
                           0,
                           ControllKeys( VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT ),
                           C_Flash( false, 250 ),
                           0,
                           true,
                           0,
                           C_DotEater::ItemInventory(),
                           LEFT,
                           Structs::Point(),
                           C_DotEater::EAT_DOT,
                           m_Players.size(),
                           0 ) );
   }

   m_GameMode               = GAME_IN_PROGRESS;
   if( m_bShowOnlineGameMode )
      m_Console.AddLine( GetGamemodeString() );
   PlayClip( SOUND_PACMAN, SOUND_PACMAN );
   m_Mutex.Release();
   m_Level                  = 0;
   InitialiseNewGame();
   CheckCommand( "/clear" );
}

//***************************************************************************
//******************************* MainWindow::InitialiseNewOnlineRound ******
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::InitialiseNewOnlineRound()
{
   m_bContinueWithNextPlayersCollisionDetection = true;
   if( m_Players.size() > 1 )
   {
      if( m_MultiPlayerMaps.size() > m_Level )
      {
         LoadMap( m_MultiPlayerMaps[m_Level] );
         m_CurrentMap = m_Level;
      }
   }
   std::deque<Structs::Point> PowerPellets;
   if( m_TileMap.m_SpecialData.size() >= 5 )
   {
      float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
            y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();

      m_Mutex.Lock();

      ResetPlayer1Position();
      ResetPlayer2Position();
      ResetRGhostPosition();
      ResetBGhostPosition();
      ResetYGhostPosition();
      ResetWGhostPosition();

      for( int i=0; i<m_Players.size() ;i++ )
      {
         m_Players[i].m_bIsPoweredUp   = false;
         m_Players[i].m_InvincibleTime = INVINCIBLE_TIME;
         m_Players[i].m_bIsInvincible  = true;
      }
      LoadAnimations();

      m_WhiteGhost.m_bIsPoweredDown = false;
      m_WhiteGhost.m_bIsEaten       = false;

      m_BlueGhost.m_bIsPoweredDown = false;
      m_BlueGhost.m_bIsEaten       = false;
      
      m_RedGhost.m_bIsPoweredDown = false;
      m_RedGhost.m_bIsEaten       = false;

      m_YellowGhost.m_bIsPoweredDown = false;
      m_YellowGhost.m_bIsEaten       = false;
      m_Mutex.Release();
      GetBigDots( PowerPellets );
   }
   AddDots( PowerPellets );

   if( m_Players.size() )
   {
      m_Mutex.Lock();
      GetPathAndDirection( m_WhiteGhost,   GetRandomTile() );
      GetPathAndDirection( m_BlueGhost,   GetRandomTile() );
      GetPathAndDirection( m_RedGhost,    GetRandomTile() );
      GetPathAndDirection( m_YellowGhost, GetRandomTile() );
      m_Mutex.Release();
   }
   m_Mutex.Lock();
   m_DotsEaten.clear();
   m_Mutex.Release();
}


//***************************************************************************
//******************************* MainWindow::InitialiseNewRound ************
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::InitialiseNewRound()
{
   m_bContinueWithNextPlayersCollisionDetection = true;
   if( m_TileMap.m_SpecialData.size() >= 5 )
   {
      float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
            y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();

      m_Mutex.Lock();

      ResetPlayer1Position();
      ResetPlayer2Position();
      ResetRGhostPosition();
      ResetBGhostPosition();
      ResetYGhostPosition();
      ResetWGhostPosition();


      for( int i=0; i<m_Players.size() ;i++ )
      {
         m_Players[i].m_bIsPoweredUp = false;
         m_Players[i].m_InvincibleTime = INVINCIBLE_TIME;
         m_Players[i].m_bIsInvincible  = true;
      }
      LoadAnimations();

      m_WhiteGhost.m_bIsPoweredDown = false;
      m_WhiteGhost.m_bIsEaten       = false;

      m_BlueGhost.m_bIsPoweredDown = false;
      m_BlueGhost.m_bIsEaten       = false;
      
      m_RedGhost.m_bIsPoweredDown = false;
      m_RedGhost.m_bIsEaten       = false;

      m_YellowGhost.m_bIsPoweredDown = false;
      m_YellowGhost.m_bIsEaten       = false;
      m_Mutex.Release();
   }

   m_Mutex.Lock();
   GetPathAndDirection( m_WhiteGhost,  GetRandomTile() );
   GetPathAndDirection( m_BlueGhost,   GetRandomTile() );
   GetPathAndDirection( m_RedGhost,    GetRandomTile() );
   GetPathAndDirection( m_YellowGhost, GetRandomTile() );
   m_DotsEaten.clear();
   m_Mutex.Release();
}

// Accepts: void
//
// Purpose: Reset timers, tank life, and provide a consistent envrionment for the start of a new game
//
// Returns: voidad

void MainWindow::InitialiseNewGame()
{
   try
   {
      m_bMoveGhosts                                = true;
      m_bContinueWithNextPlayersCollisionDetection = true;
      if( m_Players.size() == 1 )
      {
         if( m_SinglePlayerMaps.size() > m_Level )
         {
            LoadMap( m_SinglePlayerMaps[m_Level] );
            m_CurrentMap = m_Level;
         }
      }
      else if( m_Players.size() > 1 )
      {
         if( m_MultiPlayerMaps.size() > m_Level )
         {
            LoadMap( m_MultiPlayerMaps[m_Level] );
            m_CurrentMap = m_Level;
         }
      }
      std::deque<Structs::Point> PowerPellets;
      if( m_TileMap.m_SpecialData.size() >= 5 )
      {
         float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
               y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();

         m_Mutex.Lock();

         ResetPlayer1Position();
         ResetPlayer2Position();
         ResetRGhostPosition();
         ResetBGhostPosition();
         ResetYGhostPosition();
         ResetWGhostPosition();


         for( int i=0; i<m_Players.size() ;i++ )
         {
            m_Players[i].m_bIsPoweredUp   = false;
            m_Players[i].m_InvincibleTime = INVINCIBLE_TIME;
            m_Players[i].m_bIsInvincible  = true;
            m_Players[i].m_Lives          = INITIAL_LIVES;
         }
         LoadAnimations();

         m_WhiteGhost.m_bIsPoweredDown = false;
         m_WhiteGhost.m_bIsEaten       = false;

         m_BlueGhost.m_bIsPoweredDown = false;
         m_BlueGhost.m_bIsEaten       = false;
      
         m_RedGhost.m_bIsPoweredDown = false;
         m_RedGhost.m_bIsEaten       = false;

         m_YellowGhost.m_bIsPoweredDown = false;
         m_YellowGhost.m_bIsEaten       = false;
         m_Mutex.Release();

         GetBigDots( PowerPellets );
      }
      AddDots( PowerPellets );

      if( m_Players.size() )
      {
         m_Mutex.Lock();
         GetPathAndDirection( m_WhiteGhost,  GetRandomTile() );
         GetPathAndDirection( m_BlueGhost,   GetRandomTile() );
         GetPathAndDirection( m_RedGhost,    GetRandomTile() );
         GetPathAndDirection( m_YellowGhost, GetRandomTile() );
         m_Mutex.Release();
      }
      m_Mutex.Lock();
      m_DotsEaten.clear();
      m_Mutex.Release();
   }
   catch( std::exception& e )
   {
      Alert( e.what() );
   }
   catch( ... )
   {
   }
}

//***************************************************************************
//******************************* MainWindow::Initialise ********************
// Accepts: void
//
// Purpose: Initialise the graphics engine, sound engine and load textures and sounds
//
// Returns: void

void MainWindow::Initialise()
{
   int ret = IDRETRY;

   do
   {
      if( !EnableOpenGL() )
      {
         ret = Retry( std::string("Unable to initialize OpenGL\n") + GetLastSystemError() + "Would you like to retry" );
      }
      else
      {
         break;
      }
   } while( ret == IDRETRY );

   if( ret == IDCANCEL )
   {
      Alert( "Program exiting" );
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
   }

   InitialiseOpenGL();

   m_Font.BuildFont( m_DataDirectory+"\\texture\\font.dds", RGB( 0, 0, 0 ), 0.65, 16.0, 20 );
   m_TextureManager.LoadTAI( m_DataDirectory+"\\texture.tai" );
   LoadTextures();
   LoadSounds();
   LoadQuickMessages();
   LoadMapsFilenames();
   if( m_MapFilenames.size() > TITLE_MAP  )
   {
      LoadMap( m_MapFilenames[TITLE_MAP] );
   }
   InitializeListBoxes();
   InitializeButtons();
   m_TitleScreenOptionsListBox.MoveCursorOnDownKey(); // 2nd option selected first
   LoadHighScores();
}

//***************************************************************************
//******************************* MainWindow::InitialiseOpenGL **************
// Accepts: void
//
// Purpose: set up some intitial opengl state variables
//
// Returns: void

void MainWindow::InitialiseOpenGL()
{
//   ::glEnable( GL_DEPTH_TEST );
//   ::glDepthFunc( GL_LEQUAL );  // The Type Of Depth Testing (Less Or Equal)
   ::glLoadIdentity();
   ::glDepthMask( 0 );
   ::glDisable( GL_DEPTH_TEST );
   ::glShadeModel( GL_FLAT );
   ::glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST );
   ::glEnable( GL_BLEND );
   ::glClearDepth( 1.0f );
   ::glEnable( GL_CULL_FACE );
   ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   ::glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ); // black
   ::glColor4f( 1.0, 1.0, 1.0, 1.0 );        // white

   //init_depth_buffer_trick();
   glDrawRangeElements = NULL;
   glGenBuffersARB     = NULL;
   glBindBufferARB     = NULL;
   glBufferDataARB     = NULL;
   glDeleteBuffersARB  = NULL;

   if( IsExtensionSupported( "GL_ARB_vertex_buffer_object" ) )
   {
      // Get Pointers To The GL Functions
      glGenBuffersARB     = (PFNGLGENBUFFERSARBPROC)     ::wglGetProcAddress( "glGenBuffersARB"    );
      glBindBufferARB     = (PFNGLBINDBUFFERARBPROC)     ::wglGetProcAddress( "glBindBufferARB"    );
      glBufferDataARB     = (PFNGLBUFFERDATAARBPROC)     ::wglGetProcAddress( "glBufferDataARB"    );
      glDeleteBuffersARB  = (PFNGLDELETEBUFFERSARBPROC)  ::wglGetProcAddress( "glDeleteBuffersARB" );
      glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) ::wglGetProcAddress( "glDrawRangeElements" );
   }
}
