#ifndef __SideScrollingText_H
#define __SideScrollingText_H

#include <string>
#include "../TimeInterval/TimeInterval.h"

class SideScrollingText
{
   public:
      SideScrollingText( void ) : m_Index( 0 ), m_Range( 10 ), m_Duration( 200 )
      {
         m_Interval.Begin();
      }

      SideScrollingText( unsigned short Range, DWORD duration=200 ) : m_Index( 0 ), m_Range( Range ), m_Duration( duration )
      {
         m_Interval.Begin(); 
      }

      ~SideScrollingText( void ){}

      void SetRange( unsigned short Range )
      {
         m_Range = Range;
      }

      void SetDuration( DWORD duration )
      {
         m_Duration = duration;
      }

      void clear( void )
      {
         m_Text.erase( m_Text.begin(), m_Text.end() );
      }

      inline void AddText( std::string text )
      {
         AddText( text.c_str() );
      }

      inline void AddText( const char* text )
      {
         m_Text += text;
      }

      const char* ScrollLeft( void )
      {
         if( m_Text.size() )
         {
            if( m_Interval.TimeSinceLastCall() >= m_Duration )
            {
               m_Interval.Begin();
               m_Index++;
               if( m_Index >= m_Text.size() )
                  m_Index = 0;
            }
//            return (m_Text.c_str()+m_Index);
              if( m_Index < m_Range )
              {
//                 if( m_Index >= m_Text.size() )
//                 {
//                 }
                 std::string temp( m_Range-m_Index, ' ' );
                 return (temp+m_Text.substr( 0, m_Index )).c_str();
              }
              else
                 return m_Text.substr( m_Index, m_Range ).c_str();
         }
         return NULL;
      }

      const char* ScrollRight( void )
      {
         if( m_Text.size() )
         {
            if( m_Interval.TimeSinceLastCall() >= m_Duration )
            {
               m_Interval.Begin();
               m_Index++;
               if( m_Index >= m_Text.size() )
                  m_Index = 0;
            }
//            return (m_Text.c_str()+m_Index);
              return m_Text.substr( m_Index, m_Range ).c_str();
         }
         return NULL;
      }

   protected:
      std::string    m_Text;
//      std::string    m_TempString;
      TimeInterval m_Interval;
      unsigned short m_Index;
      unsigned short m_Range;
      DWORD          m_Duration;

   private:
};

#endif
