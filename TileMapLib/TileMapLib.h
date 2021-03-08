#ifndef __TileMapLib_H
#define __TileMapLib_H

#include <map>
#include <string>
#include <queue>
#include <fstream>
//#include <unistd.h>
#include "../CHBITMAP/CHBITMAP.h"
#include "../Tile/Tile.h"
#include "../Structs/Structs.h"
#include "../errorutil/errorutil.h"
#include "../AStar/AStar2d.h"
#include "../C_SpriteMap2D/C_SpriteMap2D.h"
#include "../C_Sprite/C_Sprite.h"
//#include "../NonStandardHeaders/boost/tokenizer.hpp"

#pragma warning( disable : 4561 ) 
//disable some stupid warning messages
// __fastcall' incompatible with the '/clr' option: converting to '__stdcall'
#include <boost/tokenizer.hpp>

namespace TileMapLib
{

class C_TileMap
{
   public:
      C_TileMap() :
         m_TileSet(),
         m_Tiles(),
         m_OrignalTilesSelected(),
         m_TilesSelected(),
         m_Bitmaps(),
         m_MapFileName(),
         m_TileSetFileName(),
         m_MapWorkingDirectory(),
         m_TileSetWorkingDirectory(),
         m_Hwnd( NULL ),
         m_TileHeight( 32 ), 
         m_TileWidth( 32 ),
         m_StartDrawTile(),
         m_StartDrawTileOffSet(),
         m_EndDrawTile(),
         m_EndDrawTileOffSet(),
         m_AStarStartPos( -1, -1 ),
         m_AStarEndPos( -1, -1 ),
         m_bShowPath( true ),
         m_bShowNodesSearched( false ),
         m_bAllowDiagonal( false ),
         m_bShowStartAndEndNode( true ),
         m_AStarPath(),
         m_Solidity( 1000 ),
         m_SelectedTiles(),
         m_AStar2d(),
         m_GraphicSolidityMap(),
         m_SpriteMap(),
         m_Sprites()
      {
         m_AStarPath.reserve( 32 );
         m_Tiles.reserve( 32 );
         m_OrignalTilesSelected.reserve( 32 );
         m_TilesSelected.reserve( 32 );

      }

      C_TileMap( const C_TileMap& rhs ) :
         m_TileHeight(              rhs.m_TileHeight    ), 
         m_TileWidth(               rhs.m_TileWidth     ),
         m_Solidity(                rhs.m_Solidity      ),
         m_AStarStartPos(           rhs.m_AStarStartPos ),
         m_AStarEndPos(             rhs.m_AStarEndPos   ),
         m_bShowPath(               rhs.m_bShowPath            ),
         m_bShowNodesSearched(      rhs.m_bShowNodesSearched   ),
         m_bAllowDiagonal(          rhs.m_bAllowDiagonal       ),
         m_bShowStartAndEndNode(    rhs.m_bShowStartAndEndNode ),
         m_GraphicSolidityMap(      rhs.m_GraphicSolidityMap   ),
         m_Hwnd(                    rhs.m_Hwnd                 ),
         m_SelectedTiles(           rhs.m_SelectedTiles        ),
         m_OrignalTilesSelected(    rhs.m_OrignalTilesSelected ),
         m_TilesSelected(           rhs.m_TilesSelected        ),
         m_Tiles(                   rhs.m_Tiles   ),
         m_AStar2d(                 rhs.m_AStar2d ),
         m_TileSet(                 rhs.m_TileSet ),
         m_Bitmaps(                 rhs.m_Bitmaps ), // fix
         m_MapFileName(             rhs.m_MapFileName     ),
         m_TileSetFileName(         rhs.m_TileSetFileName ),
         m_AStarPath(               rhs.m_AStarPath       ),
         m_EndDrawTile(             rhs.m_EndDrawTile     ),
         m_EndDrawTileOffSet(       rhs.m_EndDrawTileOffSet ),
         m_StartDrawTile(           rhs.m_StartDrawTile     ),
         m_StartDrawTileOffSet(     rhs.m_StartDrawTileOffSet ),
         m_SpriteMap(               rhs.m_SpriteMap           ),
         m_MapWorkingDirectory(     rhs.m_MapWorkingDirectory ),
         m_TileSetWorkingDirectory( rhs.m_TileSetWorkingDirectory ),
         m_Sprites(                 rhs.m_Sprites )
      {
      }

