#pragma once

#ifndef __errorutil_H
#define __errorutil_H

#include <windows.h>
#include <map>
#include <string>
#include <sstream>
#include <fstream>

#pragma comment(lib, "user32.lib")

#include "../CommonTypes.h"
//#include "ErrorIDs.h"

#define MsgBox( str, title, flags ) ::MessageBox( HWND_DESKTOP, str, title, flags )
//#define MsgBeep( uType) MessageBeep( uType )
#define DEFAULT_ERROR_MESSAGE "Unknown Error"

int MsgBeep( DWORD uType = MB_OK );

// Calls A MessageBox with title Alert and a OK button
int Alert(  HWND = HWND_DESKTOP );

// Calls A MessageBox with title Alert and a OK button
int AlertC( char, HWND = HWND_DESKTOP );

// Calls A MessageBox with title Alert and a OK button
//int Alert( long, HWND = HWND_DESKTOP );

// Calls A MessageBox with title Alert and a OK button
int Alert( std::string, HWND = HWND_DESKTOP );

// Calls A MessageBox with title Alert and a OK button
int Alert( std::wstring, HWND = HWND_DESKTOP );

// Calls A MessageBox with title Alert and a OK button
int Alert( double, HWND = HWND_DESKTOP );

// Calls A MessageBox with title “Confirm” and a OK and Cancel button
int Confirm( std::string, HWND = HWND_DESKTOP );

// Calls A MessageBox with title “Confirm” and a OK and Cancel button
int Confirm( const char *, HWND = HWND_DESKTOP );

// Calls A MessageBox with title “Retry” and a RETRY and Cancel button
int Retry( std::string, HWND = HWND_DESKTOP );

// Calls A MessageBox with title “Retry” and a RETRY and Cancel button
int Retry( const char *, HWND = HWND_DESKTOP );

// typedefs   
typedef BOOL (*ErrorHandler)( long );
typedef std::map< long, ErrorHandler > ErrorHandlerMap;
typedef ErrorHandlerMap::iterator ErrorHandlerMapIterator;
typedef std::map< long, std::string > ErrorStringMap;
typedef ErrorStringMap::iterator ErrorStringMapIterator;

// Gets the last error reported by windows no unicode yet
void GetLastSystemError( std::string MsgBuf );

// Gets the last error reported by windows no unicode yet
std::string GetLastSystemError( void );

static std::string LastSystemError( void );

// Gets the last error reported by windows no unicode yet
void GetLastSystemError( char * lpMsgBuf );

// Append the error to the file error.log
bool LogError( std::string error_string = "" );

      // Append the error to the file error.log
bool LogLastSystemError( std::string additional_error_string = "" );
      // Append the error to the file error.log
bool LogMsg( std::string file_name, std::string msg = "" );

#endif
