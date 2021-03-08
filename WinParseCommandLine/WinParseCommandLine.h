#ifndef WinParseCommandLine_H
#define WinParseCommandLine_H

#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include "../errorutil/errorutil.h"

class WinParseCommandLine
{
   public:
      WinParseCommandLine( LPSTR, char[] = " -\"" );
      virtual ~WinParseCommandLine( void ){};

      unsigned short size( void )
      {
         return m_argv.size();
      }
      
      bool find( std::string test_string )
         { return find( test_string.c_str() ); }

      bool find( char test_string[] )
      {
         for( int i=0; i<m_argv.size() ; ++i )
         {
            if( m_argv.at(i) == test_string )
            {
               return true;
            }
         }
         return false;
      }

      short findatstart( std::string test_string )
         { return find( test_string.c_str() ); }

      short findatstart( char test_string[] )
      {
         for( int i=0; i<m_argv.size() ; ++i )
         {
            if( m_argv.at(i).find( test_string ) == 0 )
            {
               return i;
            }
         }
         return -1;
      }

      std::string operator () ( unsigned short index )
         {
            if( index < m_argv.size() )
               return m_argv.at( index );
            else
            {
               std::string temp;
               return temp;
            }
         }

      operator std::string( void )
      {
         return m_orignalcommandlinestring;
      }

   protected:
      std::vector<std::string> m_argv;
      std::string m_orignalcommandlinestring;

   private:
};

#endif
