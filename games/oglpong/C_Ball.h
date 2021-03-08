#ifndef __C_Ball_H
#define __C_Ball_H

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "../../Structs/Structs.h"
#include "../../C_HDC/C_MemoryDC/C_MemoryDC.h"
#include "../../OpenGLLib/C_GLTexQuad/C_GLTexQuad.h"
#include "../../MathVector/Vector2D/Vector2D.h"

class C_Ball
{
   friend class MainWindow;

   public:
      class C_Particle
      {
         public:
            C_Particle( void ){}
            ~C_Particle(){}
         protected:
         private:
      };

      C_Ball( void ) : m_Radius( 0.0 ), m_Diameter( 0.0 ), m_Mass( 0.1 ), m_Z( -40 )
      {
         quadratic = ::gluNewQuadric(); // Create A Pointer To The Quadric Object ( NEW )
      }

      C_Ball( const double Radius, const double Mass = 0.1 ) : m_Radius( Radius ), m_Diameter( Radius*2.0 ), m_Mass( Mass ), m_Z( -40 )
      {
         quadratic = ::gluNewQuadric(); // Create A Pointer To The Quadric Object ( NEW )
      }

      ~C_Ball( void ){ ::gluDeleteQuadric( quadratic ); }

      void ApplyForce( double x, double y, double a_x, double a_y )
      {
         m_AccelerationVector.m_x        += x/m_Mass;
         m_AccelerationVector.m_y        += y/m_Mass;
         m_AngularAccelerationVector.m_x += a_x/m_Mass;
         m_AngularAccelerationVector.m_y += a_y/m_Mass;
      }

      void ApplyForce( Vector2D LinearForce, Vector2D AngularForce )
      {
         m_AccelerationVector        += LinearForce*(1.0/m_Mass);
         m_AngularAccelerationVector += AngularForce*(1.0/m_Mass);
      }

      inline void OGLDraw( void )
      {
         ::glRotatef( m_AngularPositionVector.m_x, 0, 0, -1 );
      }

      inline void DrawConstant( void )
      {
         ::gluQuadricNormals( quadratic, GLU_SMOOTH ); // Create Smooth Normals ( NEW )
         ::gluQuadricTexture( quadratic, GL_TRUE );    // Create Texture Coords ( NEW )
         ::gluDisk( quadratic, 0.0f, m_Radius, 32, 32 );
      }

      inline void DrawTrail( void )
      {
         ::glBegin( GL_QUADS );
//         ::glColor4f( 1, 1, 1, 0.5 ); ::glTexCoord2d( 0.0, 0.0 ); ::glVertex3d( -0.5, -0.250, 0 );
//         ::glColor4f( 1, 1, 1, 0.5 ); ::glTexCoord2d( 1.0, 0.0 ); ::glVertex3d( +0.5, -0.225, 0 );
//         ::glColor4f( 1, 1, 1, 0.5 ); ::glTexCoord2d( 1.0, 1.0 ); ::glVertex3d( +0.5, +0.225, 0 );
//         ::glColor4f( 1, 1, 1, 0.5 ); ::glTexCoord2d( 0.0, 1.0 ); ::glVertex3d( -0.5, +0.250, 0 );
         ::glColor4f( 0.1, 0.1, 0.1, 1.0 ); ::glVertex3d( -m_Radius*4.0, -m_Radius-0.05, 0 );
         ::glColor4f( 0.5, 0.5, 0.5, 1.0 ); ::glVertex3d( +m_Radius*4.0, -m_Radius, 0 );
         ::glColor4f( 0.5, 0.5, 0.5, 1.0 ); ::glVertex3d( +m_Radius*4.0, +m_Radius, 0 );
         ::glColor4f( 0.1, 0.1, 0.1, 1.0 ); ::glVertex3d( -m_Radius*4.0, +m_Radius+0.05, 0 );
         ::glEnd();
      }

      inline double Diameter( void ) const
      {
         return m_Diameter;
      }

      inline double Radius( void ) const
      {
         return m_Radius;
      }

      Vector2D    m_PositionVector, m_VelocityVector, m_AccelerationVector, m_AngularVelocityVector, m_AngularPositionVector, m_AngularAccelerationVector;
      GLUquadricObj *quadratic;
      double        m_Radius;
      double        m_Diameter;
      double        m_Mass;
      double        m_Z;

   protected:

   private:
};

#endif
