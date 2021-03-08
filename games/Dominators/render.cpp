#include "main.h"

//***************************************************************************
//******************************* MainWindow::RenderExplosion ***************
// Accepts: void
//
// Purpose: Render the explosions
//
// Returns: void

void MainWindow::RenderExplosions( void )
{
   if( m_TextureManager.Size() )
   {
      ::glEnable( GL_TEXTURE_2D );
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[7] );

      for( int i=0; i<m_Explosions.size() ;i++ )
      {
         ::glLoadIdentity();
         ::glTranslatef( m_Explosions[i].m_PositionVector.m_x, m_Explosions[i].m_PositionVector.m_y,
                         m_Z );
         ::glBegin( GL_QUADS );
         m_Explosions[i].m_TexQuad.Draw( 0, 0, 0 );
         ::glEnd();
      }

      ::glLoadIdentity();
   }
}

//***************************************************************************
//******************************* MainWindow::RenderTitleScreen **************
// Accepts: void
//
// Purpose: Render the title screen
//
// Returns: void

void MainWindow::RenderTitleScreen()
{
   ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   ::glClearDepth( 1.0f );
   ::glLoadIdentity();
   ::glColor4f( 1.0, 1.0, 1.0, 0.0 );

   ::glDisable( GL_TEXTURE_2D );
   ::glDisable( GL_BLEND );

   m_Font.Print( "The Dominators" , -2.0,  0.0, m_TextZ );
   m_Font.Print( "Press SpaceBar To Fire" , -2.0, -2.0, m_TextZ );
   m_Font.Print( "Press F11 To Toggle Sound" , -2.0, -4.0, m_TextZ );
   m_Font.Print( "Press F12 To Toggle FullScreen" , -2.0, -6.0, m_TextZ );
}

//***************************************************************************
//******************************* MainWindow::RenderGameOverScreen **********
// Accepts: void
//
// Purpose: Render the gameover screen
//
// Returns: void

void MainWindow::RenderGameOverScreen()
{
   ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   ::glClearDepth( 1.0f );
   ::glLoadIdentity();
   ::glColor4f( 1.0, 1.0, 1.0, 0.0 );

   ::glDisable( GL_TEXTURE_2D );
   ::glDisable( GL_BLEND );

   m_Font.Print( "GameOver" , 0, 0, m_TextZ );
   m_Font.Print( "Press Enter To Continue" , 0, -1.0, m_TextZ );
   RenderExplosions();
}

//***************************************************************************
//******************************* MainWindow::RenderYouWinScreen ************
// Accepts: void
//
// Purpose: Render the you win screen
//
// Returns: void

void MainWindow::RenderYouWinScreen()
{
   ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   ::glClearDepth( 1.0f );
   ::glLoadIdentity();
   ::glColor4f( 1.0, 1.0, 1.0, 0.0 );

   ::glDisable( GL_TEXTURE_2D );
   ::glDisable( GL_BLEND );

   m_Font.Print( "You Win" , 0, 0, m_TextZ );
   m_Font.Print( "Press Enter To Continue" , 0, -2.0, m_TextZ );
   RenderExplosions();
}

//***************************************************************************
//******************************* MainWindow::RenderBossScreen **************
// Accepts: void
//
// Purpose: Render the boss screen
//
// Returns: void

