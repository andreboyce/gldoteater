#include "main.h"

/*
goal   of rghost
dir    of rghost
rghost iseaten
rghost PoweredDownTime
rghost IsPoweredDown

goal of bghost
dir  of bghost
bghost iseaten
bghost PoweredDownTime
bghost IsPoweredDown

goal of yghost
dir  of yghost
yghost iseaten
yghost PoweredDownTime
yghost IsPoweredDown

goal of wghost
dir  of wghost
wghost iseaten
wghost PoweredDownTime
wghost IsPoweredDown

pos  of pacman1
dir  of pacman1
rdir of pacman1

pos  of pacman2
dir  of pacman2
rdir of pacman2

id   of randomitems
which random items were eaten

timestamp
*/

void MainWindow::GenerateGameState( std::vector< char >& packet )
{
   m_Mutex.Lock();
   packet.reserve( 1000 );
   int index = m_LastGameStateID++;
   InsertData( packet, (char*)&index, sizeof(index) );

   InsertData( packet, (char*)&m_TimeSinceRoundStarted, sizeof(m_TimeSinceRoundStarted) );

   InsertGhostData( packet, m_BlueGhost   );
   InsertGhostData( packet, m_WhiteGhost  );
   InsertGhostData( packet, m_RedGhost    );
   InsertGhostData( packet, m_YellowGhost );

   if( m_Players.size() )
      InsertPlayerData( packet, m_Players[0] );

   if( m_Players.size() > 1 )
   {
     if( m_Players[1].m_TimeTillRespawn > 0 )
     {
        InsertData( packet, (char*)&m_Players[1].m_Lives,           sizeof(m_Players[1].m_Lives)           );
        InsertData( packet, (char*)&m_Players[1].m_TimeTillRespawn, sizeof(m_Players[1].m_TimeTillRespawn) );
        InsertData( packet, (char*)&m_Players[1].m_PositionVector,  sizeof(m_Players[1].m_PositionVector)  );
     }
   }

   m_Mutex.Release();
}

void MainWindow::ReadGameState( std::vector< char >& packet )
{
   m_Mutex.Lock();
   packet.reserve( 1000 );
   int index = 0;
   RetreiveData( packet, (char*)&index, sizeof(index) );
   if( index < m_LastGameStateID )
     return;
   m_LastGameStateID = index;

   unsigned long TimeSinceRoundStarted = 0;
   RetreiveData( packet, (char*)&TimeSinceRoundStarted, sizeof(m_TimeSinceRoundStarted) );
   if( (TimeSinceRoundStarted<m_TimeSinceRoundStarted) )
     m_DeltaClientServerRoundStart = (m_TimeSinceRoundStarted-TimeSinceRoundStarted);
   else
     m_DeltaClientServerRoundStart = (TimeSinceRoundStarted-m_TimeSinceRoundStarted);

   RetreiveGhostData( packet, m_BlueGhost   );
   RetreiveGhostData( packet, m_WhiteGhost  );
   RetreiveGhostData( packet, m_RedGhost    );
   RetreiveGhostData( packet, m_YellowGhost );

   if( m_Players.size() )
      RetreivePlayerData( packet, m_Players[0] );

   if( packet.size() )
   {
      if( m_Players.size() > 1 )
      {
        if( m_Players[1].m_TimeTillRespawn > 0 )
        {
           RetreiveData( packet, (char*)&m_Players[1].m_Lives,           sizeof(m_Players[1].m_Lives)           );
           RetreiveData( packet, (char*)&m_Players[1].m_TimeTillRespawn, sizeof(m_Players[1].m_TimeTillRespawn) );
           RetreiveData( packet, (char*)&m_Players[1].m_PositionVector,  sizeof(m_Players[1].m_PositionVector)  );
        }
      }
   }

//   }
//   if( m_Players.size() )
//      MovePlayerOnNetworkUpdate( m_Players[0] );
   m_Mutex.Release();
}

void MainWindow::InsertPlayerData( std::vector<char>& packet, C_DotEater& Player )
{
//   InsertData( packet, (char*)&Player.m_PositionVector,       sizeof(Player.m_PositionVector)       );
//   InsertData( packet, (char*)&Player.m_VelocityVector,       sizeof(Player.m_VelocityVector)       );
   Player.m_Tile = GetTile( Player.m_PositionVector );
   InsertData( packet, (char*)&Player.m_Tile,                 sizeof(Player.m_Tile)                 );
   InsertData( packet, (char*)&Player.m_direction,            sizeof(Player.m_direction)            );
   InsertData( packet, (char*)&Player.m_Lives,                sizeof(Player.m_Lives)                );
   InsertData( packet, (char*)&Player.m_bIsPoweredUp,         sizeof(Player.m_bIsPoweredUp)         );
   InsertData( packet, (char*)&Player.m_Score,                sizeof(Player.m_Score)                );
   InsertData( packet, (char*)&Player.m_InvincibleTime,       sizeof(Player.m_InvincibleTime)       );
   InsertData( packet, (char*)&Player.m_bIsInvincible,        sizeof(Player.m_bIsInvincible)        );
   InsertData( packet, (char*)&Player.m_LastLifeUpFromPoints, sizeof(Player.m_LastLifeUpFromPoints) );
   InsertData( packet, (char*)&Player.m_RequestedDirection,   sizeof(Player.m_RequestedDirection)   );
   InsertData( packet, (char*)&Player.m_TimeTillRespawn,      sizeof(Player.m_TimeTillRespawn)      );
}

