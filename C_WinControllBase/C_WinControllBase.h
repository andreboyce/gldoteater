#ifndef __C_WinControllBase_H
#define __C_WinControllBase_H

//#define WIN32_LEAN_AND_MEAN
#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include "../CWinBase/CWinBase.h"

class C_WinControllBase: public CWinBase
{
   friend class C_WinButton;
   public:
      typedef std::vector< std::string > V_String;

       virtual ~C_WinControllBase( void )
          {
             if( ::IsWindow( m_Hwnd ) )
                ::DestroyWindow( m_Hwnd );
          }

       void LoadCommonControlls( void );

       virtual bool Resize( short width, short height, short x=0, short y=0 )
       {
          return ::SetWindowPos( m_Hwnd, NULL, x, y, width, height, SWP_NOZORDER );
       }

      operator HWND ( void ) const
         {
            return m_Hwnd;
         }

   protected:
       C_WinControllBase( const C_WinControllBase& RHS ) :
          CWinBase( RHS ),
          m_HwndParent( RHS.m_HwndParent ),
          m_Strings( RHS.m_Strings )          
       {
          m_bisControll = true; // prevent CWinBase::Create() from trying to register a WNDCLASS
          LoadCommonControlls();
       }
       C_WinControllBase( void ) :
             m_HwndParent( NULL ),
             m_Strings()
          {
             m_bisControll = true; // prevent CWinBase::Create() from trying to register a WNDCLASS
             LoadCommonControlls();
          }

       C_WinControllBase( char WndClassName[], DWORD dwStyle, DWORD dwStyleEx = 0 ) :
             CWinBase( WndClassName, dwStyle, dwStyleEx ),
             m_HwndParent( NULL ),
             m_Strings()
          {
             m_bisControll = true; // prevent CWinBase::Create() from trying to register a WNDCLASS
             LoadCommonControlls();
          }

       C_WinControllBase& operator = ( const C_WinControllBase & RHS )
       {
          try
          {
             CWinBase::operator = (RHS);
//             *dynamic_cast<CWinBase*>(this) = dynamic_cast<const CWinBase&>(RHS);
          }
          catch(...)
          {}
          m_HwndParent            = RHS.m_HwndParent;
          m_Strings               = RHS.m_Strings;
          return *this;
       }

      HWND        m_HwndParent;
      static bool m_bCommonControlsLoaded;
      V_String    m_Strings;

   private:
};

#endif

