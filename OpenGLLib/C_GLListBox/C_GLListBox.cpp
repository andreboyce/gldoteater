#include "C_GLListBox.h"

C_GLListBox::C_GLListBox( std::string Title,
                          short NumberOfStrings,
                          C_GLTextureMappedFont& OutLinedFont,
                          short VerticalIndex,
                          short SelectedIndex,
                          bool wrap ) :
         m_NumberOfStringsToDraw( NumberOfStrings ), 
         m_OutLinedFont( OutLinedFont ),
         m_VerticalIndex( VerticalIndex ),
         m_ArrowTexture( 0xFFFF ),
         m_ArrowUpWidth( 1.0 ), 
         m_ArrowDownWidth( 1.0 ),
         m_Title( Title ),
         m_SelectedIndex( SelectedIndex ),
         m_bMouseHoveringOnUpArrow( false ),
         m_bMouseHoveringOnDownArrow( false ),
         m_bWrap( wrap )
{
   m_UpArrowTextCoords[0].x = 1; m_UpArrowTextCoords[0].y = 1;
   m_UpArrowTextCoords[1].x = 1; m_UpArrowTextCoords[1].y = 0;
   m_UpArrowTextCoords[2].x = 0; m_UpArrowTextCoords[2].y = 0;
   m_UpArrowTextCoords[3].x = 0; m_UpArrowTextCoords[3].y = 1;

   m_DownArrowTextCoords[0].x = 0; m_DownArrowTextCoords[0].y = 0;
   m_DownArrowTextCoords[1].x = 0; m_DownArrowTextCoords[1].y = 1;
   m_DownArrowTextCoords[2].x = 1; m_DownArrowTextCoords[2].y = 1;
   m_DownArrowTextCoords[3].x = 1; m_DownArrowTextCoords[3].y = 0;
}


void C_GLListBox::clear( void )
{
   m_ListHeader.clear();
   m_Widths.clear();
   m_Strings.clear();
}

void C_GLListBox::clearrows( void )
{
   m_Strings.clear();
}

void C_GLListBox::Draw( const float _x, const float _y, const float _z )
{
   m_Position.m_x = _x;
   m_Position.m_y = _y;
   m_Position.m_z = _z;

   ::glLoadIdentity();
   short max_index = (m_VerticalIndex+m_NumberOfStringsToDraw);
   if( max_index > m_Strings.size() )
   {
      max_index = m_Strings.size();
   }
   if( max_index < 0 )
   {
      max_index = 0;
   }

   float y = _y;
   float h = 1.5;

   m_OutLinedFont.Print( m_Title, _x, y-m_OutLinedFont.Height( 'A' )*0.5, _z );

   y -= h;
   std::list<std::string>::iterator temp = m_ListHeader.begin();
   short counter = 0;
   float length  = 0.0;
   for( length = 0.0; temp != m_ListHeader.end() ; temp++, counter++ )
   {
      if( m_Widths.size() )
      {
         m_OutLinedFont.Print( *temp, _x+length, y-m_OutLinedFont.Height( 'A' )*0.5, _z );
         length += m_Widths[counter];
      }
   }

   length = 0.0;
   for( int i = 0; i < m_Widths.size() ; i++ )
   {
       length += m_Widths[i];//length;
   }
   m_FirstVisableRowRect.right = _x+length;

   y -= h;

   m_FirstVisableRowRect.left   = _x;
   m_FirstVisableRowRect.top    = +0.5*h+y;
   m_FirstVisableRowRect.bottom = -0.5*h+y;
   m_FirstVisableRowRect.right  = _x+length;

   if( m_Strings.size() )
   {
      for( short index = m_VerticalIndex ; ( index < max_index ) ; ++index, y -= h )
      {
         if( m_Strings[index].size() ) //m_Strings.[index]
         {
            temp    = m_Strings[index].begin();
            counter = 0;
            for( length = 0.0; temp != m_Strings[index].end() ; temp++, counter++ )
            {
               if( m_Widths.size() )
               {
                  m_OutLinedFont.Print( *temp, _x+length, y-m_OutLinedFont.Height( 'A' )*0.5, _z, true, true, true );
                  length += m_Widths[counter];
               }                     
            }
            DrawRow( 0.0, _x, y, _z, length, h );
         }
         if( index == m_SelectedIndex ) //m_VerticalIndex )
         {
            DrawRow( 0.2, _x, y, _z, length, h );
         }
      }
   }

   float x = _x-1;

   DrawUpArrow( x, y, _y, _z );
   DrawDownArrow( x, y, _y, _z );
}

