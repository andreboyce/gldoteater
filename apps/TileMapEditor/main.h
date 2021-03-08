#ifndef __Main_H
#define __Main_H

#include "../../projects.h"

#include <algorithm>
#include <math.h>
#include <string>
#include <queue>
#include <list>
#include <fstream>
#include <stack>
#include <sstream>
#include <windef.h>
//#include <unistd.h>
#include <boost/tokenizer.hpp>
#include <exception>
#include <typeinfo>

#include "resource.h"
//#include "sdts.h"


class MainWindow: public CWinMain
{
   friend int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                              LPSTR lpszArgument,      int nFunsterStil );
   public:
      MainWindow( void );
      virtual ~MainWindow( void );
      enum 
      {
         DRAW_TIMER = 1,
         ADD_SPRITE_DRAW_TIMER,
      };

      static LRESULT MWBaseOnCommand(       MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnRButtonU(      MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLButtonU(      MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnRDoubleClick(  MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLDoubleClick(  MainWindow &mw, WPARAM wparam, LPARAM lparam );      
      static LRESULT MWBaseOnRButtonD(      MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLButtonD(      MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnMouseMove(     MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnCreate(        MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnSize(          MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnPaint(         MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnNotify(        MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnKeyDown(       MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnKeyUp(         MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnHScroll(       MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnVScroll(       MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnTimer(         MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnKillFocus(     MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnSetFocus(      MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnClose(         MainWindow &mw, WPARAM wparam, LPARAM lparam );

      static BOOL CALLBACK NewMapDlgProc(         HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
      static BOOL CALLBACK ResizeMapDlgProc(      HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
      static BOOL CALLBACK AboutDlgProc(          HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
      static BOOL CALLBACK TilePropertiesDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
      static BOOL CALLBACK GetTilesFromBitmapDlg( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
      static BOOL CALLBACK SolidityDlgProc(       HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
      static BOOL CALLBACK AStarStatsDlgProc(     HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
      static BOOL CALLBACK AddSpriteDlgProc(      HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
      static BOOL CALLBACK DeleteSpriteDlgProc(   HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
      static BOOL CALLBACK SpriteStatsDlgProc(   HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );

      //static int luaAlert( lua_State *LuaState );

      void RipTiles( Structs::Point tile_dimensions = Structs::Point(32,32) );
      bool UnPackFiles( std::string path_source_pak, std::string path_destination_dir );
      bool PackFiles( std::string path_source_dir, std::string path_destination_pak );
      bool AddSpriteDlgMakeNewTab( void );
      bool ReNameMap( void );
      //void RegisterLuaFunctions( void );
      //void RegisterFunctionsCalledByLua( void );
      void HandleCommandLine( LPSTR lpszArgument );
      bool LoadMapAndMakeTab( std::string filename );
      void DrawMouseRightClickRect( C_HDC & hdc );
      bool ValidTileIndex( Structs::Point p );
      void HilightMouseOverTile( Structs::Point tile );
      Structs::Point GetMapTile( Structs::Point MouseDownPosition );
      Structs::Point GetMapTile( void );
      Structs::Point GetMapTileSetTile( void );
      int  GetMapTileSetTileID( Structs::Point tile );
      void HilightTile( void );
      void ResizeTileSetsIDs( void );
      void MakeNewTab( void );
      bool RemoveMap( int index );
      void DrawSprites( int index, C_HDC& hdc );
      void DrawAStarNodes( int index, C_HDC& hdc );
      void DrawAStarNodesSearched( int index, C_HDC& hdc );
      void DrawTileOnSolidityDialog( C_HDC& hdc );
      void DrawTileOnPropertiesDialog( C_HDC& hdc );
      void DrawOnAddSpriteDialog( C_HDC& hdc );
      void DrawOnDeleteSpriteDialog( C_HDC& hdc );
      void DrawOnSpriteStatsDialog( C_HDC& hdc );
      void ResizeMap( void );
      void ResetScrollBars( void );
      void ResizeWindows( int width, int height );
      bool ShowSolidityDlg( void );
      bool ShowNewMapDlg( void );
      bool ShowResizeMapDlg( void );
      bool ShowAStarStatsDlg( void );
      bool ShowAboutDlg( void );
      bool ShowTilePropertiesDlg( void );
      bool ShowGetTilesFromBitmapDlg( void );
      bool ShowAddSpriteDlg( void );
      bool ShowDeleteSpriteDlg( void );
      bool ShowSpriteStatsDlg( void );
      bool LoadConfig( void );
      bool SaveConfig( void );
      bool MakeTileSet( void );
      bool SaveTileSet( void );
      void SaveTileMap( void );
      void OpenTileMap( void );
      void CloseTileMap( int index );
      void OpenTile( void );
      void Initialise( void );
      void MakeTileMap( int width, int height );
      bool SaveTileSet( std::string filename );
      bool SaveMap( std::string filename );
      bool LoadMap( std::string filename );
      bool LoadTileSet( std::string filename );
      bool AddBitmap( std::string filename );
      void RemoveBitmap( int index );
      bool DeleteBitmaps( void );
      bool DeleteTiles( void );
      void DrawCurrentTile( int index, C_HDC& hdc );
      void DrawMap( int index, C_HDC& hdc );
      void DrawAStarPath( int index, C_HDC& hdc );
      void DrawTileSet( int index, C_HDC& hdc );
      void DrawSelectedTilesRect( C_HDC& hdc );
      void DrawBitmapInGetTilesFromBitmapDlg( C_HDC & hdc );
      void DrawGridInGetTilesFromBitmapDlg( C_HDC & hdc );
      void ValidateTiles( void );
      bool IsSelected( int row, int col );
      void PasteSelectedTiles( void );
      void GetSelectedTiles( void );
      void DeleteSelectedTiles( void );
      void DeleteOrignalSelectedTiles( void );
      void CopySelectedTiles( void );
      void OpenHelp( void );

      bool CheckCollide( const Structs::Rect& Rect, const Structs::Point& Point );

   protected:
//      SpecialDataTypesDialog *m_SpecialDataTypesDialog;
      HWND             m_HwndNewMapDlg,
                       m_HwndResizeMapDlg,
                       m_HwndAboutDlg,
                       m_HwndTilePropertiesDlg,
                       m_HwndGetTilesFromBitmapDlg,
                       m_HwndSolidityDlg,
                       m_HwndAStarSearchStatsDlg,
                       m_HwndAddSpriteDlg,
                       m_HwndDeleteSpriteDlg,
                       m_HwndSpriteStatsDlg;
      C_WinStatusBar   m_StatusBar;
      C_WinTab         m_MapsTab, 
                       m_TileSetsTab;
      C_WinStatic      m_MapsTabStatic,
                       m_TileSetsTabStatic,
                       m_TileSetsTabStatic2;
      C_WinScrollBar   m_MapsVScroll,
                       m_MapsHScroll,
                       m_TileSetsVScroll,
                       m_TileSetsHScroll;
      CXMLMenu        m_XMLMenu;
      CTrackPopupMenu m_TPopupMenu;
      Structs::Point m_PreviousMouseOverTile,
                       m_PreviousMouseOverTabTile,
                       m_TileOnLButtonDown,
                       m_RButtonDownStartPoint,
                       m_RButtonUpStartPoint,
                       m_MousePosition,
                       m_ClickedTile,
                       m_TilePropertiesTile,
                       m_TileAddSpriteTile;
      CWinFileHandler m_WinFileHandler;
      C_WinToolTip     m_EnLargedTileToolTip;
      Timer            m_DrawTimer,
                       m_AddSpriteDrawTimer;

      std::deque<CHBITMAP>                  m_Sprites;
	  std::deque< TileMapLib::C_TileMap >   m_TileMaps;

      typedef std::deque< std::deque< int > > dint2d;
      std::deque< dint2d > m_TileSetsIDs;

      std::deque<int> m_CurrentTileGraphics;
      std::deque<int> m_Solidity;

      std::string      m_ConfgFileName,
                       m_PackFileName,
                       m_AppOrignalWorkingDirectory;

      int              m_yoffset,
                       m_xoffset,
                       m_XMLMenuFirstCommand,
                       m_CurrentMap,
                       m_TilePropertiesGraphic,
                       m_TilePropertiesSolidity,
                       m_TileSolidityGraphic,
                       m_TileSoliditySolidity,
                       m_MapVisableX,
                       m_MapVisableY,
                       m_TileSetTabStaticVisibleX,
                       m_TileSetTabStaticVisibleY,
                       m_TilePropertiesGraphicOrignal,
                       m_TilePropertiesSolidityOrignal,
                       m_AddSpriteZoom;

      bool             m_bLMouseDown,
                       m_bRMouseDown,
                       m_bUnSaved,
                       m_bTileSelected,
                       m_bIncrementTileGraphic,
                       m_bHasKeyboardFocus,
                       m_bLoopSpriteAnimation;

      HDCs             m_MapsStaticDCs,
                       m_TileSetsStaticDCs,
                       m_TileSetsStaticDCs2,
                       m_PropertiesDialogStaticDCs,
                       m_GetTilesFromBitmapDlgDCs,
                       m_SolidityDialogStaticDCs,
                       m_AddSpriteDialogStaticDCs,
                       m_DeleteSpriteDialogStaticDCs,
                       m_SpriteStatsDlgDCs;

      CHBITMAP        m_GetTilesBitmap,
                       m_AstarStart,
                       m_AstarEnd,
                       m_HBUp,
                       m_HBDown,
                       m_HBLeft,
                       m_HBRight,
                       m_HBDownRight,
                       m_HBUpRight,
                       m_HBDownLeft,
                       m_HBUpLeft;

      struct Scroll
      {
         Scroll( void ) :
            xMinScroll( 0 ),
            xCurrentScroll( 0 ),
            xMaxScroll( 0 ),
            yMinScroll( 0 ),
            yCurrentScroll( 0 ),
            yMaxScroll( 0 )
         {
         }
         ~Scroll(){}
         // These variables are required for horizontial scrolling.  
         int xMinScroll;       // minimum horizontal scroll value 
         int xCurrentScroll;   // current horizontal scroll value 
         int xMaxScroll;       // maximum horizontal scroll value  
         // These variables are required for vertical scrolling.  
         int yMinScroll;       // minimum vertical scroll value 
         int yCurrentScroll;   // current vertical scroll value 
         int yMaxScroll;       // maximum vertical scroll value 
      };
      std::deque< Scroll > MapsScrolls,
                           TileSetsScrolls;

      //CLuaState       m_LuaState;
      //CLuaToCFunction luaDrawSprites;

   private:
};

#endif

