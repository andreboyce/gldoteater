#include "C_GLTextureMappedFont.h"

bool C_GLTextureMappedFont::BuildFont( std::string filename, COLORREF transparent_color, float char_offset_x, float scale_x, float scale_y )
{
   Destroy();
   bool flip_vertical = false;
   if( filename.find( ".dds" ) != std::string::npos )
   {
      flip_vertical = true;
   }
   else if( filename.find( ".tga" ) != std::string::npos )
   {
   }

   m_texture_id = m_TextureManager.LoadGLTextureFile( filename );

   if( m_texture_id == ERROR_CNLT )
   {
      Destroy();
      return false;
   }
   m_char_offset_x = char_offset_x;
   float cx;                     // Holds Our X Character Coord
   float cy;                     // Holds Our Y Character Coord
   m_base = ::glGenLists( 256 ); // Creating 256 Display Lists
   if( !::glIsList( m_base ) || m_base == 0 )
   {
      Alert( C_GLErrorStrings::GetError() );
      Destroy();
      return false;
   }

   float char_width  = 1.0f/16.0f*scale_x;
   float char_height = 1.0f/16.0f*scale_y;
   m_char_width  = char_width;
   m_char_height = char_height;
   ::glLoadIdentity();
   ::glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
   ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   ::glShadeModel( GL_SMOOTH );                    // Enables Smooth Color Shading
   ::glBindTexture( GL_TEXTURE_2D, m_texture_id ); // Select Our Font Texture
   for( int loop=0; loop<256 ; loop++ ) // Loop Through All 256 Lists
   {
      cx = float(loop%16)/16.0f; // X Position Of Current Character
      cy = float(loop/16)/16.0f; // Y Position Of Current Character

      ::glNewList( m_base+loop, GL_COMPILE );          // Start Building A List
      
//      ::glBegin( GL_TRIANGLE_STRIP );                           // Use A Quad For Each Character
      ::glBegin( GL_TRIANGLES );
      if( flip_vertical )
      {/*
         ::glTexCoord2f( cx, cy );                  // Texture Coord (Top Left)
         ::glVertex2f( 0, char_height );            // Vertex Coord (Top Left)

         ::glTexCoord2f( cx, cy+0.0625f );          // Texture Coord (Bottom Left)
         ::glVertex2f( 0, 0 ) ;                     // Vertex Coord (Bottom Left)

         ::glTexCoord2f( cx+0.0625f, cy+0.0625f );  // Texture Coord (Bottom Right)
         ::glVertex2f( char_width, 0 );             // Vertex Coord (Bottom Right)

         ::glTexCoord2f( cx+0.0625f, cy );          // Texture Coord (Top Right)
         ::glVertex2f( char_width, char_height );   // Vertex Coord (Top Right)
       */         
         ::glTexCoord2f( cx, cy+0.0625f );          // Texture Coord (Bottom Left)
         ::glVertex2f( 0, 0 ) ;                     // Vertex Coord (Bottom Left)
         ::glTexCoord2f( cx+0.0625f, cy+0.0625f );  // Texture Coord (Bottom Right)
         ::glVertex2f( char_width, 0 );             // Vertex Coord (Bottom Right)
         ::glTexCoord2f( cx+0.0625f, cy );          // Texture Coord (Top Right)
         ::glVertex2f( char_width, char_height );   // Vertex Coord (Top Right)

         ::glTexCoord2f( cx, cy+0.0625f );          // Texture Coord (Bottom Left)
         ::glVertex2f( 0, 0 ) ;                     // Vertex Coord (Bottom Left)
         ::glTexCoord2f( cx+0.0625f, cy );          // Texture Coord (Top Right)
         ::glVertex2f( char_width, char_height );   // Vertex Coord (Top Right)
         ::glTexCoord2f( cx, cy );                  // Texture Coord (Top Left)
         ::glVertex2f( 0, char_height );            // Vertex Coord (Top Left)
      }
      else
      {
         ::glTexCoord2f( cx, 1-cy-0.0625f );           // Texture Coord (Bottom Left)
         ::glVertex2f( 0, 0 ) ;                     // Vertex Coord (Bottom Left)
         ::glTexCoord2f( cx+0.0625f, 1-cy-0.0625f );   // Texture Coord (Bottom Right)
         ::glVertex2f( char_width, 0 );             // Vertex Coord (Bottom Right)
         ::glTexCoord2f( cx+0.0625f, 1-cy );           // Texture Coord (Top Right)
         ::glVertex2f( char_width, char_height );   // Vertex Coord (Top Right)

         ::glTexCoord2f( cx, 1-cy-0.0625f );           // Texture Coord (Bottom Left)
         ::glVertex2f( 0, 0 ) ;                     // Vertex Coord (Bottom Left)
         ::glTexCoord2f( cx+0.0625f, 1-cy );           // Texture Coord (Top Right)
         ::glVertex2f( char_width, char_height );   // Vertex Coord (Top Right)
         ::glTexCoord2f( cx, 1-cy );                   // Texture Coord (Top Left)
         ::glVertex2f( 0, char_height );            // Vertex Coord (Top Left)
      }
      ::glEnd();                                          // Done Building Our Quad (Character)
      ::glTranslatef( char_width*m_char_offset_x, 0, 0 ); // Move To The Right Of The Character
                                                          // it is less than the width of a character 
                                                          // because all characters have unused space in them
      ::glEndList();                                      // Done Building The Display List
   } // Loop Until All 256 Are Built
   ::glLoadIdentity();
   m_bFontBuilt = true;
   return true;
}

