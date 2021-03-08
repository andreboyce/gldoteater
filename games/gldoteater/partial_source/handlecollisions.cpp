#include "main.h"

//***************************************************************************
//******************************* MainWindow::KillPlayer ********************
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::KillPlayer( C_DotEater& Player, int index )
{
   m_Mutex.Lock();
   if( !Player.m_bIsInvincible )
   {
      if( !m_bNetworkActive && (m_Players.size() == 1) )
         Player.m_Lives -= 1;

      if( Player.m_Lives <= 0 )
      {
         switch( index )
         {
            case 0:
            {
               m_Mutex.Lock();
               if( m_bNetworkActive )
                  NetorkCommandsQueue.push_back( COMMAND_PLAYER1_DIED );
               m_Mutex.Release();
            } break;
            case 1:
            {
               m_Mutex.Lock();
               if( m_bNetworkActive )
                  NetorkCommandsQueue.push_back( COMMAND_PLAYER2_DIED );
               m_Mutex.Release();
            } break;
         }
         return;
      }

      m_Mutex.Lock();
      if( index > 0 && m_bNetworkActive )
         NetorkCommandsQueue.push_back( COMMAND_RESET_PLAYER_POSITION );
      m_Mutex.Release();

      PlayClip( SOUND_DIE, DIE_CHANNEL );

      Player.m_RequestedDirection = Player.m_direction;
      Player.play_animation( C_DotEater::DIE, false );
      Player.m_TimeTillRespawn = Player.m_DieAnimation.total_duration()+200;
      m_Mutex.Release();
   }
}

//***************************************************************************
//******************************* MainWindow::KillPlayerAndStartNewRound ****
// Accepts: void
//
// Purpose: 
//
// Returns: void


void MainWindow::KillPlayerAndStartNewRound()
{
   if( m_Players.size() )
   {
      if( !m_Players[0].m_bIsInvincible )
      {
         if( !m_bNetworkActive )
            m_Players[0].m_Lives -= 1;
         InitialiseNewGame();
         PlayClip( SOUND_DIE, DIE_CHANNEL );
         if( m_Players[0].m_Lives == 0 )
         {
            m_Players[0].m_Lives = INITIAL_LIVES;
            m_GameMode = GAME_OVER;
            if( m_MapFilenames.size() >= 4  )
            {
               LoadMap( m_MapFilenames[GAMEOVER_MAP] );
            }
         }
      }
   }
}

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

void MainWindow::HandleCollisions(  )
{
   m_Mutex.Lock();

   if( m_bNetworkActive )
   {
      for( int i=0; i<m_Players.size() ;i++ )
      {
         if( m_bContinueWithNextPlayersCollisionDetection )
         {
            if( m_Players[i].m_TimeTillRespawn <= 0 )
            {
               HandleOnlinePlayerCollisions( m_Players[i], i );
            }
         }
      }
   }
   else
   {
      for( int i=0; i<m_Players.size() ;i++ )
      {
         if( m_Players[i].m_TimeTillRespawn <= 0 )
         {
            HandlePlayerCollisions( m_Players[i], i );
         }
      }
   }
   m_Mutex.Release();
}