void C_GLListBox::DrawArrows( const float x, const float y, const float _y, const float _z )
{
   DrawUpArrow( x, y, _y, _z );
   DrawDownArrow( x, y, _y, _z );
}

void C_GLListBox::DrawUpArrow( const float x, const float y, const float _y, const float _z )
{
   m_UpArrowPosition.m_x = x;
   m_UpArrowPosition.m_y = y;
   m_UpArrowPosition.m_z = _z;
   float c1 = (m_bMouseHoveringOnUpArrow)   ? 0.20: 1.0;
   float c2 = (m_bMouseHoveringOnDownArrow) ? 0.20: 1.0;

   ::glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

   if( m_Strings.size() > m_NumberOfStringsToDraw )
   {
      ::glLoadIdentity();
      ::glEnable( GL_TEXTURE_2D );
      ::glBindTexture( GL_TEXTURE_2D, m_ArrowTexture );
      ::glBegin( GL_QUADS );
      ::glColor4f( c1, c1, c1, 0.0 ); ::glTexCoord2d( m_UpArrowTextCoords[0].x, m_UpArrowTextCoords[0].y ); ::glVertex3d( x-m_ArrowUpWidth*0.5, _y-m_ArrowUpWidth*0.5, _z );
      ::glColor4f( c1, c1, c1, 0.0 ); ::glTexCoord2d( m_UpArrowTextCoords[1].x, m_UpArrowTextCoords[1].y ); ::glVertex3d( x+m_ArrowUpWidth*0.5, _y-m_ArrowUpWidth*0.5, _z );
      ::glColor4f( c1, c1, c1, 0.0 ); ::glTexCoord2d( m_UpArrowTextCoords[2].x, m_UpArrowTextCoords[2].y ); ::glVertex3d( x+m_ArrowUpWidth*0.5, _y+m_ArrowUpWidth*0.5, _z );
      ::glColor4f( c1, c1, c1, 0.0 ); ::glTexCoord2d( m_UpArrowTextCoords[3].x, m_UpArrowTextCoords[3].y ); ::glVertex3d( x-m_ArrowUpWidth*0.5, _y+m_ArrowUpWidth*0.5, _z );
      ::glEnd();

//      ::glLoadIdentity();
//      ::glBegin( GL_QUADS );
//      ::glColor4f( c2, c2, c2, 0.0 ); ::glTexCoord2d( m_DownArrowTextCoords[0].x, m_DownArrowTextCoords[0].y ); ::glVertex3d( x-m_ArrowDownWidth*0.5, y-m_ArrowDownWidth*0.5, _z );
//      ::glColor4f( c2, c2, c2, 0.0 ); ::glTexCoord2d( m_DownArrowTextCoords[1].x, m_DownArrowTextCoords[1].y ); ::glVertex3d( x+m_ArrowDownWidth*0.5, y-m_ArrowDownWidth*0.5, _z );
//      ::glColor4f( c2, c2, c2, 0.0 ); ::glTexCoord2d( m_DownArrowTextCoords[2].x, m_DownArrowTextCoords[2].y ); ::glVertex3d( x+m_ArrowDownWidth*0.5, y+m_ArrowDownWidth*0.5, _z );
//      ::glColor4f( c2, c2, c2, 0.0 ); ::glTexCoord2d( m_DownArrowTextCoords[3].x, m_DownArrowTextCoords[3].y ); ::glVertex3d( x-m_ArrowDownWidth*0.5, y+m_ArrowDownWidth*0.5, _z );
//      ::glEnd();
      ::glDisable( GL_TEXTURE_2D );
      ::glLoadIdentity();
   }

   m_Arrow1Rect.left   = x-m_ArrowUpWidth*0.5;
   m_Arrow1Rect.top    = _y+m_ArrowUpWidth*0.5;
   m_Arrow1Rect.right  = x+m_ArrowUpWidth*0.5;
   m_Arrow1Rect.bottom = _y-m_ArrowUpWidth*0.5;

   m_Arrow2Rect.left   = x-m_ArrowDownWidth*0.5;
   m_Arrow2Rect.top    = y+m_ArrowDownWidth*0.5;
   m_Arrow2Rect.right  = x+m_ArrowDownWidth*0.5;
   m_Arrow2Rect.bottom = y-m_ArrowDownWidth*0.5;

   ::glPopAttrib();
}

