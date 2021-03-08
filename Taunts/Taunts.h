#ifndef __Taunts_H
#define __Taunts_H

#include <string>
#include <vector>
#include <fstream>
#include "../errorutil/errorutil.h"

class Taunts
{
   public:
      
      Taunts( void ){}
      Taunts( std::string tauntsfilename )
      {
         LoadStringTaunts( tauntsfilename );
      }
      ~Taunts(){}

      bool LoadStringTaunts( std::string tauntsfilename )
      {
         try
         {
            std::ifstream fin( tauntsfilename.c_str(), std::ios::in );
            if( !fin.is_open() )
               return false;
            std::string tmp;
            while( !std::getline( fin, tmp ).eof() )
            {
               m_StringTaunts.push_back( tmp );
            }
         }
         catch( ... )
         {
            return false;
         }
         return true;
      }

      std::string RandomTaunt( bool ballow_same_taunt_twice_in_a_row = false )
      {
         if( !m_StringTaunts.size() )
            return std::string();

         int index = rand()%m_StringTaunts.size();
         static int last_index = index;

         if( ballow_same_taunt_twice_in_a_row )
            return m_StringTaunts.at( index );

         while( last_index == index )
            index = rand()%m_StringTaunts.size();

         std::string returnvalue = m_StringTaunts.at( index );
         last_index = index;
         return returnvalue;
      }

//      std::string RandomTaunt( int number_of_times_to_wait = 3 )
//      {
//         return std::string();
//      }

      std::string operator [] ( unsigned int index )
      {
         return m_StringTaunts.at( index );
      }

      unsigned long size( void ) const
      {
         return m_StringTaunts.size();
      }

   protected:

      std::vector< std::string > m_StringTaunts;
      
   private:
};

#endif
