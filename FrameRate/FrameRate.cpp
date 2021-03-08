#include "FrameRate.h"

FrameRate::~FrameRate()
{
}

float FrameRate::ShowFPSinTitle( const HWND hwnd )
{
          char   str[256];
          DWORD  time       = 0;
   static DWORD  prevtime   = GetTickCount();
   static float  frames     = 0;

   if( ( time = (::GetTickCount() - prevtime) ) >= 1000 )
   {
      frames /= ((float)time/1000); // (time/1000) changes time from miliseconds to seconds
                          // If time = 1000 then frames = frames per second 
                          // If time > 1000 then frames is calculated to a few decimal places
                          // eg . frames = frames/1.2 ...
      if( hwnd )
      {
         sprintf_s( str, "%.2f", frames );
         windowtitle  = orignaltitleplusfps;
         windowtitle += str;
         ::SetWindowText( hwnd, windowtitle.c_str() );
      }
      prevtime = ::GetTickCount();
      m_fps    =  frames;
      frames = 0;
   }
   frames += 1;
   return m_fps;
}


