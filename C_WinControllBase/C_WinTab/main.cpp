#include <windows.h>
#include "C_WinTab.h"
#include "../C_WinScrollBar/C_WinScrollBar.h"

C_WinTab WinTab;
C_WinScrollBar m_VScroll;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Windows App",       /* Title Text */
           WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    C_WinTab::V_String Strings;
    Strings.push_back( "1" );
    Strings.push_back( "2" );
    Strings.push_back( "3" );
    Strings.push_back( "4" );
    WinTab.Create( hwnd, 200, Strings );
    m_VScroll.Create( hwnd, 0, "", CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,  true );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
      case WM_VSCROLL:
      {
//         Alert("");
      } break;
      case WM_HSCROLL:
      {
//         Alert("");
      } break;
      case WM_SIZE:
      {
         HDWP hdwp; 
         RECT rc; 
 
         // Calculate the display rectangle, assuming the 
         // tab control is the size of the client area. 
         SetRect( &rc, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
         TabCtrl_AdjustRect( WinTab, FALSE, &rc );
 
         // Size the tab control to fit the client area. 
         hdwp = BeginDeferWindowPos( 2 ); 
         DeferWindowPos( hdwp, WinTab, NULL, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), SWP_NOMOVE | SWP_NOZORDER );
 
         // Position and size the scrollbar to fit in
         // tab control's display area, and make sure the 
         // static control is in front of the tab control. 
         int st  = ::GetSystemMetrics( SM_CXVSCROLL );
         DeferWindowPos( hdwp, m_VScroll, HWND_TOP, rc.right-st, 29, st, rc.bottom-st-4, 0 );
         EndDeferWindowPos( hdwp );
      } break;
      case WM_NOTIFY:
      {
         NMHDR *lpnmhdr = (NMHDR*)(lParam);
         switch( lpnmhdr->code )
         {
            case TCN_SELCHANGE:
            {
               int iPage = TabCtrl_GetCurSel( WinTab );
               Alert( iPage );
            } break;
         }
      } break;
      case WM_RBUTTONDOWN:
      {
         WinTab.AddTab( "", 0 );
      } break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
