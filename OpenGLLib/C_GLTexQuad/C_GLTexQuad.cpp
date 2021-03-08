#include "C_GLTexQuad.h"

C_GLTexQuad::C_GLTexQuad( const C_GLTexQuad& rhs )
{
   texture     = rhs.texture;
   x           = rhs.x;
   y           = rhs.y;
   z           = rhs.z;
   size_x      = rhs.size_x;
   size_y      = rhs.size_y;
   halfsize_x  = rhs.halfsize_x;
   halfsize_y  = rhs.halfsize_y;

   Vertices[0] = rhs.Vertices[0];
   Vertices[1] = rhs.Vertices[1];
   Vertices[2] = rhs.Vertices[2];
   Vertices[3] = rhs.Vertices[3];

   dVertices[0] = rhs.dVertices[0];
   dVertices[1] = rhs.dVertices[1];
   dVertices[2] = rhs.dVertices[2];
   dVertices[3] = rhs.dVertices[3];
}

C_GLTexQuad::C_GLTexQuad()
{
   texture     = 0xFFFFFF;
   x           = 0;
   y           = 0;
   z           = 0;
   size_x      = 1.0f;
   size_y      = 1.0f;
   halfsize_x  = 0.5f;
   halfsize_y  = 0.5f;

   Vertices[0].x = -0.5; Vertices[0].y = -0.5;  Vertices[0].z = 0.0;//  {-0.5f,+0.5f}*-----*{+0.5f,+0.5f}
   Vertices[1].x = +0.5; Vertices[1].y = -0.5;  Vertices[1].z = 0.0;//               |     |
   Vertices[2].x = +0.5; Vertices[2].y = +0.5;  Vertices[2].z = 0.0;//               |     |
   Vertices[3].x = -0.5; Vertices[3].y = +0.5;  Vertices[3].z = 0.0;//  {-0.5f,-0.5f}*-----*{+0.5f,-0.5f}

//   Vertices[0].x = -1.0f; Vertices[0].y = -1.0f;  Vertices[0].z = 0.0f;//  {-1.0f,+1.0f}*-----*{+0.5f,+1.0f}
//   Vertices[1].x = +1.0f; Vertices[1].y = -1.0f;  Vertices[1].z = 0.0f;//               |     |
//   Vertices[2].x = +1.0f; Vertices[2].y = +1.0f;  Vertices[2].z = 0.0f;//               |     |
//   Vertices[3].x = -1.0f; Vertices[3].y = +1.0f;  Vertices[3].z = 0.0f;//  {-1.0f,-1.0f}*-----*{+0.5f,-1.0f}

   dVertices[0].x = Vertices[0].x*size_x; dVertices[0].y = Vertices[0].y*size_y; dVertices[0].z = Vertices[0].z;
   dVertices[1].x = Vertices[1].x*size_x; dVertices[1].y = Vertices[1].y*size_y; dVertices[1].z = Vertices[1].z;
   dVertices[2].x = Vertices[2].x*size_x; dVertices[2].y = Vertices[2].y*size_y; dVertices[2].z = Vertices[2].z;
   dVertices[3].x = Vertices[3].x*size_x; dVertices[3].y = Vertices[3].y*size_y; dVertices[3].z = Vertices[3].z;
}

C_GLTexQuad::C_GLTexQuad( double new_size_x, double new_size_y )
{
   texture     = 0xFFFFFF;
   x           = 0;
   y           = 0;
   z           = 0;
   size_x      = 1.0f;
   size_y      = 1.0f;
   halfsize_x  = 0.5f;
   halfsize_y  = 0.5f;

   Vertices[0].x = -0.5; Vertices[0].y = -0.5;  Vertices[0].z = 0.0;//  {-0.5f,+0.5f}*-----*{+0.5f,+0.5f}
   Vertices[1].x = +0.5; Vertices[1].y = -0.5;  Vertices[1].z = 0.0;//               |     |
   Vertices[2].x = +0.5; Vertices[2].y = +0.5;  Vertices[2].z = 0.0;//               |     |
   Vertices[3].x = -0.5; Vertices[3].y = +0.5;  Vertices[3].z = 0.0;//  {-0.5f,-0.5f}*-----*{+0.5f,-0.5f}
   SetSize( new_size_x, new_size_y );
}

