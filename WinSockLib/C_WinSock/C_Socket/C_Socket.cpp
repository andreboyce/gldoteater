#include "C_Socket.h"

int C_Socket::sendall( int clientSocket, char *data, unsigned long orignalsizeofdata, int flags )
{
  unsigned long sizeofdataremaining = orignalsizeofdata;
  int           bytes_sent          = 0;
  unsigned long total_bytes_sent    = 0;

  while( total_bytes_sent < orignalsizeofdata ) 
  { 
     bytes_sent = ::send( clientSocket, data+total_bytes_sent, sizeofdataremaining, flags ); //pointer Arithmetic dont be scared
     if( bytes_sent == SOCKET_ERROR )
     {
//        ReportError( "send() failed" );
        return total_bytes_sent;
     }
     total_bytes_sent    += bytes_sent;
     sizeofdataremaining  = (orignalsizeofdata-total_bytes_sent);
  } 
  return total_bytes_sent;
} 

int C_Socket::recvall( int clientSocket, char *data, unsigned long orignalsizeofdata, int flags )
{
  int           bytes_recv           = 0;
  int           total_bytes_recv     = 0;
  unsigned long sizeofdataremaining  = orignalsizeofdata;

  while( total_bytes_recv < orignalsizeofdata )
  {
     bytes_recv = ::recv( clientSocket, data+total_bytes_recv, sizeofdataremaining, flags ); // data+total_bytes_recv pointer Arithmetic dont be scared
     if( bytes_recv == SOCKET_ERROR )
     {
//        ReportError( "recv() failed");
        return total_bytes_recv;
     }
     total_bytes_recv    += bytes_recv;
     sizeofdataremaining  = (orignalsizeofdata-total_bytes_recv);
  } 
  return total_bytes_recv;
} 

