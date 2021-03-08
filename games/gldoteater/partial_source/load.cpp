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
   m_Mutex.Lock();
   m_TileMap.clear();
   m_Mutex.Release();

   m_Mutex.Lock();

   ::chdir( m_DataDirectory.c_str() );
   if( !m_TileMap.read( map ) )
   {
      Alert( "Could not load map" + map );
      ::exit( -1 );
      return false;
   }
   m_MessageQueue.AddLine( "Loaded Tile Map: " + map );
   ::chdir( m_AppWorkingDirectory.c_str() );
   m_Mutex.Release();
   Reshape( (long)m_Width, (long)m_Height );
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
   m_Mutex.Lock();
   ::chdir( m_DataDirectory.c_str() );
/*
   m_TextureManager.LoadGLTexturePNGFile( "texture\\die.png" ); //     RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\die2.png" ); //    RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\die3.png" ); //    RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\deater.png" ); //  RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\deater2.png" ); // RGB( 0, 0, 0 ) );

   m_TextureManager.LoadGLTexturePNGFile( "texture\\p2die.png" ); //     RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\p2die2.png" ); //    RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\p2die3.png" ); //    RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\p2deater.png" ); //  RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\p2deater2.png" ); // RGB( 0, 0, 0 ) );

   m_TextureManager.LoadGLTexturePNGFile( "texture\\dot.png" );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\bigdot.png" ); //     RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\arrow.png" ); //      RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\ip.png" ); //         RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\start.png" ); //      RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\reconnect.png" ); //  RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\retry.png" ); //      RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\back.png" ); //       RGB( 0, 0, 0 ) );

   m_TextureManager.LoadGLTexturePNGFile( "texture\\wghost.png" ); //  RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\bghost.png" ); //  RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\rghost.png" ); //  RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\yghost.png" ); //  RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\gghost.png" ); //  RGB( 0, 0, 0 ) );
   m_TextureManager.LoadGLTexturePNGFile( "texture\\dghost.png" ); //  RGB( 0, 0, 0 ) );
   */
   m_RandomItemTextureMap.clear();
   m_RandomItemTextureMap[ 0 ] = "texture\\banana.png";
//   m_TextureManager.LoadGLTexturePNGFile( "texture\\banana.png" ); //     RGB( 0, 0, 0 ) );
   m_RandomItemTextureMap[ 1 ] = "texture\\cherry.png";
//   m_TextureManager.LoadGLTexturePNGFile( "texture\\cherry.png" ); //     RGB( 0, 0, 0 ) );
   m_RandomItemTextureMap[ 2 ] = "texture\\blindfold.png";
//   m_TextureManager.LoadGLTexturePNGFile( "texture\\blindfold.png" ); //  RGB( 255, 255, 255 ) );
   m_RandomItemTextureMap[ 3 ] = "texture\\grapes.png";
//   m_TextureManager.LoadGLTexturePNGFile( "texture\\grapes.png" ); //  RGB( 0, 0, 0 ) );
   m_RandomItemTextureMap[ 4 ] = "texture\\mushroom.png";
//   m_TextureManager.LoadGLTexturePNGFile( "texture\\mushroom.png" ); //  RGB( 0, 0, 0 ) );

   //m_NumberOfInventoryTextures = m_TextureManager.size();
   ::chdir( m_AppWorkingDirectory.c_str() );
   m_Mutex.Release();
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
   m_Mutex.Lock();
   m_Fmod.LoadClip( "data/sounds/pacman.wav" );
   m_Fmod.LoadClip( "data/sounds/die.wav" );
   m_Fmod.LoadClip( "data/sounds/chomp.wav" );
   m_Fmod.LoadClip( "data/sounds/chomp2.wav" );
   m_Fmod.LoadClip( "data/sounds/eatghost.wav" );
   m_Fmod.LoadClip( "data/sounds/lifeup.wav" );
   m_Fmod.LoadClip( "data/sounds/connect.wav" );
   m_Fmod.LoadClip( "data/sounds/randomitem.wav" );
   m_Mutex.Release();
   return true;
}

//***************************************************************************
//******************************* MainWindow::LoadHighScores ****************
// Accepts: void
//
// Purpose: Load the scores
//
// Returns: void