C_GLTexQuad::C_GLTexQuad( GLuint newtexture, double new_size_x, double new_size_y )
{
   texture     = newtexture;
   x           = 0;
   y           = 0;
   z           = 0;
   size_x      = 1.0f;
   size_y      = 1.0f;
   halfsize_x  = 0.5f;
   halfsize_y  = 0.5f;

   Vertices[0].x = -0.5; Vertices[0].y = -0.5;  Vertices[0].z = 0.0;//  {-0.5f,+0.5f}*-----*{+0.5f,+0.5f}
   Vertices[1].x = +0.5; Vertices[1].y = -0.5;  Vertices[1].z = 0.0;//               |     |
   Vertices[2].x = +0.5; Vertices[2].y = +0.5;  Vertices[2].z = 0.0;//               |     |
   Vertices[3].x = -0.5; Vertices[3].y = +0.5;  Vertices[3].z = 0.0;//  {-0.5f,-0.5f}*-----*{+0.5f,-0.5f}
   SetSize( new_size_x, new_size_y );
}

C_GLTexQuad::~C_GLTexQuad()
{
}
     
void C_GLTexQuad::ApplyTexture( GLuint text )
{
   texture = text;
}

void C_GLTexQuad::SetSize( double new_size_x, double new_size_y )
{
   size_x     = new_size_x;
   size_y     = new_size_y;
   halfsize_x = new_size_x*0.5f;
   halfsize_y = new_size_y*0.5f;
//   Vertices[0].x = -1.0f; Vertices[0].y = -1.0f;  Vertices[0].z = 0.0f;//  {-1.0f,+1.0f}*-----*{+0.5f,+1.0f}
//   Vertices[1].x = +1.0f; Vertices[1].y = -1.0f;  Vertices[1].z = 0.0f;//               |     |
//   Vertices[2].x = +1.0f; Vertices[2].y = +1.0f;  Vertices[2].z = 0.0f;//               |     |
//   Vertices[3].x = -1.0f; Vertices[3].y = +1.0f;  Vertices[3].z = 0.0f;//  {-1.0f,-1.0f}*-----*{+0.5f,-1.0f}

   dVertices[0].x = Vertices[0].x*size_x; dVertices[0].y = Vertices[0].y*size_y; dVertices[0].z = Vertices[0].z;
   dVertices[1].x = Vertices[1].x*size_x; dVertices[1].y = Vertices[1].y*size_y; dVertices[1].z = Vertices[1].z;
   dVertices[2].x = Vertices[2].x*size_x; dVertices[2].y = Vertices[2].y*size_y; dVertices[2].z = Vertices[2].z;
   dVertices[3].x = Vertices[3].x*size_x; dVertices[3].y = Vertices[3].y*size_y; dVertices[3].z = Vertices[3].z;

//::glVertex3d( (Vertices[0].x*size_x)+x, (Vertices[0].y*size_y)+y, Vertices[0].z+z );
//::glVertex3d( (Vertices[1].x*size_x)+x, (Vertices[1].y*size_y)+y, Vertices[1].z+z );
//::glVertex3d( (Vertices[2].x*size_x)+x, (Vertices[2].y*size_y)+y, Vertices[2].z+z );
//::glVertex3d( (Vertices[3].x*size_x)+x, (Vertices[3].y*size_y)+y, Vertices[3].z+z );
}

