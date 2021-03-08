#include "main.h"

//***************************************************************************
//******************************* MainWindow::LoadMap ***********************
// Accepts: the location of the map
//
// Purpose: load a tilemap
//
// Returns: void

bool MainWindow::LoadMap( std::string map )
{
   m_MapTexturesIndex = m_TextureManager.Size();
   m_TileMap.LoadMap( map );
   if( !m_TileMap.LoadMap( map ) )
   {
      Alert( "Could not load map" );
      return false;
   }
   
   std::vector<std::string> mapTiles = m_TileMap.GetTileSet();
   for (int i = 0; i < mapTiles.size(); i++)
   {
	   std::string filename = m_TileMap.m_TileSetWorkingDirectory + "/" + mapTiles[i];
	   m_TextureManager.LoadGLTextureBitmapFile( filename, RGB(255, 255, 255) );
   }

   // Fix this later
   //TileMapLib::C_TileMap::HBitmapVector& hbm = m_TileMap.Bitmaps();
   //for( int i=0; i<hbm.size(); i++ )
   //{
   //   m_TextureManager.LoadGLBitmapHandle( hbm[i] );
   //}
   return true;
}

//***************************************************************************
//******************************* MainWindow::LoadTextures ******************
// Accepts: void
//
// Purpose: load the textures
//
// Returns: void

bool MainWindow::LoadTextures( void )
{
   /*LONG result = 0;
   result = m_TextureManager.LoadGLTextureBitmapFile( "tanks/MainTank Full 64x64.bmp", RGB( 255, 255, 255 ) );
   if( ERROR_CNLT == result )
   {
      Alert( "tanks/MainTank Full 64x64.bmp" );
   }*/
   m_TextureManager.LoadGLTextureBitmapFile( "tanks/MainTank Full 64x64.bmp", RGB( 255, 255, 255 ) );
   m_TextureManager.LoadGLTextureBitmapFile( "tanks/Tank1 Full 64x64.bmp", RGB( 255, 255, 255 ) );
   m_TextureManager.LoadGLTextureBitmapFile( "tanks/Tank2 Full 64x64.bmp", RGB( 255, 255, 255 ) );
   m_TextureManager.LoadGLTextureBitmapFile( "tanks/Tank3 Full 64x64.bmp", RGB( 255, 255, 255 ) );
   m_TextureManager.LoadGLTextureBitmapFile( "tanks/Tank4 Full 64x64.bmp", RGB( 255, 255, 255 ) );
   m_TextureManager.LoadGLTextureBitmapFile( "shells/shell1.bmp", RGB( 255, 255, 255 ) );
   m_TextureManager.LoadGLTextureBitmapFile( "boss/BossTankA Full 64x64.bmp", RGB( 255, 255, 255 ) );
   m_TextureManager.LoadGLTextureBitmapFile( "explosions/explosion.bmp", RGB( 255, 255, 255 ) );
   return true;
}

//***************************************************************************
//******************************* MainWindow::LoadSounds ********************
// Accepts: void
//
// Purpose: Load the sounds
//
// Returns: void

bool MainWindow::LoadSounds( void )
{
   m_Fmod.LoadClip( "sounds/missile1.wav" );
   m_Fmod.LoadClip( "sounds/explos.wav" );
   m_Fmod.LoadClip( "sounds/firewrks.wav" );
   m_Fmod.LoadClip( "sounds/youwin.wav" );
   return true;
}

