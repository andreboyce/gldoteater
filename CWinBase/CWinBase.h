#ifndef __CWinBase_H
#define __CWinBase_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <string>
#include <map>
#include <cstring>

#include "../errorutil/errorutil.h"
#include "../Structs/Structs.h"

#ifdef GetHwnd
#undef GetHwnd
#endif

#define DEFAULT_WNDCLASS_NAME "WindowsApp"

class CWinApplication;

class CWinBase
{

   friend class CWinApplication;
   friend class OpenGLLib;
   friend class C_WinMDIBase;
   friend class C_WinControllBase;

   public:

      virtual ~CWinBase( void );

      // inline functions

      DWORD inline SetStyle( DWORD Style = WS_OVERLAPPEDWINDOW )
                 { return m_dwStyle = Style; }

      DWORD inline GetStyle( void )
                 { return m_dwStyle; }

      DWORD inline SetExStyle( DWORD ExStyle = 0 )
                 { return m_dwExStyle = ExStyle; }

      DWORD inline GetExStyle( void )
                 { return m_dwExStyle; }

      UINT  inline SetWndClassStyle( UINT style = CS_DBLCLKS )
                 { return m_wincl.style = style; }

      HICON inline SetLargeIcon( HICON hLargeIcon = LoadIcon(NULL, IDI_APPLICATION) )
                 { return m_wincl.hIcon   = hLargeIcon; }

      HICON inline SetSmallIcon( HICON hSmallIcon = LoadIcon(NULL, IDI_APPLICATION) )
                 { return m_wincl.hIconSm = hSmallIcon; }

      int inline SetcbClsExtra( int cbClsExtra = 0 )
                 { return m_wincl.cbClsExtra = cbClsExtra; }

      int   inline SetcbWndExtra( int cbWndExtra = 0 )
                 { return m_wincl.cbWndExtra = cbWndExtra; }

      LPCTSTR inline SetMenu( LPCTSTR MenuName = NULL )
                 {
                    return m_wincl.lpszMenuName = MenuName;
                 }

      HCURSOR inline SetCursor( HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW) )
                 { return m_wincl.hCursor = hCursor; }

      LPCTSTR inline SetClassName( LPCTSTR szNewClassName = DEFAULT_WNDCLASS_NAME )
                 {
                    if( !m_bisClassRegistered )
                    {
                       m_WndClassName = szNewClassName;
                       m_wincl.lpszClassName = szNewClassName; // ?!
                    }
                    return m_wincl.lpszClassName;
                 }

      HWND    inline GetHwnd(void)// const 
                 { return m_Hwnd; }

      bool inline SetFocus( void )// const
         {
            return (bool)::SetFocus( m_Hwnd );
         }
         
      virtual bool SetText( std::string new_text )
      {
         m_WindowTitle = new_text;
         return (bool)::SetWindowText( m_Hwnd, (LPCTSTR) new_text.c_str() );
      }

      std::string GetTitle( void ) const
      {
         return m_WindowTitle;
      }

      void GetClientRect( Structs::Rect & rect )
      {
         ::GetClientRect( m_Hwnd, &rect );
      }

      Structs::Rect GetClientRect( void )
      {
         Structs::Rect rect;
        ::GetClientRect( m_Hwnd, &rect );
         return rect;
      }

      void GetWindowRect( Structs::Rect & rect )
      {
         ::GetWindowRect( m_Hwnd, &rect );
      }

      Structs::Rect GetWindowRect( void )
      {
         Structs::Rect rect;
         ::GetWindowRect( m_Hwnd, &rect );
         return rect;
      }

      WINDOWPLACEMENT GetWindowPlacement( void )
      {
         WINDOWPLACEMENT wp = {0};
         wp.length = sizeof(WINDOWPLACEMENT);
         if( !::GetWindowPlacement( m_Hwnd, &wp ) )
         {
         }
         return wp;
      }

      Structs::Point GetWindowPos( void )
      {
         Structs::Point p( -1, -1 );
         WINDOWPLACEMENT  wp   = GetWindowPlacement();
         Structs::Rect  rect = wp.rcNormalPosition;
         switch( wp.showCmd )
         {
            case SW_HIDE:
            case SW_MINIMIZE:
            {               
            } return p;

            case SW_RESTORE:
            case SW_SHOW:
            case SW_SHOWMAXIMIZED:
            case SW_SHOWMINIMIZED:
            case SW_SHOWMINNOACTIVE:
            case SW_SHOWNA:
            case SW_SHOWNOACTIVATE:
            case SW_SHOWNORMAL:
            {
            } break;
         }
         p.x = rect.left;
         p.y = rect.top;
         return p;
      }

      int inline width( void ) const
      {
         Structs::Rect rect;
         ::GetWindowRect( m_Hwnd, &rect );
         return rect.top;
      }

      int inline height( void ) const
      {
         Structs::Rect rect;
         ::GetWindowRect( m_Hwnd, &rect );
         return rect.bottom;
      }

      void inline SetNumberOfWindows( int NumberOfWindows ) const
      {
         m_iNumberOfWindows = NumberOfWindows;
      }

      int inline GetNumberOfWindows( int NumberOfWindows ) const
      {
         return m_iNumberOfWindows;
      }



