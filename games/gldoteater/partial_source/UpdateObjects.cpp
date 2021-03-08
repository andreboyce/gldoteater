#include "main.h"

void MainWindow::UpdateTimeTillRespawn( C_DotEater& Player )
{
   m_Mutex.Lock();
   if( Player.m_TimeTillRespawn > 0 )
   {
      Player.m_TimeTillRespawn -= m_deltaGameTime;
      if( Player.m_TimeTillRespawn <= 0 )
      {
         Player.m_TimeTillRespawn = 0;
         Player.play_animation( C_DotEater::EAT_DOT, false );
         Player.m_bIsPoweredUp   = false;
         Player.m_bIsInvincible  = true;
         Player.m_InvincibleTime = INVINCIBLE_TIME;
         switch( Player.m_PlayerID )
         {
            case 0:
            {
               ResetPlayer1Position();
            } break;
            case 1:
            {
               ResetPlayer2Position();
            } break;
         }
      }
   }
   m_Mutex.Release();
}

void MainWindow::CheckForLifeUpFromPoints( C_DotEater& Player )
{
   if( (Player.m_Score - Player.m_LastLifeUpFromPoints ) > SCORE_FOR_LIFEUP )
   {
      Player.m_Lives += (int)(Player.m_Score - Player.m_LastLifeUpFromPoints )/SCORE_FOR_LIFEUP;
      Player.m_LastLifeUpFromPoints += (int)SCORE_FOR_LIFEUP*(Player.m_Score - Player.m_LastLifeUpFromPoints )/SCORE_FOR_LIFEUP;
      PlayClip( 5, 5 );
   }
}


void MainWindow::UpdatePlayerInvincibleTime( C_DotEater& Player )
{
   if( Player.m_bIsInvincible )
   {
      Player.m_InvincibleTime -= m_deltaGameTime;
      if( Player.m_InvincibleTime <= 0 )
      {
         Player.m_InvincibleTime = 0;
         Player.m_bIsInvincible  = false;
      }
   }
}

void MainWindow::UpdateGhostPowerDownTime( C_GHost& ghost )
{
   if( !ghost.m_bIsEaten )
   {
      ghost.m_PoweredDownTime -= m_deltaGameTime;
      if( ghost.m_PoweredDownTime < 0 )
      {
         ghost.m_PoweredDownTime = 0;
         ghost.m_bIsPoweredDown = false;
      }
   }
}

void MainWindow::UpdatePlayerPowerUpTime( C_DotEater& player )
{
   player.m_PoweredUpTime -= m_deltaGameTime;
   if( player.m_PoweredUpTime <= 0 )
   {
      player.m_bIsPoweredUp = false;
   }
}


void MainWindow::MoveWhiteGhost()
{
   if( m_WhiteGhost.m_bIsEaten )
   {
      if( m_TileMap.m_SpecialData.size() >= 5 )
      {
//         Alert( GetWhiteGhostSpawn() );
         MoveGhost( m_WhiteGhost, GetWhiteGhostSpawn() );
//         MoveGhost( m_WhiteGhost, m_TileMap.m_SpecialData[1].pos() );
         if( GetTile( m_WhiteGhost.m_PositionVector ) == GetWhiteGhostSpawn() )
         {
            m_WhiteGhost.m_bIsPoweredDown = false;
            m_WhiteGhost.m_bIsEaten       = false;
         }
      }
      else
         MoveGhost( m_WhiteGhost,   GetTile( m_Players[0].m_PositionVector ) );         
   }
   else if( m_WhiteGhost.m_bIsPoweredDown )
   {
      UpdateGhostPowerDownTime( m_WhiteGhost );
      MoveGhost( m_WhiteGhost, GetRandomTile() );
   }
   else
   {
      if( m_Players.size() )
      {
         if( !m_Players[0].m_bIsInvincible && m_Players[0].m_Lives > 0 )
            MoveGhost( m_WhiteGhost, GetTile( m_Players[0].m_PositionVector ) );
         else
            MoveGhost( m_WhiteGhost, GetRandomTile() );
      }
      else
      {
         MoveGhost( m_WhiteGhost, GetRandomTile() );
      }
   }
}