void MainWindow::RenderBossScreen()
{
   ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   ::glClearDepth( 1.0f );
   ::glLoadIdentity();
   ::glColor4f( 1.0, 1.0, 1.0, 0.0 );
   ::glClearColor( 0.0, 0.0, 0.0, 1.0 );

   ::glDisable( GL_TEXTURE_2D );
   ::glDisable( GL_BLEND );

//   m_Font.Print( "Boss" , 0, 0, m_Z );

   if( m_TextureManager.Size() )
   {
      ::glEnable( GL_TEXTURE_2D );
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

      RenderMap();

      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[0] );
      ::glTranslatef( m_MainTank.m_PositionVector.m_x, m_MainTank.m_PositionVector.m_y, m_Z );
      ::glBegin( GL_QUADS );
      m_MainTank.Draw();
      ::glEnd();
      ::glLoadIdentity();
      m_MainTank.DrawShells( m_TextureManager );


      Structs::Point TextureCoordinates[4];
      TextureCoordinates[0] = Structs::Point( 0 , 1 );
      TextureCoordinates[1] = Structs::Point( 1 , 1 );
      TextureCoordinates[2] = Structs::Point( 1 , 0 );
      TextureCoordinates[3] = Structs::Point( 0 , 0 );

      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[6] );
      ::glTranslatef( m_BossTankA.m_PositionVector.m_x, m_BossTankA.m_PositionVector.m_y, m_Z );
      ::glBegin( GL_QUADS );
      m_BossTankA.Draw( TextureCoordinates );
      ::glEnd();
      ::glLoadIdentity();
      m_BossTankA.DrawShells( m_TextureManager, TextureCoordinates );

      RenderMainTankLife();
      RenderBossTankALife();
   }
   ::glDisable( GL_TEXTURE_2D );
   ::glDisable( GL_BLEND );

}

//***************************************************************************
//******************************* MainWindow::RenderMainTankLife **************
// Accepts: void
//
// Purpose: Render the maintank life
//
// Returns: void

void MainWindow::RenderMainTankLife()
{
   ::glDisable( GL_TEXTURE_2D );
   ::glDisable( GL_BLEND );
   std::ostringstream life;
   life << "Life: " << m_MainTank.m_Life;
   m_Font.Print( life.str(), -20.0, -20.0, m_TextZ );
}

//***************************************************************************
//******************************* MainWindow::RenderBossTankALife ************
// Accepts: void
//
// Purpose: Render the boss tank life
//
// Returns: void

void MainWindow::RenderBossTankALife()
{
   ::glDisable( GL_TEXTURE_2D );
   ::glDisable( GL_BLEND );
   std::ostringstream life;
   life << "Boss Life: " << m_BossTankA.m_Life;
   m_Font.Print( life.str(), -20.0, 20.0, m_TextZ );
}


//***************************************************************************
//******************************* MainWindow::RenderGameScreen **************
// Accepts: void
//
// Purpose: Render the game objects
//
// Returns: void

void MainWindow::RenderGameScreen()
{
   ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   ::glClearDepth( 1.0f );
   ::glLoadIdentity();
   ::glColor4f( 1.0, 1.0, 1.0, 0.0 );
   ::glClearColor( 0.0, 0.0, 0.0, 1.0 );

   ::glDisable( GL_TEXTURE_2D );
   ::glDisable( GL_BLEND );

//   m_Font.Print( "Press Space To Shoot" , 0, 0, m_Z );

//   std::ostringstream tmp;
//   tmp << "x: " << m_MainTank.m_PositionVector.m_x << " y: " << m_MainTank.m_PositionVector.m_y;
//   m_Font.Print( tmp.str() , -5.0, -5.0, m_Z );
//   m_Font.Print( RectToString( m_MainTank.Rect() ) , -5.0, -5.0, m_Z );

//   m_Font.Print( RectToString( m_ATanks[0].Rect() ) , -5.0, -5.0,  m_Z+20 );
//   m_Font.Print( RectToString( m_BTanks[0].Rect() ) , -5.0, -7.0,  m_Z+20 );
//   m_Font.Print( RectToString( m_CTanks[0].Rect() ) , -5.0, -9.0,  m_Z+20 );
//   m_Font.Print( RectToString( m_DTanks[0].Rect() ) , -5.0, -11.0, m_Z+20 );


   if( m_TextureManager.Size() )
   {
      ::glEnable( GL_TEXTURE_2D );
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

      RenderMap();

      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[0] );
	  ::glLoadIdentity();
      ::glTranslatef( m_MainTank.m_PositionVector.m_x, m_MainTank.m_PositionVector.m_y, m_Z );
      ::glBegin( GL_QUADS );
      m_MainTank.Draw();
      ::glEnd();
      ::glLoadIdentity();
      m_MainTank.DrawShells( m_TextureManager );

      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[1] );
      RenderTanks( m_ATanks );
      RenderTanksShells( m_ATanks );

      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[2] );
      RenderTanks( m_BTanks );
      RenderTanksShells( m_BTanks );

      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[3] );
      RenderTanks( m_CTanks );
      RenderTanksShells( m_CTanks );

      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[4] );
      RenderTanks( m_DTanks );
      RenderTanksShells( m_DTanks );

      RenderMainTankLife();
      RenderFreeShells();
      RenderExplosions();

   }
   ::glDisable( GL_TEXTURE_2D );
   ::glDisable( GL_BLEND );
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
   char str[25];
