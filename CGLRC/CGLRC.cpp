#include "CGLRC.h"

CGLRC::CGLRC( unsigned short bitsPerPixel )
{
   m_Hwnd     = NULL;
   m_Hdc      = NULL;
   m_Hrc      = NULL;
   m_bEnabled = false;
   ::ZeroMemory( &m_pfd, sizeof(m_pfd) );
   SetUpPFD( bitsPerPixel );
}

CGLRC::~CGLRC( void )
{
  if( m_bEnabled )
  {
     DisableOpenGL();
  }
}

void CGLRC::SetUpPFD( unsigned short bitsPerPixel )
{
   m_pfd.nSize      = sizeof(m_pfd);
   m_pfd.nVersion   = 1;
   m_pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_GENERIC_ACCELERATED;
   m_pfd.iPixelType = PFD_TYPE_RGBA;
//   m_pfd.cColorBits = 24;
//   m_pfd.cDepthBits = bitsPerPixel;
   m_pfd.cColorBits = bitsPerPixel;
   m_pfd.cDepthBits = 32;
   m_pfd.iLayerType = PFD_MAIN_PLANE;
/*
   m_pfd.cColorBits      = ;
   m_pfd.cRedBits        = ; 
   m_pfd.cRedShift       = ; 
   m_pfd.cGreenBits      = ; 
   m_pfd.cGreenShift     = ; 
   m_pfd.cBlueBits       = ;
   m_pfd.cBlueShift      = ;
   m_pfd.cAlphaBits      = ;
   m_pfd.cAlphaShift     = ;
   m_pfd.cAccumBits      = ;
   m_pfd.cAccumRedBits   = ;
   m_pfd.cAccumGreenBits = ;
   m_pfd.cAccumBlueBits  = ;
   m_pfd.cAccumAlphaBits = ;
   m_pfd.cStencilBits    = ;
   m_pfd.cAuxBuffers     = ;
   m_pfd.bReserved       = ;
   m_pfd.dwLayerMask     = ;
   m_pfd.dwVisibleMask   = ;
   m_pfd.dwDamageMask    = ;
*/
}

bool CGLRC::EnableOpenGL( const HWND& Hwnd )
{
   m_Hwnd = Hwnd;
   if( !( m_Hdc = ::GetDC( m_Hwnd ) ) )
      return false;
   
   GLuint iFormat;

   /* set the pixel format for the DC */
   if( !(iFormat = ::ChoosePixelFormat( m_Hdc, &m_pfd )) )
      return false;

   if( !::SetPixelFormat( m_Hdc, iFormat, &m_pfd ) )
      return false;

   /* create and enable the render context (RC) */
   if( ( m_Hrc = ::wglCreateContext( m_Hdc ) ) == NULL )
      return false;
   
   return m_bEnabled = MakeCurrent();
}

bool CGLRC::DisableOpenGL( void )
{
   if( m_bEnabled )
   {
      if( !::wglMakeCurrent( NULL, NULL ) )
      {
         ::ReleaseDC( m_Hwnd, m_Hdc );
         return false;
      }
      if( !::wglDeleteContext( m_Hrc ) )
      {
         ::ReleaseDC( m_Hwnd, m_Hdc );
         return false;
      }   
      if( !::ReleaseDC( m_Hwnd, m_Hdc ) )
         return false;
      m_bEnabled = false;
   }
   return true;
}

