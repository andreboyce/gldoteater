#ifndef __C_WinUpDown_H
#define __C_WinUpDown_H

#include "../C_WinControllBase.h"

class C_WinUpDown: public C_WinControllBase
{
   public:
       C_WinUpDown( void );
      ~C_WinUpDown( void );
       bool Create( HWND HwndParent, HWND HwndBuddy, DWORD id, std::string title, short x=0, short y=0, short width=500, short height=500 );

   protected:
      HWND m_HwndBuddy;
   private:
};

#endif
