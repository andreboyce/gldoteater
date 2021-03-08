#include "main.h"

#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 

MainWindow::MainWindow( void ) :
m_yoffset( 0 ),
m_xoffset( 0 ),
m_XMLMenuFirstCommand( 1 ),
m_CurrentMap( 0 ),
m_PackFileName( "data.pak" ),
m_ConfgFileName( "config.xml" ),
m_TileSolidityGraphic( 0 ),
m_TileSoliditySolidity( 0 ),
m_TilePropertiesGraphic( 0 ),
m_TilePropertiesGraphicOrignal( 0 ),
m_TilePropertiesSolidityOrignal( 0 ),
m_TilePropertiesSolidity( 0 ),
m_MapVisableX( 0 ),
m_MapVisableY( 0 ),
m_bLMouseDown( false ),
m_bRMouseDown( false ),
m_bUnSaved( true ),
m_HwndSpriteStatsDlg( NULL ),
m_HwndDeleteSpriteDlg( NULL ),
m_HwndAddSpriteDlg( NULL ),
m_HwndNewMapDlg( NULL ),
m_HwndResizeMapDlg( NULL ),
m_HwndAboutDlg( NULL ),
m_HwndTilePropertiesDlg( NULL ),
m_HwndGetTilesFromBitmapDlg( NULL ),
m_HwndSolidityDlg( NULL ),
m_HwndAStarSearchStatsDlg( NULL ),
m_TileSetTabStaticVisibleX( 0 ),
m_TileSetTabStaticVisibleY( 0 ),
m_bHasKeyboardFocus( false ),
m_bLoopSpriteAnimation( true ),
m_AddSpriteZoom( 1 )
//m_SpecialDataTypesDialog( NULL )
{
   //RegisterFunctionsCalledByLua();
   //RegisterLuaFunctions();
   //m_LuaState.opendefaultlibs();
   //m_LuaState.LoadScript( "tilemapeditor.lua" );
   AddMessageHandler( WM_SIZE,          reinterpret_cast<MessageHandler>(MWBaseOnSize)         );
   AddMessageHandler( WM_CREATE,        reinterpret_cast<MessageHandler>(MWBaseOnCreate)       );
   AddMessageHandler( WM_LBUTTONDOWN,   reinterpret_cast<MessageHandler>(MWBaseOnLButtonD)     );
   AddMessageHandler( WM_RBUTTONDOWN,   reinterpret_cast<MessageHandler>(MWBaseOnRButtonD)     );
   AddMessageHandler( WM_LBUTTONUP,     reinterpret_cast<MessageHandler>(MWBaseOnLButtonU)     );
   AddMessageHandler( WM_RBUTTONUP,     reinterpret_cast<MessageHandler>(MWBaseOnRButtonU)     );
   AddMessageHandler( WM_COMMAND,       reinterpret_cast<MessageHandler>(MWBaseOnCommand)      );
   AddMessageHandler( WM_PAINT,         reinterpret_cast<MessageHandler>(MWBaseOnPaint)        );
   AddMessageHandler( WM_MOUSEMOVE,     reinterpret_cast<MessageHandler>(MWBaseOnMouseMove)    );
   AddMessageHandler( WM_HSCROLL,       reinterpret_cast<MessageHandler>(MWBaseOnHScroll)      );
   AddMessageHandler( WM_VSCROLL,       reinterpret_cast<MessageHandler>(MWBaseOnVScroll)      );
   AddMessageHandler( WM_KEYUP,         reinterpret_cast<MessageHandler>(MWBaseOnKeyUp)        );
   AddMessageHandler( WM_KEYDOWN,       reinterpret_cast<MessageHandler>(MWBaseOnKeyDown)      );
   AddMessageHandler( WM_NOTIFY,        reinterpret_cast<MessageHandler>(MWBaseOnNotify)       );
   AddMessageHandler( WM_TIMER,         reinterpret_cast<MessageHandler>(MWBaseOnTimer)        );
   AddMessageHandler( WM_SETFOCUS,      reinterpret_cast<MessageHandler>(MWBaseOnSetFocus)     );
   AddMessageHandler( WM_KILLFOCUS,     reinterpret_cast<MessageHandler>(MWBaseOnKillFocus)    );
   AddMessageHandler( WM_LBUTTONDBLCLK, reinterpret_cast<MessageHandler>(MWBaseOnLDoubleClick) );
   AddMessageHandler( WM_RBUTTONDBLCLK, reinterpret_cast<MessageHandler>(MWBaseOnRDoubleClick) );
   AddMessageHandler( WM_CLOSE,         reinterpret_cast<MessageHandler>(MWBaseOnClose)        );
}

MainWindow::~MainWindow( void )
{

}

//void MainWindow::RegisterLuaFunctions( void )
//{
//   luaDrawSprites.Register( m_LuaState, "luaDrawSprites" );
//}

bool MainWindow::CheckCollide( const Structs::Rect& Rect, const Structs::Point& Point )
{
   if( (Rect.bottom <= Point.y) ) return false;
   if( (Rect.top    >= Point.y) ) return false;
   if( (Rect.right  <= Point.x) ) return false;
   if( (Rect.left   >= Point.x) ) return false;
   return true;
}

