#ifndef __C_HDC_H
#define __C_HDC_H

#include <string>
#include <sstream>
#include "../C_Structs/C_Structs.h"
#define WIN32_LEAN_AND_MEAN
#include "../WindowsUtility/WindowsUtility.h"

class C_HDC
{
   public:
       virtual ~C_HDC( void );

      void GetTextSize( int &, int & );
      void Line( const C_Structs::Point &start, const C_Structs::Point &end );

      int BitmapWidth( void )
      {
         return m_BitmapWidth;
      }

      int BitmapHeight( void )
      {
         return m_BitmapHeight;
      }

      HGDIOBJ SelectObject( HGDIOBJ hobj )
      {
         return ::SelectObject( m_Hdc, hobj );
      }

      void Clear( C_Structs::Rect &rect, int Object = BLACK_BRUSH )
      {         
         ::FillRect( m_Hdc, &rect, (HBRUSH) ::GetStockObject( Object ) );
      }

      void Clear( int Object = BLACK_BRUSH )
      {
         C_Structs::Rect rect;
         rect.right  = GetWindowWidth( m_Hwnd );
         rect.bottom = GetWindowHeight( m_Hwnd );
         ::FillRect( m_Hdc, &rect, (HBRUSH) ::GetStockObject( Object ) );
      }

      bool TextOut( int nXStart, int nYStart, double num )
      {
         std::ostringstream temp;
         if( num )
            temp << num;
         else
            temp << "0";
         std::string str;
         temp.str( str );
         return (bool)::TextOut( m_Hdc, nXStart, nYStart, (LPCTSTR)str.c_str(), str.size() );
      }

      bool TextOut( int nXStart, int nYStart, LPCTSTR lpString )
      {
         return (bool)::TextOut( m_Hdc, nXStart, nYStart, lpString, strlen( lpString ) );
      }

      bool TextOut( int nXStart, int nYStart, std::string text )
      {
         return (bool)::TextOut( m_Hdc, nXStart, nYStart, (LPCTSTR)text.c_str(), text.size() );
      }

      SIZE GetTextDimensions( std::string text )
      {
         SIZE size = {0};   
         if( !::GetTextExtentPoint32( m_Hdc, (LPCTSTR)text.c_str(), (int)text.size(), &size ) )
         {
         }
         return size;
      }

      SIZE GetTextDimensions( const char* text )
      {
         SIZE size = {0};
         if( !::GetTextExtentPoint32( m_Hdc, (LPCTSTR)text, (int)strlen( text ), &size ) )
         {
         }
         return size;
      }

      operator HDC ( void )
      {
         return m_Hdc;
      }
      operator HBITMAP ( void )
      {
         return m_Hbitmap;
      }
      operator HWND ( void )
      {
         return m_Hwnd;
      }

   protected:
      C_HDC( HDC );
      C_HDC( void );
      HDC     m_Hdc;
      HWND    m_Hwnd;
      HBITMAP m_Hbitmap, 
              hBitmapOrignal;
      int     m_BitmapWidth,
              m_BitmapHeight;

   private:
};

#endif 

