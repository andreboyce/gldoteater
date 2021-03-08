#ifndef __C_Errors_H
#define __C_Errors_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <map>
#include <string>
#include <sstream>
#include <fstream>

#include "../C_AllTypes.h"
#include "ErrorIDs.h"

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
int Alert( const std::string &, HWND = HWND_DESKTOP );

// Calls A MessageBox with title Alert and a OK button
int Alert( const std::wstring &, HWND = HWND_DESKTOP );

// Calls A MessageBox with title Alert and a OK button
int Alert( double, HWND = HWND_DESKTOP );

// Calls A MessageBox with title “Confirm” and a OK and Cancel button
int Confirm( const std::string &, HWND = HWND_DESKTOP );

// Calls A MessageBox with title “Confirm” and a OK and Cancel button
int Confirm( const char *, HWND = HWND_DESKTOP );

// Calls A MessageBox with title “Retry” and a RETRY and Cancel button
int Retry( const std::string &, HWND = HWND_DESKTOP );

// Calls A MessageBox with title “Retry” and a RETRY and Cancel button
int Retry( const char *, HWND = HWND_DESKTOP );

// This should not even be a class
/* TODO (#1#): Fix this mess
 */
class C_Errors
{
   public:
   
      C_Errors::C_Errors() :
         m_LastErrorID( 0 )
      {
         AddErrorString( -1, DEFAULT_ERROR_MESSAGE );
      }
      ~C_Errors( void ){}
   
      // typedefs   
      typedef BOOL (*ErrorHandler)( long );
      typedef std::map< long, ErrorHandler > ErrorHandlerMap;
      typedef ErrorHandlerMap::iterator ErrorHandlerMapIterator;
      typedef std::map< long, std::string > ErrorStringMap;
      typedef ErrorStringMap::iterator ErrorStringMapIterator;

      // Gets the last error reported by windows no unicode yet
      void GetLastSystemError( std::string& MsgBuf )
      {
         LPVOID lpMsgBuf = NULL;
         ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                          NULL, GetLastError(),
                          MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
                          (LPTSTR) &lpMsgBuf, 0,
                          NULL );
         MsgBuf += (LPTSTR)lpMsgBuf;
         m_LastErrorString = (LPTSTR)lpMsgBuf;
      }

      // Gets the last error reported by windows no unicode yet
      std::string GetLastSystemError( void )
      {
         std::string MsgBuf;
         LPVOID lpMsgBuf = NULL;
         ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                          NULL, GetLastError(),
                          MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
                          (LPTSTR) &lpMsgBuf, 0,
                          NULL );
         MsgBuf += (LPTSTR)lpMsgBuf;
         m_LastErrorString = (LPTSTR)lpMsgBuf;
         return MsgBuf;
      }

      // Gets the last error reported by windows no unicode yet
      void GetLastSystemError( char * lpMsgBuf )
      {
         lpMsgBuf = NULL;
         ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                          NULL, GetLastError(),
                          MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
                          (LPTSTR) &lpMsgBuf, 0,
                          NULL );
      }      
   
      // Depreciated   
      bool AlertError( char ErrorDescription[], HWND hwnd = HWND_DESKTOP )
      {
         m_LastErrorString = ErrorDescription;
         return ::MessageBox( hwnd, m_LastErrorString.c_str(), 
                              "An Error Occured", MB_OK );
      }

      // Depreciated   
      bool AlertError( long Error_ID, HWND hwnd = HWND_DESKTOP )
      {
         m_LastErrorString = GetErrorString( Error_ID );
         return ::MessageBox( hwnd, m_LastErrorString.c_str(), 
                              "An Error Occured", MB_OK );
      }   

      // Depreciated 
      bool AbortRetryIgnore( char ErrorDescription[], HWND hwnd = HWND_DESKTOP )
      {
         m_LastErrorString = ErrorDescription;
         return ::MessageBox( hwnd, m_LastErrorString.c_str(), "An Error Occured", 
                              MB_ABORTRETRYIGNORE );   
      }

      // Depreciated      
      bool AddErrorString( long Error_ID, char Error_Descrption[] )
      {
         m_ErrorString_ID_Map[ Error_ID ] = Error_Descrption;
         return ( (GetErrorString( Error_ID )) == DEFAULT_ERROR_MESSAGE ) ? true: false;
      }   

      // Depreciated
      std::string GetErrorString( long Error_ID )
      {
      
         ErrorStringMapIterator Error_StringIterator = m_ErrorString_ID_Map.find( Error_ID );

         // If the handler is not found return the predefined error string
         return ( Error_StringIterator != m_ErrorString_ID_Map.end() )
                ?  Error_StringIterator->second : (m_ErrorString_ID_Map.find( -1 ))->second;
      }            

      // Depreciated
      operator bool() const
      {
         return ( m_LastErrorID != 0 ) ? true: false;
      }

      // Append the error to the file error.log
      // Depreciated
      bool LogError( long error_ID )
      {
         return LogError( GetErrorString( error_ID ) );
      }

      // Append the error to the file error.log
      bool LogError( std::string error_string = "" )
      {
         std::ofstream errorlog( "error.log", std::ios::app );
         if( !errorlog )
         {
            Alert( "Could not open file 'error.log' for appending." );
            return false;
         }
         short bufferlen;
         char time[ ( bufferlen = ::GetTimeFormat( (LCID)NULL, 0, NULL, NULL, NULL, 0 ), ' ' ) ];
         ::GetTimeFormat( (LCID)NULL, 0, NULL, NULL, time, bufferlen );
         errorlog << time << " | " << error_string << " | " << m_LastErrorString;
         if( !m_LastErrorString.size() )
         {
            errorlog << std::endl;
         }
         errorlog.close();
         return true;
      }

   protected:

      ErrorStringMap  m_ErrorString_ID_Map;
      std::string     m_LastErrorString;
      long            m_LastErrorID;
};

#endif
