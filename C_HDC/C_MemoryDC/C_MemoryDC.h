#pragma once

#ifndef __C_MemoryDC_H
#define __C_MemoryDC_H

#define WIN32_LEAN_AND_MEAN
#include "../C_HDC.h"

class C_MemoryDC: public C_HDC
{
   public:
       C_MemoryDC( void );
       C_MemoryDC( HWND );
      ~C_MemoryDC( void );

      void SetHWND( HWND Hwnd )
      {
         Structs::Rect rect;
         ::GetClientRect( Hwnd, &rect );
         CreateBitmap( Hwnd, rect );
      }

      void Draw( void );
      void Destroy( void );

      bool CreateBitmap( HWND Hwnd, Structs::Rect dimensions );
      bool CreateBitmap( HWND Hwnd, int width, int height );

   protected:
      HDC m_HdcGet;

   private:   
};

#endif

