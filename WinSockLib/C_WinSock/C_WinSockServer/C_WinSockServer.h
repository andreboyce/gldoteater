#ifndef __C_WinSockServer_H
#define __C_WinSockServer_H

#include "../C_WinSock.h"
#include "../C_Socket/C_Socket.h"

class C_WinSockServer
{
   public:
      C_WinSockServer( void ) :
         m_Hwnd( NULL )
      {
         m_Socket.open();
      }

      ~C_WinSockServer( void )
      {
      }

//      int setsockopt( short index, int level, int optname, char* optval, int optlen )
//      {
//         return setsockopt( m_ClientSockets.at( index ), level, optname, optval, optlen );
//      }

      int setsockopt( int level, int optname, char* optval, int optlen )
      {
         return ::setsockopt( m_Socket, level, optname, optval, optlen );
      }

      int SetWindowSize( int window_size )
      {
         return m_Socket.SetWindowSize( window_size );
      }

      int send( int clientSocket, void* data, unsigned long sizeofdata, int flags = 0 )
      {
         return ::send( clientSocket, (char*)data, sizeofdata, flags );
      }

      int recv( int clientSocket, void* data, unsigned long sizeofdata, int flags = 0 )
      {
         return ::recv( clientSocket, (char*)data, sizeofdata, flags );
      }

      int recvall( int clientSocket, void *data, unsigned long orignalsizeofdata, int flags = 0 )
      {
         return m_Socket.recvall( clientSocket, (char*)data, orignalsizeofdata, flags );
      }

      int sendall( int clientSocket, void *data, unsigned long orignalsizeofdata, int flags = 0 )
      {
         return m_Socket.sendall( clientSocket, (char*)data, orignalsizeofdata, flags );
      }

      std::string GetLastWinSockError( void )
      {
         return m_WinSock.GetLastWinSockError();
      }

      std::string GetLastWinSockError( int errorcode )
      {
         return m_WinSock.GetLastWinSockError( errorcode );
      }

      int listen( unsigned short Port, int backlog = 1 )
      {
         if( m_Socket.bind( Port ) == SOCKET_ERROR )
            return SOCKET_ERROR;
         return m_Socket.listen( backlog );
      }

      int accept( WPARAM wParam )
      {
         int ret = ::accept( wParam, NULL, NULL );
         m_ClientSockets.push_back( ret );
         return ret;
      }

      int Async( HWND Hwnd, unsigned int wMsg, long lEvent = (FD_WRITE | FD_ACCEPT | FD_READ | FD_CLOSE) )
      {
         m_Hwnd = Hwnd;
         return ::WSAAsyncSelect( m_Socket, m_Hwnd, wMsg, lEvent );
      }

      int Async( unsigned int wMsg, long lEvent = (FD_WRITE | FD_ACCEPT | FD_READ | FD_CLOSE) )
      {
         return ::WSAAsyncSelect( m_Socket, m_Hwnd, wMsg, lEvent );
      }

      void SetHwnd( HWND Hwnd )
      {
         if( !m_Hwnd )
            m_Hwnd = Hwnd;
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

      std::string GetHostName( void ) const
      {
         return m_HostName;
      }

      unsigned short GetNumberofHostIpAddress( void )
      {
         return m_Ipaddresses.size();
      }

      std::vector<std::string> GetHostIpAddresses( void )
      {
         return m_Ipaddresses;
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

      void close( void )
      {
         m_Socket.close();
         for( int index=0; index<m_ClientSockets.size() ; index++ )
           ::closesocket( m_ClientSockets.at( index ) );
         m_ClientSockets.clear();
      }

      int open( int af = AF_INET, int type = SOCK_STREAM, int protocol = 0 )
      {
         return m_Socket.open( af, type, protocol );
      }

      SOCKET GetServerSocket( void )
      {
         return m_Socket;
      }

      SOCKET GetSocket( unsigned short index )
      {
         return m_ClientSockets.at( index );
      }

   protected:
      C_Socket                 m_Socket;
      std::vector<SOCKET>      m_ClientSockets;
      C_WinSock                m_WinSock;
      std::vector<std::string> m_Ipaddresses;
      std::string              m_HostName;
      HWND                     m_Hwnd;

   private:
};

#endif