void MainWindow::RetreivePlayerData( std::vector<char>& packet, C_DotEater& Player )
{
//   RetreiveData( packet, (char*)&Player.m_PositionVector,       sizeof(Player.m_PositionVector)       );
//   RetreiveData( packet, (char*)&Player.m_VelocityVector,       sizeof(Player.m_VelocityVector)       );
   RetreiveData( packet, (char*)&Player.m_Tile,                 sizeof(Player.m_Tile)                 );
   if( Player.m_Tile != GetTile( Player.m_PositionVector ) )
      Player.m_PositionVector = GetTilePos( Player.m_Tile );

   RetreiveData( packet, (char*)&Player.m_direction,            sizeof(Player.m_direction)            );
   RetreiveData( packet, (char*)&Player.m_Lives,                sizeof(Player.m_Lives)                );
   RetreiveData( packet, (char*)&Player.m_bIsPoweredUp,         sizeof(Player.m_bIsPoweredUp)         );
   RetreiveData( packet, (char*)&Player.m_Score,                sizeof(Player.m_Score)                );
   RetreiveData( packet, (char*)&Player.m_InvincibleTime,       sizeof(Player.m_InvincibleTime)       );
   RetreiveData( packet, (char*)&Player.m_bIsInvincible,        sizeof(Player.m_bIsInvincible)        );
   RetreiveData( packet, (char*)&Player.m_LastLifeUpFromPoints, sizeof(Player.m_LastLifeUpFromPoints) );
   RetreiveData( packet, (char*)&Player.m_RequestedDirection,   sizeof(Player.m_RequestedDirection)   );
   RetreiveData( packet, (char*)&Player.m_TimeTillRespawn,      sizeof(Player.m_TimeTillRespawn)      );
}

void MainWindow::InsertGhostData( std::vector<char>& packet, C_GHost& Ghost )
{
//   InsertData( packet, (char*)&Ghost.m_PositionVector,  sizeof(Ghost.m_PositionVector)  );
   Ghost.m_Tile = GetTile( Ghost.m_PositionVector );
   InsertData( packet, (char*)&Ghost.m_Tile,                 sizeof(Ghost.m_Tile)       );
   InsertData( packet, (char*)&Ghost.m_direction,       sizeof(Ghost.m_direction)       );
   InsertData( packet, (char*)&Ghost.m_bIsEaten,        sizeof(Ghost.m_bIsEaten)        );
   InsertData( packet, (char*)&Ghost.m_bIsPoweredDown,  sizeof(Ghost.m_bIsPoweredDown)  );
   InsertData( packet, (char*)&Ghost.m_PoweredDownTime, sizeof(Ghost.m_PoweredDownTime) );
   InsertData( packet, (char*)&Ghost.m_Goal,            sizeof(Ghost.m_Goal)            );
}

void MainWindow::RetreiveGhostData( std::vector<char>& packet, C_GHost& Ghost )
{
//   RetreiveData( packet, (char*)&Ghost.m_PositionVector,  sizeof(Ghost.m_PositionVector)  );
   RetreiveData( packet, (char*)&Ghost.m_Tile,            sizeof(Ghost.m_Tile)            );
   if( Ghost.m_Tile != GetTile( Ghost.m_PositionVector ) )
      Ghost.m_PositionVector = GetTilePos( Ghost.m_Tile );
   RetreiveData( packet, (char*)&Ghost.m_direction,       sizeof(Ghost.m_direction)       );
   RetreiveData( packet, (char*)&Ghost.m_bIsEaten,        sizeof(Ghost.m_bIsEaten)        );
   RetreiveData( packet, (char*)&Ghost.m_bIsPoweredDown,  sizeof(Ghost.m_bIsPoweredDown)  );
   RetreiveData( packet, (char*)&Ghost.m_PoweredDownTime, sizeof(Ghost.m_PoweredDownTime) );
   RetreiveData( packet, (char*)&Ghost.m_Goal,            sizeof(Ghost.m_Goal)            );
}

void MainWindow::InsertData( std::vector<char>& packet, char *data, unsigned long sizeofdata )
{
   std::vector<char>::reference back = packet.back();
   packet.insert( packet.end(), sizeofdata, 0 );
   std::memcpy( (void*)((&back)+1), data, sizeofdata );
}

void MainWindow::RetreiveData( std::vector<char>& packet, char *data, unsigned long sizeofdata )
{
   std::memcpy( data, &packet.front(), 
               ( sizeofdata > packet.size() ) ? packet.size() : sizeofdata ); // Array bounds check
   packet.erase( packet.begin(), ( packet.end() < (packet.begin()+sizeofdata) ) ? packet.end() : (packet.begin()+sizeofdata) );
}
