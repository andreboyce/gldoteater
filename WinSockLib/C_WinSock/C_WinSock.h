#ifndef __C_WinSock_H
#define __C_WinSock_H

#include <winsock2.h>
#include <string>
#include <vector>

#include "../../errorutil/errorutil.h"
#include "C_WinSockErrors/C_WinSockErrors.h"

class C_WinSock
{
   public:
      C_WinSock( unsigned short version_number = 2, unsigned short version_decimal = 2 );
      ~C_WinSock();

      std::string GetLastWinSockError( void )
      {
         return m_WinSockErrors.GetLastErrorString();
      }

      std::string GetLastWinSockError( int errorcode )
      {
         return m_WinSockErrors.GetLastErrorString( errorcode );
      }

      bool WinSockWasLoaded( void ) const
      {
         return m_bWinSockWasLoaded;
      }

      bool LoadWinSock( unsigned short version_number = 2, unsigned short version_decimal = 2 )
      {
         if( !m_bWinSockWasLoaded )
            return _LoadWinSock( version_number, version_decimal );
         return true;
      }

   protected:

      bool _LoadWinSock( unsigned short version_number = 2, unsigned short version_decimal = 2 )
      {
         WSAData wsaData;

         if( ::WSAStartup( MAKEWORD(version_number, version_decimal), &wsaData) == SOCKET_ERROR )
         {
//            m_LastError = m_WinSockErrors.GetLastErrorString();
            return false;
         }
         return true;
      }

      void UnLoadWinSock( void )
      {
         ::WSACleanup();
      }

      C_WinSockErrors m_WinSockErrors;
      bool            m_bWinSockWasLoaded;
//      std::string     m_LastError;

   private:
      static unsigned long m_NumberOfInstances;
};

#endif
