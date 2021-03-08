#include "main.h"

#include <Wininet.h>

bool MainWindow::EraseGameFromMasterServer( void )
{
   if( !m_MasterServerHost.size() )
      return false;
   if( m_bGameSavedOnServer )
   {
      short buffer_length = 256;
	  std::vector<TCHAR> error_msg_buffer( buffer_length );
      DWORD error_id      = 0;
      ::ZeroMemory( &error_msg_buffer[0], buffer_length );

      m_Mutex.Lock();
      HINTERNET hInternet = ::InternetOpen( m_WinBase.GetTitle().c_str(), INTERNET_OPEN_TYPE_PRECONFIG,
                                            NULL, NULL, 0 ); // INTERNET_FLAG_ASYNC
      m_Mutex.Release();

      if( !hInternet )
      {
         ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
         m_Mutex.Lock();
         m_Console.AddLine( "Call to InternetOpen Failed:" );
         m_Console.AddLine( &error_msg_buffer[0] );
         m_Mutex.Release();
         return false;
      }

      std::string GetGamesURL   = m_MasterServerPath;
      std::string SaveGamesURL  = m_MasterServerPath;
      std::ostringstream    temp;
      SaveGamesURL          += "erasegame.php?";
      SaveGamesURL          += "game=doteater";
      temp << m_Port;
      SaveGamesURL          += "&port=" + temp.str();
      SaveGamesURL          += "&nick=" + m_Nick;
      temp.str(std::string());
      temp << GetFileVersionValue( NULL, "ProductVersion" );
      SaveGamesURL          += "&version=" + temp.str();

      HINTERNET
      hConnection = ::InternetConnect( hInternet, m_MasterServerHost.c_str(), 80, 
                                     " "," ", INTERNET_SERVICE_HTTP, 0, 0 );

      if ( !hConnection )
      {
         ::InternetCloseHandle( hInternet );
         ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
         m_Mutex.Lock();
         m_Console.AddLine( "Call to InternetConnect Failed:" );
         m_Console.AddLine( &error_msg_buffer[0] );
         m_Mutex.Release();
         return false;
      }

      // Get data
      HINTERNET 
      hData = ::HttpOpenRequest( hConnection, "GET", SaveGamesURL.c_str(),
                                 NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0 );

      if ( !hData )
      {
         ::InternetCloseHandle( hConnection);
         ::InternetCloseHandle( hInternet );
         ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
         m_Mutex.Lock();
         m_Console.AddLine( "Call to HttpOpenRequest Failed:" );
         m_Console.AddLine( &error_msg_buffer[0] );
         m_Mutex.Release();
         return false;
      }

      ::HttpSendRequest( hData, NULL, 0, NULL, 0 );

      CHAR buffer[2048];
      DWORD dwRead;
      std::string Contents;

      while( ::InternetReadFile( hData, buffer, 255, &dwRead ) )
      {
         if ( dwRead == 0 )
         {
            if( !Contents.size() )
            { 
               m_Mutex.Lock();
               m_Console.AddLine( "No responce from Master Server" );
               m_Mutex.Release();
            }
            else
            {
               m_Mutex.Lock();
               m_Console.AddLine( "Game Has Been Erased from the Master Server List" );
               if( Contents.find_first_of( "\r" ) != std::string::npos )
               {
                  Contents.erase( Contents.find_first_of( "\r" ), 1 );
               }
               if( Contents.find_first_of( "\n" ) != std::string::npos )
               {
                  Contents.erase( Contents.find_first_of( "\n" ), 1 );
               }
               m_Console.AddLine( "Reply from Master Server: " + Contents );
               m_Mutex.Release();
            }
            break;
         }
         buffer[dwRead] = 0;
         Contents += buffer;
      }

      ::InternetCloseHandle( hConnection );
      ::InternetCloseHandle( hInternet );
      ::InternetCloseHandle( hData );

      m_bGameSavedOnServer = false;
      return true;
   }
   return false;
}

