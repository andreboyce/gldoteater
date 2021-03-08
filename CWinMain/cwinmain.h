#ifndef __CWinMain_H
#define __CWinMain_H

//#define WIN32_LEAN_AND_MEAN
#include "../CWinApplication/CWinApplication.h"

class CWinMain: public CWinApplication
{
   public:

      CWinMain( void );
      virtual ~CWinMain( void );

      bool Create( char title[], HWND HwndParent = HWND_DESKTOP, short x_pos = CW_USEDEFAULT, short y_pos = CW_USEDEFAULT,
                   short width = 640, short height = 480, HMENU Hmenu = NULL );

      int RunMessageLoop( void );

   protected:
      HACCEL m_Haccel;
   private:
};

#endif

