#ifndef __C_WinStatusBar_H
#define __C_WinStatusBar_H

#include "../C_WinControllBase.h"
#include <vector>
#include <string>
#include <cmath>
#include <math.h>

//#define ERROR_TMPTA "Too Many Parts To Add"

class C_WinStatusBar: public C_WinControllBase
{
   public:

      struct S_Part
      {
         S_Part( std::string _text="", short _width = short() )
         {
            text  = _text;
            width = _width;
         }
         S_Part( const S_Part& rhs )
         {
            text  = rhs.text;
            width = rhs.width;
         }
         S_Part& operator = ( const S_Part& rhs )
         {
            text  = rhs.text;
            width = rhs.width;
            return *this;
         }
         std::string text;
         short       width;
      };

      typedef std::vector<S_Part> V_Parts;

      C_WinStatusBar( void );
      ~C_WinStatusBar( void );
      void SetParts( V_Parts &parts );
      bool Create( HWND HwndParent, DWORD id, V_Parts& parts, std::string text = "",
                   short x_pos = 0, short y_pos = 0, short width = 100, short height = 100 );
      void UpdateStatusBar( std::string, WORD, WORD );
      void InitializeStatusBar( void );
      void SetSimpleModeText( std::string );
      void EnableSimpleMode( std::string );
      void EnableSimpleMode( void );
      void DisableSimpleMode( void );

   protected:

      void SetEqualParts( V_Parts );
      void validateparts( V_Parts );

      V_Parts            m_parts;
      bool               m_bisSimpleMode;
      std::string        m_SimpleModeText;

   private:
};

#endif

