#include "CWinMain.h"

class MainWindow: public CWinMain
{
   friend int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                             LPSTR lpszArgument,      int nFunsterStil );
   public:
      MainWindow( void );
      virtual ~MainWindow( void );

      static LRESULT MWBaseOnCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnRButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnCreate( MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnSize( MainWindow &mw, WPARAM wparam, LPARAM lparam );

   protected:
   private:
};

