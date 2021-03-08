#include "main.h"

void MainWindow::StartServerThread()
{
   m_Mutex.Create( 0, "enetservermutex" );
   m_bProcessServerThread = true;
   m_EnetServerThread.Start();
}

void MainWindow::StopServerThread()
{
   m_bProcessServerThread = false;
   EraseGameFromMasterServer();
}


DWORD WINAPI MainWindow::EnetServerThreadFunc( LPVOID m )
{
   DWORD ret = 0;
   MainWindow *mw = reinterpret_cast<MainWindow*>(m);
   if( !mw )
     return ret = 1;
   mw->m_bNetworkThreadActive = true;

   mw->CreateServer();
   if( !mw->m_bServerCreationFailed )
   {
      mw->ManageEnetServer();
      mw->DestroyServer();
   }

   mw->m_bNetworkActive = false;

   mw->m_Mutex.Release();
   mw->m_bNetworkThreadActive = false;
   mw->m_GameMode                 = LISTEN_MODE;
   mw->m_MapToLoad                = TITLE_MAP;
   if( !mw->m_bServerCreationFailed )
   {
      mw->m_GameMode                 = TITLE_SCREEN;
      mw->m_bShowTheConsole          = false;
      mw->m_bTheConsoleAcceptsTyping = false;
   }

   mw->m_TimeSinceConnectInterval.ResetTime();
   return ret;
}

void MainWindow::DisconnectClient( int index )
{
   ENetEvent event;
    
   enet_peer_disconnect( &m_Server->peers[0] );

   // Allow up to 3 seconds for the disconnect to succeed
   // and drop any packets received packets.
   while( enet_host_service( m_Server, &event, 3000 ) > 0 )
   {
      switch( event.type )
      {
         case ENET_EVENT_TYPE_RECEIVE:
         {
            enet_packet_destroy( event.packet );
         } break;
         case ENET_EVENT_TYPE_DISCONNECT:
         {
            m_Mutex.Lock();
            m_Console.AddLine( "Disconnection succeeded." );
            m_Mutex.Release();
         } break;
      }
   }
    
    // We've arrived here, so the disconnect attempt didn't 
    // succeed yet.  Force the connection down.             
    enet_peer_reset( &m_Server->peers[0] );
}


void MainWindow::SendToClient( char *data, long length, int index, int channel, int flag )
{
   if( !m_Server )
      return;

   m_Mutex.Lock();
   if( m_TotalDataSent == 0 )
   {
      m_TotalDataSentThisSecondInterval.Begin();
   }

   m_TotalDataSent += length;
   m_TotalDataSentSStream.str( std::string() );
   m_TotalDataSentSStream << m_TotalDataSent/1000.0;

   if( m_TotalDataSentThisSecondInterval.TimeSinceLastCall() > 1000 )
   {
      m_TotalDataSentThisSecond =+ length/1000.0;
      m_TransferRateSStream.str(std::string());
      m_TransferRateSStream << m_TotalDataSentThisSecond;
      m_TotalDataSentThisSecondInterval.Begin();
   }
   m_Mutex.Release();

   // Create a packet of size lenght containing data
   ENetPacket * packet = enet_packet_create( data, 
                                             length, 
                                             flag );
    
    // Send the packet to the peer over channel id channel. 
    // One could also broadcast the packet by         
    // enet_host_broadcast( m_Client, channel, packet );
    enet_peer_send( &m_Server->peers[index], channel, packet );

    // One could just use enet_host_service() instead. 
    enet_host_flush( m_Server );
//    enet_host_flush( m_Client );
}

