#pragma once

#ifndef __CHPen_H
#define __CHPen_H

#include "../errorutil/errorutil.h"

class CHPen
{
   public:
      CHPen( void ) : m_Hpen( NULL ), m_Hdc(NULL), m_HpenOrignal(NULL) {};
      CHPen( int style, int width, COLORREF color ) : m_Hpen(NULL), m_Hdc(NULL), m_HpenOrignal(NULL)
      {
         m_Hpen = ::CreatePen( style, width, color);
      }
      ~CHPen( void )
      {
         Destroy();
      }

      void Create( int style, int width, COLORREF color )
      {
         Destroy();
         m_Hpen = ::CreatePen( style, width, color );
      }

      void SelectToDC( HDC hdc )
      {
         m_Hdc = hdc;
         m_HpenOrignal = (HPEN)::SelectObject( hdc, m_Hpen );
      }

      operator HPEN( void ) const
      {  return m_Hpen;  }

   protected:
      HPEN     m_Hpen, m_HpenOrignal;
      HDC      m_Hdc;
      
      void Destroy( void )
      {
         if( m_Hpen )
         {
            try
            {
               ::SelectObject( m_Hdc, m_HpenOrignal );
               if( !::DeleteObject( m_Hpen ) )
                  throw;
            }
            catch(...)
            {
               LogError( GetLastSystemError() );
            }
         }      
      }

   private:
};

#endif

