#ifndef __C_GetDC_H
#define __C_GetDC_H

#define WIN32_LEAN_AND_MEAN
#include "../C_HDC.h"

class C_GetDC: public C_HDC
{
   public:
       C_GetDC( void );
       C_GetDC( HWND );
      ~C_GetDC( void );

      void SetHWND( HWND Hwnd )
      {
         if( !m_Hwnd ) // If the  C_PaintDC( HWND ) constructor wasnt used
         {
            m_Hwnd = Hwnd;
            m_Hdc  = ::GetDC( m_Hwnd );
         }
         else
         {
            ::ReleaseDC( m_Hwnd, m_Hdc );
            m_Hwnd = Hwnd;
            m_Hdc  = ::GetDC( m_Hwnd );
         }
      }

   protected:
   private:   
};

#endif
