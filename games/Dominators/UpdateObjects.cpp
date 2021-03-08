#include "main.h"

//****************************************************************************
//******************************* MainWindow::MoveMainTank *******************
// Accepts: void
//
// Purpose: move the tank that the player controlls if a d pad key is pressed
//          fire a shell if the fire button is pressed and the fire interval has passed
//
// Returns: void

void MainWindow::MoveMainTank()
{
   if( m_Keys[VK_UP] )
   {
      m_MainTank.m_PositionVector.m_y += m_MainTank.m_VelocityVector.m_y*m_deltaGameTime;
      if( m_MainTank.m_PositionVector.m_y > MainTank_boundary().top )
      {
         m_MainTank.m_PositionVector.m_y -= m_MainTank.m_VelocityVector.m_y*m_deltaGameTime;
      }
   }
   if( m_Keys[VK_DOWN] )
   {
      m_MainTank.m_PositionVector.m_y -= m_MainTank.m_VelocityVector.m_y*m_deltaGameTime;
      if( m_MainTank.m_PositionVector.m_y < MainTank_boundary().bottom )
      {
         m_MainTank.m_PositionVector.m_y += m_MainTank.m_VelocityVector.m_y*m_deltaGameTime;
      }
   }
   if( m_Keys[VK_LEFT] )
   {
      m_MainTank.m_PositionVector.m_x -= m_MainTank.m_VelocityVector.m_x*m_deltaGameTime;
      if( m_MainTank.m_PositionVector.m_x < MainTank_boundary().left )
      {
         m_MainTank.m_PositionVector.m_x += m_MainTank.m_VelocityVector.m_x*m_deltaGameTime;
      }
   }
   if( m_Keys[VK_RIGHT] )
   {
      m_MainTank.m_PositionVector.m_x += m_MainTank.m_VelocityVector.m_x*m_deltaGameTime;
      if( m_MainTank.m_PositionVector.m_x > MainTank_boundary().right )
      {
         m_MainTank.m_PositionVector.m_x -= m_MainTank.m_VelocityVector.m_x*m_deltaGameTime;
      }
   }

   if( m_Keys[VK_SPACE] )
   {
      if( m_MainTank.m_FireInterval.IntervalExceeded() )
      {
         m_MainTank.m_FireInterval.Begin();
         m_MainTank.AddShell();
         PlayClip( 0, MAIN_TANK_SHELL_CHANNEL );
      }
   }
}

//****************************************************************************
//******************************* MainWindow::UpdateFreeShells ***************
// Accepts: void
//
// Purpose: update the free shells, and remove them if they move off the screen
//
// Returns: void

void MainWindow::UpdateFreeShells()
{
   for( int i=0; i<m_FreeShells.size() ; i++ )
   {
      m_FreeShells[i].m_PositionVector.m_y += m_FreeShells[i].m_VelocityVector.m_y*m_deltaGameTime;
      if( m_FreeShells[i].m_PositionVector.m_y > Shells_boundary().top )
      {
         m_FreeShells.erase( m_FreeShells.begin()+i );
      }
   }
}

//****************************************************************************
//******************************* MainWindow::UpdateObjects ******************
// Accepts: void
//
// Purpose: nothing in the game should be moved unless this function tells it to
//          stop things from moving if the game is paused
//
// Returns: void

void MainWindow::UpdateObjects( void )
{
   if( !m_bGamePaused )
   {
      MoveMainTank();
      m_MainTank.UpdateShells( m_deltaGameTime, Shells_boundary() );
      UpdateEnemyTanks();
      UpdateFreeShells();
      UpdateExplosions();
      if( m_GameMode != BOSS_MODE )
      {
         if( (m_ATanks.size() == 0) && (m_BTanks.size() == 0) && (m_CTanks.size() == 0) && (m_DTanks.size() == 0) )
         {
            AddNewTanks();
         }
      }
      else if( m_GameMode == BOSS_MODE )
      {
         MoveBossTankA();
         m_BossTankA.UpdateShells( m_deltaGameTime, Shells_boundary() );
         if( m_BossTankA.m_FireInterval.IntervalExceeded() )
         {
            m_BossTankA.m_FireInterval.Begin();
            m_BossTankA.AddShell( Vector2D( 0.03, -0.03 ), Vector2D( 0.0, 0.0 ) );
            PlayClip( 0, MAIN_TANK_SHELL_CHANNEL );
         }
      }
   }
}

//****************************************************************************
//******************************* MainWindow::UpdateEnemyTanks ***************
// Accepts: void
//
// Purpose: update each enmy tank position and its shells positions
//          fire a shell if the fireinterval has been exceeded
//
// Returns: void

