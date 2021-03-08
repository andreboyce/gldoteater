#ifndef __C_GLTexTriangle_H
#define __C_GLTexTriangle_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "..\C_GLVertex\C_GLVertex.h"

#define C_GLTexTriangle_NO_OF_VERTICIES 3

class C_GLTexTriangle
{
   public:
   
       C_GLTexTriangle( void );
      ~C_GLTexTriangle( void );
      
      void  ApplyTexture( GLuint text);
      void  Draw( float, float, float );
      void  SetVertexColor( short, float, float, float, float );
      void  SetVertexPosition( short, float, float, float );
      
      void inline Bind( const GLuint& newtexture )
      {
         texture = newtexture;
         ::glBindTexture( GL_TEXTURE_2D, texture );
      }

      void inline Bind( void )
      {
         ::glBindTexture( GL_TEXTURE_2D, texture );
      }
      
      bool inline FlipUpsideDown( void )
      {
         (flip) ? flip = false : flip = true;
      }
      
      C_GLTexTriangle& operator = ( const C_GLTexTriangle& C_GLTexTriangleRHS )
      {
      
         a       = C_GLTexTriangleRHS.a;
         b       = C_GLTexTriangleRHS.b;
         c       = C_GLTexTriangleRHS.c;      
         flip    = C_GLTexTriangleRHS.flip;
         texture = C_GLTexTriangleRHS.texture;
         
         for( int i=0; i < C_GLTexTriangle_NO_OF_VERTICIES ; i++ )
         {
            Vertices[i] = C_GLTexTriangleRHS.Vertices[i];
         }
         
         return *this;      
      }
   
   protected:
      
      float a;
      float b;
      float c;
      
      bool  flip;
      
      GLuint     texture;
      
      C_GLVertex Vertices[C_GLTexTriangle_NO_OF_VERTICIES];
      
   private:
   
};

#endif
