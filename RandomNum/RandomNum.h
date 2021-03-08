#ifndef __RandomNum_H
#define __RandomNum_H

#include <stdlib.h>

class RandomNum
{
   public:
      RandomNum( void ) : m_Seed( 0 ), m_Index( 0 )
      {
         srand( m_Seed );
      }
      RandomNum( int seed ) : m_Seed( seed ), m_Index( 0 )
      {
         srand( m_Seed );
      }
      RandomNum( int seed, int index ) : m_Seed( seed ), m_Index( index )
      {
         srand( m_Seed );
         MoveToIndex( index );
      }
      void MoveToIndex( int index )
      {
         this->srand( m_Seed );
         for( int i=0; i<index ; i++ )
            this->rand();
      }
      unsigned int rand()
      {
         return ::rand();
      }
      void srand( unsigned int seed )
      {
         return ::srand( seed );
      }

   private:
      unsigned int m_Seed;
      unsigned int m_Index;

   protected:
};

#endif
