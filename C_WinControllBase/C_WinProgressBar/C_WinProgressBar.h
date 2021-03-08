#ifndef __C_WinProgressBar_H
#define __C_WinProgressBar_H

#include "../C_WinControllBase.h"

class C_WinProgressBar: public C_WinControllBase
{
   public:
      C_WinProgressBar() : 
         C_WinControllBase( PROGRESS_CLASS, WS_VISIBLE | WS_CHILD, 0 )
      {}
      C_WinProgressBar::C_WinProgressBar( const C_WinProgressBar & RHS ) :
         C_WinControllBase( RHS )
      {}
      C_WinProgressBar& operator=( const C_WinProgressBar& rhs )
      {
      }
      ~C_WinProgressBar(){}
      bool C_WinProgressBar::Create( HWND HwndParent, DWORD id, std::string title="",
                                short x=0, short y=0, short width=800, short height=100 )
      {
         m_HwndParent = HwndParent;
         return (bool)CWinBase::Create( (char*)title.c_str(), HwndParent, x, y, width, height, (HMENU)id, 0 );
      }
      void StepNext( void )
      {
         ::SendMessage( m_Hwnd, PBM_STEPIT, 0, 0 );
      }
      void SetRange( int minrange=0, int maxrange=100 )
      {
         ::SendMessage( m_Hwnd, PBM_SETRANGE, 0, MAKELPARAM( minrange, maxrange ) );
      }
      void SetStep( WPARAM wParam=1 )
      {
         ::SendMessage( m_Hwnd, PBM_SETSTEP, wParam, 0 );
      }
      void SetPosition( WPARAM wParam=0 )
      {
         ::SendMessage( m_Hwnd, PBM_SETPOS, wParam, 0 );
      }
      void DeltaPosition( WPARAM wParam=0 )
      {
         ::SendMessage( m_Hwnd, PBM_DELTAPOS, wParam, 0 );
      }
   protected:
   private:
};

#endif
