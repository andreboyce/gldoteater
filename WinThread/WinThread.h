#ifndef __WinThread_H
#define __WinThread_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class WinThread
{
   public:

      WinThread( LPSECURITY_ATTRIBUTES lpThreadAttributes, DWORD StackSize, LPTHREAD_START_ROUTINE ThreadProc, LPVOID parameters, DWORD dwCreationFlags ) :
         m_Handle( NULL ),
         m_ID( 0 ),
         m_bIsRunning( false ),
         m_ThreadProc( ThreadProc ),
         m_parameters( parameters ),
         m_dwCreationFlags( dwCreationFlags ),
         m_lpThreadAttributes( lpThreadAttributes ),
         m_StackSize( StackSize )
      {}

      ~WinThread( void )
      { 
         Stop( 0 );
      }

      bool Start( void );
      bool Stop( DWORD dwMilliseconds );

      void reset_handle()
      {
         m_Handle     = NULL;
         m_bIsRunning = false;
      }


      operator HANDLE ( void ) const
      {
         return m_Handle;
      }

      operator bool ( void ) const
      {
         return m_bIsRunning;
      }

   protected:

      HANDLE                 m_Handle;
      DWORD                  m_ID;
      bool                   m_bIsRunning;
      LPTHREAD_START_ROUTINE m_ThreadProc;
      LPVOID                 m_parameters;
      DWORD                  m_dwCreationFlags;
      LPSECURITY_ATTRIBUTES  m_lpThreadAttributes;
      DWORD                  m_StackSize;

   private:
};

#endif