void MainWindow::Initialise( void )
{
   // status bar
   C_WinStatusBar::V_Parts SBParts;
   SBParts.push_back( C_WinStatusBar::S_Part( "Ready...",      30 ) );
   SBParts.push_back( C_WinStatusBar::S_Part( "TileSet Size:", 30 ) );
   SBParts.push_back( C_WinStatusBar::S_Part( "width:  ",      10 ) );
   SBParts.push_back( C_WinStatusBar::S_Part( "height: ",      10 ) );
   SBParts.push_back( C_WinStatusBar::S_Part( "x: ",           10 ) );
   SBParts.push_back( C_WinStatusBar::S_Part( "y: ",           10 ) );

//   m_StatusBar.Create( m_WinBase, 400, SBParts );
//   m_StatusBar.SetSimpleModeText( "Ready ..." );
//   m_StatusBar.EnableSimpleMode();

   // MapsTab
   C_WinTab::V_String TileSetsTabStrings;
//   TileSetsTabStrings.push_back( "Tile Set" );
   m_TileSetsTab.Create( m_WinBase, 401, TileSetsTabStrings, "", CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT );

   // TileSetsTab
   C_WinTab::V_String MapsTabStrings;
//   MapsTabStrings.push_back( "Untitled" );
   m_MapsTab.Create( m_WinBase, 402, MapsTabStrings, "", CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT );

   // MapsTabStatic
   m_MapsTabStatic.Create( m_WinBase, 403, "", CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT );

   // TileSetsTabStatic   
   m_TileSetsTabStatic.Create( m_WinBase, 404, "", CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT );

   // MapsVScroll
   m_MapsVScroll.Create( m_WinBase, 405, "", CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,  true );

   // MapsHScroll
   m_MapsHScroll.Create( m_WinBase, 406, "", CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, false );

   // TileSetsVScroll
   m_TileSetsVScroll.Create( m_WinBase, 407, "", CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,  true );

   // TileSetsHScroll
   m_TileSetsHScroll.Create( m_WinBase, 408, "", CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, false );

   // Tilesettab2
   m_TileSetsTabStatic2.Create( m_WinBase, 409, "", CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, false );

   // tool tip
//   Structs::Rect rect;
//   m_EnLargedTileToolTip.Create( m_WinBase, 410, rect, "Currently Selected Tile" );

   m_MapsStaticDCs.SetHwnd( m_MapsTabStatic );
   m_TileSetsStaticDCs.SetHwnd( m_TileSetsTabStatic );
   m_TileSetsStaticDCs2.SetHwnd( m_TileSetsTabStatic2 );

   m_AstarStart.Load(  "astarstart.bmp" );
   m_AstarEnd.Load(    "astarend.bmp"   );
   m_HBUp.Load(        "up.bmp"         );
   m_HBDown.Load(      "down.bmp"       );
   m_HBLeft.Load(      "left.bmp"       );
   m_HBRight.Load(     "right.bmp"      );
   m_HBDownRight.Load( "downright.bmp"  );
   m_HBUpRight.Load(   "upright.bmp"    );
   m_HBDownLeft.Load(  "downleft.bmp"   );
   m_HBUpLeft.Load(    "upleft.bmp"     );

   m_AstarStart.SetHwnd( m_MapsTabStatic );
   m_AstarEnd.SetHwnd( m_MapsTabStatic );
   m_HBUp.SetHwnd( m_MapsTabStatic );
   m_HBDown.SetHwnd( m_MapsTabStatic );
   m_HBLeft.SetHwnd( m_MapsTabStatic );
   m_HBRight.SetHwnd( m_MapsTabStatic );
   m_HBDownRight.SetHwnd( m_MapsTabStatic );
   m_HBUpRight.SetHwnd( m_MapsTabStatic );
   m_HBDownLeft.SetHwnd( m_MapsTabStatic );
   m_HBUpLeft.SetHwnd( m_MapsTabStatic );

}

void MainWindow::ResizeWindows( int width, int height )
{
   HDWP hdwp = ::BeginDeferWindowPos( 10 );
   Structs::Rect Rect;
   int maptab_width    = (int)((float)(width*0.7));
   int tilestab_width  = (int)((float)(width*0.3));
   int st = ::GetSystemMetrics( SM_CXVSCROLL );
   m_WinBase.GetClientRect( Rect );
   TabCtrl_AdjustRect( m_MapsTab, false, &Rect );
   m_yoffset      = Rect.top;
   m_xoffset      = Rect.left;
   int sbarhieght = ::GetWindowHeight( m_StatusBar );
   int x_border   = ::GetSystemMetrics( SM_CXBORDER );
   int y_border   = ::GetSystemMetrics( SM_CYBORDER );

   // 1 Size the map tab control to fit the client area. 
   ::DeferWindowPos( hdwp, m_MapsTab, NULL, 0, 0, maptab_width, height-sbarhieght, SWP_NOZORDER );

   // 2 Size the tiles tab control to fit the client area. 
   ::DeferWindowPos( hdwp, m_TileSetsTab, NULL, maptab_width, 0, tilestab_width, height-sbarhieght, SWP_NOZORDER );

   // 3 Size the MapsTabStatic Static control to fit the tab
   ::DeferWindowPos( hdwp, m_MapsTabStatic, HWND_TOP, m_xoffset, m_yoffset, maptab_width-st-m_xoffset, height-st-m_yoffset-sbarhieght, 0 );

   // 4 Size the TileSetsTabStatic Static control to fit the tab
   ::DeferWindowPos( hdwp, m_TileSetsTabStatic, HWND_TOP, maptab_width+m_xoffset, m_yoffset, tilestab_width-st-m_xoffset, (height-st-m_yoffset-sbarhieght)/2, 0 );

   // 5 Size the TileSetsTabStatic Static control to fit the tab
   ::DeferWindowPos( hdwp, m_TileSetsTabStatic2, HWND_TOP, maptab_width+m_xoffset, m_yoffset+(height-st-m_yoffset-sbarhieght)/2, tilestab_width-st-m_xoffset, (height-st-m_yoffset-sbarhieght)/2, 0 );

   // 6 Size the map tab vscroll control to fit the tab
   ::DeferWindowPos( hdwp, m_MapsVScroll, HWND_TOP, maptab_width-st-x_border, m_yoffset, st, height-st-m_yoffset-sbarhieght, 0 );

   // 7 Size the map tab hscroll control to fit the tab
   ::DeferWindowPos( hdwp, m_MapsHScroll, HWND_TOP, 0, height-st-sbarhieght, maptab_width-st, st, 0 );

   // 8 Size the tileset tab vscroll control to fit the tab
   ::DeferWindowPos( hdwp, m_TileSetsVScroll, HWND_TOP, width-st-x_border, m_yoffset, st, height-st-m_yoffset-sbarhieght, 0 );

   // 9 Size the tileset tab hscroll control to fit the tab
   ::DeferWindowPos( hdwp, m_TileSetsHScroll, HWND_TOP, maptab_width, height-st-sbarhieght, tilestab_width-st, st, 0 );

   // 10 Size the tiles tab hscroll control to fit the tab
   ::DeferWindowPos( hdwp, m_StatusBar, HWND_TOP, 0, 0, 0, 0, 0 );

   ::EndDeferWindowPos( hdwp );

//   Structs::Rect EnLargedTileToolTipRect;
//   m_WinBase.GetClientRect( EnLargedTileToolTipRect );
//   EnLargedTileToolTipRect.left   = maptab_width+m_xoffset;
//   EnLargedTileToolTipRect.top    = m_yoffset+(height-st-m_yoffset-sbarhieght)/2;
//   EnLargedTileToolTipRect.right  = tilestab_width-st-m_xoffset;
//   EnLargedTileToolTipRect.bottom = (height-st-m_yoffset-sbarhieght)/2;
//   m_EnLargedTileToolTip.SetRect( EnLargedTileToolTipRect );

   m_MapVisableX = GetWindowWidth( m_MapsTabStatic ) - st;
   m_MapVisableY = GetWindowHeight( m_MapsTabStatic ) - st;

   m_TileSetTabStaticVisibleX = GetWindowWidth( m_TileSetsTabStatic );// - st;
   m_TileSetTabStaticVisibleY = GetWindowHeight( m_TileSetsTabStatic );// - st;

   m_MapsStaticDCs.m_MemoryDC.CreateBitmap( m_MapsTabStatic, GetWindowWidth( m_MapsTabStatic ), GetWindowHeight( m_MapsTabStatic ) );
   m_TileSetsStaticDCs.m_MemoryDC.CreateBitmap( m_TileSetsTabStatic, GetWindowWidth( m_TileSetsTabStatic ), GetWindowHeight( m_TileSetsTabStatic ) );
}

