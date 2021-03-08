#include "C_Shell.h"

//***************************************************************************
//******************************* C_Shells::Draw ****************************
// Accepts: void
//
// Purpose: tell the textured quad to render its primatives
//
// Returns: void

void C_Shell::Draw()
{
   m_TexQuad.Draw( 0, 0, 0 );
}

//***************************************************************************
//******************************* C_Shells::Draw ****************************
// Accepts: const Structs::Point TextureCoordinates[4] // the texture coordinates
//
// Purpose: tell the textured quad to render its primatives
//          with the given texture coordinates
//
// Returns: void

void C_Shell::Draw( const Structs::Point TextureCoordinates[4] )
{
   m_TexQuad.Draw2( 0, 0, 0, TextureCoordinates );
}

//***************************************************************************
//******************************* C_Shells::Rect ****************************
// Accepts: void
//
// Purpose: get the bounding box used for collision detection
//
// Returns: void

Structs::fRect C_Shell::Rect()
{  
   return Structs::fRect( m_PositionVector.m_x-m_TexQuad.halfwidth(),  // left
                            m_PositionVector.m_y+m_TexQuad.halfheight(), // top
                            m_PositionVector.m_x+m_TexQuad.halfwidth(),  // right
                            m_PositionVector.m_y-m_TexQuad.halfheight()  // bottom
                          );
}

