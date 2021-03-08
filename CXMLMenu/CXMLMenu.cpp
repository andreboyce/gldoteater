#include "CXMLMenu.h"

CXMLMenu::CXMLMenu( HWND Hwnd, std::string MenuName, DWORD FIRST_MENU_COMMAND_VALUE, bool bisPopupMenu ) : 
                      m_Hwnd( Hwnd ),
                      m_MenuName( MenuName ), 
                      m_HmenuParent( NULL ), 
                      m_bisPopupMenu( bisPopupMenu ),
                      m_FIRST_MENU_COMMAND_VALUE( FIRST_MENU_COMMAND_VALUE ),
                      m_bAssignedToWindow( false )
{
   LoadMenu( MenuName, FIRST_MENU_COMMAND_VALUE );
}

// TiXmlElement &parentmenuElement should be menu.FirstChildElement( "MENU" );
// where menu is a TiXmlDocument

CXMLMenu::CXMLMenu( const TiXmlElement & parentmenuElement, DWORD command_value ) :
                      m_Hwnd( NULL ),
                      m_MenuName( "" ), 
                      m_HmenuParent( NULL ), 
                      m_bisPopupMenu( bool() ),
                      m_FIRST_MENU_COMMAND_VALUE( 0 ),
                      m_bAssignedToWindow( false )
{
   m_FIRST_MENU_COMMAND_VALUE = command_value;
   AssembleMenu( parentmenuElement, command_value );
}

CXMLMenu::~CXMLMenu( void )
{
   if( m_HmenuParent )
   {
      if( m_bAssignedToWindow )
      {
         ::DestroyMenu( m_HmenuParent );
      }      
   }
}

bool CXMLMenu::AssembleMenu( const TiXmlElement &parentmenuElement )
{
   DWORD commandvalue = 0;
   parentmenuElement.FirstChildElement( "POPUP" )->FirstChildElement( "MENUITEM" )->QueryIntAttribute( "value", (int*)&commandvalue);
   return AssembleMenu( parentmenuElement, commandvalue );
}

// TiXmlElement &parentmenuElement should be menu.FirstChildElement( "MENU" );
// where menu is a TiXmlDocument

bool CXMLMenu::AssembleMenu( const TiXmlElement &parentmenuElement, const DWORD& command_value )
{
   if( !m_HmenuParent ) // Load the menu once
   {
      m_bisPopupMenu = parentmenuElement.Attribute( "isPopup" );

      if( m_bisPopupMenu )
      {
         m_HmenuParent = m_HmenuParent = ::CreatePopupMenu();
      }
      else
      {
         m_HmenuParent = m_HmenuParent = ::CreateMenu();
      }

      assert( m_HmenuParent );

      int InitalMenuOnCommandValue = m_FIRST_MENU_COMMAND_VALUE = command_value;

      TiXmlElement *popupmenuElement  = (TiXmlElement *)parentmenuElement.FirstChildElement( "POPUP" );
      if( popupmenuElement == NULL )
      {
         return false;
      }

      HandleMenus( popupmenuElement, m_HmenuParent, InitalMenuOnCommandValue );
   }
   return true;   
}

bool CXMLMenu::LoadMenu( const std::string & menufile, const DWORD command_value, const bool bisPopupMenu )
{
   m_bisPopupMenu = bisPopupMenu;
   TiXmlDocument menu( menufile.c_str() );
   if ( !menu.LoadFile() )
   {
      LogError( "Could Not Load Menu File" );
   }
   TiXmlElement *parentmenuElement = menu.FirstChildElement( "MENU" );
   assert( parentmenuElement );
   return ( AssembleMenu( *parentmenuElement, command_value ) && menu.SaveFile() );
}

bool CXMLMenu::LoadMenuFromString( const std::string &menustring, const DWORD& command_value, bool bisPopupMenu )
{
   //m_bisPopupMenu = bisPopupMenu;
   //TiXmlDocument menu;
   //if ( !menu.LoadFromString( menustring ) )
   //{
   //   LogError( "Could Not Load Menu File" );
   //}
   //TiXmlElement *parentmenuElement = menu.FirstChildElement( "MENU" );
   //assert( parentmenuElement );
   //return ( AssembleMenu( *parentmenuElement, command_value ) && menu.SaveFile() );
   return false;
}

void CXMLMenu::HandleMenus( TiXmlElement *parentmenuElement, HMENU HmenuParent,
                              int &wm_command_value )
{
   TiXmlElement *nextElement = parentmenuElement;
   TiXmlElement *menuitemElement = NULL;
   TiXmlText    *menuitemText    = NULL;

   for( ;
        nextElement != NULL;
        nextElement = nextElement->NextSiblingElement() )
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

