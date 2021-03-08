#ifndef __CHBITMAP_H
#define __CHBITMAP_H

#include <string>
#include "../WindowsUtility/WindowsUtility.h"

class CHBITMAP
{
   public:
       CHBITMAP( void );
       CHBITMAP( const CHBITMAP& Hbitmap )
       {
          Copy( Hbitmap );
       }

      ~CHBITMAP( void );

       bool Load( std::string path );
       bool Load( DWORD ID );
       void Display( HDC Hdc, int x, int y, DWORD dwRop = SRCCOPY );
       void StretchDisplay( HDC Hdc, int x, int y, int width_of_dest, int height_of_dest, DWORD dwRop = SRCCOPY );
       bool LoadBitmapFromResource( DWORD resource_ID   );
       bool LoadBitmapFromFile( std::string path );
       bool Save( std::string szPath );
       bool GetRect( const CHBITMAP &bitmap, Structs::Rect rect, HWND Hwnd );
       bool SetSize( unsigned short width, unsigned short height );


       long Width(  void ) const
       {
          return m_Bitmap.bmWidth;
       }

       long Height( void ) const
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
          m_bSelected = true;
          //m_Hwnd = Hwnd;
       }
       
       operator HBITMAP (void) const
       {
          return m_Hbitmap;
       }

       operator HDC (void) const
       {
          return m_MemoryDC;
       }

       CHBITMAP operator = ( const CHBITMAP& Hbitmap )
       {
          Copy( Hbitmap );
          return *this;
       }

   protected:
      void Destroy( void );
      bool SelectHbitmapIntoMemoryDC( void );
      bool ValidBitmapHandle( void );
      void Copy( const CHBITMAP& rhs );

      HBITMAP    m_Hbitmap, 
                 m_HbitmapOrignal;
//      BITMAPINFO m_Binfo;
      BITMAP     m_Bitmap;
      HDC        m_MemoryDC;
      bool       m_bSelected;
//      HWND       m_Hwnd;

   private:
};

#endif

