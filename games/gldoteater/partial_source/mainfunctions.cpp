#include "main.h"

bool MainWindow::WinCondition( void )
{
   if( m_Players.size() > 1 )
   {
      return ( m_Level >= m_MultiPlayerMaps.size() ) ? true : false;
   }
   else if( m_Players.size() == 1 )
   {
      return ( m_Level >= m_SinglePlayerMaps.size() ) ? true : false;
   }
}

void MainWindow::IncrementLevel( void )
{
   m_Level++;
   if( m_bNetworkActive )
   {
      return;
   }

   if( WinCondition() )
   {
      m_Level = 0;
      if( m_MapFilenames.size() > YOUWIN_MAP )
      {
         LoadMap( m_MapFilenames[YOUWIN_MAP] );
         m_CurrentMap = YOUWIN_MAP;
      }
      m_GameMode = YOU_WIN_MODE;
      if( m_Players.size() > 1 )
      {
         m_bIgnoreVK_RETURN = true;
         if( m_Players[0].m_Score > m_Players[1].m_Score )
         {
            GetHighScorePlayer1Name();
         }
         else if( m_Players[0].m_Score < m_Players[1].m_Score )
         {
            GetHighScorePlayer2Name();
         }
      }
      else if( m_Players.size() )
      {
         GetHighScorePlayer1Name();
      }
   }
   else
   {
      InitialiseNewGame();
      m_GameMode = GAME_IN_PROGRESS;
   }
}

void MainWindow::SaveSpriteTiles()
{
   m_SpriteTiles.clear();
   m_SpriteTiles.resize( 4+m_Players.size() );
   if( !(m_SpriteTiles.size() >= 4+m_Players.size()) )
      return;
   m_SpriteTiles[0] = GetTile( m_WhiteGhost.m_PositionVector  );
   m_SpriteTiles[1] = GetTile( m_BlueGhost.m_PositionVector   );
   m_SpriteTiles[2] = GetTile( m_RedGhost.m_PositionVector    );
   m_SpriteTiles[3] = GetTile( m_YellowGhost.m_PositionVector );
   if( m_Players.size() )
      m_SpriteTiles[4] = GetTile( m_Players[0].m_PositionVector  );
   if( m_Players.size() > 1 )
      m_SpriteTiles[5] = GetTile( m_Players[1].m_PositionVector  );
}

void MainWindow::SetDirection( C_GHost& ghost )
{
   if( !ghost.m_AStarPath.size() )
   {
       return;
   }
   Structs::Point tile = GetTile( ghost.m_PositionVector );
   if( UpTileIsSolid(ghost.m_PositionVector) && IsValidTile( tile ) && ghost.m_direction == UP )
   {
      GetPathAndDirection( ghost, GetRandomTile() );
      return;
   }
   if( DownTileIsSolid(ghost.m_PositionVector) && IsValidTile( tile ) && ghost.m_direction == DOWN )
   {
      GetPathAndDirection( ghost, GetRandomTile() );
      return;
   }
   if( LeftTileIsSolid(ghost.m_PositionVector) && IsValidTile( tile ) && ghost.m_direction == LEFT )
   {
      GetPathAndDirection( ghost, GetRandomTile() );
         return;
   }
   if( RightTileIsSolid(ghost.m_PositionVector) && IsValidTile( tile ) && ghost.m_direction == RIGHT )
   {
      GetPathAndDirection( ghost, GetRandomTile() );
      return;
   }
}

void MainWindow::RestoreSpriteTiles()
{
   if( !(m_SpriteTiles.size() >= 4+m_Players.size()) )
      return;

   m_WhiteGhost.m_PositionVector  = GetTilePos( m_SpriteTiles[0] );
   GetPathAndDirection( m_WhiteGhost, m_SpriteTiles[0] );

   m_BlueGhost.m_PositionVector   = GetTilePos( m_SpriteTiles[1] );
   GetPathAndDirection( m_BlueGhost, GetRandomTile() );

   m_RedGhost.m_PositionVector    = GetTilePos( m_SpriteTiles[2] );
   GetPathAndDirection( m_RedGhost, GetRandomTile() );

   m_YellowGhost.m_PositionVector = GetTilePos( m_SpriteTiles[3] );
   GetPathAndDirection( m_YellowGhost, GetRandomTile() );

   if( m_Players.size() )
      m_Players[0].m_PositionVector  = GetTilePos( m_SpriteTiles[4] );

   if( m_Players.size() > 1 )
      m_Players[0].m_PositionVector  = GetTilePos( m_SpriteTiles[5] );
}

void MainWindow::GetplayerGoal( int index )
{
   if( m_Players.size() > index )
   {
      GetPathAndDirection( m_Players[index], m_PlayerGoal );
   }
}