//***************************************************************************
//******************************* MainWindow::HandlePlayerCollisions ********
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::HandlePlayerCollisions( C_DotEater& Player, int index )
{
   if( Player.m_Lives <= 0 )
      return;

   Structs::Point tile = GetTile( Player.m_PositionVector );

   if( IsRandomItemTile( tile ) )
   {
      int index = -1;
      if( (index = GetRandomItemIndex( tile )) > -1 )
      {
         EatRandomItem( Player, index );
         Player.play_animation( C_DotEater::EAT_DOT, false );
      }
   }

   for( int i=0; i<m_Dots.size(); i++  )
   {
      if( m_Dots[i].m_tile == tile )
      {
         Player.play_animation( C_DotEater::EAT_DOT, false );
         int score = 0;
         if( m_Dots[i].m_bIsPowerPellet )
         {
            PlayClip( 3, 3 );
            for( int i=0; i<m_Players.size() ;i++ )
            {
               m_Players[i].m_bIsPoweredUp     = true;
               m_Players[i].m_PoweredUpTime    = POWER_UP_AND_DOWN_TIME;
            }
            m_WhiteGhost.m_bIsPoweredDown   = true;
            m_WhiteGhost.m_PoweredDownTime  = POWER_UP_AND_DOWN_TIME;
            m_BlueGhost.m_bIsPoweredDown    = true;
            m_BlueGhost.m_PoweredDownTime   = POWER_UP_AND_DOWN_TIME;
            m_RedGhost.m_bIsPoweredDown     = true;
            m_RedGhost.m_PoweredDownTime    = POWER_UP_AND_DOWN_TIME;
            m_YellowGhost.m_bIsPoweredDown  = true;
            m_YellowGhost.m_PoweredDownTime = POWER_UP_AND_DOWN_TIME;
            score = EAT_BIGDOT_SCORE;
         }
         else
         {
            score = EAT_DOT_SCORE;
         }
         Player.m_Score += score;
         m_DotsEaten.push_back( m_Dots[i].m_tile );
         m_Dots.erase( m_Dots.begin()+i );
         BuildDotDisplayLists();
         PlayClip( 2, 2 );
      }      
   }
   if( m_Dots.size() == 0 )
   {
      m_GameMode = VIEW_STATS;
      LoadMap( m_MapFilenames[STATS_MAP] );
   }

   if( ( GetTile( m_WhiteGhost.m_PositionVector )   == tile && !m_WhiteGhost.m_bIsEaten   )  ||
            ( GetTile( m_BlueGhost.m_PositionVector )   == tile && !m_BlueGhost.m_bIsEaten   )  ||
            ( GetTile( m_RedGhost.m_PositionVector )    == tile && !m_RedGhost.m_bIsEaten    )  ||
            ( GetTile( m_YellowGhost.m_PositionVector ) == tile && !m_YellowGhost.m_bIsEaten ) )
   {
      if( !Player.m_bIsPoweredUp  )
      {
         if( !Player.m_bIsInvincible )
         {
            KillPlayer( Player, index );
            if( m_Players.size() == 1 )
            {
               if( Player.m_Lives == 0 )
               {
                  Player.m_Lives = INITIAL_LIVES;
                  m_GameMode = GAME_OVER;
                  GetHighScorePlayer1Name();
                  if( m_Players.size() > 1 )
                     GetHighScorePlayer2Name();
                  if( m_MapFilenames.size() >= 4  )
                  {
                     LoadMap( m_MapFilenames[GAMEOVER_MAP] );
                  }
               }
            }
         }
      }
      else
      {
         if( GetTile( m_WhiteGhost.m_PositionVector ) == tile )
         {
            if( m_WhiteGhost.m_bIsPoweredDown )
            {
               m_WhiteGhost.m_bIsEaten = true;
               PlayClip( 4, 4 );
               Player.m_Score += EAT_GHOST_SCORE;
               Player.play_animation( C_DotEater::EAT_DOT, false );
            }
            else
            {
               if( m_Players.size() == 1 )
               {
//                  KillPlayerAndStartNewRound();
                  KillPlayer( Player, index );
               }
               else
               {
                  KillPlayer( Player, index );
               }
            }
         }
         if( GetTile( m_BlueGhost.m_PositionVector ) == tile  )
         {
            if( m_BlueGhost.m_bIsPoweredDown )
            {
               m_BlueGhost.m_bIsEaten = true;
               PlayClip( 4, 4 );
               Player.m_Score += EAT_GHOST_SCORE;
               Player.play_animation( C_DotEater::EAT_DOT, false );
            }
            else
            {
               if( m_Players.size() == 1 )
               {
//                  KillPlayerAndStartNewRound();
                  KillPlayer( Player, index );
               }
               else
               {
                  KillPlayer( Player, index );
               }
            }
         }
         if( GetTile( m_RedGhost.m_PositionVector )  == tile  )
         {
            if( m_RedGhost.m_bIsPoweredDown )
            {
               m_RedGhost.m_bIsEaten = true;
               PlayClip( 4, 4 );
               Player.m_Score += EAT_GHOST_SCORE;
               Player.play_animation( C_DotEater::EAT_DOT, false );
            }
            else
            {
               if( m_Players.size() == 1 )
               {
//                  KillPlayerAndStartNewRound();
                  KillPlayer( Player, index );
               }
               else
               {
                  KillPlayer( Player, index );
               }
            }
         }
         if( GetTile( m_YellowGhost.m_PositionVector ) == tile )
         {
            if( m_YellowGhost.m_bIsPoweredDown )
            {
               m_YellowGhost.m_bIsEaten = true;
               PlayClip( 4, 4 );
               Player.m_Score += EAT_GHOST_SCORE;
               Player.play_animation( C_DotEater::EAT_DOT, false );
            }
            else
            {
               if( m_Players.size() == 1 )
               {
//                  KillPlayerAndStartNewRound();
                  KillPlayer( Player, index );
               }
               else
               {
                  KillPlayer( Player, index );
               }
            }
         }
      }
   }
}

