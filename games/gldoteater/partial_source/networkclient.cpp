#include "main.h"

void MainWindow::StartClientThread()
{
   m_Mutex.Create( 0, "enetclientmutex" );
   m_bProcessClientThread = true;
   m_EnetClientThread.Start();
}

void MainWindow::StopClientThread()
{
   m_bProcessClientThread = false;
}

DWORD WINAPI MainWindow::EnetClientThreadFunc( LPVOID m )
{
   DWORD ret = 0;
   MainWindow *mw = reinterpret_cast<MainWindow*>(m);
   if( !mw )
     return ret = 1;
   mw->m_bNetworkThreadActive = true;
   
   mw->CreateClient();
   mw->ConnectToHost();
   mw->ManageEnetClient();
   mw->DestroyClient();

   mw->m_bNetworkActive = false;

   mw->m_Mutex.Release();
   mw->m_bNetworkThreadActive = false;
//   mw->m_GameMode                 = TITLE_SCREEN;
//   mw->m_MapToLoad                = TITLE_MAP;
//   mw->m_bShowTheConsole          = false;
//   mw->m_bTheConsoleAcceptsTyping = false;
   mw->m_TimeSinceConnectInterval.ResetTime();
   return ret;
}

void MainWindow::DisconnectFromHost( )
{
   ENetEvent event;
    
   enet_peer_disconnect( &m_Client->peers[0] );

   // Allow up to 3 seconds for the disconnect to succeed
   // and drop any packets received packets.
   while( enet_host_service( m_Client, &event, 3000 ) > 0 )
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
    enet_peer_reset( &m_Client->peers[0] );
}

void MainWindow::SendToHost( char *data, long length, int channel, int flag )
{
   if( !m_Client )
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

   ENetPeer *peer = &m_Client->peers[0];//m_HostPeer;//

   // Create a packet of size lenght containing data
   ENetPacket * packet = enet_packet_create( data, 
                                             length, 
                                             flag );
    
    // Send the packet to the peer over channel id channel. 
    // One could also broadcast the packet by         
    // enet_host_broadcast( m_Client, channel, packet );
    enet_peer_send( peer, channel, packet );

    // One could just use enet_host_service() instead. 
//    enet_host_flush( m_Server );
    enet_host_flush( m_Client );
}

