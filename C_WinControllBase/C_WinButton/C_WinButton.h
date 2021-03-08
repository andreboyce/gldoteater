#ifndef __C_WinButton_H
#define __C_WinButton_H

#include "../C_WinControllBase.h"

class C_WinButton: public C_WinControllBase
{
   public:

       C_WinButton( void );
       C_WinButton( const C_WinButton & );
       virtual ~C_WinButton( void );
       std::string GetText( void );
       bool Resize( short width, short height, short x=0, short y=0 );
       bool Create( HWND, DWORD, std::string, short, short, short = 100, short = 100, bool=0 );
       C_WinButton& operator = ( const C_WinButton & RHS)
       {
          try
          {
             C_WinControllBase::operator = (RHS);
//      *dynamic_cast<C_WinControllBase*>(this) = dynamic_cast<const C_WinControllBase&>(RHS);
          }
          catch(...){}
          SetText( m_WindowTitle );
          m_bresize_for_text = RHS.m_bresize_for_text;
          return *this;
       }

   protected:
      bool m_bresize_for_text;

   private:

};

#endif

