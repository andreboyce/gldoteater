#pragma once

#ifndef __CFmod_H
#define __CFmod_H

#include <string>
#include <map>

//#include <fmod.hpp>
//#include <fmod_errors.h>

#include <FMOD/inc/fmod.hpp>
#include <FMOD/inc/fmod_errors.h>

#include "../errorutil/errorutil.h"

class CFmod
{
   public:

       CFmod( bool bUseAlert = true );
      ~CFmod( void );

      void SetBgMusic( std::string BgSound )
      {
         m_BgSound = BgSound;
      }

      void SetBgMusic( const char* BgSound )
      {
         m_BgSound = BgSound;
      }

      bool GoodResult( FMOD_RESULT result )
      {
         if( result != FMOD_OK )
         {
			if( m_bUseAlert )
			{
			   Alert( std::string( "FMOD Error: " ) + FMOD_ErrorString( result ) );
			}
            return false;
         }
		 return true;
      }

      void SetVolume( short new_voulme );

      bool PlayBgMusic( void );
      bool PlayClip( std::string );
      bool PlayClip( short index, short channel=0 );
      bool LoadClip( std::string filename );
      bool IsPlaying( int channel );

	  inline FMOD_RESULT Update( void )
	  {
         return m_System->update();
	  }

   protected:

	  std::map< int, std::string > m_Sounds;

	  bool           m_bUseAlert;
      std::string    m_BgSound;
      bool           m_bFmodStarted;
	  FMOD::System   *m_System;
	  //FMOD::Studio::System* m_System;

};

#endif

