#ifndef __C_WinEdit_H
#define __C_WinEdit_H

#include "../C_WinControllBase.h"

class C_WinEdit: public C_WinControllBase
{
   public:

       C_WinEdit( void );
       C_WinEdit( const C_WinEdit& rhs ) :
          C_WinControllBase(rhs)
       {}
      ~C_WinEdit( void );

       bool Create( HWND, DWORD, std::string, short=0, short=0, short = 100, short = 100 );
       bool SetFont( HFONT );
       std::string GetText( void );
       bool SetText( std::string );

       C_WinEdit& operator = ( const C_WinEdit & rhs )
       {
          C_WinControllBase::operator = (rhs);
          return *this;
       }

//       std::string operator << ( std::string text_to_append )
//       {
//          return GetText( void );
//       }

//       std::string operator >> ()
//       {
//          return GetText( void );
//       }
//

   protected:

   private:
};

#endif