bool MainWindow::LoadHighScores( void )
{
   std::ifstream HighScoresFile( "data/HighScores.txt", std::ios::in );
   if( !HighScoresFile.is_open() )
      return false;

   m_HighScoresListBox.clear();
   std::string line;
   std::list< std::string > Row;

   std::map< int, std::string, HighestFirst > HighScoreMap;

   while( !std::getline( HighScoresFile, line ).eof() )
   {
      int index         = line.find_last_of( '=', line.size()-1 ); // find the position of the last = character
      std::string name  = line.substr( 0, index ); // get the name
      line.erase( 0, index+1 ); // remove the name and the = character
      int         score = atoi( line.c_str() );
      HighScoreMap[ score ] = name;
      line.clear();
   }

   for( std::map< int, std::string, HighestFirst >::iterator i = HighScoreMap.begin();
        i != HighScoreMap.end() ;
        i++ )
   {
      std::ostringstream tmp;
      tmp << (*i).first;
      Row.push_back( (*i).second ); Row.push_back( tmp.str() ); 
      m_HighScoresListBox.AddRow( Row );
      Row.clear();
   }

   HighScoresFile.close();
   SortHighScores();
   return true;
}

//***************************************************************************
//******************************* MainWindow::LoadConfig ********************
// Accepts: void
//
// Purpose: Load the scores
//
// Returns: void

bool MainWindow::LoadConfig( void )
{
   std::ifstream ConfigFile( "data/config.txt", std::ios::in );
   if( !ConfigFile.is_open() )
      return false;

   std::string line;
   while( !std::getline( ConfigFile, line ).eof() )
   {
      int index         = line.find_last_of( '=', line.size()-1 ); // find the position of the last = character
      std::string variablename = line.substr( 0, index ); // get the variablename
      line.erase( 0, index+1 ); // remove the name and the = character

      if( variablename.find( "nick" ) == 0 )
      {
         if( line.length() )
         {
            if( line.length() > MAX_NICK_LENGHT )
            {
               m_Nick = line.substr( 0, MAX_NICK_LENGHT-1 );
            }
            else
            {
               m_Nick = line;
            }
         }
      }
      else if( variablename.find( "port" ) == 0 )
      {
         if( line.length() )
            m_Port = atoi( line.c_str() );
      }
      else if( variablename.find( "masterserverhost" ) == 0 )
      {
         if( line.length() )
            m_MasterServerHost = line;
      }
      else if( variablename.find( "remotehostaddress" ) == 0 )
      {
         if( line.length() )
            m_RemoteHostAddress = line;
      }
      else if( variablename.find( "masterserverpath" ) == 0 )
      {
         if( line.length() )
            m_MasterServerPath = line;
      }
      line.clear();
   }

   ConfigFile.close();
   return true;
}

//*****************************************************************************
//******************************* MainWindow::LoadMapsFilenames ***************
// Accepts: void
//
// Purpose: 
//
// Returns: void

bool MainWindow::LoadMapsFilenames( void )
{
   LoadFileNames( "data/maps.txt",             m_MapFilenames     );
   LoadFileNames( "data/singleplayermaps.txt", m_SinglePlayerMaps );
   LoadFileNames( "data/multiplayermaps.txt",  m_MultiPlayerMaps  );
   return true;
}


bool MainWindow::LoadFileNames( std::string filename, std::deque<std::string>& container )
{
   container.clear();
   std::ifstream maps( filename.c_str(), std::ios::in );
   if( !maps.is_open() )
      return false;

   std::string line;
   while( !std::getline( maps, line ).eof() )
   {
      container.push_back( line );
   }
   maps.close();
   return true;
}


//*****************************************************************************
//******************************* MainWindow::LoadQuickMessages ***************
// Accepts: void
//
// Purpose: 
//
// Returns: void

bool MainWindow::LoadQuickMessages( void )
{
   std::ifstream QuickMessages( "data/quickmessages.txt", std::ios::in );
   if( !QuickMessages.is_open() )
      return false;

   std::string line;
   while( !std::getline( QuickMessages, line ).eof() )
   {
      m_QuickMessages.push_back( line );
   }

   QuickMessages.close();
   return true;
}

bool MainWindow::LoadAnimations( void )
{
   for( int i=0; i<m_Players.size() ; i++ )
   {
      switch( i )
      {
         case 0:
         {
            m_Players[i].m_DotEatAnimation.read( "data/de1.sa" );
            m_Players[i].m_DieAnimation.read( "data/die.sa" );
         } break;
         case 1:
         {
            m_Players[i].m_DotEatAnimation.read( "data/p2de1.sa" );
            m_Players[i].m_DieAnimation.read( "data/p2die.sa" );
         } break;
      }
   }
   return true;
}
