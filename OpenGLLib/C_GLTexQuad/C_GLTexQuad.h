#pragma once

#ifndef __C_GLTexQuad_H
#define __C_GLTexQuad_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <queue>
#include <string>
#include <sstream>
#include "../../Structs/Structs.h"
#include "../C_GLVertex/C_GLVertex.h"
#include "../OpenGLUtility/OpenGLUtility.h"

#define C_GLTexQuad_NO_OF_VERTICIES 4

class C_GLTexQuad
{
   public:
      C_GLTexQuad( double new_size_x, double new_size_y );
      C_GLTexQuad( GLuint newtexture, double new_size_x, double new_size_y );
      C_GLTexQuad();
      C_GLTexQuad( const C_GLTexQuad& rhs );
     ~C_GLTexQuad();

      // regular members
      
      void  ApplyTexture( GLuint );
	  void  Drawc( double x, double y, double z );
      void  Draw( double, double, double );
      void  Draw( double, double, double, C_TexCoord TexCoord[4] );
	  void  Draw( double, double, double, const Structs::fPoint Point[4] );
      void  Draw2( double, double, double, const Structs::Point Point[4] );
      void  Draw2( double, double, double, std::deque<Structs::Point> );
      void  Draw3( double, double, double );
      void  Draw4( double x = 0, double y = 0, double z = 0 );
      void  SetSize( double, double );
      void  SetVertexColor(    short, double, double, double, double );
      void  SetVertexPosition( short, double, double, double );
      void  GetVerticies( C_Vert Verticies[4], double x, double y, double z );
      Structs::fRect3D Rect( void );
      void SetRect( Structs::fRect3D );
      // inline members
      // TODO (#1#): Error checking 
      void inline Bind( void )
      {
         ::glBindTexture( GL_TEXTURE_2D, texture );
      }

      void inline Bind( const GLuint newtexture )
      {
         texture = newtexture;
         ::glBindTexture( GL_TEXTURE_2D, texture );
      }

      void glBegin()
      {
         ::glBegin( GL_QUADS );
      }

      void glEnd()
      {
         ::glEnd();
      }

      double inline halfwidth( void ) const
         { return halfsize_x;}
         
      double inline halfheight( void ) const
         { return halfsize_y;}


      double inline width( void ) const
         { return size_x;}
         
      double inline height( void ) const
         { return size_y;}
         
      C_GLTexQuad& operator = ( const C_GLTexQuad& C_GLTexQuadRHS )
      {
          texture     = C_GLTexQuadRHS.texture;
          x           = C_GLTexQuadRHS.x;
          y           = C_GLTexQuadRHS.y;
          z           = C_GLTexQuadRHS.z;
          size_x      = C_GLTexQuadRHS.size_x;
          size_y      = C_GLTexQuadRHS.size_y;
          halfsize_x  = C_GLTexQuadRHS.halfsize_x;
          halfsize_y  = C_GLTexQuadRHS.halfsize_y;
          for( int i=0 ; i < C_GLTexQuad_NO_OF_VERTICIES ; i++ )
          {
             Vertices[i] = C_GLTexQuadRHS.Vertices[i];
          }
          
          return *this;
      }

      GLuint GetTexture( void ) const
      {
         return texture;
      }
      operator std::string( void )
      {
         std::ostringstream tmp;
         tmp << "{ x: " << x << ", ";
         tmp << " y: " << y << ", ";
         tmp << " z: " << z << ", ";
         tmp << " size_x: " << size_x << ", ";
         tmp << " size_y: " << size_y << ", ";
         tmp << " halfsize_x: " << halfsize_x << ", ";
         tmp << " halfsize_y: " << halfsize_y << " }";
         return tmp.str();
      }

   protected:
   
      GLuint     texture;
      double     x;
      double     y;
      double     z;
      double     size_x, halfsize_x;
      double     size_y, halfsize_y;
      C_GLVertex Vertices[ C_GLTexQuad_NO_OF_VERTICIES ];
      C_GLVertex dVertices[ C_GLTexQuad_NO_OF_VERTICIES ];                
     
   private:
   
};

#endif

