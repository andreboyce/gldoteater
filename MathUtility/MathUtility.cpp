#include "MathUtility.h"

float my_roundf( float x )
{
	float z = 0.0f;
	//double z = 0.0f;
	double fractional_part, intpart;

    fractional_part = modf( x, &intpart );
	if( fractional_part >= 0.5 )
	   z = ceil( x );
	else
	   z = floor( x );
	return z;
}
