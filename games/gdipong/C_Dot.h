#ifndef __C_Dot_H
#define __C_Dot_H

#include "../../Structs/Structs.h"
#include "../../C_HDC/C_MemoryDC/C_MemoryDC.h"

class C_Dot
{
   friend class MainWindow;
   public:
      C_Dot( void ) : m_DotWidth( short() ), m_memDC( NULL ){}
      C_Dot( short DotWidth ) : m_DotWidth( DotWidth ), m_memDC(NULL) {}
      ~C_Dot( void ){};
      void Draw( void );
      operator HDC( void ){  return *m_memDC;  }

      C_MemoryDC      *m_memDC;
      short            m_DotWidth;
      Structs::Point m_point;

   protected:

   private:
};

#endif
