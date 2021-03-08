#include "C_SAsyncSockStream.h"

namespace TCPASYNCSocket
{

bool C_SAsyncSockStream::AddPacket( char* sdata, unsigned char sizeofsdata, unsigned char packet_id )
{
   std::vector<char> data;
   switch( packet_id )
   {
      case packet::PACKET_ENUM_PING:
      {
         data.push_back( packet_id );
         m_DataList.push_back( data );
      } break;
      case packet::PACKET_ENUM_ECHO:
      {
         data.push_back( packet_id );
         m_DataList.push_back( data );
      } break;
      default:
      {
         unsigned char PacketSize = sizeof(packet) + sizeofsdata + sizeof(char);
         data.resize( PacketSize );
         data[0] = packet_id;
         data[1] = PacketSize;
         std::memcpy( (&data[2]), sdata, sizeofsdata );
         m_DataList.push_back( data );
         //  0    1    2 ...
         // ID SIZE DATA
      }
   }
   return true;
}

int C_SAsyncSockStream::send( void )
{
   int bytes_sent = 0;
   int flags      = 0;
   if( m_DataList.size() )
   {
      std::vector<char> data = m_DataList.back();
      bytes_sent = ::send( m_Socket, &data.front(), data.size(), flags );

      if( bytes_sent != SOCKET_ERROR )
      {
         data.erase( data.begin(), 
                   ( data.end() < (data.begin()+bytes_sent) ) ? 
                     data.end() : (data.begin()+bytes_sent) );
         // add the back up data for the resending part here
         if( data.size() == 0 )
         {
            m_DataList.pop_back();
         }
      }
      if( bytes_sent != SOCKET_ERROR )
      {
         m_Bytes_Sent += bytes_sent;
      }
   }
   return bytes_sent;
}

int C_SAsyncSockStream::send( SOCKET s, int flags )
{
   int bytes_sent = 0;
   if( m_DataList.size() )
   {
      std::vector<char> data = m_DataList.back();
      bytes_sent = ::send( s, &data.front(), data.size(), flags );

      if( bytes_sent != SOCKET_ERROR )
      {
         data.erase( data.begin(), 
                   ( data.end() < (data.begin()+bytes_sent) ) ? 
                     data.end() : (data.begin()+bytes_sent) );
         // add the back up data for the resending part here
         if( data.size() == 0 )
         {
            m_DataList.pop_back();
         }
      }
      if( bytes_sent != SOCKET_ERROR )
      {
         m_Bytes_Sent += bytes_sent;
      }
   }
   return bytes_sent;
}

unsigned long C_SAsyncSockStream::size( void )
{
   unsigned long size = 0;
   for( std::list< std::vector<char> >::iterator packet_iterator = m_DataList.begin(); packet_iterator != m_DataList.end() ; packet_iterator++ )
      size += packet_iterator->size();
   return size;
}

};