void MainWindow::MoveOffSolidTile( C_GHost& ghost )
{
   Structs::Point tile = GetTile( ghost.m_PositionVector );
   if( IsValidTile( tile ) == false )
      return;
   if( m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.m_Header.max_solidity )
   {
      if( ghost.m_AStarPath.size() )
      {
         ghost.m_PositionVector = GetTilePos( ghost.m_AStarPath.back().pos );
         ghost.m_direction      = (Direction)ghost.m_AStarPath.back().oreintation;
      }
      else
      {
         if( !LeftTileIsSolid(ghost.m_PositionVector) && m_TileMap.LeftTile( GetTile(ghost.m_PositionVector) ) )
         {
//            ghost.m_PositionVector = ;
         }
         else if( !UpTileIsSolid(ghost.m_PositionVector) && m_TileMap.UpTile( GetTile(ghost.m_PositionVector) ) )
         {
         }
         else if( !RightTileIsSolid(ghost.m_PositionVector) && m_TileMap.RightTile( GetTile(ghost.m_PositionVector) ) )
         {
         }
         else if( !DownTileIsSolid(ghost.m_PositionVector) && m_TileMap.DownTile( GetTile(ghost.m_PositionVector) ) )
         {
         }
      }
   }
}

void MainWindow::MoveOffSolidTile( C_DotEater& Player )
{
   Structs::Point tile = GetTile( Player.m_PositionVector );
   if( !IsValidTile( tile ) )
      return;

//   Alert( PointToString( tile ) );
//   if( m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.m_Header.max_solidity )
   if( IsSolid( tile.y, tile.x ) )
   {
      if( Player.m_AStarPath.size() )
      {
         Player.m_PositionVector = GetTilePos( Player.m_AStarPath.back().pos );
         Player.m_direction      = (Direction)Player.m_AStarPath.back().oreintation;
      }
      else
      {
         if( !LeftTileIsSolid(Player.m_PositionVector) )
         {
         }
         else if( !UpTileIsSolid(Player.m_PositionVector) )
         {
         }
         else if( !RightTileIsSolid(Player.m_PositionVector) )
         {
         }
         else if( !DownTileIsSolid(Player.m_PositionVector) )
         {
         }
      }
   }
}

std::string MainWindow::GetMapString( int Map )
{
   std::ostringstream current_map;
   int map;
   if( Map == -1 )
      map = m_Level;
   else
      map = Map;

   m_Mutex.Lock();
   switch( map )
   {
      case TITLE_MAP:
      {
         current_map << "TITLE_MAP";
      } break;
      case YOUWIN_MAP:
      {
         current_map << "YOUWIN_MAP";
      } break;
      case GAMEOVER_MAP:
      {
         current_map << "GAMEOVER_MAP";
      } break;
   }

   if( map >= FIRST_LEVEL_MAP )
   {
      current_map << "LEVEL:" << map;
   }
   m_Mutex.Release();

   return current_map.str();
}

std::string MainWindow::GetGamemodeString( int GameMode )
{
   std::string gamemode;
   m_Mutex.Lock();
   int mode;
   if( GameMode == -1 )
      mode = m_GameMode;
   else
      mode = GameMode;
   switch( mode )
   {
      case TITLE_SCREEN:
      {
         gamemode = "TITLE_SCREEN";
      } break;
      case  GAME_IN_PROGRESS:
      {
         gamemode = "GAME_IN_PROGRESS";
      } break;
      case  GAME_OVER:
      {
         gamemode = "GAME_OVER";
      } break;
      case  YOU_WIN_MODE:
      {
         gamemode = "YOU_WIN_MODE";
      } break;
      case  TWO_PLAYER_MODE:
      {
         gamemode = "TWO_PLAYER_MODE";
      } break;
      case CHOOSE_CLIENT_OR_SERVER_MODE:
      {
         gamemode = "CHOOSE_CLIENT_OR_SERVER_MODE";
      } break;
      case CONNECT_MODE:
      {
         gamemode = "CONNECT_MODE";
      } break;
      case GET_SERVER_INFORMATION_FROM_USER:
      {
         gamemode = "GET_SERVER_INFORMATION_FROM_USER";
      } break;
      case LISTEN_MODE:
      {
         gamemode = "LISTEN_MODE";
      } break;
      case VIEW_HIGH_SCORES:
      {
         gamemode = "VIEW_HIGH_SCORES";
      } break;
      case VIEW_STATS:
      {
         gamemode = "VIEW_STATS";
      } break;
   }
   m_Mutex.Release();
   return gamemode;
}

bool MainWindow::IsValidTile( Structs::Point tile )
{
   if( !m_TileMap.m_Header.width || !m_TileMap.m_Header.height )
      return false;
   if( (tile.x > m_TileMap.m_Header.width || tile.x < 0) || ( tile.y > m_TileMap.m_Header.height || tile.y < 0 ) )
      return false;
   return true;
}

