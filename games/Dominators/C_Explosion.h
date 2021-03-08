#pragma once

#ifndef __C_Explosion_H
#define __C_Explosion_H

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "../../Structs/Structs.h"
#include "../../errorutil/errorutil.h"
#include "../../OpenGLLib/C_GLTexQuad/C_GLTexQuad.h"
#include "../../MathVector/Vector2D/Vector2D.h"
#include "../../OpenGLLib/C_GLTextureManager/C_GLTextureManager.h"
#include "../../TimeInterval/TimeInterval.h"

class C_Explosion
{
   public:
      C_Explosion( Vector2D PositionVector, double duration=100, float width=1.0f, float height=1.0f ) :
         m_duration( duration ), m_PositionVector( PositionVector )
         {
             m_TexQuad.SetSize( width, height );
             m_TexQuad.SetVertexColor( 0, 1, 1, 1, 0.80 );
             m_TexQuad.SetVertexColor( 1, 1, 1, 1, 0.80 );
             m_TexQuad.SetVertexColor( 2, 1, 1, 1, 0.80 );
             m_TexQuad.SetVertexColor( 3, 1, 1, 1, 0.80 );
         }
      C_Explosion( const C_Explosion &rhs ) :
         m_duration( rhs.m_duration ), m_PositionVector( rhs.m_PositionVector ), m_TexQuad( rhs.m_TexQuad )
         {}
      ~C_Explosion(){}

      Vector2D m_PositionVector; // the position of the explosion
      double m_duration; // how long the explosion will last
      float m_Z; // z index of explosion
      C_GLTexQuad m_TexQuad; // the quad that the explosion texture will be rendered on

   protected:

   private:
};

#endif