bool MainWindow::SaveGameOnMasterServer( void )
{
   if( !m_MasterServerHost.size() )
      return false;
   short buffer_length = 256;
   std::vector<TCHAR> error_msg_buffer( buffer_length );
   DWORD error_id      = 0;
   ::ZeroMemory( &error_msg_buffer[0], buffer_length );

   HINTERNET hInternet = ::InternetOpen( m_WinBase.GetTitle().c_str(), INTERNET_OPEN_TYPE_PRECONFIG,
                                         NULL, NULL, 0 ); // INTERNET_FLAG_ASYNC

   if( !hInternet )
   {
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_Console.AddLine( "Call to InternetOpen Failed:" );
      m_Console.AddLine( &error_msg_buffer[0] );
      return false;
   }


   std::string GetGamesURL   = m_MasterServerPath;
   std::string SaveGamesURL  = m_MasterServerPath;
   std::ostringstream    temp;
   SaveGamesURL          += "savegame.php?";
   SaveGamesURL          += "game=doteater";
   temp << m_Port;
   SaveGamesURL          += "&port=" + temp.str();
   SaveGamesURL          += "&nick=" + m_Nick;
   temp.str(std::string());
   temp << GetFileVersionValue( NULL, "ProductVersion" );
   SaveGamesURL          += "&version=" + temp.str();

   HINTERNET
   hConnection = ::InternetConnect( hInternet, m_MasterServerHost.c_str(), 80, 
                                  " "," ", INTERNET_SERVICE_HTTP, 0, 0 );

   if ( !hConnection )
   {
      ::InternetCloseHandle( hInternet );
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_Console.AddLine( "Call to InternetConnect Failed:" );
      m_Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   // Get data
   HINTERNET 
   hData = ::HttpOpenRequest( hConnection, "GET", SaveGamesURL.c_str(),
                              NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0 );

   if ( !hData )
   {
      ::InternetCloseHandle( hConnection);
      ::InternetCloseHandle( hInternet );
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_Console.AddLine( "Call to HttpOpenRequest Failed:" );
      m_Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   ::HttpSendRequest( hData, NULL, 0, NULL, 0 );

   CHAR buffer[2048];
   DWORD dwRead;
   std::string Contents;

   while( ::InternetReadFile( hData, buffer, 255, &dwRead ) )
   {
      if ( dwRead == 0 )
      {
//         Alert( Contents );
         if( !Contents.size() )
         { 
            m_Console.AddLine( "No responce from Master Server" );
         }
         else
         {
            m_Console.AddLine( "Game Has Been Added to the Master Server List Of Games" );
            if( Contents.find_first_of( "\r" ) != std::string::npos )
            {
               Contents.erase( Contents.find_first_of( "\r" ), 1 );
            }
            if( Contents.find_first_of( "\n" ) != std::string::npos )
            {
               Contents.erase( Contents.find_first_of( "\n" ), 1 );
            }
            m_Console.AddLine( "Reply from Master Server: " + Contents );
         }
         break;
      }
      buffer[dwRead] = 0;
      Contents += buffer;
   }

   ::InternetCloseHandle( hConnection );
   ::InternetCloseHandle( hInternet );
   ::InternetCloseHandle( hData );
   m_bGameSavedOnServer = true;
   return true;
}

bool MainWindow::GetGamesFromMasterServer( void )
{
   if( !m_MasterServerHost.size() )
      return false;
   short buffer_length = 256;
   std::vector<TCHAR> error_msg_buffer( buffer_length+1 );
   DWORD error_id = 0;
   CHAR  buffer[2048];
   DWORD dwRead;

   ::ZeroMemory( &error_msg_buffer[0], buffer_length );
   HINTERNET hInternet = ::InternetOpen( m_WinBase.GetTitle().c_str(), INTERNET_OPEN_TYPE_PRECONFIG,
                                         NULL, NULL, 0 ); // INTERNET_FLAG_ASYNC

   if( !hInternet )
   {
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_Console.AddLine( "Call to InternetOpen Failed:" );
      m_Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   HINTERNET
   hConnection = ::InternetConnect( hInternet, m_MasterServerHost.c_str(), 80, 
                                    " "," ", INTERNET_SERVICE_HTTP, 0, 0 );

   if ( !hConnection )
   {
      ::InternetCloseHandle( hInternet );
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_Console.AddLine( "Call to InternetConnect Failed:" );
      m_Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   std::string GetGamesURL   = m_MasterServerPath;
   GetGamesURL           += "getgames.php";

   HINTERNET
   hData = ::HttpOpenRequest( hConnection, "GET", GetGamesURL.c_str(),
                              NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0 );

   if ( !hData )
   {
      ::InternetCloseHandle( hConnection);
      ::InternetCloseHandle( hInternet );
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_Console.AddLine( "Call to HttpOpenRequest Failed:" );
      m_Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   if( !::HttpSendRequest( hData, NULL, 0, NULL, 0 ) )
   {
      ::InternetCloseHandle( hConnection );
      ::InternetCloseHandle( hInternet );
      ::InternetCloseHandle( hData );
//      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_Console.AddLine( "Call to HttpSendRequest Failed:" );
//      m_Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   ::ZeroMemory( buffer, 2048 );
   std::string Games;
   while( ::InternetReadFile( hData, buffer, 255, &dwRead ) )
   {
      if ( dwRead == 0 )
      {
         if( !Games.length() )
         {
            m_Console.AddLine( "No responce from Master Server" );
            m_Console.AddLine( "Your Internet Connection may presently be in offline mode" );
//            ::InternetCloseHandle( hConnection );
//            ::InternetCloseHandle( hInternet );
//            ::InternetCloseHandle( hData );
         }  break;
      }
      buffer[dwRead] = 0;
      Games += buffer;
      if( Games.size() > 2048 )
         break;
   }

   ::InternetCloseHandle( hConnection );
   ::InternetCloseHandle( hInternet );
   ::InternetCloseHandle( hData );

   if( ParseGamesFromMasterServer( Games ) )
   {
      return true;
   }
//   std::ifstream games_buffer( "gamesbuffer.txt", std::ios::out );
//   if( !games_buffer.is_open() )
//   {
//      m_Console.AddLine( GetLastSystemError() );
//   }
//   else
//   {
//      games_buffer << buffer;
//   }
//   games_buffer.close();
   return false;
}


bool MainWindow::ParseGamesFromMasterServer( std::string& Games )
{
   if( Games.size() <= 0 )
      return false;


   std::list< std::string > NickIPAndPort;
   std::string temp;
   std::string version;

   // Parses a string like this
   // game=doteater+version=0.4.5.3+nick=andreboyce+IP=127.0.0.1+port=7654
   while( (Games.find( "game=doteater+" ) == 0) )
   {
      temp = "game=doteater+";
      if( Games.find( temp ) == 0 )
      {
         Games.erase( 0, temp.size() );
      }

      temp  = "version=";

      if( Games.find( temp ) == 0 )
      {
         Games.erase( 0, temp.size() );
         version = Games.substr( 0, Games.find_first_of( "+" ) );
         NickIPAndPort.push_back( version );
         Games.erase( 0, Games.find_first_of( "+" )+1 );
      }

      temp = "nick=";

      if( Games.find( temp ) == 0 )
      {
         Games.erase( 0, temp.size() );
         NickIPAndPort.push_back( Games.substr( 0, Games.find_first_of( "+" ) ) );
         Games.erase( 0, Games.find_first_of( "+" )+1 );
      }

      temp = "IP=";

      if( Games.find( temp ) == 0 )
      {
         Games.erase( 0, temp.size() );
         NickIPAndPort.push_back( Games.substr( 0, Games.find_first_of( "+" ) ) );
         Games.erase( 0, Games.find_first_of( "+" )+1 );
      }

      temp = "port=";

      if( Games.find( temp ) == 0 )
      {
         Games.erase( 0, temp.size() );
         NickIPAndPort.push_back( Games.substr( 0, Games.find_first_of( "+" ) ) );
         Games.erase( 0, Games.find_first_of( "+" )+1 );
      }

      Games.erase( 0, 1 );
      Games.erase( 0, 1 );
      NickIPAndPort.push_back( "-" );
      if( GetFileVersionValue( NULL, "ProductVersion" ) == version ) // force the same versions
      {
         m_GamesListBox.AddRow( NickIPAndPort );
      }
      NickIPAndPort.clear();
   }
   return true;

/*
   std::string IP;
   std::string Port;
   std::list< std::string > row = m_GamesListBox.GetSelectedIndex();
   //    0        1      2     3     4
   //  version   nick   ip   port   ping
   int index = 0;
   for( std::list< std::string >::iterator i = row.begin();
       i != row.end();
       i++, index++ )
   {
      if( index == 2 ) // IP
      {
         IP = *i;
         m_Console.SetCurrentLine( *i );
         BuildP1ConsoleList();
      }
      else if( index == 3 ) // port
      {
         Port = (*i);
      }
   }

   if( ValidateIP( IP ) )
   {
      m_RemoteHostAddress = IP;
      m_Port              = atoi( Port.c_str() );
      return true;
   }
   else
   {
      m_Console.AddLine( "Invalid Reply from Master Server:" );
   }
*/
   return false;
}
