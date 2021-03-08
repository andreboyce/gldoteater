#ifndef __CMutex_H
#define __CMutex_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

class CMutex
{
   public:

      CMutex( void ) : m_Mutex( NULL ) {}
      ~CMutex(){ Destroy(); }

      void Destroy( void )
      { CloseHandle(m_Mutex); m_Mutex = NULL; }

      operator HANDLE (void) const
      {
         return m_Mutex;
      }

      operator std::string (void)
      {
         return m_Name;
      }

      bool Create( bool bThisThreadisIiitialOwner, std::string name )
      {
         if( m_Mutex != NULL )
            CloseHandle(m_Mutex);
         m_Name  = name;
         m_Mutex = ::CreateMutex( NULL, bThisThreadisIiitialOwner, (LPCTSTR) name.c_str() );
         if( !m_Mutex )
            return false;
         return true;
      }

      DWORD Lock( DWORD time = INFINITE )
      {
         if( m_Mutex != NULL )
            return ::WaitForSingleObject( m_Mutex, time );
         return 0;
      }

      bool Release( void )
      {
         if( m_Mutex != NULL )
            return (bool) ::ReleaseMutex( m_Mutex );
         return 0;
      }
   
   protected:
      HANDLE      m_Mutex;
      std::string m_Name;

   private:
};

#endif