void MainWindow::ManageEnetServer()
{
   if( !m_Server )
      return;

   ENetEvent event;

   TimeInterval Interval;

   double deltaNetworkFrame = 0;

   SendPlayerInfo();

   while( (enet_host_service( m_Server, &event, 10 ) >= 0) && m_bProcessServerThread )
   {
      deltaNetworkFrame = Interval.TimeSinceLastCall();
      Interval.Begin();  // start recording the time taken to reach the 

      m_deltaTickTime                       += deltaNetworkFrame;
      m_deltaPositionAndDirectionUptateTime += deltaNetworkFrame;
      if( m_bNetworkActive )
      {
//         m_deltaDataRecv                       += deltaNetworkFrame;
         m_deltaSendPing                       += deltaNetworkFrame;
      }

//      m_PingSStream.str( std::string() );
//      m_PingSStream << event.peer->roundTripTime;

      switch( event.type )
      {
         case ENET_EVENT_TYPE_CONNECT:
         {
            if( m_bNetworkActive )
            {
               // prevent the more then one client connect               
               enet_peer_disconnect( (event.peer) );
               m_Mutex.Lock();
               m_Console.AddLine( "Rejected an attemp to connect by another client" );
               m_Mutex.Release();
               break;
            }
//               msg << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port;
            char str[MAX_PATH];
            std::ostringstream msg;
            if( enet_address_get_host( &(event.peer->address), str, MAX_PATH-1 ) == 0 )
            {
               msg << "A new client connected from " << str << ":" << m_Port;
            }
            else
            {
               msg << "A new client connected from " << event.peer->address.host << ":" << m_Port;
            }

            // Store any relevant client information here.
            event.peer->data = (void*)"Client information";
            m_bNetworkActive = true;
            EraseGameFromMasterServer();
            m_Mutex.Lock();
            m_Console.AddLine( msg.str() );
            //m_Console.AddLine( "Type /start to begin the game" );
            PlayClip( SOUND_CONNECT, SOUND_CONNECT );
            FlashWindow( m_WinBase, false );
            m_TimeSinceConnectInterval.Begin();
            m_Mutex.Release();
         } break;

         case ENET_EVENT_TYPE_RECEIVE:
         {
//            char str[event.packet->dataLength+1];
//            str[event.packet->dataLength] = 0;
//            strncpy( str, (char*)event.packet->data, event.packet->dataLength );
//            std::ostringstream tmp;
//            tmp << "A packet of length " << event.packet->dataLength <<  
//                   " containing "        << "\"" << str << "\""      <<
//                   " was received from " << event.peer->data         << 
//                   " on channel "        << "\"" << event.channelID  << "\"" ;
//            m_Mutex.Lock();
//            m_Console.AddLine( tmp.str() );
//            m_Mutex.Release();
//            m_deltaDataRecv = 0;
            switch( event.channelID )
            {
               case ENET_CHANNEL_TEXT:
               {
				  std::vector<char> str(event.packet->dataLength+1);
                  str[event.packet->dataLength] = 0;
                  strncpy( &str[0], (char*)event.packet->data, event.packet->dataLength );
                  m_Mutex.Lock();
                  std::string tmp = m_Console.GetCurrentLine();
                  m_Console.AddLine( &str[0] );
                  m_Console.SetCurrentLine( tmp );
                  if( !m_bisVisible )
                  {
                     FlashWindow( m_WinBase, false );
                  }
                  m_Mutex.Release();
               } break;
               case ENET_CHANNEL_PLAYERINFO:
               {
                    CheckPlayerInfo( (char*)event.packet->data, event.packet->dataLength );
               } break;               
               case ENET_CHANNEL_P2_POSITION_AND_DIRECTION:
               {
//                  if( event.packet->dataLength != sizeof(PositionAndDirection) )
//                     break;

                  // dont move the player if they died
                  if( m_Players.size() > 1 )
                  {
                    if( m_Players[1].m_TimeTillRespawn > 0 )
                    {
                        break;
                    }
                  }


                  PositionDirectionAndRequestedDirection pd = *reinterpret_cast<PositionDirectionAndRequestedDirection*>( event.packet->data );
                  if( m_Players.size() > 1 )
                  {
                     m_Mutex.Lock();
//                     m_Console.AddLine( "pad" );
                     if( pd.id > m_PositionAndDirectionToHostID )
                     {
                        m_Players[1].m_direction          = pd.m_Direction;
                        m_Players[1].m_RequestedDirection = pd.m_RequestedDirection;
                        if( pd.m_Tile != GetTile( m_Players[1].m_PositionVector ) )
                           m_Players[1].m_PositionVector = GetTilePos( pd.m_Tile );
                     }
                     m_Mutex.Release();
                  }
               } break;
               case ENET_CHANNEL_SEND_PING:
               {
                  HandleENET_CHANNEL_SEND_PING( (char*)event.packet->data, event.packet->dataLength );
               } break;
               case ENET_CHANNEL_RECV_SEND_PING:
               {
                  HandleENET_CHANNEL_RECV_SEND_PING( (char*)event.packet->data, event.packet->dataLength );
               } break;
//               case ENET_CHANNEL_SEND_REPLY_PING:
//              {
//                  HandleENET_CHANNEL_SEND_REPLY_PING( (char*)event.packet->data, event.packet->dataLength );
//               } break;
//               case ENET_CHANNEL_RECV_REPLY_PING:
//               {
//                  HandleENET_CHANNEL_RECV_REPLY_PING( (char*)event.packet->data, event.packet->dataLength );
//               } break;
               case ENET_CHANNEL_DOTS_EATEN:
               {
                  GetDotsEaten( (char*)event.packet->data, event.packet->dataLength );
               } break;
               case ENET_CHANNEL_COMMAND:
               {
                  int command      = (int)(event.packet->data[0]);
                  int command_data = (int)(event.packet->data[1]);
                  switch( command )
                  {
                     case COMMAND_RANDOMITEM_EATEN:
                     {
                        m_Mutex.Lock();
                        if( m_Players.size() )
                        {
                           EatRandomItem( m_Players[0], command_data );
                        }
                        m_Mutex.Release();
                     } break;
                     case COMMAND_GAME_MODE:
                     {
                        m_Mutex.Lock();
                        m_Console.AddLine( (long)command_data );
                        m_GameMode = command_data;
                        m_Mutex.Release();
                     } break;
                     case COMMAND_PAUSE:
                     {
                        m_Mutex.Lock();
                        m_bGamePaused = command_data;
                        m_Mutex.Release();
                     } break;
                     case COMMAND_LOAD_MAP:
                     {
                        m_Mutex.Lock();
                        m_Console.AddLine( GetMapString(command_data) );
                        m_MapToLoad = command_data;
                        m_Mutex.Release();
                     } break;
                     case COMMAND_REQUEST_START:
                     {
                        m_Mutex.Lock();
                        m_Console.AddLine( "Player 2 requests that you: Start the game Already!" );
                        m_Mutex.Release();
                     } break;
                     default:
                     {
                     };
                  }
               } break;
               default:
               {
               };
            }

           // Clean up the packet now that we're done using it.
           enet_packet_destroy( event.packet );
         } break;
           
         case ENET_EVENT_TYPE_DISCONNECT:
         {
            std::ostringstream msg;
//            msg << event.peer ->data << " disconected";
            m_Mutex.Lock();
            if( m_PlayerNames.size() > 1 )
               msg <<  m_PlayerNames[1] << " (" << event.peer->data << ") disconected";
            else
               msg << event.peer ->data << " disconected";
            m_Console.AddLine( msg.str() );
            m_Mutex.Release();

            // Reset the peer's client information. 
            event.peer->data           = NULL;
            m_GameMode                 = TITLE_SCREEN;
            m_MapToLoad                = TITLE_MAP;
            m_bShowTheConsole          = false;
            m_bTheConsoleAcceptsTyping = false;
            m_bNetworkActive           = false;
            m_bProcessServerThread     = false;
//            Alert();
         } break;
      }

      if( m_deltaTickTime >= TICK_TIME )
      {
         m_Mutex.Lock();
         if( m_GameMode == GAME_IN_PROGRESS )
         {
            SendDotsEatenToClient( 0 );
            UpdateAllClients();
         }
         m_Mutex.Release();
         m_deltaTickTime                       = 0;
         m_deltaPositionAndDirectionUptateTime = 0;
      }
      if( m_deltaSendPing > UPDATE_PING )
      {
          SendPing();
          m_deltaSendPing = 0;
      }
      if( m_deltaPositionAndDirectionUptateTime >= UPDATE_POSITION_AND_DIRECTION )
      {
         m_deltaPositionAndDirectionUptateTime = 0;
      }
      SendNetworkCommands();
      SendNetworkData();
   }
}

