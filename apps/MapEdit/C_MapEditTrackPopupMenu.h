#ifndef __C_MapEditTrackPopupMenu_H
#define __C_MapEditTrackPopupMenu_H

#include <windows.h>

class C_MapEditTrackPopupMenu
{
   public:
      C_MapEditTrackPopupMenu() : m_Hmenu( NULL ) {}
      ~C_MapEditTrackPopupMenu(){}

      void Display( HWND hwnd, int x, int y )
      {
         POINT pt = {x,y};
         ::ClientToScreen( hwnd, &pt );
         //::SetMenu( hwnd, m_Hmenu );
         ::TrackPopupMenuEx( m_Hmenu, 0, pt.x, pt.y, hwnd, NULL );
      }
      HMENU m_Hmenu;
};

#endif