//   ::snprintf( str, 25, "%.2f fps", m_FrameRate.FPS() );
   _snprintf_s( str, 25, "%.2f fps", m_FrameRate.FPS(), 24 );
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
//******************************* MainWindow::BoundingRect ******************
// Accepts: void
//
// Purpose: The visible portion of the screen at a particular z distance
//
// Returns: void

Structs::fRect MainWindow::BoundingRect( float field_of_view_y, float z, float Width, float Height )
{
   Structs::fRect rect;
   float y = fabsf( tan( ( M_PI /180.0)*field_of_view_y/2.0 )*z );
   float x = y*(Width/Height);
   rect.left   = -x;
   rect.top    =  y;
   rect.right  =  x;
   rect.bottom = -y;
   return rect;
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
   m_BoundingRect     = BoundingRect( m_FieldOfViewY, m_Z,    m_Width, m_Height );
   m_TextBoundingRect = BoundingRect( m_FieldOfViewY, m_Z+40, m_Width, m_Height );

//   m_MapTexturedQuad.SetSize( (((m_BoundingRect.right-m_BoundingRect.left)/1.0)/(float)m_TileMap.width()), 
//                              (((m_BoundingRect.top-m_BoundingRect.bottom)/1.0)/(float)m_TileMap.height()) );
   return false;
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
   int textureManagerSize = m_TextureManager.Size();
   for( int i=m_MapTexturesIndex; i<m_TextureManager.Size(); i++ ) // for each texture
   {
      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[i] );
	  for( int row=0; row<m_TileMap.height(); row++, y-=m_MapTexturedQuad.height() ) // for each row
      {
         for( int col=0; col<m_TileMap.width(); col++, x+=m_MapTexturedQuad.width() ) // for each coloum
         {
            // if the texture is in the map render it
			// m_TextureManager indexing starts at 1 while
			// m_TileMap indexing starts at 0
            if( (m_TextureManager[i]-1) == m_TileMap[row][col].m_TileGraphic+m_MapTexturesIndex )
            {
               ::glTranslatef( x, y, m_Z );
               ::glBegin( GL_QUADS );
               //m_MapTexturedQuad.Draw( 0, 0, 0 );

			   Structs::Point TextureCoordinates[4];
			   TextureCoordinates[0] = Structs::Point(0, 0);
			   TextureCoordinates[1] = Structs::Point(1, 0);
			   TextureCoordinates[2] = Structs::Point(1, 1);
			   TextureCoordinates[3] = Structs::Point(0, 1);
			   m_MapTexturedQuad.Draw2(0,0,0,TextureCoordinates);

               ::glEnd();
               ::glLoadIdentity();
            }
         }
         x = m_BoundingRect.left+m_MapTexturedQuad.halfwidth();
      }
      y = m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   }
}
//****************************************************************************
//******************************* MainWindow::x_boundary *********************
// Accepts: void
//
// Purpose: boundary
//
// Returns: void


float MainWindow::x_min_boundary()
{
   return -40;
}

//***************************************************************************
//******************************* MainWindow::x_max_boundary ****************
// Accepts: void
//
// Purpose: boundary
//
// Returns: void

float MainWindow::x_max_boundary()
{
   return 40;
}

//****************************************************************************
//******************************* MainWindow::y_boundary *********************
// Accepts: void
//
// Purpose: boundary
//
// Returns: void