void MainWindow::DestroyServer()
{
   m_bProcessServerThread = false;
   if( !m_Server )
      return;
//   for( int index=0; index<m_Server->peerCount ; index++ )
//      DisconnectClient( index );
   DisconnectClient( 0 );
   enet_host_destroy( m_Server );
   m_Mutex.Lock();
   m_Console.AddLine( "Server Destroyed" );
   m_Mutex.Release();
   m_Server = NULL;
}


void MainWindow::CreateServer()
{
   ENetAddress  address;

   // Bind the m_Server to the default localhost.
   // A specific host address can be specified by
   
//   enet_address_set_host( &address, "x.x.x.x" );

   m_Mutex.Lock();
   EnumerateLocalAddresses();
   m_Mutex.Release();

/*
   if( m_Ipaddresses.size() > 1 )
   {
      enet_address_set_host( &address, m_Ipaddresses[1].c_str() );
//      enet_address_set_host( &address, "localhost" );
   }
   else if( m_Ipaddresses.size() > 0 )
   {
      enet_address_set_host( &address, m_Ipaddresses[0].c_str() );
//      enet_address_set_host( &address, "localhost" );
   }
   else
   {
      address.host = ENET_HOST_ANY;
//      enet_address_set_host( &address, "localhost" );
   }
*/
   address.host = ENET_HOST_ANY;

   // Bind the m_Server to a port
   address.port = m_Port;

   m_Server = enet_host_create( &address, // the address to bind the m_Server host to 
                                32,       // allow up to 32 clients and/or outgoing connections 
                                57600 / 8,   // 56K modem with 56 Kbps downstream bandwidth
                                14400 / 8 ); // 56K modem with 14 Kbps upstream bandwidth 

   if( m_Server == NULL )
   {
      m_Console.AddLine( "An error occurred while trying to create an ENet Server" );
      m_bServerCreationFailed = true;
      m_bProcessServerThread  = false;
   }
   else
   {
      m_bServerCreationFailed = false;
      m_bProcessServerThread  = true;
      std::ostringstream tmp, tmp2;
      tmp << m_Port;
      tmp2 << address.host;
      m_Console.AddLine( std::string( "Server successfully created on " ) + tmp2.str() + ":" + tmp.str() );
   }
}

