#include "main.h"

void MainWindow::HandleListenModeKeys( WPARAM wparam, LPARAM lparam, bool ignore_console )
{
   if( !m_bTheConsoleAcceptsTyping || ignore_console )
   {
      if( wparam == VK_BACK  )
      {
         m_GameMode = TITLE_SCREEN;
         m_Mutex.Lock();
//         m_bProcessServerThread = false;
         StopServerThread();
         m_Mutex.Release();
//         m_EnetServerThread.Stop( 0 );
      }
   }
}

void MainWindow::HandleConnectModeKeys( WPARAM wparam, LPARAM lparam, bool ignore_console )
{
   if( !m_bTheConsoleAcceptsTyping || ignore_console )
   {
      if( wparam == VK_BACK  )
      {
         m_GameMode = TITLE_SCREEN;
         m_Mutex.Lock();
//         m_bProcessClientThread = false;
         StopClientThread();
         m_Mutex.Release();
//         m_EnetClientThread.Stop();
      }
   }
}

void MainWindow::HandleChooseClientOrServerKeys( WPARAM wparam, LPARAM lparam, bool ignore_console )
{
   if( !m_bTheConsoleAcceptsTyping || ignore_console )
   {
      if( wparam == VK_BACK  )
      {
         m_GameMode = TITLE_SCREEN;
      }
      else if( wparam == VK_UP )
      {
         m_ChooseClientOrServerListBox.MoveCursorOnUpKey();
      }
      else if( wparam == VK_DOWN )
      {
         m_ChooseClientOrServerListBox.MoveCursorOnDownKey();
      }

      if( ( wparam == VK_RETURN || wparam == VK_SPACE ) )
      {
         switch( m_ChooseClientOrServerListBox.GetSelected() )
         {
            case 0:
            {
               m_Mutex.Lock();
               if( !m_Nick.size() )
               {
                  m_Console.AddLine( "You must type a name before the client connects" );
               }
               m_Players.clear();
               m_Console.AddLine( "Entering Connect Mode" );
               m_FrameRate.SetTitle( "Client " + m_WinBase.GetTitle() );
               m_GameMode = GET_SERVER_INFORMATION_FROM_USER;
               m_Mutex.Release();
               GetPlayerName();
            } break;
            case 1:
            {
               m_Mutex.Lock();
               if( !m_Nick.size() )
               {
                  m_Console.AddLine( "You must type a name before the server starts" );
               }
               m_Players.clear();
               m_Console.AddLine( "Entering Listen  Mode" );
               m_FrameRate.SetTitle( "Server " + m_WinBase.GetTitle() );
               m_GameMode = LISTEN_MODE;
               m_Mutex.Release();
               GetPlayerName();
            } break;
         }
      }
   }
}

//***************************************************************************
//******************************* MainWindow::HandleTitleScreenKeys *********
// Accepts: void
//
// Purpose:
//
// Returns: void

