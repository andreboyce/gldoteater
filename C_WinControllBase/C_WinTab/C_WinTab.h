#ifndef __C_WinTab_H
#define __C_WinTab_H

#include "../C_WinControllBase.h"

class C_WinTab: public C_WinControllBase
{
   public:
       C_WinTab( void );
      ~C_WinTab( void );
      bool Create( HWND, DWORD, V_String, std::string="", short=0, short=0, short = 100, short = 100 );
      bool RenameTab( std::string text, int index );
      bool AddTab( std::string text, int index );
      bool RemoveTab( int index );
      int  NumberOfTabs( void );
      void SelectTab( int index );
      
   protected:
      void Free( void );
       
   private:
};

#endif
