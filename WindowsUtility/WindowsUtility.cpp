#include "WindowsUtility.h"

std::string currentdir()
{
   char current_dir[ MAX_PATH ];
   ::ZeroMemory( current_dir, MAX_PATH );
   ::_getcwd( (char*)current_dir, MAX_PATH );
   return std::string( current_dir );
}


std::string GetSystemProcessorInfo( void )
{
   SYSTEM_INFO        SystemInfo;
   std::ostringstream temp;
   ::ZeroMemory( &SystemInfo, sizeof(SYSTEM_INFO) );
   ::GetSystemInfo( &SystemInfo );

   std::string ret;
   ret += "Architecture: ";
   switch ( SystemInfo.wProcessorArchitecture )
   {
      case PROCESSOR_ARCHITECTURE_INTEL:
      {
         ret += "Intel, ";
      } break;
      case PROCESSOR_ARCHITECTURE_MIPS:
      {
         ret += "Mips, ";
      } break;
      case PROCESSOR_ARCHITECTURE_ALPHA:
      {
         ret += "Alpha, ";
      } break;
      case PROCESSOR_ARCHITECTURE_PPC:
      {
         ret += "PPC, ";
      } break;
      case PROCESSOR_ARCHITECTURE_UNKNOWN:
      {
         ret += "Unknown, ";
      } break;
      default:
      {
         ret += "Unknown, ";
      }
   }
   temp << SystemInfo.dwNumberOfProcessors;
   ret += "NumberOfProcessors: " + temp.str();

   return ret;
}

std::string GetFileName( HMODULE hModule )
{
   char lpFilename[ MAX_PATH ];
   ::ZeroMemory( lpFilename, MAX_PATH );
   ::GetModuleFileName( hModule, (LPTSTR) lpFilename, MAX_PATH );
   return std::string( lpFilename );
}

std::string GetFileVersionValue( HMODULE hModule, std::string info )
{
   DWORD            zero  = 0;
   DWORD            dwLen = 0;

   std::string filename = GetFileName( hModule );

   if( !filename.size() )
   {
      // handle error
      return std::string( GetLastSystemError() );
   }

   if( !( dwLen = ::GetFileVersionInfoSize( (LPTSTR)filename.c_str(), &zero ) ) )
   {
      // handle error
      return std::string( GetLastSystemError() );
   }

   std::vector<LPVOID> version(dwLen);

   if( ::GetFileVersionInfo( (LPTSTR) filename.c_str(), 0, dwLen, &version[0]  ) )
   {

   }
   else
   {
      // Handle error
      return std::string( GetLastSystemError() );
   }

   LPVOID buffer = 0;
   UINT   Len    = 0;   
   
   std::string SubBlock( "\\StringFileInfo\\040904E4\\" );
   SubBlock += info;

   if( ::VerQueryValue( &version[0], (LPTSTR)SubBlock.c_str(), &buffer, &Len ) )
   {

   }
   else
   {
      // Handle error
      return std::string( GetLastSystemError() );
   }

   return std::string( (char*)buffer );
}

bool ChangeScreenResolution( int width, int height, int bitsPerPixel, DWORD dwflags )
{
   DEVMODE     dmScreenSettings;											
   ::ZeroMemory( &dmScreenSettings, sizeof( DEVMODE ) );
   dmScreenSettings.dmSize       = sizeof( DEVMODE );
   dmScreenSettings.dmPelsWidth  = width;
   dmScreenSettings.dmPelsHeight = height;
   dmScreenSettings.dmBitsPerPel = bitsPerPixel;
   dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
   return ( ::ChangeDisplaySettings ( &dmScreenSettings, dwflags ) != DISP_CHANGE_SUCCESSFUL )
   ? false : true;
}

std::string GetDlgItemText( HWND hwnd, DWORD ID )
{
   std::string text;
   int  nMaxCount = ::GetWindowTextLength( ::GetDlgItem( hwnd, ID ) );
   std::vector<char> lpString( nMaxCount+1 );
   ::GetWindowText( ::GetDlgItem( hwnd, ID ),
                    &lpString[0], nMaxCount+1 );
//   lpString[ nMaxCount+1 ] = '\0';
   text = &lpString[0];
   return text;
}

std::string RectToString( Structs::Rect Rect )
{
   std::ostringstream temp;
   temp << "{ left: ";
   temp << Rect.left;
   temp << ", top: ";
   temp << Rect.top;
   temp << ", right: ";
   temp << Rect.right;
   temp << ", bottom: ";
   temp << Rect.bottom;
   temp << " }";
   return temp.str();
}

std::string RectToString( Structs::fRect Rect )
{
   std::ostringstream temp;
   temp << "{ left: ";
   temp << Rect.left;
   temp << ", top: ";
   temp << Rect.top;
   temp << ", right: ";
   temp << Rect.right;
   temp << ", bottom: ";
   temp << Rect.bottom;
   temp << " }";
   return temp.str();
}