void MainWindow::MoveBlueGhost()
{
   if( m_BlueGhost.m_bIsEaten )
   {
      if( m_TileMap.m_SpecialData.size() >= 5 )
      {
         MoveGhost( m_BlueGhost, GetBlueGhostSpawn() );
//         MoveGhost( m_BlueGhost, m_TileMap.m_SpecialData[2].pos() );
         if( GetTile( m_BlueGhost.m_PositionVector ) == GetBlueGhostSpawn() )
         {
            m_BlueGhost.m_bIsPoweredDown = false;
            m_BlueGhost.m_bIsEaten       = false;
         }
      }
      else
         MoveGhost( m_BlueGhost,  GetTile( m_Players[0].m_PositionVector ) );         
   }
   else if( m_BlueGhost.m_bIsPoweredDown )
   {
      UpdateGhostPowerDownTime( m_BlueGhost );
      MoveGhost( m_BlueGhost, GetRandomTile() );
   }
   else
   {
      if( m_Players.size() > 1 )
      {
         if( !m_Players[1].m_bIsInvincible && m_Players[1].m_Lives > 0  )
            MoveGhost( m_BlueGhost, GetTile( m_Players[1].m_PositionVector ) );
         else
            MoveGhost( m_BlueGhost, GetRandomTile() );
      }
      else
      {
         MoveGhost( m_BlueGhost, GetRandomTile() );
      }
   }
}

void MainWindow::MoveRedGhost()
{
   if( m_RedGhost.m_bIsEaten )
   {
      if( m_TileMap.m_SpecialData.size() >= 5 )
      {
         MoveGhost( m_RedGhost, GetRedGhostSpawn() );
//         MoveGhost( m_RedGhost, m_TileMap.m_SpecialData[3].pos() );
         if( GetTile( m_RedGhost.m_PositionVector ) == GetRedGhostSpawn() )
         {
            m_RedGhost.m_bIsPoweredDown = false;
            m_RedGhost.m_bIsEaten       = false;
         }
      }
      else
         MoveGhost( m_RedGhost,  GetTile( m_Players[0].m_PositionVector ) );         
   }
   else if( m_RedGhost.m_bIsPoweredDown )
   {
      UpdateGhostPowerDownTime( m_RedGhost );
      MoveGhost( m_RedGhost, GetRandomTile() );
   }
   else
   {
      if( m_Players.size() > 2 )
      {
         if( !m_Players[2].m_bIsInvincible && m_Players[2].m_Lives > 0  )
            MoveGhost( m_RedGhost, GetTile( m_Players[2].m_PositionVector ) );
         else
            MoveGhost( m_RedGhost, GetRandomTile() );
      }
      else
      {
         MoveGhost( m_RedGhost, GetRandomTile() );
      }
   }
}

void MainWindow::MoveYellowGhost()
{
   if( m_YellowGhost.m_bIsEaten )
   {
      if( m_TileMap.m_SpecialData.size() >= 5 )
      {
         MoveGhost( m_YellowGhost, GetYellowGhostSpawn() );
//         MoveGhost( m_YellowGhost, m_TileMap.m_SpecialData[3].pos() );
         if( GetTile( m_YellowGhost.m_PositionVector ) == GetYellowGhostSpawn() )
         {
            m_YellowGhost.m_bIsPoweredDown = false;
            m_YellowGhost.m_bIsEaten       = false;
         }
      }
      else
         MoveGhost( m_YellowGhost,  GetTile( m_Players[0].m_PositionVector ) );         
   }
   else if( m_YellowGhost.m_bIsPoweredDown )
   {
      UpdateGhostPowerDownTime( m_YellowGhost );
      MoveGhost( m_YellowGhost, GetRandomTile() );
   }
   else
   {
      if( m_Players.size() > 3 )
      {
         if( !m_Players[3].m_bIsInvincible && m_Players[3].m_Lives > 0  )
            MoveGhost( m_YellowGhost, GetTile( m_Players[3].m_PositionVector ) );
         else
            MoveGhost( m_YellowGhost, GetRandomTile() );
      }
      else
      {
         MoveGhost( m_YellowGhost, GetRandomTile() );
      }
   }
}


