#ifndef __C_WinSockClient_H
#define __C_WinSockClient_H

#include "../C_WinSock.h"
#include "../C_Socket/C_Socket.h"

class C_WinSockClient
{
   public:
      C_WinSockClient( void )
      {
         m_Socket.open();
      }

      int setsockopt( int level, int optname, char* optval, int optlen )
      {
         return m_Socket.setsockopt( level, optname, optval, optlen );
      }

      int SetWindowSize( int window_size )
      {
         return m_Socket.SetWindowSize( window_size );
      }

      int recv( void *data, unsigned long orignalsizeofdata, int flags = 0 )
      {
         return m_Socket.recv( (char*)data, orignalsizeofdata, flags );
      }

      int send( void *data, unsigned long orignalsizeofdata, int flags = 0 )
      {
         return m_Socket.send( (char*)data, orignalsizeofdata, flags );
      }

      int recvall( void *data, unsigned long orignalsizeofdata, int flags = 0 )
      {
         return m_Socket.recvall( m_Socket, (char*)data, orignalsizeofdata, flags );
      }

      int sendall( void *data, unsigned long orignalsizeofdata, int flags = 0 )
      {
         return m_Socket.sendall( m_Socket, (char*)data, orignalsizeofdata, flags );
      }

      std::string GetLastWinSockError( void )
      {
         return m_WinSock.GetLastWinSockError();
      }

      std::string GetLastWinSockError( int errorcode )
      {
         return m_WinSock.GetLastWinSockError( errorcode );
      }

      int connect( unsigned short Port, std::string hostip )
      {
         return m_Socket.connect( Port, hostip );
      }

      int shutdown( int flag )
      {
         m_Socket.shutdown( flag );
		 return 0;
      }

      void close( void )
      {
         m_Socket.close();
      }

      int open( int af = AF_INET, int type = SOCK_STREAM, int protocol = 0 )
      {
         return m_Socket.open( af, type, protocol );
      }

      int Async( HWND Hwnd, unsigned int wMsg, long lEvent = (FD_WRITE | FD_CONNECT | FD_READ | FD_CLOSE) )
      {
         return ::WSAAsyncSelect( m_Socket, Hwnd, wMsg, lEvent );
      }

      SOCKET GetSocket( void )
      {
         return m_Socket;
      }

      std::string GetHostIpAddress( unsigned short index = 0 )
      {

         if( !m_Ipaddresses.size() )
         {
            EnumerateLocalAddresses();
         }
         if( index >= m_Ipaddresses.size() )
            return std::string( "null" );
         return m_Ipaddresses.at( index );
      }

      int EnumerateLocalAddresses( void )
      {
         m_Ipaddresses.clear();
         char ac[256];

         if( gethostname( ac, sizeof( ac ) ) == SOCKET_ERROR )
         {
//        cerr << "Error " << WSAGetLastError() << " when getting local host name." << endl;
            return -1;
         }

         m_HostName = ac;
 
         struct hostent *phe = gethostbyname( ac );

         if( phe == 0 )
         {
//        cerr << "Bad host lookup." << endl;
             return -1;
         }
 
         for(int i = 0; phe->h_addr_list[i] != 0 ; ++i )
         {
            struct in_addr addr;
            memcpy( &addr, phe->h_addr_list[i], sizeof( struct in_addr ) );
            m_Ipaddresses.push_back( std::string(inet_ntoa( addr )) );
         }
         return 0;
      }

   protected:
      C_Socket                 m_Socket;
      C_WinSock                m_WinSock;
      std::vector<std::string> m_Ipaddresses;
      std::string              m_HostName;

   private:
};

#endif
