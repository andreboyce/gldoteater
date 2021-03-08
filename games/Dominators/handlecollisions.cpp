#include "main.h"

//***************************************************************************
//******************************* MainWindow::HandleCollisions **************
// Accepts: void
//
// Purpose: All Collision dection occurs here
//          causes explosion when tanks are destroyed
//          causes shells to be removed when they have collided with enmys
//          causes tanks to be removed when they have exploded
//          changes the game mode if the main tank is destroyed to the game over mode
//          changes the game mode to you_win_mode if the boss is destroyed
//
// Returns: void

void MainWindow::HandleCollisions()
{
   // MainTank Shells Collision detection
   CheckATankCollisions();
   CheckBTankCollisions();
   CheckCTankCollisions();
   CheckDTankCollisions();
   /*
   for( int c=0 ; c<m_MainTank.m_Shells.size() ; c++ )
   {
      for( int i=0; i<m_ATanks.size() ; i++ )
      {
         if( CheckCollide( m_MainTank.m_Shells[c].Rect(), m_ATanks[i].Rect() ) )
         {
            GetFreeShells( m_ATanks[i] );
            AddExplosion( m_ATanks[i].m_PositionVector, 400, m_ATanks[i].m_TexQuad.width(), m_ATanks[i].m_TexQuad.height() );
            m_ATanks.erase( m_ATanks.begin()+i );
            m_MainTank.m_Shells.erase( m_MainTank.m_Shells.begin()+c );
            PlayClip( 1, MAIN_TANK_SHELL_CHANNEL );
         }
      }	  
      for( int i=0; i<m_BTanks.size() ; i++ )
      {
         if( CheckCollide( m_MainTank.m_Shells[c].Rect(), m_BTanks[i].Rect() ) )
         {
            GetFreeShells( m_BTanks[i] );
            AddExplosion( m_BTanks[i].m_PositionVector, 400, m_BTanks[i].m_TexQuad.width(), m_BTanks[i].m_TexQuad.height() );
            m_BTanks.erase( m_BTanks.begin()+i );
            m_MainTank.m_Shells.erase( m_MainTank.m_Shells.begin()+c );
            PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
         }
      }
      for( int i=0; i<m_CTanks.size() ; i++ )
      {
         if( CheckCollide( m_MainTank.m_Shells[c].Rect(), m_CTanks[i].Rect() ) )
         {
            GetFreeShells( m_CTanks[i] );
            AddExplosion( m_CTanks[i].m_PositionVector, 400, m_CTanks[i].m_TexQuad.width(), m_CTanks[i].m_TexQuad.height() );
            m_CTanks.erase( m_CTanks.begin()+i );
            m_MainTank.m_Shells.erase( m_MainTank.m_Shells.begin()+c );
            PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
         }
      }
      for( int i=0; i<m_DTanks.size() ; i++ )
      {
         if( CheckCollide( m_MainTank.m_Shells[c].Rect(), m_DTanks[i].Rect() ) )
         {
            GetFreeShells( m_DTanks[i] );
            AddExplosion( m_DTanks[i].m_PositionVector, 400, m_DTanks[i].m_TexQuad.width(), m_DTanks[i].m_TexQuad.height() );
            m_DTanks.erase( m_DTanks.begin()+i );
            m_MainTank.m_Shells.erase( m_MainTank.m_Shells.begin()+c );
            PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
         }
      }
   }*/

   // MainTank-EnmyTank and MainTank-EnmyTankShells Collision detection
   for( int i=0; i<m_ATanks.size() ; i++ )
   {
      for( int c=0; c<m_ATanks[i].m_Shells.size() ; c++ )
      {
         if( CheckCollide( m_MainTank.Rect(), m_ATanks[i].m_Shells[c].Rect() ) )
         {
            AddExplosion( m_ATanks[i].m_Shells[c].m_PositionVector, 400, m_ATanks[i].m_Shells[c].m_TexQuad.width(), m_ATanks[i].m_Shells[c].m_TexQuad.height() );
            m_ATanks[i].m_Shells.erase( m_ATanks[i].m_Shells.begin()+c ); 
            m_MainTank.ReduceLife( m_ShellDamage );
            if( m_MainTank.CheckForNoLife( 100.0 ) )
            {
               AddExplosion( m_MainTank.m_PositionVector, 400, m_MainTank.m_TexQuad.width(), m_MainTank.m_TexQuad.height() );
               m_GameMode = GAME_OVER;
            }
            PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
         }
      }
      if( CheckCollide( m_MainTank.Rect(), m_ATanks[i].Rect() ) )
      {
         GetFreeShells( m_ATanks[i] );
         m_ATanks.erase( m_ATanks.begin()+i );
         m_MainTank.ReduceLife( m_ShellDamage );
         if( m_MainTank.CheckForNoLife( 100.0 ) )
         {
            AddExplosion( m_MainTank.m_PositionVector, 400, m_MainTank.m_TexQuad.width(), m_MainTank.m_TexQuad.height() );
            m_GameMode = GAME_OVER;
         }
         PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
      }
   }
   for( int i=0; i<m_BTanks.size() ; i++ )
   {
      for( int c=0; c<m_BTanks[i].m_Shells.size() ; c++ )
      {
         if( CheckCollide( m_MainTank.Rect(), m_BTanks[i].m_Shells[c].Rect() ) )
         {
            AddExplosion( m_BTanks[i].m_Shells[c].m_PositionVector, 400, m_BTanks[i].m_Shells[c].m_TexQuad.width(), m_BTanks[i].m_Shells[c].m_TexQuad.height() );
            m_BTanks[i].m_Shells.erase( m_BTanks[i].m_Shells.begin()+c );
            m_MainTank.ReduceLife( m_ShellDamage );
            if( m_MainTank.CheckForNoLife( 100.0 ) )
            {
               AddExplosion( m_MainTank.m_PositionVector, 400, m_MainTank.m_TexQuad.width(), m_MainTank.m_TexQuad.height() );
               m_GameMode = GAME_OVER;
            }
            PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
         }
      }
      if( CheckCollide( m_MainTank.Rect(), m_BTanks[i].Rect() ) )
      {
         GetFreeShells( m_BTanks[i] );
         m_BTanks.erase( m_BTanks.begin()+i );
//         m_GameMode = GAME_OVER;
         m_MainTank.ReduceLife( m_ShellDamage );
         if( m_MainTank.CheckForNoLife( 100.0 ) )
         {
            AddExplosion( m_MainTank.m_PositionVector, 400, m_MainTank.m_TexQuad.width(), m_MainTank.m_TexQuad.height() );
            m_GameMode = GAME_OVER;
         }
         PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
      }
   }
   for( int i=0; i<m_CTanks.size() ; i++ )
   {
      for( int c=0; c<m_CTanks[i].m_Shells.size() ; c++ )
      {
         if( CheckCollide( m_MainTank.Rect(), m_CTanks[i].m_Shells[c].Rect() ) )
         {
            AddExplosion( m_CTanks[i].m_Shells[c].m_PositionVector, 400, m_CTanks[i].m_Shells[c].m_TexQuad.width(), m_CTanks[i].m_Shells[c].m_TexQuad.height() );
            m_CTanks[i].m_Shells.erase( m_CTanks[i].m_Shells.begin()+c );
            m_MainTank.ReduceLife( m_ShellDamage );
            if( m_MainTank.CheckForNoLife( 100.0 ) )
            {
               AddExplosion( m_MainTank.m_PositionVector, 400, m_MainTank.m_TexQuad.width(), m_MainTank.m_TexQuad.height() );
               m_GameMode = GAME_OVER;
            }
            PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
         }
      }
      if( CheckCollide( m_MainTank.Rect(), m_CTanks[i].Rect() ) )
      {
         GetFreeShells( m_CTanks[i] );
         m_CTanks.erase( m_CTanks.begin()+i );
         m_MainTank.ReduceLife( m_ShellDamage );
         if( m_MainTank.CheckForNoLife( 100.0 ) )
         {
            AddExplosion( m_MainTank.m_PositionVector, 400, m_MainTank.m_TexQuad.width(), m_MainTank.m_TexQuad.height() );
            m_GameMode = GAME_OVER;
         }
         PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
      }
   }
   for( int i=0; i<m_DTanks.size() ; i++ )
   {
      for( int c=0; c<m_DTanks[i].m_Shells.size() ; c++ )
      {
         if( CheckCollide( m_MainTank.Rect(), m_DTanks[i].m_Shells[c].Rect() ) )
         {
            AddExplosion( m_DTanks[i].m_Shells[c].m_PositionVector, 400, m_DTanks[i].m_Shells[c].m_TexQuad.width(), m_DTanks[i].m_Shells[c].m_TexQuad.height() );
            m_DTanks[i].m_Shells.erase( m_DTanks[i].m_Shells.begin()+c );
            m_MainTank.ReduceLife( m_ShellDamage );
            if( m_MainTank.CheckForNoLife( 100.0 ) )
            {
               AddExplosion( m_MainTank.m_PositionVector, 400, m_MainTank.m_TexQuad.width(), m_MainTank.m_TexQuad.height() );
               m_GameMode = GAME_OVER;
            }
            PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
         }
      }
      if( CheckCollide( m_MainTank.Rect(), m_DTanks[i].Rect() ) )
      {
         GetFreeShells( m_DTanks[i] );
         m_DTanks.erase( m_DTanks.begin()+i );
         m_MainTank.ReduceLife( m_ShellDamage );
         if( m_MainTank.CheckForNoLife( 100.0 ) )
         {
            AddExplosion( m_MainTank.m_PositionVector, 400, m_MainTank.m_TexQuad.width(), m_MainTank.m_TexQuad.height() );
            m_GameMode = GAME_OVER;
         }
         PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
      }
   }

   // Maintank - Boss stuff 
   if( m_GameMode == BOSS_MODE )
   {
      if( CheckCollide( m_MainTank.Rect(), m_BossTankA.Rect() ) )
      {
         m_MainTank.ReduceLife( m_ShellDamage );
         if( m_MainTank.CheckForNoLife( 100.0 ) )
         {
            m_GameMode = GAME_OVER;
         }
         PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
      }
      for( int c=0; c<m_MainTank.m_Shells.size() ; c++ )
      {
         if( CheckCollide( m_BossTankA.Rect(), m_MainTank.m_Shells[c].Rect() ) )
         {
            AddExplosion( m_MainTank.m_Shells[c].m_PositionVector, 400, m_MainTank.m_Shells[c].m_TexQuad.width(), m_MainTank.m_Shells[c].m_TexQuad.height() );
            m_MainTank.m_Shells.erase( m_MainTank.m_Shells.begin()+c );
            m_BossTankA.ReduceLife( m_ShellDamage );
            if( m_BossTankA.CheckForNoLife( 200.0 ) )
            {
               AddExplosion( m_BossTankA.m_PositionVector, 1000, m_BossTankA.m_TexQuad.width(), m_BossTankA.m_TexQuad.height() );
               m_GameMode = YOU_WIN_MODE;
               PlayClip( 3, YOU_WIN_CHANNEL );
               m_bYouWinMusicPlayed = false; // the game loop will allow it to be played now
               PlayClip( 2, BOSS_EXPLOSION_CHANNEL );
            }
            PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL2 );
         }
      }
      for( int c=0; c<m_BossTankA.m_Shells.size() ; c++ )
      {
         if( CheckCollide( m_MainTank.Rect(), m_BossTankA.m_Shells[c].Rect() ) )
         {
            m_BossTankA.m_Shells.erase( m_BossTankA.m_Shells.begin()+c );
            m_MainTank.ReduceLife( m_ShellDamage );
            if( m_MainTank.CheckForNoLife( 100.0 ) )
            {
               AddExplosion( m_MainTank.m_PositionVector, 400, m_MainTank.m_TexQuad.width(), m_MainTank.m_TexQuad.height() );
               m_GameMode = GAME_OVER;
            }
            PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
         }
      }
   }

   // freeshells - MainTank stuff
   for( int i=0; i<m_FreeShells.size() ; i++ )
   {
      if( CheckCollide( m_MainTank.Rect(), m_FreeShells[i].Rect() ) )
      {
         AddExplosion( m_FreeShells[i].m_PositionVector, 400, m_FreeShells[i].m_TexQuad.width(), m_FreeShells[i].m_TexQuad.height() );
         m_FreeShells.erase( m_FreeShells.begin()+i );
         m_MainTank.ReduceLife( m_ShellDamage );
         if( m_MainTank.CheckForNoLife( 100.0 ) )
         {
            AddExplosion( m_MainTank.m_PositionVector, 400, m_MainTank.m_TexQuad.width(), m_MainTank.m_TexQuad.height() );
            m_GameMode = GAME_OVER;
         }
         PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
      }
   }   
}