      C_TileMap& operator = ( const C_TileMap& rhs )
      {
         m_TileHeight              = rhs.m_TileHeight;
         m_TileWidth               = rhs.m_TileWidth;
         m_Solidity                = rhs.m_Solidity;
         m_AStarStartPos           = rhs.m_AStarStartPos;
         m_AStarEndPos             = rhs.m_AStarEndPos;
         m_bShowPath               = rhs.m_bShowPath;
         m_bShowNodesSearched      = rhs.m_bShowNodesSearched;
         m_bAllowDiagonal          = rhs.m_bAllowDiagonal;
         m_bShowStartAndEndNode    = rhs.m_bShowStartAndEndNode;
         m_GraphicSolidityMap      = rhs.m_GraphicSolidityMap;
         m_Hwnd                    = rhs.m_Hwnd;
         m_SelectedTiles           = rhs.m_SelectedTiles;
         m_OrignalTilesSelected    = rhs.m_OrignalTilesSelected;
         m_TilesSelected           = rhs.m_TilesSelected;
         m_Tiles                   = rhs.m_Tiles;
         m_AStar2d                 = rhs.m_AStar2d;
         m_TileSet                 = rhs.m_TileSet;
         m_Bitmaps                 = rhs.m_Bitmaps; // fixed I think
         m_MapFileName             = rhs.m_MapFileName;
         m_TileSetFileName         = rhs.m_TileSetFileName;
         m_AStarPath               = rhs.m_AStarPath;
         m_EndDrawTile             = rhs.m_EndDrawTile;
         m_EndDrawTileOffSet       = rhs.m_EndDrawTileOffSet;
         m_StartDrawTile           = rhs.m_StartDrawTile;
         m_StartDrawTileOffSet     = rhs.m_StartDrawTileOffSet;
         m_SpriteMap               = rhs.m_SpriteMap;
         m_MapWorkingDirectory     = rhs.m_MapWorkingDirectory;
         m_TileSetWorkingDirectory = rhs.m_TileSetWorkingDirectory;
         m_Sprites                 = rhs.m_Sprites;
         return *this;
      }

      ~C_TileMap()
      {
         Destroy();
      }

      typedef std::deque<CHBITMAP> HBitmapVector;
      struct SelectedTiles
      {
         SelectedTiles() : StartX( -1 ), EndY( -1 ), StartY( -1 ), EndX( -1 )
         {}
         SelectedTiles( const SelectedTiles& rhs )
          : StartX( rhs.StartX ), EndY( rhs.EndY ), StartY( rhs.StartY ), EndX( rhs.EndX )
         {
         }
         SelectedTiles& operator = ( const SelectedTiles& rhs )
         {
            StartX = rhs.StartX;
            EndY   = rhs.EndY;
            StartY = rhs.StartY;
            EndX   = rhs.EndX;   
			return *this;
         }
         int StartX, EndY,
             StartY, EndX;
      };

      void SetHwnd( HWND Hwnd )
      {
         m_Hwnd = Hwnd;
      }

      void SetMapWorkingDirectory( std::string new_WorkingDirectory )
      {
         m_MapWorkingDirectory = new_WorkingDirectory;
      }

      std::string GetMapWorkingDirectory( void )
      {
         return m_MapWorkingDirectory;
      }

      void SetTileSetWorkingDirectory( std::string new_WorkingDirectory )
      {
         m_TileSetWorkingDirectory = new_WorkingDirectory;
      }

      std::string GetTileSetWorkingDirectory( void )
      {
         return m_TileSetWorkingDirectory;
      }

      void CalculateAStarPath( void );
      void ReLoadTile( int index );
      void ResizeRow( int index, int width );
      void ResizeMap( int width, int height );
      bool LoadMap( std::string filename );
      bool SaveMap( std::string filename );
      bool AddBitmap( std::string filename );
      void RemoveBitmap( int index );
      void RemoveBitmap( HBitmapVector::iterator index );
      bool DeleteBitmaps( void );
      bool DeleteTiles( void );
      bool ValidateTiles( void );

      bool LoadTileSet( std::string filename );
      bool SaveTileSet( std::string filename );

      bool LoadSpriteMap( std::string filename );
      bool SaveSpriteMap( std::string filename );

      void Destroy( void );
      void MakeNewMap( int width, int height );
      bool AddTile( std::string filename );
      void DeleteOrignalSelectedTiles( void );
      void DeSelectedTiles( void );
      void CutSelectedTiles( void );
      void CopySelectedTiles( void );
      void DeleteSelectedTiles( void );
      void PasteSelectedTiles( int start_row, int start_col );
      bool IsSelected( int row, int col );
      void GetSelectedTiles( Structs::Point m_MousePosition, Structs::Point m_RButtonDownStartPoint );

