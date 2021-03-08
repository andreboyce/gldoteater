#include "main.h"

bool MainWindow::IamServer()
{
   bool bIsServer = false;
   if( m_Server )
      bIsServer = true;
   else if( m_Client )
      bIsServer = false;
   return bIsServer;
}

bool MainWindow::ValidateIP( std::string ipstring )
{
   int count = 0;
   for( int i = 0; i < ipstring.size(); ++i )
      if( ipstring[i] == '.') count++;

   unsigned long ip = ::inet_addr( ipstring.c_str() );

   if( (ip != INADDR_NONE) && (ip != 0) && (count == 3) )
      return true;
   return false;
}

void MainWindow::InitializeEnet()
{
   if( enet_initialize() != 0 )
   {
      Alert( "An error occurred while initializing ENet." );
   }
   atexit( enet_deinitialize );
}

int MainWindow::EnumerateLocalAddresses( void )
{
   m_Ipaddresses.clear();
   std::string HostName;

   char ac[256];

   if( gethostname( ac, sizeof( ac ) ) == SOCKET_ERROR )
   {
//      m_Console.AddLine( std::string( "Error " ) + WSAGetLastError() + std::string( " when getting local host name." ) );
      return -1;
   }

   HostName = ac;
 
   struct hostent *phe = gethostbyname( ac );

   if( phe == 0 )
   {
      m_Console.AddLine( "Bad host lookup." );
      return -1;
   }
 
   for( int i = 0; phe->h_addr_list[i] != 0 ; ++i )
   {
      struct in_addr addr;
      memcpy( &addr, phe->h_addr_list[i], sizeof( struct in_addr ) );
      m_Ipaddresses.push_back( std::string( inet_ntoa( addr ) ) );
   }
   return 0;
}




void MainWindow::SendPlayer2Data()
{
/*   if( m_Players.size() > 1 )
   {
      m_Mutex.Lock();
      PlayerData p2( m_Players[1] );
//      p2.m_tile = GetTile( m_Players[1].m_PositionVector );
      m_Mutex.Release();
      SendToClient( (char*)&p2, sizeof(p2), 0,
                    ENET_CHANNEL_P2_DATA,
                    ENET_PACKET_FLAG_RELIABLE );
   }
   */
}

void MainWindow::SendStateToServer()
{
   SendPositionAndDirectionToHost();
   SendDotsEatenToHost();
}

void MainWindow::SendPositionAndDirectionToClient( int client )
{
}

void MainWindow::SendPositionAndDirectionToHost()
{
   if( m_Players.size() > 1 )
   {
      PositionDirectionAndRequestedDirection pd;
      m_Mutex.Lock();
      pd.m_Tile               = GetTile( m_Players[1].m_PositionVector );
      pd.m_Direction          = m_Players[1].m_direction;
      pd.m_RequestedDirection = m_Players[1].m_RequestedDirection;
      pd.id                   = m_PositionAndDirectionToHostID++;
      pd.timestamp            = 0;
      m_Mutex.Release();
      SendToHost( (char*)&pd, sizeof(pd), 
                   ENET_CHANNEL_P2_POSITION_AND_DIRECTION, 
                   0 );
   }
}

void MainWindow::GetDotsEaten( char* data, long packet_length )
{
   if( packet_length%2 )
   {
      return;
   }

   m_Mutex.Lock();

   int length = (int)packet_length/2;

   std::vector<Structs::Point> points( length );

   for( int i=0, c=0; 
        i<length ; 
        i++, c += sizeof( unsigned char)* 2 )
   {
      points[i].x = data[c];
      points[i].y = data[c+1];
   }

   for( int i=0; i<length ; i++ )
   {
      for( int k=0; k<m_DotsEaten.size() ; k++ )
      {
         if( points[i] == m_DotsEaten[k] )
         {
            m_DotsEaten.erase( m_DotsEaten.begin()+k );
         }
      }
   }

   for( int i=0; i<length ; i++ )
   {
      for( int k=0; k<m_Dots.size() ; k++ )
      {
         if( points[i] == m_Dots[k].m_tile )
         {
            m_Dots.erase( m_Dots.begin()+k );
            PlayClip( 2, 2 );
         }
      }
   }

   m_Mutex.Release();
}

