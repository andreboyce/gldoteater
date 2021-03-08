#include "C_GLVBO.h"

void C_GLVBO::Destroy()
{
   m_Mesh.Destroy();
}

bool C_GLVBO::Build( AStar::AStar2d::TileVector2D& tiles, float x, float y, float z )
{
   Destroy();
	if( IsExtensionSupported( "GL_ARB_vertex_buffer_object" ) )
	{
      // Get Pointers To The GL Functions
      glGenBuffersARB    = (PFNGLGENBUFFERSARBPROC)    wglGetProcAddress( "glGenBuffersARB"    );
      glBindBufferARB    = (PFNGLBINDBUFFERARBPROC)    wglGetProcAddress( "glBindBufferARB"    );
      glBufferDataARB    = (PFNGLBUFFERDATAARBPROC)    wglGetProcAddress( "glBufferDataARB"    );
      glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress( "glDeleteBuffersARB" );
		// Load Vertex Data Into The Graphics Card Memory
      LoadVerticiesIntoMesh( tiles, x, y, z );
      BuildVBOs();
      return true;
	}
	return false;
}

void C_GLVBO::LoadVerticiesIntoMesh( AStar::AStar2d::TileVector2D& tiles, float x, float y, float z )
{
   if( !tiles.size() )
   {
      return;
   }
   if( !tiles[0].size() )
   {
      return;
   }

   int width            = tiles[0].size();
   int height           = tiles.size();
   m_Mesh.m_VertexCount = (int) ( width * height * 4 );           // 4 verts per quad
   m_Mesh.m_pVertices   = new C_Vert[ m_Mesh.m_VertexCount ];     // Allocate Vertex Data
   m_Mesh.m_pTexCoords  = new C_TexCoord[ m_Mesh.m_VertexCount ]; // Allocate Tex Coord Data

   float size_x = 1.0f;
   float size_y = 1.0f;
   C_Vert  Vertices[4];
   C_Vert dVertices[4];

   Vertices[0].x = -0.5; Vertices[0].y = -0.5;  Vertices[0].z = 0.0;//  {-0.5f,+0.5f}*-----*{+0.5f,+0.5f}
   Vertices[1].x = +0.5; Vertices[1].y = -0.5;  Vertices[1].z = 0.0;//               |     |
   Vertices[2].x = +0.5; Vertices[2].y = +0.5;  Vertices[2].z = 0.0;//               |     |
   Vertices[3].x = -0.5; Vertices[3].y = +0.5;  Vertices[3].z = 0.0;//  {-0.5f,-0.5f}*-----*{+0.5f,-0.5f}

   dVertices[0].x = Vertices[0].x*size_x; dVertices[0].y = Vertices[0].y*size_y; dVertices[0].z = Vertices[0].z;
   dVertices[1].x = Vertices[1].x*size_x; dVertices[1].y = Vertices[1].y*size_y; dVertices[1].z = Vertices[1].z;
   dVertices[2].x = Vertices[2].x*size_x; dVertices[2].y = Vertices[2].y*size_y; dVertices[2].z = Vertices[2].z;
   dVertices[3].x = Vertices[3].x*size_x; dVertices[3].y = Vertices[3].y*size_y; dVertices[3].z = Vertices[3].z;

   int Index = 0;

   for( ; x<width*size_x ; x+=size_x )
   {
      for( ; y<height*size_y ; y+=size_y )
      {
         for( int i=0; i<4 ; i++ )
         {
            m_Mesh.m_pVertices[Index].x = dVertices[i].x+x;
            m_Mesh.m_pVertices[Index].y = dVertices[i].y+y;
            m_Mesh.m_pVertices[Index].z = dVertices[i].z+z;
            Index++;
         }
      }
   }
}

void C_GLVBO::BuildVBOs()
{
   // Generate And Bind The Vertex Buffer
   glGenBuffersARB( 1, &m_Mesh.m_VBOVertices ); // Get A Valid Name
   glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_Mesh.m_VBOVertices ); // Bind The Buffer

   // Load The Data
   glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_Mesh.m_VertexCount*3*sizeof(float), 
                    m_Mesh.m_pVertices, GL_STATIC_DRAW_ARB );

   // Generate And Bind The Texture Coordinate Buffer
   glGenBuffersARB( 1, &m_Mesh.m_VBOTexCoords ); // Get A Valid Name
   glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_Mesh.m_VBOTexCoords ); // Bind The Buffer
   // Load The Data
   glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_Mesh.m_VertexCount*2*sizeof(float), 
                    m_Mesh.m_pTexCoords, GL_STATIC_DRAW_ARB );

   // Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card
   delete [] m_Mesh.m_pVertices;  m_Mesh.m_pVertices  = NULL;
   delete [] m_Mesh.m_pTexCoords; m_Mesh.m_pTexCoords = NULL;
}

void C_GLVBO::Draw()
{
   glEnableClientState( GL_VERTEX_ARRAY );        // Enable Vertex Arrays
   glEnableClientState( GL_TEXTURE_COORD_ARRAY ); // Enable Texture Coord Arrays

   // Set Pointers To Our Data
   if( glGenBuffersARB != NULL ) // this is only valid if vbo's are supported
   {
      glBindBufferARB(   GL_ARRAY_BUFFER_ARB, m_Mesh.m_VBOVertices  );
      glVertexPointer(   3, GL_FLOAT, 0, (char *) NULL              ); // Set The Vertex Pointer To The Vertex Buffer
      glBindBufferARB(   GL_ARRAY_BUFFER_ARB, m_Mesh.m_VBOTexCoords );
      glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL              ); // Set The TexCoord Pointer To The TexCoord Buffer
   }
   else
   {
      glVertexPointer(   3, GL_FLOAT, 0, m_Mesh.m_pVertices );  // Set The Vertex Pointer To Our Vertex Data
      glTexCoordPointer( 2, GL_FLOAT, 0, m_Mesh.m_pTexCoords ); // Set The Vertex Pointer To Our TexCoord Data
   }

   // Render
   glDrawArrays( GL_QUADS, 0, m_Mesh.m_VertexCount ); // Draw All Of The Triangles At Once

   // Disable Pointers
   glDisableClientState( GL_VERTEX_ARRAY );        // Disable Vertex Arrays
   glDisableClientState( GL_TEXTURE_COORD_ARRAY ); // Disable Texture Coord Arrays

}

