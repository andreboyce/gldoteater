#include "errorutil.h"

int MsgBeep( DWORD uType )
{
   return ::MessageBeep( uType );
}

// Calls A MessageBox with title Alert and a OK button
int Alert( HWND hwnd )
{
   return ::MessageBox( hwnd, NULL, (LPCTSTR)"Alert", MB_OK );
}

int AlertC( char i, HWND hwnd )
{
   char str[2];
   str[0] = i;
   str[1] = '\0';
   return ::MessageBox( hwnd, (LPCTSTR)str, (LPCTSTR)"Alert", MB_OK );
}

int Alert( double i, HWND hwnd )
{
   std::ostringstream osstr;
   osstr << i;
   std::string t = osstr.str();
   return ::MessageBox( hwnd, (LPCTSTR)t.c_str(), (LPCTSTR)"Alert", MB_OK );
}

int Alert( std::string str, HWND hwnd )
{
   return ::MessageBox( hwnd, (LPCTSTR)str.c_str(), (LPCTSTR)"Alert", MB_OK );
}

int Alert( std::wstring str, HWND hwnd )
{
   return ::MessageBox( hwnd, (LPCTSTR)(std::string( "" ).c_str()), (LPCTSTR)"Alert", MB_OK );
//   return ::MessageBox( hwnd, str.c_str(), "Alert", MB_OK );
}

int Confirm( std::string str, HWND hwnd )
{
   return ::MessageBox( hwnd, (LPCTSTR)str.c_str(), (LPCTSTR)"Confirm", MB_YESNO );
}

int Confirm( const char *str, HWND hwnd )
{
   return ::MessageBox( hwnd, (LPCTSTR)str, (LPCTSTR)"Confirm", MB_YESNO );
}

int Retry( std::string str, HWND hwnd )
{
   return ::MessageBox( hwnd, (LPCTSTR)str.c_str(), (LPCTSTR)"Retry", MB_RETRYCANCEL );
}

int Retry( const char *str, HWND hwnd )
{
   return ::MessageBox( hwnd, (LPCTSTR)str, (LPCTSTR)"Retry", MB_RETRYCANCEL );
}

// Gets the last error reported by windows no unicode yet
void GetLastSystemError( std::string MsgBuf )
{
   LPVOID lpMsgBuf = NULL;
   ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL, GetLastError(),
                    MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
                    (LPTSTR) &lpMsgBuf, 0,
                    NULL );
    //MsgBuf += (LPTSTR)lpMsgBuf;
   MsgBuf += (char*)lpMsgBuf;
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
         //MsgBuf += (LPTSTR)lpMsgBuf;
         MsgBuf += (char*)lpMsgBuf;
         return MsgBuf;
}

static std::string LastSystemError( void )
{
   std::string MsgBuf;
   LPVOID lpMsgBuf = NULL;
   ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL, GetLastError(),
                    MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
                    (LPTSTR) &lpMsgBuf, 0,
                    NULL );
   //MsgBuf += (LPTSTR)lpMsgBuf;
   MsgBuf += (char*)lpMsgBuf;
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
   

// Append the error to the file error.log
bool LogError( std::string error_string )
{
   std::ofstream errorlog( "error.log", std::ios::app );
   if( !errorlog )
   {
      Alert( "Could not open file 'error.log' for appending." );
      return false;
   }
   short bufferlen = ::GetTimeFormat( (LCID)NULL, 0, NULL, NULL, NULL, 0 );
   std::vector<TCHAR> time( bufferlen );
   ::GetTimeFormat( (LCID)NULL, 0, NULL, NULL, (TCHAR*)&time[0], bufferlen );
   errorlog << (TCHAR*)&time[0] << " | " << error_string << " | " << "";//m_LastErrorString;
//   if( !m_LastErrorString.size() )
//   {
      errorlog << std::endl;
//   }
   errorlog.close();
   return true;
}

      // Append the error to the file error.log
bool LogLastSystemError( std::string additional_error_string )
{
   std::ofstream errorlog( "error.log", std::ios::app );
   if( !errorlog )
   {
      Alert( "Could not open file 'error.log' for appending." );
      return false;
   }
   short bufferlen = ::GetTimeFormat( (LCID)NULL, 0, NULL, NULL, NULL, 0 );
   std::vector<TCHAR> time(bufferlen);
   ::GetTimeFormat( (LCID)NULL, 0, NULL, NULL, &time[0], bufferlen );
   errorlog << &time[0] << " | " << GetLastSystemError();
   if( !additional_error_string.size() )
   {
      errorlog << std::endl;
   }
   else
   {
      errorlog << " | " << additional_error_string << std::endl;
   }
   errorlog.close();
   return true;
}

      // Append the error to the file error.log
bool LogMsg( std::string file_name, std::string msg )
{
   std::ofstream log( file_name.c_str(), std::ios::app );
   if( !log.is_open() )
   {
      Alert( "Could not open file" + file_name + " for appending." );
      return false;
   }
   short bufferlen = ::GetTimeFormat( (LCID)NULL, 0, NULL, NULL, NULL, 0 );
   std::vector<TCHAR> time(bufferlen);
   ::GetTimeFormat( (LCID)NULL, 0, NULL, NULL, &time[0], bufferlen );
   log << &time[0];
   if( msg.length() )
   {
      log << " | " << msg << std::endl;
   }
   else
   {
      log << std::endl;
   }
   log.close();
   return true;
}
