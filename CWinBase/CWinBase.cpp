#include "CWinBase.h"

bool CWinBase::m_bisClassRegistered = false;
int  CWinBase::m_iNumberOfWindows   = 0;

//***************************************************************************
//******************************* CWinBase::Initialise *********************
// Accepts: ClassName, CreateWindowEx( DWORD dwstyleEx, dwstyle ...
//
// Purpose: Initialize all variables and Assign message handlers and error 
// strings
//
// Returns: void

void CWinBase::Initialise( std::string ClassName, DWORD dwStyle, DWORD dwExStyle, DWORD dwWndClassStyle, CWinApplication* WinApplication )
{
   ::ZeroMemory( &m_wincl, sizeof( m_wincl ) );
   m_Hinstance        = ::GetModuleHandle( NULL );
   m_WndClassName     = ClassName;
   m_bisVisible       = false;
   m_bisControll      = false;
   m_Hwnd             = NULL;
   m_WinApplication   = WinApplication;
   SetWndClassStyle( dwWndClassStyle );
   SetStyle( dwStyle );
   SetExStyle( dwExStyle );
}

//***************************************************************************
//******************************* CWinBase Constructor *********************
// Accepts: ClassName, CreateWindowEx( DWORD dwstyleEx, dwstyle ...
//
// Purpose: Initialize all variables and Assign message handlers and error 
// strings
//
// Returns: void

CWinBase::CWinBase( char ClassName[], DWORD dwStyle, DWORD dwExStyle, CWinApplication* WinApplication )
{
   Initialise( ClassName, dwStyle, dwExStyle, CS_DBLCLKS, WinApplication );
}

//***************************************************************************
//******************************* CWinBase Constructor *********************
// Accepts: WNDCLASS.style, ClassName
//
// Purpose: Initialize all variables and Assign message handlers and error 
// strings
//
// Returns: void

CWinBase::CWinBase( DWORD dwWndClassStyle, char ClassName[], DWORD dwStyle, DWORD dwExStyle, CWinApplication* WinApplication )
{
   Initialise( ClassName, 0, 0, dwWndClassStyle, WinApplication );
}

//***************************************************************************
//******************************* CWinBase Destructor **********************
// Accepts: void
// Purpose: Destroy the window and free memory.
// Returns: void

CWinBase::~CWinBase( void )
{
   if( ::IsWindow( m_Hwnd ) )
      ::DestroyWindow( m_Hwnd );
      
// TODO (#1#): Check the below code ... How will it cause problems 
//               with multiple instances 
      
//   if( m_iNumberOfWindows == 0 )
//   {
//      ::UnregisterClass( m_WndClassName.c_str(), m_Hinstance );
//   }
}


//***************************************************************************
//******************************* CWinBase::Create *************************
// Accepts: Title, Handle to Parent Window, x coord, y coord, width, height, 
//          Handle to Menu
//
// Purpose: Create the window and save the handle to it in CWinBase::Hwnd
//
// Returns: If the function succeeds, the return value is true.
//          If the function fails, the return value is false.

bool CWinBase::Create( std::string title, HWND HwndParent, short x_pos, short y_pos,
                        short width, short height, HMENU Hmenu, bool bPassWinApplicationpointer )
{
   m_WindowTitle = title;
   //std::string err;
   
   if( !m_bisClassRegistered && !m_iNumberOfWindows && !m_bisControll ) // if it is a controll the class is already registered
   {
      if( !RegisterWndClass()  )
      {
         //err += "Could Not Register Window Class";//GetErrorString( ERROR_CNRWC );
         //err += ". ";
         //GetLastSystemError( err );
         //LogError( ERROR_CNRWC );
      }
      m_bisClassRegistered = true;
   }

   m_Hwnd = ::CreateWindowEx( m_dwExStyle, m_WndClassName.c_str(), title.c_str(), 
                              m_dwStyle, x_pos, y_pos, width, height, 
                              HwndParent, Hmenu, m_Hinstance, (bPassWinApplicationpointer) ? m_WinApplication : NULL );
   if( m_Hwnd )
   {
      if( !m_bisControll )
      {
//         m_iNumberOfWindows++;
      }
      return true;
   }
   
   //err  = "";
   //err += "Could Not Create Window        ";//GetErrorString( ERROR_CNCW );
   //err += ". ";
   //GetLastSystemError( err );
   //LogError( ERROR_CNCW );

   return false;
}

//***************************************************************************
//******************************* CWinBase::Update *************************
// Accepts: void
//
// Purpose: Send a WM_PAINT message to the window specified by CWinBase::Hwnd
//
// Returns: If the function succeeds, the return value is nonzero.
//          If the function fails, the return value is zero. 

bool CWinBase::Update( void )
{
   return (bool) ::UpdateWindow( m_Hwnd );
}

//***************************************************************************
//******************************* CWinBase::Destroy ************************
// Accepts: void
//
// Purpose: Send a WM_DESTROY message to the window specified by CWinBase::Hwnd
//          and set Hwnd = 0
//
// Returns: If the function succeeds, the return value is nonzero.
//          If the function fails, the return value is zero.

bool CWinBase::Destroy( void )
{
   bool bReturnValue = (bool) ::DestroyWindow( m_Hwnd );
   m_Hwnd = 0;
   return bReturnValue;
}

//***************************************************************************
//******************************* CWinBase::RegisterWndClass ***************
// Accepts: void
//
// Purpose: Register the WNDCLASS for this instance of CWinBase
//
// Returns: true if registereg, false otherwise

bool CWinBase::RegisterWndClass( void )
{
   if( m_bisClassRegistered )
      return true;

   if( ::RegisterClassEx( &m_wincl ) )
   {
      return m_bisClassRegistered = true;
   }
   return false;
}

//***************************************************************************
//******************************* CWinBase::Show ***************************
// Accepts: int ( How the window is to be shown )
//
// Purpose: Show the window specified by CWinBase::Hwnd
//
// Returns: If the window was previously visible, the return value is nonzero. 
//          If the window was previously hidden, the return value is zero. 

bool CWinBase::Show( int iCmdShow )
{
  return (bool) ::ShowWindow( m_Hwnd, iCmdShow );
}


bool CWinBase::InvalidateClient( void )
{
   Structs::Rect rect;
   ::GetClientRect( m_Hwnd, &rect );
   return ::InvalidateRect( m_Hwnd, &rect, true );
}

bool CWinBase::InvalidateWindow( void )
{
   Structs::Rect rect;
   ::GetWindowRect( m_Hwnd, &rect );
   return ::InvalidateRect( m_Hwnd, &rect, true );
}
