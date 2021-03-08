#ifndef __PongNetwork_H
#define __PongNetwork_H

#include <vector>
#include "../../WinSockLib/WinSockLib.h"
#include "../../errorutil/errorutil.h"

struct packet
{
   typedef unsigned char  byte;
   typedef unsigned short size_t;
   enum
   {
      PACKET_FLAG_ECHO = 0x1,
      PACKET_FLAG_PADDLE_POSITION, // Padle positions
      PACKET_FLAG_BALL_POSITION,   // ball  positions
      PACKET_FLAG_STATUS,          // score round etc ...

      PACKET_FLAG_SERVER_SERVES, // client serves
      PACKET_FLAG_CLIENT_SERVES, // server serves

      PACKET_FLAG_TOGGLE_SERVES, // toggle serves

      PACKET_FLAG_GAME_SPEED,  // game speed

      PACKET_FLAG_CHATTING_ON,  // let the player know when the other is typing
      PACKET_FLAG_CHATTING_OFF, // let the player know when the other is not typing

      PACKET_FLAG_TEXT,        // Text
      PACKET_FLAG_NIC,         // Nick change
      PACKET_FLAG_VERSION,     // client version
      PACKET_FLAG_COMMAND,     // command
      PACKET_FLAG_IP,          // Ip
      PACKET_FLAG_SPING,       // send ping
      PACKET_FLAG_RPING,       // reply to ping
      PACKET_FLAG_CURRENTPING, // Added this so that only the server pings the client, and then sends the ping
      PACKET_FLAG_REJECT,      // Connection was refused
   };

   packet( void ) : flags( PACKET_FLAG_ECHO ), size(0){}
   size_t size;
   char   flags;
   char   data[0];
};

class C_RAsyncSockStream
{
   public:
      C_RAsyncSockStream();
      ~C_RAsyncSockStream(){}
      void Clear( void );

      packet::size_t GetPacketSize( void ) const;
      bool PacketReceived( void );
      int recv( SOCKET s, int flags = 0 );
      void GetDataFromPacket( char * Data );

      std::size_t Buffer_Size( void ) const
      {
         return m_Data.size();
      }

      unsigned long Bytes_Recv( void ) const
      {
         return m_Bytes_Recv;
      }

      unsigned long GetDataSize( void ) const
      {
         return m_Data.size();
      }

      operator bool ( void )
      {
         return PacketReceived();
      }

   protected:
      void AddPacket( char* sdata, unsigned char sizeofsdata, unsigned char packet_flags );
      void AddData( char * Data, unsigned long sizeofdata );

      unsigned long GetMaxPacketSize( void ) const
      {
         return m_MaxPacketSize;
      }

      void GetData( char * Data, unsigned long sizeofdata );

      std::vector<char> m_Data;
      unsigned long     m_MaxPacketSize;
      unsigned long     m_Bytes_Recv;

   private:
};

class C_SAsyncSockStream
{
   public:
      C_SAsyncSockStream();
      ~C_SAsyncSockStream(){}
      void Clear( void );

      packet::size_t GetPacketSize( void ) const;
      bool PacketSent( void );
      int send( SOCKET s, int flags = 0 );
      void AddPacket( char* sdata, unsigned char sizeofsdata, unsigned char packet_flags );

      std::size_t Buffer_Size( void ) const
      {
         return m_Data.size();
      }

      unsigned long Bytes_Sent( void ) const
      {
         return m_Bytes_Sent;
      }

      unsigned long GetDataSize( void ) const
      {
         return m_Data.size();
      }

      operator bool ( void )
      {
         return PacketSent();
      }

   protected:
      void GetDataFromPacket( char * Data );
      void AddData( char * Data, unsigned long sizeofdata );
      void GetData( char * Data, unsigned long sizeofdata );

      unsigned long GetMaxPacketSize( void ) const
      {
         return m_MaxPacketSize;
      }

      std::vector<char> m_Data;
      unsigned long     m_MaxPacketSize;
      unsigned long     m_Bytes_Sent;

   private:
};

#endif