//****************************************************************************
//******************************* MainWindow::MoveGhost **************************
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::MoveGhost( C_GHost& ghost, Structs::Point goal )
{
   MoveOffSolidTile( ghost );
   Structs::Point tile = GetTile( ghost.m_PositionVector );

   if( ghost.m_AStarPath.size() )
   {
      if( tile == ghost.m_AStarPath.back().pos )
      {
         ghost.m_AStarPath.pop_back();
         if( ghost.m_AStarPath.size() )
         {
            ghost.m_direction = GetDirectionToNextTile( tile, ghost.m_AStarPath.back().pos );
         }
         else
         {
            ghost.m_direction = GetDirectionToNextTile( tile, goal );
         }         
      }
   }
   else
   {
      GetPathAndDirection( ghost, goal );
   }

   if( ghost.m_direction == UP )
   {
      AlignToVerticalAxis( ghost );
      if( !UpTileIsSolid( ghost.m_PositionVector ) )
         ghost.m_PositionVector.m_y += ghost.m_VelocityVector.m_y*m_deltaGameTime;
      else
      {
         ghost.m_PositionVector.m_y += ghost.m_VelocityVector.m_y*m_deltaGameTime;
         Vector2D pos = AlignToHorizontialAxis( ghost.m_PositionVector );
         if( ghost.m_PositionVector.m_y > pos.m_y )
         {
            AlignToHorizontialAxis( ghost );
         }
      }
      tile = GetTile( ghost.m_PositionVector );
      if( (ghost.m_PositionVector.m_y > m_BoundingRect.top-m_MapTexturedQuad.halfheight())          || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
       )
      {
         ghost.m_PositionVector.m_y -= ghost.m_VelocityVector.m_y*m_deltaGameTime;
      }
   }
   if( ghost.m_direction == DOWN )
   {
      AlignToVerticalAxis( ghost );
      if( !DownTileIsSolid( ghost.m_PositionVector ) )
         ghost.m_PositionVector.m_y -= ghost.m_VelocityVector.m_y*m_deltaGameTime;
      else
      {
         ghost.m_PositionVector.m_y -= ghost.m_VelocityVector.m_y*m_deltaGameTime;
         Vector2D pos = AlignToHorizontialAxis( ghost.m_PositionVector );
         if( ghost.m_PositionVector.m_y < pos.m_y )
         {
            AlignToHorizontialAxis( ghost );
         }
      }
      tile = GetTile( ghost.m_PositionVector );
      if( (ghost.m_PositionVector.m_y < m_BoundingRect.bottom+m_MapTexturedQuad.halfheight())       || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
        )
      {
         ghost.m_PositionVector.m_y += ghost.m_VelocityVector.m_y*m_deltaGameTime;
      }
   }
   if( ghost.m_direction == LEFT )
   {
      AlignToHorizontialAxis( ghost );
      if( !LeftTileIsSolid( ghost.m_PositionVector ) )
         ghost.m_PositionVector.m_x -= ghost.m_VelocityVector.m_x*m_deltaGameTime;
      else
      {
         ghost.m_PositionVector.m_x -= ghost.m_VelocityVector.m_x*m_deltaGameTime;
         Vector2D pos = AlignToVerticalAxis( ghost.m_PositionVector );
         if( ghost.m_PositionVector.m_x < pos.m_x )
         {
            AlignToVerticalAxis( ghost );
         }
      }
      tile = GetTile( ghost.m_PositionVector );
      if( (ghost.m_PositionVector.m_x < m_BoundingRect.left+m_MapTexturedQuad.halfwidth())         || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
        )
      {
         ghost.m_PositionVector.m_x += ghost.m_VelocityVector.m_x*m_deltaGameTime;
      }
   }
   if( ghost.m_direction == RIGHT )
   {
      AlignToHorizontialAxis( ghost );
      if( !RightTileIsSolid( ghost.m_PositionVector ) )
         ghost.m_PositionVector.m_x += ghost.m_VelocityVector.m_x*m_deltaGameTime;
      else
      {
         ghost.m_PositionVector.m_x += ghost.m_VelocityVector.m_x*m_deltaGameTime;
         Vector2D pos = AlignToVerticalAxis( ghost.m_PositionVector );
         if( ghost.m_PositionVector.m_x > pos.m_x )
         {
            AlignToVerticalAxis( ghost );
         }
      }
      tile = GetTile( ghost.m_PositionVector );
      if( (ghost.m_PositionVector.m_x > m_BoundingRect.right-m_MapTexturedQuad.halfwidth())        || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
         )
      {
         ghost.m_PositionVector.m_x -= ghost.m_VelocityVector.m_x*m_deltaGameTime;
      }
   }
   MoveOffSolidTile( ghost );
}

