#include "CFmod.h"

CFmod::CFmod( bool bUseAlert ) :
   m_bFmodStarted( false ),
   m_bUseAlert( bUseAlert )
{
   FMOD_RESULT result;

   // Create the main system object.
   result = FMOD::System_Create( &m_System );
   //result = FMOD::Studio::System::create(&m_System);
   if( !GoodResult( result ) )
   {
      return;
   }

   // Initialize FMOD.
   result = m_System->init( 32, FMOD_INIT_NORMAL, 0 );
   //result = m_System->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
   if( !GoodResult( result ) )
   {
      return;
   }

   m_bFmodStarted = true;
}

CFmod::~CFmod( void )
{
   if( m_System != NULL )
   {
      m_System->release();
   }
}

bool CFmod::PlayBgMusic( void )
{
   if( !m_bFmodStarted )
      return false;

   FMOD_RESULT  result;
   FMOD::Sound *sound = NULL;

   // FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE.
   result = m_System->createSound( m_BgSound.c_str(), FMOD_DEFAULT, NULL, &sound );
   if( !GoodResult( result ) )
   {
      return false;
   }

   FMOD::Channel *channel = NULL;
   //result = m_System->playSound( FMOD_CHANNEL_FREE, sound, false, &channel );
   result = m_System->playSound( sound, NULL, false, &channel );
   if( !GoodResult( result ) )
   {
      return false;
   }

   return true;
}

bool CFmod::PlayClip( std::string str )
{
   if( !m_bFmodStarted )
      return false;
   // FMUSIC_MODULE *clip = FMUSIC_LoadSong( str.c_str() );
   // FSOUND_SAMPLE *clip = ::FSOUND_Sample_Load( FSOUND_FREE , str.c_str(), 0, 0, 0 );
   // ::FSOUND_PlaySound( 0, clip );
   // FMUSIC_PlaySong( clip );

   FMOD_RESULT  result;
   FMOD::Sound *sound = NULL;

   // FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE.
   result = m_System->createSound( str.c_str(), FMOD_DEFAULT, NULL, &sound );
   if( !GoodResult( result ) )
   {
      return false;
   }

   FMOD::Channel *channel = NULL;
   //result = m_System->playSound( FMOD_CHANNEL_FREE, sound, false, &channel );
   //result = m_System->playSound( FMOD_CHANNEL_FREE, sound, false, &channel );
   result = m_System->playSound( sound, NULL, false, &channel );
   if( !GoodResult( result ) )
   {
      return false;
   }

   return true;
}

bool CFmod::PlayClip( short index, short channel )
{
   if( !m_bFmodStarted )
      return false;

   if( ( index < 0 ) || ( index >= m_Sounds.size() ) )
      return false;

   FMOD_RESULT  result;
   FMOD::Sound *sound = NULL;

   // FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE.
   result = m_System->createSound( m_Sounds[ index ].c_str(), FMOD_DEFAULT, NULL, &sound );
   if( !GoodResult( result ) )
   {
      return false;
   }

   //FMOD::Channel *channel;
   //result = m_System->playSound( FMOD_CHANNEL_FREE, sound, false, NULL );
   result = m_System->playSound( sound, NULL, false, NULL );
   if( !GoodResult( result ) )
   {
      return false;
   }

   return true;
}

bool CFmod::LoadClip( std::string str )
{
   try
   {
      short prevsize = m_Sounds.size();
	  //m_System->createSound( str, FMOD_DEFAULT, NULL, &sound );
      m_Sounds[ prevsize ] = str;
      //if( m_SoundSamples.size() <= prevsize )
      //{
      //   return false;
      //}
      return true;
   }
   catch( std::exception& e )
   {
      Alert( e.what() );
	  return false;
   }
   catch(...)
   {
      Alert( "Unknown Exception" );
      return false;
   }
}

void CFmod::SetVolume( short vol )
{
   if( !m_bFmodStarted )
      return;
   //FSOUND_SetVolume( 0, vol );
}

bool CFmod::IsPlaying( int channel )
{
   if( !m_bFmodStarted )
      return false;
   return true;
   //return (bool)::FSOUND_IsPlaying( channel );
}
