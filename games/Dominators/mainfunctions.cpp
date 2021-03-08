#include "main.h"

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

   do
   {
      if( !m_Font.BuildFont( m_WinBase ) )
      {
         ret = Retry( std::string( "Could not create font\n" + GetLastSystemError() ) + "Would you like to retry" );
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

   LoadTextures();
   //LoadMap( "dominators.tm" );
   LoadMap( "dominators.btm" );
   LoadSounds();
}

//***************************************************************************
//******************************* MainWindow::InitialiseNewGame *************
// Accepts: void
//
// Purpose: Reset timers, tank life, and provide a consistent envrionment for the start of a new game
//
// Returns: void

void MainWindow::InitialiseNewGame()
{
   m_AddEnmyRowInterval.Begin();

   // set the MainTank velocity and position
   m_MainTank.m_VelocityVector = Vector2D( 0.03, 0.03 );
   m_MainTank.m_PositionVector = Vector2D( 0.03, -35.53 );
   m_MainTank.m_Z              = m_Z;
   m_MainTank.m_Shells.clear();
   m_MainTank.m_Life = 100;

   RemoveEnmyTanks();

   // give each other Tank a "personality" by assigning them seperate velocities
   AddNewATank();
   AddNewBTank();
   AddNewCTank();
   AddNewDTank();

   m_BossTankA.m_VelocityVector = Vector2D( 0.03, -0.03 );
   m_BossTankA.m_PositionVector = Vector2D( 0.03, -35.53 );
   m_BossTankA.m_Z              = m_Z;
   m_BossTankA.m_Shells.clear();
   m_BossTankA.m_FireInterval.Begin();
   m_BossTankA.m_Life = 200;

   m_FreeShells.clear();
   m_Explosions.clear();
}

//***************************************************************************
//******************************* MainWindow::AddNewATank *******************
// Accepts: void
//
// Purpose: Add a tank to the A tanks list and give it the characteristics of a A tank
//
// Returns: void

bool MainWindow::AddNewATank()
{
   m_ATanks.push_back(C_Tank( 6.0, 6.0, 3000 ));
   m_ATanks.back().m_VelocityVector    = Vector2D(   0.01, -0.03 );
   m_ATanks.back().m_PositionVector    = Vector2D( -25.0, y_max_boundary() );
   m_ATanks.back().m_Z                 = m_Z;
   m_ATanks.back().SetLife( 9 );
   return true;
}

//***************************************************************************
//******************************* MainWindow::AddNewBTank *******************
// Accepts: void
//
// Purpose: Add a tank to the B tanks list and give it the characteristics of a B tank
//
// Returns: void

bool MainWindow::AddNewBTank()
{
   m_BTanks.push_back(C_Tank( 6.0, 6.0, 2500 ));
   m_BTanks.back().m_VelocityVector    = Vector2D( -0.005, -0.01 );
   m_BTanks.back().m_PositionVector    = Vector2D( -7.03, y_max_boundary() );
   m_BTanks.back().m_Z                 = m_Z;
   m_BTanks.back().SetLife( 9 );
   return true;
}

//***************************************************************************
//******************************* MainWindow::AddNewCTank *******************
// Accepts: void
//
// Purpose: Add a tank to the C tanks list and give it the characteristics of a C tank
//
// Returns: void

bool MainWindow::AddNewCTank()
{
   m_CTanks.push_back(C_Tank( 6.0, 6.0, 2000 ));
   m_CTanks.back().m_VelocityVector    = Vector2D( 0.02, -0.002 );
   m_CTanks.back().m_PositionVector    = Vector2D( 7.03, y_max_boundary() );
   m_CTanks.back().m_Z                 = m_Z;
   m_CTanks.back().SetLife( 9 );
   return true;
}

//***************************************************************************
//******************************* MainWindow::AddNewDTank *******************
// Accepts: void
//
// Purpose: Add a tank to the D tanks list and give it the characteristics of a D tank
//
// Returns: void

bool MainWindow::AddNewDTank()
{
   m_DTanks.push_back(C_Tank( 6.0, 6.0, 1000 ));
   m_DTanks.back().m_VelocityVector    = Vector2D( -0.005, -0.005 );
   m_DTanks.back().m_PositionVector    = Vector2D( 25.03, y_max_boundary() );
   m_DTanks.back().m_Z                 = m_Z;
   m_DTanks.back().SetLife( 9 );
   return true;
}

//***************************************************************************
//******************************* MainWindow::RemoveEnmyTanks ***************
// Accepts: void
//
// Purpose: remove all the enmy tanks except the boss
//          so you dont start a new game with more thanks than there should be 
//
// Returns: void

void MainWindow::RemoveEnmyTanks()
{
   m_ATanks.clear();
   m_BTanks.clear();
   m_CTanks.clear();
   m_DTanks.clear();
}

//***************************************************************************
//******************************* MainWindow::AddNewTanks *******************
// Accepts: void
//
// Purpose: Add a new row of enmies and check to see if the max amount of enmies have been added
//          If the max amount of enmies have been added it changes the ghame mode to boss mode
//
// Returns: void

void MainWindow::AddNewTanks()
{
   if( m_RowsOfTanksAdded >= m_MaxEnmyRows )
   {
      m_RowsOfTanksAdded = 1;
      m_GameMode = BOSS_MODE;
      m_MainTank.m_Shells.clear();
      m_BossTankA.m_Shells.clear();
      RemoveEnmyTanks();
      m_FreeShells.clear();
      return;
   }
   AddNewATank();
   AddNewBTank();
   AddNewCTank();
   AddNewDTank();
   m_RowsOfTanksAdded++;
}

//***************************************************************************
//******************************* MainWindow::AddNewATank *******************
// Accepts: the tank to get the shells from
//
// Purpose: Add the shells that came from a tank that is about to be destroyed
//          so the shells dont dissapear when the tank is gone
//
// Returns: void

void MainWindow::GetFreeShells( C_Tank &Tank )
{
   m_FreeShells.insert( m_FreeShells.end(), Tank.m_Shells.begin(), Tank.m_Shells.end() );
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
   ::glDepthFunc( GL_LEQUAL );   // The Type Of Depth Testing (Less Or Equal)
   ::glEnable( GL_DEPTH_TEST );
//      m_p1Console.LogThisLine( " glEnable( GL_DEPTH_TEST ) " + m_GLErrorStrings.GetError(), " gl" );
   ::glShadeModel( GL_SMOOTH );  // Select Smooth Shading
   ::glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
//      ::glBlendFunc( GL_ONE, GL_SRC_ALPHA ); // Set Blending Mode (Cheap / Quick)
   ::glEnable( GL_BLEND );
//   ::glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );
   ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   ::glClearDepth( 1.0f );
   ::glLoadIdentity();
//   ::glColor4f( 1.0, 1.0, 1.0, 0.0 );
}

//***************************************************************************
//******************************* MainWindow::AddExplosion ******************
// Accepts: Vector2D PositionVector,  the position of the explosion
//          double duration,            the duration the explosion should last
//          float width,                the width of the explosion
//          float height                the height of the explosion
//
// Purpose: Push an explosion onto the deque and it will be rendered with all the other explosions.
//
// Returns: void

void MainWindow::AddExplosion( Vector2D PositionVector, double duration, float width, float height )
{
   m_Explosions.push_back( C_Explosion( PositionVector, duration, width, height ) );
}

//***************************************************************************
//******************************* MainWindow::SaveIntervals *****************
// Accepts: void
//
// Purpose: TimeInterval records the amount of time elasped since TimeInterval::Begin() is called.
//          when the game is paused time is still being recorded so when you unpause the time elasped will be grater than it should be.
//          This function saves the time elasped since the game was paused and then resumes from that time when the game is unpaused.
//
// Returns: void

void MainWindow::SaveIntervals( void )
{
   if( m_bGamePaused )
   {
      m_AddEnmyRowInterval.SaveTime();
      for( int i=0; i<m_ATanks.size() ; i++ )
      {
         m_ATanks[i].m_FireInterval.SaveTime();
      }
      for( int i=0; i<m_BTanks.size() ; i++ )
      {
         m_BTanks[i].m_FireInterval.SaveTime();
      }
      for( int i=0; i<m_CTanks.size() ; i++ )
      {
         m_CTanks[i].m_FireInterval.SaveTime();
      }
      for( int i=0; i<m_DTanks.size() ; i++ )
      {
         m_DTanks[i].m_FireInterval.SaveTime();
      }
   }
   else
   {
      m_AddEnmyRowInterval.Begin( m_AddEnmyRowInterval.GetSavedTime() );
      for( int i=0; i<m_ATanks.size() ; i++ )
      {
         m_ATanks[i].m_FireInterval.Begin( m_ATanks[i].m_FireInterval.GetSavedTime() );
      }
      for( int i=0; i<m_BTanks.size() ; i++ )
      {
         m_BTanks[i].m_FireInterval.Begin( m_BTanks[i].m_FireInterval.GetSavedTime() );
      }
      for( int i=0; i<m_CTanks.size() ; i++ )
      {
         m_CTanks[i].m_FireInterval.Begin( m_CTanks[i].m_FireInterval.GetSavedTime() );
      }
      for( int i=0; i<m_DTanks.size() ; i++ )
      {
         m_DTanks[i].m_FireInterval.Begin( m_DTanks[i].m_FireInterval.GetSavedTime() );
      }
   }
}

