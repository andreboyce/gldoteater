#pragma once

#ifndef __C_GLVBO_H
#define __C_GLVBO_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include <queue>
#include "../OpenGLUtility/OpenGLUtility.h"
#include "../C_GLTextureManager/C_GLTextureManager.h" // ERROR_CNLT
#include "../../Astar/AStar2d.h"

#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB  0x88E4


class C_GLVBO
{

   typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC)    ( GLenum target, GLuint buffer         );
   typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) ( GLsizei n,     const GLuint *buffers );
   typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC)    ( GLsizei n,     GLuint *buffers       );
   typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC)    ( GLenum target, int size, const GLvoid *data, GLenum usage );

   public:
      C_GLVBO():
      glGenBuffersARB(    NULL ),
      glBindBufferARB(    NULL ),
      glBufferDataARB(    NULL ),
      glDeleteBuffersARB( NULL ),
      m_Mesh()
      {}

      ~C_GLVBO(){ Destroy(); }

      bool Build( AStar::AStar2d::TileVector2D& tiles, float x, float y, float z );
      void LoadVerticiesIntoMesh( AStar::AStar2d::TileVector2D& tiles, float x, float y, float z );
      void BuildVBOs();
      void Draw();
      void Destroy();
   protected:

      PFNGLGENBUFFERSARBPROC    glGenBuffersARB;    // VBO Name Generation Procedure
      PFNGLBINDBUFFERARBPROC    glBindBufferARB;    // VBO Bind Procedure
      PFNGLBUFFERDATAARBPROC    glBufferDataARB;    // VBO Data Loading Procedure
      PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB; // VBO Deletion Procedure

      CMesh<unsigned short> m_Mesh;

   private:
};

#endif

