#include "main.h"

void MainWindow::BackButtonClick( void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      mw->m_Mutex.Lock();

      switch( mw->m_GameMode )
      {
         case GAME_IN_PROGRESS:
         {
         } break;
         case TWO_PLAYER_MODE:
         {
         } break;
         case GAME_OVER:
         {
            mw->m_GameMode = TITLE_SCREEN;
            if( mw->m_MapFilenames.size() > TITLE_MAP )
            {
               mw->LoadMap( mw->m_MapFilenames[TITLE_MAP] );
            }
         } break;
         case VIEW_HIGH_SCORES:
         {
            mw->m_GameMode = TITLE_SCREEN;
            if( mw->m_MapFilenames.size() > TITLE_MAP )
            {
               mw->LoadMap( mw->m_MapFilenames[TITLE_MAP] );
            }
         } break;
         case LISTEN_MODE:
         {
            mw->m_bShowTheConsole          = false;
            mw->m_bTheConsoleAcceptsTyping = false;
            mw->StopServerThread();
            mw->m_GameMode                 = TITLE_SCREEN;
            if( mw->m_MapFilenames.size() > TITLE_MAP )
            {
               mw->LoadMap( mw->m_MapFilenames[TITLE_MAP] );
            }
         } break;
         case CONNECT_MODE:
         {
            mw->m_GamesListBox.clearrows();
            mw->m_bShowTheConsole          = false;
            mw->m_bTheConsoleAcceptsTyping = false;
            mw->StopClientThread();
            mw->m_GameMode                 = TITLE_SCREEN;
            if( mw->m_MapFilenames.size() > TITLE_MAP )
            {
               mw->LoadMap( mw->m_MapFilenames[TITLE_MAP] );
            }
         } break;
         case CHOOSE_CLIENT_OR_SERVER_MODE:
         {
            mw->m_GameMode = TITLE_SCREEN;
            if( mw->m_MapFilenames.size() > TITLE_MAP )
            {
               mw->LoadMap( mw->m_MapFilenames[TITLE_MAP] );
            }
         } break;
         case GET_SERVER_INFORMATION_FROM_USER:
         {
            mw->m_GameMode = TITLE_SCREEN;
            mw->m_GamesListBox.clearrows();
            mw->m_bShowTheConsole          = false;
            mw->m_bTheConsoleAcceptsTyping = false;
         } break;
      }
      mw->m_Mutex.Release();
   }
}

void MainWindow::WhatIsMyIPButtonClick( void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      mw->m_Mutex.Lock();
      switch( mw->m_GameMode )
      {
         case GAME_IN_PROGRESS:
         {
         } break;
         case TWO_PLAYER_MODE:
         {
         } break;
         case GAME_OVER:
         {
         } break;
         case VIEW_HIGH_SCORES:
         {
         } break;
         case LISTEN_MODE:
         {
            std::deque<std::string> Ipaddresses;
            mw->EnumerateLocalAddresses( Ipaddresses );
            std::string ip = "no ip external addresses found";
            if( Ipaddresses.size() )
            {
               for( int i=0 ; i<Ipaddresses.size() ; i++ )
               {
                  if( Ipaddresses[i].find( "127.0.0.1" ) == 0 )
                  {
                     mw->m_Console.AddLine( "Your Local IP is " + Ipaddresses[i] );
                     continue;
                  }
                  if( Ipaddresses[i].find( "192.168." ) == 0 )
                  {
                     mw->m_Console.AddLine( "Your LAN IP is " + Ipaddresses[i] );
                     continue;
                  }
                  if( Ipaddresses[i].find( "10." ) == 0 )
                  {
                     mw->m_Console.AddLine( "Your LAN IP is " + Ipaddresses[i] );
                     continue;
                  }
                  ip = Ipaddresses[i];
                  mw->m_Console.AddLine( "Your External IP is " + Ipaddresses[i] );
                  mw->CopyTextToClipBoard( ip );
               }
            }
            mw->m_Console.AddLine( "Your IP has been copied to the clipboard" );
            mw->m_Console.SetCurrentLine( ip );
         } break;
         case CONNECT_MODE:
         {
         } break;
         case CHOOSE_CLIENT_OR_SERVER_MODE:
         {
         } break;
         case GET_SERVER_INFORMATION_FROM_USER:
         {
         } break;
      }
      mw->m_Mutex.Release();
   }
}

