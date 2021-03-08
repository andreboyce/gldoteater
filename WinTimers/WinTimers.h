#ifndef __WinTimers_H
#define __WinTimers_H

#include "../errorutil/errorutil.h"

#include <windows.h>
#include <vector>
#include <string>

#define INITIAL_TIMER_ID 0xFF
#define ERROR_CNST 300

      class Timer
      {
         public:
            Timer( void ) :
               m_ID( 0 ),
               m_Hwnd( NULL ),
               m_bisStarted( false ),
               m_tproc( NULL )
            {}
            ~Timer( void ) { Kill(); }

            bool Set( HWND Hwnd, DWORD ID, DWORD interval, TIMERPROC tproc )
            {
               if( m_bisStarted )
                  return false;

               m_Hwnd   = Hwnd;
               m_ID     = ID;
               m_tproc  = tproc;
               if( ::SetTimer( m_Hwnd, ID, interval, tproc ) )
               {
                  return m_bisStarted = true;
               }
               else
               {
                  return false;
               }
            }
            bool Kill( void )
            {
               if( m_bisStarted )
               {
                  if( ::KillTimer( m_Hwnd, m_ID ) )
                  {
                     return m_bisStarted = false;
                  }
                  else
                  {
                     return m_bisStarted = true;
                  }
               }
			   return m_bisStarted;
            }

            operator bool (void) const
            {
               return m_bisStarted;
            }
            operator DWORD (void) const
            {
               return m_ID;
            }

         protected:
            DWORD     m_ID;
            HWND      m_Hwnd;
            bool      m_bisStarted;
            TIMERPROC m_tproc;
      };


class WinTimers
{
   struct S_Timer
   {
       S_Timer( void ): m_interval( 1000 ), m_timerproc( NULL ), m_TimerID( 0 ), m_Set( false )
          {   }

       S_Timer( TIMERPROC timerproc, DWORD interval, DWORD TimerID  ): 
                m_interval( interval ), m_timerproc( timerproc ), m_TimerID( TimerID ), m_Set( false )
          {   }

      ~S_Timer( void )
          {
             ::KillTimer( m_Hwnd, m_TimerID );
          }
      operator bool( void )
      {
         return m_Set;
      }
      
      public:
         TIMERPROC m_timerproc;
         DWORD     m_interval;
         DWORD     m_TimerID;
         bool      m_Set;
         
         static    HWND m_Hwnd;
   };
   
   typedef std::vector<S_Timer> V_Timers;
   typedef V_Timers::iterator   V_TimersIterator;

   public:

      WinTimers( void ): m_Hwnd( NULL ), m_NextTimerID( INITIAL_TIMER_ID ), m_Timers( 0 )
         {   
            //ErrorHandlers.AddErrorString( ERROR_CNST, "Error could not Set Timer" );
         }

      WinTimers( const HWND& Hwnd, const short& initialtimerID = INITIAL_TIMER_ID ):
                   m_Hwnd( Hwnd ), m_NextTimerID( initialtimerID ), m_Timers( 0 )
         {
            WinTimers::S_Timer::m_Hwnd = m_Hwnd;
            //ErrorHandlers.AddErrorString( ERROR_CNST, "Error CouldNot Set Timer" );
         }

//      WinTimers( const WinTimers& );
     ~WinTimers( void )
         {   }

      bool AddTimer( DWORD interval, TIMERPROC timerproc );
      bool SetTimer( const DWORD& index  );
      
      bool KillTimer( const short& index )
         {
            if( m_Timers.at( index ) )
            {
               V_TimersIterator temp = m_Timers.begin()+index;
               m_Timers.erase( temp );
            }
         }

      void inline Clear( void )
         {  m_Timers.clear(); }
      
      void inline SetHwnd( const HWND& Hwnd )
      {
         if( !m_Timers.size() && ::IsWindow( Hwnd ) )
         {
            WinTimers::S_Timer::m_Hwnd = m_Hwnd;
            m_Hwnd = Hwnd;
         }
      }

   protected:

      V_Timers    m_Timers;
      HWND        m_Hwnd;
      short       m_NextTimerID;
      std::string lasterror;
      //errorutil    ErrorHandlers;

   private:
};

#endif
