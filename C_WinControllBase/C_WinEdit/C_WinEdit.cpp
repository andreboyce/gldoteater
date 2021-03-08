#include "C_WinEdit.h"

C_WinEdit::C_WinEdit( void ):
   C_WinControllBase( "EDIT", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
                              ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 
                              WS_EX_CLIENTEDGE )
{
}

C_WinEdit::~C_WinEdit( void )
{
}

bool C_WinEdit::Create( HWND HwndParent, DWORD id, std::string title, short x, short y, short width, short height )
{
   m_HwndParent = HwndParent;
   return (bool)CWinBase::Create( (char*)title.c_str(), HwndParent, x, y, width, height, (HMENU)id, 0 );

//   SetFont( (HFONT) ::GetStockObject( DEFAULT_GUI_FONT ) );
   return true;
}

bool C_WinEdit::SetFont( HFONT hFont )
{
   return ::SendMessage( m_Hwnd, WM_SETFONT, (WPARAM)hFont,
                         MAKELPARAM(FALSE, 0) );
}

std::string C_WinEdit::GetText( void )
{
   DWORD dwTextLength = ::GetWindowTextLength( m_Hwnd );
   std::string windowtext;
   if( dwTextLength > 0 )
   {
      LPSTR pszText;
      DWORD dwBufferSize = dwTextLength + 1;
      pszText = (LPSTR) ::GlobalAlloc( GPTR, dwBufferSize );
      if( pszText != NULL )
      {
         if( ::GetWindowText( m_Hwnd, pszText, dwBufferSize ) )
         {
            windowtext = pszText;
         }
      }
      ::GlobalFree( pszText );
   }
   return windowtext;
}

bool C_WinEdit::SetText( std::string new_text )
{
   return (bool)::SetWindowText( m_Hwnd, (LPCTSTR) new_text.c_str() );
}
