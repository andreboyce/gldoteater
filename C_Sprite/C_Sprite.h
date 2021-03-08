#ifndef __C_Sprite_H
#define __C_Sprite_H

#include <queue>
#include "../errorutil/errorutil.h"
#include "../Structs/Structs.h"
class C_Sprite
{
   public:
      struct S_Frame
      {
         S_Frame() : m_duration( 10 ), m_index( 0 ) {}
         S_Frame( const S_Frame& rhs ) : m_duration( rhs.m_duration ), m_index( rhs.m_index ) {}
         S_Frame& operator=( const S_Frame& rhs )
         {
            m_duration = rhs.m_duration;
            m_index    = rhs.m_index;
            return *this;
         }
         int       m_index;
         DWORD     m_duration;
      };
      C_Sprite( DWORD PrevTime ) : m_PrevTime( PrevTime ), m_Pos(), m_Frames() {}
      C_Sprite() : m_PrevTime( 0 ), m_Pos(), m_Frames() {}
      ~C_Sprite(){}
      C_Sprite( const C_Sprite& rhs ) : 
         m_PrevTime( rhs.m_PrevTime ), 
         m_Pos( rhs.m_Pos ), 
         m_Frames( rhs.m_Frames ) {}
      C_Sprite& operator=( const C_Sprite& rhs )
      {
         m_PrevTime = rhs.m_PrevTime;
         m_Frames   = rhs.m_Frames;
         m_Pos      = rhs.m_Pos;
         return *this;
      }
      S_Frame& GetFrame( int index );
      std::deque< S_Frame >& GetFrames( void )
      {
         return m_Frames;
      }
      bool AddFrame( void );
      Structs::Point& GetPosition( void )
      {
         return m_Pos;
      }
      void SetPosition( Structs::Point Pos )
      {
         m_Pos = Pos;
      }

   protected:
      DWORD                 m_PrevTime;
      std::deque< S_Frame > m_Frames;
      Structs::Point      m_Pos;
      

   private:
};

#endif

