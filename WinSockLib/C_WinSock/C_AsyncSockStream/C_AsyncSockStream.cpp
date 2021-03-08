#include "C_AsyncSockStream.h"

namespace TCPASYNCSocket
{

void C_AsyncSockStream::clear( void )
{
   m_DataList.clear();
}

unsigned long C_AsyncSockStream::size( void )
{
   unsigned long size = 0;
   for( BufferIterator packet; packet != m_DataList.end() ; packet++ )
   {
      size += (*packet).size();
   }
   return size;
}

};

