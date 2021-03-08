#include "C_WinSock.h"

unsigned long C_WinSock::m_NumberOfInstances             = 0;

C_WinSock::C_WinSock( unsigned short version_number, unsigned short version_decimal ) :
   m_bWinSockWasLoaded( false )
{
   if( m_NumberOfInstances == 0 )
   {
      if( _LoadWinSock() )
         m_bWinSockWasLoaded = true;
   }
   m_NumberOfInstances++;
}

C_WinSock::~C_WinSock()
{
   if( m_NumberOfInstances == 1 )
   {
      UnLoadWinSock();
   }
   --m_NumberOfInstances;
}
