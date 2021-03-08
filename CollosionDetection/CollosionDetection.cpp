#include "CollosionDetection.h"


CollosionDetection::CollosionDetection( void )
{
}

CollosionDetection::~CollosionDetection( void )
{
}

bool CollosionDetection::CheckCollide( const S_Rectangle& Rect1, const S_Rectangle &Rect2 )
{
    float left1,   left2;
    float right1,  right2;
    float top1,    top2;
    float bottom1, bottom2;

    left1   = Rect1.m_x;
    left2   = Rect2.m_x;
    right1  = Rect1.m_x + Rect1.m_width;
    right2  = Rect2.m_x + Rect2.m_width;
    top1    = Rect1.m_y;
    top2    = Rect2.m_y;
    bottom1 = Rect1.m_y + Rect1.m_height;
    bottom2 = Rect2.m_y + Rect2.m_height;

    if (bottom1 < top2) return false;
    if (top1 > bottom2) return false;

    if (right1 < left2) return false;
    if (left1 > right2) return false;

    return true;
}

bool CollosionDetection::CheckCollide( S_Shape& Shape1, S_Shape& Shape2 )
{

//   S_Rectangle& Rect1;
//   S_Rectangle& Rect2;
//   S_Triangle&  Tri1;
//   S_Triangle&  Tri2;
   
   return CheckCollide( dynamic_cast<S_Rectangle&>(Shape1), dynamic_cast<S_Rectangle&>(Shape2) );

/*
   try
   {
      Rect1 = dynamic_cast<S_Rectangle&>(Shape1);
   }
   catch( std::bad_cast )
   {
      try
      {
         Tri1 = dynamic_cast<S_Triangle&>(Shape1);
      }
      catch( std::bad_cast )
      {
         return 0;
      }      
   }
   
   try
   {
      Rect2 = dynamic_cast<S_Rectangle&>(Shape2);
   }
   catch( std::bad_cast )
   {
      try
      {
         Tri2 = dynamic_cast<S_Triangle&>(Shape2);
      }
      catch( std::bad_cast )
      {
         return 0;
      }      
   }
   
   if( Rect1 )
   {
      if( Rect2)
   }
*/
}
