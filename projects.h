#pragma once
#ifndef __Projects_H
#define __Projects_H

#pragma comment( lib, "version.lib" )
#pragma comment( lib, "opengl32.lib" )
//#pragma comment( lib, "fmodex_vc.lib" )
//#pragma comment( lib, "lua5.1.lib" )
#pragma comment( lib, "Glu32.lib" )
#pragma comment( lib, "Ws2_32.lib" )
#pragma comment( lib, "Wininet.lib" )



#define _USE_MATH_DEFINES
#include <cmath>

#include "WinSockLib/WinSockLib.h"

#include "commontypes.h"
#include "errorutil/errorutil.h"
#include "Tile/Tile.h"
#include "TileSet/TileSet.h"

#include "RandomNum/RandomNum.h"
#include "apps/SpriteAnimationEditor/TextureList.h"
#include "apps/SpriteAnimationEditor/SpriteAnimation.h"
#include "TileMap/TileMap.h"
#include "LimitFPS/LimitFPS.h"
#include "MathUtility/MathUtility.h"
#include "WinFileHandler/WinFileHandler.h"
#include "Taunts/Taunts.h"

#include "WinThread/WinThread.h"
#include "WinThread/CMutex/CMutex.h"
#include "MathVector/Vector2D/Vector2D.h"
#include "MathVector/Vector3D/Vector3D.h"
#include "WindowsUtility/WindowsUtility.h"
#include "CHBITMAP/CHBITMAP.h"
#include "Tile/Tile.h"

//AStar
#include "Astar/AStar2d.h"

// Lua
//#include "Lua/CLuaState/CLuaState.h"
//#include "Lua/CLuaToCFunction/CLuaToCFunction.h"

// winmain
#include "CWinMain/CWinMain.h"

//#include "libVFS/libVFS.h"
#include "C_WinControllBase/C_WinToolTip/C_WinToolTip.h"
#include "C_WinControllBase/C_WinStatic/C_WinStatic.h"
#include "CXMLMenu/CXMLMenu.h"
#include "CTrackPopupMenu/CTrackPopupMenu.h"
#include "C_WinControllBase/C_WinTab/C_WinTab.h"
#include "C_WinControllBase/C_WinScrollBar/C_WinScrollBar.h"
#include "C_WinControllBase/C_WinStatusBar/C_WinStatusBar.h"
#include "C_WinControllBase/C_WinEdit/C_WinEdit.h"
#include "C_WinControllBase/C_WinUpDown/C_WinUpDown.h"
#include "CWinFileHandler/CWinFileHandler.h"

//#include "C_HCursor/C_HCursor.h"


// tinyxml
#include "tinyxml/tinyxml.h"

#include "TileMap/TileMap.h"       // Todo 
#include "TileMapLib/TileMapLib.h" // Merge these 2 classes

#include "WinTimers/WinTimers.h"//
#include "C_HDC/HDCs.h"

#include "CFMOD/CFMOD.h"
#include "FrameRate/FrameRate.h"
#include "TimeInterval/TimeInterval.h"
#include "CGLRC/CGLRC.h"
#include "Console/Console.h"
#include "OpenGLLib/OpenGLLib.h"
#include "Options/Options.h"

#include "WinParseCommandLine/WinParseCommandLine.h"
#include "ParticleSystems/ParticleSystems.h"
#include "KeyInput/KeyInput.h"

#include "C_Sprite/C_Sprite.h"

// Need to work on
//#include "enet/enet.h"

// Not necessary yet
//#include "C_Object/C_Object.h"
#include "CHPen/CHPen.h"
#include "SideScrollingText/SideScrollingText.h"
//#include "GameStateLib/GameStateLib.h"

// For removal
//#include "C_DebugThread/C_DebugThread.h"
//#include "PackageLib/PackageLib.h"

#endif