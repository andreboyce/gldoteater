#include "OpenGLUtility.h"

//   ::glTexCoord2f(0.0f, 0.0f); // Bottom Left  Of The Texture
//   ::glTexCoord2f(1.0f, 0.0f); // Bottom Right Of The Texture
//   ::glTexCoord2f(1.0f, 1.0f); // Top    Right Of The Texture
//   ::glTexCoord2f(0.0f, 1.0f); // Top    Left  Of The Texture

void flip_quad_vertically( C_TexCoord TexCoords[4] )
{
   C_TexCoord texcoords[4];
   texcoords[0] = TexCoords[0];
   texcoords[1] = TexCoords[1];
   texcoords[2] = TexCoords[2];
   texcoords[3] = TexCoords[3];

   TexCoords[0].V = texcoords[3].V;
   TexCoords[1].V = texcoords[2].V;
   TexCoords[2].V = texcoords[1].V;
   TexCoords[3].V = texcoords[0].V;
}
void flip_quad_horizontially( C_TexCoord TexCoords[4] )
{
   C_TexCoord texcoords[4];
   texcoords[0] = TexCoords[0];
   texcoords[1] = TexCoords[1];
   texcoords[2] = TexCoords[2];
   texcoords[3] = TexCoords[3];

   TexCoords[0].U = texcoords[1].U;
   TexCoords[1].U = texcoords[0].U;
   TexCoords[2].U = texcoords[3].U;
   TexCoords[3].U = texcoords[2].U;
}

void rotate_quad_clockwise( C_TexCoord TexCoords[4] )
{
   C_TexCoord texcoords[4];
   texcoords[0] = TexCoords[0];
   texcoords[1] = TexCoords[1];
   texcoords[2] = TexCoords[2];
   texcoords[3] = TexCoords[3];

   TexCoords[0] = texcoords[1];
   TexCoords[1] = texcoords[2];
   TexCoords[2] = texcoords[3];
   TexCoords[3] = texcoords[0];
}

void rotate_quad_anticlockwise( C_TexCoord TexCoords[4] )
{
   C_TexCoord texcoords[4];
   texcoords[0] = TexCoords[0];
   texcoords[1] = TexCoords[1];
   texcoords[2] = TexCoords[2];
   texcoords[3] = TexCoords[3];

   TexCoords[0] = texcoords[0];
   TexCoords[1] = texcoords[3];
   TexCoords[2] = texcoords[2];
   TexCoords[3] = texcoords[1];
}

Vector3D WindowToGLCoordinates( int x, int y )
{
   ::glLoadIdentity();
   GLint    viewport[4];
   GLdouble modelview[16];
   GLdouble projection[16];
   GLfloat  winX, winY, winZ;
   GLdouble posX, posY, posZ;

   ::glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
   ::glGetDoublev( GL_PROJECTION_MATRIX, projection );
   ::glGetIntegerv( GL_VIEWPORT, viewport );

   winX = (float)x;
   winY = (float)viewport[3] - (float)y;
   ::glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

   ::gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

   return Vector3D( posX, posY, posZ );
}

Vector2D GLToWindowCoordinates( double x, double y, double z )
{
   GLdouble *model = NULL, *proj = NULL, *win[3];
   GLint    *view  = NULL;
   ::glGetDoublev(  GL_MATRIX_MODE,       model );
   ::glGetDoublev(  GL_PROJECTION_MATRIX, proj  );
   ::glGetIntegerv( GL_VIEWPORT,          view  );

   ::gluProject( (GLdouble)x, (GLdouble)y, z, model, proj, view, win[0], win[1], win[2] );
   return Vector2D( (double)*(win[0]), (double)*(win[1]) );
}

std::string SetVSync( int interval )
{
   typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
   PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
   const unsigned char *extensions = ::glGetString( GL_EXTENSIONS );
   if( !extensions )
   {
      return std::string( "::glGetString( GL_EXTENSIONS ) returned NULL" );
   }

   if( ::strstr( (const char *)extensions, "WGL_EXT_swap_control" ) == 0 )
   {
      return std::string( "WGL_EXT_swap_control extension not supported on your computer." );
   }
   else
   {
      wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)::wglGetProcAddress( "wglSwapIntervalEXT" );
      if( wglSwapIntervalEXT )
      {
          wglSwapIntervalEXT( interval );
          return std::string( "VSync change successfull" );
      }
      else
      {
          return std::string( "wglGetProcAddress failed" );
      }
   }
}

std::vector<std::string> EnumGLExtensions( void )
{
   std::vector<std::string> Extensions;
   const unsigned char *extensions = ::glGetString( GL_EXTENSIONS );   
   ::strtok( (char*)extensions, " " );
   char * str;
   while( str = ::strtok( NULL, " " ) )
   {
      Extensions.push_back( str );
   }
   return Extensions;
}

bool IsExtensionSupported( std::string TargetExtension )
{
   return IsExtensionSupported( TargetExtension.c_str() );
}

bool IsExtensionSupported( char* szTargetExtension )
{
	const unsigned char *pszExtensions = NULL;
	const unsigned char *pszStart;
	unsigned char *pszWhere, *pszTerminator;

	// Extension names should not have spaces
	pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
	if( pszWhere || *szTargetExtension == '\0' )
		return false;

	// Get Extensions String
	pszExtensions = glGetString( GL_EXTENSIONS );

   if( pszExtensions == NULL )
      return false;

	// Search The Extensions String For An Exact Copy
	pszStart = pszExtensions;
	for(;;)
	{
		pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
		if( !pszWhere )
			break;
		pszTerminator = pszWhere + strlen( szTargetExtension );
		if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
			if( *pszTerminator == ' ' || *pszTerminator == '\0' )
				return true;
		pszStart = pszTerminator;
	}
	return false;
}