void MainWindow::ResetScrollBars( void )
{
   SCROLLINFO si;

   //if( m_TileMaps.size() && MapsScrolls.size() && TileSetsScrolls.size() && m_TileSetsIDs.size() )
   if( m_TileMaps.size() && MapsScrolls.size() && TileSetsScrolls.size() && m_TileSetsIDs.size()
	   && (MapsScrolls.size() > m_CurrentMap)
	 )
   {
	  int tileMapsCurrentMapheight     = m_TileMaps[m_CurrentMap].height();
	  int tileMapsCurrentMaptileheight = m_TileMaps[m_CurrentMap].tileheight();
	  if(MapsScrolls.size()>m_CurrentMap)
	  {
		  MapsScrolls[m_CurrentMap].yMaxScroll = max((tileMapsCurrentMapheight * tileMapsCurrentMaptileheight), 0);
	  }
	  
	  if (MapsScrolls.size() > m_CurrentMap)
	  {
		  int mapsScrollsCurrentMapyCurrentScrol = MapsScrolls[m_CurrentMap].yCurrentScroll;
		  int mapsScrollsCurrentMapyMaxScroll = MapsScrolls[m_CurrentMap].yMaxScroll;

		  MapsScrolls[m_CurrentMap].yCurrentScroll = min(mapsScrollsCurrentMapyCurrentScrol, mapsScrollsCurrentMapyMaxScroll);
	  }

      //MapsScrolls[ m_CurrentMap ].yMaxScroll     = max( (m_TileMaps[ m_CurrentMap ].height()*m_TileMaps[ m_CurrentMap ].tileheight()), 0 );
      //MapsScrolls[ m_CurrentMap ].yCurrentScroll = min( MapsScrolls[ m_CurrentMap ].yCurrentScroll, MapsScrolls[ m_CurrentMap ].yMaxScroll );

      // MapsVScroll Set the vertical scrolling range and page size. 
      si.cbSize = sizeof(si); 
      si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
      si.nMin   = MapsScrolls[ m_CurrentMap ].yMinScroll; //Alert( MapsScrolls[ m_CurrentMap ].yMinScroll );
      si.nMax   = MapsScrolls[ m_CurrentMap ].yMaxScroll; //Alert( MapsScrolls[ m_CurrentMap ].yMaxScroll );
      si.nPage  = m_MapVisableY; //Alert( m_MapVisableY );//m_MapVisableY;
      si.nPos   = MapsScrolls[ m_CurrentMap ].yCurrentScroll;
      ::SetScrollInfo( m_MapsVScroll, SB_CTL, &si, TRUE ); 

      MapsScrolls[ m_CurrentMap ].xMaxScroll     = max( (m_TileMaps[ m_CurrentMap ].width()*m_TileMaps[ m_CurrentMap ].tilewidth()), 0 );
      MapsScrolls[ m_CurrentMap ].xCurrentScroll = min( MapsScrolls[ m_CurrentMap ].xCurrentScroll, MapsScrolls[ m_CurrentMap ].xMaxScroll );
 
      // MapsHScroll Set the horizontal scrolling range and page size. 
      si.cbSize = sizeof(si); 
      si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
      si.nMin   = MapsScrolls[ m_CurrentMap ].xMinScroll;
      si.nMax   = MapsScrolls[ m_CurrentMap ].xMaxScroll;
      si.nPage  = m_MapVisableX;
      si.nPos   = MapsScrolls[ m_CurrentMap ].xCurrentScroll;
      ::SetScrollInfo( m_MapsHScroll, SB_CTL, &si, TRUE );

      TileSetsScrolls[ m_CurrentMap ].yMaxScroll     = 0;//max( m_TileSetsIDs[ m_CurrentMap ].size()*m_TileMaps[ m_CurrentMap ].tileheight(), 0 );
      TileSetsScrolls[ m_CurrentMap ].yCurrentScroll = 0;//min( TileSetsScrolls[ m_CurrentMap ].yCurrentScroll, TileSetsScrolls[ m_CurrentMap ].yMaxScroll );

      // Set the vertical scrolling range and page size. 
      si.cbSize = sizeof(si); 
      si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;;
      si.nMin   = TileSetsScrolls[ m_CurrentMap ].yMinScroll;
      si.nMax   = TileSetsScrolls[ m_CurrentMap ].yMaxScroll;
      si.nPage  = m_TileSetTabStaticVisibleY;
      si.nPos   = TileSetsScrolls[ m_CurrentMap ].yCurrentScroll;
      ::SetScrollInfo( m_TileSetsVScroll, SB_CTL, &si, TRUE ); 

      TileSetsScrolls[ m_CurrentMap ].xMaxScroll     = 0;//max( m_TileSetsIDs[ m_CurrentMap ][0].size()*m_TileMaps[ m_CurrentMap ].tilewidth(), 0 );
      TileSetsScrolls[ m_CurrentMap ].xCurrentScroll = 0;//min( TileSetsScrolls[ m_CurrentMap ].xCurrentScroll, TileSetsScrolls[ m_CurrentMap ].xMaxScroll );
 
      // Set the horizontal scrolling range and page size. 
      si.cbSize = sizeof(si); 
      si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
      si.nMin   = TileSetsScrolls[ m_CurrentMap ].xMinScroll;
      si.nMax   = TileSetsScrolls[ m_CurrentMap ].xMaxScroll;
      si.nPage  = m_TileSetTabStaticVisibleX;
      si.nPos   = TileSetsScrolls[ m_CurrentMap ].xCurrentScroll;
      ::SetScrollInfo( m_TileSetsHScroll, SB_CTL, &si, TRUE );
   }
   else
   {
      si.cbSize = sizeof(si); 
      si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
      si.nMin   = 0;
      si.nMax   = 0;
      si.nPage  = m_MapVisableY;
      si.nPos   = 0;
      ::SetScrollInfo( m_MapsVScroll, SB_CTL, &si, TRUE ); 
 
      // MapsHScroll Set the horizontal scrolling range and page size. 
      si.cbSize = sizeof(si); 
      si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
      si.nMin   = 0;
      si.nMax   = 0;
      si.nPage  = m_MapVisableX;
      si.nPos   = 0;
      ::SetScrollInfo( m_MapsHScroll, SB_CTL, &si, TRUE );

      // Set the vertical scrolling range and page size. 
      si.cbSize = sizeof(si); 
      si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;;
      si.nMin   = 0;
      si.nMax   = 0;
      si.nPage  = m_TileSetTabStaticVisibleY;
      si.nPos   = 0;
      ::SetScrollInfo( m_TileSetsVScroll, SB_CTL, &si, TRUE ); 

      // Set the horizontal scrolling range and page size. 
      si.cbSize = sizeof(si); 
      si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
      si.nMin   = 0;
      si.nMax   = 0;
      si.nPage  = m_TileSetTabStaticVisibleX;
      si.nPos   = 0;
      ::SetScrollInfo( m_TileSetsHScroll, SB_CTL, &si, TRUE );
   }
}

