#ifndef __C_WinListBox_H
#define __C_WinListBox_H

#include "../C_WinControllBase.h"

class C_WinListBox: public C_WinControllBase
{
   public:
      C_WinListBox( void );
      ~C_WinListBox( void ){}
      bool Create( HWND, DWORD, std::string="", short=0, short=0, short = 100, short = 100 );
      bool SetFont( HFONT );
   protected:
   private:
};

#endif
