#pragma once

#ifndef __Options_H
#define __Options_H

#include <vector>
#include <cmath>
#include "../errorutil/errorutil.h"

class Options
{
   public:
      typedef std::vector< bool > V_Option;
      typedef V_Option::reference V_OptionReference;

      Options( void ) :
         m_CursorPos( 0 )
      {
      }

      Options( const unsigned long number_of_options, long CursorPos = 0 ) :
         m_Options( number_of_options, false ),
         m_CursorPos( CursorPos )
      {
//         if( m_Options.size() == 0 )
//            LogError( "Could not push_back Options" );
      }

      Options( const Options & rhs ) :
          m_Options( rhs.m_Options ),
          m_CursorPos( rhs.m_CursorPos )
      {
      }

      ~Options( void )
      {
      }

      Options & operator = ( const Options & rhs )
      {
         m_Options   = rhs.m_Options;
         m_CursorPos = rhs.m_CursorPos;
      }

//      inline V_OptionReference operator [] ( const unsigned short index )
//      {
//         return m_Options.at( index );
//      }

      inline V_OptionReference operator [] ( const unsigned short index )
      {
            return m_Options[ index ];
      }

      void operator++ ( void )
      {
         ++m_CursorPos;
         if( (m_CursorPos > (m_Options.size()-1)) && (m_CursorPos >= 0) )
            m_CursorPos = 0;
         if( m_CursorPos < 0 )
            m_CursorPos = m_Options.size()-1;
      }

      void operator-- ( void )
      {
         --m_CursorPos;
         if( (m_CursorPos > (m_Options.size()-1)) && (m_CursorPos >= 0) )
            m_CursorPos = 0;
         if( m_CursorPos < 0 )
            m_CursorPos = m_Options.size()-1;
      }

      void SetOnlyTrue( const long index )
      {
         Reset();
         m_Options.at( index ) = true;
         m_CursorPos = index;
      }

      void SetOnlyFalse( const long index )
      {
         Reset( true );
         m_Options.at( index ) = false;
         m_CursorPos = index;
      }

      void SetOnlyTrueAtCursor( void )
      {
         Reset();
         m_Options.at( m_CursorPos ) = true;
      }

      void SetOnlyFalseAtCursor( void )
      {
         Reset( true );
         m_Options.at( m_CursorPos ) = false;
      }

      bool inline Reset( bool bSelected = false )
      {
         for( int i = 0; i < m_Options.size() ; ++i )
            m_Options[i] = bSelected;
         return true;
      }

      long GetCorsorPosition( void ) const
      {
         return m_CursorPos;
      }

      V_Option m_Options;
      long     m_CursorPos;

   protected:
   private:
};

#endif

