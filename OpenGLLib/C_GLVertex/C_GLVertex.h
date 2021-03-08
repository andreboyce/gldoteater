#pragma once

#ifndef __C_GLVertex_H
#define __C_GLVertex_H

class C_GLVertex
{
   friend class C_GLTexQuad;
   friend class C_GLTexTriangle;
   friend class C_GLLine;
   friend class C_GLPoint;   
   
   public:
   
       C_GLVertex( void );
       C_GLVertex( const C_GLVertex& C_GLVertexRHS );
      ~C_GLVertex( void );
      
   protected:
   
      float x;
      float y;
      float z;      
      float red;
      float green;
      float blue;
      float alpha;                  
      
   private:
   
};

#endif
