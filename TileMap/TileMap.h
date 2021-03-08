#pragma once

#ifndef __TileMap_H
#define __TileMap_H

#include <map>
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <exception>
#include <direct.h>

#include "../Tile/Tile.h"
#include "../errorutil/errorutil.h"

#include "TileMapHeader.h"
#include "TileMapSpecialData.h"
#include "TileMapSpecialDataTypes.h"
#include "../TileMapLib/TileMapLib.h"


/*
File Layout

------------------------
Header      // header
------------------------
------------------------
MapData     // tiles
------------------------
------------------------
SpecialDataTypes // name and id of each spacial data type
------------------------
------------------------
SpecialData      // special attributes about certain tiles
------------------------
*/

class TileMap
{
   public:

      TileMap() : m_Header()
      {
         m_Tiles.reserve( 32*32 );
      }

      TileMap& operator = (TileMapLib::C_TileMap& tilemap );
	  void assign( TileMapLib::C_TileMap& tilemap );

      ~TileMap(){}

      bool FindSpecialDataType( std::string name );
      int GetSpecialDataTypeIndex( std::string name );

      std::deque< TileMapSpecialData > GetSpecialData( std::string name )
      {
         std::deque< TileMapSpecialData > special_data;
         int index = GetSpecialDataTypeIndex( name );
         if( index > -1 )
         {
            if( int(m_SpecialDataTypes.size()) > index )
            {
               TileMapSpecialDataTypes sdt = m_SpecialDataTypes[index];
               for( int i=0 ; i<int(m_SpecialData.size()) ; ++i )
               {
                   if( m_SpecialData[i].value == sdt.id )
                   {
                       special_data.push_back( m_SpecialData[i] );
                   }
               }
            }
         }
         return special_data;
      }

      TileMapHeader                         m_Header;
      std::deque< TileMapSpecialData >      m_SpecialData;
      std::deque< TileMapSpecialDataTypes > m_SpecialDataTypes;
      std::vector< std::vector<Tile> >      m_Tiles;
      std::vector< std::string >            m_TileSet;
      std::string                           m_TileSetWorkingDirectory;

      std::vector< std::vector<Tile> >& GetTiles()
      {
         return m_Tiles;
      }

      bool LoadMap( std::string map )
      {
         return read( map );
      }
      bool LoadTileSet( std::string filename );
      std::vector< std::string >& GetTileSet();
      void clear( void );
      bool write( std::string filename );
	  bool writeXTM( std::string filename );
      bool read( std::string filename );
      bool resize( int width, int height );

      std::vector<Tile>& operator []( int index )
      {
         return m_Tiles[index];
      }

      int maxsolidity() const
      {
          return m_Header.max_solidity;
      }

      bool LeftTile( Structs::Point tile )
      {
         if( !m_Header.width || !m_Header.height )
            return false;
         if( tile.x-1 >= 0 && tile.x-1 < m_Header.width )
            return true;
         return false;
      }

      bool RightTile( Structs::Point tile )
      {
         if( !m_Header.width || !m_Header.height )
            return false;
         if( tile.x+1 >= 0 && tile.x+1 < m_Header.width )
            return true;
         return false;
      }

      bool UpTile( Structs::Point tile )
      {
         if( !m_Header.width || !m_Header.height )
            return false;
         if( tile.y-1 >= 0 && tile.y-1 < m_Header.height )
            return true;
         return false;
      }

      bool DownTile( Structs::Point tile )
      {
         if( !m_Header.width || !m_Header.height )
            return false;
         if( tile.y+1 >= 0 && tile.y+1 < m_Header.height )
            return true;
         return false;
      }

      unsigned int width()
      {
         return m_Header.width;
      }

      unsigned int height()
      {
         return m_Header.height;
      }

      unsigned long size()
      {
         return width()*height();
      }

   protected:
      void ResizeRow( int index, int width );
      void SetHeader( void );
      bool WriteTiles( std::ofstream& file );
	  bool WriteTilesXTM( std::ofstream& file, std::string tabs );
      bool ReadTiles( std::ifstream& file );

      bool WriteSpecialDataTypes( std::ofstream& file );
	  bool WriteSpecialDataTypesXTM( std::ofstream& file, std::string tabs );
      bool ReadSpecialDataTypes( std::ifstream& file );

      bool WriteSpecialData( std::ofstream& file );
	  bool WriteSpecialDataXTM( std::ofstream& file, std::string tabs );
      bool ReadSpecialData( std::ifstream& file );


   private:
};

#endif
