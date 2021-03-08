#ifndef __C_AsyncSockStream_H
#define __C_AsyncSockStream_H

#include <winsock2.h>
#include <vector>
#include <list>

namespace TCPASYNCSocket
{
/*
   Spec:
   The socket these classes send and recv on must already exist
   Should be able to detect errors in sending and resend the data
   Should be able to handle sending to multiple clients by having multiple instances of the recv class
*/

struct packet
{
   enum
   {
      PACKET_ENUM_ECHO = 0,
      PACKET_ENUM_PING,
   };

   packet( void ){}
   unsigned char   ID[0];
};

class C_AsyncSockStream
{
   public:
      C_AsyncSockStream() : m_Socket( 0 ) {}
      ~C_AsyncSockStream(){}

      void SetSocket( SOCKET Socket )
      {
         m_Socket = Socket;
      }

      void clear( void );
      unsigned long size( void );

   protected:
      typedef std::list< std::vector<char> > Buffer;
      typedef Buffer::iterator               BufferIterator;
      std::list< std::vector<char> > m_DataList;
      std::list< std::vector<char> > m_PacketsSent;
      SOCKET                         m_Socket;

   private:
};

};
#endif

