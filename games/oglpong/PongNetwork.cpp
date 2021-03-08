#include "PongNetwork.h"

C_RAsyncSockStream::C_RAsyncSockStream() :
         m_Bytes_Recv( 0 ),
         m_MaxPacketSize( 1024 )
//         m_MaxPacketSize( 0 )
{
   // 2^n-1
//         m_MaxPacketSize = (long)pow( 2, sizeof(unsigned short) ) - 1;
//         m_Data.reserve( m_MaxPacketSize*2 ); // reserve max size of 2 packets
   m_Data.reserve( m_MaxPacketSize*2 );
}

void C_RAsyncSockStream::Clear( void )
{
   m_Data.clear();
}

int C_RAsyncSockStream::recv( SOCKET s, int flags )
{
	std::vector<char> temp( m_MaxPacketSize );
   int bytes_recv = ::recv( s, &temp[0], m_MaxPacketSize, flags );
   if( bytes_recv != SOCKET_ERROR )
   {
      m_Bytes_Recv += bytes_recv;
      AddData( &temp[0], bytes_recv );
   }
   return bytes_recv;
}

void C_RAsyncSockStream::GetDataFromPacket( char * Data )
{
   if( PacketReceived() )
   {
      packet::size_t sizeofdata = GetPacketSize();
//      std::size_t sizeofdata = GetPacketSize();
//      unsigned short sizeofdata = GetPacketSize();
      std::memcpy( Data, &m_Data.front(), ( sizeofdata > m_Data.size() ) ? m_Data.size() : sizeofdata );
      m_Data.erase( m_Data.begin(), ( m_Data.end() < (m_Data.begin()+sizeofdata) ) ? m_Data.end() : (m_Data.begin()+sizeofdata) );
   }
}

packet::size_t
//std::size_t 
//      unsigned short 
C_RAsyncSockStream::GetPacketSize( void ) const
{
   if( m_Data.size() >= sizeof( packet::size_t ) )
//   if( m_Data.size() >= sizeof( std::size_t ) )
//   if( m_Data.size() >= sizeof( unsigned short ) )
   {
      packet::size_t *length = (packet::size_t*)&m_Data.front();
//      std::size_t *length = (std::size_t*)&m_Data.front();
//      unsigned short *length = (unsigned short *)&m_Data.front();
      return *(length);
   }
   return 0;
}

bool C_RAsyncSockStream::PacketReceived( void )
{
   if( m_Data.size() >= sizeof( packet::size_t ) )
//   if( m_Data.size() >= sizeof( std::size_t ) )
//   if( m_Data.size() >= sizeof( unsigned short ) )
   {
      packet::size_t *length = (packet::size_t*)&m_Data.front();
//      std::size_t *length = (std::size_t*)&m_Data.front();
//      unsigned short *length = (unsigned short *)&m_Data.front();
      if( m_Data.size() >= *length )
         return true;
   }
   return false;
}


void C_RAsyncSockStream::AddPacket( char* sdata, unsigned char sizeofsdata, unsigned char packet_flags )
{
   unsigned char PacketSize = sizeof(packet) + sizeofsdata;
   std::vector<char> data( PacketSize );
   packet* p  = reinterpret_cast<packet*>( &data[0] );
   p->size    = PacketSize;
   std::memcpy( (char*)p->data, sdata, sizeofsdata );
   p->flags   = packet_flags;
   AddData( &data[0], PacketSize );
}

void C_RAsyncSockStream::AddData( char * Data, unsigned long sizeofdata )
{
   std::vector<char>::reference back = m_Data.back();
   m_Data.insert( m_Data.end(), sizeofdata, 0 );
   std::memcpy( (void*)((&back)+1) , Data , sizeofdata );
}


void C_RAsyncSockStream::GetData( char * Data, unsigned long sizeofdata )
{
   std::memcpy( Data, &m_Data.front(), ( sizeofdata > m_Data.size() ) ? m_Data.size() : sizeofdata );
    m_Data.erase( m_Data.begin(), ( m_Data.end() < (m_Data.begin()+sizeofdata) ) ? m_Data.end() : (m_Data.begin()+sizeofdata) );
}

//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------

