#include "main.h"

//*****************************************************************************
//******************************* MainWindow::GetQuickMessage *****************
// Accepts: void
//
// Purpose: 
//
// Returns: void

std::string MainWindow::GetQuickMessage( int index )
{
   std::string tmp;
   if( m_QuickMessages.size() > index )
      tmp = m_QuickMessages[index];   
   return tmp;
}

int MainWindow::CheckCommand( std::string command )
{
   if( command.length() == 0 )
      return 0;

   m_Mutex.Lock();
   if( command[0] == '/' )
   {
      if( (m_GameMode == VIEW_STATS   || 
           m_GameMode == YOU_WIN_MODE || 
           m_GameMode == GAME_OVER) 
          && m_bNetworkActive )
      {
         if( (command.find( "/s" ) == 0) )
         {
            CheckCommand( "/clear" );
            if( IamServer() )
            {
               switch( m_GameMode )
               {
                  case YOU_WIN_MODE:
                  {
                     InitialiseServerOnlineGame();
                  } break;
                  case GAME_OVER:
                  {
                     InitialiseServerOnlineGame();
                  } break;
                  case VIEW_STATS:
                  {
                     m_Mutex.Lock();
                     if( m_bGotToGameOverAfterStats )
                     {
                        m_bGotToGameOverAfterStats = false;
                        m_GameMode = GAME_OVER;
                        if( m_MapFilenames.size() > GAMEOVER_MAP )
                        {
                           LoadMap( m_MapFilenames[GAMEOVER_MAP] );
                           m_CurrentMap = GAMEOVER_MAP;
                        }
                        NetorkCommandsQueue.push_back( COMMAND_GAME_OVER );
                     }
                     else
                     {
                        IncrementLevel();
                        if( WinCondition() )
                        {
                           m_Level = 0;
                           if( m_MapFilenames.size() > YOUWIN_MAP )
                           {
                              LoadMap( m_MapFilenames[YOUWIN_MAP] );
                              m_CurrentMap = YOUWIN_MAP;
                           }
                           m_GameMode = YOU_WIN_MODE;
                           NetorkCommandsQueue.push_back( COMMAND_YOU_WIN );
                        }
                        else
                        {
                           InitialiseNewOnlineRound();
                           m_GameMode   = GAME_IN_PROGRESS;
                           m_CurrentMap = m_Level;
                           NetorkCommandsQueue.push_back( COMMAND_NEW_ROUND );
                        }
                     }
                     m_Mutex.Release();
                  } break;
               }
               m_Mutex.Lock();
               if( m_bShowOnlineGameMode )
                  m_Console.AddLine( GetGamemodeString() );
               m_Mutex.Release();
            }
         }
      }

      if( command.length() > 1 && isdigit( command[1] ) )
      {
         command.erase( command.begin() );
         std::string tmp = GetQuickMessage( atoi( command.c_str() ) );
         if( tmp.size() )
         {
            m_Console.AddLine( m_Nick + ": " + tmp );
            if( m_bNetworkActive  )
            {
               std::string text = m_Console.GetLastLineAdded();

               if( m_Server != NULL )
               {
                  SendToClient( (char*)text.c_str(), text.length(), 0,
                                ENET_CHANNEL_TEXT, ENET_PACKET_FLAG_RELIABLE );
               }
               else
               {
                  SendToHost( (char*)text.c_str(), text.length(), 
                              ENET_CHANNEL_TEXT, ENET_PACKET_FLAG_RELIABLE );
               }
            }
         }
      }

      else if( (command.find( "/exit" ) == 0) || (command.find("/quit") == 0) )
      {
         if( m_WinBase )
         {
            ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
         }
         else
         {
            m_bQuit = true;
         }
      }
      else if( (command == "/clear") )
      {
         m_Console.Reset();
      }
      else if( command == "/moveghosts" )
      {
         TOGGLE( m_bMoveGhosts );
         std::ostringstream tmp;
         tmp << "MoveGhosts set to: " << (m_bMoveGhosts) ? "true" : "false";
         m_Console.AddLine( tmp.str() );
      }
      else if( (command == "/c") )
      {
         short command = COMMAND_START;
         SendToClient( (char*)&command, sizeof(command), 0,
                       ENET_CHANNEL_COMMAND, ENET_PACKET_FLAG_RELIABLE );
         m_Console.AddLine( "tried to send" );
      }
      else if( (command == "/start") )
      {
         if( !m_bRemoteVersionReceived )
         {
            m_Mutex.Lock();
            m_Console.AddLine( "Remote Version Not Received, game cannot start." );
            m_Mutex.Release();
         }
         else
         {
            if( m_bNetworkActive && (m_GameMode == LISTEN_MODE) )
            {
               InitialiseServerOnlineGame();
               CheckCommand( "/clear" );
            }
         }
      }
      else if( command.find( "/nick " ) == 0 )
      {
         m_Nick = command.erase( 0, sizeof("/nick ")-1 );
         if( m_Nick.size() > MAX_NICK_LENGHT )
            m_Nick = m_Nick.substr( 0, MAX_NICK_LENGHT );
         m_Console.AddLine( std::string( "Your new nick is: " ) + m_Nick );
      }
      else if( command.find( "/mode " ) == 0 )
      {
         std::string tmp = command.erase( 0, sizeof("/mode ")-1 );
         m_GameMode = atoi(tmp.c_str());
         if( m_bShowOnlineGameMode )
            m_Console.AddLine( GetGamemodeString() );
      }
      else if( command.find( "/mode" ) == 0 )
      {
         m_Console.AddLine( GetGamemodeString() );
      }
      else if( command.find( "/novsync" ) == 0 )
      {
         SetVSync( 0 );
      }
      else if( command.find( "/vsync" ) == 0 )
      {
         //SetVSync( 0 );
      }
   }
   m_Mutex.Release();
   return 0;
}