void C_GLTexQuad::Draw( double x, double y, double z )
{
   //::glColor4f( Vertices[0].red, Vertices[0].green, Vertices[0].blue, Vertices[0].alpha ); 
   ::glTexCoord2d( 0.0, 0.0 ); ::glVertex3d( dVertices[0].x+x, dVertices[0].y+y, dVertices[0].z+z );
   //::glColor4f( Vertices[1].red, Vertices[1].green, Vertices[1].blue, Vertices[1].alpha ); 
   ::glTexCoord2d( 1.0, 0.0 ); ::glVertex3d( dVertices[1].x+x, dVertices[1].y+y, dVertices[1].z+z );
   //::glColor4f( Vertices[2].red, Vertices[2].green, Vertices[2].blue, Vertices[2].alpha ); 
   ::glTexCoord2d( 1.0, 1.0 ); ::glVertex3d( dVertices[2].x+x, dVertices[2].y+y, dVertices[2].z+z );
   //::glColor4f( Vertices[3].red, Vertices[3].green, Vertices[3].blue, Vertices[3].alpha ); 
   ::glTexCoord2d( 0.0, 1.0 ); ::glVertex3d( dVertices[3].x+x, dVertices[3].y+y, dVertices[3].z+z );
}

void C_GLTexQuad::Drawc( double x, double y, double z )
{
   ::glColor4f( Vertices[0].red, Vertices[0].green, Vertices[0].blue, Vertices[0].alpha ); 
   ::glTexCoord2d( 0.0, 0.0 ); ::glVertex3d( dVertices[0].x+x, dVertices[0].y+y, dVertices[0].z+z );
   ::glColor4f( Vertices[1].red, Vertices[1].green, Vertices[1].blue, Vertices[1].alpha ); 
   ::glTexCoord2d( 1.0, 0.0 ); ::glVertex3d( dVertices[1].x+x, dVertices[1].y+y, dVertices[1].z+z );
   ::glColor4f( Vertices[2].red, Vertices[2].green, Vertices[2].blue, Vertices[2].alpha ); 
   ::glTexCoord2d( 1.0, 1.0 ); ::glVertex3d( dVertices[2].x+x, dVertices[2].y+y, dVertices[2].z+z );
   ::glColor4f( Vertices[3].red, Vertices[3].green, Vertices[3].blue, Vertices[3].alpha ); 
   ::glTexCoord2d( 0.0, 1.0 ); ::glVertex3d( dVertices[3].x+x, dVertices[3].y+y, dVertices[3].z+z );
}

void C_GLTexQuad::Draw( double x, double y, double z, C_TexCoord TexCoord[4] )
{
   //::glColor4f( Vertices[0].red, Vertices[0].green, Vertices[0].blue, Vertices[0].alpha ); 
   ::glTexCoord2d( TexCoord[0].U, TexCoord[0].V ); ::glVertex3d( dVertices[0].x+x, dVertices[0].y+y, dVertices[0].z+z );
   //::glColor4f( Vertices[1].red, Vertices[1].green, Vertices[1].blue, Vertices[1].alpha ); 
   ::glTexCoord2d( TexCoord[1].U, TexCoord[1].V ); ::glVertex3d( dVertices[1].x+x, dVertices[1].y+y, dVertices[1].z+z );
   //::glColor4f( Vertices[2].red, Vertices[2].green, Vertices[2].blue, Vertices[2].alpha ); 
   ::glTexCoord2d( TexCoord[2].U, TexCoord[2].V ); ::glVertex3d( dVertices[2].x+x, dVertices[2].y+y, dVertices[2].z+z );
   //::glColor4f( Vertices[3].red, Vertices[3].green, Vertices[3].blue, Vertices[3].alpha ); 
   ::glTexCoord2d( TexCoord[3].U, TexCoord[3].V ); ::glVertex3d( dVertices[3].x+x, dVertices[3].y+y, dVertices[3].z+z );
}

