#ifndef __C_HBITMAP_H
#define __C_HBITMAP_H

#include <string>
#include "../WindowsUtility/WindowsUtility.h"

class C_HBITMAP
{
   public:
       C_HBITMAP( void );
       C_HBITMAP( const C_HBITMAP& Hbitmap )
       {
          Copy( Hbitmap );
       }

      ~C_HBITMAP( void );

       bool Load( std::string path );
       bool Load( DWORD ID );
       void Display( HDC Hdc, int x, int y, DWORD dwRop = SRCCOPY );
       void StretchDisplay( HDC Hdc, int x, int y, int width_of_dest, int height_of_dest, DWORD dwRop = SRCCOPY );
       bool LoadBitmapFromResource( DWORD resource_ID   );
       bool LoadBitmapFromFile(  std::string path );
       bool Save( std::string szPath );
       bool GetRect( const C_HBITMAP &bitmap, C_Structs::Rect rect, HWND Hwnd );


       short Width(  void ) const
       {
          return m_Bitmap.bmWidth;
       }

       short Height( void ) const
       {
          return m_Bitmap.bmHeight;
       }

       void SetHwnd( HWND Hwnd )
       {
          BITMAP bm;
          HDC hdc    = ::GetDC( Hwnd );
          m_MemoryDC = ::CreateCompatibleDC( hdc );
          m_HbitmapOrignal = (HBITMAP) ::SelectObject( m_MemoryDC, m_Hbitmap );
          ::GetObject( m_Hbitmap, sizeof(bm), &bm );
          ::ReleaseDC( Hwnd, hdc );
       }
       
       operator HBITMAP (void) const
       {
          return m_Hbitmap;
       }

       operator HDC (void) const
       {
          return m_MemoryDC;
       }

       C_HBITMAP operator = ( const C_HBITMAP& Hbitmap )
       {
          Copy( Hbitmap );
          return *this;
       }

   protected:
      void Destroy( void );
      bool SelectHbitmapIntoMemoryDC( void );
      bool ValidBitmapHandle( void );
      void Copy( const C_HBITMAP& rhs );

      HBITMAP    m_Hbitmap, 
                 m_HbitmapOrignal;
      BITMAPINFO m_Binfo;
      BITMAP     m_Bitmap;
      HDC        m_MemoryDC;
      bool       m_bSelected;

   private:
};

#endif

