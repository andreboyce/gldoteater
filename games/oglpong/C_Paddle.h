#ifndef __C_Paddle_H
#define __C_Paddle_H

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "../../C_HDC/C_MemoryDC/C_MemoryDC.h"
#include "../../Structs/Structs.h"
#include "../../OpenGLLib/C_GLTexQuad/C_GLTexQuad.h"
#include "../../MathVector/Vector2D/Vector2D.h"

class C_Paddle
{
   friend class MainWindow;
   public:
      C_Paddle( void ) : m_Z( -40 )
         {
            quadratic = gluNewQuadric(); // Create A Pointer To The Quadric Object ( NEW )

             m_TexQuad.SetSize( 1.0f, 1.0f );
//             m_TexQuad.ApplyTexture( m_TextureManager[texindex] );
             m_TexQuad.SetVertexColor( 0, 1, 1, 1, 0.80 );
             m_TexQuad.SetVertexColor( 1, 1, 1, 1, 0.80 );
             m_TexQuad.SetVertexColor( 2, 1, 1, 1, 0.80 );
             m_TexQuad.SetVertexColor( 3, 1, 1, 1, 0.80 );
         }

      C_Paddle( double PaddleWidth, double PaddleLength ) : m_Z( -40 )
         {
            quadratic = gluNewQuadric(); // Create A Pointer To The Quadric Object ( NEW )
            m_TexQuad.SetSize( PaddleWidth, PaddleLength );
            m_TexQuad.SetSize( PaddleWidth, PaddleLength );
//             m_TexQuad.ApplyTexture( m_TextureManager[texindex] );
            m_TexQuad.SetVertexColor( 0, 1, 1, 1, 0.80 );
            m_TexQuad.SetVertexColor( 1, 1, 1, 1, 0.80 );
            m_TexQuad.SetVertexColor( 2, 1, 1, 1, 0.80 );
            m_TexQuad.SetVertexColor( 3, 1, 1, 1, 0.80 );
         }

      ~C_Paddle( void ){}

      void OGLDraw( void );

      inline void SetLength( const double PaddleLength )
      {
         m_TexQuad.SetSize( m_TexQuad.width(), PaddleLength );
      }

      inline void SetWidth( const double PaddleWidth )
      {
         m_TexQuad.SetSize( PaddleWidth, m_TexQuad.height() );
      }

      GLUquadricObj *quadratic;
      C_GLTexQuad   m_TexQuad;
      Vector2D    m_PositionVector, m_VelocityVector, m_AccelerationVector;
      double        m_Z;

   protected:
   private:
};

#endif

