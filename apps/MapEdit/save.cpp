#include "main.h"

bool MainWindow::SaveMap( std::string map )
{
   if( !map.size() )
   {
      return false;
   }

   if( !m_TileMap.write( map ) )
   {
      Alert( "Could not save map: " + map );
      return false;
   }
   m_MapFileName = map;
   return true;
}
