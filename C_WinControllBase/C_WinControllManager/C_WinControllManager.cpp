#include "C_WinControllManager.h"

C_WinControllManager::C_WinControllManager( void ) :
   m_WinBase( NULL )
{
}

C_WinControllManager::~C_WinControllManager( void )
{
   for( int i=0; i<WinButtons.size(); i++)
   {
      delete WinButtons.at( i );
//      LogError( "deleted" );
   }
   for( int i=0; i<WinEdits.size(); i++)
   {
      delete WinEdits.at( i );
//      LogError( "deleted" );
   }
   for( int i=0; i<WinStatics.size(); i++)
   {
      delete WinStatics.at( i );
//      LogError( "deleted" );
   }
   for( int i=0; i<WinListBoxes.size(); i++)
   {
      delete WinListBoxes.at( i );
//      LogError( "deleted" );
   }
   for( int i=0; i<WinToolTips.size(); i++)
   {
      delete WinToolTips.at( i );
//      LogError( "deleted" );
   }
   for( int i=0; i<WinTabs.size(); i++)
   {
      delete WinTabs.at( i );
//      LogError( "deleted" );
   }
   for( int i=0; i<WinStatusBars.size(); i++)
   {
      delete WinStatusBars.at( i );
//      LogError( "deleted" );
   }
   for( int i=0; i<WinTreeViews.size(); i++)
   {
      delete WinTreeViews.at( i );
//      LogError( "deleted" );
   }
   for( int i=0; i<WinToolBars.size(); i++)
   {
      delete WinToolBars.at( i );
//      LogError( "deleted" );
   }
   for( int i=0; i<WinProgressBars.size(); i++)
   {
      delete WinProgressBars.at( i );
//      LogError( "deleted" );
   }
}

bool C_WinControllManager::AddButton( const ControllProperties& cp )
{
   try
   {
      WinButtons.push_back( new C_WinButton );
      if( !WinButtons.size() )
         throw( std::string( "Bad Allocation of memory" ) );
      if( !(*WinButtons.at( WinButtons.size()-1 )).Create( m_Hwnd, cp.m_id, cp.m_title, cp.m_x, cp.m_y, cp.m_width, cp.m_height, cp.m_bresizetofittext ) )
      {
//         Alert( "Window Not Created" );
      }
   }
   catch( std::string errmsg )
   {
      LogError( errmsg );
   }
   catch(...){}
}

bool C_WinControllManager::AddEdit( const ControllProperties& cp )
{
   try
   {
      WinEdits.push_back( new C_WinEdit );
      if( !WinEdits.size() )
         throw( std::string( "Bad Allocation of memory" ) );
      if( !(*WinEdits.at( WinEdits.size()-1 )).Create( m_Hwnd, cp.m_id, cp.m_title, cp.m_x, cp.m_y, cp.m_width, cp.m_height ) )
      {
         Alert( "Window Not Created" );
      }
   }
   catch( std::string errmsg )
   {
      LogError( errmsg );
   }
   catch(...){}
}

bool C_WinControllManager::AddStatic( const ControllProperties& cp )
{
   try
   {
      WinStatics.push_back( new C_WinStatic );
      if( !WinStatics.size() )
         throw( std::string( "Bad Allocation of memory" ) );
      if( !(*WinStatics.at( WinStatics.size()-1 )).Create( m_Hwnd, cp.m_id, cp.m_title, cp.m_x, cp.m_y, cp.m_width, cp.m_height ) )
      {
         Alert( "Window Not Created" );
      }
   }
   catch( std::string errmsg )
   {
      LogError( errmsg );
   }
   catch(...){}
}

bool C_WinControllManager::AddUpDown( const ControllProperties& cp )
{
   try
   {
      WinUpDowns.push_back( new C_WinUpDown );
      if( !WinUpDowns.size() )
         throw( std::string( "Bad Allocation of memory" ) );
      if( !(*WinUpDowns.at( WinUpDowns.size()-1 )).Create( m_Hwnd, cp.m_HwndBuddy, cp.m_id, cp.m_title, cp.m_x, cp.m_y, cp.m_width, cp.m_height ) )
      {
         Alert( "Window Not Created" );
      }
   }
   catch( std::string errmsg )
   {
      LogError( errmsg );
   }
   catch(...){}
}

