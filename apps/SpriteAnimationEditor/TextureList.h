#ifndef __TextureList_H
#define __TextureList_H

#include <queue>
#include <string>
#include <fstream>
#include "../../errorutil/errorutil.h"

class TextureList
{
   public:
      TextureList(){}
      ~TextureList(){}
      std::deque<std::string> m_Strings;
      void clear()
      {
         m_Strings.clear();
      }
      bool read( std::string filename )
      {
         std::ifstream in_file( filename.c_str(), std::ios::in );
         if( !in_file.is_open() )
            return false;
         std::string tmp;
         while( !std::getline( in_file, tmp ).eof() )
         {
            m_Strings.push_back( tmp );
         }
         return true;
      }
      bool write( std::string filename )
      {
         std::ofstream out_file( filename.c_str(), std::ios::out );
         if( !out_file.is_open() )
            return false;
         for( unsigned int i=0; i<m_Strings.size() ; i++ )
         {
            out_file << m_Strings[i] << std::endl;
         }
         return true;
      }
   protected:
   private:
};

#endif
