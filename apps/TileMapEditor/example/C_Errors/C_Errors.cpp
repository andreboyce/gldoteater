#include "C_Errors.h"

int MsgBeep( DWORD uType )
{
   return ::MessageBeep( uType );
}

// Calls A MessageBox with title Alert and a OK button
int Alert( HWND hwnd )
{
   return ::MessageBox( hwnd, NULL, "Alert", MB_OK );
}

int AlertC( char i, HWND hwnd )
{
   char str[2];
   str[0] = i;
   str[1] = '\0';
   return ::MessageBox( hwnd, str, "Alert", MB_OK );
}

/*
int Alert( long i, HWND hwnd )
{
   char str[256];
   ::ZeroMemory( str, sizeof( str ) );
   ::ltoa( i, str, 10);
   return ::MessageBox( hwnd, str, "Alert", MB_OK );
}
*/

int Alert( double i, HWND hwnd )
{
   std::ostringstream osstr;
   osstr << i;
   std::string t = osstr.str();
   return ::MessageBox( hwnd, (char*)t.c_str(), "Alert", MB_OK );
}

int Alert( const std::string & str, HWND hwnd )
{
   return ::MessageBox( hwnd, str.c_str(), "Alert", MB_OK );
}

int Alert( const std::wstring & str, HWND hwnd )
{
   return ::MessageBox( hwnd, std::string( "" ).c_str(), "Alert", MB_OK );
//   return ::MessageBox( hwnd, str.c_str(), "Alert", MB_OK );
}

int Confirm( const std::string &str, HWND hwnd )
{
   return ::MessageBox( hwnd, str.c_str(), "Confirm", MB_YESNO );
}

int Confirm( const char *str, HWND hwnd )
{
   return ::MessageBox( hwnd, str, "Confirm", MB_YESNO );
}

int Retry( const std::string &str, HWND hwnd )
{
   return ::MessageBox( hwnd, str.c_str(), "Retry", MB_RETRYCANCEL );
}

int Retry( const char *str, HWND hwnd )
{
   return ::MessageBox( hwnd, str, "Retry", MB_RETRYCANCEL );
}
