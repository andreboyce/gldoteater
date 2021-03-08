#include "TileMapLib.h"

namespace TileMapLib
{

bool C_TileMap::LoadSpriteMap( std::string filename )
{
   return m_SpriteMap.Load( filename );
}

bool C_TileMap::SaveSpriteMap( std::string filename )
{
   return m_SpriteMap.Save( filename );
}

AStar::AStar2d::SearchStats C_TileMap::GetAStarSearchStats( void )
{
   return m_AStar2d.GetSearchStats();
}

void C_TileMap::CalculateAStarPath( void )
{
   m_AStarPath = m_AStar2d.CalculatePath( m_Tiles, m_AStarStartPos, m_AStarEndPos, maxsolidity(), m_bAllowDiagonal );
}

void C_TileMap::ReLoadTile( int index )
{
   if( (index < m_Bitmaps.size()) && (index < m_TileSet.size()) )
   {
      m_Bitmaps[ index ].Load( m_TileSet[ index ] );
      m_Bitmaps[ index ].SetHwnd( m_Hwnd );
   }
}

void C_TileMap::ResizeRow( int index, int width )
{
   // out of bounds
   if( width < 0 || index > m_Tiles.size() )
     return;
   // no resizing necessary
   if( width == m_Tiles[index].size() )
      return;
   if( m_Tiles.size() > index )
   {
      // clear
      if( width == 0 )
      {
         m_Tiles.clear();
         return;
      }
      // make larger
      else if( width > m_Tiles[index].size() )
      {
         int number_of_tiles_to_add = width - m_Tiles[index].size();
         for( int i = 0; i<number_of_tiles_to_add ; ++i )
         {
            m_Tiles[index].push_back( Tile( 0, 0 ) );
         }
         return;    
      }
      // make smaller
      else if( width < m_Tiles[index].size() )
      {
         int number_of_tiles_to_remove = m_Tiles[index].size() - width;
         for( int i = 0; i<number_of_tiles_to_remove ; ++i )
         {
            m_Tiles[index].pop_back();
         }
      }
   }
}

void C_TileMap::ResizeMap( int width, int height )
{
   if( height < 0 || width < 0 )
     return;

   if( height == 0 || width == 0 )
   {
      // delete the emtire map
      DeleteTiles();
   }

   // Add rows
   if( height > m_Tiles.size() )
   {
      int rows_to_add = height - m_Tiles.size();
      for( int i=0; i<rows_to_add ; ++i )
      {
         m_Tiles.push_back( std::vector<Tile>() );
      }
   }
   // Delete Rows
   if( height < m_Tiles.size() )
   {
      int rows_to_delete = m_Tiles.size() - height;
      for( int i=0; i<rows_to_delete ; ++i )
      {
         m_Tiles.pop_back();
      }
   }
   // resize the colums
   for( int i=0; i<height ; ++i )
   {
      ResizeRow( i, width );
   }
   m_EndDrawTile.x = this->width();
   m_EndDrawTile.y = this->height();
}

void C_TileMap::DeleteOrignalSelectedTiles( void )
{
   m_OrignalTilesSelected.clear();
}

void C_TileMap::MakeNewMap( int width, int height )
{
   DeleteBitmaps();
   DeleteTiles();
   for( int r=0; r<height ; r++ )
   {
      std::vector<Tile> row;
      for( int c=0; c<width ; c++ )
      {
         row.push_back( Tile( 0, 0 ) );
      }
      m_Tiles.push_back( row );
   }
//   ValidateTiles();
   m_EndDrawTile.x = this->width();
   m_EndDrawTile.y = this->height();
}

bool C_TileMap::SaveTileSet( std::string filename )
{
   std::ofstream tilesetfile( filename.c_str(), std::ios::out );
   if( !tilesetfile.is_open() )
      return false;
   tilesetfile << m_TileSetWorkingDirectory << std::endl; // save the root directory
   for( int i = 0; i<m_TileSet.size() ; ++i )
   {
      tilesetfile << m_TileSet[i] << std::endl; // save the bitmap filename
   }
   tilesetfile.close();
   return true;
}

void C_TileMap::Destroy( void )
{
   m_Tiles.clear();
   m_Bitmaps.clear();
   m_SpriteMap.clear();
   m_Sprites.clear();
   m_TileSet.clear();
   m_Sprites.clear();

   m_MapFileName.clear();
   m_TileSetFileName.clear();
   m_MapWorkingDirectory.clear();
   m_TileSetWorkingDirectory.clear();
   m_SpriteMap.clear();
}

bool C_TileMap::LoadTileSet( std::string filename )
{
   try
   {

      /*char buffer[MAX_PATH];
      ::ZeroMemory( buffer, MAX_PATH );
      ::getcwd( buffer, MAX_PATH-1 );
      std::string temp = buffer;
      temp += "\\";*/

      std::ifstream tileset( filename.c_str(), std::ios::in );
      if( !tileset.is_open() )
         return false;

      std::getline( tileset, m_TileSetWorkingDirectory );
	  if( m_TileSetWorkingDirectory.size() )
	  {
	     ::chdir( m_TileSetWorkingDirectory.c_str() );
	  }

      std::string bitmapfilename;

      while( !std::getline( tileset, bitmapfilename ).eof() )
      {
         if( AddBitmap( bitmapfilename ) )
         {
            m_TileSet.push_back( bitmapfilename );
         }
      }
      tileset.close();
      ::chdir( m_MapWorkingDirectory.c_str() );
      return true;
   }
   catch( std::exception& e )
   {
      ::chdir( m_MapWorkingDirectory.c_str() );
      Alert( e.what() );
      return false;
   }
   catch( ... )
   {
      ::chdir( m_MapWorkingDirectory.c_str() );
      return false;
   }
}

bool C_TileMap::LoadMap( std::string filename )
{
   try
   {
      Destroy();
      char buffer[MAX_PATH];
      ::ZeroMemory( buffer, MAX_PATH );
      ::getcwd( buffer, MAX_PATH-1 );
      std::string temp = buffer;
      temp += "\\";
      SetMapWorkingDirectory( temp );
      std::ifstream tilemap( filename.c_str(), std::ios::in );
      if( !tilemap.is_open() )
         return false;
      m_MapFileName = filename;
      std::string   tilesetfilename;
      std::getline( tilemap, tilesetfilename );
      if( !LoadTileSet( tilesetfilename ) )
      {
         tilemap.close();
         return false;
      }
      m_TileSetFileName = tilesetfilename;
      std::string spritemapfilename;
      std::getline( tilemap, spritemapfilename );
      if( !LoadSpriteMap( spritemapfilename ) )
      { 
         return false;
      }
      std::string line; // will contain a line of tile id's
      while( !std::getline( tilemap, line ).eof() )
      {
         std::vector<Tile> row;
         boost::tokenizer< boost::escaped_list_separator<char> > tiles( line );
         for( boost::tokenizer< boost::escaped_list_separator<char> >::iterator beg = tiles.begin();
              beg != tiles.end();
              ++beg )
         {
            if( !(*beg).size() )
               break;
            boost::escaped_list_separator<char> els( '\\', '|', '\"' );
            boost::tokenizer< boost::escaped_list_separator<char> > tile( (*beg), els );
            short graphic  = 0;
            short solidity = 0;

            if( tile.begin() != tile.end() )
            {
               boost::tokenizer< boost::escaped_list_separator<char> >::iterator token = tile.begin();
               graphic  = ::atoi( (*token).c_str() );
               token++;
               if( token != tile.end() )
                  solidity = ::atoi( (*token).c_str() );
            }
            row.push_back( Tile( graphic, solidity ) );
         }
         m_Tiles.push_back( row );
      }
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
   ValidateTiles();
   m_EndDrawTile.x = width();
   m_EndDrawTile.y = height();
   return true;
}

bool C_TileMap::SaveMap( std::string filename )
{
   try
   {
      std::ofstream tilemap( filename.c_str(), std::ios::out );
      if( !tilemap.is_open() )
         return false;
      std::ostringstream line;
      std::vector<Tile>::iterator tile;

      tilemap << m_TileSetFileName         << std::endl;
      tilemap << m_SpriteMap.GetFileName() << std::endl;

      SaveTileSet( m_TileSetFileName );
      m_SpriteMap.Save( m_SpriteMap.GetFileName() );

      for( int r = 0; r < m_Tiles.size(); r++ )
      {
         for( tile = m_Tiles[r].begin(); tile < m_Tiles[r].end() ; tile++ )
         {
            line << (*tile).m_TileGraphic << "|" << (*tile).m_Solidity << ',';
         }
         tilemap << line.str() << std::endl;
         line.str( std::string() );
      }
      tilemap.close();
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
   return true;
}

bool C_TileMap::AddTile( std::string filename )
{
   bool ret = AddBitmap( filename );
   if( ret )
   {
      try
      {
         m_TileSet.push_back( filename );
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
   return ret;
}

bool C_TileMap::AddBitmap( std::string filename )
{
   try
   {
      m_Bitmaps.push_back( CHBITMAP() );
      if( !m_Bitmaps.back().Load( filename ) )
      {
         m_Bitmaps.pop_back();
         return false;
      }
      m_Bitmaps.back().SetHwnd( m_Hwnd );
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
   return true;
}

void C_TileMap::RemoveBitmap( int index )
{
   if( m_Bitmaps.size() > index )
   {
      m_Bitmaps.erase( m_Bitmaps.begin()+index );
   }
}

void C_TileMap::RemoveBitmap( HBitmapVector::iterator index )
{
   if( index < m_Bitmaps.end() )
   {
      m_Bitmaps.erase( index );
   }
}

bool C_TileMap::DeleteBitmaps( void )
{
   m_Bitmaps.clear();
   return true;
}

bool C_TileMap::DeleteTiles( void )
{
   m_Tiles.clear();
   return true;
}

bool C_TileMap::ValidateTiles( void )
{
//   if( !m_Bitmaps.size() )
//   {
//      DeleteTiles();
//   }
   int max_y = m_Tiles.size();
   int max_x;
   if( m_Tiles.size() )
   {
      max_x = m_Tiles[0].size();
   }

   for( int r = 0 ; r < max_y ; ++r )
   {
      for( int c = 0 ; c < max_x ; ++c )
      {
         if( m_Bitmaps.size() )
         {
            if( (m_Tiles[r][c].m_TileGraphic >= m_Bitmaps.size()) || (m_Tiles[r][c].m_TileGraphic < 0) )
            {
               m_Tiles[r][c].m_TileGraphic = 0;
            }
         }
      }
   }
   return true;
}

void C_TileMap::DeSelectedTiles( void )
{
   m_SelectedTiles.EndX   = -1;
   m_SelectedTiles.EndY   = -1;
   m_SelectedTiles.StartX = -1;
   m_SelectedTiles.StartY = -1;
}

void C_TileMap::CopySelectedTiles( void )
{
   DeleteSelectedTiles();
   DeleteOrignalSelectedTiles();
   if( (m_SelectedTiles.StartY < 0) || (m_SelectedTiles.StartX < 0) ||
       (m_SelectedTiles.EndY < 0)   || (m_SelectedTiles.EndX < 0) )
   {
      return;
   }
   int max_y = m_SelectedTiles.StartY + (m_SelectedTiles.EndY - m_SelectedTiles.StartY) +1;
   int max_x = m_SelectedTiles.StartX + (m_SelectedTiles.EndX - m_SelectedTiles.StartX) +1;
   int x = 0;
   int y = 0;
   for( int r = m_SelectedTiles.StartY ; r < max_y ; ++r, ++y )
   {
      m_TilesSelected.push_back( std::vector<Tile>() );
      m_OrignalTilesSelected.push_back( std::vector<Tile>() );
      for( int c = m_SelectedTiles.StartX ; c < max_x ; ++c, ++x )
      {
         m_TilesSelected[y].push_back( Tile() );
         m_OrignalTilesSelected[y].push_back( Tile() );
         m_TilesSelected[y][x]        = m_Tiles[r][c];
         m_OrignalTilesSelected[y][x] = m_Tiles[r][c];
      }
      x = 0;
   }
}

void C_TileMap::CutSelectedTiles( void )
{
   DeleteSelectedTiles();
   DeleteOrignalSelectedTiles();
   if( (m_SelectedTiles.StartY < 0) || (m_SelectedTiles.StartX < 0) ||
       (m_SelectedTiles.EndY < 0)   || (m_SelectedTiles.EndX < 0) )
   {
      return;
   }
   int max_y = m_SelectedTiles.StartY + (m_SelectedTiles.EndY - m_SelectedTiles.StartY) +1;
   int max_x = m_SelectedTiles.StartX + (m_SelectedTiles.EndX - m_SelectedTiles.StartX) +1;
   int x = 0;
   int y = 0;
   for( int r = m_SelectedTiles.StartY ; r < max_y ; ++r, ++y )
   {
      m_TilesSelected.push_back( std::vector<Tile>() );
      m_OrignalTilesSelected.push_back( std::vector<Tile>() );
      for( int c = m_SelectedTiles.StartX ; c < max_x ; ++c, ++x )
      {
         m_TilesSelected[y].push_back( Tile() );
         m_OrignalTilesSelected[y].push_back( Tile() );
         m_TilesSelected[y][x]        = m_Tiles[r][c];
         m_OrignalTilesSelected[y][x] = m_Tiles[r][c];
         m_Tiles[r][c].m_TileGraphic = 0;
         m_Tiles[r][c].m_TileGraphic = 0;
      }
      x = 0;
   }
}

void C_TileMap::DeleteSelectedTiles( void )
{
   m_TilesSelected.clear();
}

void C_TileMap::PasteSelectedTiles( int row, int col )
{
   Structs::Point tile( col, row );

   int max_x = 0;

   if( m_TilesSelected.size() )
   {
      max_x = m_TilesSelected[0].size();
   }
   else
   {
      return;
   }

   int max_y = m_TilesSelected.size();
   int x     = 0;
   int y     = 0;

   for( int r = tile.y ; y < max_y ; ++r, ++y )
   {
      for( int c = tile.x ; x < max_x ; ++c, ++x )
      {
         if( r >= m_Tiles.size() )
         {
            break;
         }
         if( c >= m_Tiles[r].size() )
         {
            break;
         }
         m_Tiles[r][c] = m_TilesSelected[y][x];
      }
      x = 0;
   }
}

bool C_TileMap::IsSelected( int row, int col )
{
   if( (row < m_SelectedTiles.StartY) || (row > m_SelectedTiles.EndY) )
   {
      return false;
   }
   if( (col < m_SelectedTiles.StartX) || (col > m_SelectedTiles.EndX) )
   {
      return false;
   }
   return true;
}

void C_TileMap::GetSelectedTiles( Structs::Point m_MousePosition, Structs::Point m_RButtonDownStartPoint )
{
   // Get the start position which should be the smaller of the two points
   if( m_MousePosition.y < m_RButtonDownStartPoint.y )
   {
      m_SelectedTiles.StartY = (int)m_MousePosition.y/m_TileHeight;
      m_SelectedTiles.EndY   = (int)m_RButtonDownStartPoint.y/m_TileHeight;
   }
   else //if( m_MousePosition.y < m_RButtonDownStartPoint.y || m_MousePosition.y == m_RButtonDownStartPoint.y )
   {
      m_SelectedTiles.StartY = (int)m_RButtonDownStartPoint.y/m_TileHeight;
      m_SelectedTiles.EndY   = (int)m_MousePosition.y/m_TileHeight;
   }

   if( m_MousePosition.x < m_RButtonDownStartPoint.x )
   {
      m_SelectedTiles.StartX = (int)m_MousePosition.x/m_TileWidth;
      m_SelectedTiles.EndX   = (int)m_RButtonDownStartPoint.x/m_TileWidth;
   }
   else //if( m_MousePosition.x < m_RButtonDownStartPoint.x || m_MousePosition.x == m_RButtonDownStartPoint.x )
   {
      m_SelectedTiles.StartX = (int)m_RButtonDownStartPoint.x/m_TileWidth;
      m_SelectedTiles.EndX   = (int)m_MousePosition.x/m_TileWidth;
   }
//  __ __ __ __ __
// |__|__|__|__|__|
// |__|__|__|__|__|
// |__|__|__|__|__|
// |__|__|__|__|__|
// |__|__|__|__|__|
//
}

}
