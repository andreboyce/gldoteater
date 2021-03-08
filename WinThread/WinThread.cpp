#include "WinThread.h"

bool WinThread::Start( void )
{
   if( m_Handle )
   {
      Stop( 0 );
   }

   m_Handle = ::CreateThread( m_lpThreadAttributes, m_StackSize, (LPTHREAD_START_ROUTINE)m_ThreadProc, m_parameters,
                              m_dwCreationFlags, (LPDWORD)&m_ID );

   if( !m_Handle )
   {
      // Handle Error
      m_bIsRunning = false;
      return false;
   }

   m_bIsRunning = true;
   return true;
}

bool WinThread::Stop( DWORD dwMilliseconds )
{
   if( m_Handle != NULL )
   {
      m_bIsRunning = false;
      ::WaitForSingleObject( m_Handle, dwMilliseconds );
      ::CloseHandle( m_Handle );
      m_Handle = NULL;
      return true;
   }
   return false;
}

