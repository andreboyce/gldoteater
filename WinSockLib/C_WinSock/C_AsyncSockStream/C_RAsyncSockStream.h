#ifndef __C_RAsyncSockStream_H
#define __C_RAsyncSockStream_H

#include "C_AsyncSockStream.h"

namespace TCPASYNCSocket
{

//extern int RecvData( );

class C_RAsyncSockStream: public C_AsyncSockStream
{
   public:
      C_RAsyncSockStream() : m_bPacketRecv( true ),
                             m_bPacketSizeRecv( false )
      {}
      ~C_RAsyncSockStream(){}

      int recv( void );
      int recv( SOCKET s, int flags = 0 );

      int GetPacketSize( void );
      int GetPacketData( void* data );
      int GetPacketID( void );
      bool packetrecv( void );

   protected:
      unsigned long m_Bytes_Recv;
      bool          m_bPacketRecv,
                    m_bPacketSizeRecv;
      void AddData( char* data_recv, int sizeof_data_recv );

   private:
};

};
#endif

