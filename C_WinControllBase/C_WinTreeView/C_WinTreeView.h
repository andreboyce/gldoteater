#ifndef __C_TreeView_H
#define __C_TreeView_H

#include "../C_WinControllBase.h"

class C_WinTreeView: public C_WinControllBase
{
   public:
       C_WinTreeView( void );
      ~C_WinTreeView( void );
       bool Create( HWND, DWORD, std::string="", short=0, short=0, short = 100, short = 100 );

   protected:

   private:

};

#endif
