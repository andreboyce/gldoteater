#ifndef __CWinApplication_H
#define __CWinApplication_H

#include <windows.h>
#include <map>
#include <string>

#include "../errorutil/errorutil.h"
#include "../CWinBase/CWinBase.h"

#define ERROR_CNRWC_s "Could Not Register Window Class"
#define ERROR_CNCW_s  "Could Not Create Window        "
#define ERROR_CNSW_s  "Could Not Show Window          "
#define ERROR_CNDW_s  "Could Not Destroy Window       "

//class CWinApplication;
//typedef LRESULT (*MessageHandler)( CWinApplication &, WPARAM, LPARAM );

class CWinApplication
{
   public:
      CWinApplication( DWORD = CS_DBLCLKS, char [] = DEFAULT_WNDCLASS_NAME, DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwExStyle = 0 );
      virtual ~CWinApplication( void );

      // typedefs
	  typedef LRESULT (*MessageHandler)( CWinApplication &, WPARAM, LPARAM );
      typedef std::map< UINT, MessageHandler > MessageHandlerMap;
      typedef MessageHandlerMap::iterator      MessageHandlerMapIterator;

      static LRESULT CALLBACK MessageRouterProc( HWND, UINT, WPARAM, LPARAM );
      static LRESULT WinBaseOnDestroy( CWinApplication &, WPARAM, LPARAM );
      static LRESULT WinBaseOnClose(   CWinApplication &, WPARAM, LPARAM );

      bool   AddMessageHandler( UINT, MessageHandler );      
      MessageHandler GetMessageHandler( UINT );
      void   FillWndClass( void );
      CWinBase m_WinBase;
      static HWND m_LastHwndPassed;

      operator HWND ( void ) const
      {
         return m_WinBase;
      }

      operator HINSTANCE ( void )
      {
         return m_Hinstance = ::GetModuleHandle( NULL );
      }

   protected:

      MessageHandlerMap m_MsgHandlerMap;
      WNDCLASSEX        m_wincl;
      HINSTANCE         m_Hinstance;

   private:

};

#endif

