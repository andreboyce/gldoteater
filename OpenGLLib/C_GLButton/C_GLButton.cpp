#include "C_GLButton.h"

void C_GLButton::BuildQuadList( void )
{
   m_TexQuadList.destroy();
   m_TexQuadList.begin();
   ::glLoadIdentity();
   ::glBindTexture( GL_TEXTURE_2D, m_Texture );
   C_TexCoord TexCoords[4];
   C_Vert     Verticies[4];
   ::glTranslatef( 0, 0, m_Position.m_z );
   m_TexQuad.GetVerticies( Verticies, m_Position.m_x, m_Position.m_y, m_Position.m_z );
   ::glBegin( GL_TRIANGLES );
      m_TexCoords[0].glTexCoord2f();
      Verticies[0].glVertex2f();

      m_TexCoords[1].glTexCoord2f();
      Verticies[1].glVertex2f();

      m_TexCoords[2].glTexCoord2f();
      Verticies[2].glVertex2f();

      m_TexCoords[0].glTexCoord2f();
      Verticies[0].glVertex2f();

      m_TexCoords[2].glTexCoord2f();
      Verticies[2].glVertex2f();

      m_TexCoords[3].glTexCoord2f();
      Verticies[3].glVertex2f();
   ::glEnd();
   m_TexQuadList.end();
}

bool C_GLButton::OnClick( Structs::Point MousePosition )
{
   if( ButtonSelected( MousePosition ) )
   {
      m_ClickCallBack.call();
      return true;
   }
   return false;
}

bool C_GLButton::OnMouseOver( const Structs::Point MousePosition )
{
   if( ButtonSelected( MousePosition ) )
   {
      if( !mouseon )
      {
         mouseon = true;
         m_TexQuad.SetSize( m_MouseOverSize.m_x, m_MouseOverSize.m_y );
         BuildQuadList();
      }
      return true;
   }
   if( mouseon )
   {
      mouseon = false;
      m_TexQuad.SetSize( m_MouseOffSize.m_x, m_MouseOffSize.m_y );
      BuildQuadList();
   }
   return false;
}

bool C_GLButton::ButtonSelected( const Structs::Point MousePosition )
{
   GLuint   buffer_size = 512;
   std::vector<GLuint>   buffer(buffer_size);
   ::glMatrixMode( GL_PROJECTION );
   ::glPushMatrix();
   ::glLoadIdentity();
   GLint viewport[4];
   ::glGetIntegerv( GL_VIEWPORT, viewport );
   ::glSelectBuffer( buffer_size, &buffer[0] );
   ::glRenderMode( GL_SELECT );
   // viewport[3]- change window coordinates to glu window coordinates
   ::gluPickMatrix( MousePosition.x, viewport[3]-MousePosition.y, 1.0f, 1.0f, viewport );
   ::gluPerspective( 45.0f, 
                    (GLfloat)(viewport[2]-viewport[0]) / (GLfloat)(viewport[3]-viewport[1]), 
                    0.01f, 1000.0f );
   ::glMatrixMode( GL_MODELVIEW );
   // Do drawinf here
   Draw();
   // end drawing
   ::glMatrixMode( GL_PROJECTION );
   ::glPopMatrix();
   ::glMatrixMode( GL_MODELVIEW );
   GLint hits = 0;
   hits = ::glRenderMode( GL_RENDER );
   if( hits > 0 )
   {
      return true;
   }
   return false;
}

bool C_GLButton::OnClick( Structs::dPoint3D MousePosition )
{
   if( ButtonSelected( MousePosition ) )
   {
      m_ClickCallBack.call();
      return true;
   }
   return false;
}

bool C_GLButton::OnMouseOver( const Structs::dPoint3D MousePosition )
{
   if( ButtonSelected( MousePosition ) )
   {
      if( !mouseon )
      {
         mouseon = true;
         m_TexQuad.SetSize( m_MouseOverSize.m_x, m_MouseOverSize.m_y );
         BuildQuadList();
      }
      return true;
   }
   if( mouseon )
   {
      mouseon = false;
      m_TexQuad.SetSize( m_MouseOffSize.m_x, m_MouseOffSize.m_y );
      BuildQuadList();
   }
   return false;
}

bool C_GLButton::ButtonSelected( const Structs::dPoint3D MousePosition )
{
   GLuint   buffer_size = 512;
   std::vector<GLuint>   buffer(buffer_size);
   ::glMatrixMode( GL_PROJECTION );
   ::glPushMatrix();
   ::glLoadIdentity();
   GLint viewport[4];
   ::glGetIntegerv( GL_VIEWPORT, viewport );
   ::glSelectBuffer( buffer_size, &buffer[0] );
   ::glRenderMode( GL_SELECT );
   // viewport[3]- change window coordinates to glu window coordinates
   ::gluPickMatrix( MousePosition.m_x, viewport[3]-MousePosition.m_y, 1.0f, 1.0f, viewport );
   ::gluPerspective( 45.0f, 
                    (GLfloat)(viewport[2]-viewport[0]) / (GLfloat)(viewport[3]-viewport[1]), 
                    0.01f, 1000.0f );
   ::glMatrixMode( GL_MODELVIEW );
   // Do drawinf here
   Draw();
   // end drawing
   ::glMatrixMode( GL_PROJECTION );
   ::glPopMatrix();
   ::glMatrixMode( GL_MODELVIEW );
   GLint hits = 0;
   hits = ::glRenderMode( GL_RENDER );
   if( hits > 0 )
   {
      return true;
   }
   return false;
}
