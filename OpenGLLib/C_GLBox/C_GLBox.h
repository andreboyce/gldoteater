#pragma once

#ifndef __C_GLBox_H
#define __C_GLBox_H

#include "../C_GLLine/C_GLLine.h"

class C_GLBox
{
   public:
      C_GLBox()
      {
         m_Lines[0].SetX( -0.5, 0.5 );
         m_Lines[0].SetY( 0.5, 0.5 ); // start----------end
         m_Lines[0].SetZ( 0, 0 );

         m_Lines[1].SetX( -0.5, 0.5 );
         m_Lines[1].SetY( 0.5, 0.5 ); // start----------end
         m_Lines[1].SetZ( 0, 0 );      

         m_Lines[2].SetX( 0.5, 0.5 ); // start
         m_Lines[2].SetY( +0.5, -0.5 ); // 
         m_Lines[2].SetZ( 0, 0 );     // end

         m_Lines[3].SetX( 0.5, 0.5 ); // start
         m_Lines[3].SetY( +0.5, -0.5 ); // 
         m_Lines[3].SetZ( 0, 0 );     // end
      }

      ~C_GLBox(){}

      void Draw( void )
      {
      }

   protected:
      C_GLLine m_Lines[4];

   private:
};

#endif
