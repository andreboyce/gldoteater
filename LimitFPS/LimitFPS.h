#pragma once

#ifndef __LimitFPS_H
#define __LimitFPS_H

#include <windows.h>
#include "../TimeInterval/TimeInterval.h"

class LimitFPS
{
   public:
      LimitFPS( void ) : m_MaxFPS( 100.0 ), m_Frequency( (1/m_MaxFPS*1000) ) 
      {
         m_TimeInterval.Begin();
      }
      LimitFPS( float maxfps ) : m_MaxFPS( maxfps ), m_Frequency( (1/m_MaxFPS*1000) ) 
      {
         m_TimeInterval.Begin();
      }
      ~LimitFPS( void )
      {
      }

      void SetFrequency( void )
      {
         m_Frequency = (1/m_MaxFPS*1000);
      }

      bool ShouldRender( void )
      {
         DWORD deltatime    = m_TimeInterval.TimeSinceLastCall();
         bool  return_value = false;
         if( deltatime >= m_Frequency )
         {
            return_value = true;
//            if( deltatime >= 1000 )
//            {
               m_TimeInterval.Begin();
//            }
         }
         else
         {
            return_value = false;
         }
         return return_value;
      }

      float fps( void )
      {
         return m_MaxFPS;
      }

      void SetFPS( float new_max_fps )
      {
         m_MaxFPS = new_max_fps;
         SetFrequency();
      }

      operator bool( void )
      {
         return ShouldRender();
      }

      operator float( void ) const
      {
         return m_MaxFPS;
      }

   protected:
      float          m_MaxFPS;
      float          m_Frequency;
      TimeInterval m_TimeInterval;

   private:
};


#endif
