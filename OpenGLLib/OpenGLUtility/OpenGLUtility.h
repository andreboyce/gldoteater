#pragma once

#ifndef __OpenGLUtility_H
#define __OpenGLUtility_H

#include <windows.h>
#include <string>
#include <vector>
#include <gl/glu.h>
#include <gl/glext.h>


#include "../../Structs/Structs.h"
#include "../../MathVector/Vector3D/Vector3D.h"
#include "../../MathVector/Vector2D/Vector2D.h"

#ifndef ERROR_CNLT
#define ERROR_CNLT 0xFFFFFF
#endif

class C_TexCoord
{
   public:
      C_TexCoord(): U( 0.0 ), V( 0.0 ){}
      C_TexCoord( const C_TexCoord& rhs ): U( rhs.U ), V( rhs.V ){}
      C_TexCoord( float _U, float _V ): U( _U ), V( _V ){}
      C_TexCoord& operator = ( const C_TexCoord& rhs )
      {
         U = rhs.U;
         V = rhs.V;
         return *this;
      }
	  C_TexCoord& operator = ( const Structs::Point& rhs )
      {
         U = rhs.x;
         V = rhs.y;
         return *this;
      }

      float U, V;
      void glTexCoord2f( void )
      {
         ::glTexCoord2f( U, V );
      }
};

void flip_quad_vertically( C_TexCoord TexCoords[4] );
void flip_quad_horizontially( C_TexCoord TexCoords[4] );
void rotate_quad_clockwise( C_TexCoord TexCoords[4] );
void rotate_quad_anticlockwise( C_TexCoord TexCoords[4] );

class C_Vert
{
   public:
      C_Vert(): x( 0.0 ), y( 0.0 ), z( 0.0 ){}
      C_Vert( const C_Vert& rhs ): x( rhs.x ), y( rhs.y ), z( rhs.y ){}
      C_Vert( float _x, float _y, float _z ): x( _x ), y( _y ), z( _z ){}
      C_Vert& operator = ( const C_Vert& rhs )
      {
         x = rhs.x;
         y = rhs.y;
         z = rhs.z;
         return *this;
      }
      float x, y, z;
      void glVertex2f( void )
      {
         ::glVertex2f( x, y );
      }
      void glVertex3f( void )
      {
         ::glVertex3f( x, y, z );
      }
};

class C_Vert2D
{
   public:
      C_Vert2D(): x( 0.0 ), y( 0.0 ){}
      C_Vert2D( const C_Vert2D& rhs ): x( rhs.x ), y( rhs.y ){}
      C_Vert2D( float _x, float _y, float _z ): x( _x ), y( _y ){}
      C_Vert2D& operator = ( const C_Vert2D& rhs )
      {
         x = rhs.x;
         y = rhs.y;
         return *this;
      }
      float x, y;
      void glVertex2f( void )
      {
         ::glVertex2f( x, y );
      }
};

template< typename IndexType >
class CMesh
{
   public:
      CMesh():
         m_VertexCount( 0 ),
         m_pVertices( NULL ),
         m_pTexCoords( NULL ),
         m_nTextureId( ERROR_CNLT ),
         m_VBOVertices( 0 ),
         m_VBOTexCoords( 0 ),
         m_VBOIndices( 0 ),
         m_IndexCount( 0 )
         {}
      ~CMesh(){ Destroy(); }
      void Destroy()
      {
         if( m_VertexCount > 1 )
         {
            delete[] m_pVertices;
            delete[] m_pTexCoords;
         }
         else
         {
            delete m_pVertices;
            delete m_pTexCoords;
         }
         m_pVertices    = NULL;
         m_pTexCoords   = NULL;
         m_nTextureId   = ERROR_CNLT;
         m_VBOIndices   = 0;
         m_VBOTexCoords = 0;
         m_VBOVertices  = 0;
         m_IndexCount   = 0;
         m_Indices.clear();
      }

      // Mesh Data
      GLuint     m_IndexCount;       // Index Count
      GLuint     m_VertexCount;      // Vertex Count
      C_Vert     *m_pVertices;       // Vertex Data
      C_TexCoord *m_pTexCoords;      // Texture Coordinates
      GLuint     m_nTextureId;       // Texture ID
      typedef IndexType MeshIndexType;
      std::vector<IndexType> m_Indices; // Indices to vertex data

      // Vertex Buffer Object Names
      GLuint m_VBOVertices;  // Vertex VBO Name
      GLuint m_VBOTexCoords; // Texture Coordinate VBO Name
      GLuint m_VBOIndices;   // Indices VBO Name
};

template< typename IndexType >
class CMesh2D
{
   public:
      CMesh2D():
         m_VertexCount( 0 ),
         m_pVertices( NULL ),
         m_pTexCoords( NULL ),
         m_nTextureId( ERROR_CNLT ),
         m_VBOVertices( 0 ),
         m_VBOTexCoords( 0 ),
         m_VBOIndices( 0 ),
         m_IndexCount( 0 )
         {}
      ~CMesh2D(){ Destroy(); }
      void Destroy()
      {
         if( m_VertexCount > 1 )
         {
            delete[] m_pVertices;
            delete[] m_pTexCoords;
         }
         else
         {
            delete m_pVertices;
            delete m_pTexCoords;
         }
         m_pVertices    = NULL;
         m_pTexCoords   = NULL;
         m_nTextureId   = ERROR_CNLT;
         m_VBOIndices   = 0;
         m_VBOTexCoords = 0;
         m_VBOVertices  = 0;
         m_IndexCount   = 0;
         m_Indices.clear();
      }
   
      void DestroyClientSideData( void )
      {
         if( m_VertexCount > 1 )
         {
            delete[] m_pVertices;
            delete[] m_pTexCoords;
         }
         else
         {
            delete m_pVertices;
            delete m_pTexCoords;
         }
         m_pVertices    = NULL;
         m_pTexCoords   = NULL;
         m_Indices.clear();
      }

      // Mesh Data
      GLuint     m_IndexCount;       // Index  Count
      GLuint     m_VertexCount;      // Vertex Count
      C_Vert2D   *m_pVertices;       // Vertex Data
      C_TexCoord *m_pTexCoords;      // Texture Coordinates
      GLuint     m_nTextureId;       // Texture ID
      typedef IndexType MeshIndexType;
      std::vector<IndexType> m_Indices; // Indices to vertex data

      // Vertex Buffer Object Names
      GLuint m_VBOVertices;  // Vertex VBO Name
      GLuint m_VBOTexCoords; // Texture Coordinate VBO Name
      GLuint m_VBOIndices;   // Indices VBO Name
};

std::string SetVSync( int interval = 0 );
std::vector<std::string> EnumGLExtensions( void );
Vector3D WindowToGLCoordinates( int y, int x );
Vector2D GLToWindowCoordinates( double y, double x, double z );

bool IsExtensionSupported( std::string TargetExtension );
bool IsExtensionSupported( char* szTargetExtension );

#endif
