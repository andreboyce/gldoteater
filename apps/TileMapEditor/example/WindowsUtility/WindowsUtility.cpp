#include "WindowsUtility.h"

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
   C_Errors ErrorHandlers;

   std::string filename = GetFileName( hModule );

   if( !filename.size() )
   {
      // handle error
      return std::string( ErrorHandlers.GetLastSystemError() );
   }

   if( !( dwLen = ::GetFileVersionInfoSize( (LPTSTR)filename.c_str(), &zero ) ) )
   {
      // handle error
      return std::string( ErrorHandlers.GetLastSystemError() );
   }

   LPVOID version[ dwLen ];

   if( ::GetFileVersionInfo( (LPTSTR) filename.c_str(), 0, dwLen, version  ) )
   {

   }
   else
   {
      // Handle error
      return std::string( ErrorHandlers.GetLastSystemError() );
   }

   LPVOID buffer = 0;
   UINT   Len    = 0;   
   
   std::string SubBlock( "\\StringFileInfo\\040904E4\\" );
   SubBlock += info;

   if( ::VerQueryValue( version, (LPTSTR)SubBlock.c_str(), &buffer, &Len ) )
   {

   }
   else
   {
      // Handle error
      return std::string( ErrorHandlers.GetLastSystemError() );
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
   char lpString[ nMaxCount+1 ];
   ::GetWindowText( ::GetDlgItem( hwnd, ID ),
                    lpString, nMaxCount+1 );
//   lpString[ nMaxCount+1 ] = '\0';
   text = lpString;
   return text;
}

std::string RectToString( C_Structs::Rect Rect )
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

std::string RectToString( C_Structs::fRect Rect )
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

std::string PointToString( C_Structs::Point Point )
{
   std::ostringstream temp;
   temp << "{ x: " << Point.x;
   temp << ", y:" << Point.y << " }";
   return temp.str();
}

int GetWindowWidth( HWND Hwnd )
{
   C_Structs::Rect Rect;
   GetWindowRect( Hwnd, &Rect );
   return Rect.right - Rect.left;
}

int GetWindowHeight( HWND Hwnd )
{
   C_Structs::Rect Rect;
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


