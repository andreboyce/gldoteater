#include "main.h"

//***************************************************************************
//******************************* MainWindow::BoundingRect ******************
// Accepts: void
//
// Purpose: The visible portion of the screen at a particular z distance
//
// Returns: void

Structs::fRect MainWindow::BoundingRect( float field_of_view_y, float z, float Width, float Height )
{
   Structs::fRect rect;
   float y = fabsf( tan( (M_PI/180.0)*field_of_view_y/2.0 )*z );
   float x = y*(Width/Height);
   rect.left   = -x;
   rect.top    =  y;
   rect.right  =  x;
   rect.bottom = -y;
   return rect;
}

GLfloat	cnt1;				// 1st Counter Used To Move Text & For Coloring
GLfloat	cnt2;				// 2nd Counter Used To Move Text & For Coloring

void MainWindow::RenderMouse( void )
{
   if( m_bRenderMousePos )
   {
      cnt1 += 0.01f;    // Increase The First Counter
      cnt2 += 0.0081f;  // Increase The Second Counter
      m_Font.Print( m_MouseSStream.str(), 0, 0, m_TextZ );
      m_Font.Print( GetTile( m_GLMousePosition ), 0, 1, m_TextZ );
   }
}

//***************************************************************************
//******************************* MainWindow::RenderMap **************
// Accepts: void
//
// Purpose: Render the map
//
// Returns: void

void MainWindow::RenderMap( void )
{
   ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   ::glClearDepth( 1.0f );
   ::glLoadIdentity();

   ::glEnable( GL_TEXTURE_2D );
   float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(), y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();

   std::vector< std::string >& maptiles = m_TileMap.GetTileSet();
   for( int i=0; i<maptiles.size(); i++ ) // for each texture
   {
      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[ maptiles[i] ] );
      for( int row=0; row<m_TileMap.height(); row++, y-=m_MapTexturedQuad.height() ) // for each row
      {
         for( int col=0; col<m_TileMap.width(); col++, x+=m_MapTexturedQuad.width() ) // for each coloum
         {
            /*
            // if the texture is in the map render it
            if( (m_TextureManager[i]-1) == m_TileMap[row][col].m_TileGraphic+m_MapTexturesIndex )
            // m_TextureManager indexing starts at 1 while
            // m_TileMap indexing starts at 0
            {
               ::glTranslatef( x, y, m_Z );
               ::glBegin( GL_QUADS );
               m_MapTexturedQuad.Draw( 0, 0, 0 );
               ::glEnd();
               ::glLoadIdentity();*/

            // if the texture is in the map render it
            if( i == m_TileMap[row][col].m_TileGraphic )
            // m_TextureManager indexing starts at 1 while
            // m_TileMap indexing starts at 0
            {
               ::glTranslatef( x, y, m_Z );
               m_MapTexturedQuad.glBegin();
               m_MapTexturedQuad.Draw( 0, 0, 0 );
               ::glEnd();
               ::glLoadIdentity();
            }


               Structs::Point tile1 = GetTile( m_GLMousePosition );
               Structs::Point tile2( col, row );
               for( int i=0; i<m_TileMap.m_SpecialData.size() ; i++ )
               {
                  if( m_TileMap.m_SpecialDataTypes.size() > m_TileMap.m_SpecialData[i].value )
                  {
                     if( (m_TileMap.m_SpecialData[i].y == row) &&
                         (m_TileMap.m_SpecialData[i].x == col)  )
                     {
                        ::glDisable( GL_TEXTURE_2D );
                        ::glTranslatef( x, y, m_Z );
                        ::glBegin( GL_QUADS );
                        m_MapTexturedQuad.Draw( 0, 0, 0 );
                        ::glEnd();
                        ::glLoadIdentity();
                        ::glEnable( GL_TEXTURE_2D );
                     }
                  }
               }

               if( tile1 == tile2 )
               {
                  ::glDisable( GL_TEXTURE_2D );
                  ::glTranslatef( x, y, m_Z );
                  ::glBegin( GL_QUADS );
                  m_MapTexturedQuad.Draw( 0, 0, 0 );
                  ::glEnd();
                  ::glLoadIdentity();
                  ::glEnable( GL_TEXTURE_2D );
               }
//            }
         }
         x = m_BoundingRect.left+m_MapTexturedQuad.halfwidth();
      }
      y = m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   }