//****************************************************************************
//******************************* MainWindow::MovePlayer *********************
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::MovePlayer( C_DotEater& Player, Structs::Point goal )
{
   MoveOffSolidTile( Player );
   Structs::Point tile = GetTile( Player.m_PositionVector );
   if( !IsValidTile( tile ) || IsSolid( tile.x, tile.y ) )
   {
      ResetPlayer1Position();
      ResetPlayer2Position();
   }

   if( Player.m_AStarPath.size() )
   {
      if( tile == Player.m_AStarPath.back().pos )
      {
         Player.m_AStarPath.pop_back();
         if( Player.m_AStarPath.size() )
         {
            Player.m_direction = GetDirectionToNextTile( tile, Player.m_AStarPath.back().pos );
         }
         else
         {
            Player.m_direction = GetDirectionToNextTile( tile, goal );
         }         
      }
   }
   else
   {
      GetPathAndDirection( Player, goal );
   }

   switch( Player.m_RequestedDirection )
   {
      case UP:
      {
         if( !UpTileIsSolid( Player.m_PositionVector ) )
         {
            Player.m_direction = UP;
         }
      } break;
      case DOWN:
      {
         if( !DownTileIsSolid( Player.m_PositionVector ) )
         {
            Player.m_direction = DOWN;
         }
      } break;
      case LEFT:
      {
         if( !LeftTileIsSolid( Player.m_PositionVector ) )
         {
            Player.m_direction = LEFT;
         }
      } break;
      case RIGHT:
      {
         if( !RightTileIsSolid( Player.m_PositionVector ) )
         {
            Player.m_direction = RIGHT;
         }
      } break;
   }

   if( Player.m_direction == UP  )
   {
      AlignToVerticalAxis( Player );
      if( !UpTileIsSolid( Player.m_PositionVector ) )
      {
         Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_deltaGameTime;
      }
      else
      {
         Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_deltaGameTime;
         Vector2D pos = AlignToHorizontialAxis( Player.m_PositionVector );
         if( Player.m_PositionVector.m_y > pos.m_y )
         {
            AlignToHorizontialAxis( Player );
         }
      }
      tile = GetTile( Player.m_PositionVector );
      if( (Player.m_PositionVector.m_y > m_BoundingRect.top-m_MapTexturedQuad.halfheight())          || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
       )
      {
         Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_deltaGameTime;
      }
   }
   if( Player.m_direction == DOWN  )
   {
      AlignToVerticalAxis( Player );
      if( !DownTileIsSolid( Player.m_PositionVector ) )
      {
         Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_deltaGameTime;
	  }
      else
      {
         Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_deltaGameTime;
         Vector2D pos = AlignToHorizontialAxis( Player.m_PositionVector );
         if( Player.m_PositionVector.m_y < pos.m_y )
         {
            AlignToHorizontialAxis( Player );
         }
      }
      tile = GetTile( Player.m_PositionVector );
      if( (Player.m_PositionVector.m_y < m_BoundingRect.bottom+m_MapTexturedQuad.halfheight())       || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
        )
      {
         Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_deltaGameTime;
      }
   }
   if( Player.m_direction == LEFT  )
   {
      AlignToHorizontialAxis( Player );
      if( !LeftTileIsSolid( Player.m_PositionVector ) )
         Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_deltaGameTime;
      else
      {
         Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_deltaGameTime;
         Vector2D pos = AlignToVerticalAxis( Player.m_PositionVector );
         if( Player.m_PositionVector.m_x < pos.m_x )
         {
            AlignToVerticalAxis( Player );
         }
      }
      tile = GetTile( Player.m_PositionVector );
      if( (Player.m_PositionVector.m_x < m_BoundingRect.left+m_MapTexturedQuad.halfwidth())         || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
        )
      {
         Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_deltaGameTime;
      }
   }
   if( Player.m_direction == RIGHT  )
   {
      AlignToHorizontialAxis( Player );
      if( !RightTileIsSolid( Player.m_PositionVector ) )
         Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_deltaGameTime;
      else
      {
         Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_deltaGameTime;
         Vector2D pos = AlignToVerticalAxis( Player.m_PositionVector );
         if( Player.m_PositionVector.m_x > pos.m_x )
         {
            AlignToVerticalAxis( Player );
         }
      }
      tile = GetTile( Player.m_PositionVector );
      if( (Player.m_PositionVector.m_x > m_BoundingRect.right-m_MapTexturedQuad.halfwidth())        || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
         )
      {
         Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_deltaGameTime;
      }
   }
}

