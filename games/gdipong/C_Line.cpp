#include "C_Line.h"

void C_Line::Draw( void )
{
   Structs::Point lineend( m_point.x, m_point.y+m_LineLength);
   m_memDC->Line( m_point, lineend );
}