/*   
      ::glEnable( GL_TEXTURE_2D );
      ::glLoadIdentity();
      ::glTranslatef( -5, -5, -10 );
      ::glBindTexture( GL_TEXTURE_2D, m_Font.GetTexture() );
      float char_width  = 10.0;
      float char_height = 10.0;
      ::glBegin( GL_QUADS );                           // Use A Quad For Each Character
         ::glTexCoord2f( 0, 0 );                       // Texture Coord (Bottom Left)
         ::glVertex3f( 0, 0, 0 ) ;                     // Vertex Coord (Bottom Left)
         ::glTexCoord2f( 1, 0 );                       // Texture Coord (Bottom Right)
         ::glVertex3f( char_width, 0, 0 );             // Vertex Coord (Bottom Right)
         ::glTexCoord2f( 1, 1 );                       // Texture Coord (Top Right)
         ::glVertex3f( char_width, char_height, 0 );   // Vertex Coord (Top Right)
         ::glTexCoord2f( 0, 1 );                       // Texture Coord (Top Left)
         ::glVertex3f( 0, char_height, 0 );            // Vertex Coord (Top Left)
      ::glEnd();                                       // Done Building Our Quad (Character)
*/
   ::glDisable( GL_TEXTURE_2D );

   // print text here
   RenderMouse();
}

//***************************************************************************
//******************************* MainWindow::ShowFPSinWindow **************
// Accepts: void
//
// Purpose: Render the frame rate in the window rather than the title bar
//
// Returns: void

void MainWindow::ShowFPSinWindow( float x, float y, float z )
{
   ::glDisable( GL_TEXTURE_2D );
   ::glDisable( GL_BLEND );
   char str[26];
   _snprintf_s( str, 25, 25, "%.2f fps", m_FrameRate.FPS() );
   m_Font.Print( str, x, y-m_Font.Height(str)-1.0, z );
}

//***************************************************************************
//******************************* MainWindow::EnableOpenGL ******************
// Accepts: void
//
// Purpose: Enable opengl to render to the client area of the main window
//
// Returns: void

bool MainWindow::EnableOpenGL()
{
   return m_GLRC.EnableOpenGL( m_WinBase );
}

//***************************************************************************
//******************************* MainWindow::Reshape ***********************
// Accepts: void
//
// Purpose: Resize the viewport when the window has been resized
//
// Returns: void

bool MainWindow::Reshape( long width, long height )
{
   m_Width  = width;
   m_Height = height;
   std::ostringstream temp;
   temp << "{ width="  << width;
   temp << ", height=" << height << " }";
   m_Console.AddLine( temp.str() );
   if( height < 1 ) // Prevent A Divide By Zero By
      height=1;
   ::glViewport( 0, 0, (GLsizei)(width), (GLsizei)(height) );
   ::glMatrixMode( GL_PROJECTION );
   ::glLoadIdentity();                                                  // Reset The Projection Matrix
   ::gluPerspective( m_FieldOfViewY, (GLfloat)(width)/(GLfloat)(height), // Calculate The Aspect Ratio Of The Window
                     0.01f, 1000.0f );
   ::glMatrixMode( GL_MODELVIEW );                                      // Select The Modelview Matrix
   ::glLoadIdentity();

   m_BoundingRect     = BoundingRect( m_FieldOfViewY, m_Z,     m_Width, m_Height );
   m_TextBoundingRect = BoundingRect( m_FieldOfViewY, m_TextZ, m_Width, m_Height );

   m_BoundingRectWidth      = (m_BoundingRect.right-m_BoundingRect.left); 
   m_BoundingRectHeight     = (m_BoundingRect.top-m_BoundingRect.bottom);
   m_TextBoundingRectWidth  = (m_TextBoundingRect.right-m_TextBoundingRect.left);
   m_TextBoundingRectHeight = (m_TextBoundingRect.top-m_TextBoundingRect.bottom);;

   m_TextBoundingRectHalfWidth  = m_TextBoundingRectWidth*0.5;
   m_TextBoundingRectHalfHeight = m_TextBoundingRectHeight*0.5;
   m_BoundingRectHalfWidth      = m_BoundingRectWidth*0.5;
   m_BoundingRectHalfHeight     = m_BoundingRectHeight*0.5;

   m_MapTexturedQuad.SetSize( (((m_BoundingRect.right-m_BoundingRect.left)/1.0)/(float)m_TileMap.width()), 
                              (((m_BoundingRect.top-m_BoundingRect.bottom)/1.0)/(float)m_TileMap.height()) );

   m_InvBoundingRectWidthDivTileMapWidth                = 1.0f/(m_BoundingRectWidth/m_TileMap.width());
   m_InvBoundingRectHeightDivTileMapHeight              = 1.0f/(m_BoundingRectHeight/m_TileMap.height());
   m_BoundingRectHalfWidthMinMapTexturedQuadhalfwidth   = m_BoundingRectHalfWidth - m_MapTexturedQuad.halfwidth();
   m_BoundingRectHalfHeightMinMapTexturedQuadhalfheight = m_BoundingRectHalfHeight + m_MapTexturedQuad.halfheight();

   return false;
}


