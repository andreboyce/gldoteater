#ifndef __KeyInput_H
#define __KeyInput_H

#include <windows.h>
#include <vector>
#include "../errorutil/errorutil.h"

class KeyInput
{
   public:

      typedef std::vector<bool>   KeyVector;
      typedef KeyVector::iterator KeyVectorIterator;

      typedef std::vector<DWORD>      TimeKeyWasPressed;
      typedef TimeKeyWasPressed::iterator TimeKeyWasPressedIterator;

      typedef std::vector<unsigned long>   RepeatCountIntervalVector;


      KeyInput( void );
      ~KeyInput( void );

      unsigned int SetRepeatCountInterval( unsigned short index, unsigned long newinterval )
      {
         unsigned long previnterval = m_RepeatCountIntervals.at( index );
         m_RepeatCountIntervals.at( index )    = newinterval;
         return previnterval;
      }

      unsigned int SetDefaultRepeatCountInterval( unsigned long newinterval )
      {
         for( int i=0; i< m_RepeatCountIntervals.size(); i++ )
         {
            m_RepeatCountIntervals.at( i ) = newinterval;
         }
         return 0;
      }

      unsigned int GetRepeatCountInterval( unsigned short index )
      {
         return m_RepeatCountIntervals.at( index );
      }

      unsigned int GetRepeatCount( unsigned short index )
      {
         if( Keys.at(index) )
         {
            if( m_RepeatCountIntervals.at( index ) != 0 )
            {
               return (unsigned int)(DurationPressed( index ) / m_RepeatCountIntervals.at( index ));
            }
         }
         return 0;
      }

      void inline PressKey( unsigned short index )
              {
                 if( !Keys.at(index) )
                 {
                    m_TimeKeyWasPressed.at( index ) = ::GetTickCount();
                    Keys.at(index) = true;
                 }
              }

      void inline ReleaseKey( unsigned short index )
              {
//                 Alert( "Key released" );
//                 Alert( ::GetTickCount() );
//                 Alert( m_TimeKeyWasPressed.at( index ) );
                 m_TimeKeyWasPressed.at( index ) = 0;
//                 Alert( DurationPressed( index ) );
                 Keys.at(index) = false;
              }

      DWORD inline DurationPressed( unsigned short index )
              {
                 if( Keys.at(index) )
                 {
                    DWORD ret = ::GetTickCount() - m_TimeKeyWasPressed.at( index );
//                    Alert( ret );
                    return ret; // currenttime - prevtime = duration
                 }
                 return 0;
              }

      bool inline IsPressed( unsigned short index )
              { return Keys.at(index); }

      KeyVector::reference inline operator[]( unsigned short index )
              { return Keys.at(index); }

      bool inline Reset( bool bPressed = false )
              {
                 for( int i = 0; i < 256 ; ++i )
                    Keys[i] = bPressed;
                 return true;
              }

   protected:

      KeyVector                 Keys;
      TimeKeyWasPressed         m_TimeKeyWasPressed;
      RepeatCountIntervalVector m_RepeatCountIntervals;
      
   private:
};
#endif