void MainWindow::ManageEnetClient()
{
   if( !m_Client )
      return;
   ENetEvent event;

   TimeInterval Interval;

   double deltaNetworkFrame = 0;

   SendPlayerInfo();

   while( ( enet_host_service( m_Client, &event, 10 ) >= 0 ) && m_bProcessClientThread )
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
            std::ostringstream msg;
            msg << "Connected To Server";

            m_Mutex.Lock();
            m_Console.AddLine( msg.str() );
            m_Mutex.Release();

           // Store any relevant client information here. 
           event.peer->data = (void*)"Server information";
           m_bNetworkActive = true;

           m_Mutex.Lock();
           PlayClip( SOUND_CONNECT, SOUND_CONNECT );
           FlashWindow( m_WinBase, false );
           m_Mutex.Release();

         }break;
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
               case ENET_CHANNEL_GAMESTATE:
               {
                  m_Mutex.Lock();
                  std::vector< char > packet;
                  packet.reserve( event.packet->dataLength );
                  InsertData( packet, (char*)event.packet->data, event.packet->dataLength );
                  ReadGameState( packet );
                  m_Mutex.Release();
               } break;
               case ENET_CHANNEL_PLAYERINFO:
               {
                    CheckPlayerInfo( (char*)event.packet->data, event.packet->dataLength );
               } break;               
               case ENET_CHANNEL_RANDOMITEMS:
               {
                  m_Mutex.Lock();
                  std::vector< char > packet;
                  packet.reserve( event.packet->dataLength );
                  InsertData( packet, (char*)event.packet->data, event.packet->dataLength );
                  GetRandomItemsFromPacket( packet );
                  m_Mutex.Release();
               } break;
               case ENET_CHANNEL_RANDOMITEM_DATA:
               {
               } break;
               case ENET_CHANNEL_P1_POSITION_AND_DIRECTION:
               {
                  PositionDirectionAndRequestedDirection pd = *reinterpret_cast<PositionDirectionAndRequestedDirection*>( event.packet->data );
                  if( m_Players.size() > 0 )
                  {
                     m_Mutex.Lock();
                     if( pd.id > m_PositionAndDirectionToHostID )
                     {
                        m_Players[0].m_direction          = pd.m_Direction;
                        m_Players[0].m_RequestedDirection = pd.m_RequestedDirection;
                        if( pd.m_Tile != GetTile( m_Players[0].m_PositionVector ) )
                           m_Players[0].m_PositionVector = GetTilePos( pd.m_Tile );
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
//               {
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
               case ENET_CHANNEL_GHOST_POSITION_AND_DIRECTION:
               {
                  GetGhostPositionAndDirection( (char*)event.packet->data, event.packet->dataLength );
               } break;
               case ENET_CHANNEL_COMMAND:
               {
                  int command      = (int)(event.packet->data[0]);
                  int command_data = (int)(event.packet->data[1]);
//                  std::ostringstream tmp;
//                  tmp << command;
//                  m_Console.AddLine( "ENET_CHANNEL_COMMAND: " + tmp.str() );
                  switch( command )
                  {
                     case COMMAND_RANDOMITEM_EATEN:
                     {
                        m_Mutex.Lock();
                        if( m_Players.size() > 1 )
                        {
                           EatRandomItem( m_Players[1], command_data );
                        }
                        m_Mutex.Release();
                     } break;
                     case COMMAND_GAME_MODE:
                     {
                        m_Mutex.Lock();
                        //"GameMode: "
                        m_Console.AddLine( GetGamemodeString( command_data ) );
                        m_GameMode = command_data;
                        m_Mutex.Release();
                     } break;
                     case COMMAND_START:
                     {
                        m_Mutex.Lock();
                        m_bInitializeOnlineGame = true;
                        m_Mutex.Release();
                     } break;
                     case COMMAND_GAME_OVER:
                     {
                        m_Mutex.Lock();
                        m_MapToLoad = GAMEOVER_MAP;
                        m_GameMode  = GAME_OVER;
                        m_Console.AddLine( "Game over :(" );
                        AddHighScore( m_Nick, m_Players[1].m_Score );
                        if( m_Players.size() > 1 )
                           m_Players[1].m_Lives = INITIAL_LIVES;
                        m_Mutex.Release();
                     } break;
                     case COMMAND_YOU_WIN:
                     {
                        m_Mutex.Lock();
                        m_MapToLoad = YOUWIN_MAP;
                        m_GameMode  = YOU_WIN_MODE;
                        m_Console.AddLine( "You guys win!" );
                        AddHighScore( m_Nick, m_Players[1].m_Score );
                        if( m_Players.size() > 1 )
                           m_Players[1].m_Lives = INITIAL_LIVES;
                        m_Mutex.Release();
                     } break;
                     case COMMAND_NEW_ROUND:
                     {
                        m_Mutex.Lock();
                        m_Level    = command_data;
                        m_GameMode = GAME_IN_PROGRESS;
                        if( m_bShowOnlineGameMode )
                           m_Console.AddLine( GetGamemodeString() );
                        CheckCommand( "/clear" );
                        m_bInitializeNewOnlineRound = true;
                        m_Mutex.Release();
                     } break;
                     case COMMAND_RESET_PLAYER_POSITION:
                     {
                        ResetPlayerPosition( command_data );
                     } break;
                     case COMMAND_VIEW_STATS:
                     {
                        m_Mutex.Lock();
                        m_GameMode = VIEW_STATS;
                        if( m_bShowOnlineGameMode )
                           m_Console.AddLine( GetGamemodeString() );
                        m_Console.AddLine( "Wating for host to start next round..." );
                        m_MapToLoad = STATS_MAP;
                        m_Mutex.Release();
                     } break;
                     case COMMAND_PLAYER1_DIED:
                     {
                        m_Mutex.Lock();
//                        if( m_Players.size() > 0 )
//                           m_Players.erase( m_Players.begin() );
                        m_Mutex.Release();
                     } break;
                     case COMMAND_PLAYER2_DIED:
                     {
                        m_Mutex.Lock();
//                        if( m_Players.size() > 1 )
//                           m_Players.erase( m_Players.begin()+1 );
                        m_Mutex.Release();
                     } break;
                     case COMMAND_PAUSE:
                     {
                        m_Mutex.Lock();
                        m_bGamePaused = (bool)command_data;
                        m_Mutex.Release();
                     } break;
                     case COMMAND_LOAD_MAP:
                     {
                        m_Mutex.Lock();
                        m_Console.AddLine( GetMapString(command_data) );
                        m_MapToLoad = command_data;
                        m_Mutex.Release();
                     } break;
                     default:
                     {
                     }
                  }
               } break;
               default:
               {
               };
            }

           // Clean up the packet now that we're done using it. 
           enet_packet_destroy( event.packet );
         }break;
           
         case ENET_EVENT_TYPE_DISCONNECT:
         {
            std::ostringstream msg;
            m_Mutex.Lock();
            if( m_PlayerNames.size() )
               msg << m_PlayerNames[0] << " (" << event.peer->data << ") disconected";
            else
               msg << event.peer->data << " disconected";
            m_Console.AddLine( msg.str() );
            m_Mutex.Release();

            // Reset the peer's client information.
            event.peer->data = NULL;

            m_Mutex.Lock();
            m_GameMode                 = TITLE_SCREEN;
            m_MapToLoad                = TITLE_MAP;
            m_bShowTheConsole          = false;
            m_bTheConsoleAcceptsTyping = false;
            m_bProcessClientThread     = false;
            m_bNetworkActive           = false;
//            m_bTheConsoleAcceptsTyping = false;
//            m_bShowTheConsole          = false;
            m_Mutex.Release();
         } break;
      }

      if( m_deltaTickTime >= TICK_TIME )
      {
         SendStateToServer();
         m_deltaTickTime = 0;
         m_deltaPositionAndDirectionUptateTime = 0;
      }
      if( m_deltaSendPing >= UPDATE_PING )
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


