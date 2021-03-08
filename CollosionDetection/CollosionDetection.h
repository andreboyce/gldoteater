#pragma once

#ifndef __CollosionDetection_H
#define __CollosionDetection_H

#include <math.h>
#include <typeinfo>

class CollosionDetection
{
   public:

      struct S_Shape
      {
         public:

            S_Shape( void ): m_x(0), m_y(0), m_z(0), m_height(1)
               {   }
               
            S_Shape( float x, float y, float z, float height ):
                       m_x(x), m_y(y), m_z(z), m_height(height)
               {   }
               
            virtual ~S_Shape( void ){}

            float m_x, m_y, m_z, m_height;
      };

      
      struct S_Rectangle: public S_Shape
      {
         public:

            float m_width;

            S_Rectangle( void ): m_width(1)
               {   }

            S_Rectangle( float x, float y, float z, float width, float height ): 
                       S_Shape( x, y, z, height ), m_width(width)
               {   }
               
            ~S_Rectangle( void )
            {}

      };

      struct S_Triangle: public S_Shape
      {
         float m_base, m_hypotenuse;

         S_Triangle( void ): m_base(1), m_hypotenuse( sqrt(m_height*m_height + m_base*m_base) )
            {   }

         S_Triangle( float x, float y, float z, float base, float height ): 
                    S_Shape( x, y, z, height ), m_base(base), m_hypotenuse(sqrt( m_height*m_height + m_base*m_base) )
            {   }
            
        ~S_Triangle( void ){}
      };
      
// TODO (#1#): Add more shapes …  well mainly ellipses 


       CollosionDetection( void );
      ~CollosionDetection( void );
      
       bool CheckCollide( const S_Triangle&,  const S_Triangle& );      
       bool CheckCollide( const S_Rectangle&, const S_Rectangle& );      
       bool CheckCollide( const S_Rectangle&, const S_Triangle& );      
       bool CheckCollide( S_Shape&, S_Shape& );

   protected:

   private:


};

#endif