//****************************************************************************
//******************************* MainWindow::MovePlayer *********************
// Accepts: void
//
// Purpose: move the tank that the player controlls if a d pad key is pressed
//
// Returns: void

void MainWindow::MovePlayer( C_DotEater& Player )
{
   MoveOffSolidTile( Player );
   Structs::Point tile = GetTile( Player.m_PositionVector );
   if( !IsValidTile( tile ) || IsSolid( tile.x, tile.y ) )
   {
      //m_Console.AddLine( "ResetPlayer1Position" );
      ResetPlayer1Position();
      ResetPlayer2Position();
   }
   UpdatePlayerPowerUpTime( Player );

   switch( Player.m_RequestedDirection )
   {
      case UP:
      {
         if( !UpTileIsSolid( Player.m_PositionVector ) )
         {
            Player.m_direction = UP;
         }
      } break;
      case DOWN:
      {
         if( !DownTileIsSolid( Player.m_PositionVector ) )
         {
            Player.m_direction = DOWN;
         }
      } break;
      case LEFT:
      {
         if( !LeftTileIsSolid( Player.m_PositionVector ) )
         {
            Player.m_direction = LEFT;
         }
      } break;
      case RIGHT:
      {
         if( !RightTileIsSolid( Player.m_PositionVector ) )
         {
            Player.m_direction = RIGHT;
         }
      } break;
   }

   if( Player.m_direction == UP )
   {
      AlignToVerticalAxis( Player );
      if( !UpTileIsSolid( Player.m_PositionVector ) )
      {
         Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_deltaGameTime;

		 //std::ostringstream message1;
		 //message1 << "Not UpTileIsSolid: Player.m_PositionVector.m_y " << Player.m_PositionVector.m_y;
		 //m_Console.AddLine(message1.str() );

//         if( m_bNetworkActive && !IamServer() )
//         {
//             Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
//         }
      }
      else
      {
         //Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_deltaGameTime;
//         if( m_bNetworkActive && !IamServer() )
//         {
//             Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
//         }
         Vector2D pos = AlignToHorizontialAxis( Player.m_PositionVector );
         //if( Player.m_PositionVector.m_y > pos.m_y )
		 if( Player.m_PositionVector.m_y > pos.m_y )
         {
            // fix this
			// Player.m_PositionVector.m_y
			// m_bMoveGhosts = false;
            // AlignToHorizontialAxis( Player );
			//AlignToVerticalAxis(Player);
         }
      }
      tile = GetTile( Player.m_PositionVector );
      if( !IsValidTile( tile ) )
         return;
      if( (Player.m_PositionVector.m_y > m_BoundingRect.top-m_MapTexturedQuad.halfheight())          || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
       )
      {
         Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_deltaGameTime;

//         if( m_bNetworkActive && !IamServer() )
//         {
//            Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
//         }
      }
   }
   if( Player.m_direction == DOWN )
   {
      AlignToVerticalAxis( Player );
      if( !DownTileIsSolid( Player.m_PositionVector ) )
      {
         Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_deltaGameTime;
//         if( m_bNetworkActive && !IamServer() )
//         {
//            Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
//         }
      }
      else
      {
         Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_deltaGameTime;
//         if( m_bNetworkActive && !IamServer() )
//         {
//            Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
//         }
         Vector2D pos = AlignToHorizontialAxis( Player.m_PositionVector );
         if( Player.m_PositionVector.m_y < pos.m_y )
         {
            AlignToHorizontialAxis( Player );
         }
      }
      tile = GetTile( Player.m_PositionVector );
      if( !IsValidTile( tile ) )
         return;
      if( (Player.m_PositionVector.m_y < m_BoundingRect.bottom+m_MapTexturedQuad.halfheight())       || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
        )
      {
         Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_deltaGameTime;
//         if( m_bNetworkActive && !IamServer() )
//         {
//            Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
//         }
      }
   }
   if( Player.m_direction == LEFT )
   {
      //AlignToHorizontialAxis( Player );
      if( !LeftTileIsSolid( Player.m_PositionVector ) )
      {
         Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_deltaGameTime;
//         if( m_bNetworkActive && !IamServer() )
//         {
//            Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_DeltaClientServerRoundStart;
//         }
      }
      else
      {
         Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_deltaGameTime;
//         if( m_bNetworkActive && !IamServer() )
//         {
//            Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_DeltaClientServerRoundStart;
//         }
         Vector2D pos = AlignToVerticalAxis( Player.m_PositionVector );
         if( Player.m_PositionVector.m_x < pos.m_x )
         {
            AlignToVerticalAxis( Player );
         }
      }
      tile = GetTile( Player.m_PositionVector );
      if( !IsValidTile( tile ) )
         return;
      if( (Player.m_PositionVector.m_x < m_BoundingRect.left+m_MapTexturedQuad.halfwidth())         || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
        )
      {
         Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_deltaGameTime;
//         if( m_bNetworkActive && !IamServer() )
//         {
//            Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_DeltaClientServerRoundStart;
//         }
      }
   }
   if( Player.m_direction == RIGHT )
   {
      //AlignToHorizontialAxis( Player );
      if( !RightTileIsSolid( Player.m_PositionVector ) )
      {
         Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_deltaGameTime;
//         if( m_bNetworkActive && !IamServer() )
//         {
//            Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_DeltaClientServerRoundStart;
//         }
      }
      else
      {
         Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_deltaGameTime;
//         if( m_bNetworkActive && !IamServer() )
//         {
//            Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_DeltaClientServerRoundStart;
//         }
         Vector2D pos = AlignToVerticalAxis( Player.m_PositionVector );
         if( Player.m_PositionVector.m_x > pos.m_x )
         {
            AlignToVerticalAxis( Player );
         }
      }
      tile = GetTile( Player.m_PositionVector );
      if( !IsValidTile( tile ) )
         return;
      if( (Player.m_PositionVector.m_x > m_BoundingRect.right-m_MapTexturedQuad.halfwidth())        || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
         )
      {
         Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_deltaGameTime;
//         if( m_bNetworkActive && !IamServer() )
//         {
//            Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_DeltaClientServerRoundStart;
//         }
      }
   }
}

