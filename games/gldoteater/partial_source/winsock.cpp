#include "main.h"

//#include <Winsock2.h>

// Winsock must be initialised first duh
void MainWindow::EnumerateLocalAddresses( std::deque<std::string>& Ipaddresses )
{
   Ipaddresses.clear();
   char ac[256];

   if( gethostname( ac, sizeof( ac ) ) == SOCKET_ERROR )
   {
//        cerr << "Error " << WSAGetLastError() << " when getting local host name." << endl;
      return;
   }
 
   struct hostent *phe = gethostbyname( ac );

   if( phe == 0 )
   {
//        cerr << "Bad host lookup." << endl;
       return;
   }
 
   for(int i = 0; phe->h_addr_list[i] != 0 ; ++i )
   {
      struct in_addr addr;
      memcpy( &addr, phe->h_addr_list[i], sizeof( struct in_addr ) );
      Ipaddresses.push_back( std::string(inet_ntoa( addr )) );
   }
}
