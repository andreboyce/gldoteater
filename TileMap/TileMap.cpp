#include "TileMap.h"

bool TileMap::FindSpecialDataType( std::string name )
{
   for( int i=0; i<int(m_SpecialDataTypes.size()); ++i )
   {
       if( m_SpecialDataTypes[i].name == name )
       {
           return true;
       }
   }
   return false;
}

int TileMap::GetSpecialDataTypeIndex( std::string name )
{
   int index = -1;
   for( int i=0 ; i<int(m_SpecialDataTypes.size()) ; ++i )
   {
       if( m_SpecialDataTypes[i].name == name )
       {
           return i;
       }
   }
   return index;
}

/*std::deque< TileMapSpecialData > TileMap::GetSpecialData( std::string name )
{
   std::deque< TileMapSpecialData > special_data;
   int index = GetSpecialDataTypeIndex( name );
   if( index > -1 )
   {
      if( m_SpecialDataTypes.size() > index )
      {
         TileMapSpecialDataTypes sdt = m_SpecialDataTypes[index];
         for( int i=0; i<m_SpecialData.size(); ++i )
         {
             if( m_SpecialData[i].value == sdt.id )
             {
                 special_data.push_back( m_SpecialData[i] );
             }
         }
      }
   }
   return special_data;
}*/

// equivalent to  TileMapLib::C_TileMap = TileMap
void TileMap::assign( TileMapLib::C_TileMap& tilemap )
{
   //tilemap.m_TileHeight              = this->;
   //tilemap.m_TileWidth               = this->;
   tilemap.Setm_Solidity( this->m_Header.max_solidity );
   //tilemap.m_AStarStartPos           = rhs.m_AStarStartPos;
   //tilemap.m_AStarEndPos             = rhs.m_AStarEndPos;
   //tilemap.m_bShowPath               = rhs.m_bShowPath;
   //tilemap.m_bShowNodesSearched      = rhs.m_bShowNodesSearched;
   //tilemap.m_bAllowDiagonal          = rhs.m_bAllowDiagonal;
   //tilemap.m_bShowStartAndEndNode    = rhs.m_bShowStartAndEndNode;
   //tilemap.m_GraphicSolidityMap      = this->;
   //tilemap.m_Hwnd                    = rhs.m_Hwnd;
   //tilemap.m_SelectedTiles           = rhs.m_SelectedTiles;
   //tilemap.m_OrignalTilesSelected    = rhs.m_OrignalTilesSelected;
   //tilemap.m_TilesSelected           = rhs.m_TilesSelected;
   tilemap.m_Tiles                   = this->m_Tiles;
   //tilemap.m_AStar2d                 = rhs.m_AStar2d;
   tilemap.m_TileSet                 = this->m_TileSet;
   //tilemap.m_Bitmaps                 = rhs.m_Bitmaps; // fixed I think
   //tilemap.m_MapFileName             = this->;
   tilemap.m_TileSetFileName         = this->m_Header.tileset_file_name;
   //tilemap.m_AStarPath               = this->m_AStarPath;
   //tilemap.m_EndDrawTile             = rhs.m_EndDrawTile;
   //tilemap.m_EndDrawTileOffSet       = rhs.m_EndDrawTileOffSet;
   //tilemap.m_StartDrawTile           = rhs.m_StartDrawTile;
   //tilemap.m_StartDrawTileOffSet     = rhs.m_StartDrawTileOffSet;
   //tilemap.m_SpriteMap               = this->
   //tilemap.m_MapWorkingDirectory     = ;
   tilemap.Setm_TileSetWorkingDirectory( this->m_TileSetWorkingDirectory );
   //tilemap.m_Sprites                 = this->m_Sprites;
}

TileMap& TileMap::operator = ( TileMapLib::C_TileMap& tilemap )
{
   clear();
   AStar::AStar2d::TileVector2D tiles = tilemap.GetTiles();
   m_TileSet = tilemap.GetTileSet();
   m_Tiles   = tiles;
   m_Header.tileset_file_name  = tilemap.GetTileSetFileName();
   SpriteMap::D_Sprite sprites = tilemap.GetSpriteIDs();
   m_SpecialDataTypes.push_back( TileMapSpecialDataTypes( 0, "special data" ) );
   for( int i=0; i<int(sprites.size()); ++i )
   {
      m_SpecialData.push_back( TileMapSpecialData( sprites[i].pos.x, sprites[i].pos.y, 0 ) );
   }
   SetHeader();
   return *this;
}

