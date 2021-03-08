#pragma once

#ifndef __Tile_H
#define __Tile_H

#include <fstream>
#include <sstream>

struct Tile
{
   public:
      Tile() : 
         m_TileGraphic( 0 ),
         m_Solidity( 0 ) {}
      ~Tile(){}
      Tile( short TileGraphic ) : 
         m_TileGraphic( TileGraphic ), 
         m_Solidity( 0 ) {}
      Tile( short TileGraphic, short Solidity ) : 
         m_TileGraphic( TileGraphic ), 
         m_Solidity( Solidity ) {}
      Tile( const Tile & rhs ) :
         m_TileGraphic( rhs.m_TileGraphic ),
         m_Solidity( rhs.m_Solidity ) {}
      Tile& operator = ( const Tile & rhs )
      {
         m_TileGraphic = rhs.m_TileGraphic;
         m_Solidity    = rhs.m_Solidity;
         return *this;
      }
      bool read( std::ifstream& in_file )
      {
         if( !in_file.is_open() )
            return false;
         std::ofstream::pos_type pos = in_file.tellg();
         in_file.read( (char*)&m_TileGraphic, sizeof(m_TileGraphic) );
         in_file.read( (char*)&m_Solidity,    sizeof(m_Solidity)    );
         if( in_file.bad() )
         {
            in_file.seekg( pos );
            return false;
         }
         return true;
      }
      bool write( std::ofstream& out_file )
      {
         if( !out_file.is_open() )
            return false;
         std::ofstream::pos_type pos = out_file.tellp();
         out_file.write( (char*)&m_TileGraphic, sizeof(m_TileGraphic) );
         out_file.write( (char*)&m_Solidity,    sizeof(m_Solidity)    );
         if( out_file.bad() )
         {
            out_file.seekp( pos );
            return false;
         }
         return true;
      }

      operator std::string ( void )
      {
         std::ostringstream tmp;
         tmp << "{ TileGraphic: " << m_TileGraphic << "," << " Solidity: " << m_Solidity << " }";
         return tmp.str();
      }

      short m_TileGraphic;
      short m_Solidity;

   protected:
   private:
};

#endif