std::string RectToString( Structs::fRect3D Rect )
{
   std::ostringstream temp;
   temp << "{ left: ";
   temp << Rect.left;
   temp << ", top: ";
   temp << Rect.top;
   temp << ", right: ";
   temp << Rect.right;
   temp << ", bottom: ";
   temp << Rect.bottom;
   temp << ", z: ";
   temp << Rect.m_z;
   temp << " }";
   return temp.str();
}

std::string PointToString( Structs::dPoint3D Point )
{
   std::ostringstream temp;
   temp << "{ x: " << Point.m_x;
   temp << ", y: " << Point.m_y;
   temp << ", z: "  << Point.m_y << " }";
   return temp.str();
}

std::string PointToString( Structs::dPoint Point )
{
   std::ostringstream temp;
   temp << "{ x: " << Point.m_x;
   temp << ", y:"  << Point.m_y << " }";
   return temp.str();
}

std::string PointToString( Structs::fPoint Point )
{
   std::ostringstream temp;
   temp << "{ x: " << Point.m_x;
   temp << ", y:"  << Point.m_y << " }";
   return temp.str();
}

std::string PointToString( Structs::Point Point )
{
   std::ostringstream temp;
   temp << "{ x: " << Point.x;
   temp << ", y:" << Point.y << " }";
   return temp.str();
}

int GetWindowWidth( HWND Hwnd )
{
   Structs::Rect Rect;
   GetWindowRect( Hwnd, &Rect );
   return Rect.right - Rect.left;
}

int GetWindowHeight( HWND Hwnd )
{
   Structs::Rect Rect;
   GetWindowRect( Hwnd, &Rect );
   return Rect.bottom - Rect.top;
}

bool OpenModule( HWND hwnd, std::string filename, std::string directory, std::string cmdline, std::string operation, int show )
{
   HINSTANCE hinst = ::ShellExecute( hwnd, operation.c_str(), filename.c_str(),  cmdline.c_str(),
                                     directory.c_str(), show );
   if( ((int)hinst) == ERROR_FILE_NOT_FOUND )
   {
      return false;
   }
   return true;
}

bool SaveBitmap( HDC hDC, HBITMAP hBitmap, std::string szPath )
{
    FILE * fp= NULL;
    fp = fopen( szPath.c_str(), "wb" );
    if( fp == NULL )
        return false;
    
    BITMAP Bm;
    BITMAPINFO BitInfo;
    ::ZeroMemory( &BitInfo, sizeof(BITMAPINFO) );
    BitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BitInfo.bmiHeader.biBitCount = 0;

    if( !::GetDIBits( hDC, hBitmap, 0, 0, NULL, &BitInfo, DIB_RGB_COLORS ) )
        return (false);

    Bm.bmHeight = BitInfo.bmiHeader.biHeight;
    Bm.bmWidth  = BitInfo.bmiHeader.biWidth;

    BITMAPFILEHEADER    BmHdr;
    
    BmHdr.bfType        = 0x4d42;   // 'BM' WINDOWS_BITMAP_SIGNATURE
    BmHdr.bfSize        = (((3 * Bm.bmWidth + 3) & ~3) * Bm.bmHeight) 
                          + sizeof(BITMAPFILEHEADER) 
                          + sizeof(BITMAPINFOHEADER);
    BmHdr.bfReserved1   = BmHdr.bfReserved2 = 0;
    BmHdr.bfOffBits     = (DWORD) sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    BitInfo.bmiHeader.biCompression = 0;
    // Writing Bitmap File Header ////
    fwrite( &BmHdr, sizeof(BITMAPFILEHEADER), 1, fp );

    fwrite( &BitInfo.bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp );

    BYTE *pData = new BYTE[ BitInfo.bmiHeader.biSizeImage + 5 ];
    if( !::GetDIBits( hDC, hBitmap, 0, Bm.bmHeight, pData, &BitInfo, DIB_RGB_COLORS ) )
        return (false);
    if( pData != NULL )
        fwrite( pData, 1, BitInfo.bmiHeader.biSizeImage, fp );

    fclose( fp );
    delete ( pData );
    return (true);
}

/*std::string GetLastSystemError( void )
{
   std::string MsgBuf;
   LPVOID lpMsgBuf = NULL;
   ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL, GetLastError(),
                    MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
                    (LPTSTR) &lpMsgBuf, 0,
                    NULL );
   MsgBuf += (LPTSTR)lpMsgBuf;
   return MsgBuf;
}
*/

