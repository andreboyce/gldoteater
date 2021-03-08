#include "C_RAsyncSockStream.h"

namespace TCPASYNCSocket
{

int C_RAsyncSockStream::recv( void )
{
   int  flags         = 0;
   int  max_recv_size = 1024;
   std::vector<char> temp( max_recv_size );
   int bytes_recv = ::recv( m_Socket, &temp[0], max_recv_size, flags );
   if( bytes_recv != SOCKET_ERROR )
   {
      std::vector<char> data;
      m_Bytes_Recv += bytes_recv;
      AddData( &temp[0], bytes_recv );
   }
   return bytes_recv;
}

int C_RAsyncSockStream::recv( SOCKET s, int flags )
{
   int  max_recv_size = 1024;
   std::vector<char> temp( max_recv_size );
   int bytes_recv = ::recv( s, &temp[0], max_recv_size, flags );
   if( bytes_recv != SOCKET_ERROR )
   {
      std::vector<char> data;
      m_Bytes_Recv += bytes_recv;
      AddData( &temp[0], bytes_recv );
   }
   return bytes_recv;
}

void C_RAsyncSockStream::AddData( char* data_recv, int sizeof_data_recv )
{
   // Guarentee that there is data to be read
   // data_recv[0] does not give incorrect data
   if( sizeof_data_recv == 0 )
      return;

   if( m_bPacketRecv ) // The start of a new packet is being received
   {
      int packet_id = data_recv[0];
      std::vector<char> data;

      switch( packet_id )
      {
         case packet::PACKET_ENUM_PING:
         {
            data.push_back( packet_id );
            m_DataList.push_back( data );
            if( sizeof_data_recv > 1 )
            {
               AddData( (&data_recv[1]), sizeof_data_recv-1 );
            }
         } break;
         case packet::PACKET_ENUM_ECHO:
         {
            data.push_back( packet_id );
            m_DataList.push_back( data );
            if( sizeof_data_recv > 1 )
            {
               AddData( (&data_recv[1]), sizeof_data_recv-1 );
            }
         } break;
         default:
         {
            data.push_back( data_recv[0] );

            // Guarentee that there is a size byte
            // data_recv[1] 
            if( sizeof_data_recv >= 2 )
            {
               int size = (int)(data_recv[1]);
               m_bPacketSizeRecv = true; // signal size recived
               data.push_back( data_recv[1] );

               // Guarentee that entire packet was obtained
               // data_recv[2] to data_recv[size+2]
               if( (sizeof_data_recv-2) >= size )
               {
                  data.resize( size+2 ); // +2 for the id and size bytes
                  std::memcpy( (&data[2]), (&data_recv[2]), size );
                  m_DataList.push_back( data );
                  m_bPacketRecv = true; // signal the start of a new packet

                  // Recursively add a new packet
                  AddData( (&data_recv[2]), sizeof_data_recv-size-2 );
               }
               // The entire packet was not obtained
               // But get what was obtained 
               else
               {
                  // signal the packet was not entirely obtained
                  m_bPacketRecv = false;
                  int sizeof_data_obtained = sizeof_data_recv-2; // -2 for the id and size bytes
                  // If sizeof_data_obtained <= 0 no data was botained so return
                  if( sizeof_data_obtained <= 0 )
                  {
                     return;
                  }
                  // make room for the data obtained + the id and size bytes we already have
                  data.resize( sizeof_data_obtained+2 );
                  std::memcpy( (&data[2]), (&data_recv[2]), sizeof_data_obtained );
                  m_DataList.push_back( data );
               }
            }
            // No size byte was botained
            else
            {
               // signal the size of the packet was not obtained
               m_bPacketSizeRecv = false;
            }
         }
      }
   }
   // Appending data to a partially received packet
   else
   {
      std::vector<char> data;

      // Guarentee that the size was received
      if( m_bPacketSizeRecv )
      {
         std::list< std::vector<char> >::reference lastpacket = m_DataList.back();
         int sizeof_data_previously_obtained = lastpacket.size()-2; // -2 for the id and size bytes
         // Here data/packet refers to the the data segment of the packet
         int sizeof_packet                   = (int)lastpacket[1];
         int sizeof_data_remaining           = sizeof_packet-sizeof_data_previously_obtained;

         // Only up to the size byte was obtained
         if( sizeof_data_previously_obtained <= 0 )
         {
            // All of the remaining data was obtained
            if( sizeof_data_remaining >= sizeof_data_recv )
            {
               std::memcpy( (&lastpacket[2]), (&data_recv[0]), sizeof_data_remaining );
               m_bPacketRecv = false;
               AddData( (&data_recv[sizeof_packet+2]), sizeof_data_recv-sizeof_data_remaining );
            }
            // Get additional data obtained
            else
            {
               std::memcpy( (&lastpacket[2]), (&data_recv[0]), sizeof_data_recv );
               m_bPacketRecv = false;
            }
         }
         // More bytes were obtained than the id and size bytes but the packet
         // is not complete
         else
         {
            // All of the remaining data was obtained
            if( sizeof_data_remaining >= sizeof_data_recv )
            {
               std::memcpy( (&lastpacket[sizeof_data_previously_obtained+2]), (&data_recv[0]), sizeof_data_remaining );
               m_bPacketRecv = false;
               AddData( (&data_recv[sizeof_packet]), sizeof_data_recv-sizeof_data_remaining );
            }
            // Get additional data obtained
            else
            {
               std::memcpy( (&lastpacket[2]), (&data_recv[0]), sizeof_data_recv );
               m_bPacketRecv = false;
            }
         }
      }
      else
      {
         // get size and concatenate data
         std::list< std::vector<char> >::reference lastpacket = m_DataList.back();
         std::memcpy( (&lastpacket[1]), (&data_recv[0]), sizeof(char) );
         m_bPacketSizeRecv = true;
         if( sizeof_data_recv >= 2 )
            AddData( (&data_recv[1]), sizeof_data_recv-1 );
      }
   }
}

int C_RAsyncSockStream::GetPacketID( void )
{
   int packet_id = -1;
   std::list< std::vector<char> >::reference lastpacket = m_DataList.back();
   if( lastpacket.size() )
      packet_id = lastpacket[0];
   return packet_id;
}

int C_RAsyncSockStream::GetPacketSize( void )
{
   int packet_size = 0;
   std::list< std::vector<char> >::reference lastpacket = m_DataList.back();
   if( lastpacket.size() >= 2 )
      packet_size = lastpacket[1];
   return packet_size;
}

int C_RAsyncSockStream::GetPacketData( void* data )
{
   std::list< std::vector<char> >::reference lastpacket = m_DataList.back();
   int sizeof_data = lastpacket.size()-2;
   std::memcpy( data, (&lastpacket[2]), sizeof_data );
   return 0;
}

bool C_RAsyncSockStream::packetrecv( void )
{
   bool ret = true;
   return ret;
}

};

