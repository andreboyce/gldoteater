#pragma once

#ifndef __TimeInterval_H
#define __TimeInterval_H

#include <windows.h>

// I want to be able to determine how much time has elapsed since a function was last 
// called. 
// Also want to determine the number of times a interval of times has passed since 
// the last call to the function

class TimeInterval
{
   public:
      TimeInterval( void ) : prevtime(0), m_interval(0), m_saved_time(0){ }
      TimeInterval( DWORD interval, bool bBegin=false ) : m_interval(interval), m_saved_time(0){ if(bBegin) Begin(); }
      ~TimeInterval( void ){}

      // Stores the previous time
      void inline Begin( DWORD additionaltime = 0 )
      {
         // additionaltime is usefull for instances when you want to pause the timer
         // and begin again
         prevtime = ::GetTickCount() - additionaltime;         
      }

      // Returns time since begin was last called
      DWORD inline TimeSinceLastCall( void )
      {
         if( prevtime ) // we dont want to return a rediclously large number
            return (::GetTickCount() - prevtime);
         return 0;
      }

      // sets the interval
      DWORD inline SetInterval( DWORD newinterval )
      {
         DWORD previnterval = m_interval;
         m_interval = newinterval;
         return previnterval;
      }

      // True if the time passed since begin is grater than the interval
      bool inline IntervalExceeded( void )
      {
         //DWORD time = ::GetTickCount();
         if( TimeSinceLastCall() > m_interval )
            return true;
         return false;
      }

      // Returns the timepassed / interval
      DWORD inline GetIntervalsPassed( void )
      {
         return (DWORD)(TimeSinceLastCall()/m_interval);
      }

      DWORD inline GetInterval( void ) const
      {
         return m_interval;
      }

      void inline ResetTime( void )
      {
         prevtime = 0;
      }
      
      // use when you want to pause the timer
      void SaveTime( void )
      {
         m_saved_time = TimeSinceLastCall();
      }

      // use when you want to restart the timer ... Interval.Begin( Interval.GetSavedTime() )
      DWORD GetSavedTime( void ) const
      {
         return m_saved_time;
      }


   protected:
      DWORD prevtime, m_interval, m_saved_time;

   private:
};

class C_Flash
{
   public:
      C_Flash( void ) : m_Duration( 500 ), m_PreviousTime( 0 ), m_TimeLeft( 500 ), m_bOn( false ){}
      C_Flash( bool bOn, DWORD duration ) : m_Duration( duration ), m_PreviousTime( 0 ), m_TimeLeft( duration ), m_bOn( bOn ){}
      ~C_Flash(){}

      void SetDuration( DWORD new_duration )
      {
         m_Duration = m_TimeLeft = new_duration;
      }

      operator bool ( void ) const
      {
         return m_bOn;
      }

      inline bool Update( void )
      {
         DWORD current_time = ::GetTickCount();
         int difference = (current_time-m_PreviousTime);

         if( difference > m_Duration )
            m_PreviousTime = current_time;

         m_TimeLeft    -= difference;
         m_PreviousTime = current_time;
         
         if( m_TimeLeft <= 0 )
         {
            (m_bOn) ? m_bOn = false : m_bOn = true;
            m_TimeLeft = m_Duration;
         }
         return m_bOn;
      }

      void Reset( void )
      {
         m_TimeLeft = m_Duration;
      }

   protected:

      DWORD m_Duration, m_PreviousTime;
      long  m_TimeLeft;
      bool  m_bOn;

   private:
};

#endif