void C_GLTexQuad::GetVerticies( C_Vert Verticies[4], double x, double y, double z )
{
   Verticies[0].x = dVertices[0].x+x;
   Verticies[0].y = dVertices[0].y+y;
   Verticies[0].z = dVertices[0].z+z;

   Verticies[1].x = dVertices[1].x+x;
   Verticies[1].y = dVertices[1].y+y;
   Verticies[1].z = dVertices[1].z+z;

   Verticies[2].x = dVertices[2].x+x;
   Verticies[2].y = dVertices[2].y+y;
   Verticies[2].z = dVertices[2].z+z;

   Verticies[3].x = dVertices[3].x+x;
   Verticies[3].y = dVertices[3].y+y;
   Verticies[3].z = dVertices[3].z+z;
}

void C_GLTexQuad::Draw( double x, double y, double z, const Structs::fPoint Point[4] )
{
   //::glColor4f( Vertices[0].red, Vertices[0].green, Vertices[0].blue, Vertices[0].alpha ); 
   ::glTexCoord2d( Point[0].m_x, Point[0].m_y ); ::glVertex3d( dVertices[0].x+x, dVertices[0].y+y, dVertices[0].z+z );
   //::glColor4f( Vertices[1].red, Vertices[1].green, Vertices[1].blue, Vertices[1].alpha ); 
   ::glTexCoord2d( Point[1].m_x, Point[1].m_y ); ::glVertex3d( dVertices[1].x+x, dVertices[1].y+y, dVertices[1].z+z );
   //::glColor4f( Vertices[2].red, Vertices[2].green, Vertices[2].blue, Vertices[2].alpha ); 
   ::glTexCoord2d( Point[2].m_x, Point[2].m_y ); ::glVertex3d( dVertices[2].x+x, dVertices[2].y+y, dVertices[2].z+z );
   //::glColor4f( Vertices[3].red, Vertices[3].green, Vertices[3].blue, Vertices[3].alpha ); 
   ::glTexCoord2d( Point[3].m_x, Point[3].m_y ); ::glVertex3d( dVertices[3].x+x, dVertices[3].y+y, dVertices[3].z+z );
}

void C_GLTexQuad::Draw2( double x, double y, double z, const Structs::Point Point[4] )
{
   ::glColor4f( Vertices[0].red, Vertices[0].green, Vertices[0].blue, Vertices[0].alpha ); ::glTexCoord2d( Point[0].x, Point[0].y ); ::glVertex3d( dVertices[0].x+x, dVertices[0].y+y, dVertices[0].z+z );
   ::glColor4f( Vertices[1].red, Vertices[1].green, Vertices[1].blue, Vertices[1].alpha ); ::glTexCoord2d( Point[1].x, Point[1].y ); ::glVertex3d( dVertices[1].x+x, dVertices[1].y+y, dVertices[1].z+z );
   ::glColor4f( Vertices[2].red, Vertices[2].green, Vertices[2].blue, Vertices[2].alpha ); ::glTexCoord2d( Point[2].x, Point[2].y ); ::glVertex3d( dVertices[2].x+x, dVertices[2].y+y, dVertices[2].z+z );
   ::glColor4f( Vertices[3].red, Vertices[3].green, Vertices[3].blue, Vertices[3].alpha ); ::glTexCoord2d( Point[3].x, Point[3].y ); ::glVertex3d( dVertices[3].x+x, dVertices[3].y+y, dVertices[3].z+z );
}

