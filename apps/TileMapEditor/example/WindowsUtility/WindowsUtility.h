#ifndef __WindowsUtility_H
#define __WindowsUtility_H

#include <windows.h>
#include <string>
#include <sstream>
#include "../C_Errors/C_Errors.h"
#include "../C_Structs/C_Structs.h"

bool        OpenModule( HWND hwnd, std::string filename, std::string directory = "", std::string cmdline = "", std::string operation = "open", int show = SW_SHOWNORMAL );
int         GetWindowHeight( HWND Hwnd );
int         GetWindowWidth( HWND Hwnd );
std::string PointToString( C_Structs::Point Point );
std::string RectToString( C_Structs::Rect Rect );
std::string RectToString( C_Structs::fRect Rect );
std::string GetFileName( HMODULE hModule );
std::string GetFileVersionValue( HMODULE hModule, std::string info );
std::string GetSystemProcessorInfo( void );
std::string GetDlgItemText( HWND hwnd, DWORD ID );
bool        ChangeScreenResolution( int width, int height, int bitsPerPixel, DWORD dwflags );
bool        SaveBitmap( HDC hDC, HBITMAP hBitmap, std::string szPath );

#endif
