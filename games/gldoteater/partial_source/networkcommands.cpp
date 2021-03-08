#include "main.h"

void MainWindow::SendNetworkData()
{
   m_Mutex.Lock();
   while( NetorkCommandsQueue.size() )
   {
      NetorkCommandsQueue.pop_front();
   }
   m_Mutex.Release();
}

void MainWindow::SendNetworkCommands()
{
   m_Mutex.Lock();

   char *command = new char[2];

   while( NetorkCommandsQueue.size() )
   {
      command[0] = NetorkCommandsQueue[0];
      command[1] = 0;

      switch( NetorkCommandsQueue[0] )
      {
         case COMMAND_START:
         {
            command[1] = 0;
         } break;
         case COMMAND_NEW_ROUND:
         {
            command[1] = m_Level;
         } break;
         case COMMAND_RESET_PLAYER_POSITION:
         {
            if( m_Players.size() > 1 )
            {
               command[1] = 1;
            }
            else if( m_Players.size() > 0 )
            {
               command[1] = 0;
            }
         } break;
         case COMMAND_VIEW_STATS:
         {
            command[1] = 0;
         } break;
         case COMMAND_GAME_OVER:
         {
            command[1] = 0;
         } break;
         case COMMAND_PLAYER1_DIED:
         {
            command[1] = 0;
         } break;
         case COMMAND_PLAYER2_DIED:
         {
            command[1] = 0;
         } break;
         case COMMAND_PAUSE:
         {
            command[1] = m_bGamePaused;
         } break;
         case COMMAND_LOAD_MAP:
         {
//            command[1] = m_MapToLoad;
            command[1] = m_CurrentMap;
         } break;
         case COMMAND_YOU_WIN:
         {
            command[1] = 0;
         } break;
         case COMMAND_GAME_MODE:
         {
            command[1] = m_GameMode;
         } break;
         case COMMAND_REQUEST_START:
         {
            command[1] = 0;
         } break;
         case COMMAND_RANDOMITEM_EATEN:
         {
            command[1] = m_EatenRandomItemIndex;
         } break;
      }
      if( IamServer() )
      {
         SendToClient( (char*)command, sizeof(char)*2, 0,
                       ENET_CHANNEL_COMMAND, 
                       ENET_PACKET_FLAG_RELIABLE );
      }
      else
      {
         SendToHost( (char*)command, sizeof(char)*2,
                      ENET_CHANNEL_COMMAND, 
                      ENET_PACKET_FLAG_RELIABLE );
      }
      NetorkCommandsQueue.pop_front();
   }
   delete[] command;
   m_Mutex.Release();
}
