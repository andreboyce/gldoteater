#pragma once

#ifndef __C_Shell_H
#define __C_Shell_H

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "../../C_HDC/C_MemoryDC/C_MemoryDC.h"
#include "../../Structs/Structs.h"
//#include "../../OpenGLLib/C_GLTexQuad/C_GLTexQuad.h"
#include "../../OpenGLLib/C_GLTexQuad/C_GLTexQuad.h"
#include "C_Projectile.h"

class C_Shell: public C_Projectile
{
   public:
      C_Shell( const C_Shell& rhs ) :
         C_Projectile( rhs ),
         m_TexQuad( rhs.m_TexQuad )
      {
      }
      C_Shell()
      {
         m_TexQuad.SetSize( 0.5f, 2.0f );
         m_TexQuad.SetVertexColor( 0, 1, 1, 1, 0.80 );
         m_TexQuad.SetVertexColor( 1, 1, 1, 1, 0.80 );
         m_TexQuad.SetVertexColor( 2, 1, 1, 1, 0.80 );
         m_TexQuad.SetVertexColor( 3, 1, 1, 1, 0.80 );

         m_PositionVector = Vector2D( 0.00, 0.00 );
         m_VelocityVector = Vector2D( 0.03, 0.03 );
      }
      ~C_Shell(){}

      void Draw();
      void Draw( const Structs::Point TextureCoordinates[4] );
      Structs::fRect Rect();
      C_GLTexQuad m_TexQuad;

   protected:
   private:
};

#endif

