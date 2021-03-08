#ifndef __CTrackPopupMenu_H
#define __CTrackPopupMenu_H

#include "../errorutil/errorutil.h"
#include "../TinyXML/TinyXML.h"

class CTrackPopupMenu
{
   public:
      CTrackPopupMenu( void ) :
         m_Hwnd( NULL ),
         m_HmenuParent( NULL ),
         m_bAssignedToWindow( false ),
         m_FIRST_MENU_COMMAND_VALUE( 0 )
      {}
      ~CTrackPopupMenu( void )
      {
//         ::DestroyMenu( m_XMLMenu );
      }
      void Display( int x, int y );

      bool Load( HWND Hwnd, const std::string& menufile, DWORD dwFirstCommandID );

      bool AssembleMenu( TiXmlElement &parentmenuElement );
      bool AssembleMenu( TiXmlElement &, const DWORD );
      bool LoadMenu( const std::string &, const DWORD );
      bool LoadMenuFromString( const std::string &, const DWORD );

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

      inline DWORD GetFirstCommand ( void )// const
      {
         return m_FIRST_MENU_COMMAND_VALUE;
      }

      inline void SetHwnd ( HWND Hwnd )
      {
         m_Hwnd = Hwnd;
      }

      inline HWND GetHwnd ( void )// const
      {
         return m_Hwnd;
      }

      operator HMENU ( void )// const
      {
         return m_HmenuParent;
      }       

   protected:

      void  HandleMenus( TiXmlElement *, HMENU, int & );

      std::map< int, std::string > m_MenuItemHash;
      HWND         m_Hwnd;
      std::string  m_MenuName;
      HMENU        m_HmenuParent;
      bool         m_bAssignedToWindow;
      DWORD        m_FIRST_MENU_COMMAND_VALUE;

   private:

};

#endif