void MainWindow::GetGhostPositionAndDirection( char* data, long paket_length )
{/*
   if( paket_length < sizeof(PositionAndDirection)*4 )
      return;

   PositionAndDirection *pd = reinterpret_cast<PositionAndDirection*>(data);
   m_Mutex.Lock();

   if( pd[0].m_tile != GetTile( m_WhiteGhost.m_PositionVector ) )
      m_WhiteGhost.m_PositionVector = GetTilePos( pd[0].m_tile );
   m_WhiteGhost.m_direction      = pd[0].m_direction;

   if( pd[1].m_tile != GetTile( m_BlueGhost.m_PositionVector ) )
      m_BlueGhost.m_PositionVector  = GetTilePos( pd[1].m_tile );
   m_BlueGhost.m_direction       = pd[1].m_direction;

   if( pd[2].m_tile != GetTile( m_RedGhost.m_PositionVector ) )
      m_RedGhost.m_PositionVector   = GetTilePos( pd[2].m_tile );
   m_RedGhost.m_direction        = pd[2].m_direction;

   if( pd[3].m_tile != GetTile( m_YellowGhost.m_PositionVector ) )
      m_YellowGhost.m_PositionVector = GetTilePos( pd[3].m_tile );
   m_YellowGhost.m_direction      = pd[3].m_direction;

   m_Mutex.Release();
   */
}

void MainWindow::SendGhostPositionAndDirectionToClient( int index )
{
//   PositionAndDirection *pd = new PositionAndDirection[4];
//   m_Mutex.Lock();

//  pd[0].m_tile      = GetTile( m_WhiteGhost.m_PositionVector );
//   pd[0].m_direction = m_WhiteGhost.m_direction;

//   pd[1].m_tile      = GetTile( m_BlueGhost.m_PositionVector );
//   pd[1].m_direction = m_BlueGhost.m_direction;

//   pd[2].m_tile      = GetTile( m_RedGhost.m_PositionVector );
//   pd[2].m_direction = m_RedGhost.m_direction;

//   pd[3].m_tile      = GetTile( m_YellowGhost.m_PositionVector );
//   pd[3].m_direction = m_YellowGhost.m_direction;

//   m_Mutex.Release();

//   SendToClient( (char*)pd, 
//                 sizeof(PositionAndDirection)*4,
//                 index,
//                 ENET_CHANNEL_GHOST_POSITION_AND_DIRECTION, 
//                 0 );
//   delete[] pd;
}

void MainWindow::SendRandomItems( int index )
{
   m_Mutex.Lock();
   if( IamServer() )
   {
      std::vector< RandomItems > RandomItemVector;
      RandomItemVector.reserve( m_RandomItems.size() );
      for( int i=0; i<m_RandomItems.size() ; i++ )
      {
         RandomItemVector.push_back( m_RandomItems[i] );
      }
      SendToClient( (char*)&RandomItemVector[0], 
                    RandomItemVector.size()*sizeof(RandomItems),
                    index, ENET_CHANNEL_RANDOMITEMS, 
                    ENET_PACKET_FLAG_RELIABLE );
   }
   m_Mutex.Release();
}

void MainWindow::GetRandomItemsFromPacket( std::vector< char >& packet )
{
   m_Mutex.Lock();
   // omg bad code need to fix!
   int items = (int)( packet.size()/sizeof(RandomItems) );

   m_RandomItems.clear();

   RandomItems ri;

   unsigned data_read = 0;
   //(data_read<=packet.size())
   for( int i=0; (i<items) ; i++ )
   {
      RetreiveData( packet, (char*)&ri, sizeof(RandomItems) );
      m_RandomItems.push_back( ri );
      data_read += sizeof(RandomItems);
   }
   m_Mutex.Release();
}

