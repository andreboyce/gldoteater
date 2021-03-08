#ifndef __C_WinControllManager_H
#define __C_WinControllManager_H

#define DEBUG

#include "../C_WinControllBase.h"
#include "../WinControlls.h"
#include "../../CWinApplication/CWinApplication.h"
#include "../../Structs/Structs.h"
#include <vector>
#include <string>

class C_WinControllManager
{
   public:

      struct ControllProperties
      {
         public:
            ControllProperties( short x, short y, short width, short height, short id, std::string title, HWND HwndBuddy, CWinApplication::MessageHandler MH, Structs::Rect rect, bool bresizetofittext ) :
               m_x( x ),
               m_y( y ),
               m_width( width ),
               m_height( height ),
               m_id( id ),
               m_title( title ),
               m_HwndBuddy( HwndBuddy ),
               m_MessageHandler( MH ),
               m_rect( rect ),
               m_bresizetofittext( bresizetofittext )
               {   }
            short m_x, m_y, m_id, m_width, m_height;
            bool m_bresizetofittext;
            CWinApplication::MessageHandler m_MessageHandler;
            std::string m_title;
            HWND m_HwndBuddy;
            Structs::Rect m_rect;
            void clear( void )
            {     }
      };

      C_WinControllManager( void );
      ~C_WinControllManager( void );

      bool AddButton( const ControllProperties& );
      C_WinButton& GetButton( short index ){ return *WinButtons.at( index ); }
      int buttons( void ) const { return WinButtons.size(); }

      bool AddEdit( const ControllProperties& cp );
      C_WinEdit& GetEdit( short index ){ return *WinEdits.at( index ); }
      int edits( void ) const { return WinEdits.size(); }

      bool AddStatic( const ControllProperties& cp );
      C_WinStatic& GetStatic( short index ){ return *WinStatics.at( index ); }
      int statics( void ) const { return WinStatics.size(); }

      bool AddUpDown( const ControllProperties& cp );
      C_WinUpDown& GetUpDown( short index ){ return *WinUpDowns.at( index ); }
      int updowns( void ) const { return WinUpDowns.size(); }

      bool AddListBox( const ControllProperties& cp );
      C_WinListBox& GetListBox( short index ){ return *WinListBoxes.at( index ); }
      int listbox( void ) const { return WinListBoxes.size(); }

      bool AddToolTip( const ControllProperties& cp );
      C_WinToolTip& GetToolTip( short index ){ return *WinToolTips.at( index ); }
      int tooltips( void ) const { return WinToolTips.size(); }
      
      bool AddTab( const ControllProperties& cp );
      C_WinTab& GetTab( short index ){ return *WinTabs.at( index ); }
      int tabs( void ) const { return WinTabs.size(); }

      bool AddStatusBar( const ControllProperties& cp );
      C_WinStatusBar& GetStatusBar( short index ){ return *WinStatusBars.at( index ); }
      int statusbars( void ) const { return WinStatusBars.size(); }

      bool AddTreeView( const ControllProperties& cp );
      C_WinTreeView& GetTreeView( short index ){ return *WinTreeViews.at( index ); }
      int treeviews( void ) const { return WinTreeViews.size(); }

      bool AddToolBar( const ControllProperties& cp );
      C_WinToolBar& GetToolBar( short index ){ return *WinToolBars.at( index ); }
      int toolbars( void ) const { return WinToolBars.size(); }

      bool AddProgressBar( const ControllProperties& cp );
      C_WinProgressBar& GetProgressBar( short index ){ return *WinProgressBars.at( index ); }
      int progressbars( void ) const { return WinProgressBars.size(); }

      void SetHwnd( const HWND& hwnd ){ m_Hwnd = hwnd; }

   protected:
      typedef std::vector<C_WinButton*>      V_WinButton;
      typedef std::vector<C_WinEdit*>        V_WinEdit;
      typedef std::vector<C_WinListBox*>     V_WinListBox;
      typedef std::vector<C_WinStatic*>      V_WinStatic;
      typedef std::vector<C_WinStatusBar*>   V_WinStatusBar;
      typedef std::vector<C_WinTab*>         V_WinTab;
      typedef std::vector<C_WinToolBar*>     V_WinToolBar;
      typedef std::vector<C_WinToolTip*>     V_WinToolTip;
      typedef std::vector<C_WinTreeView*>    V_WinTreeView;
      typedef std::vector<C_WinUpDown*>      V_WinUpDown;
      typedef std::vector<C_WinProgressBar*> V_WinProgressBar;
      typedef std::vector<HMENU*>            V_HMenu;
      
      HWND m_Hwnd;
      V_WinButton      WinButtons;
      V_WinEdit        WinEdits;
      V_WinListBox     WinListBoxes;
      V_WinStatic      WinStatics;
      V_WinStatusBar   WinStatusBars;
      V_WinTab         WinTabs;
      V_WinToolBar     WinToolBars;
      V_WinToolTip     WinToolTips;
      V_WinTreeView    WinTreeViews;
      V_WinUpDown      WinUpDowns;
      V_WinProgressBar WinProgressBars;
      V_HMenu          HMenus;
      CWinBase*       m_WinBase;

   private:
};

#endif

