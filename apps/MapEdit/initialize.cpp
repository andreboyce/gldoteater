#include "main.h"


//***************************************************************************
//******************************* MainWindow::Initialise ********************
// Accepts: void
//
// Purpose: Initialise the graphics engine, sound engine and load textures and sounds
//
// Returns: void

void MainWindow::Initialise()
{
   int ret = IDRETRY;

   do
   {
      if( !EnableOpenGL() )
      {
         ret = Retry( std::string("Unable to initialize OpenGL\n") + GetLastSystemError() + "Would you like to retry" );
      }
      else
      {
         break;
      }
   } while( ret == IDRETRY );

   if( ret == IDCANCEL )
   {
      Alert( "Program exiting" );
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
   }

   InitialiseOpenGL();

/*
   do
   {
      if( !m_Font.BuildFont( m_WinBase ) )
      {
         ret = Retry( std::string( "Could not create font\n" + GetLastSystemError() ) + "Would you like to retry" );
      }
      else
      {
         break;
      }
   } while( ret == IDRETRY );

   if( ret == IDCANCEL )
   {
      Alert( "Program exiting" );
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
   }*/
//   m_TextureManager.LoadGLTextureBitmapFile( "texture/font.bmp", RGB( 0, 0, 0 ) ); //RGB( 10, 10, 10 )
   m_Font.BuildFont( "texture/font.bmp", RGB( 0, 0, 0 ) );
   m_MapEditTrackPopupMenu.m_Hmenu = GetSubMenu( GetMenu(m_WinBase), 0 );
}

//***************************************************************************
//******************************* MainWindow::InitialiseOpenGL **************
// Accepts: void
//
// Purpose: set up some intitial opengl state variables
//
// Returns: void

void MainWindow::InitialiseOpenGL()
{
   ::glDepthFunc( GL_LEQUAL );   // The Type Of Depth Testing (Less Or Equal)
   ::glEnable( GL_DEPTH_TEST );
//      m_p1Console.LogThisLine( " glEnable( GL_DEPTH_TEST ) " + m_GLErrorStrings.GetError(), " gl" );
   ::glShadeModel( GL_SMOOTH );  // Select Smooth Shading
   ::glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
//   ::glBlendFunc( GL_ONE, GL_SRC_ALPHA ); // Set Blending Mode (Cheap / Quick)
   ::glEnable( GL_BLEND );
   ::glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );
   ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   ::glClearDepth( 1.0f );
   ::glLoadIdentity();
//   ::glColor4f( 1.0, 1.0, 1.0, 0.0 );
}



