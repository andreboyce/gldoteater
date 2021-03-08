#include "CHBITMAP.h"

CHBITMAP::CHBITMAP( void ) :
   m_Hbitmap( NULL ),
   m_MemoryDC( NULL),
   m_HbitmapOrignal( NULL ),
   m_bSelected( false )
//   m_Hwnd(NULL)
{
//   ::ZeroMemory( &m_Binfo, sizeof(BITMAPINFO) );
   ::ZeroMemory( &m_Bitmap, sizeof(BITMAP) );
}

CHBITMAP::~CHBITMAP( void )
{
   Destroy();
}

bool CHBITMAP::Save( std::string szPath )
{
   bool ret = false;
   if( m_MemoryDC != NULL )
   {
      ret = ::SaveBitmap( m_MemoryDC, m_Hbitmap, szPath );
   }
   return ret;
}

bool CHBITMAP::Load( std::string path )
{
   Destroy();
   if( path.find(".bmp") )
   {
      m_Hbitmap = (HBITMAP)::LoadImage( NULL, path.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );
   }
   return ValidBitmapHandle();
}

bool CHBITMAP::LoadBitmapFromResource( DWORD resource_id )
{
   Destroy();
   m_Hbitmap = (HBITMAP)::LoadImage( ::GetModuleHandle( NULL ), MAKEINTRESOURCE( resource_id ), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );   
   return ValidBitmapHandle();
}

bool CHBITMAP::ValidBitmapHandle( void )
{
   if( m_Hbitmap )
   {
      ::GetObject( m_Hbitmap, sizeof( BITMAP ), &m_Bitmap );
      return true;
   }
   return false;
}

void CHBITMAP::Display( HDC Hdc, int x, int y, DWORD dwRop )
{
   ::BitBlt( Hdc, x, y, Width(), Height(), m_MemoryDC, 0, 0, dwRop );
}

void CHBITMAP::StretchDisplay( HDC Hdc, int x, int y, int width_of_dest, int height_of_dest, DWORD dwRop )
{
   ::StretchBlt( Hdc, x, y, width_of_dest, height_of_dest, m_MemoryDC, 0, 0, Width(), Height(), dwRop );
}

bool CHBITMAP::SelectHbitmapIntoMemoryDC( void )
{
   if( !m_bSelected )
   {
      m_bSelected = true;
      m_HbitmapOrignal = (HBITMAP)::SelectObject( m_MemoryDC, m_Hbitmap );
      return true;
   }
   return false;
}

void CHBITMAP::Destroy( void )
{
   if( m_Hbitmap )
   {
      ::SelectObject( m_MemoryDC, m_HbitmapOrignal );
      ::DeleteDC( m_MemoryDC );
      ::DeleteObject( m_Hbitmap );
      m_bSelected = false;
      m_Hbitmap = NULL;
      m_MemoryDC = NULL;
   }
//   ::ZeroMemory( &m_Binfo, sizeof(BITMAPINFO) );
   ::ZeroMemory( &m_Bitmap, sizeof(BITMAP) );
}

void CHBITMAP::Copy( const CHBITMAP& rhs )
{
   Destroy();
   m_MemoryDC       = ::CreateCompatibleDC( rhs.m_MemoryDC );
   m_Hbitmap        = ::CreateCompatibleBitmap( rhs.m_MemoryDC, rhs.Width(), rhs.Height() );
   m_HbitmapOrignal = (HBITMAP) ::SelectObject( m_MemoryDC, m_Hbitmap );
   ::GetObject( m_Hbitmap, sizeof( BITMAP ), &m_Bitmap );
   m_bSelected      = true;
}

bool CHBITMAP::GetRect( const CHBITMAP &bitmap, Structs::Rect rect, HWND Hwnd )
{
   Destroy();
   BITMAP bm;
   HDC hdc    = ::GetDC( Hwnd );
   m_MemoryDC = ::CreateCompatibleDC( hdc );
   m_Hbitmap  = ::CreateCompatibleBitmap( hdc, rect.right-rect.left, rect.bottom-rect.top );
   m_HbitmapOrignal = (HBITMAP) ::SelectObject( m_MemoryDC, m_Hbitmap );
   ::GetObject( m_Hbitmap, sizeof(bm), &bm );

   ::BitBlt( m_MemoryDC,           // handle to destination device context 
             0,                      // x-coordinate of destination rectangle’s upper-left corner
             0,                      // y-coordinate of destination rectangle’s upper-left corner
             rect.right-rect.left,   // width of destination rectangle 
             rect.bottom-rect.top,   // height of destination rectangle 
             bitmap.m_MemoryDC,      // handle to source device context 
             rect.left,              // x-coordinate of source rectangle’s upper-left corner  
             rect.top,               // y-coordinate of source rectangle’s upper-left corner
             SRCCOPY                 // raster operation code 
           );
   ::ReleaseDC( Hwnd, hdc );
   return true;
}

bool CHBITMAP::SetSize( unsigned short width, unsigned short height )
{
   bool ret = false;
   /*
   HBITMAP temp = ::CreateCompatibleBitmap( m_MemoryDC, width, height );  // might need to get the dc of a window to preserve color
   temp;
   ::BitBlt( m_MemoryDC,             // handle to destination device context 
             0,                      // x-coordinate of destination rectangle’s upper-left corner
             0,                      // y-coordinate of destination rectangle’s upper-left corner
             rect.right-rect.left,   // width of destination rectangle 
             rect.bottom-rect.top,   // height of destination rectangle 
             bitmap.m_MemoryDC,      // handle to source device context 
             rect.left,              // x-coordinate of source rectangle’s upper-left corner  
             rect.top,               // y-coordinate of source rectangle’s upper-left corner
             SRCCOPY                 // raster operation code 
           );
   m_Hbitmap = (HBITMAP) ::SelectObject( m_MemoryDC, temp );
   ::DeleteObject( m_Hbitmap );
   m_Hbitmap = temp;
   */
   return ret;
}
