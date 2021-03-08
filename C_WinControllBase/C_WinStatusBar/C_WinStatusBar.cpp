#include "C_WinStatusBar.h"

C_WinStatusBar::C_WinStatusBar( void ):              
                C_WinControllBase( STATUSCLASSNAME, WS_CHILD | WS_VISIBLE | WS_BORDER | SBARS_SIZEGRIP, 0 )
{
   //AddErrorString( ERROR_TMPTA, "Too Many Parts To Add" );
   m_bisSimpleMode = false;
}

C_WinStatusBar::~C_WinStatusBar( void )
{}

bool C_WinStatusBar::Create( HWND HwndParent, DWORD id, V_Parts& parts, std::string title, short x, short y, short width, short height )
{
   m_HwndParent = HwndParent;
   
   CWinBase::Create( (char*)title.c_str(), HwndParent, x, y, width, height, (HMENU)id, 0 );
   if( m_Hwnd )
   {
      validateparts( parts );
      InitializeStatusBar();
      return true;
   }
   return false;
}

void C_WinStatusBar::UpdateStatusBar( std::string StatusString, WORD partNumber, WORD displayFlags )
{
   ::SendMessage( m_Hwnd, SB_SETTEXT, partNumber | displayFlags, (LPARAM)StatusString.c_str() );
}

void C_WinStatusBar::InitializeStatusBar( void )
{
   if( !m_bisSimpleMode )
   {
      RECT rect;

      std::vector<int> partsArray( m_parts.size() );
	  for( int i=0; i < partsArray.size() ; i++ )
	  {
		  partsArray[i] = 0;
	  }

      ::GetClientRect( m_HwndParent, &rect );

      int width      = 0; 
      int totalWidth = rect.right;
      int prevValue  = 0;
      for( int i=0; (i < m_parts.size()) && (i < partsArray.size()) ; i++ )
      {
         double t = (m_parts.at(i).width*totalWidth)/100.0;
         partsArray[i] = static_cast<int>(std::floor(t))+prevValue;
         prevValue = partsArray[i];
      }

      if( m_parts.size() )
      {
         partsArray[m_parts.size()-1] = rect.right;
      }

	  if( m_parts.size() )
	  {
         ::SendMessage( m_Hwnd, SB_SETPARTS, m_parts.size(), (LPARAM)(LPINT)(&partsArray[0]) );
	  }
	  else
	  {
         ::SendMessage( m_Hwnd, SB_SETPARTS, m_parts.size(), (LPARAM)(LPINT)(NULL) );
	  }

      for( int c=0 ; c<m_parts.size() ; c++ )
      {
         UpdateStatusBar( m_parts.at(c).text, c, 0 );
      }
   }
}

void C_WinStatusBar::SetParts( V_Parts &parts )
{
   m_parts = parts;
}

void C_WinStatusBar::validateparts( V_Parts parts )
{
   int totalpercentage = 0;
   for( int c=0 ; c<parts.size() ; c++ )
   {
      totalpercentage += parts.at(c).width;
   }

   if( totalpercentage > 100 || totalpercentage < 100 )
   {
      SetEqualParts( parts );
   }
   else
   {
      m_parts = parts;
   }
}

void C_WinStatusBar::SetEqualParts( V_Parts NumberOfParts )
{
   m_parts.clear();
   for( int i=0 ; i<NumberOfParts.size() ; i++ )
   {
      // Assigns an equal percentage
      m_parts.push_back( S_Part( NumberOfParts.at(i).text, (short)(100/NumberOfParts.size()) ) ); // of space to each part
   }
}

void C_WinStatusBar::SetSimpleModeText( std::string text )
{
   m_SimpleModeText = text;
}

void C_WinStatusBar::EnableSimpleMode( std::string SimpleModeText )
{
   m_SimpleModeText = SimpleModeText;
   EnableSimpleMode();
}

void C_WinStatusBar::EnableSimpleMode( void )
{
   ::SendMessage( m_Hwnd, SB_SIMPLE, true, 0 );
   ::SendMessage( m_Hwnd, SB_SETTEXT, 255, (LPARAM)(LPSTR) m_SimpleModeText.c_str() );
   m_bisSimpleMode = true;
}

void C_WinStatusBar::DisableSimpleMode( void )
{
   ::SendMessage( m_Hwnd, SB_SIMPLE, false, 0 );
   m_bisSimpleMode = false;
}

