#pragma once

#ifndef __C_Tank_H
#define __C_Tank_H

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "../../Structs/Structs.h"
#include "../../errorutil/errorutil.h"
#include "../../OpenGLLib/C_GLTexQuad/C_GLTexQuad.h"
#include "../../MathVector/Vector2D/Vector2D.h"
#include "C_Shell.h"
#include "../../OpenGLLib/C_GLTextureManager/C_GLTextureManager.h"
#include "../../TimeInterval/TimeInterval.h"

class C_Tank
{
   friend class MainWindow;
   public:
      C_Tank( void ) : m_Z( -40 ), m_Life( 100.0 ), m_OrignalLife( m_Life )
         {
             m_TexQuad.SetSize( 1.0f, 1.0f );
             m_TexQuad.SetVertexColor( 0, 1, 1, 1, 0.80 );
             m_TexQuad.SetVertexColor( 1, 1, 1, 1, 0.80 );
             m_TexQuad.SetVertexColor( 2, 1, 1, 1, 0.80 );
             m_TexQuad.SetVertexColor( 3, 1, 1, 1, 0.80 );

             m_FireInterval.Begin();
             m_FireInterval.SetInterval( 2000 );
			 m_Shells.reserve(15);
         }

      C_Tank( double PaddleWidth, double PaddleLength, long interval = 2000, double life = 100.0 ) : m_Z( -60 ), m_Life( life ), m_OrignalLife( m_Life )
         {
            m_TexQuad.SetSize( PaddleWidth, PaddleLength );
            m_TexQuad.SetSize( PaddleWidth, PaddleLength );
            m_TexQuad.SetVertexColor( 0, 1, 1, 1, 0.80 );
            m_TexQuad.SetVertexColor( 1, 1, 1, 1, 0.80 );
            m_TexQuad.SetVertexColor( 2, 1, 1, 1, 0.80 );
            m_TexQuad.SetVertexColor( 3, 1, 1, 1, 0.80 );

            m_FireInterval.Begin();
            m_FireInterval.SetInterval( interval );
			m_Shells.reserve(15);
         }

      ~C_Tank( void ){}

      void Draw( void );
      void Draw( const Structs::Point TextureCoordinates[4] );
      void AddShell();
      void AddShell( Vector2D vel, Vector2D accel );
      Structs::fRect Rect();

      // Set the width of the sprite
      inline void SetLength( const double PaddleLength )
      {
         m_TexQuad.SetSize( m_TexQuad.width(), PaddleLength );
      }

      inline void SetWidth( const double PaddleWidth )
      {
         m_TexQuad.SetSize( PaddleWidth, m_TexQuad.height() );
      }

      void DrawShells( C_GLTextureManager &TextureManager );
      void DrawShells( C_GLTextureManager &TextureManager, const Structs::Point TextureCoordinates[4] );
      void UpdateShells( double deltatime, Structs::fRect boundary );

      double ReduceLife( double how_much_damage );
      bool   CheckForNoLife( double OrignalLife );
      bool   CheckForNoLife( void );

      void   SetLife( double new_life )
      {
         m_Life = m_OrignalLife = new_life;
      }

      C_GLTexQuad   m_TexQuad;
      Vector2D      m_PositionVector,     // Position
                    m_VelocityVector,     // Velocity
                    m_AccelerationVector; // Alleceration

      double        m_Z,                 // z position
                    m_Life,              // percentage of life remaining
                    m_OrignalLife;       // orignal percentage of life

      std::vector<C_Shell> m_Shells;

      TimeInterval m_FireInterval; // space of time between which the tank can fire a shell


   protected:
   private:
};

#endif