//***************************************************************************
//******************************* MainWindow::HandleOnlinePlayerCollisions **
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::HandleOnlinePlayerCollisions( C_DotEater& Player, int index )
{
   if( Player.m_Lives <= 0 )
      return;

   Structs::Point tile = GetTile( Player.m_PositionVector );

//   if( IamServer() )
//   {
      if( IsRandomItemTile( tile ) )
      {
         int index = -1;
         if( (index = GetRandomItemIndex( tile )) > -1 )
         {
             EatRandomItem( Player, index );
             Player.play_animation( C_DotEater::EAT_DOT, false );
         }
      }
//   }

   for( int i=0; i<m_Dots.size(); i++  )
   {
      if( m_Dots[i].m_tile == tile )
      {
         Player.play_animation( C_DotEater::EAT_DOT, false );
         int score = 0;
         if( m_Dots[i].m_bIsPowerPellet )
         {
            PlayClip( 3, 3 );
            for( int i=0; i<m_Players.size() ;i++ )
            {
               m_Players[i].m_bIsPoweredUp     = true;
               m_Players[i].m_PoweredUpTime    = POWER_UP_AND_DOWN_TIME;
            }
            m_WhiteGhost.m_bIsPoweredDown    = true;
            m_WhiteGhost.m_PoweredDownTime   = POWER_UP_AND_DOWN_TIME;
            m_BlueGhost.m_bIsPoweredDown    = true;
            m_BlueGhost.m_PoweredDownTime   = POWER_UP_AND_DOWN_TIME;
            m_RedGhost.m_bIsPoweredDown     = true;
            m_RedGhost.m_PoweredDownTime    = POWER_UP_AND_DOWN_TIME;
            m_YellowGhost.m_bIsPoweredDown  = true;
            m_YellowGhost.m_PoweredDownTime = POWER_UP_AND_DOWN_TIME;
            score = EAT_BIGDOT_SCORE;
         }
         else
         {
            score = EAT_DOT_SCORE;
         }
         Player.m_Score += score;
         m_DotsEaten.push_back( m_Dots[i].m_tile );

         m_Dots.erase( m_Dots.begin()+i );
         PlayClip( SOUND_CHOMP, SOUND_CHOMP );
         BuildDotDisplayLists();
      }      
   }
   if( m_Dots.size() == 0 )
   {
      m_bContinueWithNextPlayersCollisionDetection = false;

      if( WinCondition() )
      {
         m_Level = 0;
         if( m_MapFilenames.size() > YOUWIN_MAP )
         {
            LoadMap( m_MapFilenames[YOUWIN_MAP] );
         }
         m_GameMode   = YOU_WIN_MODE;
         m_CurrentMap = YOUWIN_MAP;
         m_Mutex.Lock();
         m_Console.AddLine( "You guys win" );
         //m_Console.AddLine( "Type /start to play again" );
         if( m_Players.size() )
            m_Players[0].m_Lives = INITIAL_LIVES;
         m_Mutex.Release();

         if( IamServer() )
         {
            m_Mutex.Lock();
            NetorkCommandsQueue.push_back( COMMAND_YOU_WIN );
            m_Mutex.Release();
            AddHighScore( m_Nick, m_Players[0].m_Score );
         }
         else
         {
            AddHighScore( m_Nick, m_Players[1].m_Score );
         }
         return;
      }
      else
      {
         if( IamServer() )
         {
            m_GameMode   = VIEW_STATS;
            m_CurrentMap = STATS_MAP;
            if( m_MapFilenames.size() > STATS_MAP )
            {
               LoadMap( m_MapFilenames[STATS_MAP] );
            }
            m_Mutex.Lock();
            //m_Console.AddLine( "Type /s to begin " );
            NetorkCommandsQueue.push_back( COMMAND_VIEW_STATS );
//            NetorkCommandsQueue.push_back( COMMAND_LOAD_MAP );
            m_Mutex.Release();
         }
      }
//      m_Mutex.Lock();
//      m_deltaTickTime = 0;
//      m_Mutex.Release();
   }

   if( ( GetTile( m_WhiteGhost.m_PositionVector )  == tile && !m_WhiteGhost.m_bIsEaten  )  ||
       ( GetTile( m_BlueGhost.m_PositionVector )   == tile && !m_BlueGhost.m_bIsEaten   )  ||
       ( GetTile( m_RedGhost.m_PositionVector )    == tile && !m_RedGhost.m_bIsEaten    )  ||
       ( GetTile( m_YellowGhost.m_PositionVector ) == tile && !m_YellowGhost.m_bIsEaten )  && 
       ( m_GameMode == GAME_IN_PROGRESS) )
   {
      if( !Player.m_bIsPoweredUp  )
      {
         if( !Player.m_bIsInvincible )
         {
            KillPlayer( Player, index );
            if( AllPlayersAreDead() )
            {
//               m_GameMode = GAME_OVER;
               m_GameMode   = VIEW_STATS;
               m_CurrentMap = STATS_MAP;
               if( m_MapFilenames.size() > STATS_MAP )
               {
                  LoadMap( m_MapFilenames[STATS_MAP] );
               }
               m_Mutex.Lock();
               m_bContinueWithNextPlayersCollisionDetection = false;
               m_Console.AddLine( "GameOver :(" );
               m_bGotToGameOverAfterStats = true;
               if( IamServer() )
               {
                  //m_Console.AddLine( "Type /start to play again" );
                  if( m_Players.size() )
                     m_Players[0].m_Lives = INITIAL_LIVES;
               }
               else
               {
                  m_Console.AddLine( "Wating on host to start game" );
                  if( m_Players.size() > 1 )
                     m_Players[1].m_Lives = INITIAL_LIVES;
               }
               NetorkCommandsQueue.push_back( COMMAND_VIEW_STATS );
               m_Mutex.Release();
               if( IamServer() )
               {
                  AddHighScore( m_Nick, m_Players[0].m_Score );
               }
               else
               {
                  AddHighScore( m_Nick, m_Players[1].m_Score );
               }
            }
         }
      }
      else
      {
         if( GetTile( m_WhiteGhost.m_PositionVector ) == tile )
         {
            if( m_WhiteGhost.m_bIsPoweredDown )
            {
               m_WhiteGhost.m_bIsEaten = true;
               PlayClip( 4, 4 );
               Player.m_Score += EAT_GHOST_SCORE;
               Player.play_animation( C_DotEater::EAT_DOT, false );
            }
            else
            {
               if( m_Players.size() == 1 )
               {
//                  KillPlayerAndStartNewRound();
                  KillPlayer( Player, index );
               }
               else
               {
                  KillPlayer( Player, index );
               }
            }
         }
         if( GetTile( m_BlueGhost.m_PositionVector ) == tile  )
         {
            if( m_BlueGhost.m_bIsPoweredDown )
            {
               m_BlueGhost.m_bIsEaten = true;
               PlayClip( 4, 4 );
               Player.m_Score += EAT_GHOST_SCORE;
               Player.play_animation( C_DotEater::EAT_DOT, false );
            }
            else
            {
               if( m_Players.size() == 1 )
               {
//                  KillPlayerAndStartNewRound();
                  KillPlayer( Player, index );
               }
               else
               {
                  KillPlayer( Player, index );
               }
            }
         }
         if( GetTile( m_RedGhost.m_PositionVector )  == tile  )
         {
            if( m_RedGhost.m_bIsPoweredDown )
            {
               m_RedGhost.m_bIsEaten = true;
               PlayClip( 4, 4 );
               Player.m_Score += EAT_GHOST_SCORE;
               Player.play_animation( C_DotEater::EAT_DOT, false );
            }
            else
            {
               if( m_Players.size() == 1 )
               {
//                  KillPlayerAndStartNewRound();
                  KillPlayer( Player, index );
               }
               else
               {
                  KillPlayer( Player, index );
               }
            }
         }
         if( GetTile( m_YellowGhost.m_PositionVector ) == tile )
         {
            if( m_YellowGhost.m_bIsPoweredDown )
            {
               m_YellowGhost.m_bIsEaten = true;
               PlayClip( 4, 4 );
               Player.m_Score += EAT_GHOST_SCORE;
               Player.play_animation( C_DotEater::EAT_DOT, false );
            }
            else
            {
               if( m_Players.size() == 1 )
               {
//                  KillPlayerAndStartNewRound();
                  KillPlayer( Player, index );
               }
               else
               {
                  KillPlayer( Player, index );
               }
            }
         }
      }
   }
}

