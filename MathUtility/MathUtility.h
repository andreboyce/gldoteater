#pragma once

#ifndef __MathUtility_H
#define __MathUtility_H

#define _USE_MATH_DEFINES
#include <cmath>

// example
// a = 5 , b = 7, range = 3
// 

inline bool IsInRange( double value1, double value2, double range )
{
   if( ( value1 >= (value2-range) ) && ( value1<=(value2+range) ) )
      return true;
   return false;
}

float my_roundf ( float x );
//double my_roundf (double x );

#endif
