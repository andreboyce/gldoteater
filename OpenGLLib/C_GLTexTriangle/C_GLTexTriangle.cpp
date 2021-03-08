#include "C_GLTexTriangle.h"

C_GLTexTriangle::C_GLTexTriangle( void )
{
   a = 1.0f;
   b = 1.0f;
   c = 1.0f;
   
   flip = 0;
   texture = 0xFFFFFF;
   
   Vertices[0].x = +1.0; Vertices[0].y = +1.0;  Vertices[0].z = 0; //	       /|(1)
   Vertices[1].x = -1.0; Vertices[1].y = -1.0;  Vertices[1].z = 0; //	   a /  |b
   Vertices[2].x = +1.0; Vertices[2].y = -1.0;  Vertices[2].z = 0; //	(2)/____|(3)
                                                                   //        c
}

C_GLTexTriangle::~C_GLTexTriangle( void )
{
}

void C_GLTexTriangle::ApplyTexture( GLuint text )
{
   texture = text;
}

void C_GLTexTriangle::Draw( float x, float y, float z )
{
   ::glBindTexture( GL_TEXTURE_2D, texture );
   ::glBegin( GL_TRIANGLES );
   
   // TODO (#1#): fix this mess
   
   if( !flip )
   {
      ::glColor4f( Vertices[0].red, Vertices[0].green, Vertices[0].blue, Vertices[0].alpha ); ::glTexCoord2d( 1.0, 1.0 ); ::glVertex3d( Vertices[0].x+x, Vertices[0].y+y, Vertices[0].z+z ); //	       /|(1)
      ::glColor4f( Vertices[1].red, Vertices[1].green, Vertices[1].blue, Vertices[1].alpha ); ::glTexCoord2d( 0.0, 0.0 ); ::glVertex3d( Vertices[1].x+x, Vertices[1].y+y, Vertices[1].z+z ); //	   a /  |b
      ::glColor4f( Vertices[2].red, Vertices[2].green, Vertices[2].blue, Vertices[2].alpha ); ::glTexCoord2d( 1.0, 0.0 ); ::glVertex3d( Vertices[2].x+x, Vertices[2].y+y, Vertices[2].z+z ); //	(2)/____|(3)
                                                                                                                                                                                             //      c
   }
   else
   {                                                                                                                                                                                         //     __c__
      ::glColor4f( Vertices[0].red, Vertices[0].green, Vertices[0].blue, Vertices[0].alpha ); ::glTexCoord2d( 1.0, 1.0 ); ::glVertex3d( Vertices[0].x+x, Vertices[0].y+y, Vertices[0].z+z ); //	(2)|    / (1)
      ::glColor4f( Vertices[1].red, Vertices[1].green, Vertices[1].blue, Vertices[1].alpha ); ::glTexCoord2d( 0.0, 1.0 ); ::glVertex3d( Vertices[1].x+x, Vertices[1].y+y, Vertices[1].z+z ); //	  b|  /a
      ::glColor4f( Vertices[2].red, Vertices[2].green, Vertices[2].blue, Vertices[2].alpha ); ::glTexCoord2d( 0.0, 0.0 ); ::glVertex3d( Vertices[2].x+x, Vertices[2].y+y, Vertices[2].z+z ); //	(3)|/
   }
   
   ::glEnd();
}

void C_GLTexTriangle::SetVertexColor( short index, float red, float green, float blue, float alpha )
{
   if( index < 4 )
   {
      Vertices[index].red   = red;
      Vertices[index].green = green;
      Vertices[index].blue  = blue;
      Vertices[index].alpha = alpha;
   }
}

void C_GLTexTriangle::SetVertexPosition( short index, float x, float y, float z )
{
   if( index < 4 )
   {
      Vertices[index].x = x;
      Vertices[index].y = y;
      Vertices[index].z = z;
   }
}
