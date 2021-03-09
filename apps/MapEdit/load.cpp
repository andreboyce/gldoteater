#include "main.h"

bool MainWindow::LoadMap( std::string map )
{
//   m_MapTexturesIndex = m_TextureManager.Size();
   if( !m_TileMap.LoadMap( map ) )
   {
      Alert( "Could not load map: " + map );
      return false;
   }
   m_MapFileName = map;
   char current_dir[ MAX_PATH ];
   ::ZeroMemory( current_dir, MAX_PATH );
   ::getcwd( (char*)current_dir, MAX_PATH );


   if( m_TileMap.m_TileSetWorkingDirectory.size() )
   {
	   ::chdir( m_TileMap.m_TileSetWorkingDirectory.c_str() );
   }

   std::vector< std::string > tile_set = m_TileMap.GetTileSet();
   if( tile_set.size() == 0 )
   {
   }
   for( int i=0; i<tile_set.size() ; i++ )
   {
      if( m_TextureManager.LoadGLTextureBitmapFile( tile_set[i] ) == ERROR_CNLT )
      {
         Alert( "Error loading texture:" + tile_set[i] );
      }
   }

//   for( int i=0; i<m_TileMap.m_SpecialDataTypes.size() ; i++ )
//   {
//      Alert( m_TileMap.m_SpecialDataTypes[i] );
//   }


   ::chdir( current_dir );

   return true;
}