      bool LeftTile( Structs::Point tile )
      {
         if( !width() || !height() )
            return false;
         if( tile.x-1 >= 0 && tile.x-1 < width() )
            return true;
         return false;
      }

      bool RightTile( Structs::Point tile )
      {
         if( !width() || !height() )
            return false;
         if( tile.x+1 >= 0 && tile.x+1 < width() )
            return true;
         return false;
      }

      bool UpTile( Structs::Point tile )
      {
         if( !width() || !height() )
            return false;
         if( tile.y-1 >= 0 && tile.y-1 < height() )
            return true;
         return false;
      }

      bool DownTile( Structs::Point tile )
      {
         if( !width() || !height() )
            return false;
         if( tile.y+1 >= 0 && tile.y+1 < height() )
            return true;
         return false;
      }

      bool ShowStartAndEndNode( void )
      {
         return m_bShowStartAndEndNode;
      }

      void SetShowStartAndEndNode( bool bShowStartAndEndNode )
      {
         m_bShowStartAndEndNode = bShowStartAndEndNode;
      }

      bool showpath( void )
      {
         return m_bShowPath;
      }

      void setshowpath( bool bShowPath )
      {
         m_bShowPath = bShowPath;
      }

      bool shownodessearched( void )
      {
         return m_bShowNodesSearched;
      }

      void setshownodessearched( bool bShowNodesSearched )
      {
         m_bShowNodesSearched = bShowNodesSearched;
      }

      void SetMapFileName( std::string filename )
      { m_MapFileName = filename; }

      void SetTileSetFileName( std::string filename )
      { m_TileSetFileName = filename; }

      std::string GetMapFileName( void )
      { return m_MapFileName; }

      std::string GetTileSetFileName( void )
      { return m_TileSetFileName; }

      int maxsolidity( void ) const
      {
         return m_Solidity;
      }

      int tileheight( void ) const
      {
         return m_TileHeight;
      }

      int tilewidth( void )
      {
         return m_TileWidth;
      }

      int height( void )
      {
         return m_Tiles.size();
      }

      int width( void )
      {
         if( m_Tiles.size() )
            return m_Tiles[0].size();
         return 0;
      }

      C_TileMap::HBitmapVector& Bitmaps( void )
      {
         return m_Bitmaps;
      }

      AStar::AStar2d::TileVector2D::reference operator[]( int row )
      {
         return m_Tiles[ row ];
      }

//    Starting Tile And, Offset from begining of the first tile
      Structs::Point GetStartDrawTile( void ) const
      {
         return m_StartDrawTile;
      }

      Structs::Point GetStartDrawTileOffSet( void ) const
      {
         return m_StartDrawTileOffSet;
      }

      void SetStartDrawTile( Structs::Point newstarttile )
      {
         m_StartDrawTile = newstarttile;
      }

      void SetStartDrawTileOffSet( Structs::Point newoffset )
      {
         m_StartDrawTileOffSet = newoffset;
      }
//

//    Ending Tile And, Offset from begining of the last tile
      Structs::Point GetEndDrawTile( void ) const
      {
         return m_EndDrawTile;
      }

      Structs::Point GetEndDrawTileOffSet( void ) const
      {
         return m_EndDrawTileOffSet;
      }

      void SetEndDrawTile( Structs::Point newEndtile )
      {
         m_EndDrawTile = newEndtile;
      }

      void SetEndDrawTileOffSet( Structs::Point newoffset )
      {
         m_EndDrawTileOffSet = newoffset;
      }
//

      Structs::Point GetAStarStartPos( void  )
      {
         return m_AStarStartPos;
      }

      Structs::Point GetAStarEndPos( void )
      {
         return m_AStarEndPos;
      }

      void SetAStarStartPos( Structs::Point AStarStartPos )
      {
         m_AStarStartPos = AStarStartPos;
      }

      void SetAStarEndPos( Structs::Point AStarEndPos )
      {
         m_AStarEndPos = AStarEndPos;
      }

      void SetTileGraphic( int row, int col, int TileGraphic )
      {
         if( TileGraphic >= int(m_Bitmaps.size()) )
            return;
         if( row < height() )
            m_Tiles[ row ][ col ].m_TileGraphic = TileGraphic;
      }

      std::vector<AStar::AStar2d::PointAndDirection>& GetAStarPath( void )
      {
         return m_AStarPath;
      }

      std::vector<AStar::AStar2d::PointAndDirection>& GetAStarNodesSearched( void )
      {
         return m_AStar2d.GetNodesSearched();
      }

