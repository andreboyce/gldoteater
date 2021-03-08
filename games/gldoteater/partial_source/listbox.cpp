#include "main.h"

//*****************************************************************************
//******************************* MainWindow::HandleListBoxClickCollision *****
// Accepts: void
//
// Purpose: 
//
// Returns: void

bool MainWindow::HandleListBoxClickCollision( C_GLListBox& ListBox, Structs::Point MousePosition )
{
   m_Mutex.Lock();
   ::glFlush();
   bool ret = false;
   if( ListBox.ClickRow( Structs::Point( MousePosition.x, MousePosition.y ) ) )
   {
      ret = true;
   }
   ListBox.ClickUpArrow(   Structs::Point( MousePosition.x, MousePosition.y ) );
   ListBox.ClickDownArrow( Structs::Point( MousePosition.x, MousePosition.y ) );
//   m_Mutex.Release();
   return ret;
}

//*****************************************************************************
//******************************* MainWindow::HandleListBoxMouseOverCollision *
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::HandleListBoxMouseOverCollision( C_GLListBox& ListBox, Structs::Point MousePosition )
{
   m_Mutex.Lock();
   ::glFlush();
   ListBox.MouseOver( MousePosition );
   m_Mutex.Release();
}

//*****************************************************************************
//******************************* MainWindow::InitializeListBoxes *************
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::InitializeListBoxes( void )
{
   int arrow_texture = m_ArrowTexture;

   m_TitleScreenOptionsListBox.clear();
   m_TitleScreenOptionsListBox.SetArrowTexture( m_TextureManager["texture\\arrow.png"] );

   m_ChooseClientOrServerListBox.clear();
   m_ChooseClientOrServerListBox.SetArrowTexture( m_TextureManager["texture\\arrow.png"] );

   m_HighScoresListBox.clear();
   m_HighScoresListBox.SetArrowTexture( m_TextureManager["texture\\arrow.png"] );

   m_GamesListBox.clear();
   m_GamesListBox.SetArrowTexture( m_TextureManager["texture\\arrow.png"] );

   std::list< std::string > Header;
   std::list< std::string > Row;

   Header.push_back( "Version  " );
   Header.push_back( "Nick     " );
   Header.push_back( "IP       " );
   Header.push_back( "Port     " );
   Header.push_back( "Ping     " );
   m_GamesListBox.SetHeader( Header );
   Header.clear();

   m_TitleScreenOptionsListBox.SetHeader( Header );
   Row.push_back( "Help"       ); m_TitleScreenOptionsListBox.AddRow( Row ); Row.clear();
   Row.push_back( "1p"         ); m_TitleScreenOptionsListBox.AddRow( Row ); Row.clear();
   Row.push_back( "2p"         ); m_TitleScreenOptionsListBox.AddRow( Row ); Row.clear();
   Row.push_back( "2p online" ); m_TitleScreenOptionsListBox.AddRow( Row ); Row.clear();
   Row.push_back( "HighScores" ); m_TitleScreenOptionsListBox.AddRow( Row ); Row.clear();
   Header.clear();

   m_ChooseClientOrServerListBox.SetHeader( Header );
   Row.push_back( "Connect to a Server"     ); m_ChooseClientOrServerListBox.AddRow( Row ); Row.clear();
   Row.push_back( "Act as a Server"         ); m_ChooseClientOrServerListBox.AddRow( Row ); Row.clear();
   Header.clear();

   Header.push_back( "Name  " );
   Header.push_back( "Score     " );
   m_HighScoresListBox.SetHeader( Header );
   Header.clear();
}