bool C_WinControllManager::AddListBox( const ControllProperties& cp )
{
   try
   {
      WinListBoxes.push_back( new C_WinListBox );
      if( !WinListBoxes.size() )
         throw( std::string( "Bad Allocation of memory" ) );
      if( !(*WinListBoxes.at( WinListBoxes.size()-1 )).Create( m_Hwnd, cp.m_id, cp.m_title, cp.m_x, cp.m_y, cp.m_width, cp.m_height ) )
      {
         Alert( "Window Not Created" );
      }
   }
   catch( std::string errmsg )
   {
      LogError( errmsg );
   }
   catch(...){}
}

bool C_WinControllManager::AddStatusBar( const ControllProperties& cp )
{
   try
   {
      C_WinStatusBar::V_Parts parts;
      WinStatusBars.push_back( new C_WinStatusBar );
      if( !WinStatusBars.size() )
         throw( std::string( "Bad Allocation of memory" ) );
      if( !(*WinStatusBars.at( WinStatusBars.size()-1 )).Create( m_Hwnd, cp.m_id, parts, cp.m_title, cp.m_x, cp.m_y, cp.m_width, cp.m_height ) )
      {
         Alert( "Window Not Created" );
      }
   }
   catch( std::string errmsg )
   {
      LogError( errmsg );
   }
   catch(...){}
}

bool C_WinControllManager::AddTab( const ControllProperties& cp )
{
   try
   {
      V_String s;
      WinTabs.push_back( new C_WinTab );
      if( !WinTabs.size() )
         throw( std::string( "Bad Allocation of memory" ) );
      if( !(*WinTabs.at( WinTabs.size()-1 )).Create( m_Hwnd, cp.m_id, s, cp.m_title, cp.m_x, cp.m_y, cp.m_width, cp.m_height ) )
      {
         Alert( "Window Not Created" );
      }
   }
   catch( std::string errmsg )
   {
      LogError( errmsg );
   }
   catch(...){}
}

bool C_WinControllManager::AddToolBar( const ControllProperties& cp )
{
   try
   {
      C_WinToolBar::L_TBItems TBBStrings;
      WinToolBars.push_back( new C_WinToolBar );
      if( !WinToolTips.size() )
         throw( std::string( "Bad Allocation of memory" ) );
      if( !(*WinToolBars.at( WinToolBars.size()-1 )).Create( m_Hwnd, cp.m_id, TBBStrings, 13, cp.m_title, cp.m_x, cp.m_y, cp.m_width, cp.m_height ) )
      {
         Alert( "Window Not Created" );
      }
   }
   catch( std::string errmsg )
   {
      LogError( errmsg );
   }
   catch(...){}
}

bool C_WinControllManager::AddToolTip( const ControllProperties& cp )
{
   try
   {
      RECT rect;
      rect.left   = 0;
      rect.right  = 500;
      rect.top    = 0;
      rect.bottom = 500;
      WinToolTips.push_back( new C_WinToolTip );
      if( !WinToolTips.size() )
         throw( std::string( "Bad Allocation of memory" ) );
      if( !(*WinToolTips.at( WinToolTips.size()-1 )).Create( m_Hwnd, cp.m_id, rect, "", cp.m_title, cp.m_x, cp.m_y, cp.m_width, cp.m_height ) )
      {
         Alert( "Window Not Created" );
      }
   }
   catch( std::string errmsg )
   {
      LogError( errmsg );
   }
   catch(...){}
}

bool C_WinControllManager::AddTreeView( const ControllProperties& cp )
{
   try
   {
      WinTreeViews.push_back( new C_WinTreeView );
      if( !WinTreeViews.size() )
         throw( std::string( "Bad Allocation of memory" ) );
      if( !(*WinTreeViews.at( WinTreeViews.size()-1 )).Create( m_Hwnd, cp.m_id, cp.m_title, cp.m_x, cp.m_y, cp.m_width, cp.m_height ) )
      {
         Alert( "Window Not Created" );
      }
   }
   catch( std::string errmsg )
   {
      LogError( errmsg );
   }
   catch(...){}
}

bool C_WinControllManager::AddProgressBar( const ControllProperties& cp )
{
   try
   {
      WinProgressBars.push_back( new C_WinProgressBar );
      if( !WinProgressBars.size() )
         throw( std::string( "Bad Allocation of memory" ) );
      if( !(*WinProgressBars.at( WinProgressBars.size()-1 )).Create( m_Hwnd, cp.m_id, cp.m_title, cp.m_x, cp.m_y, cp.m_width, cp.m_height ) )
      {
         Alert( "Window Not Created" );
      }
   }
   catch( std::string errmsg )
   {
      LogError( errmsg );
   }
   catch(...){}
}

