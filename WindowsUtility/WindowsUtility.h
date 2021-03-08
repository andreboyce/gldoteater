#pragma once

#ifndef __WindowsUtility_H
#define __WindowsUtility_H

#pragma comment( lib, "version.lib" )

#include <windows.h>
#include <string>
#include <sstream>
//#include <dirent.h>
#include <direct.h>
#include "../errorutil/errorutil.h"
#include "../Structs/Structs.h"

bool        OpenModule( HWND hwnd, std::string filename, std::string directory = "", std::string cmdline = "", std::string operation = "open", int show = SW_SHOWNORMAL );
int         GetWindowHeight( HWND Hwnd );
int         GetWindowWidth( HWND Hwnd );

std::string PointToString( Structs::dPoint3D Point );
std::string PointToString( Structs::dPoint Point );
std::string PointToString( Structs::fPoint Point );
std::string PointToString( Structs::Point Point );
std::string RectToString( Structs::Rect Rect );
std::string RectToString( Structs::fRect Rect );
std::string RectToString( Structs::fRect3D Point );
std::string GetFileName( HMODULE hModule );
std::string GetFileVersionValue( HMODULE hModule, std::string info );
std::string GetSystemProcessorInfo( void );
std::string GetDlgItemText( HWND hwnd, DWORD ID );
bool        ChangeScreenResolution( int width, int height, int bitsPerPixel, DWORD dwflags );
bool        SaveBitmap( HDC hDC, HBITMAP hBitmap, std::string szPath );

std::string GetTextFromClipBoard( HWND Hwnd );
std::string CopyTextToClipBoard( std::string text, HWND Hwnd );

std::string GetWindowText( HWND Hwnd );

bool GetGameDisk( std::string volume_label );
std::string currentdir();

#endif