bool TileMap::LoadTileSet( std::string filename )
{
   char current_dir[ MAX_PATH ];
   ::ZeroMemory( current_dir, MAX_PATH );
   _getcwd( (char*)current_dir, MAX_PATH );

   try
   {
      std::ifstream tileset( filename.c_str(), std::ios::in );
      if( !tileset.is_open() )
         return false;

      m_TileSetWorkingDirectory;
      std::getline( tileset, m_TileSetWorkingDirectory );
  	  if( m_TileSetWorkingDirectory.size() )
	  {
		  ::_chdir( m_TileSetWorkingDirectory.c_str() );
	  }

      std::string bitmapfilename;

      while( !std::getline( tileset, bitmapfilename ).eof() )
      {
         m_TileSet.push_back( bitmapfilename );
      }
      tileset.close();

      ::_chdir( current_dir );
      //::_chdir( m_MapWorkingDirectory.c_str() );
      return true;
   }
   catch( std::exception& e )
   {
      //::_chdir( m_MapWorkingDirectory.c_str() );
      ::_chdir( current_dir );
      Alert( e.what() );
      return false;
   }
   catch( ... )
   {
      //::_chdir( m_MapWorkingDirectory.c_str() );
      ::_chdir( current_dir );
      return false;
   }
}

std::vector< std::string >& TileMap::GetTileSet()
{
   return m_TileSet;
}

void TileMap::clear( void )
{
   m_Header.clear();
   m_SpecialData.clear();
   m_SpecialDataTypes.clear();
   m_Tiles.clear();
   m_TileSet.clear();
   m_TileSetWorkingDirectory.clear();
}

bool TileMap::write( std::string filename )
{
   std::ofstream file;
   try
   {
      file.open( filename.c_str(), std::ios::out | std::ios::binary );
      if( !file.is_open() )
         return false;
   }
   catch( std::exception& e )
   {
      Alert( e.what() );
      return false;
   }
   catch( ... )
   {
      Alert( "Unknown Exception" );
      return false;
   }
   SetHeader();
   m_Header.write( file );
   WriteTiles( file );
   WriteSpecialDataTypes( file );
   WriteSpecialData( file );
   file.close();
   return true;
}

bool TileMap::writeXTM( std::string filename )
{
   std::ofstream file;
   try
   {
	   file.open( filename.c_str(), std::ios::out );
      if( !file.is_open() )
         return false;
   }
   catch( std::exception& e )
   {
      Alert( e.what() );
      return false;
   }
   catch( ... )
   {
      Alert( "Unknown Exception" );
      return false;
   }
   SetHeader();
   std::string temp = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + std::string("\n");
   file.write( temp.c_str(), temp.size() );
   temp = "<xtm>" + std::string("\n");
   file.write( temp.c_str(), temp.size() );
   m_Header.writeXTM( file, "\t" );
   WriteSpecialDataTypesXTM( file, "\t" );
   WriteSpecialDataXTM( file, "\t" );
   WriteTilesXTM( file, "\t" );
   temp = "</xtm>";
   file.write( temp.c_str(), temp.size() );
   file.close();
   return true;
}

bool TileMap::read( std::string filename )
{
   clear();
   std::ifstream file;
   try
   {
      file.open( filename.c_str(), std::ios::in | std::ios::binary );
      if( !file.is_open() )
         return false;
   }
   catch( std::exception& e )
   {
      Alert( e.what() );
      return false;
   }
   catch( ... )
   {
      Alert( "Unknown Exception" );
      return false;
   }
   m_Header.read( file );
   LoadTileSet( m_Header.tileset_file_name );
   ReadTiles( file );
   ReadSpecialDataTypes( file );
   ReadSpecialData( file );
   file.close();
   return true;
}

