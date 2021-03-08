#ifndef __C_Socket_H
#define __C_Socket_H

#include <winsock2.h>
#include <string>
#include <vector>

class C_Socket
{
   public:

       C_Socket( int af = AF_INET, int type = SOCK_STREAM, int protocol = 0 ) : 
          m_Socket( ::socket( af, type, protocol ) )
       {
       }
   
      ~C_Socket(){ close(); }

      int setsockopt( int level, int optname, char* optval, int optlen )
      {
         return ::setsockopt( m_Socket, level, optname, optval, optlen );
      }

      int SetWindowSize( int window_size )
      {
         int ret = 0;
         if( ret = ::setsockopt( m_Socket, SOL_SOCKET, SO_SNDBUF, (char *) &window_size, sizeof(window_size) ) )
            return ret;
         ret = ::setsockopt( m_Socket, SOL_SOCKET, SO_RCVBUF, (char *) &window_size, sizeof(window_size) );
         return ret;
      }

      SOCKET open( int af = AF_INET, int type = SOCK_STREAM, int protocol = 0 )
      {
         return m_Socket = ::socket( af, type, protocol );
      }

      void close( void )
      {
         ::closesocket( m_Socket );
         m_Socket = INVALID_SOCKET;
      }

      int shutdown( int flag )
      {
         if( m_Socket != INVALID_SOCKET )
            return ::shutdown( m_Socket, flag );
         return 0;
      }

      int recvall( int clientSocket, char *data, unsigned long orignalsizeofdata, int flags = 0 );
      int sendall( int clientSocket, char *data, unsigned long orignalsizeofdata, int flags = 0 );

      int send( void* data, unsigned long sizeofdata, int flags = 0 )
      {
         return ::send( m_Socket, (char*)data, sizeofdata, flags );
      }

      int recv( void* data, unsigned long sizeofdata, int flags = 0 )
      {
         return ::recv( m_Socket, (char*)data, sizeofdata, flags );
      }

      int listen( unsigned short number_of_clients = 1 )
      {
         if( m_Socket == INVALID_SOCKET )
         {
            if( open() == INVALID_SOCKET )
               return SOCKET_ERROR;
         }

         if( ::listen( m_Socket, number_of_clients ) == SOCKET_ERROR )
         {
           // error!  unable to listen
            return SOCKET_ERROR;
         }
         return 0;
      }

      int accept( unsigned short number_of_clients = 1 )
      {
         if( m_Socket == INVALID_SOCKET )
         {
            return INVALID_SOCKET;
         }

         int      clients   = 0;
         int      addr_size = sizeof(sockaddr);
		 std::vector<SOCKET>   client(number_of_clients);      // socket handles to clients
		 std::vector<sockaddr> client_sock(number_of_clients); // info on client sockets

         while( clients < number_of_clients )     // let MAX_CLIENTS connect
         {
            // accept a connection
            client[clients] = ::accept( m_Socket, &client_sock[clients], &addr_size );
            if( client[clients] == INVALID_SOCKET )
            {
               // error accepting connection
               return SOCKET_ERROR;
            }
            else
            {
                // client connected successfully
                // start a thread that will communicate with client
//                startThread( client[number_of_clients] );
                clients++;
            }
         }
         return 0;
      }

      int bind( unsigned short Port )
      {
         if( m_Socket == INVALID_SOCKET )
         {
            if( open() == INVALID_SOCKET )
               return SOCKET_ERROR;
         }

         sockaddr_in  addr; // the address structure for a TCP socket

         addr.sin_family      = AF_INET;             // Address family Internet
         addr.sin_port        = htons( Port );       // Assign Port
         addr.sin_addr.s_addr = htonl( INADDR_ANY ); // No destination

         if( ::bind( m_Socket, (LPSOCKADDR)&addr, sizeof(addr) ) == SOCKET_ERROR )
         {
            return SOCKET_ERROR;
         }
         return 0;
      }

      int connect( unsigned short Port, std::string hostip )
      {
         if( m_Socket == INVALID_SOCKET )
         {
            return SOCKET_ERROR;
         }

         SOCKADDR_IN target;

         target.sin_family      = AF_INET;                     // address family Internet
         target.sin_port        = htons( Port );               // set server’s port number
         target.sin_addr.s_addr = inet_addr( hostip.c_str() ); // set server’s IP

         if( ::connect( m_Socket, (LPSOCKADDR)&target, sizeof(struct sockaddr) ) == SOCKET_ERROR )
         {
            return SOCKET_ERROR;
         }
         return 0;
      }

      operator SOCKET (void) const
      {
         return m_Socket;
      }

   protected:
      SOCKET m_Socket;

   private:
};

#endif
