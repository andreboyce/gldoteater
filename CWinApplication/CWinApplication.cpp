#include "CWinApplication.h"

HWND CWinApplication::m_LastHwndPassed = NULL;

CWinApplication::CWinApplication( DWORD dwWndClassStyle, char ClassName[], DWORD dwStyle, DWORD dwExStyle ) :
   m_WinBase( dwWndClassStyle, ClassName, dwStyle, dwExStyle, this )
{
   AddMessageHandler( WM_DESTROY, reinterpret_cast<MessageHandler>(CWinApplication::WinBaseOnDestroy) );
   AddMessageHandler( WM_CLOSE,   reinterpret_cast<MessageHandler>(CWinApplication::WinBaseOnClose)   );
   //AddErrorString( ERROR_CNRWC, "Could Not Register Window Class" );
   //AddErrorString( ERROR_CNCW,  "Could Not Create Window        " );
   //AddErrorString( ERROR_CNSW,  "Could Not Show Window          " );
   //AddErrorString( ERROR_CNDW,  "Could Not Destroy Window       " );
   FillWndClass();
   m_WinBase.m_dwStyle |= WS_VISIBLE | WS_OVERLAPPEDWINDOW;
}

CWinApplication::~CWinApplication( void )
{
   ::SetWindowLong( m_WinBase, GWL_USERDATA, (long)0 );
}

//***************************************************************************
//******************************* CWinApplication::WinBaseOnClose **********
// Accepts: CWinBase & window, WPARAM wParam, LPARAM lParam
//
// Purpose: Send a WM_DESTROY message to the window specified by CWinBase::Hwnd
//
// Returns: If the function succeeds, the return value is nonzero.
//          If the function fails, the return value is zero.

LRESULT CWinApplication::WinBaseOnClose( CWinApplication & window, WPARAM wParam, LPARAM lParam )
{
/* TODO (#1#): fix the bug ... make it so when you close the main 
               window the app closes */

   window.m_WinBase.m_iNumberOfWindows = 0;
   return ::DestroyWindow( window );
}

//***************************************************************************
//******************************* CWinApplication::WInBaseOnDestroy ********
// Accepts: CWinBase Reference :  WPARAM , LPARAM from WM_DESTROY
//
// Purpose: Decrement the number of windows, Post a WM_QUIT if they are no 
//          remaining windows
//
// Returns: LRESULT

LRESULT CWinApplication::WinBaseOnDestroy( CWinApplication & window, WPARAM wParam, LPARAM lParam )
{
   window.m_WinBase.m_iNumberOfWindows--;
   if( window.m_WinBase.m_iNumberOfWindows < 0 )
   {
      ::PostQuitMessage( 0 );
   }
   return 0;
}

//***************************************************************************
//******************************* CWinApplication::::FillWndClass **********
// Accepts: void
//
// Purpose: Fill the WNDCLASS with default values
//
// Returns: void


void CWinApplication::FillWndClass( void )
{
   m_WinBase.m_wincl.hInstance      = m_Hinstance = GetModuleHandle( NULL );
   m_WinBase.m_wincl.lpszClassName  = m_WinBase.m_WndClassName.c_str();
   m_WinBase.m_wincl.lpfnWndProc    = MessageRouterProc;
   m_WinBase.m_wincl.style          = CS_DBLCLKS;
   m_WinBase.m_wincl.cbSize         = sizeof( WNDCLASSEX );
   m_WinBase.m_wincl.hIcon          = ::LoadIcon( NULL, IDI_APPLICATION );
   m_WinBase.m_wincl.hIconSm        = ::LoadIcon( NULL, IDI_APPLICATION );
   m_WinBase.m_wincl.hCursor        = ::LoadCursor( NULL, IDC_ARROW );
   m_WinBase.m_wincl.lpszMenuName   = NULL;
   m_WinBase.m_wincl.cbClsExtra     = 0;
   m_WinBase.m_wincl.cbWndExtra     = 0;
   m_WinBase.m_wincl.hbrBackground  = (HBRUSH)::GetStockObject( BLACK_BRUSH );//(HBRUSH) COLOR_BACKGROUND;//
}

//***************************************************************************
//******************************* CWinBase::AddMessageHandler **************
// Accepts: Message ID Associated with Handler, Message Handler
//
// Purpose: Associate a message handler with a message ID
//
// Returns: true if it was added
//          false if it was not added

bool CWinApplication::AddMessageHandler( UINT msg, MessageHandler MsgHandler )
{
   m_MsgHandlerMap[ msg ] = MsgHandler;
   return ( GetMessageHandler( msg ) ) ? true: false;
}

//***************************************************************************
//******************************* CWinBase::GetMessageHandler **************
// Accepts: Message ID Associated with Handler
//
// Purpose: Find the message Handler Associated with the message ID
//
// Returns: CWinBase::MessageHandler if it was found
//          NULL if it was not found

CWinApplication::MessageHandler CWinApplication::GetMessageHandler( UINT msg )
{
   MessageHandlerMapIterator MsgHandlerIterator = m_MsgHandlerMap.find( msg );

   // If the handler is not found return NULL
   return ( MsgHandlerIterator != m_MsgHandlerMap.end() )
            ? MsgHandlerIterator->second : NULL;
}

//***************************************************************************
//******************************* CWinApplication::::MessageRouterProc ******
// Accepts: HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam
//
// Purpose: Call the apporiate message handler for the message ID
//
// Returns: LRESULT

LRESULT CALLBACK CWinApplication::MessageRouterProc( HWND hwnd, UINT message,
                                                     WPARAM wparam, LPARAM lparam )
{
   // attempt to retrieve Window pointer
   CWinApplication * wnd = reinterpret_cast< CWinApplication *>( ::GetWindowLong( hwnd, GWL_USERDATA ) );
   m_LastHwndPassed = hwnd;

   // TODO (#1#): Optimisations
   if( wnd != NULL )
   {
      MessageHandler handler = wnd->GetMessageHandler( message );
	  if( handler )
	  {
		 return (*handler)( *wnd, wparam, lparam );
	  }
      return wnd->m_WinBase.DefaultWindowProcedure( hwnd, message, wparam, lparam );
   }
   else if( message == WM_NCCREATE )
   {
     // attempt to store CWinApplication
     wnd = reinterpret_cast< CWinApplication * >( ((LPCREATESTRUCT)lparam)->lpCreateParams );
     ::SetWindowLong( hwnd, GWL_USERDATA, reinterpret_cast< long >( wnd ) );
   }
   return ::DefWindowProc( hwnd, message, wparam, lparam );
}