/*      bool BlitToClientAreaP( HDC hdcMem )
      {
         Structs::Rect rect;
         GetClientRect( rect );
         static C_PaintDC  m_PDC;
         m_PDC.SetHWND( m_Hwnd );
         m_PDC.CreateCompatibleBitmap( rect );
         return ::BitBlt( m_PDC, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, hdcMem, rect.left, rect.top, SRCCOPY );
      }

      bool BlitToClientAreaM( HDC hdcMem )
      {
         Structs::Rect rect;
         GetClientRect( rect );
         C_GetDC m_GDC;
         m_GDC.SetHWND( m_Hwnd );
         m_GDC.CreateCompatibleBitmap( rect );
         return ::BitBlt( m_GDC, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, hdcMem, rect.left, rect.top, SRCCOPY );
      }
*/

      // virtual functions

      bool    virtual Create( std::string, HWND HwndParent = (HWND)HWND_DESKTOP, short x_pos = (short)CW_USEDEFAULT, short y_pos = (short)CW_USEDEFAULT,
                              short width = 640, short height = 480, HMENU Hmenu = (HMENU)NULL, bool = 1 );
      bool    virtual Destroy( void );

      // standard functions
      void           Initialise( std::string ClassName = DEFAULT_WNDCLASS_NAME, DWORD dwStyle = 0, DWORD dwExStyle = 0, DWORD dwWndClassStyle = CS_DBLCLKS, CWinApplication* = NULL );
      bool           RegisterWndClass( void );
      bool           Show( int nCmdShow = SW_NORMAL );
      bool           Update( void );
      bool           InvalidateClient( void );
      bool           InvalidateWindow( void );

      // static functions
      static LRESULT CALLBACK MessageRouterProc( HWND, UINT, WPARAM, LPARAM );

      LRESULT virtual DefaultWindowProcedure( HWND Hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
      {
         return ::DefWindowProc( Hwnd, msg, wParam, lParam );
      }

      // operators

      operator bool (void) const
      {
         return (bool) ::IsWindow( m_Hwnd );
      }

      operator HWND (void) const
      {
         return m_Hwnd;
      }

      CWinBase& operator = ( const CWinBase& RHS ) 
      {
//         m_Hwnd            = RHS.m_Hwnd;
         m_LastErrorString = RHS.m_LastErrorString;
         m_WndClassName    = RHS.m_WndClassName;
         m_WindowTitle     = RHS.m_WindowTitle;
         m_dwExStyle       = RHS.m_dwExStyle;
         m_dwStyle         = RHS.m_dwStyle;
         m_bisVisible      = RHS.m_bisVisible;
         m_bisControll     = RHS.m_bisControll;
         m_Hinstance       = RHS.m_Hinstance;
         m_WinApplication  = RHS.m_WinApplication;
         std::memcpy( &m_wincl, &RHS.m_wincl, sizeof(m_wincl) );
         m_wincl.lpszMenuName = RHS.m_wincl.lpszMenuName;
//         std::strcpy( (char*) m_wincl.lpszMenuName,  RHS.m_wincl.lpszMenuName  );
//         std::strcpy( (char*) m_wincl.lpszClassName, RHS.m_wincl.lpszClassName );
         m_wincl.lpszClassName = RHS.m_wincl.lpszClassName;
//         Initialise( m_WndClassName, m_dwStyle, m_dwExStyle, m_wincl.style, m_WinApplication );
         return *this;
      }

      //errorutil          m_ErrorHandlers;

   protected:

      CWinBase(  DWORD = CS_DBLCLKS, char [] = DEFAULT_WNDCLASS_NAME, DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwExStyle = 0, CWinApplication* = NULL);
      CWinBase(  char [], DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwExStyle = 0, CWinApplication* = NULL);

      CWinBase( const CWinBase& RHS ) :
         m_LastErrorString( RHS.m_LastErrorString ),
         m_WndClassName( RHS.m_WndClassName ),
         m_WindowTitle( RHS.m_WindowTitle ),
         m_dwExStyle( RHS.m_dwExStyle ),
         m_dwStyle( RHS.m_dwStyle ),
         m_bisVisible( RHS.m_bisVisible ),
         m_bisControll( RHS.m_bisControll ),
         m_Hinstance( RHS.m_Hinstance ),
         m_WinApplication( RHS.m_WinApplication )
      {
         std::memcpy( &m_wincl, &RHS.m_wincl, sizeof(m_wincl) );
         m_wincl.lpszMenuName = RHS.m_wincl.lpszMenuName;
//         std::strcpy( (char*) m_wincl.lpszMenuName,  RHS.m_wincl.lpszMenuName  );
         m_wincl.lpszClassName = RHS.m_wincl.lpszClassName;
//         std::strcpy( (char*) m_wincl.lpszClassName, RHS.m_wincl.lpszClassName );
      }

      void SetApplication( CWinApplication* WinApplication )
         {
            m_WinApplication = WinApplication;
         }

      WNDCLASSEX        m_wincl;
      HWND              m_Hwnd;
      std::string       m_LastErrorString;
      std::string       m_WndClassName;
      std::string       m_WindowTitle;
      DWORD             m_dwExStyle;
      DWORD             m_dwStyle;
      bool              m_bisVisible;
      bool              m_bisControll;
      static bool       m_bisClassRegistered;
      static int        m_iNumberOfWindows;
      HINSTANCE         m_Hinstance;
      CWinApplication  *m_WinApplication;
};

#endif

