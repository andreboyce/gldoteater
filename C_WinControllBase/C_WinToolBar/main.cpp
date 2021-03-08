#include <windows.h>

#include "C_WinToolBar.h"

C_WinToolBar ToolBar;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure( HWND, UINT, WPARAM, LPARAM );

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

/*
LRESULT MsgNotify( HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam )
{
    static UINT		i=0;
    LPNMHDR         lpnmhdr;

    lpnmhdr = (LPNMHDR)lparam;

    // The following code allows the toolbar to be customized. 
    // If you return FALSE the Customize Toolbar dialog flashes
    // and goes away.

    if (lpnmhdr->code == TBN_QUERYINSERT || lpnmhdr->code ==
	    TBN_QUERYDELETE)
    {
     return TRUE;
    }
        
    if (lpnmhdr->code == TBN_GETBUTTONINFO)
    {
        LPTBNOTIFY lpTbNotify = (LPTBNOTIFY)lparam;
        char  szBuffer [20];
	   
		// 20 = total number of buttons.
		// tbButton and tbButtonNew send information about
		// the other 12 buttons in tbButtonNew.
		
        if (lpTbNotify->iItem < 12)    
        {                              
         lpTbNotify->tbButton = tbButtonNew[lpTbNotify->iItem];
		 LoadString(hInst,
                    4000+ lpTbNotify->iItem,  
                    szBuffer, sizeof(szBuffer)/sizeof(szBuffer[0]));
         hr = StringCchCopy
                  (lpTbNotify->pszText, 
                          sizeof(lpTbNotify->pszText)
                          /sizeof(lpTbNotify->pszText[0]),
                          szBuffer );
                        if(SUCEEDED(hr))
                          {
                          lpTbNotify->cchText =     
                               sizeof(szBuffer)/sizeof(szBuffer[0]);
                          }
                        else
                          {
                          TODO: Write error handler.
                          }
         return TRUE;
        }
     else
        return 0;
    }
    return 0;
}
*/
HINSTANCE  g_hinst;
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
g_hinst = hThisInstance;
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
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);
    C_WinToolBar::L_TBItems TBBStrings;
    TBBStrings.push_back( C_WinToolBar::TBItems("one")      );
    TBBStrings.push_back( C_WinToolBar::TBItems("two")      );
    TBBStrings.push_back( C_WinToolBar::TBItems("three")    );
    TBBStrings.push_back( C_WinToolBar::TBItems("four")     );
    TBBStrings.push_back( C_WinToolBar::TBItems("five")     );
    TBBStrings.push_back( C_WinToolBar::TBItems("six")      );
    TBBStrings.push_back( C_WinToolBar::TBItems("seven")    );
    TBBStrings.push_back( C_WinToolBar::TBItems("eight")    );
    TBBStrings.push_back( C_WinToolBar::TBItems("nine")     );
    TBBStrings.push_back( C_WinToolBar::TBItems("ten")      );
    TBBStrings.push_back( C_WinToolBar::TBItems("eleven")   );
    TBBStrings.push_back( C_WinToolBar::TBItems("twelve")   );
    TBBStrings.push_back( C_WinToolBar::TBItems("thirteen") );
    ToolBar.Create( hwnd, 1, TBBStrings );

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
       case WM_LBUTTONDOWN:
       {
          ::SendMessage( ToolBar, TB_CUSTOMIZE, 0, 0 );
       } break;
       case WM_SIZE:
       {
          ToolBar.Resize( LOWORD(lParam), HIWORD(lParam)*15/100);
       } break;
       
    case WM_NOTIFY: 
      switch (((LPNMHDR) lParam)->code) 
      { 
        case TTN_GETDISPINFO:
        {
           ToolBar.GetToolTipText( lParam );
        } break;
        default:   return DefWindowProc (hwnd, message, wParam, lParam);
    } break; 

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