bool TileMap::WriteTiles( std::ofstream& file )
{
   try
   {
      for( int r=0; r<int(m_Tiles.size()) ; r++ )
      {
         for( int c=0; c<int(m_Tiles[r].size()) ; c++ )
         {
            if( !m_Tiles[r][c].write( file ) )
            {
                // ... some error
            }
         }
      }
   
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

bool TileMap::WriteTilesXTM( std::ofstream& file, std::string tabs )
{
   try
   {
      std::string temp = tabs + "<MapData>" + "\n";
      file.write( temp.c_str(), temp.size() );

      std::stringstream ss;
      std::string       str;

      for( int r=0; r<int(m_Tiles.size()) ; r++ )
      {
         temp = tabs + tabs + "<r>" + "\n";
		 file.write( temp.c_str(), temp.size() );
         for( int c=0; c<int(m_Tiles[r].size()) ; c++ )
         {
            ss.clear();
            str.clear();
			ss << m_Tiles[r][c].m_Solidity;
			ss >> str;
			temp = tabs + tabs + "<c d=\"" + str + "\"";

            ss.clear();
            str.clear();
			ss << m_Tiles[r][c].m_TileGraphic;
			ss >> str;
			temp += std::string(" g=\"") + str + "\" />" + "\n";
			file.write( temp.c_str(), temp.size() );
         }
         temp = tabs + tabs + "</r>" + "\n";
         file.write( temp.c_str(), temp.size() );
      }
      temp = tabs + "</MapData>" + "\n";
      file.write( temp.c_str(), temp.size() );

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


bool TileMap::ReadTiles( std::ifstream& file )
{
   try
   {
      m_Tiles.reserve( m_Header.height );
      std::vector<Tile> tempoary( m_Header.width, Tile() );
      for( int i=0; i<m_Header.height ; i++ )
      {
         m_Tiles.push_back( tempoary );
      }

      for( int r=0; r<int(m_Tiles.size()) ; r++ )
      {
         for( int c=0; c<int(m_Tiles[r].size()) ; c++ )
         {
            if( !m_Tiles[r][c].read( file ) )
            {
                // ... some error
            }
         }
      }
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

void TileMap::SetHeader()
{
   if( m_Tiles.size() )
   {
      if( m_Tiles[0].size() ) // this assumes that all the rows have the same number of colums
      {
         m_Header.width  = m_Tiles[0].size();
         m_Header.height = m_Tiles.size();
      }
   }
}

bool TileMap::resize( int width, int height )
{
   try
   {
      m_Tiles.resize( height );
      for( int i=0; i<int(m_Tiles.size()) ; i++ )
      {
         m_Tiles[i].resize( width );
      }
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

bool TileMap::WriteSpecialDataTypes( std::ofstream& file )
{
   try
   {
      std::deque< TileMapSpecialDataTypes >::size_type num_of_sdts = m_SpecialDataTypes.size();
      file.write( (char*)&num_of_sdts, sizeof(num_of_sdts) );
      for( int i=0; i<int(m_SpecialDataTypes.size()) ; i++ )
      {
         m_SpecialDataTypes[i].write( file );
      }
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

bool TileMap::ReadSpecialDataTypes( std::ifstream& file )
{
   try
   {
      std::deque< TileMapSpecialDataTypes >::size_type num_of_sdts = 0;
      file.read( (char*)&num_of_sdts, sizeof(num_of_sdts) );
      m_SpecialDataTypes.resize( num_of_sdts );
      for( int i=0; i<int(m_SpecialDataTypes.size()) ; i++ )
      {
         m_SpecialDataTypes[i].read( file );
      }
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

bool TileMap::WriteSpecialData( std::ofstream& file )
{
   try
   {
      std::deque< TileMapSpecialData >::size_type num_of_sd = m_SpecialData.size();
      file.write( (char*)&num_of_sd, sizeof( num_of_sd ) );
      for( int i=0; i<int(m_SpecialData.size()) ; i++ )
      {
         m_SpecialData[i].write( file );
      }
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

bool TileMap::WriteSpecialDataTypesXTM( std::ofstream& file, std::string tabs )
{
   try
   {
      std::deque< TileMapSpecialDataTypes >::size_type num_of_sdts = m_SpecialDataTypes.size();

	  std::string temp = tabs + "<SpecialDataTypes>" + "\n";
      file.write( temp.c_str(), temp.size() );

      std::stringstream ss;
      std::string str;
      for( int i=0; i<int(m_SpecialDataTypes.size()) ; i++ )
      {
         ss.clear();
         str.clear();
		 ss << m_SpecialDataTypes[i].id;
		 ss >> str;
         temp = tabs + tabs + "<sdt name=\"" + m_SpecialDataTypes[i].name + "\" id=\"" + str + "\" />" + "\n";
         file.write( temp.c_str(), temp.size() );
      }
	  temp = tabs + "</SpecialDataTypes>" + "\n";
	  file.write( temp.c_str(), temp.size() );
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

bool TileMap::WriteSpecialDataXTM( std::ofstream& file, std::string tabs )
{
   try
   {
      std::deque< TileMapSpecialData >::size_type num_of_sd = m_SpecialData.size();
      //file.write( (char*)&num_of_sd, sizeof( num_of_sd ) );

      std::string temp = tabs + "<SpecialData>" + "\n";
      file.write( temp.c_str(), temp.size() );

      std::stringstream ss;
      std::string str;
      for( int i=0; i<int(m_SpecialData.size()) ; i++ )
      {
	     ss.clear();
         str.clear();
		 ss << m_SpecialData[i].y;
		 ss >> str;
		 temp = tabs + tabs + "<sd r=\"" + str + "\"";

	     ss.clear();
         str.clear();
		 ss << m_SpecialData[i].x;
		 ss >> str;
		 temp +=  " c=\"" + str + "\"";

	     ss.clear();
         str.clear();
		 ss << m_SpecialData[i].value;
		 ss >> str;
         temp += " id=\"" + str + "\" />" + "\n";
         file.write( temp.c_str(), temp.size() );
      }
	  temp = tabs + "</SpecialData>" + "\n";
	  file.write( temp.c_str(), temp.size() );

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

bool TileMap::ReadSpecialData( std::ifstream& file )
{
   try
   {
      std::deque< TileMapSpecialData >::size_type num_of_sd = 0;
      file.read( (char*)&num_of_sd, sizeof( num_of_sd ) );
      m_SpecialData.resize( num_of_sd );
      for( int i=0; i<int(m_SpecialData.size()) ; i++ )
      {
         m_SpecialData[i].read( file );
      }
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