void MainWindow::UpdateAllClients()
{
/*
   if( m_Players.size() > 0 )
   {
      m_Mutex.Lock();
      PlayerData p1( m_Players[0] );
      p1.m_tile = GetTile( m_Players[0].m_PositionVector );
      m_Mutex.Release();
//      SendToClient( (char*)&p1, sizeof(p1), 0,
//                    ENET_CHANNEL_P1_DATA, ENET_PACKET_FLAG_RELIABLE );
   }
*/
   if( m_Players.size() > 0 )
   {
      m_Mutex.Lock();
      std::vector< char > packet;
      GenerateGameState( packet );
      SendToClient( (char*)&packet[0], packet.size(), 0,
                     ENET_CHANNEL_GAMESTATE, 0 );
      m_Mutex.Release();
   }

//   bool byte;
//   SendToClient( (char*)&byte, sizeof(byte), 0,
//                  ENET_CHANNEL_S_PING, ENET_PACKET_FLAG_RELIABLE );
//   m_deltaPing = 0;
//   m_bPingSent = true;
}

void MainWindow::SendDotsEatenToClient( int index )
{
   m_Mutex.Lock();
   //  if( m_TileMap.Width() > 255 || m_TileMap.Height() > 255 )
   //  make aother function to send their positions

   if( m_DotsEaten.size() )
   {
      //            number of dots    * size of our point
      int  length = m_DotsEaten.size()* sizeof(unsigned char)* 2;
      unsigned char *points = new unsigned char[ length ];

      for( int i=0, c=0; 
           i<m_DotsEaten.size() ; 
           i++, c += sizeof( unsigned char)* 2 )
      {
         points[c]   = m_DotsEaten[i].x;
         points[c+1] = m_DotsEaten[i].y;
      }

      m_DotsEaten.clear();

      SendToClient( (char*)points, 
                    length,
                    index,
                    ENET_CHANNEL_DOTS_EATEN, 
                    ENET_PACKET_FLAG_RELIABLE );

      delete[] points;
   }
   m_Mutex.Release();
}