void MainWindow::OpenHelp( void )
{
   if( !OpenModule( m_WinBase, "TileMapEdtior.hlp" ) )
   {
      Alert( "Could Not Find Help File" );
   }
}

bool MainWindow::LoadConfig( void )
{
   TiXmlDocument config( m_ConfgFileName.c_str() );
   if( !config.LoadFile() )
   {
      return 0;
   }
   TiXmlHandle   configHandle( &config );
   TiXmlElement *parentElement = configHandle.FirstChild( "CONFIG" ).Element();
   if( parentElement )
   {
      TiXmlElement *varElement = configHandle.FirstChildElement( "CONFIG" ).FirstChildElement( "VAR" ).Element();
      if( varElement )
      {
         if( varElement->Attribute( "lastmap" ) )
         {
            std::string MapFileName = varElement->Attribute( "lastmap" );
            if( LoadMap( MapFileName ) )
            {
               MakeNewTab();
            }
            else
            {
               OpenTileMap();
            }
         }
         else
         {
            OpenTileMap();
         }
      }
   }
}

bool MainWindow::SaveConfig( void )
{
   TiXmlNode *configNode      = NULL;
   TiXmlNode *varNode         = NULL;
   TiXmlNode *DeclarationNode = NULL;
   try
   {
      TiXmlDocument config;
      configNode      = new TiXmlElement( "CONFIG" );
      varNode         = new TiXmlElement( "VAR" );

      // var lastmap
      std::string MapFileName;
      if( m_TileMaps.size() > m_CurrentMap )
      {
         MapFileName = m_TileMaps[ m_CurrentMap ].GetMapFileName();
      }
      varNode->ToElement()->SetAttribute( "lastmap", MapFileName.c_str() );
      configNode->LinkEndChild( varNode );
      //

      // finally add the config node
      config.InsertEndChild( *configNode->ToElement() );
      config.SaveFile( m_ConfgFileName.c_str() );
      delete configNode; // varNode will be deleted as well
      return true;
   }
   catch( ... )
   {
      delete configNode; // varNode will be deleted as well
      return false;
   }
}

void MainWindow::SaveTileMap( void )
{
   if( m_TileMaps.size() )
   {
      std::string MapFileName;
      m_WinFileHandler.SaveFile( MapFileName, "Tile Map Files *.tm\0*.tm\0All files *.*\0*.*\0\0",
                                 ".tm", 0, 0, "Save Map" );
      if( MapFileName.size() )
      {
         if( !m_TileMaps[ m_CurrentMap ].SaveMap( MapFileName ) )
         {
            Alert( "Could Not Save Map" + GetLastSystemError() );
         }
         else
         {
            m_MapsTab.RenameTab( MapFileName, m_CurrentMap );
            m_TileMaps[ m_CurrentMap ].SetMapFileName( MapFileName );
            m_StatusBar.EnableSimpleMode( "File " + MapFileName + " Saved" );
         }

         std::string TileSetFileName = MapFileName;
         m_WinFileHandler.SaveFile( TileSetFileName, "TileSet Files *.ts\0*.ts\0All files *.*\0*.*\0\0",
                                    ".ts", 0, 0, "Save TileSet" );
         if( TileSetFileName.size() )
         {
            if( !m_TileMaps[ m_CurrentMap ].SaveTileSet( TileSetFileName ) )
            {
               Alert( "Could Not Save TileSet: " + GetLastSystemError() );
            }
            else
            {
               m_TileSetsTab.RenameTab( TileSetFileName, m_CurrentMap );
               m_TileMaps[ m_CurrentMap ].SetTileSetFileName( TileSetFileName );
               m_StatusBar.EnableSimpleMode( "File " + TileSetFileName + " Saved" );
               m_TileMaps[ m_CurrentMap ].SaveMap( MapFileName );
            }
         }
      }
   }
}

void MainWindow::CloseTileMap( int index )
{
   if( (index >= m_TileMaps.size()) || (index < 0) || !m_TileMaps.size() )
      return;
   m_MapsTab.RemoveTab( index );
   m_TileSetsTab.RemoveTab( index );
   RemoveMap( index );
   m_CurrentMap = m_CurrentMap-1;
   if( m_CurrentMap < 0 )
      m_CurrentMap = 0;
   m_MapsTab.SelectTab( m_CurrentMap );
   m_TileSetsTab.SelectTab( m_CurrentMap );

   ResetScrollBars();
   DrawMap( m_CurrentMap, m_MapsStaticDCs.m_GetDC );
   DrawAStarPath( m_CurrentMap, m_MapsStaticDCs.m_GetDC );
   DrawTileSet( m_CurrentMap, m_TileSetsStaticDCs.m_GetDC );
   m_TileSetsIDs.erase( m_TileSetsIDs.begin()+m_CurrentMap );
   m_CurrentTileGraphics.erase( m_CurrentTileGraphics.begin()+m_CurrentMap );
   m_Solidity.erase( m_Solidity.begin()+m_CurrentMap );
   MapsScrolls.erase( MapsScrolls.begin()+m_CurrentMap );
   TileSetsScrolls.erase( TileSetsScrolls.begin()+m_CurrentMap );
}

bool MainWindow::ReNameMap( void )
{
   bool ret = false;
   std::string MapFileName;
   m_WinFileHandler.SaveFile( MapFileName, "Tile Map Files *.tm\0*.tm\0All files *.*\0*.*\0\0",
                              ".tm", 0, 0, "Save Map" );
   if( MapFileName.size() )
   {
      if( !m_TileMaps[ m_CurrentMap ].SaveMap( MapFileName ) )
      {
         Alert( "Could Not Re-Name Map" + GetLastSystemError() );
      }
      else
      {
         m_MapsTab.RenameTab( MapFileName, m_CurrentMap );
         m_TileMaps[ m_CurrentMap ].SetMapFileName( MapFileName );
         m_StatusBar.EnableSimpleMode( "File " + MapFileName + " Saved" );
      }
   }
   return ret;
}

