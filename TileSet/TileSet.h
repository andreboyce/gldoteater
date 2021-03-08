#pragma once

#ifndef __TileSet_H
#define __TileSet_H

#include <queue>
#include <string>
#include <fstream>
#include <exception>
#include "../errorutil/errorutil.h"

class TileSet
{
   public:
      TileSet(){}
      TileSet( const TileSet& rhs ) : m_Tiles( rhs.m_Tiles ){}
      TileSet& operator = ( const TileSet& rhs )
      {
         m_Tiles = rhs.m_Tiles;
         return *this;
      }
      ~TileSet(){}

      bool read( std::string filename )
      {
         std::ifstream file;
         try
         {
            file.open( filename.c_str(), std::ios::in );
            if( !file.is_open() )
            {
               return false;
            }

			// read tile directory
            std::getline( file, m_TileDirectory );

			// read the name of each tile
            std::string line;
            while( !std::getline( file, line ).eof() )
            {
               m_Tiles.push_back( line );
            }
         }
         catch( std::exception& e )
         {
            Alert( e.what() );
            return false;
         }
         catch( ... )
         {
            Alert( "Unknown Error" );
            return false;
         }
      }

	  ///<summary>Write the name of each tile + the directory they are in</summary>
      bool write( std::string filename )
      {
         std::ofstream file;
         try
         {
            file.open( filename.c_str(), std::ios::out );
            if( !file.is_open() )
            {
               return false;
            }
			file << m_TileDirectory << std::endl;
            for( unsigned int i=0; i<m_Tiles.size() ; i++ )
            {
               file << m_Tiles[i] << std::endl;
            }
         }
         catch( std::exception& e )
         {
            Alert( e.what() );
            return false;
         }
         catch( ... )
         {
            Alert( "Unknown Error" );
            return false;
         }
		 return false;
      }

      std::string operator []( int index )
      {
         return m_Tiles[index];
      }

      bool AddTile( std::string filename, int index = -1 )
      {
         try
         {
            if( index == -1 )
            {
               m_Tiles.push_back( filename );
               return true;
            }
            if( (unsigned int)index > m_Tiles.size() ||  index < 0 )
            {
               return false;
            }
            std::deque<std::string>::iterator position = m_Tiles.begin();
            position += index;
            m_Tiles.insert( position, 1, filename );
            return true;
         }
         catch( std::exception& e )
         {
            Alert( e.what() );
            return false;
         }
         catch( ... )
         {
            return false;
         }
      }
	  inline const size_t size()
	  {
		  return m_Tiles.size();
	  }

	  void SetTileDirectory( std::string TileDirectory )
	  {
		  m_TileDirectory = TileDirectory;
	  }

  	  void GetTileDirectory( void )
	  {
		  m_TileDirectory;
	  }

   protected:
      std::deque<std::string> m_Tiles; // filename of each tile
	  std::string             m_TileDirectory; // Name of the directory the tiles are in
	                                           // relative to the tileset file

   private:
};

#endif