void C_GLTexQuad::Draw2( double x, double y, double z, std::deque<Structs::Point> Point )
{
   ::glColor4f( Vertices[0].red, Vertices[0].green, Vertices[0].blue, Vertices[0].alpha ); ::glTexCoord2d( Point[0].x, Point[0].y ); ::glVertex3d( dVertices[0].x+x, dVertices[0].y+y, dVertices[0].z+z );
   ::glColor4f( Vertices[1].red, Vertices[1].green, Vertices[1].blue, Vertices[1].alpha ); ::glTexCoord2d( Point[1].x, Point[1].y ); ::glVertex3d( dVertices[1].x+x, dVertices[1].y+y, dVertices[1].z+z );
   ::glColor4f( Vertices[2].red, Vertices[2].green, Vertices[2].blue, Vertices[2].alpha ); ::glTexCoord2d( Point[2].x, Point[2].y ); ::glVertex3d( dVertices[2].x+x, dVertices[2].y+y, dVertices[2].z+z );
   ::glColor4f( Vertices[3].red, Vertices[3].green, Vertices[3].blue, Vertices[3].alpha ); ::glTexCoord2d( Point[3].x, Point[3].y ); ::glVertex3d( dVertices[3].x+x, dVertices[3].y+y, dVertices[3].z+z );
}

void C_GLTexQuad::Draw3( double x, double y, double z )
{
  ::glColor4f( Vertices[0].red, Vertices[0].green, Vertices[0].blue, Vertices[0].alpha ); ::glTexCoord2d( 0.0, 0.0 ); ::glVertex3d( dVertices[0].x+x, dVertices[0].y+y, dVertices[0].z+z );
  ::glColor4f( Vertices[1].red, Vertices[1].green, Vertices[1].blue, Vertices[1].alpha ); ::glTexCoord2d( 1.0, 0.0 ); ::glVertex3d( dVertices[1].x+x, dVertices[1].y+y, dVertices[1].z+z );
  ::glColor4f( Vertices[2].red, Vertices[2].green, Vertices[2].blue, Vertices[2].alpha ); ::glTexCoord2d( 1.0, 1.0 ); ::glVertex3d( dVertices[2].x+x, dVertices[2].y+y, dVertices[2].z+z );
  ::glColor4f( Vertices[3].red, Vertices[3].green, Vertices[3].blue, Vertices[3].alpha ); ::glTexCoord2d( 0.0, 1.0 ); ::glVertex3d( dVertices[3].x+x, dVertices[3].y+y, dVertices[3].z+z );
}

void C_GLTexQuad::Draw4( double x, double y, double z )
{
  ::glTexCoord2d( 0.0, 1.0 ); ::glVertex3d( dVertices[0].x+x, dVertices[0].y+y, dVertices[0].z+z );
  ::glTexCoord2d( 1.0, 1.0 ); ::glVertex3d( dVertices[1].x+x, dVertices[1].y+y, dVertices[1].z+z );
  ::glTexCoord2d( 1.0, 0.0 ); ::glVertex3d( dVertices[2].x+x, dVertices[2].y+y, dVertices[2].z+z );
  ::glTexCoord2d( 0.0, 0.0 ); ::glVertex3d( dVertices[3].x+x, dVertices[3].y+y, dVertices[3].z+z );
}


Structs::fRect3D C_GLTexQuad::Rect( void )
{
//  {-0.5f,+0.5f}*-----*{+0.5f,+0.5f}
//               |     |
//               |     |
//  {-0.5f,-0.5f}*-----*{+0.5f,-0.5f}

//   float _left, float _top, float _right, float _bottom, float z
   return Structs::fRect3D( dVertices[0].x, dVertices[2].y, dVertices[2].x, dVertices[0].y, dVertices[0].z );
}

void C_GLTexQuad::SetRect( Structs::fRect3D rect )
{
   dVertices[0].x = rect.left; 
   dVertices[2].y = rect.top; 
   dVertices[2].x = rect.right; 
   dVertices[0].y = rect.bottom; 
   dVertices[0].z = rect.m_z;
}

void C_GLTexQuad::SetVertexColor( short index, double red, double green, double blue, double alpha )
{
   Vertices[index].red   = red;
   Vertices[index].green = green;
   Vertices[index].blue  = blue;
   Vertices[index].alpha = alpha;
}

void C_GLTexQuad::SetVertexPosition( short index, double x, double y, double z )
{
   Vertices[index].x = x;
   Vertices[index].y = y;
   Vertices[index].z = z;
}