void MainWindow::UpdateEnemyTanks()
{
   for( int i=0; i<m_ATanks.size() ; i++ )
   {
      MoveTank( m_ATanks[i] );
      if( m_ATanks[i].m_FireInterval.IntervalExceeded() )
      {
         m_ATanks[i].m_FireInterval.Begin();
         m_ATanks[i].AddShell( Vector2D( 0.03, -0.03 ), Vector2D( 0.0, 0.0 ) );
         PlayClip( 0, MAIN_ATANK_SHELL_CHANNEL );
      }
      m_ATanks[i].UpdateShells( m_deltaGameTime, Shells_boundary() );
   }
   for( int i=0; i<m_BTanks.size() ; i++ )
   {
      MoveTank( m_BTanks[i] );
      if( m_BTanks[i].m_FireInterval.IntervalExceeded() )
      {
         m_BTanks[i].m_FireInterval.Begin();
         m_BTanks[i].AddShell( Vector2D( 0.03, -0.03 ), Vector2D( 0.0, 0.0 ) );
         PlayClip( 0, MAIN_BTANK_SHELL_CHANNEL );
      }
      m_BTanks[i].UpdateShells( m_deltaGameTime, Shells_boundary() );
   }
   for( int i=0; i<m_CTanks.size() ; i++ )
   {
      MoveTank( m_CTanks[i] );
      if( m_CTanks[i].m_FireInterval.IntervalExceeded() )
      {
         m_CTanks[i].m_FireInterval.Begin();
         m_CTanks[i].AddShell( Vector2D( 0.03, -0.03 ), Vector2D( 0.0, 0.0 ) );
         PlayClip( 0, MAIN_CTANK_SHELL_CHANNEL );
      }
      m_CTanks[i].UpdateShells( m_deltaGameTime, Shells_boundary() );
   }
   for( int i=0; i<m_DTanks.size() ; i++ )
   {
      MoveTank( m_DTanks[i] );
      if( m_DTanks[i].m_FireInterval.IntervalExceeded() )
      {
         m_DTanks[i].m_FireInterval.Begin();
         m_DTanks[i].AddShell( Vector2D( 0.03, -0.03 ), Vector2D( 0.0, 0.0 ) );
         PlayClip( 0, MAIN_DTANK_SHELL_CHANNEL );
      }
      m_DTanks[i].UpdateShells( m_deltaGameTime, Shells_boundary() );
   }
}

//****************************************************************************
//******************************* MainWindow::RestrictTankPosition ***********
// Accepts: C_Tank& Tank // The tank to restrict
//
// Purpose: make the enmy tank loop to the next side of the screen if it reaches the boundary
//
// Returns: void

void MainWindow::RestrictTankPosition( C_Tank& Tank )
{
   if( Tank.m_PositionVector.m_y < y_min_boundary() )
   {
      Tank.m_PositionVector.m_y = y_max_boundary();
   }

   if( Tank.m_PositionVector.m_x > x_max_boundary() )
   {
      Tank.m_PositionVector.m_x = x_min_boundary();
   }
   else if( Tank.m_PositionVector.m_x < x_min_boundary() )
   {
      Tank.m_PositionVector.m_x = x_max_boundary();
   }
}

//****************************************************************************
//******************************* MainWindow::MoveTank ***********************
// Accepts: C_Tank& Tank // The tank to move
//
// Purpose: The algorithm, for artificial intelegence for this game is pretty simple
//          give the tank a velocity and let it move
//          this ubdates the position of each tank on the queue but the equation
//          s = ut
//          also to restrict the position of the tank to the visable screen
//
// Returns: void

void MainWindow::MoveTank( C_Tank& Tank )
{
   Tank.m_PositionVector.m_y += Tank.m_VelocityVector.m_y*m_deltaGameTime;
   Tank.m_PositionVector.m_x += Tank.m_VelocityVector.m_x*m_deltaGameTime;
   RestrictTankPosition( Tank );
}

//****************************************************************************
//******************************* MainWindow::MoveBossTankA ******************
// Accepts: void
//
// Purpose: The algorithm, for artificial intelegence for this game is pretty simple
//          give the tank a velocity and let it move
//          this ubdates the position of each tank on the queue but the equation
//          s = ut
//          also to restrict the position of the tank to the visable screen
//
// Returns: void

void MainWindow::MoveBossTankA()
{
   // s = ut
   // t = m_deltaGameTime
   // u = initial velocity // since there is no acceleration the velocity does not change
   // s = distance // in this case the position vector
   m_BossTankA.m_PositionVector.m_y += m_BossTankA.m_VelocityVector.m_y*m_deltaGameTime;
   m_BossTankA.m_PositionVector.m_x += m_BossTankA.m_VelocityVector.m_x*m_deltaGameTime;
   RestrictTankPosition( m_BossTankA );
}

//****************************************************************************
//******************************* MainWindow::UpdateExplosions ***************
// Accepts: void
//
// Purpose: subtract the game delta time from the explosions 
//                   if the time is less than or equal to zero
//                   remove the explosion
//
// Returns: void

void MainWindow::UpdateExplosions( void )
{
   for( int i=0; i<m_Explosions.size() ; i++ )
   {
      m_Explosions[i].m_duration -= m_deltaGameTime; // subtract the game delta time
      if( m_Explosions[i].m_duration <= 0 )
      {
         m_Explosions.erase( m_Explosions.begin()+i ); // remove the explosions
      }
   }
}

