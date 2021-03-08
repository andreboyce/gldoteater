#pragma once

#ifndef __C_GLLines_H
#define __C_GLLines_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "../C_GLVertex/C_GLVertex.h"

class C_GLLine
{
   public:
   
       C_GLLine( void );
       C_GLLine( const C_GLLine& );
      ~C_GLLine( void );

       void Draw( float x, float y, float z );
       
       void inline SetX( float new_x_start, float new_x_end )
       {
          Start.x = new_x_start;
          End.x   = new_x_end;   
       }
       
       void inline SetY( float new_y_start, float new_y_end )
       {
          Start.y = new_y_start;
          End.y   = new_y_end;
       }
       
       void inline SetZ( float new_z_start, float new_z_end )
       {
          Start.z = new_z_start;
          End.z   = new_z_end;
       }
       
       void inline SetWidth( const short& new_width )
       {
          width = new_width;
       }
       
       const short & GetWidth( void )
       {
          return width;
       }       
      
   protected:
   
      short width;
      
      C_GLVertex Start, End;
   
   private:
   
};

#endif