float C_GLTextureMappedFont::Length( const char* text )
{
   if( !m_bFontBuilt )
      return 0;
   float length = 0;
   int   len    = ::strlen( text );
   if( len == 1 )
      return m_char_width;
   for( unsigned int loop=0; loop < len ; loop++ ) // Loop To Find Text Length
   {
      length += m_char_width-(m_char_width*((1.0-m_char_offset_x)));
      // Increase Length By Each Characters Width - the overlap 
      // the characters quads overlap to make them space more closely togeather
   }
   return length;
}

GLvoid C_GLTextureMappedFont::KillFont( GLvoid ) // Delete The Font From Memory
{
   ::glDeleteLists( m_base, 256 );  // Delete All 256 Display Lists
   m_base = 0;
}

void C_GLTextureMappedFont::Destroy( void )
{
   if( !m_bFontBuilt )
      return;
   KillFont();
   m_texture_id  = ERROR_CNLT;
   m_bFontBuilt  = false;
   m_char_width  = 0;
   m_char_height = 0;
   texturename.clear();
}

void C_GLTextureMappedFont::Print( const char* text, bool bshow_transparent, int set )
{
   if( !m_bFontBuilt )
      return;
   if( text != NULL )
   {
      ::glEnable( GL_TEXTURE_2D );
      ::glBindTexture( GL_TEXTURE_2D, m_texture_id ); // Select Our Font Texture
      ::glPushAttrib( GL_LIST_BIT );                  // Pushes The Display List Bits
      ::glListBase( m_base-32+(128*set) );
      ::glCallLists( ::strlen( text ), GL_UNSIGNED_BYTE, text ); // Draws The Display List Text
      ::glPopAttrib();                                           // Pops The Display List Bits
//      ::glDisable( GL_TEXTURE_2D );
   }
}

void C_GLTextureMappedFont::Print( const char* text, const float x, const float y, const float z, bool bshow_transparent, bool PushMatrix, int set )
{
   if( !m_bFontBuilt )
      return;
   if( text != NULL )
   {
      if( PushMatrix )
      {
         ::glPushMatrix();
         ::glLoadIdentity();
      }
      if (set>1)
      {
         set=1;
      }

      ::glEnable( GL_TEXTURE_2D );
//      bool bpush_blend = false;
      if( bshow_transparent )
      {
//         bpush_blend = true;
//         ::glPushAttrib( GL_ENABLE_BIT );
         ::glEnable( GL_BLEND );
         ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      }
      ::glBindTexture( GL_TEXTURE_2D, m_texture_id ); // Select Our Font Texture
      ::glTranslatef( x, y, z );                      // Center Our Text On The Screen
      ::glPushAttrib( GL_LIST_BIT );                  // Pushes The Display List Bits
      ::glListBase( m_base-32+(128*set) );
      ::glCallLists( ::strlen( text ), GL_UNSIGNED_BYTE, text ); // Draws The Display List Text
      ::glPopAttrib();                                           // Pops The Display List Bits
//      ::glDisable( GL_TEXTURE_2D );

      if( PushMatrix )
      {
         ::glPopMatrix();
      }
//      if( bpush_blend )
//      {
//         ::glPopAttrib();
//      }
   }
}
