#include "main.h"

//***************************************************************************
//******************************* MainWindow::SaveHighScores ****************
// Accepts: void
//
// Purpose: Load the sounds
//
// Returns: void

bool MainWindow::SaveHighScores( void )
{
   int index = 0;
   std::list< std::string > row;
   std::string line;

   std::ofstream HighScoresFile( "data/HighScores.txt", std::ios::out );
   if( !HighScoresFile.is_open() )
      return false;

   row = m_HighScoresListBox.GetRow( index );
   while( row.size() )
   {
      line = row.front() + "=" + row.back();
      HighScoresFile << line.c_str();
      HighScoresFile << std::endl;
      index++;
      row = m_HighScoresListBox.GetRow( index );
   }
   HighScoresFile.close();
   return true;
}

//***************************************************************************
//******************************* MainWindow::SaveConfig ********************
// Accepts: void
//
// Purpose: Load the sounds
//
// Returns: void

bool MainWindow::SaveConfig( void )
{
   std::ofstream ConfigFile( "data/config.txt", std::ios::out );
   if( !ConfigFile.is_open() )
      return false;

   m_MasterServerHost = "";
   ConfigFile << "nick="              << m_Nick              << std::endl;
   ConfigFile << "port="              << m_Port              << std::endl;
   ConfigFile << "masterserverhost="  << m_MasterServerHost  << std::endl;
   ConfigFile << "remotehostaddress=" << m_RemoteHostAddress << std::endl;
   ConfigFile << "masterserverpath="  << m_MasterServerPath  << std::endl;

   ConfigFile.close();
   return true;
}



