#ifndef __C_WinScrollBar_H
#define __C_WinScrollBar_H

#include "../C_WinControllBase.h"

class C_WinScrollBar: public C_WinControllBase
{
   public:
      C_WinScrollBar( void );
      ~C_WinScrollBar( void );
      bool Create( HWND HwndParent, DWORD id, std::string title, short x, short y, short width, short height, bool bIsVertical = true );

   protected:
   private:
};

#endif
