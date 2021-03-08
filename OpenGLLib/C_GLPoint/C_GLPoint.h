#pragma once

#ifndef __C_GLPoint_H
#define __C_GLPoint_H

#include <windows.h>
#include <gl/gl.h>
#include "../C_GLVertex/C_GLVertex.h"

class C_GLPoint
{

   public:

       C_GLPoint( void )
          { Pointsize = 5; }

      ~C_GLPoint( void )
          {}
         
       void SetSize( const short& newPointsize )
          {
             Pointsize = newPointsize;
          }

       const short& GetSize( void )
          {
             return Pointsize;
          }
          
       void inline Draw( float x, float y, float z )
          {
             ::glColor4f( Position.red, Position.green, Position.blue, Position.alpha );
             ::glVertex3f( x, y, z );
          }

       void inline SetVertexColor( float red, float green, float blue, float alpha )
          {
             Position.red   = red;
             Position.green = green;
             Position.blue  = blue;
             Position.alpha = alpha;
          }

   protected:

      C_GLVertex Position;
      short      Pointsize;

   private:

};

#endif