C_SAsyncSockStream::C_SAsyncSockStream() :
         m_Bytes_Sent( 0 ),
         m_MaxPacketSize( 1024 )
//         m_MaxPacketSize( 0 )
{
// 2^n-1
//   m_MaxPacketSize = (long)pow( 2, sizeof(unsigned short) ) - 1;
//   m_Data.reserve( m_MaxPacketSize*2 ); // reserve max size of 2 packets
   m_Data.reserve( m_MaxPacketSize*2 );
}

void C_SAsyncSockStream::Clear( void )
{
   m_Data.clear();
}

int C_SAsyncSockStream::send( SOCKET s, int flags )
{
   int bytes_sent = 0;
   if( m_Data.size() )
   {
      bytes_sent = ::send( s, &m_Data.front(), ( m_MaxPacketSize < m_Data.size() ) ? m_MaxPacketSize : m_Data.size(), flags );
      if( bytes_sent != SOCKET_ERROR )
      {
         m_Data.erase( m_Data.begin(), ( m_Data.end() < (m_Data.begin()+bytes_sent) ) ? m_Data.end() : (m_Data.begin()+bytes_sent) );
      }
   }
   if( bytes_sent != SOCKET_ERROR )
   {
      m_Bytes_Sent += bytes_sent;
   }
   return bytes_sent;
}

void C_SAsyncSockStream::AddPacket( char* sdata, unsigned char sizeofsdata, unsigned char packet_flags )
{
   unsigned char PacketSize = sizeof(packet) + sizeofsdata;
   std::vector<char> data( PacketSize );
   packet* p  = reinterpret_cast<packet*>( &data[0] );
   p->size    = PacketSize;
   std::memcpy( (char*)p->data, sdata, sizeofsdata );
   p->flags   = packet_flags;
   AddData( &data[0], PacketSize );
}

packet::size_t
//std::size_t 
//unsigned short 
C_SAsyncSockStream::GetPacketSize( void ) const
{
   if( m_Data.size() >= sizeof( packet::size_t ) )
//   if( m_Data.size() >= sizeof( std::size_t ) )
//   if( m_Data.size() >= sizeof( unsigned short ) )
   {
      packet::size_t *length = (packet::size_t*)&m_Data.front();
//      std::size_t *length = (std::size_t*)&m_Data.front();
//      unsigned short *length = (unsigned short*)&m_Data.front();
      return *(length);
   }
   return 0;
}

bool C_SAsyncSockStream::PacketSent( void )
{
   if( m_Data.size() >= sizeof( packet::size_t ) )
//         if( m_Data.size() >= sizeof( std::size_t ) )
//   if( m_Data.size() >= sizeof( unsigned short ) )
   {
      packet::size_t *length = (packet::size_t*)&m_Data.front();
//      std::size_t *length = (std::size_t*)&m_Data.front();
//      unsigned short *length = (unsigned short*)&m_Data.front();
      if( m_Data.size() >= *length )
      return true;
   }
   return false;
}

void C_SAsyncSockStream::GetDataFromPacket( char * Data )
{
//   if( PacketReceived() )
//   {
//      std::size_t sizeofdata = GetPacketSize();
//      std::memcpy( Data, &m_Data.front(), ( sizeofdata > m_Data.size() ) ? m_Data.size() : sizeofdata );
//      m_Data.erase( m_Data.begin(), ( m_Data.end() < (m_Data.begin()+sizeofdata) ) ? m_Data.end() : (m_Data.begin()+sizeofdata) );
//   }
}

void C_SAsyncSockStream::AddData( char * Data, unsigned long sizeofdata )
{
   std::vector<char>::reference back = m_Data.back();
   m_Data.insert( m_Data.end(), sizeofdata, 0 );
   std::memcpy( (void*)((&back)+1) , Data , sizeofdata );
}

void C_SAsyncSockStream::GetData( char * Data, unsigned long sizeofdata )
{
   std::memcpy( Data, &m_Data.front(), ( sizeofdata > m_Data.size() ) ? m_Data.size() : sizeofdata );
   m_Data.erase( m_Data.begin(), ( m_Data.end() < (m_Data.begin()+sizeofdata) ) ? m_Data.end() : (m_Data.begin()+sizeofdata) );
}

