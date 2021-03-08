#ifndef __C_WinToolTip_H
#define __C_WinToolTip_H

#include "../C_WinControllBase.h"
#include "../../Structs/Structs.h"

class C_WinToolTip: public C_WinControllBase
{
   public:

       C_WinToolTip( void );
      ~C_WinToolTip( void );

       bool Create( HWND hwndparent, DWORD id, Structs::Rect rect, std::string text, std::string title="", short=0, short=0, short = 100, short = 100 );
       bool Activate( int, int );
       void SetRect( Structs::Rect rect );

   protected:
      TOOLINFO        m_ti;
      Structs::Rect m_rect;
      std::string     m_Text;

   private:

};

#endif