float MainWindow::y_min_boundary()
{
   return -21.0;
}

//****************************************************************************
//******************************* MainWindow::y_max_boundary *****************
// Accepts: void
//
// Purpose: boundary
//
// Returns: void

float MainWindow::y_max_boundary()
{
   return 35.0;
}

//****************************************************************************
//******************************* MainWindow::MainTank_boundary **************
// Accepts: void
//
// Purpose: boundary
//
// Returns: void

Structs::fRect MainWindow::MainTank_boundary()
{
   return Structs::fRect( x_min_boundary(),    // left
                            y_min_boundary()+10, // top
                            x_max_boundary(),    // right
                            -35.53               // bottom
                          );
}

//****************************************************************************
//******************************* MainWindow::Shells_boundary ****************
// Accepts: void
//
// Purpose: boundary
//
// Returns: void

Structs::fRect MainWindow::Shells_boundary()
{
   return Structs::fRect( x_min_boundary(), // left
                            y_max_boundary(), // top
                            x_max_boundary(), // right
                            -35.53            // bottom
                          );
}

//****************************************************************************
//******************************* MainWindow::RenderTanks ********************
// Accepts: void
//
// Purpose: boundary
//
// Returns: void

void MainWindow::RenderTanks( std::deque<C_Tank> &Tanks )
{
   // New texture coordinates to draw the tank upside down
   Structs::Point TextureCoordinates[4];
   TextureCoordinates[0] = Structs::Point( 0 , 1 );
   TextureCoordinates[1] = Structs::Point( 1 , 1 );
   TextureCoordinates[2] = Structs::Point( 1 , 0 );
   TextureCoordinates[3] = Structs::Point( 0 , 0 );

   for( int i=0; i<Tanks.size() ; i++ )
   {
      ::glTranslatef( Tanks[i].m_PositionVector.m_x, Tanks[i].m_PositionVector.m_y, m_Z );
      ::glBegin( GL_QUADS );
      Tanks[i].Draw( TextureCoordinates );
      ::glEnd();
      ::glLoadIdentity();
   }
}

//****************************************************************************
//******************************* MainWindow::RenderTanksShells ********************
// Accepts: void
//
// Purpose: boundary
//
// Returns: void

void MainWindow::RenderTanksShells( std::deque<C_Tank> &Tanks )
{
   // New texture coordinates to draw the shell upside down
   Structs::Point TextureCoordinates[4];
   TextureCoordinates[0] = Structs::Point( 0 , 1 );
   TextureCoordinates[1] = Structs::Point( 1 , 1 );
   TextureCoordinates[2] = Structs::Point( 1 , 0 );
   TextureCoordinates[3] = Structs::Point( 0 , 0 );

   for( int i=0; i<Tanks.size() ; i++ )
   {
      Tanks[i].DrawShells( m_TextureManager, TextureCoordinates );
   }
}

//***************************************************************************
//******************************* MainWindow::RenderFreeShells **************
// Accepts: void
//
// Purpose: Render the free shells
//
// Returns: void

void MainWindow::RenderFreeShells()
{
   // New texture coordinates to draw the shell upside down
   Structs::Point TextureCoordinates[4];
   TextureCoordinates[0] = Structs::Point( 0 , 1 );
   TextureCoordinates[1] = Structs::Point( 1 , 1 );
   TextureCoordinates[2] = Structs::Point( 1 , 0 );
   TextureCoordinates[3] = Structs::Point( 0 , 0 );

   ::glEnable( GL_TEXTURE_2D );
   ::glEnable( GL_BLEND );

   for( int i=0; i<m_FreeShells.size() ; i++ )
   {
      ::glLoadIdentity();
      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[5] );
      ::glTranslatef( m_FreeShells[i].m_PositionVector.m_x, m_FreeShells[i].m_PositionVector.m_y, m_Z );
      ::glBegin( GL_QUADS );
      m_FreeShells[i].Draw();
      ::glEnd();
   }
}

