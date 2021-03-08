#ifndef __C_PaintDC_H
#define __C_PaintDC_H

#include "../C_HDC.h"

class C_PaintDC: public C_HDC
{
   public:
       C_PaintDC( void );
       C_PaintDC( HWND );
      ~C_PaintDC( void );

      void SetHWND( HWND Hwnd )
      {
         m_Hwnd = Hwnd;
      }

      void Begin( void )
      {
         m_Hdc  = ::BeginPaint( m_Hwnd, & m_ps );
      }

      void End( void )
      {
         ::EndPaint( m_Hwnd, & m_ps );
      }

   protected:
      PAINTSTRUCT m_ps;

   private:
};

#endif