bool MainWindow::LeftTileIsSolid( Vector2D position )
{
   Structs::Point tile = GetTile( position );
   tile.x -= 1;
   if( !IsValidTile( tile ) )
      return false;
   if( m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.m_Header.max_solidity )
      return true;
   return false;
}

bool MainWindow::RightTileIsSolid( Vector2D position )
{
   Structs::Point tile = GetTile( position );
   tile.x += 1;
   if( !IsValidTile( tile ) )
      return false;
   if( m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.m_Header.max_solidity )
      return true;
   return false;
}

bool MainWindow::UpTileIsSolid( Vector2D position )
{
   Structs::Point tile = GetTile( position );
   tile.y -= 1;
   if( !IsValidTile( tile ) )
      return false;
   if( m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.m_Header.max_solidity )
      return true;
   return false;
}

bool MainWindow::DownTileIsSolid( Vector2D position )
{
   Structs::Point tile = GetTile( position );
   tile.y += 1;
   if( !IsValidTile( tile ) )
      return false;
   if( m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.m_Header.max_solidity )
      return true;
   return false;
}

Vector2D MainWindow::AlignToVerticalAxis( Vector2D position )
{
   Structs::Point tile = GetTile( position );
   if( !IsValidTile( tile ) )
      return position;
   Vector2D _position  = GetTilePos( tile );
   _position.m_y         = position.m_y;
   return _position;
}

Vector2D MainWindow::AlignToHorizontialAxis( Vector2D position )
{
   Structs::Point tile = GetTile( position );
   if( !IsValidTile( tile ) )
      return position;
   Vector2D _position  = GetTilePos( tile );
   _position.m_x         = position.m_x;
   return _position;
}

void MainWindow::AlignToVerticalAxis( C_DotEater& Player )
{
   Structs::Point tile       = GetTile( Player.m_PositionVector );
   if( !IsValidTile( tile ) )
      return;
   Vector2D position         = GetTilePos( tile );
   Player.m_PositionVector.m_x = position.m_x;
}

void MainWindow::AlignToHorizontialAxis( C_DotEater& Player )
{
   Structs::Point tile       = GetTile( Player.m_PositionVector );
   if( !IsValidTile( tile ) )
      return;

   // delete
   //std::ostringstream message1, message2, message3;
   //message1 << " AlignToHorizontialAxis y1: " << Player.m_PositionVector.m_y;
   //m_Console.AddLine( message1.str() );
   // end delete

   Vector2D position         = GetTilePos( tile );
   //Player.m_PositionVector.m_y = position.m_y;
   Player.m_PositionVector.m_y = position.m_y;

   // delete
   //message3 << " AlignToHorizontialAxis position.m_y: " << position.m_y;
   //m_Console.AddLine( message3.str() );
   // end delete

   // delete
   //message2 << " AlignToHorizontialAxis y2: " << Player.m_PositionVector.m_y;
   //m_Console.AddLine( message2.str() );
   // end delete
}

void MainWindow::AlignToVerticalAxis( C_GHost& ghost )
{
   Structs::Point tile      = GetTile( ghost.m_PositionVector );
   if( !IsValidTile( tile ) )
      return;
   Vector2D position        = GetTilePos( tile );
   ghost.m_PositionVector.m_x = position.m_x;
}

void MainWindow::AlignToHorizontialAxis( C_GHost& ghost )
{
   Structs::Point tile      = GetTile( ghost.m_PositionVector );
   if( !IsValidTile( tile ) )
      return;
   Vector2D position        = GetTilePos( tile );
   ghost.m_PositionVector.m_y = position.m_y;
}

