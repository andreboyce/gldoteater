#include "C_GLSprite.h"

C_GLSprite::C_GLSprite( void )
{
}

C_GLSprite::C_GLSprite( const C_GLSprite& C_GLSpriteRHS )
{
   x_pos = C_GLSpriteRHS.x_pos;
   y_pos = C_GLSpriteRHS.y_pos;
   
   CollisionObjs   = C_GLSpriteRHS.CollisionObjs;
   TexQuads        = C_GLSpriteRHS.TexQuads;
   TexTriangles    = C_GLSpriteRHS.TexTriangles;
   Lines           = C_GLSpriteRHS.Lines;
   Points          = C_GLSpriteRHS.Points;
}

C_GLSprite::~C_GLSprite( void )
{
}

void C_GLSprite::Draw( float x, float y, float z, DWORD currenttime )
{
   int i;

   for( i=0 ; i < CollisionObjs.size() ; i++ )
   {
      CollisionObjs[i].m_Rect.m_x = x;
      CollisionObjs[i].m_Rect.m_y = y;
      CollisionObjs[i].m_Tri.m_x  = x;
      CollisionObjs[i].m_Tri.m_y  = y;
   }
   
   ::glEnable( GL_TEXTURE_2D );
   
   for( i=0 ; i < TexQuads.size() ; i++ )
   {
      TexQuads[i].BindCurrentTexture( currenttime );

      ::glBegin( GL_QUADS );
      TexQuads[i].DrawAtOffSet( x, y, z );
      ::glEnd();
   }
   
   for( i=0 ; i < TexTriangles.size() ; i++ )
   {
      TexTriangles[i].BindCurrentTexture( currenttime );

      ::glBegin( GL_TRIANGLES );
      TexTriangles[i].DrawAtOffSet( x, y, z );
      ::glEnd();
   }   

   ::glDisable( GL_TEXTURE_2D );
//*/
   if( Lines.size() )
   {
      ::glLineWidth( Lines[0].GetWidth() );
   }
   ::glBegin( GL_LINES );
   for( i=0; i < Lines.size() ; i++ )
   {
      Lines[i].DrawAtOffSet( x, y, z );
   }
   ::glEnd();
///*
   if( Points.size() )
   {
      ::glPointSize( Points[0].GetSize() );
   }
   ::glBegin( GL_POINTS );
   for( i=0; i < Points.size() ; i++ )
   {         
      Points[i].DrawAtOffSet( x, y, z );
   }
   ::glEnd();
//*/
}