std::string CopyTextToClipBoard( std::string text, HWND Hwnd )
{
   LPTSTR  lptstrCopy = NULL;
   HGLOBAL hglbCopy   = NULL;
   int     length     = (text.size()+1) * sizeof(TCHAR);

   try
   {
      if( !::OpenClipboard( Hwnd ) )
         throw std::string( "Could Not Open ClipBoard " );

      if( !::EmptyClipboard() )
         throw std::string( "Could Not Empty Clip Board " );

      hglbCopy = ::GlobalAlloc( GMEM_MOVEABLE, length );
      if( hglbCopy == NULL )
         throw std::string( "Not Enough Memory to Copy String to ClipBoard " );
       
      if( ( lptstrCopy = (TCHAR*)::GlobalLock( (HGLOBAL)hglbCopy ) ) == NULL )
         throw std::string( "Unable to LockMemory" );

      memcpy( lptstrCopy, text.c_str(), length-1 );
      lptstrCopy[length] = 0;

      ::GlobalUnlock( (HGLOBAL)hglbCopy );
//      if( !::GlobalUnlock( (HGLOBAL)hglbCopy ) )
//         throw std::string( "Unable to UnLockMemory " );

      if( ::SetClipboardData( CF_TEXT, (HANDLE)hglbCopy ) == NULL )
         throw std::string( "Unable To Set ClipBoard Data " );

      if( !::CloseClipboard() )
         throw std::string( "Could Not Close ClipBoard " );

      return std::string();
   }
   catch ( std::string error_msg )
   {
      std::string return_value = std::string( "Error : " ) + error_msg + GetLastSystemError();
      ::CloseClipboard();
      return return_value;
   }
}

std::string GetTextFromClipBoard( HWND Hwnd )
{
   std::string text;
   LPTSTR  lptstrCopy = NULL;
   HGLOBAL hglbCopy   = NULL;

   try
   {
      if( !::IsClipboardFormatAvailable( CF_TEXT ) )
         throw 0;

      if( !::OpenClipboard( Hwnd ) )
         throw std::string( "Could Not Open ClipBoard " );

      if( (hglbCopy = ::GetClipboardData( CF_TEXT )) == NULL )
         throw std::string( "Unable To Get ClipBoard Data " );

      if( (lptstrCopy = (LPTSTR)::GlobalLock(hglbCopy)) == NULL )
         throw std::string( "Clould Not Lock ClipBoard Data " );

      text = lptstrCopy;

      ::GlobalUnlock( (HGLOBAL)hglbCopy );
//      if( !::GlobalUnlock( (HGLOBAL)hglbCopy ) )
//         throw std::string( "Unable to UnLockMemory " );

      if( !::CloseClipboard() )
         throw std::string( "Could Not Close ClipBoard " );
   }
   catch ( std::string error_msg )
   {
      std::string return_value = std::string( "Error : " ) + error_msg + GetLastSystemError();
      ::CloseClipboard();
      return return_value;
   }
   catch( int i )
   {
	  i = i+1;
      MsgBeep( 0xFFFFFFFF );
   }
   return text;
}

std::string GetWindowText( HWND Hwnd )
{
   std::string title;
   int len = GetWindowTextLength( Hwnd );
   if( !len )
   {
      return title;
   }
   len += 1;
   std::vector<char> str(len+1);
   if( !GetWindowText( Hwnd, (LPTSTR)&str[0], len ) )
   {
      return title;
   }
   str[len] = '\0';
   title = &str[0];
   return title;
}

//#define VOL_NAME "GAMEVOL"

bool GetGameDisk( std::string volume_label )
{
   char dr_str[]="c:\\"; // drive path
	int i;                // general index counter
	DWORD drives;         // drive bit map
	BOOL status;          // return status from volume function
	char vol_buf[40];     // volume name buffer
	DWORD max_file_len;   // maximum file length
	DWORD system_flags;   // volume file system flags
	int stat;             // message box return value
   // find what drives are available
   lp: drives=GetLogicalDrives();

	// loop through drives
	for( i=0; i<26 ; i++ )
   {
      // is this drive available?
		if( drives&( 1<<i ) )
      {
         // yes, is it a CD?
         dr_str[0]='A'+i;
         UINT driveType=GetDriveType(dr_str);
         if( driveType==DRIVE_CDROM )
         {
            // yes, get the volume name
            ::SetErrorMode( SEM_FAILCRITICALERRORS );
            status = ::GetVolumeInformation( dr_str, vol_buf, sizeof(vol_buf), 
                                           NULL, &max_file_len,
                                           &system_flags, NULL, 0 );
            SetErrorMode( 0 );

            // did we successfully get it?
            if( status )
            {
               // yes, is this the one?
               if( !memicmp( vol_buf, volume_label.c_str(), volume_label.size() ) )
               {
                  // yes, return success to caller
                  return TRUE;
               }
            }
         }
      }
   }

   // not found, display error message and query user
   stat = MessageBox( NULL, "Please Insert Game CD", "CD Not Found",
                      MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION );

   // does the user want to retry?
	if( stat==IDRETRY )
		// yes, try again
		goto lp;

   // return failure to caller
   return false;
}