void MainWindow::CheckATankCollisions()
{
   for( int c=0 ; c<m_MainTank.m_Shells.size() ; c++ )
   {
      for( int i=0; i<m_ATanks.size() ; i++ )
      {
         if( CheckCollide( m_MainTank.m_Shells[c].Rect(), m_ATanks[i].Rect() ) )
         {
            GetFreeShells( m_ATanks[i] );
            AddExplosion( m_ATanks[i].m_PositionVector, 400, m_ATanks[i].m_TexQuad.width(), m_ATanks[i].m_TexQuad.height() );
            m_ATanks.erase( m_ATanks.begin()+i );
            m_MainTank.m_Shells.erase( m_MainTank.m_Shells.begin()+c );
            PlayClip( 1, MAIN_TANK_SHELL_CHANNEL );
			break;
         }
      }
   }
}

void MainWindow::CheckBTankCollisions()
{
   for( int c=0 ; c<m_MainTank.m_Shells.size() ; c++ )
   {
	  for( int i=0; i<m_BTanks.size() ; i++ )
      {
         if( CheckCollide( m_MainTank.m_Shells[c].Rect(), m_BTanks[i].Rect() ) )
         {
            GetFreeShells( m_BTanks[i] );
            AddExplosion( m_BTanks[i].m_PositionVector, 400, m_BTanks[i].m_TexQuad.width(), m_BTanks[i].m_TexQuad.height() );
            m_BTanks.erase( m_BTanks.begin()+i );
            m_MainTank.m_Shells.erase( m_MainTank.m_Shells.begin()+c );
            PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
			break;
         }
      }
   }
}