void MainWindow::OpenTileMap( void )
{
   chdir( m_AppOrignalWorkingDirectory.c_str() );
   std::string MapFileName;
   m_WinFileHandler.OpenFile( MapFileName, "Tile Map Files *.tm\0*.tm\0All files *.*\0*.*\0\0",
                              ".tm", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER,
                              0, "Open" );
      if( MapFileName.size() )
      {
         std::deque< std::string > filenames = m_WinFileHandler.GetMultiSelectFileNames();
         std::string current_directory       = m_WinFileHandler.GetCurrentDirectory();
         if( filenames.size() == 1 )
         {
//            m_TileMaps[ m_CurrentMap ].AddTile( MapFileName );
            if( LoadMap( MapFileName ) )
               MakeNewTab();
            else
               Alert( "Map Could Not be Loaded.\n" + MapFileName );
         }
         else
         {
            for( int i=0; i<filenames.size() ; ++i )
            {
//               Alert( current_directory+filenames[i] );
//               m_TileMaps[ m_CurrentMap ].AddTile( filenames[i] );
               if( LoadMap( current_directory+filenames[i] ) )
               {
                  MakeNewTab();
               }
               else
               {
                  Alert( "Map Could Not be Loaded.\n" + filenames[i] );
               }
            }
         }
      }

   DrawMap( m_CurrentMap, m_MapsStaticDCs.m_GetDC );
   DrawAStarPath( m_CurrentMap, m_MapsStaticDCs.m_GetDC );
   DrawTileSet( m_CurrentMap, m_TileSetsStaticDCs.m_GetDC );
   DrawCurrentTile( m_CurrentMap, m_TileSetsStaticDCs2.m_GetDC );
   ::SetWindowPos( m_WinBase, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
}

void MainWindow::OpenTile( void )
{
   if( m_TileMaps.size() )
   {
      std::string TileFileName;
      m_WinFileHandler.OpenFile( TileFileName, "Bitmap Files *.bmp\0*.bmp\0All files *.*\0*.*\0\0",
                                 ".bmp", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER, 0, "Open" );
      if( TileFileName.size() )
      {
         std::deque< std::string > filenames = m_WinFileHandler.GetMultiSelectFileNames();
         if( filenames.size() == 1 )
         {
            m_TileMaps[ m_CurrentMap ].AddTile( TileFileName );
         }
         else
         {
            for( int i=0; i<filenames.size() ; ++i )
            {
               if( !m_TileMaps[ m_CurrentMap ].AddTile( filenames[i] ) )
               {
                  Alert( "Tile could not be loaded: " + filenames[i] );
               }
            }
         }
      }
      ResizeTileSetsIDs();
      DrawMap( m_CurrentMap, m_MapsStaticDCs.m_GetDC );
      DrawAStarPath( m_CurrentMap, m_MapsStaticDCs.m_GetDC );
      DrawTileSet( m_CurrentMap, m_TileSetsStaticDCs.m_GetDC );
      DrawCurrentTile( m_CurrentMap, m_TileSetsStaticDCs2.m_GetDC );
   }
   else
   {
      Alert( "You Must Load Map First" );
   }
}

void MainWindow::MakeNewTab( void )
{
   try
   {
      if( m_TileMaps.size() > m_MapsTab.NumberOfTabs() )
      {
         m_MapsTab.AddTab( m_TileMaps[ m_MapsTab.NumberOfTabs() ].GetMapFileName(), m_MapsTab.NumberOfTabs() );
         m_TileSetsTab.AddTab( m_TileMaps[ m_TileSetsTab.NumberOfTabs() ].GetTileSetFileName(), m_TileSetsTab.NumberOfTabs() );
         m_CurrentMap = m_TileSetsTab.NumberOfTabs()-1;
         m_MapsTab.SelectTab( m_CurrentMap );
         m_TileSetsTab.SelectTab( m_CurrentMap );
         m_TileSetsIDs.resize( m_TileMaps.size() );
         m_CurrentTileGraphics.resize( m_TileMaps.size() );
         m_Solidity.resize( m_TileMaps.size() );
         MapsScrolls.resize( m_TileMaps.size() );
         TileSetsScrolls.resize( m_TileMaps.size() );
         //*m_CurrentTileGraphics.end() = 0;
         //*m_Solidity.end()            = 0;
         ResizeTileSetsIDs();
         ResetScrollBars();
         DrawMap( m_CurrentMap, m_MapsStaticDCs.m_GetDC );
         DrawAStarPath( m_CurrentMap, m_MapsStaticDCs.m_GetDC );
         DrawTileSet( m_CurrentMap, m_TileSetsStaticDCs.m_GetDC );
         ::SetWindowPos( m_WinBase, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
      }
   }
   catch( ... )
   {
   }
}

bool MainWindow::LoadMap( std::string filename )
{
   try
   {
      bool ret = false;
      try
      {
         m_TileMaps.push_back( TileMapLib::C_TileMap() ) ;
      }
      catch( std::string s )
      {
         Alert( s );
         return false;
      }
      catch( std::exception& e )
      {
         Alert( e.what() );
         return false;
      }
      catch( ... )
      {
         Alert( "Error adding a new map to the list." );
         return false;
      }
      ret = m_TileMaps[ m_TileMaps.size()-1 ].LoadMap( filename );
      if( !ret )
      {
         m_TileMaps.pop_back();
      }
      ::chdir( m_AppOrignalWorkingDirectory.c_str() );
      return ret;
   }
   catch( ... )
   {
      return false;
   }
}

bool MainWindow::LoadMapAndMakeTab( std::string filename )
{
   if( !LoadMap( filename ) )
   {
      Alert( "Could not load tile map.\n" + filename );
      return false;
   }
   else
   {
      MakeNewTab();
      return true;
   }
   return false;
}

bool MainWindow::RemoveMap( int index )
{
   if( m_TileMaps.size() > index )
   {
      m_TileMaps.erase( m_TileMaps.begin()+index );
	  return true;
   }
   return false;
}

void MainWindow::DrawCurrentTile( int index, C_HDC& hdc )
{
   hdc.Clear( GRAY_BRUSH );
   if( m_TileMaps.size() 
	   && (m_TileMaps.size() > index)
	   && (m_CurrentTileGraphics.size() > index)
	 )
   {
      if( m_CurrentTileGraphics[index] < m_TileMaps[index].Bitmaps().size() )
      {
         m_TileMaps[index].Bitmaps()[ m_CurrentTileGraphics[index] ].StretchDisplay( hdc, 0, 0, m_TileMaps[ m_CurrentMap ].tilewidth()*8, m_TileMaps[ m_CurrentMap ].tilewidth()*8 );
      }
   }
}

void MainWindow::DrawSprites( int index, C_HDC& hdc )
{
   if( m_TileMaps.size() > index )
   {
      std::deque< C_Sprite >& Sprites = m_TileMaps[ m_CurrentMap ].GetSprites();
      for( int i=0; i<m_TileMaps[index].GetSpriteIDs().size() ; ++i )
      {
         if( m_TileMaps[index].GetSpriteIDs()[i].id < Sprites.size() )
         {
            std::deque< C_Sprite::S_Frame >& Frames = Sprites[ m_TileMaps[index].GetSpriteIDs()[i].id ].GetFrames();
            SpriteMap::D_HBitmaps& Images = m_TileMaps[ m_CurrentMap ].GetSpriteBitmaps();
            int img_index = Frames.front().m_index;
            if( img_index < Images.size() )
            {            
               Images[img_index].Display( hdc, m_TileMaps[index].GetSpriteIDs()[i].pos.x*m_TileMaps[index].tilewidth(), 
                                               m_TileMaps[index].GetSpriteIDs()[i].pos.y*m_TileMaps[index].tileheight(), 
                                               SRCCOPY );
            }
         }
      }
   }
}

void MainWindow::DrawMap( int index, C_HDC& hdc )
{
   hdc.Clear();
   if( index < 0 )
      return;
   if( m_TileMaps.size() > index )
   {
	  int N = m_TileMaps[index].GetEndDrawTile().x;
      for( int r=0, row = m_TileMaps[index].GetStartDrawTile().y; r<m_TileMaps[index].GetEndDrawTile().y ; ++r, ++row )
      {
         for( int c=0, col = m_TileMaps[index].GetStartDrawTile().x; c<m_TileMaps[index].GetEndDrawTile().x ; ++c, ++col )
         {
            if( ( (c+1)*m_TileMaps[index].tilewidth() ) > m_MapVisableX )
               break;
            if( ( (r+1)*m_TileMaps[index].tileheight() ) > m_MapVisableY )
               break;
            if( m_TileMaps[index].height() > row )
            {
               if( m_TileMaps[index].Bitmaps().size() > (m_TileMaps[index])[row][col].m_TileGraphic )
               {
                  m_TileMaps[index].Bitmaps()[ (m_TileMaps[index])[row][col].m_TileGraphic ].Display( hdc, c*m_TileMaps[index].tilewidth(), r*m_TileMaps[index].tileheight(), ( m_TileMaps[index].IsSelected(row,col) ) ? NOTSRCCOPY : SRCCOPY );
               }
               //
               if( m_TileMaps[index].ShowStartAndEndNode() )
               {
                  if( (m_TileMaps[index].GetAStarStartPos().x == col) && (m_TileMaps[index].GetAStarStartPos().y == row) )
                  {
                     m_AstarStart.Display( hdc, c*m_TileMaps[index].tilewidth(), r*m_TileMaps[index].tileheight(), SRCAND );					 
                  }
                  if( (m_TileMaps[index].GetAStarEndPos().x == col) && (m_TileMaps[index].GetAStarEndPos().y == row) )
                  {
                     m_AstarEnd.Display( hdc, c*m_TileMaps[index].tilewidth(), r*m_TileMaps[index].tileheight(), SRCAND );
                  }
               }
            }
         }
      }
      DrawSprites( index, hdc );
   }
}

void MainWindow::DrawAStarNodes( int index, C_HDC& hdc )
{
   if( m_TileMaps.size() > m_CurrentMap )
   {
      if( !m_TileMaps[ m_CurrentMap ].shownodessearched() )
         DrawAStarPath( m_CurrentMap, hdc );
      else
         DrawAStarNodesSearched( m_CurrentMap, hdc );
   }
}

void MainWindow::DrawAStarPath( int index, C_HDC& hdc )
{
   if( m_TileMaps.size() > index )
   {
      if( !m_TileMaps[index].showpath() )
         return;
      std::vector<AStar::AStar2d::PointAndDirection>& path = m_TileMaps[index].GetAStarPath();
      int i = path.size();

      int width   = m_TileMaps[index].tilewidth();
      int height  = m_TileMaps[index].tileheight();
      int start_x = m_TileMaps[index].GetStartDrawTile().x;
      int start_y = m_TileMaps[index].GetStartDrawTile().y;
      int xpos    = 0;
      int ypos    = 0;

      for( int i = 0; i<path.size() ; i++ )
      {

         xpos = path[i].pos.x-start_x;
         ypos = path[i].pos.y-start_y;
         if( (ypos < 0) || (xpos < 0) )
         {
            continue;
         }
         if( (height*ypos > m_MapVisableY) || (width*xpos > m_MapVisableX) )
         {
            continue;
         }

         switch( path[i].oreintation )
         {
            case AStar::AStar2d::PointAndDirection::UP:
            {
               m_HBUp.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::DOWN:
            {
               m_HBDown.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::LEFT:
            {
               m_HBLeft.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::RIGHT:
            {
               m_HBRight.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::UPRIGHT:
            {
               m_HBUpRight.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::UPLEFT:
            {
               m_HBUpLeft.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::DOWNRIGHT:
            {
               m_HBDownRight.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::DOWNLEFT:
            {
               m_HBDownLeft.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
         }
      }      
   }
}

void MainWindow::DrawAStarNodesSearched( int index, C_HDC& hdc )
{
   if( m_TileMaps.size() > index )
   {
      if( !m_TileMaps[index].showpath() )
         return;
      std::vector<AStar::AStar2d::PointAndDirection>& path = m_TileMaps[index].GetAStarNodesSearched();
      int i = path.size();

      int width   = m_TileMaps[index].tilewidth();
      int height  = m_TileMaps[index].tileheight();
      int start_x = m_TileMaps[index].GetStartDrawTile().x;
      int start_y = m_TileMaps[index].GetStartDrawTile().y;
      int xpos    = 0;
      int ypos    = 0;

      for( int i = 0; i<path.size() ; i++ )
      {

         xpos = path[i].pos.x-start_x;
         ypos = path[i].pos.y-start_y;
         if( (ypos < 0) || (xpos < 0) )
         {
            continue;
         }
         if( (height*ypos > m_MapVisableY) || (width*xpos > m_MapVisableX) )
         {
            continue;
         }

         switch( path[i].oreintation )
         {
            case AStar::AStar2d::PointAndDirection::UP:
            {
               m_HBUp.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::DOWN:
            {
               m_HBDown.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::LEFT:
            {
               m_HBLeft.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::RIGHT:
            {
               m_HBRight.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::UPRIGHT:
            {
               m_HBUpRight.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::UPLEFT:
            {
               m_HBUpLeft.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::DOWNRIGHT:
            {
               m_HBDownRight.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
            case AStar::AStar2d::PointAndDirection::DOWNLEFT:
            {
               m_HBDownLeft.Display( hdc, width*xpos, height*ypos, SRCCOPY );
            } break;
         }
      }      
   }
}

void MainWindow::DrawTileSet( int index, C_HDC& hdc )
{
   hdc.Clear();

   if( m_TileMaps.size() > index )
   {
      if( m_TileSetsIDs.size() < m_TileMaps.size() )
      {
         return;
      }
      if( m_CurrentTileGraphics.size() < m_TileMaps.size() )
      {
         return;
      }
      if( m_TileMaps[index].Bitmaps().size() )
      {
        int tilewidth  = m_TileMaps[index].tilewidth();
        int tileheight = m_TileMaps[index].tileheight();
         for( int r = 0; r < m_TileSetsIDs[index].size() ; ++r )
         {
            for( int c = 0 ; c < m_TileSetsIDs[index][r].size() ; ++c )
            {
               if( m_TileSetsIDs[index][r][c] >= m_TileMaps[index].Bitmaps().size() )
                  break;
               m_TileMaps[index].Bitmaps()[m_TileSetsIDs[index][r][c]].Display( hdc, c*tilewidth, r*tileheight, ( m_TileSetsIDs[index][r][c] == m_CurrentTileGraphics[ m_CurrentMap ] ) ? NOTSRCCOPY : SRCCOPY );
            }
         }
      }
   }
}

void MainWindow::ResizeTileSetsIDs( void )
{
   if( m_TileSetsIDs.size() > m_CurrentMap )
   {
      int max_x = GetWindowWidth( m_TileSetsTabStatic )/m_TileMaps[ m_CurrentMap ].tilewidth();
      if( max_x <= 0 )
         max_x = 1;
      int max_y = (int)(m_TileMaps[ m_CurrentMap ].Bitmaps().size()/max_x)+1;
      m_TileSetsIDs[ m_CurrentMap ].clear();
      int index = 0;
      while( (max_y*m_TileMaps[ m_CurrentMap ].tilewidth()) > GetWindowHeight( m_TileSetsTabStatic ) )
      {
         --max_y;
      }
      if( max_y <= 0 )
      {
         return;
      }
      for( int r=0; r<max_y ; ++r )
      {
         std::deque<int> row;
         for( int c=0; c<max_x; ++c )
         {
            if( index >= m_TileMaps[ m_CurrentMap ].Bitmaps().size() )
            {
               m_TileSetsIDs[m_CurrentMap].push_back( row );
               return;
            }
            row.push_back( index++ );
         }
         m_TileSetsIDs[m_CurrentMap].push_back( row );
      }
   }
}

void MainWindow::HilightTile( void )
{
}

void MainWindow::HilightMouseOverTile( Structs::Point tile )
{

   if( tile != m_PreviousMouseOverTile )
   {
      if( m_TileMaps.size() > m_CurrentMap )
      {         
         m_PreviousMouseOverTile = tile;
         DrawMap( m_CurrentMap, m_MapsStaticDCs.m_MemoryDC );
         DrawAStarNodes( m_CurrentMap, m_MapsStaticDCs.m_MemoryDC );
         if( ( tile.x >= 0 ) || ( tile.y >= 0 ) )
         {
            Structs::Point starttile = m_TileMaps[ m_CurrentMap ].GetStartDrawTile();
            // Draging the mouse 
            // each new tile is changed to the tile graphic that the mouse clicked before it was dragged
            if( m_bLMouseDown )
            {
               if( ValidTileIndex( tile ) )
               {
                  m_TileMaps[ m_CurrentMap ].SetTileGraphic( tile.y+starttile.y, tile.x+starttile.x, m_CurrentTileGraphics[ m_CurrentMap ] );
                  (m_TileMaps[m_CurrentMap])[tile.y+starttile.y][tile.x+starttile.x].m_Solidity = m_Solidity[ m_CurrentMap ];
                  DrawMap( m_CurrentMap, m_MapsStaticDCs.m_MemoryDC );
                  DrawAStarNodes( m_CurrentMap, m_MapsStaticDCs.m_MemoryDC );               }
            }
            if( ValidTileIndex( tile ) )
            {
               if( m_TileMaps[ m_CurrentMap ].Bitmaps().size() )
                  m_TileMaps[ m_CurrentMap ].Bitmaps()[ (m_TileMaps[m_CurrentMap])[tile.y][tile.x].m_TileGraphic ].Display( m_MapsStaticDCs.m_MemoryDC, tile.x*m_TileMaps[ m_CurrentMap ].tilewidth(), tile.y*m_TileMaps[ m_CurrentMap ].tileheight(), DSTINVERT );
            }
         }
         m_MapsStaticDCs.BltMemoryToGet( m_MapVisableX, m_MapVisableY );
      }
   }
}

bool MainWindow::ValidTileIndex( Structs::Point p )
{
   if( m_TileMaps.size() > m_CurrentMap )
   {
      if( !m_TileMaps[ m_CurrentMap ].height() )
         return false;
      if( (m_TileMaps[ m_CurrentMap ].height() > p.y) && (p.y >= 0) )
      {
         if( ((m_TileMaps[m_CurrentMap])[p.y].size()) > p.x && (p.x >= 0)  )
            return true;
      }
   }
   return false;
}

void MainWindow::DrawMouseRightClickRect( C_HDC& hdc )
{
   Structs::Point MousePosition;
   ::GetCursorPos( &MousePosition );
   ::ScreenToClient( m_MapsTabStatic, &MousePosition );
   // same y
   hdc.Line( Structs::Point( m_RButtonDownStartPoint.x, m_RButtonDownStartPoint.y  ), Structs::Point( MousePosition.x, m_RButtonDownStartPoint.y ) );
   hdc.Line( Structs::Point( m_RButtonDownStartPoint.x, MousePosition.y      ),       Structs::Point( MousePosition.x, MousePosition.y           ) );
   // same x
   hdc.Line( Structs::Point( m_RButtonDownStartPoint.x, MousePosition.y ), Structs::Point( m_RButtonDownStartPoint.x, m_RButtonDownStartPoint.y ) );
   hdc.Line( Structs::Point( MousePosition.x,           MousePosition.y ), Structs::Point( MousePosition.x,           m_RButtonDownStartPoint.y ) );
}

void MainWindow::GetSelectedTiles( void )
{
   Structs::Point MousePosition;
   ::GetCursorPos( &MousePosition );
   ::ScreenToClient( m_MapsTabStatic, &MousePosition );
   if( m_TileMaps.size() )
   {
      Structs::Point starttile = m_TileMaps[ m_CurrentMap ].GetStartDrawTile();
      Structs::Point start( m_RButtonUpStartPoint.x+(starttile.x*m_TileMaps[ m_CurrentMap ].tilewidth()), m_RButtonUpStartPoint.y+(starttile.y*m_TileMaps[ m_CurrentMap ].tileheight()) ),
                       end(   m_RButtonDownStartPoint.x+(starttile.x*m_TileMaps[ m_CurrentMap ].tilewidth()), m_RButtonDownStartPoint.y+(starttile.y*m_TileMaps[ m_CurrentMap ].tileheight()) );
      m_TileMaps[ m_CurrentMap ].GetSelectedTiles( start, end );
   }
}

Structs::Point MainWindow::GetMapTile( Structs::Point MouseDownPosition )
{
   if( m_TileMaps.size() )
   {
      Structs::Point MousePosition = MouseDownPosition;
//      ::GetCursorPos( &MousePosition );
//      ::ScreenToClient( m_MapsTabStatic, &MousePosition );
      Structs::Point point( (int)((MousePosition.x)/m_TileMaps[ m_CurrentMap ].tilewidth()), (int)(MousePosition.y/m_TileMaps[ m_CurrentMap ].tileheight()) );
      Structs::Rect  Rect;
      Rect.right  = m_MapVisableX-(m_MapVisableX%m_TileMaps[ m_CurrentMap ].tilewidth());
      Rect.bottom = m_MapVisableY-(m_MapVisableY%m_TileMaps[ m_CurrentMap ].tileheight());
//      m_MapsStaticDCs.m_GetDC.Clear( Rect, WHITE_BRUSH );
      if( !CheckCollide( Rect, MousePosition ) )
      {
         return Structs::Point( -1, -1 );
      }
      if( m_TileMaps[ m_CurrentMap ].height() )
      {
         if( ( point.x >= m_TileMaps[ m_CurrentMap ].width() ) || ( point.y >= m_TileMaps[ m_CurrentMap ].height() ) )
         {
            return Structs::Point( -1, -1 );
         }
      }
      return point;
   }
   return Structs::Point( -1, -1 );
}


Structs::Point MainWindow::GetMapTile( void )
{
   if( m_TileMaps.size() )
   {
      Structs::Point MousePosition;// = m_MousePosition;
      ::GetCursorPos( &MousePosition );
      ::ScreenToClient( m_MapsTabStatic, &MousePosition );
      Structs::Point point( (int)((MousePosition.x)/m_TileMaps[ m_CurrentMap ].tilewidth()), (int)(MousePosition.y/m_TileMaps[ m_CurrentMap ].tileheight()) );
      Structs::Rect  Rect;
      Rect.right  = m_MapVisableX-(m_MapVisableX%m_TileMaps[ m_CurrentMap ].tilewidth());
      Rect.bottom = m_MapVisableY-(m_MapVisableY%m_TileMaps[ m_CurrentMap ].tileheight());
//      m_MapsStaticDCs.m_GetDC.Clear( Rect, WHITE_BRUSH );
      if( !CheckCollide( Rect, MousePosition ) )
      {
         return Structs::Point( -1, -1 );
      }
      if( m_TileMaps[ m_CurrentMap ].height() )
      {
         if( ( point.x >= m_TileMaps[ m_CurrentMap ].width() ) || ( point.y >= m_TileMaps[ m_CurrentMap ].height() ) )
         {
            return Structs::Point( -1, -1 );
         }
      }
      return point;
   }
   return Structs::Point( -1, -1 );
}

Structs::Point MainWindow::GetMapTileSetTile( void )
{
   if( m_TileMaps.size() )
   {
      if( m_TileMaps[ m_CurrentMap ].Bitmaps().size() )
      {
         Structs::Point MousePosition;
         ::GetCursorPos( &MousePosition );
         ::ScreenToClient( m_TileSetsTabStatic, &MousePosition );

         Structs::Point point( (int)((MousePosition.x)/m_TileMaps[ m_CurrentMap ].tilewidth()), (int)((MousePosition.y)/m_TileMaps[ m_CurrentMap ].tileheight()) );
         return point;
      }
   }
   return Structs::Point( -1, -1 );
}

int MainWindow::GetMapTileSetTileID( Structs::Point tile )
{
   if( tile < 0 )
      return -1;
   if( m_TileMaps.size() )
   {
      if( m_TileMaps[ m_CurrentMap ].Bitmaps().size() )
      {
         if( m_TileSetsIDs.size() )
         {
            if( m_TileSetsIDs[0].size() )
            {
               int index = 0;
               index += (tile.x)+(tile.y*m_TileSetsIDs[0][0].size());
               return index;
            }
            else
            {
               int index = 0;
               return index;
            }
         }
      }
   }
   return -1;
}

// usage: UnPackFiles( "path\\to\\pakfile\\", "path\\to\\destination" )
bool MainWindow::UnPackFiles( std::string path_source_pak, std::string path_destination_dir )
{
   /*WIN32_FIND_DATA FileData;
	HANDLE          hSearch = NULL;
	char buffer[MAX_PATH];
	strncpy( buffer, path_source_pak.c_str(), MAX_PATH );
	lstrcat( buffer, m_PackFileName.c_str() );
   ::ZeroMemory( &FileData, sizeof(WIN32_FIND_DATA) );
   hSearch = ::FindFirstFile( buffer, &FileData );
	if( hSearch == INVALID_HANDLE_VALUE )
   { 
      return false;
   }
   PackageLib::PakFile pack;
   pack.UnPakFile( buffer, path_destination_dir );*/
   return false;
}

// usage: PackFiles( "path\\to\\dir", "path\\to\\destination\\pakfile.pak" )
bool MainWindow::PackFiles( std::string path_source_dir, std::string path_destination_pak )
{
   //PackageLib::PakFile pack;
   //pack.CreatePakFile( path_source_dir, path_destination_pak );
   return false;
}

void MainWindow::HandleCommandLine( LPSTR lpszArgument )
{
/*   std::string commandline = lpszArgument;
   boost::escaped_list_separator<char> els( '\\', ' ', '\"' );
   boost::tokenizer< boost::escaped_list_separator<char> >           args( commandline, els );
   boost::tokenizer< boost::escaped_list_separator<char> >::iterator arg = args.begin();
   for( ;
       arg != args.end();
       arg++ )
   {
     LoadMapAndMakeTab( (*arg) );
   }
*/
}

int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,      int nFunsterStil )
{
   try
   {
      MainWindow App;

      char buffer[MAX_PATH];
      ::getcwd( buffer, MAX_PATH-1 );
      App.m_AppOrignalWorkingDirectory = buffer;

      int x_pos = (int)(::GetSystemMetrics( SM_CXSCREEN )*0.05),
          y_pos = (int)(::GetSystemMetrics( SM_CYSCREEN )*0.05);
      int width  = ::GetSystemMetrics( SM_CXSCREEN )-x_pos*2,
          height = ::GetSystemMetrics( SM_CYSCREEN )-y_pos*2;

      App.m_WinBase.SetMenu( MAKEINTRESOURCE( ID_MAINMENU ) );
//      App.Create( "TileMapEditor", NULL, CW_USEDEFAULT, CW_USEDEFAULT,
//                  640, 480, HMENU Hmenu = NULL );


      if( App.Create( "TileMapEditor", NULL ) )
      {
         App.m_Sprites.push_back( CHBITMAP() );
         App.m_Sprites[0].Load( "dot.bmp" );
         App.m_Sprites[0].SetHwnd( App );

         App.HandleCommandLine( lpszArgument );
         App.Initialise();
         App.UnPackFiles( "", "data" );
         App.LoadConfig();

         App.m_WinBase.Show( SW_MAXIMIZE );
         App.m_WinBase.Update();
         App.m_Haccel = ::LoadAccelerators( App, MAKEINTRESOURCE(ID_ACCELERATORS) );
         int ret = App.RunMessageLoop();
         App.PackFiles( "data", App.m_PackFileName );
         return ret;
      }
   }
   catch( std::exception& e )
   {
      Alert( e.what() );
   }
   catch( ... )
   {
      // error
   }
   return -1;
}