void MainWindow::HandleTitleScreenKeys( WPARAM wparam, LPARAM lparam, bool ignore_console )
{
   if( m_bNetworkActive && ( wparam == VK_RETURN || wparam == VK_SPACE ) )//&& (!m_bTheConsoleAcceptsTyping || ignore_console) )
   {
      m_Mutex.Lock();
      //m_Console.AddLine( "Type /s to begin new round" );
      m_GameMode = VIEW_STATS;
      //LoadMap( m_MapFilenames[STATS_MAP] );
      if( m_bShowOnlineGameMode )
         m_Console.AddLine( GetGamemodeString() );
      NetorkCommandsQueue.push_back( COMMAND_VIEW_STATS );
      m_Mutex.Release();
      return;
   }

   if( wparam == VK_UP )
   {
      m_TitleScreenOptionsListBox.MoveCursorOnUpKey();
   }
   else if( wparam == VK_DOWN )
   {
      m_TitleScreenOptionsListBox.MoveCursorOnDownKey();
   }

   if( ( wparam == VK_RETURN || wparam == VK_SPACE ) && (!m_bTheConsoleAcceptsTyping || ignore_console) )
   {
      switch( m_TitleScreenOptionsListBox.GetSelected() )
      {
         case 1:
         {
            m_Players.clear();
            try
            {
               if( m_Players.size() == 0 )
               {
                         C_DotEater m( Vector2D( 0.0, 0.0 ), 
                                       Vector2D( 0.08, 0.08 ), 
                                       C_GLTexQuad( 1.0, 1.0 ), 
                                       LEFT, 
                                       std::vector<AStar::AStar2d::PointAndDirection>(), 
                                       INITIAL_LIVES,
                                       false,
                                       0,
                                       0,
                                       ControllKeys( VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT ),
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
                  m_Players.push_back( m );
               }
            }
            catch( std::exception& e )
            {
               Alert( e.what() );
               return;
            }
            catch( ... )
            {
              // some sor of error
            }
            m_GameMode = GAME_IN_PROGRESS;
            PlayClip( 0, 0 );
            m_Level = 0;
            InitialiseNewGame();
         } break;
         case 2:
         {
            m_Players.clear();
            try
            {
               if( m_Players.size() == 0 )
               {
                  m_Players.push_back( C_DotEater( Vector2D( 0.0, 0.0 ), 
                                       Vector2D( 0.08, 0.08 ), 
                                       C_GLTexQuad( 1.0, 1.0 ), 
                                       LEFT, 
                                       std::vector<AStar::AStar2d::PointAndDirection>(), 
                                       INITIAL_LIVES,
                                       false,
                                       0,
                                       0,
                                       ControllKeys( VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT ),
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

                  m_Players.push_back( C_DotEater( Vector2D( 0.0, 0.0 ), 
                                       Vector2D( 0.08, 0.08 ), 
                                       C_GLTexQuad( 1.0, 1.0 ), 
                                       LEFT, 
                                       std::vector<AStar::AStar2d::PointAndDirection>(), 
                                       INITIAL_LIVES,
                                       false,
                                       0,
                                       0,
                                       ControllKeys( 'W', 'S', 'A', 'D' ),
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
               }
            }
            catch( std::exception& e )
            {
               Alert( e.what() );
               return;
            }
            catch( ... )
            {
               Alert( "Unknown error" );
               return;
            }
            m_GameMode = GAME_IN_PROGRESS;
            PlayClip( 0, 0 );
            m_Level = 0;
            InitialiseNewGame();
         } break;
         case 3:
         {
            m_GameMode = CHOOSE_CLIENT_OR_SERVER_MODE;
         } break;
         case 4:
         {
            m_GameMode = VIEW_HIGH_SCORES;
            m_CurrentMap = STATS_MAP;
            LoadMap( m_MapFilenames[HIGH_SCORES] );
         } break;
      }
   }
}

//***************************************************************************
//******************************* MainWindow::GetPlayerDirection ************
// Accepts: void
//
// Purpose:
//
// Returns: void

void MainWindow::GetPlayerDirection( WPARAM wparam, C_DotEater& Player )
{
   m_Mutex.Lock();
   Structs::Point tile = GetTile( Player.m_PositionVector );
   if( wparam == Player.m_ControllKeys.m_Up )
   {
      //Alert();
      if( !UpTileIsSolid( Player.m_PositionVector ) )
      {
         Player.m_direction = UP;
      }
      Player.m_RequestedDirection = UP;
   }
   else if( wparam == Player.m_ControllKeys.m_Down )
   {
      if( !DownTileIsSolid( Player.m_PositionVector ) )
      {
         Player.m_direction = DOWN;
      }
      Player.m_RequestedDirection = DOWN;
   }
   else if( wparam == Player.m_ControllKeys.m_Left )
   {
      if( !LeftTileIsSolid( Player.m_PositionVector ) )
      {
         Player.m_direction = LEFT;
      }
      Player.m_RequestedDirection = LEFT;
   }
   else if( wparam == Player.m_ControllKeys.m_Right )
   {
      if( !RightTileIsSolid( Player.m_PositionVector ) )
      {
         Player.m_direction = RIGHT;
      }
      Player.m_RequestedDirection = RIGHT;
   }
   m_Mutex.Release();
}

//***************************************************************************
//******************************* MainWindow::HandleDownKeys ****************
// Accepts: void
//
// Purpose: performs actions when keys are pressed
//          depending on what mode the game is in
//
// Returns: void

void MainWindow::HandleDownKeys( WPARAM wparam, LPARAM lparam, bool ignore_console )
{
   if( KEYDOWN(VK_CONTROL) && KEYDOWN('V') && (m_bTheConsoleAcceptsTyping && !ignore_console ) || (KEYDOWN(VK_SHIFT) && KEYDOWN(VK_INSERT)) )
   {
       std::string clipboardtext = GetTextFromClipBoard( false );
       m_Console.SetCurrentLine( clipboardtext );
       return;
   }
   else if( (KEYDOWN(VK_CONTROL) && KEYDOWN('C')) && (m_bTheConsoleAcceptsTyping && !ignore_console ) || (KEYDOWN(VK_CONTROL) && KEYDOWN(VK_INSERT)) )
   {
      CopyTextToClipBoard( m_Console.GetCurrentLine() );
      return;
   }

   switch( m_GameMode )
   {
      case TITLE_SCREEN:
      {
      } break;
      case GAME_IN_PROGRESS:
      {
         for( int i=0; i<m_Players.size() ;i++ )
         {
            GetPlayerDirection( wparam, m_Players[i] );
         }

      } break;
      case GAME_OVER:
      {
      } break;
      case YOU_WIN_MODE:
      {
      } break;
      default:
      {
      };
   };

   if( KEYDOWN(VK_DELETE) )
   {
      if( m_Players.size() )
         m_Players[0].m_Lives = 1;
      if( m_Players.size() > 1)
         m_Players[1].m_Lives = 1;
   }

   if( KEYDOWN(VK_HOME) )
   {
      if( m_bNetworkActive )
      {
         if( IamServer() )
         {
            m_Mutex.Lock();
            m_Dots.clear();
            m_GameMode   = VIEW_STATS;
            m_CurrentMap = STATS_MAP;
            if( m_MapFilenames.size() > STATS_MAP )
               LoadMap( m_MapFilenames[STATS_MAP] );
            if( m_bShowOnlineGameMode )
               m_Console.AddLine( GetGamemodeString() );
            NetorkCommandsQueue.push_back( COMMAND_VIEW_STATS );
            //m_Console.AddLine( "Type /s to begin " );
            m_Mutex.Release();
         }
      }
      else
      {
         m_Dots.clear();
      }
   }

//   if( KEYDOWN(VK_INSERT) )
//   {
//      m_Mutex.Lock();
//      NetorkCommandsQueue.push_back( COMMAND_GAME_MODE );
//      m_Console.AddLine( GetGamemodeString() );
//      m_Mutex.Release();
//   }

   if( KEYDOWN(VK_END) )
   {
      TOGGLE(m_bGamePaused);
      SaveIntervals();
      if( m_bNetworkActive )
      {
         m_Mutex.Lock();
         NetorkCommandsQueue.push_back( COMMAND_PAUSE );
         m_Mutex.Release();
      }
   }

   if( KEYDOWN(VK_F11) )
   {
      m_Mutex.Lock();
      TOGGLE(m_bSoundOn);
      std::string tmp;
      if( m_bSoundOn )
         tmp = "Sound is on";
      else
         tmp = "Sound is off";
      m_MessageQueue.AddLine( tmp );
      m_Mutex.Release();
   }

   if( KEYDOWN(VK_F9) )
   {
      m_Mutex.Lock();
      SetVSync( 0 );
      m_Mutex.Release();
   }

   if( KEYDOWN(VK_F5) )
   {
      TOGGLE( m_bMoveGhosts );
   }

   if( KEYDOWN(VK_F12) )
   {
      ::PostMessage( m_WinBase, WM_TOGGLEFULLSCREEN, 0, 0 );
      // SendMessage() wont work with the game loop so you need to use PostMessage()
   }

   if( KEYDOWN(VK_ESCAPE) )
   {
      m_bQuit = true;
      m_Mutex.Lock();
      m_bProcessServerThread = false;
      m_Mutex.Release();
      StopServerThread();
//      m_EnetServerThread.Stop( INFINITE );

      m_Mutex.Lock();
      m_bProcessClientThread = false;
      m_Mutex.Release();
      StopClientThread();
//      m_EnetClientThread.Stop( INFINITE );
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
      // SendMessage() wont work with the game loop so you need to use PostMessage()
   }
}

//***************************************************************************
//******************************* MainWindow::HandleUpKeys ******************
// Accepts: void
//
// Purpose: performs actions when keys are released
//          depending on what mode the game is in
//
// Returns: void

void MainWindow::HandleUpKeys( WPARAM wparam, LPARAM lparam, bool ignore_console )
{
   if( m_bIgnoreVK_RETURN && wparam == VK_RETURN )
   {
      m_bIgnoreVK_RETURN = false;
      return;
   }
   switch( m_GameMode )
   {
      case TITLE_SCREEN:
      {
         HandleTitleScreenKeys( wparam, lparam );
      } break;
      case VIEW_HIGH_SCORES:
      {
         if( wparam == VK_UP )
         {
            m_HighScoresListBox.MoveCursorOnUpKey();
            m_HighScoresListBox.MoveCursorUp();
         }
         if( wparam == VK_DOWN )
         {
            m_HighScoresListBox.MoveCursorOnDownKey();
            m_HighScoresListBox.MoveCursorDown();
         }

         if( wparam == VK_RETURN || wparam == VK_SPACE || wparam == VK_BACK )
         {
            if( m_bNetworkActive )
            {
               m_Mutex.Lock();
               //m_Console.AddLine( "Type /s to begin new round" );
               m_GameMode   = VIEW_STATS;
               m_CurrentMap = STATS_MAP;
               if( m_bShowOnlineGameMode )
                  m_Console.AddLine( GetGamemodeString() );
               NetorkCommandsQueue.push_back( COMMAND_VIEW_STATS );
               m_Mutex.Release();
            }
            else
            {
//               InitialiseNewGame();
//               m_GameMode = GAME_IN_PROGRESS;
//               if()
//               {
                  m_GameMode   = TITLE_SCREEN;
                  if( m_MapFilenames.size() > TITLE_MAP )
                  {
                     LoadMap( m_MapFilenames[TITLE_MAP] );
                     m_CurrentMap = TITLE_MAP;
                  }
//               }
            }
         }
      } break;
      case GAME_IN_PROGRESS:
      {
         if( !m_bNetworkActive )
         {
            if( wparam == VK_RETURN )
            {
               AddNewPlayer();
            }
            if( wparam == VK_F2 )
            {
               if( m_MapFilenames.size() >= 4  )
               {
                  LoadMap( m_MapFilenames[TITLE_MAP] );
               }
               m_GameMode   = TITLE_SCREEN;
               m_CurrentMap = TITLE_MAP;
            }
         }
      } break;
      case GAME_OVER:
      {
         if( wparam == VK_RETURN && (!m_bTheConsoleAcceptsTyping || ignore_console) )
         {
            if( m_bNetworkActive )
            {
               m_Mutex.Lock();
               //m_Console.AddLine( "Type /s to begin new round" );
               m_GameMode   = VIEW_STATS;
               m_CurrentMap = STATS_MAP;
               if( m_bShowOnlineGameMode )
                  m_Console.AddLine( GetGamemodeString() );
               NetorkCommandsQueue.push_back( COMMAND_VIEW_STATS );
               m_Mutex.Release();
            }
            else
            {
               m_GameMode   = VIEW_HIGH_SCORES;
               m_CurrentMap = HIGH_SCORES;
               m_TitleScreenOptionsListBox.SetSelected( 4 );
               if( m_MapFilenames.size() > HIGH_SCORES )
               {
                  LoadMap( m_MapFilenames[HIGH_SCORES] );
               }
            }
         }
      } break;
      case YOU_WIN_MODE:
      {
         if( wparam == VK_RETURN && (!m_bTheConsoleAcceptsTyping || ignore_console) )
         {
            m_GameMode   = VIEW_HIGH_SCORES;
            m_CurrentMap = HIGH_SCORES;
            if( m_MapFilenames.size() >= 4  )
            {
               LoadMap( m_MapFilenames[HIGH_SCORES] );
            }
            m_TitleScreenOptionsListBox.SetSelected( 4 );
            InitialiseNewGame();
         }
      } break;
      case CHOOSE_CLIENT_OR_SERVER_MODE:
      {
         HandleChooseClientOrServerKeys( wparam, lparam );
      } break;
      case CONNECT_MODE:
      {
         HandleConnectModeKeys( wparam, lparam );
      } break;
      case LISTEN_MODE:
      {
         HandleListenModeKeys( wparam, lparam );
      } break;
      case VIEW_STATS:
      {
         if( !m_bNetworkActive )
         {
            if( !m_bTheConsoleAcceptsTyping || ignore_console )
            {
               if( wparam == VK_RETURN )
               {
//                  if( WinCondition() )
//                  {//Alert("game keys.cpp: WinCondition");
//                     m_GameMode = VIEW_HIGH_SCORES;
//                     if( m_MapFilenames.size() > HIGH_SCORES )
//                     {
//                        LoadMap( m_MapFilenames[HIGH_SCORES] );
//                        m_CurrentMap = HIGH_SCORES;
//                     }
//                  }
//                  else
//                  {
                     if( m_Players.size() )
                     {
                        IncrementLevel();
                     }
//                  }
               }
            }
         }
         else
         {
            if( !m_bTheConsoleAcceptsTyping || ignore_console )
            {
               if( wparam == VK_RETURN )
               {
                  if( m_Players.size() )
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
                        if( IamServer() )
                        {
                           m_Mutex.Lock();
                           NetorkCommandsQueue.push_back( COMMAND_YOU_WIN );
                           m_Mutex.Release();
                        }
                     }
                     else
                     {
                        InitialiseNewOnlineRound();
                        m_GameMode   = GAME_IN_PROGRESS;
                        m_CurrentMap = m_Level;
                        NetorkCommandsQueue.push_back( COMMAND_NEW_ROUND );
                     }
                  }
               }
            }
         }
      } break;
      default:
      {
      };
   };
}
