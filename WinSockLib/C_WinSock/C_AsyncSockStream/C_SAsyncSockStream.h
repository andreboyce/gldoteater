#ifndef __C_SAsyncSockStream_H
#define __C_SAsyncSockStream_H

#include "C_AsyncSockStream.h"

namespace TCPASYNCSocket
{

class C_SAsyncSockStream: public C_AsyncSockStream
{
   public:
      C_SAsyncSockStream(){}
      ~C_SAsyncSockStream(){}

      unsigned long size( void );
      int  send( void );
      int  send( SOCKET s, int flags = 0 );
      bool AddPacket( char* sdata, unsigned char sizeofsdata, unsigned char packet_id );

   protected:

      unsigned long m_Bytes_Sent;

   private:
};

};

#endif

