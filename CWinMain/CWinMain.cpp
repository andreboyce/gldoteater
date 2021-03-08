#include "CWinMain.h"

CWinMain::CWinMain( void ) :
   CWinApplication(),
   m_Haccel( NULL )
{
}

CWinMain::~CWinMain( void )
{
}

bool CWinMain::Create( char title[], HWND HwndParent, short x_pos, short y_pos, short width, 
                        short height, HMENU Hmenu )
{
   return m_WinBase.Create( title, HwndParent, x_pos, y_pos, width, height, 
                            Hmenu, true );
}

int CWinMain::RunMessageLoop( void )
{
   MSG  messages;
   int  iReturn;

   if( ::IsWindow( m_WinBase ) )
   {
      // Run the message loop. It will run until GetMessage() returns 0 
      while ( iReturn = ::GetMessage( &messages, NULL, 0, 0 ) )
      {
         if( iReturn != -1 )
         {
            if ( !::TranslateAccelerator( m_WinBase, m_Haccel, &messages ) )
            {
               ::TranslateMessage(&messages);
               ::DispatchMessage(&messages);
            }
         }
         else
         {
            return -1;
         }
      }
      return messages.wParam;
   }
   return -1;
}