void MainWindow::StartButtonClick( void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      if( !mw->m_bRemoteVersionReceived && mw->IamServer() )
      {
         mw->m_Mutex.Lock();
         mw->m_Console.AddLine( "Remote Version Not Received, game cannot start." );
         mw->m_Mutex.Release();
         return;
      }
      mw->m_Mutex.Lock();
      switch( mw->m_GameMode )
      {
         case GAME_IN_PROGRESS:
         {
         } break;
         case TWO_PLAYER_MODE:
         {
         } break;
         case GAME_OVER:
         {
            if( mw->m_bNetworkActive )
            {
               if( mw->IamServer() )
               {
                  mw->CheckCommand( "/start" );
               }
               else
               {
                  mw->m_Mutex.Lock();
                  mw->NetorkCommandsQueue.push_back( COMMAND_REQUEST_START );
                  if( mw->m_PlayerNames.size() )
                  mw->m_Console.AddLine( "Start the Game Already!" );
                  mw->m_Mutex.Release();
               }
            }
         } break;
         case YOU_WIN_MODE:
         {
            if( mw->m_bNetworkActive )
            {
               if( mw->IamServer() )
               {
                  mw->CheckCommand( "/start" );
               }
               else
               {
                  mw->m_Mutex.Lock();
                  mw->NetorkCommandsQueue.push_back( COMMAND_REQUEST_START );
                  if( mw->m_PlayerNames.size() )
                  mw->m_Console.AddLine( "Start the Game Already!" );
                  mw->m_Mutex.Release();
               }
            }
         } break;
         case VIEW_HIGH_SCORES:
         {
         } break;
         case LISTEN_MODE:
         {
            if( mw->m_bNetworkActive )
            {
                if( mw->IamServer() )
                {
                   mw->CheckCommand( "/start" );
                }
                else
                {
                   mw->m_Mutex.Lock();
                   mw->NetorkCommandsQueue.push_back( COMMAND_REQUEST_START );
                   mw->m_Console.AddLine( "Start the Game Already!" );
                   mw->m_Mutex.Release();
                }
            }
         } break;
         case CONNECT_MODE:
         {
            if( mw->m_bNetworkActive )
            {
                if( !mw->IamServer() )
                {
                   mw->m_Mutex.Lock();
                   mw->NetorkCommandsQueue.push_back( COMMAND_REQUEST_START );
                   mw->m_Console.AddLine( "Start the Game Already!" );
                   mw->m_Mutex.Release();
                }
            }
         } break;
         case CHOOSE_CLIENT_OR_SERVER_MODE:
         {
         } break;
         case GET_SERVER_INFORMATION_FROM_USER:
         {
         } break;
         case VIEW_STATS:
         {
            if( mw->m_bNetworkActive )
            {
                if( mw->IamServer() )
                {
                   mw->CheckCommand( "/s" );
                }
                else
                {
                   mw->m_Mutex.Lock();
                   mw->NetorkCommandsQueue.push_back( COMMAND_REQUEST_START );
                   mw->m_Console.AddLine( "Start the Game Already!" );
                   mw->m_Mutex.Release();
                }
            }
            else
            {
               mw->HandleUpKeys( VK_RETURN, LPARAM(), false );
            }
         } break;
      }
      mw->m_Mutex.Release();
   }
}

void MainWindow::RetryButtonClick( void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      mw->m_Mutex.Lock();

      switch( mw->m_GameMode )
      {
         case GAME_IN_PROGRESS:
         {
         } break;
         case TWO_PLAYER_MODE:
         {
         } break;
         case GAME_OVER:
         {
         } break;
         case VIEW_HIGH_SCORES:
         {
         } break;
         case LISTEN_MODE:
         {
            if( mw->m_bServerCreationFailed && !mw->m_bProcessServerThread && !mw->m_bNetworkThreadActive )
            {
               mw->StopServerThread();
               mw->StartServerThread();
            }
         } break;
         case CONNECT_MODE:
         {
            if( mw->m_bConnectionFailed && !mw->m_bProcessClientThread && !mw->m_bNetworkThreadActive )
            {
               mw->StopClientThread();
               mw->StartClientThread();
            }
         } break;
         case CHOOSE_CLIENT_OR_SERVER_MODE:
         {
         } break;
         case GET_SERVER_INFORMATION_FROM_USER:
         {
         } break;
      }
      mw->m_Mutex.Release();
   }
}

void MainWindow::ReconnectButtonClick( void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      mw->m_Mutex.Lock();

      switch( mw->m_GameMode )
      {
         case GAME_IN_PROGRESS:
         {
         } break;
         case TWO_PLAYER_MODE:
         {
         } break;
         case GAME_OVER:
         {
         } break;
         case VIEW_HIGH_SCORES:
         {
         } break;
         case LISTEN_MODE:
         {
         } break;
         case CONNECT_MODE:
         {
         } break;
         case CHOOSE_CLIENT_OR_SERVER_MODE:
         {
         } break;
         case GET_SERVER_INFORMATION_FROM_USER:
         {
         } break;
      }
      mw->m_Mutex.Release();
   }
}
