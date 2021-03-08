#include "CTrackPopupMenu.h"

void CTrackPopupMenu::Display( int x, int y )
{
   POINT pt = {x,y};
   ::ClientToScreen( GetHwnd(), &pt );
   ::TrackPopupMenuEx( m_HmenuParent, 0, pt.x, pt.y, GetHwnd(), NULL );
}

bool CTrackPopupMenu::AssembleMenu( TiXmlElement &parentmenuElement )
{
   DWORD commandvalue = 0;
   parentmenuElement.FirstChildElement( "POPUP" )->FirstChildElement( "MENUITEM" )->QueryIntAttribute( "value", (int*)&commandvalue);
   return AssembleMenu( parentmenuElement, commandvalue );
}

// TiXmlElement &parentmenuElement should be menu.FirstChildElement( "MENU" );
// where menu is a TiXmlDocument

bool CTrackPopupMenu::AssembleMenu( TiXmlElement &parentmenuElement, const DWORD command_value )
{
   if( !m_HmenuParent ) // Load the menu once
   {
      m_HmenuParent = ::CreatePopupMenu();

      assert( m_HmenuParent );

      int InitalMenuOnCommandValue = m_FIRST_MENU_COMMAND_VALUE = command_value;

      TiXmlElement *popupmenuElement  = parentmenuElement.FirstChildElement();
      if( popupmenuElement == NULL )
      {
         return false;
      }

      HandleMenus( popupmenuElement, m_HmenuParent, InitalMenuOnCommandValue );
   }
   return true;   
}

bool CTrackPopupMenu::Load( HWND Hwnd, const std::string& menufile, DWORD dwFirstCommandID )
{
   m_Hwnd = Hwnd;
   return LoadMenu( menufile, dwFirstCommandID );
}

bool CTrackPopupMenu::LoadMenu( const std::string & menufile, const DWORD command_value )
{
   TiXmlDocument menu( menufile.c_str() );
   if ( !menu.LoadFile() )
   {
      LogError( "Could Not Load Menu File" );
   }
   TiXmlElement *parentmenuElement = menu.FirstChildElement( "MENU" );
   //assert( parentmenuElement );
   return ( AssembleMenu( *parentmenuElement, command_value ) && menu.SaveFile() );
}

bool CTrackPopupMenu::LoadMenuFromString( const std::string &menustring, const DWORD command_value )
{
   TiXmlDocument menu;
/*   if ( !menu.LoadFromString( menustring ) )
   {
      LogError( "Could Not Load Menu File" );
   }
   TiXmlElement *parentmenuElement = menu.FirstChildElement( "MENU" );
   assert( parentmenuElement );
   return ( AssembleMenu( *parentmenuElement, command_value ) && menu.SaveFile() );*/
   return false;
}

void CTrackPopupMenu::HandleMenus( TiXmlElement *parentmenuElement, HMENU HmenuParent,
                              int &wm_command_value )
{
   TiXmlElement *nextElement     = parentmenuElement;
   TiXmlElement *menuitemElement = NULL;
   TiXmlText    *menuitemText    = NULL;

   for( ;
        nextElement != NULL;
        nextElement  = nextElement->NextSiblingElement() )
   {
      if( !strcmpi( nextElement->Value(), "POPUP" ) )
      {
         HMENU hSubMenu = ::CreatePopupMenu();
         ::AppendMenu( HmenuParent, MF_STRING | MF_POPUP, (UINT)hSubMenu,
                       nextElement->Attribute( "name" ) );

         HandleMenus( nextElement->FirstChildElement(), hSubMenu,
                      wm_command_value );
      }

      else if( !strcmpi( nextElement->Value(), "MENUITEM" ) )
      {
         nextElement->SetAttribute( "value" , wm_command_value );
         menuitemText = (nextElement->FirstChild())->ToText();
         m_MenuItemHash[ wm_command_value ] = menuitemText->Value();
         if(menuitemText)
         {
            ::AppendMenu( HmenuParent, MF_STRING, wm_command_value++,
                          menuitemText->Value() );
         }
      }
   }
}