void C_GLListBox::DrawDownArrow( const float x, const float y, const float _y, const float _z )
{
   m_DownArrowPosition.m_x = x;
   m_DownArrowPosition.m_y = _y;
   m_DownArrowPosition.m_z = _z;
   float c1 = (m_bMouseHoveringOnUpArrow)   ? 0.20: 1.0;
   float c2 = (m_bMouseHoveringOnDownArrow) ? 0.20: 1.0;

   ::glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

   if( m_Strings.size() > m_NumberOfStringsToDraw )
   {
      ::glLoadIdentity();
      ::glEnable( GL_TEXTURE_2D );
      ::glBindTexture( GL_TEXTURE_2D, m_ArrowTexture );
//      ::glBegin( GL_QUADS );
//      ::glColor4f( c1, c1, c1, 0.0 ); ::glTexCoord2d( m_UpArrowTextCoords[0].x, m_UpArrowTextCoords[0].y ); ::glVertex3d( x-m_ArrowUpWidth*0.5, _y-m_ArrowUpWidth*0.5, _z );
//      ::glColor4f( c1, c1, c1, 0.0 ); ::glTexCoord2d( m_UpArrowTextCoords[1].x, m_UpArrowTextCoords[1].y ); ::glVertex3d( x+m_ArrowUpWidth*0.5, _y-m_ArrowUpWidth*0.5, _z );
//      ::glColor4f( c1, c1, c1, 0.0 ); ::glTexCoord2d( m_UpArrowTextCoords[2].x, m_UpArrowTextCoords[2].y ); ::glVertex3d( x+m_ArrowUpWidth*0.5, _y+m_ArrowUpWidth*0.5, _z );
//      ::glColor4f( c1, c1, c1, 0.0 ); ::glTexCoord2d( m_UpArrowTextCoords[3].x, m_UpArrowTextCoords[3].y ); ::glVertex3d( x-m_ArrowUpWidth*0.5, _y+m_ArrowUpWidth*0.5, _z );
//      ::glEnd();

      ::glLoadIdentity();
      ::glBegin( GL_QUADS );
      ::glColor4f( c2, c2, c2, 0.0 ); ::glTexCoord2d( m_DownArrowTextCoords[0].x, m_DownArrowTextCoords[0].y ); ::glVertex3d( x-m_ArrowDownWidth*0.5, y-m_ArrowDownWidth*0.5, _z );
      ::glColor4f( c2, c2, c2, 0.0 ); ::glTexCoord2d( m_DownArrowTextCoords[1].x, m_DownArrowTextCoords[1].y ); ::glVertex3d( x+m_ArrowDownWidth*0.5, y-m_ArrowDownWidth*0.5, _z );
      ::glColor4f( c2, c2, c2, 0.0 ); ::glTexCoord2d( m_DownArrowTextCoords[2].x, m_DownArrowTextCoords[2].y ); ::glVertex3d( x+m_ArrowDownWidth*0.5, y+m_ArrowDownWidth*0.5, _z );
      ::glColor4f( c2, c2, c2, 0.0 ); ::glTexCoord2d( m_DownArrowTextCoords[3].x, m_DownArrowTextCoords[3].y ); ::glVertex3d( x-m_ArrowDownWidth*0.5, y+m_ArrowDownWidth*0.5, _z );
      ::glEnd();
      ::glDisable( GL_TEXTURE_2D );
      ::glLoadIdentity();
   }

   m_Arrow1Rect.left   = x-m_ArrowUpWidth*0.5;
   m_Arrow1Rect.top    = _y+m_ArrowUpWidth*0.5;
   m_Arrow1Rect.right  = x+m_ArrowUpWidth*0.5;
   m_Arrow1Rect.bottom = _y-m_ArrowUpWidth*0.5;

   m_Arrow2Rect.left   = x-m_ArrowDownWidth*0.5;
   m_Arrow2Rect.top    = y+m_ArrowDownWidth*0.5;
   m_Arrow2Rect.right  = x+m_ArrowDownWidth*0.5;
   m_Arrow2Rect.bottom = y-m_ArrowDownWidth*0.5;

   ::glPopAttrib();
}

