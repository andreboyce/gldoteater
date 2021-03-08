#ifndef __C_WinStatic_H
#define __C_WinStatic_H

#include "../C_WinControllBase.h"

class C_WinStatic: public C_WinControllBase
{
   public:
      C_WinStatic( void );
      ~C_WinStatic( void ){}
      bool Create( HWND, DWORD, std::string, short, short, short = 100, short = 100, bool = true );
      bool SetFont( HFONT );
      bool SetText( std::string text );
      bool Resize( short width, short height, short x=0, short y=0 );
      std::string GetText( void );

   protected:
      bool m_bresize_for_text;
   private:
};

#endif
