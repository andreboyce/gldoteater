#ifndef __CGLRC_H
#define __CGLRC_H

#include <windows.h>
#include <gl/glu.h>

class CGLRC
{
   public:

       CGLRC( unsigned short = (short)24 );
      ~CGLRC( void );

      bool EnableOpenGL( const HWND& );
      bool DisableOpenGL( void);
      void SetUpPFD( unsigned short );
      
      bool MakeCurrent( void ) const
         {
            return (bool)::wglMakeCurrent( m_Hdc, m_Hrc );
         }
         
      bool SwapBuffers( void ) const
         {
            ::SwapBuffers( m_Hdc );
         }
         
      void SetPFD( const PIXELFORMATDESCRIPTOR &pfd )
         {
            m_pfd = pfd;
         }

      void SetHWND( const HWND &Hwnd )
         {
            m_Hwnd = Hwnd;
         }

      operator HGLRC( void ) const
         {
            return m_Hrc;
         }

      operator HDC( void ) const
         {
            return m_Hdc;
         }

      operator bool ( void ) const
      {
         return m_bEnabled;
      }

   protected:

      PIXELFORMATDESCRIPTOR m_pfd;
      HWND                  m_Hwnd;
      HDC                   m_Hdc;
      HGLRC                 m_Hrc;
      bool                  m_bEnabled;

   private:
};

#endif

