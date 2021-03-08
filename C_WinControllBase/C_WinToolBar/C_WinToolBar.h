#ifndef __C_WinToolBar_H
#define __C_WinToolBar_H

#include <vector>
#include <string>
#include <list>

#include "../C_WinControllBase.h"
#include "resource.h"

//#define NUM_TOOLBAR_BUTTONS 13

class C_WinToolBar: public C_WinControllBase
{
   public:
       struct TBItems
       {
          TBItems( std::string str ) :
             text( str ),
             id( int() ),
             tooltiptext( "" )
          {}
          TBItems( const TBItems& TBItemsRHS ) :
             text( TBItemsRHS.text ),
             id( TBItemsRHS.id ),
             tooltiptext( TBItemsRHS.tooltiptext )
          {}
          TBItems operator = ( const TBItems& TBItemsRHS )
          {
             text        = TBItemsRHS.text;
             id          = TBItemsRHS.id;
             tooltiptext = TBItemsRHS.tooltiptext;
             return *this;
          }
          std::string tooltiptext;
          std::string text;
          int         id;
       };
       typedef std::list<TBItems> L_TBItems;

       C_WinToolBar( void );
      ~C_WinToolBar( void );

       bool Create( HWND, DWORD , L_TBItems, short = 13, std::string="", short=0, short=0, short = 100, short = 100 );
       bool AddButtons( L_TBItems );
       void GetToolTipText( LPARAM & );

   protected:

       TBBUTTON     m_Tbb;
       L_TBItems    m_ToolBarItems;

   private:

};

#endif
