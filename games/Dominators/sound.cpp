#include "main.h"

//***************************************************************************
//******************************* MainWindow::PlayClip **********************
// Accepts: long sound_clip, // the index of the clip that was loded
//          short channel // thye channel to play the sound on
//
// Purpose: play a sound on a channel if sound is on
//
// Returns: true if the sound was played
//          false if there was an error

bool MainWindow::PlayClip( long sound_clip, short channel )
{
   if( m_bSoundOn )
   {
      return m_Fmod.PlayClip( sound_clip, channel );
   }
   return false;
}