void MainWindow::CheckCTankCollisions()
{
   for( int c=0 ; c<m_MainTank.m_Shells.size() ; c++ )
   {
	  for( int i=0; i<m_CTanks.size() ; i++ )
      {
         if( CheckCollide( m_MainTank.m_Shells[c].Rect(), m_CTanks[i].Rect() ) )
         {
            GetFreeShells( m_CTanks[i] );
            AddExplosion( m_CTanks[i].m_PositionVector, 400, m_CTanks[i].m_TexQuad.width(), m_CTanks[i].m_TexQuad.height() );
            m_CTanks.erase( m_CTanks.begin()+i );
            m_MainTank.m_Shells.erase( m_MainTank.m_Shells.begin()+c );
            PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
			break;
         }
	  }
   }
}

void MainWindow::CheckDTankCollisions()
{
   for( int c=0 ; c<m_MainTank.m_Shells.size() ; c++ )
   {
      for( int i=0; i<m_DTanks.size() ; i++ )
      {
         if( CheckCollide( m_MainTank.m_Shells[c].Rect(), m_DTanks[i].Rect() ) )
         {
            GetFreeShells( m_DTanks[i] );
            AddExplosion( m_DTanks[i].m_PositionVector, 400, m_DTanks[i].m_TexQuad.width(), m_DTanks[i].m_TexQuad.height() );
            m_DTanks.erase( m_DTanks.begin()+i );
            m_MainTank.m_Shells.erase( m_MainTank.m_Shells.begin()+c );
            PlayClip( 1, MAIN_TANK_EXPLOSION_CHANNEL );
			break;
         }
      }
   }
}