void MainWindow::SendPing()
{
//   DWORD time = m_TimeSinceConnectInterval.TimeSinceLastCall();
   m_LastPingSent.m_LocalTime  = m_TimeSinceConnectInterval.TimeSinceLastCall();
   m_LastPingSent.m_RemoteTime = m_LastPingRecv.m_RemoteTime;

   if( IamServer() )
   {
      SendToClient( (char*)&m_LastPingSent, sizeof(m_LastPingSent),
                    0, ENET_CHANNEL_SEND_PING, 0 );
   }
   else
   {
      SendToHost( (char*)&m_LastPingSent, sizeof(m_LastPingSent),
                   ENET_CHANNEL_SEND_PING, 0 );
   }
}

void MainWindow::HandleENET_CHANNEL_SEND_PING( char* data, unsigned long length )
{
   if( length < sizeof(Ping) )
      return;

   Ping *p = reinterpret_cast<Ping*>(data);

   // discard old or duplicated packets
//   if( m_LastPingSent.m_LocalTime < p->m_LocalTime )
//      return;

   if( m_LastPingRecv.m_LocalTime > p->m_LocalTime )
      return;

   m_LastPingRecv = *p;
   m_LastPingRecv.m_RemoteTime = m_TimeSinceConnectInterval.TimeSinceLastCall();

   if( IamServer() )
   {
      SendToClient( (char*)&m_LastPingRecv, sizeof(m_LastPingRecv),
                    0, ENET_CHANNEL_RECV_SEND_PING, 0 );
   }
   else
   {
      SendToHost( (char*)&m_LastPingRecv, sizeof(m_LastPingRecv),
                   ENET_CHANNEL_RECV_SEND_PING, 0 );
   }
}
void MainWindow::HandleENET_CHANNEL_RECV_SEND_PING( char* data, unsigned long length )
{
   if( length < sizeof(Ping) )
      return;

   Ping *p = reinterpret_cast<Ping*>(data);

   if( p->m_RemoteTime < m_LastPingRecv.m_RemoteTime )
      return;

   DWORD deltaPingRecv = (m_TimeSinceConnectInterval.TimeSinceLastCall()) - p->m_LocalTime;

   m_PingSStream.str(std::string());
   m_PingSStream << deltaPingRecv;
}

void MainWindow::SendPlayerInfo()
{
   unsigned __int32 ip = 0;
   char name[ MAX_NICK_LENGHT ];
   char version[ VERSION_STRING_LENGTH ];
   std::memset( name,    '\0', MAX_NICK_LENGHT       );
   std::memset( version, '\0', VERSION_STRING_LENGTH );

   std::string local_version = m_Version;
   std::strncpy( name, m_Nick.c_str(), MAX_NICK_LENGHT-1 );
   std::strncpy( version, local_version.c_str(), VERSION_STRING_LENGTH-1 );

   name[ MAX_NICK_LENGHT-1 ]          = '\0';
   version[ VERSION_STRING_LENGTH-1 ] = '\0';

   PlayerInfo p( name, ip, version );

   if( !IamServer() )
   {
      SendToHost( (char*)&p, sizeof(p),
                  ENET_CHANNEL_PLAYERINFO, 
                  ENET_PACKET_FLAG_RELIABLE );
   }
   else
   {
      SendToClient( (char*)&p, sizeof(p), 0,
                  ENET_CHANNEL_PLAYERINFO, 
                  ENET_PACKET_FLAG_RELIABLE );
   }
}

void MainWindow::CheckPlayerInfo( char* data, unsigned long length )
{
   if( length< sizeof(PlayerInfo) )
   {
      if( IamServer() )
      {
         m_bProcessServerThread = false;
      }
      else
      {
         m_bProcessClientThread = false;
      }
      return;
   }
   m_bRemoteVersionReceived = true;
   PlayerInfo *p = reinterpret_cast< PlayerInfo* >( data );
   std::string remote_version = p->version;
   std::string local_version = m_Version;
   if( local_version != remote_version )
   {
      m_Mutex.Lock();
      m_Console.AddLine( "Client Server Version mismatch!" );
      m_Console.AddLine( "local_version: " + local_version + " : " + " remote_version " + remote_version );
      if( IamServer() )
      {
         m_bProcessServerThread = false;
      }
      else
      {
         m_bProcessClientThread = false;
      }
      m_Mutex.Release();
   }
   else
   {
      m_RemotePlayerInfo = *p;
   }
}