void C_GLListBox::DrawRow( const float c, const float _x, const float y, const float _z, const float length, const float h )
{
   ::glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT );
   ::glLoadIdentity();
   ::glEnable( GL_BLEND );
   ::glBlendFunc( GL_SRC_ALPHA, GL_SRC_ALPHA );
   float halflength = length*0.5;
   ::glDisable( GL_TEXTURE_2D );
   ::glBegin( GL_QUADS );
   ::glColor4f( c, c, c, 1.0 ); ::glVertex3d( -0.5*length+_x+halflength, -0.5*h+y, _z ); // -(h)
   ::glColor4f( c, c, c, 1.0 ); ::glVertex3d( +0.5*length+_x+halflength, -0.5*h+y, _z );
   ::glColor4f( c, c, c, 1.0 ); ::glVertex3d( +0.5*length+_x+halflength, +0.5*h+y, _z );
   ::glColor4f( c, c, c, 1.0 ); ::glVertex3d( -0.5*length+_x+halflength, +0.5*h+y, _z );
   ::glEnd();
   ::glPopAttrib();
}

bool C_GLListBox::UpArrowSelected( Structs::fPoint MousePosition )
{
   GLuint buffer_size = 512;
   std::vector<GLuint> buffer(buffer_size);
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
   DrawDownArrow( m_UpArrowPosition.m_x, m_UpArrowPosition.m_y, m_UpArrowPosition.m_y, m_UpArrowPosition.m_z );
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

bool C_GLListBox::DownArrowSelected( Structs::fPoint MousePosition )
{
   GLuint buffer_size = 512;
   std::vector<GLuint> buffer(buffer_size);
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
   // Do drawing here
   DrawDownArrow( m_DownArrowPosition.m_x, m_DownArrowPosition.m_y, m_DownArrowPosition.m_y, m_DownArrowPosition.m_z );
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


bool C_GLListBox::UpArrowSelected( Structs::Point MousePosition )
{
   GLuint buffer_size = 512;
   std::vector<GLuint> buffer(buffer_size);
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
   DrawDownArrow( m_UpArrowPosition.m_x, m_UpArrowPosition.m_y, m_UpArrowPosition.m_y, m_UpArrowPosition.m_z );
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

bool C_GLListBox::DownArrowSelected( Structs::Point MousePosition )
{
   GLuint buffer_size = 512;
   std::vector<GLuint> buffer(buffer_size);
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
   // Do drawing here
   DrawDownArrow( m_DownArrowPosition.m_x, m_DownArrowPosition.m_y, m_DownArrowPosition.m_y, m_DownArrowPosition.m_z );
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

bool C_GLListBox::MouseOver( Structs::Point MousePosition )
{
   if( UpArrowMouseOver( MousePosition ) )
      return true;
   if( DownArrowMouseOver( MousePosition ) )
      return true;
   return false;
}

bool C_GLListBox::UpArrowMouseOver( Structs::Point MousePosition )
{
   if( UpArrowSelected(  MousePosition ) )
   {
      MouseHoverArrowUp();
      return true;
   }
   else
   {
      MouseHoverOffArrowUp();
      return false;
   }
   return false;
}

bool C_GLListBox::DownArrowMouseOver( Structs::Point MousePosition )
{
   if( DownArrowSelected( MousePosition ) )
   {
      MouseHoverArrowDown();
      return true;
   }
   else
   {
      MouseHoverOffArrowDown();
      return false;
   }
   return false;
}

bool C_GLListBox::SelecteRow( Structs::Point MousePosition )
{
   float _x = m_Position.m_x;
   float _y = m_Position.m_y;
   float _z = m_Position.m_z;

   ::glLoadIdentity();
   short max_index = (m_VerticalIndex+m_NumberOfStringsToDraw);
   if( max_index > m_Strings.size() )
   {
      max_index = m_Strings.size();
   }
   if( max_index < 0 )
   {
      max_index = 0;
   }

   float y = _y;
   float h = 1.5;

   y -= h;
   std::list<std::string>::iterator temp = m_ListHeader.begin();
   short counter = 0;
   float length  = 0.0;
   for( length = 0.0; temp != m_ListHeader.end() ; temp++, counter++ )
   {
      if( m_Widths.size() )
      {
         length += m_Widths[counter];
      }
   }

   length = 0.0;
   for( int i = 0; i < m_Widths.size() ; i++ )
   {
       length += m_Widths[i];
   }
   m_FirstVisableRowRect.right = _x+length;

   y -= h;

   m_FirstVisableRowRect.left   = _x;
   m_FirstVisableRowRect.top    = +0.5*h+y;
   m_FirstVisableRowRect.bottom = -0.5*h+y;
   m_FirstVisableRowRect.right  = _x+length;

   if( m_Strings.size() )
   {
      for( short index = m_VerticalIndex ; ( index < max_index ) ; ++index, y -= h )
      {
         if( m_Strings[index].size() ) //m_Strings.[index]
         {
            temp    = m_Strings[index].begin();
            counter = 0;
            for( length = 0.0; temp != m_Strings[index].end() ; temp++, counter++ )
            {
               if( m_Widths.size() )
               {
                  length += m_Widths[counter];
               }                     
            }
            ///////////////////////////////////////////////////////////////
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
            DrawRow( 0.0, _x, y, _z, length, h );
            // end drawing
            ::glMatrixMode( GL_PROJECTION );
            ::glPopMatrix();
            ::glMatrixMode( GL_MODELVIEW );
            GLint hits = 0;
            hits = ::glRenderMode( GL_RENDER );
            if( hits > 0 )
            {
               m_SelectedIndex = index;
               return true;
            }
            ///////////////////////////////////////////////////////////////
         }/*
         if( index == m_SelectedIndex )
         {
            ///////////////////////////////////////////////////////////////
            GLuint   buffer_size = 512;
            GLuint   buffer[buffer_size];
            ::glMatrixMode( GL_PROJECTION );
            ::glPushMatrix();
            ::glLoadIdentity();
            GLint viewport[4];
            ::glGetIntegerv( GL_VIEWPORT, viewport );
            ::glSelectBuffer( buffer_size, buffer );
            ::glRenderMode( GL_SELECT );
            // viewport[3]- change window coordinates to glu window coordinates
            ::gluPickMatrix( MousePosition.x, viewport[3]-MousePosition.y, 1.0f, 1.0f, viewport );
            ::gluPerspective( 45.0f, 
                              (GLfloat)(viewport[2]-viewport[0]) / (GLfloat)(viewport[3]-viewport[1]), 
                              0.01f, 1000.0f );
            ::glMatrixMode( GL_MODELVIEW );
            // Do drawinf here
            DrawRow( 0.2, _x, y, _z, length, h );
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
            ///////////////////////////////////////////////////////////////
         }*/
      }
   }
   return false;
}

bool C_GLListBox::SelecteRow( Structs::fPoint MousePosition )
{
   float _x = m_Position.m_x;
   float _y = m_Position.m_y;
   float _z = m_Position.m_z;

   ::glLoadIdentity();
   short max_index = (m_VerticalIndex+m_NumberOfStringsToDraw);
   if( max_index > m_Strings.size() )
   {
      max_index = m_Strings.size();
   }
   if( max_index < 0 )
   {
      max_index = 0;
   }

   float y = _y;
   float h = 1.5;

   y -= h;
   std::list<std::string>::iterator temp = m_ListHeader.begin();
   short counter = 0;
   float length  = 0.0;
   for( length = 0.0; temp != m_ListHeader.end() ; temp++, counter++ )
   {
      if( m_Widths.size() )
      {
         length += m_Widths[counter];
      }
   }

   length = 0.0;
   for( int i = 0; i < m_Widths.size() ; i++ )
   {
       length += m_Widths[i];
   }
   m_FirstVisableRowRect.right = _x+length;

   y -= h;

   m_FirstVisableRowRect.left   = _x;
   m_FirstVisableRowRect.top    = +0.5*h+y;
   m_FirstVisableRowRect.bottom = -0.5*h+y;
   m_FirstVisableRowRect.right  = _x+length;

   if( m_Strings.size() )
   {
      for( short index = m_VerticalIndex ; ( index < max_index ) ; ++index, y -= h )
      {
         if( m_Strings[index].size() ) //m_Strings.[index]
         {
            temp    = m_Strings[index].begin();
            counter = 0;
            for( length = 0.0; temp != m_Strings[index].end() ; temp++, counter++ )
            {
               if( m_Widths.size() )
               {
                  length += m_Widths[counter];
               }                     
            }
            ///////////////////////////////////////////////////////////////
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
            DrawRow( 0.0, _x, y, _z, length, h );
            // end drawing
            ::glMatrixMode( GL_PROJECTION );
            ::glPopMatrix();
            ::glMatrixMode( GL_MODELVIEW );
            GLint hits = 0;
            hits = ::glRenderMode( GL_RENDER );
            if( hits > 0 )
            {
               m_SelectedIndex = index;
               return true;
            }
            ///////////////////////////////////////////////////////////////
         }
      }
   }
   return false;
}
