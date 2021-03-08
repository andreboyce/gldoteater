#include "main.h"

//***************************************************************************
//******************************* MainWindow::GetText ***********************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::GetText( std::string TextFromConsole )
{
   bool bgetinputagain = false;

   switch( m_ConsoleInput )
   {
      case GET_NOTHING:
      {
      } break;
      case GET_PLAYER_NAME:
      {
         m_Mutex.Lock();
         m_Nick = TextFromConsole;
         bgetinputagain = true;
         if( !m_Nick.size() )
         {
            m_ConsoleInput = GET_PLAYER_NAME;
            GetPlayerName();
         }
         else
         {
            if( m_GameMode == LISTEN_MODE )
            {
               SaveGameOnMasterServer();
               StartServerThread();
               m_ConsoleInput = GET_NOTHING;
            }
            else if( m_GameMode == GET_SERVER_INFORMATION_FROM_USER )
            {
               GetGamesFromMasterServer();
               GetServerIP();
            }
         }
         m_Mutex.Release();
      } break;
      case GET_PLAYER1_NAME:
      {
         m_Mutex.Lock();
         if( m_PlayerNames.size() )
         {
            m_PlayerNames[0] = TextFromConsole;
         }
         else
         {
            m_PlayerNames.resize( 1 );
            m_PlayerNames[0] = TextFromConsole;
         }
         if( m_GameMode == YOU_WIN_MODE || m_GameMode == GAME_OVER )
         {
            if( m_Players.size() > 0 )
               AddHighScore( TextFromConsole, m_Players[0].m_Score );
            m_GameMode = VIEW_HIGH_SCORES;
         }
         m_Mutex.Release();
      } break;
      case GET_PLAYER2_NAME:
      {
         m_Mutex.Lock();
         if( m_PlayerNames.size() >= 2 )
         {
            m_PlayerNames[1] = TextFromConsole;
         }
         else
         {
            m_PlayerNames.resize( 2 );
            m_PlayerNames[1] = TextFromConsole;
         }
         if( m_GameMode == YOU_WIN_MODE || m_GameMode == GAME_OVER )
         {
            if( m_Players.size() > 1 )
               AddHighScore( TextFromConsole, m_Players[1].m_Score );
            m_GameMode = VIEW_HIGH_SCORES;
         }
         m_Mutex.Release();
      } break;
      case GET_SERVER_IP:
      {
         if( ValidateIP( TextFromConsole ) )
         {
            m_RemoteHostAddress = TextFromConsole;
            GetPort();
         }
         else
         {
            m_Mutex.Lock();
            m_Console.AddLine( "Invalid IP Address" );
            m_Mutex.Release();
            GetServerIP();
         }
         bgetinputagain = true;
      } break;
      case GET_SERVER_PORT:
      {
         m_Port             = atoi( TextFromConsole.c_str() );
         m_GameMode         = CONNECT_MODE;
         m_bIgnoreVK_RETURN = true;
         bgetinputagain     = true;
         ShowConsole();
//         m_EnetClientThread.Start();
         StartClientThread();
      } break;
      default:
      {
      }      
   }

   if( !bgetinputagain )
   {
      m_bShowTheConsole          = false;
      m_bTheConsoleAcceptsTyping = false;
      m_ConsoleInput             = GET_NOTHING;
   }
}

//***************************************************************************
//******************************* MainWindow::GetPort ********************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::GetPort()
{
   m_bShowTheConsole          = true;
   m_bTheConsoleAcceptsTyping = true;
   m_Mutex.Lock();
   m_Console.AddLine( "Enter Port" );
   m_Mutex.Release();
   m_ConsoleInput = GET_SERVER_PORT;
   std::ostringstream tmp;
   tmp << m_Port;
   m_Mutex.Lock();
   m_Console.SetCurrentLine( tmp.str() );
   m_Mutex.Release();
}

//***************************************************************************
//******************************* MainWindow::GetServerIP *******************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::GetServerIP()
{
   m_bShowTheConsole          = true;
   m_bTheConsoleAcceptsTyping = true;
   m_Mutex.Lock();
   m_Console.AddLine( "Enter Server IP" );
   m_Mutex.Release();
   m_ConsoleInput = GET_SERVER_IP;

   if( m_RemoteHostAddress.size() )
   {
      m_Console.SetCurrentLine( m_RemoteHostAddress );
   }
   else
   {
      std::string clipboardtext = GetTextFromClipBoard( false );
      if( ValidateIP( clipboardtext ) )
      {
         m_Mutex.Lock();
         m_Console.SetCurrentLine( clipboardtext );
         m_Mutex.Release();
      }
      else
      {
         std::list< std::string > row         = m_GamesListBox.GetSelectedIndex();
         std::list< std::string >::iterator i = row.begin();
         if( i != row.end() )
         {
            i++;
            if( i != row.end() )
            {
               i++;
               //if( i != NULL )
               //{
                  m_Mutex.Lock();
                  m_Console.SetCurrentLine( *i );
                  m_Mutex.Release();
               //}
            }
         }
      }
   }
}


//***************************************************************************
//******************************* MainWindow::GetHighScorePlayer1Name *******
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::GetHighScorePlayer1Name()
{
   m_bShowTheConsole          = true;
   m_bTheConsoleAcceptsTyping = true;
   m_Mutex.Lock();
   m_Console.AddLine( "Player1 Type your Name" );
   m_Mutex.Release();
   m_ConsoleInput = GET_PLAYER1_NAME;
}

//***************************************************************************
//******************************* MainWindow::GetHighScorePlayer2Name *******
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::GetHighScorePlayer2Name()
{
   m_bShowTheConsole          = true;
   m_bTheConsoleAcceptsTyping = true;
   m_Mutex.Lock();
   m_Console.AddLine( "Player1 Type your Name" );
   m_Mutex.Release();
   m_ConsoleInput = GET_PLAYER2_NAME;
}

//***************************************************************************
//******************************* MainWindow::GetPlayerName *****************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::GetPlayerName( bool bReplaceNick )
{
   if( ( m_Nick.size() && bReplaceNick ) || !m_Nick.size() )
   {
      m_bShowTheConsole          = true;
      m_bTheConsoleAcceptsTyping = true;
      m_Mutex.Lock();
      m_Console.AddLine( "Type your Name" );
      m_Mutex.Release();
      m_ConsoleInput = GET_PLAYER_NAME;
   }
   else
   {
      if( m_GameMode == LISTEN_MODE )
      {
         m_bShowTheConsole          = true;
         m_bTheConsoleAcceptsTyping = true;
         SaveGameOnMasterServer();
         StartServerThread();
         m_ConsoleInput = GET_NOTHING;
      }
      else if( m_GameMode == GET_SERVER_INFORMATION_FROM_USER )
      {
         m_bShowTheConsole          = true;
         m_bTheConsoleAcceptsTyping = true;
         GetGamesFromMasterServer();
         GetServerIP();
      }
   }
}