void MainWindow::DestroyClient()
{
   m_bProcessClientThread = false;
   if( !m_Client )
      return;

   DisconnectFromHost();
   enet_host_destroy( m_Client );
   m_Mutex.Lock();
   m_Console.AddLine( "Client Destroyed" );
   m_Mutex.Release();
   m_Client = NULL;
   m_bNetworkActive = false;
}

void MainWindow::CreateClient()
{
   m_Client = enet_host_create( NULL,              // create a client host
//                                ENET_CHANNEL_SIZE, // only allow 1 outgoing connection
                                1,
                                57600 / 8,         // 56K modem with 56 Kbps downstream bandwidth
                                14400 / 8 );       // 56K modem with 14 Kbps upstream bandwidth 

   if( m_Client == NULL )
   {
      m_Mutex.Lock();
      m_Console.AddLine( "An error occurred while trying to create an ENet client host." );
      m_bProcessClientThread  = false;
      m_Mutex.Release();
   }
   else
   {
      m_Mutex.Lock();
      m_Console.AddLine( "An enet client was successfully created." );
      m_Mutex.Release();
   }
}


void MainWindow::ConnectToHost()
{
   ENetAddress address;
   ENetEvent   event;
   ENetPeer    *peer;

   // Connect to some.server.net:1234.
   enet_address_set_host( &address, m_RemoteHostAddress.c_str() );
//   enet_address_set_host( &address, "localhost" );
   address.port = m_Port;

   // Initiate the connection, allocating the two channels 0 and 1.
   peer = enet_host_connect( m_Client, &address, ENET_CHANNEL_SIZE );
    
   if( peer == NULL )
   {
      m_Mutex.Lock();
      m_Console.AddLine( "No available peers for initiating an ENet connection." );
      m_Mutex.Release();
   }
   else
   {
      // Wait up to 10 seconds for the connection attempt to succeed.
      if( enet_host_service( m_Client, &event, 10000 ) > 0 &&
          event.type == ENET_EVENT_TYPE_CONNECT )
      {
         m_HostPeer = peer;
         m_bNetworkActive = true;
         PlayClip( SOUND_CONNECT, SOUND_CONNECT );
         std::ostringstream line;
         line << "Connection to " << m_RemoteHostAddress << ":" << m_Port << " succeeded.";
         m_TimeSinceConnectInterval.Begin();
         m_bConnectionFailed = false;
         m_Mutex.Lock();
         //m_WinBase.SetText( "Client " + m_WinBase.GetTitle() );
         m_Console.AddLine( line.str() );
         m_Mutex.Release();
      }
      else
      {
         // Either the 5 seconds are up or a disconnect event was 
         // received. Reset the peer in the event the 5 seconds   
         // had run out without any significant event.            
         enet_peer_reset( peer );
         std::ostringstream line;
         line << "Connection to " << m_RemoteHostAddress << ":" << m_Port << " failed.";
         m_Mutex.Lock();
         m_bConnectionFailed    = true;
         m_bProcessClientThread = false;
         m_Console.AddLine( line.str() );
         m_Mutex.Release();
      }
   }
}


void MainWindow::SendDotsEatenToHost()
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

      SendToHost( (char*)points, 
                    length,
                    ENET_CHANNEL_DOTS_EATEN, 
                    ENET_PACKET_FLAG_RELIABLE );

      delete[] points;
   }
   m_Mutex.Release();
}