      void IncrementTileGraphic( int row, int col )
      {
         if( row < height() )
         {
            m_Tiles[ row ][ col ].m_TileGraphic++;
            if( m_Tiles[ row ][ col ].m_TileGraphic >= int(m_Bitmaps.size()) )
               m_Tiles[ row ][ col ].m_TileGraphic = 0;
         }
      }

      int GetSolidity( int graphic )
      {
         std::map< int, int >::iterator i = m_GraphicSolidityMap.find( graphic );
         // If the solidity is not found return 0
         return ( i != m_GraphicSolidityMap.end() ) ? i->second : -1;
      }

      void SetSolidity( int graphic, int solidity )
      {
         if( solidity <= maxsolidity() )
            m_GraphicSolidityMap[ graphic ] = solidity;
      }

      void SetSolidityForAllTilesWithThisGraphic( int graphic, int solidity )
      {
         SetSolidity( graphic, solidity );
         for( int r = 0; r < int(m_Tiles.size()); r++ )
         {
            for( int c = 0; c < int(m_Tiles[r].size()) ; c++ )
            {
               if( m_Tiles[r][c].m_TileGraphic == graphic )
               {
                  m_Tiles[r][c].m_Solidity = solidity;
               }
            }
         }
      }

      AStar::AStar2d::SearchStats GetAStarSearchStats( void );

      void SetAllowDiagonal( bool bAllowDiagonal )
      {
         m_bAllowDiagonal = bAllowDiagonal;
      }

      bool GetAllowDiagonal( void )
      {
         return m_bAllowDiagonal;
      }

      void SetMaxSearchPercentage( float max_search_percentage )
      {
         m_AStar2d.SetMaxSearchPercentage( max_search_percentage );
      }

      float GetMaxSearchPercentage( void )
      {
         return m_AStar2d.GetMaxSearchPercentage();
      }

      AStar::AStar2d::TileVector2D& GetTiles( void )
      {
         return m_Tiles;
      }

      operator AStar::AStar2d::TileVector2D& ( void )
      {
         return m_Tiles;
      } 

      SpriteMap::C_SpriteMap2D& GetSpriteMap( void )
      {
         return m_SpriteMap;
      }

      SpriteMap::D_HBitmaps& GetSpriteBitmaps( void )
      {
         return m_SpriteMap.GetBitmaps();
      }

      SpriteMap::D_Sprite & GetSpriteIDs( void )
      {
         return m_SpriteMap.GetSprites();
      }

      std::deque< C_Sprite >& GetSprites( void )
      {
         return m_Sprites;
      }

      void AddSprite( void )
      {
         m_Sprites.push_back( C_Sprite() );
      }

      void RemoveSprite( void )
      {
         m_Sprites.pop_back();
      }

      std::vector< std::string >& GetTileSet( void )
      {
         return m_TileSet;
      }

	  void Setm_TileSetWorkingDirectory( std::string TileSetWorkingDirectory )
	  {
		 m_TileSetWorkingDirectory = TileSetWorkingDirectory;
	  }

	  void Setm_Solidity( int Solidity )
	  {
         m_Solidity = Solidity;
	  }

   //protected:
      std::vector< std::string >   m_TileSet;

      AStar::AStar2d::TileVector2D m_Tiles,
                                   m_OrignalTilesSelected,
                                   m_TilesSelected;

      HBitmapVector              m_Bitmaps;

      std::string                m_MapFileName,
                                 m_TileSetFileName,
                                 m_MapWorkingDirectory,
                                 m_TileSetWorkingDirectory;

      HWND                       m_Hwnd;

      int                        m_TileHeight, 
                                 m_TileWidth;

      Structs::Point           m_StartDrawTile,
                                 m_StartDrawTileOffSet,
                                 m_EndDrawTile,
                                 m_EndDrawTileOffSet,
                                 m_AStarStartPos,
                                 m_AStarEndPos;

      bool                       m_bShowPath,
                                 m_bShowNodesSearched,
                                 m_bAllowDiagonal,
                                 m_bShowStartAndEndNode;

      std::vector<AStar::AStar2d::PointAndDirection> m_AStarPath;

      int                      m_Solidity;
      SelectedTiles            m_SelectedTiles;
      AStar::AStar2d           m_AStar2d;
      std::map< int, int >     m_GraphicSolidityMap;
      SpriteMap::C_SpriteMap2D m_SpriteMap;
      std::deque< C_Sprite >   m_Sprites;

   private:
};

};

#endif

