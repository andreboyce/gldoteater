#ifndef __C_Line_H
#define __C_Line_H

#include "../../C_HDC/C_MemoryDC/C_MemoryDC.h"
#include "../../Structs/Structs.h"

class C_Line
{
   friend class MainWindow;
   public:
      C_Line( void ) : 
         m_LineLength( short() ), 
         m_halfLineLength( short() ), 
         m_memDC( NULL ) {};
      C_Line( LONG LineLength ) : 
         m_LineLength( LineLength ), 
         m_halfLineLength( (short)( m_LineLength*0.5) ), 
         m_memDC( NULL ){};
      ~C_Line( void ){};
      
      void Draw( void );

      inline operator HDC( void ){  return *m_memDC;  }
      inline void SetLength( LONG LineLength ){ m_LineLength = LineLength; m_halfLineLength = (short)(m_LineLength*0.5); }

      C_MemoryDC      *m_memDC;
      Structs::Point m_point;
      Structs::Point m_rect;
      short            m_LineLength;
      short            m_halfLineLength;

   protected:


   private:
};

#endif