void MainWindow::MovePlayerOnNetworkUpdate( C_DotEater& Player )
{
   MoveOffSolidTile( Player );
   Structs::Point tile = GetTile( Player.m_PositionVector );
   if( !IsValidTile( tile ) || IsSolid( tile.x, tile.y ) )
   {
      ResetPlayer1Position();
      ResetPlayer2Position();
   }
   UpdatePlayerPowerUpTime( Player );

   switch( Player.m_RequestedDirection )
   {
      case UP:
      {
         if( !UpTileIsSolid( Player.m_PositionVector ) )
         {
            Player.m_direction = UP;
         }
      } break;
      case DOWN:
      {
         if( !DownTileIsSolid( Player.m_PositionVector ) )
         {
            Player.m_direction = DOWN;
         }
      } break;
      case LEFT:
      {
         if( !LeftTileIsSolid( Player.m_PositionVector ) )
         {
            Player.m_direction = LEFT;
         }
      } break;
      case RIGHT:
      {
         if( !RightTileIsSolid( Player.m_PositionVector ) )
         {
            Player.m_direction = RIGHT;
         }
      } break;
   }

   if( Player.m_direction == UP )
   {
      AlignToVerticalAxis( Player );
      if( !UpTileIsSolid( Player.m_PositionVector ) )
      {
//         Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_deltaGameTime;
         if( m_bNetworkActive && !IamServer() )
         {
             Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
         }
      }
      else
      {
//         Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_deltaGameTime;
         if( m_bNetworkActive && !IamServer() )
         {
             Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
         }
         Vector2D pos = AlignToHorizontialAxis( Player.m_PositionVector );
         if( Player.m_PositionVector.m_y > pos.m_y )
         {
            AlignToHorizontialAxis( Player );
         }
      }
      tile = GetTile( Player.m_PositionVector );
      if( !IsValidTile( tile ) )
         return;
      if( (Player.m_PositionVector.m_y > m_BoundingRect.top-m_MapTexturedQuad.halfheight())          || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
       )
      {
//         Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_deltaGameTime;
         if( m_bNetworkActive && !IamServer() )
         {
            Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
            if( m_bNetworkActive && !IamServer() )
            {
                Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
            }
         }
      }
   }
   if( Player.m_direction == DOWN )
   {
      AlignToVerticalAxis( Player );
      if( !DownTileIsSolid( Player.m_PositionVector ) )
      {
//         Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_deltaGameTime;
         if( m_bNetworkActive && !IamServer() )
         {
            Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
         }
      }
      else
      {
//         Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_deltaGameTime;
         if( m_bNetworkActive && !IamServer() )
         {
            Player.m_PositionVector.m_y -= Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
         }
         Vector2D pos = AlignToHorizontialAxis( Player.m_PositionVector );
         if( Player.m_PositionVector.m_y < pos.m_y )
         {
            AlignToHorizontialAxis( Player );
         }
      }
      tile = GetTile( Player.m_PositionVector );
      if( !IsValidTile( tile ) )
         return;
      if( (Player.m_PositionVector.m_y < m_BoundingRect.bottom+m_MapTexturedQuad.halfheight())       || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
        )
      {
//         Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_deltaGameTime;
         if( m_bNetworkActive && !IamServer() )
         {
            Player.m_PositionVector.m_y += Player.m_VelocityVector.m_y*m_DeltaClientServerRoundStart;
         }
      }
   }
   if( Player.m_direction == LEFT )
   {
      AlignToHorizontialAxis( Player );
      if( !LeftTileIsSolid( Player.m_PositionVector ) )
      {
 //        Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_deltaGameTime;
         if( m_bNetworkActive && !IamServer() )
         {
            Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_DeltaClientServerRoundStart;
         }
      }
      else
      {
//         Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_deltaGameTime;
         if( m_bNetworkActive && !IamServer() )
         {
            Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_DeltaClientServerRoundStart;
         }
         Vector2D pos = AlignToVerticalAxis( Player.m_PositionVector );
         if( Player.m_PositionVector.m_x < pos.m_x )
         {
            AlignToVerticalAxis( Player );
         }
      }
      tile = GetTile( Player.m_PositionVector );
      if( !IsValidTile( tile ) )
         return;
      if( (Player.m_PositionVector.m_x < m_BoundingRect.left+m_MapTexturedQuad.halfwidth())         || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
        )
      {
//         Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_deltaGameTime;
         if( m_bNetworkActive && !IamServer() )
         {
            Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_DeltaClientServerRoundStart;
         }
      }
   }
   if( Player.m_direction == RIGHT )
   {
      AlignToHorizontialAxis( Player );
      if( !RightTileIsSolid( Player.m_PositionVector ) )
      {
//         Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_deltaGameTime;
         if( m_bNetworkActive && !IamServer() )
         {
            Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_DeltaClientServerRoundStart;
         }
      }
      else
      {
//         Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_deltaGameTime;
         if( m_bNetworkActive && !IamServer() )
         {
            Player.m_PositionVector.m_x += Player.m_VelocityVector.m_x*m_DeltaClientServerRoundStart;
         }
         Vector2D pos = AlignToVerticalAxis( Player.m_PositionVector );
         if( Player.m_PositionVector.m_x > pos.m_x )
         {
            AlignToVerticalAxis( Player );
         }
      }
      tile = GetTile( Player.m_PositionVector );
      if( !IsValidTile( tile ) )
         return;
      if( (Player.m_PositionVector.m_x > m_BoundingRect.right-m_MapTexturedQuad.halfwidth())        || 
          (m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.maxsolidity()) 
         )
      {
//         Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_deltaGameTime;
         if( m_bNetworkActive && !IamServer() )
         {
            Player.m_PositionVector.m_x -= Player.m_VelocityVector.m_x*m_DeltaClientServerRoundStart;
         }
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
      m_Mutex.Lock();
      for( int i=0; i<m_Players.size() ; i++ )
      {

         if( m_Players[i].m_AStarPath.size() )
         {
            if( !IsSolid( m_PlayerGoal.x, m_PlayerGoal.y ) )
            {
               MovePlayer( m_Players[i], m_PlayerGoal );
            }
            else
            {
               if( m_Players[i].m_TimeTillRespawn <= 0 )
               {
                  MovePlayer( m_Players[i] );
               }
            }
         }
         else
         {
            if( m_Players[i].m_TimeTillRespawn <= 0 )
            {
               MovePlayer( m_Players[i] );
            }
         }
         UpdatePlayerInvincibleTime( m_Players[i] );
         UpdateTimeTillRespawn( m_Players[i] );
         CheckForLifeUpFromPoints( m_Players[i] );
      }

      if( m_bMoveGhosts )
      {
         if( m_bNetworkActive )
         {
            if( IamServer() )
            {
               SetDirection( m_WhiteGhost  );
               SetDirection( m_BlueGhost   );
               SetDirection( m_RedGhost    );
               SetDirection( m_YellowGhost );
            }

            MoveWhiteGhost();
            MoveBlueGhost();
            MoveRedGhost();
            MoveYellowGhost();
         }
         else
         {
            MoveWhiteGhost();
            MoveBlueGhost();
            MoveRedGhost();
            MoveYellowGhost();
            SetDirection( m_WhiteGhost  );
            SetDirection( m_BlueGhost   );
            SetDirection( m_RedGhost    );
            SetDirection( m_YellowGhost );
         }
      }
      m_Mutex.Release();
   }
}

