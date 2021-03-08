#ifndef __FrameRate_H
#define __FrameRate_H

#include <windows.h>
#include <string>

#define FRAME_RATE_STRING ",  Frame Rate: "

class FrameRate
{
   public:
      FrameRate( void ) : m_fps( 0 )
      {
      }
      
      FrameRate( std::string& Title ) : orignaltitle( Title ), windowtitle( "" )
         { 
            orignaltitle         = Title;
            orignaltitleplusfps  = orignaltitle;
            orignaltitleplusfps  += FRAME_RATE_STRING;
         } 
         
      FrameRate( char Title[] ) : orignaltitle( Title ), windowtitle( "" )
         { 
            orignaltitle = Title; 
            orignaltitleplusfps  = orignaltitle;
            orignaltitleplusfps  += FRAME_RATE_STRING;
         }
         
      ~FrameRate();
      
      void SetTitle( char Title[] )
         { 
            orignaltitle = Title; 
            orignaltitleplusfps  = orignaltitle;
            orignaltitleplusfps  += FRAME_RATE_STRING;
         }
         
      void SetTitle( const std::string& Title )
         { 
            orignaltitle = Title;
            orignaltitleplusfps  = orignaltitle;
            orignaltitleplusfps  += FRAME_RATE_STRING;
         }
         
      float ShowFPSinTitle( const HWND );
      float CalculateFrameRate( void );
      
      float inline FPS( void )
      {
         ShowFPSinTitle( NULL );
         return m_fps;
      }
      
   protected:

   private:

      std::string orignaltitle;
      std::string orignaltitleplusfps;
      std::string windowtitle;
      float       m_fps;
};

#endif

