#include "C_HBITMAP.h"

C_HBITMAP::C_HBITMAP( void ) :
   m_Hbitmap( NULL ),
   m_MemoryDC( NULL),
   m_HbitmapOrignal( NULL ),
   m_bSelected( false )
{
   ::ZeroMemory( &m_Binfo, sizeof(BITMAPINFO) );
   ::ZeroMemory( &m_Bitmap, sizeof(BITMAP) );
}

C_HBITMAP::~C_HBITMAP( void )
{
   Destroy();
}

bool C_HBITMAP::Save( std::string szPath )
{
   bool ret = false;
   ret = ::SaveBitmap( m_MemoryDC, m_Hbitmap, szPath );
   return ret;
}

bool C_HBITMAP::Load( std::string path )
{
   Destroy();
   if( path.find(".bmp") )
   {
      m_Hbitmap = (HBITMAP)::LoadImage( NULL, path.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );
   }
   return ValidBitmapHandle();
}

bool C_HBITMAP::LoadBitmapFromResource( DWORD resource_id )
{
   Destroy();
   m_Hbitmap = (HBITMAP)::LoadImage( ::GetModuleHandle( NULL ), MAKEINTRESOURCE( resource_id ), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );   
   return ValidBitmapHandle();
}

bool C_HBITMAP::ValidBitmapHandle( void )
{
   if( m_Hbitmap )
   {
      ::GetObject( m_Hbitmap, sizeof( BITMAP ), &m_Bitmap );
      return true;
   }
   return false;
}

void C_HBITMAP::Display( HDC Hdc, int x, int y, DWORD dwRop )
{
   ::BitBlt( Hdc, x, y, Width(), Height(), m_MemoryDC, 0, 0, dwRop );
}

void C_HBITMAP::StretchDisplay( HDC Hdc, int x, int y, int width_of_dest, int height_of_dest, DWORD dwRop )
{
   ::StretchBlt( Hdc, x, y, width_of_dest, height_of_dest, m_MemoryDC, 0, 0, Width(), Height(), dwRop );
}

bool C_HBITMAP::SelectHbitmapIntoMemoryDC( void )
{
   if( !m_bSelected )
   {
      m_bSelected = true;
      m_HbitmapOrignal = (HBITMAP)::SelectObject( m_MemoryDC, m_Hbitmap );
      return true;
   }
   return false;
}

void C_HBITMAP::Destroy( void )
{
   if( m_Hbitmap )
   {
      ::SelectObject( m_MemoryDC, m_HbitmapOrignal );
      ::DeleteObject( m_Hbitmap );
      m_bSelected = false;
      m_Hbitmap = NULL;
      ::DeleteDC( m_MemoryDC );
      m_MemoryDC = NULL;
   }
   ::ZeroMemory( &m_Binfo, sizeof(BITMAPINFO) );
   ::ZeroMemory( &m_Bitmap, sizeof(BITMAP) );
}

void C_HBITMAP::Copy( const C_HBITMAP& rhs )
{
//   m_Hbitmap        = rhs.m_Hbitmap;
//   m_HbitmapOrignal = rhs.m_HbitmapOrignal;
//   std::memcpy( &m_Binfo,  &rhs.m_Binfo, sizeof(m_Binfo) );
//   std::memcpy( &m_Bitmap, &rhs.m_Bitmap, sizeof(m_Bitmap) );
//   m_MemoryDC       = rhs.m_MemoryDC;
//   m_bSelected      = rhs.m_bSelected;
}

bool C_HBITMAP::GetRect( const C_HBITMAP &bitmap, C_Structs::Rect rect, HWND Hwnd )
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


