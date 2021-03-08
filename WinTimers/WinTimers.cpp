#include "WinTimers.h"

HWND WinTimers::S_Timer::m_Hwnd = NULL;

bool WinTimers::AddTimer( DWORD interval, TIMERPROC timerproc )
{
   S_Timer temptimer;
   temptimer.m_TimerID   = m_NextTimerID;
   temptimer.m_interval  = interval;
   temptimer.m_timerproc = timerproc;

   m_Timers.push_back( temptimer );   
   m_NextTimerID++;
   return false;
}

bool WinTimers::SetTimer( const DWORD& index )
{   
   if( ( index < m_Timers.size() ) )
   {
      if( !::SetTimer( m_Hwnd, m_Timers[index].m_TimerID, m_Timers[index].m_interval, m_Timers[index].m_timerproc ) )
      {
         GetLastSystemError( lasterror );
         lasterror += "Error could not Set Timer";
         Alert( lasterror.c_str() );
      }
      else
      {
         m_Timers[index].m_Set = true;
      }
   }
   return false;
}


