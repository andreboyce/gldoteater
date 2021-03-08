#pragma once

#ifndef __C_glTriangleStrip_H
#define __C_glTriangleStrip_H

#include "../C_GLTexQuad/C_GLTexQuad.h"
#include "../C_GLVBO/C_GLVBO.h"
#include "../OpenGLUtility/OpenGLUtility.h"
#include <vector>

class C_glTriangleStrip
{
   public:
      C_glTriangleStrip(){}
      ~C_glTriangleStrip(){}
      bool Add( const C_GLTexQuad& rhs );
   protected:
      std::vector<C_Vert> m_Verts;
   private:
};

#endif
