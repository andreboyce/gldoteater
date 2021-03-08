#pragma once

#ifndef __HDCs_H
#define __HDCs_H

#include "C_HDC.h"
#include "C_PaintDC/C_PaintDC.h"
#include "C_MemoryDC/C_MemoryDC.h"
#include "C_GetDC/C_GetDC.h"

struct HDCs
{
   HDCs(){}
   ~HDCs(){}
   C_PaintDC  m_PaintDC;
   C_MemoryDC m_MemoryDC;
   C_GetDC    m_GetDC;
   void SetHwnd( HWND hwnd )
   {
      m_GetDC.SetHWND( hwnd );
      m_PaintDC.SetHWND( hwnd );
      m_MemoryDC.SetHWND( hwnd );
   }
   void BltMemoryToGet( void )
   {
      ::BitBlt( m_GetDC, 0, 0, m_MemoryDC.BitmapWidth(),
                m_MemoryDC.BitmapHeight(), m_MemoryDC, 0, 0, SRCCOPY );
   }
   void BltMemoryToGet( int width, int height )
   {
      ::BitBlt( m_GetDC, 0, 0, width, height, m_MemoryDC, 0, 0, SRCCOPY );
   }

   void BltMemoryToPaint( void )
   {
      ::BitBlt( m_GetDC, 0, 0, m_MemoryDC.BitmapWidth(),
                m_MemoryDC.BitmapHeight(), m_MemoryDC, 0, 0, SRCCOPY );
   }
};


#endif

