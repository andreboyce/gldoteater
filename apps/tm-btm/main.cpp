#include <iostream>
#include <cstdlib>
#include <string>

#include "../../TileMap/TileMap.h"
#include "../../TileMapLib/TileMapLib.h"

int main( int argc, char *argv[] )
{

   std::string map_name;
   if( argc > 1 )
   {
      map_name = argv[1];
   }
   else
   {
      Alert( "Usage: \r\ntm-to-btm.exe file.tm \r\nThis will convert a tm file to a btm file." );
      return 0;
   }
   if( map_name.find( ".btm" ) != std::string::npos )
   {
      Alert( "Already a btm file");
      return -1;
   }

   TileMap                t;
   TileMapLib::C_TileMap tm;

   std::string path = ( map_name.substr( 0, map_name.rfind("\\") + 1 ) );
   chdir( path.c_str() );   

   if( map_name.find( ".tm" ) != std::string::npos )
   {
      if( !tm.LoadMap( map_name ) )
      {
         Alert( "error loading map: " + map_name );
         return -1;
      }
   }
   else
   {
      if( !tm.LoadMap( map_name + ".tm" ) )
      {
         Alert( "error loading map: " + map_name + ".tm" );
         return -1;
      }
   }
   t = tm;
   t.m_SpecialDataTypes.clear();
   t.m_SpecialData.clear();

   t.m_SpecialDataTypes.push_back( TileMapSpecialDataTypes( t.m_SpecialDataTypes.size(), "p1spawn"    ) );
   t.m_SpecialDataTypes.push_back( TileMapSpecialDataTypes( t.m_SpecialDataTypes.size(), "p2spawn"    ) );
   t.m_SpecialDataTypes.push_back( TileMapSpecialDataTypes( t.m_SpecialDataTypes.size(), "rspawn"     ) );
   t.m_SpecialDataTypes.push_back( TileMapSpecialDataTypes( t.m_SpecialDataTypes.size(), "bspawn"     ) );
   t.m_SpecialDataTypes.push_back( TileMapSpecialDataTypes( t.m_SpecialDataTypes.size(), "yspawn"     ) );
   t.m_SpecialDataTypes.push_back( TileMapSpecialDataTypes( t.m_SpecialDataTypes.size(), "wspawn"     ) );
   t.m_SpecialDataTypes.push_back( TileMapSpecialDataTypes( t.m_SpecialDataTypes.size(), "bigdot"     ) );
   t.m_SpecialDataTypes.push_back( TileMapSpecialDataTypes( t.m_SpecialDataTypes.size(), "randomitem" ) );
   int pos = 0;
   if( ( pos = map_name.find( ".tm" ) ) != std::string::npos )
   {
      map_name.erase( pos, 3 );
   }
   if( !t.write( map_name + ".btm" ) )
   {
      Alert( "error saving map: " + map_name + ".btm" );
      return -1;
   }
   if( !t.writeXTM( map_name + ".xtm" ) )
   {
      Alert( "error saving map: " + map_name + ".xtm" );
      return -1;
   }
   return 0;
}
