#include <iostream>
#include <cstdlib>
#include <string>

#include "../../../TileMap/TileMap.h"
#include "../../../TileMapLib/TileMapLib.h"

int main( int argc, char *argv[] )
{
   try
   {
      std::string map_name;
      if( argc > 1 )
      {
         map_name = argv[1];
      }
      else
      {
	     
	     std::string msg( " Usage: \r\n\r\n map_converter.exe file \r\n\r\n This will convert a tm/btm/xtm to each of the other formats." );
		 std::cout << msg;
		 system( "pause" );
		 //Alert( msg );
         return 0;
      }

      std::string map_type;

      std::string path = ( map_name.substr( 0, map_name.rfind("\\") + 1 ) );
      if( path.size() )
	  {
	    if (chdir(path.c_str()) != 0)
		{
		}
	  }

      TileMap                t;
      TileMapLib::C_TileMap tm;

      if( map_name.find( ".btm" ) != std::string::npos )
      {
         if( !t.LoadMap( map_name ) )
         {
            //Alert( "error loading map: " + map_name );
            std::string msg( "error loading map: " + map_name );
            std::cout << msg << std::endl;
            system( "pause" );

            return -1;
         }
         map_type = "btm";
		 t.assign( tm );
      }

      if( map_name.find( ".xtm" ) != std::string::npos )
      {
         std::string msg( "Error loading map: " + map_name + "... XTM Loading not supported yet." );
         std::cout << msg << std::endl;
         system( "pause" );
         return -1;

         /*if( !t.LoadMapXTM( map_name ) )
         {
            //Alert( "error loading map: " + map_name );
            std::string msg( "error loading map: " + map_name );
            std::cout << msg << std::endl;
            system( "pause" );

            return -1;
         }*/
         map_type = "xtm";
		 t.assign( tm );
      }

      if( map_name.find( ".tm" ) != std::string::npos )
      {
         if( !tm.LoadMap( map_name ) )
         {
            //Alert( "error loading map: " + map_name );
            std::string msg( "error loading map: " + map_name );
            std::cout << msg << std::endl;
            system( "pause" );

            return -1;
         }

         map_type = "tm";

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

      }

      int pos = 0;
      if( ( pos = map_name.find( ".tm" ) ) != std::string::npos )
      {
         map_name.erase( pos, 3 );
      }
      if( ( pos = map_name.find( ".xtm" ) ) != std::string::npos )
      {
         map_name.erase( pos, 4 );
      }
      if( ( pos = map_name.find( ".btm" ) ) != std::string::npos )
      {
         map_name.erase( pos, 4 );
      }

	  if( map_type == "btm" || map_type == "xtm" )
	  {
		 if( !tm.SaveMap( map_name + ".tm" ) )
         {
            //Alert( "error saving map: " + map_name + ".tm" );
            std::string msg( "error saving map: " + map_name + ".tm" );
			std::cout << msg << std::endl;

            return -1;
         }
         std::string msg( "Saved map: " + map_name + ".tm" );
         std::cout << msg << std::endl;
	  }
	  if( map_type == "tm" || map_type == "xtm" )
	  {
         if( !t.write( map_name + ".btm" ) )
         {
            //Alert( "error saving map: " + map_name + ".btm" );
            std::string msg( "error saving map: " + map_name + ".btm" );
            std::cout << msg << std::endl;
            system( "pause" );

            return -1;
         }
         std::string msg( "Saved map: " + map_name + ".btm" );
         std::cout << msg << std::endl;
	  }
	  if( map_type == "tm" || map_type == "btm" )
	  {
         if( !t.writeXTM( map_name + ".xtm" ) )
         {
            //Alert( "error saving map: " + map_name + ".xtm" );
            std::string msg( "error saving map: " + map_name + ".xtm" );
            std::cout << msg<< std::endl;
            system( "pause" );

            return -1;
         }
         std::string msg( "Saved map: " + map_name + ".xtm" );
         std::cout << msg << std::endl;
	  }
	  system( "pause" );
      return 0;
   }
   catch( ... )
   {
      return -1;
   }
}
