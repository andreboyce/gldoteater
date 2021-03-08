#ifndef __CXMLMenu_H
#define __CXMLMenu_H

#include <vector>
#include <string>
#include <map>
#include <windows.h>

#include "../TinyXML/TinyXML.h"
#include "../errorutil/errorutil.h"

//#define FIRST_MENU_COMMAND_VALUE 3000

class CXMLMenu
{
   public:

       CXMLMenu( const TiXmlElement &, const DWORD = 0 );
       CXMLMenu( HWND, std::string, DWORD, bool bisPopupMenu = false );
       CXMLMenu( void ) : m_Hwnd( NULL ),
                           m_MenuName( "" ),
                           m_bisPopupMenu( false ),
                           m_FIRST_MENU_COMMAND_VALUE( 0 ),
                           m_bAssignedToWindow( false ),
                           m_HmenuParent( NULL ) {}
      ~CXMLMenu( void );
       bool AssembleMenu( const TiXmlElement &parentmenuElement );
       bool AssembleMenu( const TiXmlElement &, const DWORD& );
       bool LoadMenu( const std::string &, const DWORD, const bool bisPopupMenu = false );
       bool LoadMenuFromString( const std::string &, const DWORD&, bool bisPopupMenu = false );

       bool  SetMenu( HWND hwnd )
       {
          m_Hwnd = hwnd;
          if( m_Hwnd && m_HmenuParent )
          {
             ::SetMenu( m_Hwnd, m_HmenuParent );
             m_bAssignedToWindow = true;
             return true;
          }
          return false;
       }

       inline DWORD GetFirstCommand ( void ) const
       {
          return m_FIRST_MENU_COMMAND_VALUE;
       }

       inline HWND GetHwnd ( void ) const
       {
          return m_Hwnd;
       }

       operator HMENU ( void ) const
       {
          return m_HmenuParent;
       }
       
//       void operator () ( HWND Hwnd, std::string MenuName, DWORD FIRST_MENU_COMMAND_VALUE, bool bisPopupMenu = false )
//       {
//          m_bisPopupMenu             = bisPopupMenu;
//          m_Hwnd                     = Hwnd;
//          m_MenuName                 = MenuName;
//          m_FIRST_MENU_COMMAND_VALUE = FIRST_MENU_COMMAND_VALUE;
//          AssembleMenu();
//       }

       void operator () ( HWND Hwnd )
       {
          m_Hwnd = Hwnd;
       }

/*       void operator () ( const std::string & MenuName, DWORD FIRST_MENU_COMMAND_VALUE, bool bisPopupMenu = false )
       {
          m_bisPopupMenu             = bisPopupMenu;
          m_FIRST_MENU_COMMAND_VALUE = FIRST_MENU_COMMAND_VALUE;
          m_MenuName                 = MenuName;

          if( m_HmenuParent )
          {
             if( m_bAssignedToWindow )
             {
                ::SetMenu( m_Hwnd, NULL );
             }
             ::DestroyMenu( m_HmenuParent );
          }
          AssembleMenu();
       }
*/
      std::map< int, std::string > m_MenuItemHash;

   protected:

      void  HandleMenus( TiXmlElement *, HMENU, int & );

      HWND         m_Hwnd;
      std::string  m_MenuName;
      HMENU        m_HmenuParent;
      bool         m_bisPopupMenu;
      bool         m_bAssignedToWindow;
      DWORD        m_FIRST_MENU_COMMAND_VALUE;

   private:
};

#endif