void MainWindow::ResetPlayerPosition( int index )
{
   switch( index )
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

void MainWindow::ResetPlayer1Position( void )
{
   if( m_TileMap.FindSpecialDataType( "p1spawn" ) )
   {
      if( m_Players.size() )
      {
         std::deque< TileMapSpecialData > sd = m_TileMap.GetSpecialData( "p1spawn" );
         if( sd.size() )
         {
            float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
                  y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
            m_Players[0].m_PositionVector.m_x = x+(m_MapTexturedQuad.width() *(float)sd[0].x);
            m_Players[0].m_PositionVector.m_y = y-(m_MapTexturedQuad.height()*(float)sd[0].y);
         }
      }
   }
}

void MainWindow::ResetPlayer2Position( void )
{
   if( m_TileMap.FindSpecialDataType( "p2spawn" ) )
   {
      if( m_Players.size() > 1 )
      {
         std::deque< TileMapSpecialData > sd = m_TileMap.GetSpecialData( "p2spawn" );
         if( sd.size() )
         {
            //Alert( sd.size() );
            float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
                  y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
            m_Players[1].m_PositionVector.m_x = x+(m_MapTexturedQuad.width() *(float)sd[0].x);
            m_Players[1].m_PositionVector.m_y = y-(m_MapTexturedQuad.height()*(float)sd[0].y);
         }
      }
   }
}

void MainWindow::ResetRGhostPosition()
{
   float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
         y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   if( m_TileMap.FindSpecialDataType( "rspawn" ) )
   {
         std::deque< TileMapSpecialData > sd = m_TileMap.GetSpecialData( "rspawn" );
         if( sd.size() )
         {
            float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
                  y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
            m_RedGhost.m_PositionVector.m_x = x+(m_MapTexturedQuad.width() *(float)sd[0].x);
            m_RedGhost.m_PositionVector.m_y = y-(m_MapTexturedQuad.height()*(float)sd[0].y);
      }
   }
}

void MainWindow::ResetBGhostPosition()
{
   float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
         y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   if( m_TileMap.FindSpecialDataType( "bspawn" ) )
   {
         std::deque< TileMapSpecialData > sd = m_TileMap.GetSpecialData( "bspawn" );
         if( sd.size() )
         {
            float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
                  y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
            m_BlueGhost.m_PositionVector.m_x = x+(m_MapTexturedQuad.width() *(float)sd[0].x);
            m_BlueGhost.m_PositionVector.m_y = y-(m_MapTexturedQuad.height()*(float)sd[0].y);
      }
   }
}

void MainWindow::ResetYGhostPosition()
{
   float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
         y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   if( m_TileMap.FindSpecialDataType( "yspawn" ) )
   {
         std::deque< TileMapSpecialData > sd = m_TileMap.GetSpecialData( "yspawn" );
         if( sd.size() )
         {
            float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
                  y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
            m_YellowGhost.m_PositionVector.m_x = x+(m_MapTexturedQuad.width() *(float)sd[0].x);
            m_YellowGhost.m_PositionVector.m_y = y-(m_MapTexturedQuad.height()*(float)sd[0].y);
      }
   }
}

void MainWindow::ResetWGhostPosition()
{
   float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
         y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   if( m_TileMap.FindSpecialDataType( "wspawn" ) )
   {
         std::deque< TileMapSpecialData > sd = m_TileMap.GetSpecialData( "wspawn" );
         if( sd.size() )
         {
            float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
                  y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
            m_WhiteGhost.m_PositionVector.m_x = x+(m_MapTexturedQuad.width() *(float)sd[0].x);
            m_WhiteGhost.m_PositionVector.m_y = y-(m_MapTexturedQuad.height()*(float)sd[0].y);
      }
   }
}

Structs::Point MainWindow::GetWhiteGhostSpawn()
{
   Structs::Point p( -1, -1 );
   if( m_TileMap.FindSpecialDataType( "wspawn" ) )
   {
         std::deque< TileMapSpecialData > sd = m_TileMap.GetSpecialData( "wspawn" );
         if( sd.size() )
         {
            float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
                  y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
            p = GetTile( Structs::dPoint3D( x+(m_MapTexturedQuad.width() *(float)sd[0].x), y-(m_MapTexturedQuad.height()*(float)sd[0].y), 0 ) );
      }
   }
   return p;
}

Structs::Point MainWindow::GetBlueGhostSpawn()
{
   Structs::Point p( -1, -1 );
   if( m_TileMap.FindSpecialDataType( "bspawn" ) )
   {
         std::deque< TileMapSpecialData > sd = m_TileMap.GetSpecialData( "bspawn" );
         if( sd.size() )
         {
            float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
                  y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
            p = GetTile( Structs::dPoint3D( x+(m_MapTexturedQuad.width() *(float)sd[0].x), y-(m_MapTexturedQuad.height()*(float)sd[0].y), 0 ) );
      }
   }
   return p;
}

Structs::Point MainWindow::GetYellowGhostSpawn()
{
   Structs::Point p( -1, -1 );
   if( m_TileMap.FindSpecialDataType( "yspawn" ) )
   {
         std::deque< TileMapSpecialData > sd = m_TileMap.GetSpecialData( "yspawn" );
         if( sd.size() )
         {
            float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
                  y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
            p = GetTile( Structs::dPoint3D( x+(m_MapTexturedQuad.width() *(float)sd[0].x), y-(m_MapTexturedQuad.height()*(float)sd[0].y), 0 ) );
      }
   }
   return p;
}

Structs::Point MainWindow::GetRedGhostSpawn()
{
   Structs::Point p( -1, -1 );
   if( m_TileMap.FindSpecialDataType( "rspawn" ) )
   {
         std::deque< TileMapSpecialData > sd = m_TileMap.GetSpecialData( "rspawn" );
         if( sd.size() )
         {
            float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
                  y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
            p = GetTile( Structs::dPoint3D( x+(m_MapTexturedQuad.width() *(float)sd[0].x), y-(m_MapTexturedQuad.height()*(float)sd[0].y), 0 ) );
      }
   }
   return p;
}

// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::AddNewPlayer()
{
   m_Mutex.Lock();
   if( m_Players.size() == 1 )
   {
      if( m_TitleScreenOptionsListBox.GetSelected() == 2 ) // 2 player mode
      {
         ControllKeys keys;
         if( m_Players[0].m_ControllKeys.m_Up == 'W' )
         {
            keys.set( VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT );
         }
         else
         {
            keys.set( 'W', 'S', 'A', 'D' );
         }
         int i = m_Players.size();
         float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
               y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
         m_Players.push_back( C_DotEater( Vector2D( 0.0, 0.0 ), 
                              Vector2D( 0.08, 0.08 ), 
                              C_GLTexQuad( 1.0, 1.0 ), 
                              LEFT, 
                              std::vector<AStar::AStar2d::PointAndDirection>(), 
                              INITIAL_LIVES,
                              false,
                              0,
                              0,
                              keys,
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
         //SpriteMap::D_Sprite sprites = m_TileMap.GetSpriteIDs();
         if( m_TileMap.m_SpecialData.size() >= 5 )
         {
            m_Players[i].m_PositionVector.m_x = x+(m_MapTexturedQuad.width() *(float)m_TileMap.m_SpecialData[0].x);
            m_Players[i].m_PositionVector.m_y = y-(m_MapTexturedQuad.height()*(float)m_TileMap.m_SpecialData[0].y);
         }

         float tile_width  = (((m_BoundingRect.right-m_BoundingRect.left))/(float)m_TileMap.m_Header.width);
         float tile_height = (((m_BoundingRect.top-m_BoundingRect.bottom))/(float)m_TileMap.m_Header.height);

         m_Players[i].m_TexQuad.SetSize( tile_width, 
                                         tile_height );

         Vector2D VelocityVector;
         VelocityVector.m_x = tile_width*0.005;
         VelocityVector.m_y = tile_height*0.005;

         m_Players[i].m_VelocityVector = VelocityVector;

         m_Players[i].m_bIsPoweredUp = false;
         m_Players[i].m_InvincibleTime = INVINCIBLE_TIME;
         m_Players[i].m_bIsInvincible  = true;
      }
   }
   else if( m_Players.size() == 2 )
   {
      if( m_Players[0].m_Lives <= 0 && m_Players[1].m_Lives >= 2 )
      {
         if( !m_bNetworkActive )
         {
            m_Players[0].m_Lives += 1;
            m_Players[1].m_Lives -= 1;
         }
      }
      else if( m_Players[1].m_Lives <= 0 && m_Players[0].m_Lives >= 2 )
      {
         if( !m_bNetworkActive )
         {
            m_Players[0].m_Lives -= 1;
            m_Players[1].m_Lives += 1;
         }
      }
   }
   m_Mutex.Release();
}

C_DotEater MainWindow::MakePlayer1()
{
         ControllKeys keys;
/*         if( m_Players[0].m_ControllKeys.m_Up == 'W' )
         {
            keys.set( VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT );
         }
         else
         {
            keys.set( 'W', 'S', 'A', 'D' );
         }*/
   C_DotEater p1( Vector2D( 0.0, 0.0 ), 
                              Vector2D( 0.08, 0.08 ), 
                              C_GLTexQuad( 1.0, 1.0 ), 
                              LEFT, 
                              std::vector<AStar::AStar2d::PointAndDirection>(), 
                              INITIAL_LIVES,
                              false,
                              0,
                              0,
                              keys,
                              C_Flash( false, 250 ),
                              0,
                              true,
                              0,
                              C_DotEater::ItemInventory(),
                              LEFT,
                              Structs::Point(),
                              C_DotEater::EAT_DOT,
                              m_Players.size(),
                              0 );
   return p1;
}

C_DotEater MainWindow::MakePlayer2()
{

         ControllKeys keys;
/*
         if( m_Players[0].m_ControllKeys.m_Up == 'W' )
         {
            keys.set( VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT );
         }
         else
         {
            keys.set( 'W', 'S', 'A', 'D' );
         }
*/
   C_DotEater p2( Vector2D( 0.0, 0.0 ), 
                              Vector2D( 0.08, 0.08 ), 
                              C_GLTexQuad( 1.0, 1.0 ), 
                              LEFT, 
                              std::vector<AStar::AStar2d::PointAndDirection>(), 
                              INITIAL_LIVES,
                              false,
                              0,
                              0,
                              keys,
                              C_Flash( false, 250 ),
                              0,
                              true,
                              0,
                              C_DotEater::ItemInventory(),
                              LEFT,
                              Structs::Point(),
                              C_DotEater::EAT_DOT,
                              m_Players.size(),
                              0 );
   return p2;
}


// Accepts: void
//
// Purpose: 
//
// Returns: void

Structs::Point MainWindow::GetRandomTile()
{
   if( m_TileMap.m_Header.width && m_TileMap.m_Header.height && m_WalkableTiles.size() )
      return m_WalkableTiles[m_Rand.rand()%(m_WalkableTiles.size())];
   return Structs::Point();
}

//*****************************************************************************
//******************************* MainWindow::SortHighScores ******************
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::SortHighScores()
{
   std::list< std::string > Row;

   std::map< int, std::string, HighestFirst > HighScoreMap;

   std::vector< std::list< std::string > > Rows = m_HighScoresListBox.GetRows();

   for( int i=0; i<Rows.size() ; i++ )
   {
      std::list< std::string > row = Rows[i];
      std::list< std::string >::iterator itr = row.begin();
      itr++;
      if( row.size() > 1 )
      {
         HighScoreMap[ atoi( (*itr).c_str() ) ] = *row.begin();
      }
   }

   m_HighScoresListBox.clear();
   for( std::map< int, std::string, HighestFirst >::iterator i = HighScoreMap.begin();
        i != HighScoreMap.end() ;
        i++ )
   {
      std::ostringstream tmp;
      tmp << (*i).first;
      Row.push_back( (*i).second ); Row.push_back( tmp.str() ); 
      m_HighScoresListBox.AddRow( Row );
      Row.clear();
   }
}

//*****************************************************************************
//******************************* MainWindow::AddHighScore ********************
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::AddHighScore( std::string name, int score )
{
   std::list< std::string > Row;
   std::ostringstream tmp;
   tmp << score;
   Row.push_back( name ); Row.push_back( tmp.str() ); 
   m_HighScoresListBox.AddRow( Row );
   SortHighScores();
}

// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::GetPathAndDirection( C_GHost& ghost, Structs::Point end )
{
   if( m_TileMap.GetTiles().size() == 0 )
      return;
   ghost.m_AStarPath = m_Astar.CalculatePath( m_TileMap.GetTiles(),
                                              GetTile(ghost.m_PositionVector), 
                                              end, 
                                              m_TileMap.m_Header.max_solidity, 
                                              false ); // allow diagonal movement

   if( ghost.m_AStarPath.size() )
   {
      ghost.m_direction = GetDirectionToNextTile( GetTile(ghost.m_PositionVector),
                                                  ghost.m_AStarPath.back().pos );
      ghost.m_Goal = end;
   }
   else
   {
      ghost.m_direction = GetDirectionToNextTile( GetTile(ghost.m_PositionVector),
                                                  end );
   }

}

//*****************************************************************************
//******************************* MainWindow::GetPathAndDirection *************
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::GetPathAndDirection( C_DotEater& Player, Structs::Point end )
{
   if( m_TileMap.GetTiles().size() == 0 )
      return;
   Player.m_AStarPath = m_Astar.CalculatePath( m_TileMap.GetTiles(), 
                                               GetTile(Player.m_PositionVector), 
                                               end, 
                                               m_TileMap.m_Header.max_solidity, 
                                               false ); // allow diagonal movement

   if( Player.m_AStarPath.size() )
   {
      Player.m_direction = GetDirectionToNextTile( GetTile(Player.m_PositionVector),
                                                   Player.m_AStarPath.back().pos );
   }
   else
   {
      Player.m_direction = GetDirectionToNextTile( GetTile(Player.m_PositionVector),
                                                   end );
   }

}

//*****************************************************************************
//******************************* MainWindow::GetDirectionToNextTile **********
// Accepts: void
//
// Purpose: 
//
// Returns: void

Direction MainWindow::GetDirectionToNextTile( Structs::Point start, Structs::Point end )
{
   Direction direction;
   if( start.x == end.x ) // in same col
   {
      if( start.y < end.y )
      {
         return direction = DOWN;
      }
      return direction = UP;
   }

   if( start.y == end.y ) // in same row
   {
      if( start.x < end.x )
      {
         return direction = RIGHT;
      }
      return direction = LEFT;
   }

   return direction = UPLEFT;
}

// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::AddDots( std::deque<Structs::Point> PowerPellets )
{
   m_Mutex.Lock();
   m_Dots.clear();
   GetRandomItems();
   if( m_TileMap.m_Header.height>0 && m_TileMap.m_Header.width>0 )
   {
      std::deque<C_Dot> valid_power_pellets;
      for( int row=0; row<m_TileMap.m_Header.height; row++ ) // for each row
      {
         for( int col=0; col< m_TileMap.m_Header.width ; col++ ) // for each coloum
         {
            // 
            if( m_TileMap[row][col].m_Solidity == 0 && !IsRandomItemTile( col, row ) )
            {
               m_Dots.push_back( C_Dot( col, row, false ) );
               m_WalkableTiles.push_back( Structs::Point( col, row ) );
               for( int i=0; i<PowerPellets.size(); i++ )
               {
                  if( m_Dots.back().m_tile == PowerPellets[i] )
                  {
                     m_Dots.pop_back();
                     valid_power_pellets.push_back( C_Dot( col, row, true ) );
                     PowerPellets.erase( PowerPellets.begin()+i );
                     break;
                  }
               }
            }
         }
      }
      for( int i=0; i<valid_power_pellets.size() ;i++ )
      {
         m_Dots.push_back( valid_power_pellets[i] );
      }
   }
   BuildDotVBOs();
   m_Mutex.Release();
}

bool MainWindow::DotOnTile( Structs::Point tile )
{
   m_Mutex.Lock();
   for( int i=0 ; i<m_Dots.size() ; i++ )
   {
      if( m_Dots[i].m_tile == tile )
      {
          return true;
      }
   }
   return false;
   m_Mutex.Release();
}

void MainWindow::GetBigDots( std::deque<Structs::Point>& big_dots )
{
   if( m_TileMap.FindSpecialDataType( "bigdot" ) )
   {
      std::deque< TileMapSpecialData > sd = m_TileMap.GetSpecialData( "bigdot" );
      if( sd.size() )
      {
         for( int i=0; i<sd.size() ;i++ )
         {
            big_dots.push_back( Structs::Point( sd[i].x, sd[i].y ) );
         }
      }
   }
}

void MainWindow::GetRandomItems( void )
{
   if( m_bNetworkActive )
   {
//      if( IamServer() )
//      {
         m_RandomItems.clear();
//      }
      if( m_TileMap.FindSpecialDataType( "randomitem" ) )
      {
         std::deque< TileMapSpecialData > sdq = m_TileMap.GetSpecialData( "randomitem" );
         for( int i=0; i<sdq.size(); i++ )
         {
//            if( IamServer() )
               m_RandomItems.push_back( sdq[i] );
         }
         for( int i=0; i<m_RandomItems.size(); i++ )
         {
            if( IamServer() )
               m_RandomItems[i].id = m_Rand.rand()%m_RandomItemTextureMap.size();
         }
      }
      SendRandomItems( 0 );
   }
   else if( !m_bNetworkActive )
   {
      m_RandomItems.clear();
      if( m_TileMap.FindSpecialDataType( "randomitem" ) )
      {
        std::deque< TileMapSpecialData > sdq = m_TileMap.GetSpecialData( "randomitem" );
         for( int i=0; i<sdq.size(); i++ )
            m_RandomItems.push_back( sdq[i] );
         for( int i=0; i<m_RandomItems.size(); i++ )
            m_RandomItems[i].id = m_Rand.rand()%m_RandomItemTextureMap.size();
      }
   }
}

bool MainWindow::IsRandomItemTile( int x, int y )
{
   for( int i=0; i<m_RandomItems.size() ; i++ )
   {
      if( m_RandomItems[i].x == x &&
          m_RandomItems[i].y == y )
      {
          return true;
      }
   }
   return false;
}

bool MainWindow::IsRandomItemTile( Structs::Point tile )
{
   return IsRandomItemTile( tile.x, tile.y );
}

int MainWindow::GetRandomItemIndex( int x, int y )
{
   for( int i=0; i<m_RandomItems.size() ; ++i )
   {
      if( m_RandomItems[i].x == x &&
          m_RandomItems[i].y == y )
      {
         return i;
      }
   }
   return -1;    
}

int MainWindow::GetRandomItemIndex( Structs::Point tile )
{
   return GetRandomItemIndex( tile.x, tile.y );
}

void MainWindow::EatRandomItem( C_DotEater& Player, int index )
{
   m_Mutex.Lock();
   if( m_RandomItems.size() > index )
   {
      m_RandomItems.erase( m_RandomItems.begin()+index );
      Player.m_Score += EAT_RANDOMITEM_SCORE;
      Player.m_Inventory[index] = 1;
      PlayClip( SOUND_RANDOMITEM, MISC_CHANNEL );
      if( m_bNetworkActive )
      {
         m_EatenRandomItemIndex = index;
         NetorkCommandsQueue.push_back( COMMAND_RANDOMITEM_EATEN );
//         if( !IamServer() )
//            Alert();
      }
   }
   m_Mutex.Release();
}

//*****************************************************************************
//******************************* MainWindow::GetTileRect *********************
// Accepts: void
//
// Purpose: 
//
// Returns: void

Structs::fRect MainWindow::GetTileRect( int x, int y )
{
   Structs::fRect rect;
   float tile_width  = m_MapTexturedQuad.width();
   float tile_height = m_MapTexturedQuad.height();

   // The tile at 0,0 (the top left tile)
   rect.left   = m_BoundingRect.left+m_MapTexturedQuad.halfwidth();
   rect.top    = m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   rect.right  = m_BoundingRect.left+m_MapTexturedQuad.halfwidth()+tile_width;
   rect.bottom = m_BoundingRect.top-m_MapTexturedQuad.halfheight()-tile_height;

   rect.left   += tile_width;
   rect.top    -= tile_height;
   rect.right  += tile_width;
   rect.bottom -= tile_height;
   
   return rect;
}

//*****************************************************************************
//******************************* MainWindow::GetTilePos **********************
// Accepts: void
//
// Purpose: 
//
// Returns: void

Vector2D MainWindow::GetTilePos( Structs::Point point )
{
   float tile_width  = m_MapTexturedQuad.width();
   float tile_height = m_MapTexturedQuad.height();

   float tile_zero_pos_x = m_BoundingRect.left+m_MapTexturedQuad.halfwidth();
   float tile_zero_pos_y = m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   
   return Vector2D( tile_zero_pos_x + (tile_width*point.x ),
                    tile_zero_pos_y - (tile_height*point.y) );
}


//*****************************************************************************
//******************************* MainWindow::GetTilePos **********************
// Accepts: void
//
// Purpose: 
//
// Returns: void

Structs::fPoint MainWindow::GetTilePos( int x, int y )
{
   float tile_width  = m_MapTexturedQuad.width();
   float tile_height = m_MapTexturedQuad.height();

   float tile_zero_pos_x = m_BoundingRect.left+m_MapTexturedQuad.halfwidth();
   float tile_zero_pos_y = m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   
   return Structs::fPoint( tile_zero_pos_x + (tile_width*x ),
                           tile_zero_pos_y - (tile_height*y) );
}

//***************************************************************************
//******************************* MainWindow::SaveIntervals *****************
// Accepts: void
//
// Purpose: 
//
// Returns: void

bool MainWindow::CollideWithSolidTile( Structs::fRect Rect )
{
   Vector2D position;
   position.m_x = Rect.left   + (Rect.right-Rect.left)*0.5;
   position.m_y = Rect.bottom + (Rect.top-Rect.bottom)*0.5;

   Structs::Point tile = GetTile( position );

   if( m_TileMap[tile.y][tile.x].m_Solidity == m_TileMap.m_Header.max_solidity )
   {
      return true;
   }
   return false;
}

// Accepts: void
//
// Purpose: 
//
// Returns: void

Structs::Point MainWindow::GetTile( const Vector2D& position )
{
   Structs::Point tile;
   if( !m_TileMap.m_Header.width || !m_TileMap.m_Header.height )
      return tile;
   long x =      (long)my_roundf((position.m_x + m_BoundingRectHalfWidthMinMapTexturedQuadhalfwidth)        * m_InvBoundingRectWidthDivTileMapWidth),
	    y = abs( (long)my_roundf((position.m_y - m_BoundingRectHalfHeight + m_MapTexturedQuad.halfheight()) * m_InvBoundingRectHeightDivTileMapHeight) );

   //Structs::Point point = Structs::Point(      (long)my_roundf( (position.m_x + m_BoundingRectHalfWidthMinMapTexturedQuadhalfwidth) * m_InvBoundingRectWidthDivTileMapWidth),
   //                                       abs( (long)my_roundf( (position.m_y - m_BoundingRectHalfHeight + m_MapTexturedQuad.halfheight()) * m_InvBoundingRectHeightDivTileMapHeight) ) );
   Structs::Point point( x, y );

   //m_bMoveGhosts = false;
   //std::ostringstream message1, message2;
   //message1 << "GetTile: point " << (std::string)point;
   //message1 << "GetTile: position x:" << position.m_x << " y: " << position.m_y;
   //m_Console.AddLine(message1.str() );

   return point;
}

Structs::Point MainWindow::GetTile( const Structs::dPoint3D& position )
{
   Structs::Point tile;
   if( !m_TileMap.m_Header.width || !m_TileMap.m_Header.height )
      return tile;
   return Structs::Point( (long)my_roundf( ( position.m_x + m_BoundingRectHalfWidthMinMapTexturedQuadhalfwidth ) * m_InvBoundingRectWidthDivTileMapWidth ),
                             abs( (long)my_roundf( ( position.m_y - m_BoundingRectHalfHeight + m_MapTexturedQuad.halfheight() ) * m_InvBoundingRectHeightDivTileMapHeight ) ) );
}

// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::SaveIntervals( void )
{
}


//***************************************************************************
//******************************* MainWindow::AllPlayersAreDead *************
// Accepts: void
//
// Purpose: set up some intitial opengl state variables
//
// Returns: void

bool MainWindow::AllPlayersAreDead()
{
   bool all_players_dead = true;
   m_Mutex.Lock();
   for( int i=0; i<m_Players.size() ; i++ )
   {
      if( m_Players[i].m_Lives > 0 )
      {
         all_players_dead = false;
         break;
      }
   }
   m_Mutex.Release();
   return all_players_dead;
}

