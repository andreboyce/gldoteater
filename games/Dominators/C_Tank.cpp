#include "C_Tank.h"

//***************************************************************************
//******************************* C_Tank::CheckForNoLife ****************
// Accepts: void
//
// Purpose: if life less than zero
//          life is set equal to orignal life and
//          it returns true
//          else it returns false because the tank still has life
//
// Returns: return false if it has life
//          return true it has no life

bool C_Tank::CheckForNoLife( void )
{
   if( m_Life < 0 )
   {
      m_Life = m_OrignalLife;
      return true;
   }
   return false;
}


//***************************************************************************
//******************************* C_Tank::CheckForNoLife ****************
// Accepts: void
//
// Purpose: if life less than zero
//          life is set equal to orignal life and
//          it returns true
//          else it returns false because the tank still has life
//
// Returns: return false if it has life
//          return true it has no life

bool C_Tank::CheckForNoLife( double OrignalLife )
{
   if( m_Life < 0 )
   {
      m_Life = OrignalLife;
      return true;
   }
   return false;
}

//***************************************************************************
//******************************* C_Tank::ReduceLife ************************
// Accepts: void
//
// Purpose: reduce the life by the amound of damage
//
// Returns: life remaining

double C_Tank::ReduceLife( double how_much_damage )
{
   m_Life -= how_much_damage;
   return m_Life;
}

//***************************************************************************
//******************************* C_Tank::Draw **************************
// Accepts: void
//
// Purpose: tell the textured quad to draw its primatives
//
// Returns: void

void C_Tank::Draw( void )
{
   m_TexQuad.Drawc( 0, 0, 0 );
}

//***************************************************************************
//******************************* C_Tank::Draw **************************
// Accepts: const Structs::Point TextureCoordinates[4] // texture coordinates
//
// Purpose: tell the textured quad to draw its primatives
//          with the given texture coordinates
//
// Returns: void

void C_Tank::Draw( const Structs::Point TextureCoordinates[4] )
{
   m_TexQuad.Draw2( 0, 0, 0, TextureCoordinates );
}

//***************************************************************************
//******************************* C_Tank::DrawShells ********************
// Accepts: void
//
// Purpose: render all the shells
//
// Returns: void

void C_Tank::DrawShells( C_GLTextureManager &TextureManager )
{
   for( int i=0; i<m_Shells.size() ; i++ )
   {
      ::glLoadIdentity();
      ::glBindTexture( GL_TEXTURE_2D, TextureManager[5] );
      ::glTranslatef( m_Shells[i].m_PositionVector.m_x, m_Shells[i].m_PositionVector.m_y, m_Z );
      ::glBegin( GL_QUADS );
      m_Shells[i].Draw();
      ::glEnd();
   }

   ::glLoadIdentity();
}

//***************************************************************************
//******************************* C_Tank::DrawShells ************************
// Accepts: C_GLTextureManager &TextureManager, // the texture manager class to get thetexture from
//          const Structs::Point TextureCoordinates[4] // texture coordinates that can cause it to be drawn upside down
//
// Purpose: render all the shells with a given set of texture coordinates
//
// Returns: void

void C_Tank::DrawShells( C_GLTextureManager &TextureManager, const Structs::Point TextureCoordinates[4] )
{
   for( int i=0; i<m_Shells.size() ; i++ )
   {
      ::glLoadIdentity();
      ::glBindTexture( GL_TEXTURE_2D, TextureManager[5] );
      ::glTranslatef( m_Shells[i].m_PositionVector.m_x, m_Shells[i].m_PositionVector.m_y, m_Z );
      ::glBegin( GL_QUADS );
      m_Shells[i].Draw( TextureCoordinates );
      ::glEnd();
   }
   ::glLoadIdentity();
}

//***************************************************************************
//******************************* C_Tank::UpdateShells ******************
// Accepts: void
//
// Purpose: cause the shells to move according to their velocity vector
//          erase them if they move off the visable screen
//
// Returns: void

void C_Tank::UpdateShells( double deltatime, Structs::fRect boundary )
{
   for( int i=0; i<m_Shells.size() ; i++ )
   {
      m_Shells[i].m_PositionVector.m_y += m_Shells[i].m_VelocityVector.m_y*deltatime;
      if( m_Shells[i].m_PositionVector.m_y > boundary.top )
      {
         m_Shells.erase( m_Shells.begin()+i );
      }
   }
}

//***************************************************************************
//******************************* C_Tank::AddShell **********************
// Accepts: Vector2D vel, velocity
//          Vector2D accel, acceleration
//
// Purpose: fire a shell, with a given velocity and acceleration
//
// Returns: void

void C_Tank::AddShell( Vector2D vel, Vector2D accel )
{
   m_Shells.push_back( C_Shell() );
   m_Shells.back().m_PositionVector     = m_PositionVector;
   m_Shells.back().m_VelocityVector     = vel;
   m_Shells.back().m_AccelerationVector = accel;
}

//***************************************************************************
//******************************* C_Tank::AddShell **********************
// Accepts: void
//
// Purpose: fire a shell
//
// Returns: void

void C_Tank::AddShell()
{
   m_Shells.push_back( C_Shell() );
   m_Shells.back().m_PositionVector = m_PositionVector;
}

//***************************************************************************
//******************************* C_Tank::Rect **************************
// Accepts: void
//
// Purpose: get the rectangle bounding box used for collision detection
//
// Returns: void

Structs::fRect C_Tank::Rect()
{  
   return Structs::fRect( m_PositionVector.m_x-m_TexQuad.halfwidth(),  // left
                            m_PositionVector.m_y+m_TexQuad.halfheight(), // top
                            m_PositionVector.m_x+m_TexQuad.halfwidth(),  // right
                            m_PositionVector.m_y-m_TexQuad.halfheight()  // bottom
                          );
}

