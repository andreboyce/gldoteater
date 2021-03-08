#include "C_GLVertex.h"

C_GLVertex::C_GLVertex( void )
{
   x     = 0.0f;
   y     = 0.0f;
   z     = 0.0f;      
   red   = 1.0f;
   green = 1.0f;
   blue  = 1.0f;
   alpha = 1.0f;
}

C_GLVertex::C_GLVertex( const C_GLVertex& C_GLVertexRHS )
{
   x     = C_GLVertexRHS.x;
   y     = C_GLVertexRHS.y;
   z     = C_GLVertexRHS.z;
   red   = C_GLVertexRHS.red;
   green = C_GLVertexRHS.green;
   blue  = C_GLVertexRHS.blue;
   alpha = C_GLVertexRHS.alpha;
}

C_GLVertex::~C_GLVertex( void )
{
}
