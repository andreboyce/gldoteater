#include "main.h"

//***************************************************************************
//******************************* MainWindow::RenderConsole *****************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::RenderConsole()
{
   RenderMessages();
   if( m_bShowTheConsole )
   {
	  double x = m_TextBoundingRect.left+(m_TextBoundingRect.right-m_TextBoundingRect.left)*0.05;
      double y = 0; //-(m_TextBoundingRect.top-m_TextBoundingRect.bottom)*0.5;

      m_Mutex.Lock();
      std::vector< std::string > &temp = m_Console;
      int size = m_Console.LinesAdded();

      for( int i=0; i <= size ; i++, y-- )
      {
         m_Font.Print( temp[i], x, y, m_TextZ );
      }

      m_Font.Print( ">", x-1, y+1, m_TextZ );

      if( m_Console.GetFlash().Update() )
      {

         m_Font.Print( "|", x+m_Font.Length( m_Console.GetStringBehindCursor() ), y+1, m_TextZ );
      }
      m_Mutex.Release();
   }
}


//***************************************************************************
//******************************* MainWindow::RenderTransferRate ************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::RenderTransferRate()
{
   if( m_bNetworkActive )
   {
      m_Mutex.Lock();
      if( !m_TransferRateSStream.str().length() )
         m_TransferRateSStream << "0";

      m_Font.Print( "Rate: " + m_TransferRateSStream.str() + " kB/sec", 
                    m_TextBoundingRect.right - (m_TextBoundingRect.right-m_TextBoundingRect.left)*0.35, 
                    m_TextBoundingRect.top   - (m_TextBoundingRect.top-m_TextBoundingRect.bottom)*0.2, 
                    m_TextZ );
      m_Mutex.Release();
   }
}


//***************************************************************************
//******************************* MainWindow::RenderPing ********************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::RenderTotalDataSent()
{
   if( m_bNetworkActive )
   {
      m_Mutex.Lock();
      if( !m_TotalDataSentSStream.str().length() )
         m_TotalDataSentSStream << "0";

      m_Font.Print( "Total Data Sent: " + m_TotalDataSentSStream.str() + " kbytes", 
                    m_TextBoundingRect.right - (m_TextBoundingRect.right-m_TextBoundingRect.left)*0.35, 
                    m_TextBoundingRect.top   - (m_TextBoundingRect.top-m_TextBoundingRect.bottom)*0.15, 
                    m_TextZ );
      m_Mutex.Release();
   }
}

//***************************************************************************
//******************************* MainWindow::RenderPing ********************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::RenderPing()
{
   if( m_bNetworkActive )
   {
      m_Mutex.Lock();
      if( !m_PingSStream.str().length() )
         m_PingSStream << "0";

      m_Font.Print( "Ping: " + m_PingSStream.str() + " ms" , 
                    m_TextBoundingRect.right - (m_TextBoundingRect.right-m_TextBoundingRect.left)*0.35, 
                    m_TextBoundingRect.top   - (m_TextBoundingRect.top-m_TextBoundingRect.bottom)*0.1,                     
                    m_TextZ );
      m_Mutex.Release();
      RenderTotalDataSent();
      RenderTransferRate();
   }
}

//***************************************************************************
//******************************* MainWindow::RenderButtons *****************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::RenderButtons()
{
   if( m_TextureManager.size() )
   {
      m_Mutex.Lock();
      ::glEnable( GL_TEXTURE_2D );
      m_Mutex.Release();
   }
   m_Mutex.Lock();
   switch( m_GameMode )
   {
      case GAME_IN_PROGRESS:
      {
      } break;
      case TWO_PLAYER_MODE:
      {
      } break;
      case YOU_WIN_MODE:
      {
         m_StartButton.Draw();
      } break;
      case GAME_OVER:
      {
         m_StartButton.Draw();
         m_BackButton.Draw();
      } break;
      case VIEW_HIGH_SCORES:
      {
         m_BackButton.Draw();
      } break;
      case LISTEN_MODE:
      {
         if( m_bNetworkActive )
            m_StartButton.Draw();
         m_BackButton.Draw();
         m_WhatIsMyIPButton.Draw();
         if( m_bServerCreationFailed && !m_bProcessServerThread && !m_bNetworkThreadActive )
            m_RetryButton.Draw();
      } break;
      case CONNECT_MODE:
      {
         if( m_bNetworkActive )
            m_StartButton.Draw();
         m_BackButton.Draw();
         if( m_bConnectionFailed && !m_bProcessClientThread && !m_bNetworkThreadActive )
            m_RetryButton.Draw();
      } break;
      case CHOOSE_CLIENT_OR_SERVER_MODE:
      {
         m_BackButton.Draw();
      } break;
      case GET_SERVER_INFORMATION_FROM_USER:
      {
         m_BackButton.Draw();
      } break;
      case VIEW_STATS:
      {
         m_StartButton.Draw();
      } break;
   }
   ::glEnable( GL_BLEND );
   ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   m_Mutex.Release();
}

void MainWindow::RenderRandomItems()
{
   if( m_bRenderRandomItems )
   {
      ::glLoadIdentity();
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      ::glEnable( GL_TEXTURE_2D );

      if( m_RandomItems.size() )
      {
         C_TexCoord TexCoords[4];
         C_Vert     Verticies[4];
         ::glTranslatef( 0, 0, m_Z );
         ::glBegin( GL_TRIANGLES );
         for( int i=0; i<m_RandomItems.size(); i++ )
         {

			//m_Console.AddLine(m_RandomItems. );

            m_TextureManager.BindTexture( m_RandomItemTextureMap[m_RandomItems[i].id] );
            m_TextureManager.GetSubTextureCoords( m_RandomItemTextureMap[m_RandomItems[i].id], TexCoords );
            Structs::fPoint tile_pos = GetTilePos( m_RandomItems[i].x, m_RandomItems[i].y );
               m_MapTexturedQuad.GetVerticies( Verticies, tile_pos.m_x, tile_pos.m_y, m_Z );
               TexCoords[0].glTexCoord2f();
               Verticies[0].glVertex2f();

               TexCoords[1].glTexCoord2f();
               Verticies[1].glVertex2f();

               TexCoords[2].glTexCoord2f();
               Verticies[2].glVertex2f();

               TexCoords[0].glTexCoord2f();
               Verticies[0].glVertex2f();

               TexCoords[2].glTexCoord2f();
               Verticies[2].glVertex2f();

               TexCoords[3].glTexCoord2f();
               Verticies[3].glVertex2f();
         }
         ::glEnd();
      }
      ::glLoadIdentity();
   }
}

//***************************************************************************
//******************************* MainWindow::RenderChooseClientOrServer ****
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::RenderChooseClientOrServer()
{
   m_Mutex.Lock();
   //::glClear( GL_DEPTH_BUFFER_BIT );
   ::glLoadIdentity();
   m_Mutex.Release();

   if( m_TextureManager.size() )
   {
      m_Mutex.Lock();
      ::glEnable( GL_TEXTURE_2D );
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      m_Mutex.Release();
      RenderMap();
      RenderButtons();
   }
   m_Mutex.Lock();
   m_Mutex.Release();
   RenderConsole();
   RenderListboxes();
   RenderPing();


   m_Mutex.Lock();
   switch( m_ChooseClientOrServerListBox.GetSelected() )
   {
      case 0:
      {
         m_Font.Print( "This allows you to join a game",  -4.0, 0, m_TextZ );
      } break;
      case 1:
      {
         m_Font.Print( "This allows you to host a game",  -4.0, 0, m_TextZ );
         std::ostringstream tmp;
         tmp << "Port: " << m_Port << " must be open. You can change the port in config.txt";
         m_Font.Print( tmp.str(),  -8.0, -2.0, m_TextZ );
      } break;
   }
   m_Font.Print( "Backspace or Click BackArrow to cancel",  -2.0, -4.0, m_TextZ );
   m_Mutex.Release();
}


//***************************************************************************
//******************************* MainWindow::RenderListenMode **************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::RenderListenMode()
{
   m_Mutex.Lock();
   ////::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   //::glClear( GL_DEPTH_BUFFER_BIT );
   ::glLoadIdentity();
   m_Mutex.Release();

   if( m_TextureManager.size() )
   {
      m_Mutex.Lock();
      ::glEnable( GL_TEXTURE_2D );
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      m_Mutex.Release();
      RenderMap();
      RenderButtons();
   }
   m_Mutex.Lock();
   m_Mutex.Release();
   RenderConsole();
   RenderPing();
   m_Mutex.Lock();
   if( m_bNetworkActive )
      m_Font.Print( "Connected",       -2.0,  0.0, m_TextZ );
   else
      m_Font.Print( "Listening ...",       -2.0,  0.0, m_TextZ );

   m_Font.Print( "BackSpace or Click BackArrow to cancel",  -2.0, -4.0, m_TextZ );

//      std::ostringstream t4, t5;
//      t4 << m_LastPingSent.m_LocalTime;
//      t5 << m_LastPingRecv.m_LocalTime;
//      m_Font.Print( "m_LastPingSent.m_LocalTime: "     + t4.str(), m_TextBoundingRect.left+5, m_TextBoundingRect.bottom+6.5, m_TextZ );
//      m_Font.Print( "m_LastPingRecv.m_LocalTime: "     + t5.str(), m_TextBoundingRect.left+5, m_TextBoundingRect.bottom+7.5, m_TextZ );

   m_Mutex.Release();
   
}

//***************************************************************************
//******************************* MainWindow::RenderConnectMode *************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::RenderConnectMode()
{
   m_Mutex.Lock();
   ////::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   //::glClear( GL_DEPTH_BUFFER_BIT );
   ::glLoadIdentity();
   m_Mutex.Release();

   if( m_TextureManager.size() )
   {
      m_Mutex.Lock();
      ::glEnable( GL_TEXTURE_2D );
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      m_Mutex.Release();
      RenderMap();
      RenderButtons();
   }
   m_Mutex.Lock();
   m_Mutex.Release();
   RenderConsole();
   RenderPing();
   m_Mutex.Lock();
   if( m_bNetworkActive )
      m_Font.Print( "Connected",       -2.0,  0.0, m_TextZ );
   else
      m_Font.Print( "Connecting ...",       -2.0,  0.0, m_TextZ );

   m_Font.Print( "BackSpace or Click BackArrow to cancel",  -2.0, -4.0, m_TextZ );

//      std::ostringstream t4, t5;
//      t4 << m_LastPingSent.m_LocalTime;
//      t5 << m_LastPingRecv.m_LocalTime;
//      m_Font.Print( "m_LastPingSent.m_LocalTime: "     + t4.str(), m_TextBoundingRect.left+5, m_TextBoundingRect.bottom+6.5, m_TextZ );
//      m_Font.Print( "m_LastPingRecv.m_LocalTime: "     + t5.str(), m_TextBoundingRect.left+5, m_TextBoundingRect.bottom+7.5, m_TextZ );

   m_Mutex.Release();
}

//***************************************************************************
//******************************* MainWindow::RenderStatsMode ***************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::RenderStatsMode()
{
   m_Mutex.Lock();
   ////::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   //::glClear( GL_DEPTH_BUFFER_BIT );
   ::glLoadIdentity();
   m_Mutex.Release();

   if( m_TextureManager.size() )
   {
      m_Mutex.Lock();
      ::glEnable( GL_TEXTURE_2D );
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      m_Mutex.Release();
      RenderMap();
      RenderButtons();
   }
   m_Mutex.Lock();
   m_Mutex.Release();

   RenderListboxes();
   RenderConsole();
   RenderPing();
   RenderMousePos();
   m_Mutex.Lock();
   if( m_Players.size() > 0 )
   {
      std::ostringstream P1Score;
      P1Score << "Player 1 Score: " << m_Players[0].m_Score;
      m_Font.Print( P1Score.str(),  -6.0,  15.0, m_TextZ );
   }
   if( m_Players.size() > 1 )
   {
      std::ostringstream P2Score;
      P2Score << "Player 2 Score: " << m_Players[1].m_Score;
      m_Font.Print( P2Score.str(),  -6.0,  8.0, m_TextZ );

      char str[25];
      ::_snprintf_s( str, 25, 25, "Player %d Takes the Lead!", ( m_Players[0].m_Score > m_Players[1].m_Score ) ? 1 : 2 );
      m_Font.Print( str, -5.0, -3.0, m_TextZ );
   }
   std::ostringstream t;
   t << m_Level;
//   m_Font.Print( t.str(), -5.0, 3.0, m_TextZ );
   m_Mutex.Release();
}

//***************************************************************************
//******************************* MainWindow::RenderConsole *****************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::RenderMessages()
{
   if( m_bShowMessageQueue )
   {
      float x = m_TextBoundingRect.left+(m_TextBoundingRect.right-m_TextBoundingRect.left)*0.05;
      float y = m_TextBoundingRect.bottom+(m_TextBoundingRect.top-m_TextBoundingRect.bottom)*0.1;

      m_Mutex.Lock();
      std::vector< std::string > &temp = m_MessageQueue;
      m_MessageQueue.Update();

      int size = m_MessageQueue.LinesAdded();

      for( int i=0; i <= size ; i++, y-- )
      {
         m_Font.Print( temp[i], x, y, m_TextZ );
      }
      m_Mutex.Release();
   }
}

//***************************************************************************
//******************************* MainWindow::RenderListboxes ***************
// Accepts:
//
// Purpose:
//
// Returns:

void MainWindow::RenderListboxes()
{
   m_Mutex.Lock();
   switch( m_GameMode )
   {
      case TITLE_SCREEN:
      {
         m_TitleScreenOptionsListBox.Draw( 0, 15, m_TextZ );
      } break;
      case CHOOSE_CLIENT_OR_SERVER_MODE:
      {
         m_ChooseClientOrServerListBox.Draw( 0, 15, m_TextZ );
      } break;
      case GET_SERVER_INFORMATION_FROM_USER:
      {
         m_GamesListBox.Draw( -5.0, 15, m_TextZ );
      } break;
      case VIEW_HIGH_SCORES:
      {
         m_HighScoresListBox.Draw( -5.0, 17, m_TextZ );
      } break;
   }
   m_Mutex.Release();
}

void MainWindow::init_depth_buffer_trick( void )
{
   //::glClearDepth( 1.0f );
   //::glClear( GL_DEPTH_BUFFER_BIT );
   ::glDepthRange( 0.0f, 0.5f );
   ::glDepthFunc( GL_LESS );
   m_bDepthEvenFlag = true;
}

void MainWindow::depth_buffer_trick( void )
{
   if( m_bDepthEvenFlag )
   {
      ::glDepthFunc( GL_LESS );
      ::glDepthRange( 0.0f, 0.5f );
   }
   else
   {
      ::glDepthFunc( GL_GREATER );
      ::glDepthRange( 1.0f, 0.5f );
   }
   m_bDepthEvenFlag = !m_bDepthEvenFlag;
}

//***************************************************************************
//******************************* MainWindow::RenderTitleScreen *************
// Accepts: void
//
// Purpose: Render the title screen
//
// Returns: void

void MainWindow::RenderTitleScreen()
{
   m_Mutex.Lock();
   //::glEnable( GL_SCISSOR_TEST );
   //::glScissor( (int)m_Width, (int)m_Height, 0, 0 );
   //::glDisable( GL_SCISSOR_TEST );

   //::glClear( GL_DEPTH_BUFFER_BIT );
   //depth_buffer_trick();
   ::glLoadIdentity();
   m_Mutex.Release();

   if( m_TextureManager.size() )
   {
      m_Mutex.Lock();
      ::glEnable( GL_TEXTURE_2D );
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      m_Mutex.Release();
      RenderMap();
      RenderButtons();
   }
   m_Mutex.Lock();
   m_Mutex.Release();

   RenderListboxes();
   RenderConsole();
   RenderPing();
   RenderMousePos();
   m_Mutex.Lock();
   switch( m_TitleScreenOptionsListBox.GetSelected() )
   {
      case 0:
      {
         m_Font.Print( "The Object of the game is to eat all the dots.",  -5.0,  0.0, m_TextZ );
         m_Font.Print( "Press SpaceBar or Enter To Begin",                -5.0, -2.0, m_TextZ );
         m_Font.Print( "Press F11 To Toggle Sound",                       -5.0, -4.0, m_TextZ );
         m_Font.Print( "Press F12 To Toggle FullScreen",                  -5.0, -6.0, m_TextZ );
         m_Font.Print( "Press ` to toggle the chat",                      -5.0, -8.0, m_TextZ );
         m_Font.Print( "Press F2 to return to tile screen",               -5.0, -10.0, m_TextZ );
         m_Font.Print( "Press Escape to exit",                            -5.0, -12.0, m_TextZ );
      } break;
      case 1:
      {
         m_Font.Print( "1 player",                    -5.0,  0.0, m_TextZ );
         m_Font.Print( "Press SpaceBar or Enter To Begin", -5.0, -4.0, m_TextZ );
      } break;
      case 2:
      {
         m_Font.Print( "2 player [Fight for more points]", -5.0,  0.0, m_TextZ );
         m_Font.Print( "Player 2 Uses W A S D to move",    -5.0, -2.0, m_TextZ );
         m_Font.Print( "Press SpaceBar or Enter To Begin", -5.0, -4.0, m_TextZ );
      } break;
      case 3:
      {
         m_Font.Print( "2 people play with each other over a network.",          -5.0,  0.0, m_TextZ );
      } break;
      case 4:
      {
         if( ((int)!m_GameMode) == ((int)VIEW_HIGH_SCORES) )
            m_Font.Print( "View High Scores",               -5.0,  0.0, m_TextZ );
      } break;
   }
   m_Mutex.Release();
}

//***************************************************************************
//******************************* MainWindow::RenderGameOverScreen **********
// Accepts: void
//
// Purpose: Render the gameover screen
//
// Returns: void

void MainWindow::RenderGameOverScreen()
{
   m_Mutex.Lock();
   ////::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   //::glClear( GL_DEPTH_BUFFER_BIT );
   ::glLoadIdentity();
   m_Mutex.Release();

   if( m_TextureManager.size() )
   {
      m_Mutex.Lock();
      ::glEnable( GL_TEXTURE_2D );
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      m_Mutex.Release();
      RenderMap();
      RenderButtons();
   }

   m_Mutex.Lock();
   m_Mutex.Release();

   RenderConsole();
   RenderPing();
   RenderRandomItems();
   RenderMousePos();

//   m_Font.Print( "GameOver" , 0, 0, m_TextZ );

//   for( int i=0; i<m_Players.Size() ; i++ )
//   {
//      points << "Player1 Score: " << m_Players[0].m_Score;
//      m_Font.Print( points.str(),    m_BoundingRect.left+20, m_BoundingRect.bottom+20, m_Z+40 );
//      tmp.str(std::String());
//   }
   m_Mutex.Lock();
   m_Font.Print( "Press Enter To Continue" , 0, -1.0, m_TextZ );
   m_Mutex.Release();
}

//***************************************************************************
//******************************* MainWindow::RenderYouWinScreen ************
// Accepts: void
//
// Purpose: Render the you win screen
//
// Returns: void

void MainWindow::RenderYouWinScreen()
{
   m_Mutex.Lock();
   ////::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   //::glClear( GL_DEPTH_BUFFER_BIT );
   ::glLoadIdentity();
   m_Mutex.Release();

   if( m_TextureManager.size() )
   {
      m_Mutex.Lock();
      ::glEnable( GL_TEXTURE_2D );
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      m_Mutex.Release();
      RenderMap();
      RenderButtons();
   }
   m_Mutex.Lock();
   m_Mutex.Release();

   RenderListboxes();
   RenderConsole();
   RenderPing();
   RenderMousePos();

   m_Mutex.Lock();
   m_Font.Print( "You Win" , 0, 0, m_TextZ );
   m_Font.Print( "Press Enter To Continue" , 0, -2.0, m_TextZ );
   m_Mutex.Release();
}

//***************************************************************************
//******************************* MainWindow::RenderGhosts ******************
// Accepts: void
//
// Purpose: Render the game objects
//
// Returns: void

void MainWindow::RenderGhosts()
{
   if( m_TextureManager.size() )
   {
      m_Mutex.Lock();
      ::glEnable( GL_TEXTURE_2D );
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      m_Mutex.Release();

//      std::deque<Structs::Point> TextureCoordinates;
      C_TexCoord TexCoords[4];

      if( !m_WhiteGhost.m_bIsPoweredDown )
      {
         m_TextureManager.BindTexture( "texture\\wghost.png" );
         m_TextureManager.GetSubTextureCoords( "texture\\wghost.png", TexCoords );
      }
      else
      {
         if( !m_WhiteGhost.m_bIsEaten )
         {
            m_TextureManager.BindTexture(  "texture\\gghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\gghost.png", TexCoords );
         }
         else
         {
            m_TextureManager.BindTexture(  "texture\\dghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\dghost.png", TexCoords );
         }
      }
      ::glTranslatef( m_WhiteGhost.m_PositionVector.m_x, m_WhiteGhost.m_PositionVector.m_y, m_Z );
      if( m_WhiteGhost.m_PoweredDownTime < POWER_UP_AND_DOWN_TIME*0.5 && m_WhiteGhost.m_bIsPoweredDown )
      {
         if( m_WhiteGhost.m_PowerUpFlash.Update() && !m_WhiteGhost.m_bIsEaten )
         {
            m_TextureManager.BindTexture(  "texture\\wghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\wghost.png", TexCoords );
         }
         m_Mutex.Lock();
//         ::glBegin( GL_QUADS );
         m_WhiteGhost.Draw( TexCoords );
//         ::glEnd();
         m_Mutex.Release();
      }
      else
      {
         m_Mutex.Lock();
//         ::glBegin( GL_QUADS );
         m_WhiteGhost.Draw( TexCoords );
//         ::glEnd();
         m_Mutex.Release();
      }
      ::glLoadIdentity();

      if( !m_BlueGhost.m_bIsPoweredDown )
      {
         m_TextureManager.BindTexture(  "texture\\bghost.png" );
         m_TextureManager.GetSubTextureCoords( "texture\\bghost.png", TexCoords );
      }
      else
      {
         if( !m_BlueGhost.m_bIsEaten )
         {
            m_TextureManager.BindTexture(  "texture\\gghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\gghost.png", TexCoords );
         }
         else
         {
            m_TextureManager.BindTexture(  "texture\\dghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\dghost.png", TexCoords );
         }
      }
      ::glTranslatef( m_BlueGhost.m_PositionVector.m_x,   m_BlueGhost.m_PositionVector.m_y, m_Z );
      if( m_BlueGhost.m_PoweredDownTime < POWER_UP_AND_DOWN_TIME*0.5 && m_BlueGhost.m_bIsPoweredDown )
      {
         if( m_BlueGhost.m_PowerUpFlash.Update() && !m_BlueGhost.m_bIsEaten )
         {
            m_TextureManager.BindTexture(  "texture\\bghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\bghost.png", TexCoords );
         }
         m_Mutex.Lock();
//         ::glBegin( GL_QUADS );
         m_BlueGhost.Draw( TexCoords );
//         ::glEnd();
         m_Mutex.Release();
      }
      else
      {
         m_Mutex.Lock();
//         ::glBegin( GL_QUADS );
         m_BlueGhost.Draw( TexCoords );
//         ::glEnd();
         m_Mutex.Release();
      }
      ::glLoadIdentity();

      if( !m_RedGhost.m_bIsPoweredDown )
      {
         m_TextureManager.BindTexture(  "texture\\rghost.png" );
         m_TextureManager.GetSubTextureCoords( "texture\\rghost.png", TexCoords );
      }
      else
      {
         if( !m_RedGhost.m_bIsEaten )
         {
            m_TextureManager.BindTexture(  "texture\\gghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\gghost.png", TexCoords );
         }
         else
         {
            m_TextureManager.BindTexture(  "texture\\dghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\dghost.png", TexCoords );
         }
      }
      ::glTranslatef( m_RedGhost.m_PositionVector.m_x,    m_RedGhost.m_PositionVector.m_y, m_Z );
      if( m_RedGhost.m_PoweredDownTime < POWER_UP_AND_DOWN_TIME*0.5 && m_RedGhost.m_bIsPoweredDown )
      {
         if( m_RedGhost.m_PowerUpFlash.Update() && !m_RedGhost.m_bIsEaten )
         {
            m_TextureManager.BindTexture(  "texture\\rghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\rghost.png", TexCoords );
         }
         m_Mutex.Lock();
//         ::glBegin( GL_QUADS );
         m_RedGhost.Draw( TexCoords );
//         ::glEnd();
         ;m_Mutex.Release();
      }
      else
      {
         m_Mutex.Lock();
//         ::glBegin( GL_QUADS );
         m_RedGhost.Draw( TexCoords );
//         ::glEnd();
         m_Mutex.Release();
      }
      ::glLoadIdentity();

      if( !m_YellowGhost.m_bIsPoweredDown  )
      {
         m_TextureManager.BindTexture(  "texture\\yghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\yghost.png", TexCoords );
      }
      else
      {
         if( !m_YellowGhost.m_bIsEaten )
         {
            m_TextureManager.BindTexture(  "texture\\gghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\gghost.png", TexCoords );
         }
         else
         {
            m_TextureManager.BindTexture(  "texture\\dghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\dghost.png", TexCoords );
         }
      }
      ::glTranslatef( m_YellowGhost.m_PositionVector.m_x, m_YellowGhost.m_PositionVector.m_y, m_Z );
      if( m_YellowGhost.m_PoweredDownTime < POWER_UP_AND_DOWN_TIME*0.5 && m_YellowGhost.m_bIsPoweredDown )
      {
         if( m_YellowGhost.m_PowerUpFlash.Update() && !m_YellowGhost.m_bIsEaten )
         {
            m_TextureManager.BindTexture(  "texture\\yghost.png" );
            m_TextureManager.GetSubTextureCoords( "texture\\yghost.png", TexCoords );
         }
         m_Mutex.Lock();
//         ::glBegin( GL_QUADS );
         m_YellowGhost.Draw( TexCoords );
//         ::glEnd();
         m_Mutex.Release();
      }
      else
      {
         m_Mutex.Lock();
//         ::glBegin( GL_QUADS );
         m_YellowGhost.Draw( TexCoords );
//         ::glEnd();
         m_Mutex.Release();
      }
      ::glLoadIdentity();
   }
}

//***************************************************************************
//******************************* MainWindow::RenderPlayer ******************
// Accepts: void
//
// Purpose: 
//
// Returns: void

void MainWindow::RenderPlayer( C_DotEater& Player )
{
   if( Player.m_Lives > 0 )
   {
      m_Mutex.Lock();
      C_TexCoord TexCoords[4];
      int frame = Player.GetCurrentAnimation().update( false );
      std::string tmp;
      if( Player.GetCurrentAnimation().m_Frames.size() )
      {
         tmp = Player.GetCurrentAnimation().m_Frames[ frame ].texture_name;
      }
      m_TextureManager.BindTexture( tmp );
      m_TextureManager.GetSubTextureCoords( tmp, TexCoords );
      
      ::glTranslatef( Player.m_PositionVector.m_x, Player.m_PositionVector.m_y, m_Z );
      m_Mutex.Release();

      switch( Player.m_direction )
      {
         case UP:
         {
            rotate_quad_anticlockwise( TexCoords );
         } break;
         case DOWN:
         {
            rotate_quad_clockwise( TexCoords );
         } break;
         case LEFT:
         {
            flip_quad_horizontially( TexCoords );
         } break;
         case RIGHT:
         {
         } break;
      }

      m_Mutex.Lock();
      if( Player.m_InvincibleTime < POWER_UP_AND_DOWN_TIME*0.5 && Player.m_bIsInvincible )
      {
         if( Player.m_InvincibleFlash.Update() )
         {
//            ::glBegin( GL_QUADS );
            Player.Draw( TexCoords );
//            ::glEnd();
         }
      }
      else
      {
//         ::glBegin( GL_QUADS );
         Player.Draw( TexCoords );
		 
//         ::glEnd();
      }

	  // { x: , y: }
	  // prints player location
		 //m_Font.Print( (std::string)Player.m_PositionVector, m_TextBoundingRect.left+5.0, m_TextBoundingRect.top-2.0, m_TextZ );
		 //m_Font.Print( (std::string)GetTile( Player.m_PositionVector ), m_TextBoundingRect.left+5.0, m_TextBoundingRect.top-5.0, m_TextZ );

      ::glLoadIdentity();
      m_Mutex.Release();
   }
}

void MainWindow::RenderPlayer1Inventory()
{
   m_Mutex.Lock();
   if( m_Players.size() )
   {
      if( m_Players[0].m_Lives > 0 )
      {
//         m_TextureManager.BindTexture(  "texture\\deater.png" );
//         ::glTranslatef( m_Players[0].m_PositionVector.m_x, m_Players[0].m_PositionVector.m_y, m_Z );
      }
   }
   m_Mutex.Release();
}

void MainWindow::RenderPlayer2Inventory()
{
   if( m_Players.size() > 1 )
   {
   }
}


//***************************************************************************
//******************************* MainWindow::RenderGameScreen **************
// Accepts: void
//
// Purpose: Render the game objects
//
// Returns: void

void MainWindow::RenderGameScreen()
{
   m_Mutex.Lock();
   ////::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   //::glClear( GL_DEPTH_BUFFER_BIT );
   ::glLoadIdentity();

   m_Mutex.Release();

   if( m_TextureManager.size() )
   {
      ::glEnable( GL_TEXTURE_2D );
      ::glEnable( GL_BLEND );
      ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      RenderMap();
      RenderButtons();
      RenderDots();
      RenderGhosts();

      m_Mutex.Lock();
      for( int i=0; i<m_Players.size() ; i++ )
      {
         RenderPlayer( m_Players[i] );
      }
      m_Mutex.Release();
   }

   m_Mutex.Lock();
   m_Mutex.Release();

   RenderListboxes();
   RenderConsole();
   RenderPing();
   RenderRandomItems();

   std::ostringstream points, points2, player1info;
   m_Mutex.Lock();

//   if( !m_bNetworkActive )
//   {
      if( m_Players.size() == 1 )
      {
         player1info << "Lives Left: " << m_Players[0].m_Lives;
         player1info << "   Score: "      << m_Players[0].m_Score;
		 // delete
		 //player1info << "   Position: x: " << m_Players[0].m_PositionVector.m_x << " y: " << m_Players[0].m_PositionVector.m_y << " tile: " << m_Players[0].m_PositionVector;
		 //m_Console.AddLine( player1info.str() );
		 // end delete
         m_Font.Print( player1info.str(),  m_TextBoundingRect.left+5, m_TextBoundingRect.bottom+2.0, m_TextZ );
      }
      else if( m_Players.size() > 1 )
      {
         points << "P1 Score: "      << m_Players[0].m_Score;
         m_Font.Print( points.str(), m_TextBoundingRect.left+20, m_TextBoundingRect.bottom+2.0, m_TextZ );

         points2 << "P2 Score: "      << m_Players[1].m_Score;
         m_Font.Print( points2.str(), m_TextBoundingRect.left+20, m_TextBoundingRect.bottom+1.0, m_TextZ );
      }
/*   }
   else
   {
      if( m_Players.size() )
      {
         points << "Player1 Score: "      << m_Players[0].m_Score;
         m_Font.Print( points.str(), m_TextBoundingRect.left+20, m_TextBoundingRect.bottom+2.0, m_TextZ );
      }
      if( m_Players.size() > 1 )
      {
         points2 << "Player2 Score: "      << m_Players[1].m_Score;
         m_Font.Print( points2.str(), m_TextBoundingRect.left+20, m_TextBoundingRect.bottom+1.0, m_TextZ );
      }
   }*/

   RenderMousePos();

   m_Mutex.Release();
}


void MainWindow::RenderMousePos()
{
   if( m_bRenderMousePos )
   {
      m_Font.Print( m_MouseSStream.str(), 0, 0, m_TextZ );
      /*m_Font.Print( GetTile( Vector2D( m_GLMousePosition.m_x, m_GLMousePosition.m_y ) ), 
                    0, 7, m_TextZ );
      Structs::Point tile = GetTile( Vector2D( m_GLMousePosition.m_x, m_GLMousePosition.m_y ) );
      m_Font.Print( IsSolid( tile.x, tile.y ) ? "issolid" : "notsolid" , 0, 5, m_TextZ );
      m_Font.Print( m_TileMap[tile.y][tile.x], 0, 3, m_TextZ );
      std::ostringstream t;
      t << "maxsolidity : " << m_TileMap.maxsolidity();
      m_Font.Print( t.str(), 0, -2, m_TextZ );*/
   }
}


//***************************************************************************
//******************************* MainWindow::BoundingRect ******************
// Accepts: void
//
// Purpose: The visible portion of the screen at a particular z distance
//
// Returns: void

Structs::fRect MainWindow::BoundingRect( float field_of_view_y, float z, float Width, float Height )
{
   Structs::fRect rect;
   float y = fabsf( tan( (M_PI/180.0)*field_of_view_y/2.0 )*z );
   float x = y*(Width/Height);
   rect.left   = -x;
   rect.top    =  y;
   rect.right  =  x;
   rect.bottom = -y;
   return rect;
}

//***************************************************************************
//******************************* MainWindow::ShowFPSinWindow **************
// Accepts: void
//
// Purpose: Render the frame rate in the window rather than the title bar
//
// Returns: void

void MainWindow::ShowFPSinWindow( float x, float y, float z )
{
   m_Mutex.Lock();
   char str[25];
   ::_snprintf_s( str, 25, 25, "%.2f fps", m_FrameRate.FPS() );
   m_Font.Print( str, x, y-m_Font.Height(str)-1.0, z );
   m_Mutex.Release();
}

// Accepts: void
//
// Purpose: Enable opengl to render to the client area of the main window
//
// Returns: void

bool MainWindow::EnableOpenGL()
{
   return m_GLRC.EnableOpenGL( m_WinBase );
}

//***************************************************************************
//******************************* MainWindow::DisableOpenGL *****************
// Accepts: void
//
// Purpose: 
//
// Returns: 

bool MainWindow::DisableOpenGL()
{
   return m_GLRC.DisableOpenGL();
}


// Accepts: void
//
// Purpose: Resize the viewport when the window has been resized
//
// Returns: void

bool MainWindow::Reshape( long width, long height )
{
   bool bOrignalPauseState = m_bGamePaused;

   SaveSpriteTiles();

   m_bGamePaused = true;

   m_Width  = width;
   m_Height = height;
   std::ostringstream temp;
   temp << "{ width="  << width;
   temp << ", height=" << height << " }";

//   m_Mutex.Lock();
//   m_Console.AddLine( temp.str() );
//   m_Mutex.Release();

   if( height < 1 ) // Prevent A Divide By Zero By
      height=1;
   m_Mutex.Lock();
   ::glViewport( 0, 0, (GLsizei)(width), (GLsizei)(height) );
   ::glMatrixMode( GL_PROJECTION );
   ::glLoadIdentity();                                                  // Reset The Projection Matrix
   ::gluPerspective( m_FieldOfViewY, (GLfloat)(width)/(GLfloat)(height), // Calculate The Aspect Ratio Of The Window
                     0.01f, 1000.0f );
   ::glMatrixMode( GL_MODELVIEW );                                      // Select The Modelview Matrix
   ::glLoadIdentity();

   m_Mutex.Release();

   m_BoundingRect     = BoundingRect( m_FieldOfViewY, m_Z,     m_Width, m_Height );
   m_TextBoundingRect = BoundingRect( m_FieldOfViewY, m_TextZ, m_Width, m_Height );

   m_BoundingRectWidth      = (m_BoundingRect.right-m_BoundingRect.left); 
   m_BoundingRectHeight     = (m_BoundingRect.top-m_BoundingRect.bottom);
   m_TextBoundingRectWidth  = (m_TextBoundingRect.right-m_TextBoundingRect.left);
   m_TextBoundingRectHeight = (m_TextBoundingRect.top-m_TextBoundingRect.bottom);;

   m_TextBoundingRectHalfWidth  = m_TextBoundingRectWidth*0.5;
   m_TextBoundingRectHalfHeight = m_TextBoundingRectHeight*0.5;
   m_BoundingRectHalfWidth      = m_BoundingRectWidth*0.5;
   m_BoundingRectHalfHeight     = m_BoundingRectHeight*0.5;

   float tile_width  = (((m_BoundingRect.right-m_BoundingRect.left))/(float)m_TileMap.width());
   float tile_height = (((m_BoundingRect.top-m_BoundingRect.bottom))/(float)m_TileMap.height());

   m_MapTexturedQuad.SetSize( tile_width, 
                              tile_height );

   m_InvBoundingRectWidthDivTileMapWidth                = 1.0f/(m_BoundingRectWidth/m_TileMap.width());
   m_InvBoundingRectHeightDivTileMapHeight              = 1.0f/(m_BoundingRectHeight/m_TileMap.height());
   m_BoundingRectHalfWidthMinMapTexturedQuadhalfwidth   = m_BoundingRectHalfWidth - m_MapTexturedQuad.halfwidth();
   m_BoundingRectHalfHeightMinMapTexturedQuadhalfheight = m_BoundingRectHalfHeight + m_MapTexturedQuad.halfheight();


   m_Mutex.Lock();
   for( int i=0; i<m_Players.size() ; i++ )
   {
      m_Players[i].m_TexQuad.SetSize( tile_width, 
                                      tile_height );
   }

   m_WhiteGhost.m_TexQuad.SetSize(  tile_width, tile_height );
   m_BlueGhost.m_TexQuad.SetSize(   tile_width, tile_height );
   m_RedGhost.m_TexQuad.SetSize(    tile_width, tile_height );
   m_YellowGhost.m_TexQuad.SetSize( tile_width, tile_height );

   Vector2D VelocityVector;
   VelocityVector.m_x = tile_width*0.005;
   VelocityVector.m_y = tile_height*0.005;

   for( int i=0; i<m_Players.size() ; i++ )
   {
      m_Players[i].m_VelocityVector = VelocityVector;
   }

   m_WhiteGhost.m_VelocityVector   = VelocityVector*0.75;
   if( m_Players.size() > 1 )
      m_BlueGhost.m_VelocityVector   = VelocityVector*0.75;
   else
      m_BlueGhost.m_VelocityVector   = VelocityVector;
   if( m_Players.size() > 2 )
      m_RedGhost.m_VelocityVector    = VelocityVector*0.75;
   else
      m_RedGhost.m_VelocityVector    = VelocityVector;
   if( m_Players.size() > 3 )
      m_YellowGhost.m_VelocityVector = VelocityVector*0.75;
   else
      m_YellowGhost.m_VelocityVector = VelocityVector;


   BuildMapDisplayLists();

   RestoreSpriteTiles();
//   m_bGamePaused = false;
   m_bGamePaused = bOrignalPauseState;
   m_Mutex.Release();

   return false;
}

void MainWindow::RenderDirtyTiles()
{
   m_Mutex.Lock();
   //::glClear( GL_DEPTH_BUFFER_BIT );
   ::glLoadIdentity();
   /*
   C_TexCoord TexCoords[4];
   float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(), y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   std::vector< std::string >& maptiles = m_TileMap.GetTileSet();
   for( int i=0; i<maptiles.size(); i++ ) // for each texture
   {
      m_TextureManager.BindTexture( "texture\\maps\\"+maptiles[i] );
      m_TextureManager.GetSubTextureCoords( "texture\\maps\\"+maptiles[i], TexCoords );
      for( int row=0; row<m_TileMap.height(); row++, y-=m_MapTexturedQuad.height() ) // for each row
      {
         for( int col=0; col<m_TileMap.width(); col++, x+=m_MapTexturedQuad.width() ) // for each coloum
         {
            // if the texture is in the map render it
            if( i == m_TileMap[row][col].m_TileGraphic )
            // m_TextureManager indexing starts at 1 while
            // m_TileMap indexing starts at 0
            {
               ::glTranslatef( x, y, m_Z );
               m_MapTexturedQuad.glBegin();
               m_MapTexturedQuad.Draw( 0, 0, 0, TexCoords );
               ::glEnd();
               ::glLoadIdentity();
            }
         }
         x = m_BoundingRect.left+m_MapTexturedQuad.halfwidth();
      }
      y = m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   }
*/
   m_Mutex.Release();
}

// Accepts: void
//
// Purpose: Render the dots
//
// Returns: void

void MainWindow::RenderDots()
{
   m_Mutex.Lock();
   if( m_bUseVBOs && (glGenBuffersARB != NULL) )
   {
      m_DisplayListManager.call( "dot" );
   }
   else
   {
      RenderDotVBOs();
   }

   /*
   C_TexCoord TexCoords[4];
   C_Vert     Verticies[4];

   ::glEnable( GL_TEXTURE_2D );
   m_TextureManager.BindTexture( "texture\\dot.png" );
   m_TextureManager.GetSubTextureCoords( "texture\\dot.png", TexCoords );

   float x = m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
         y = m_BoundingRect.top-m_MapTexturedQuad.halfheight(),
         _x = 0, _y = 0;
   int i = 0;

   ::glLoadIdentity();
   ::glTranslatef( 0, 0, m_Z );
   ::glBegin( GL_TRIANGLES );
   for( ; i<m_Dots.size(); i++ )
   {
      if( m_Dots[i].m_bIsPowerPellet )
         break; // dots sorted by if it is a power pellet or not so it 
                // is not necessiary to continue
       _x = x+(m_MapTexturedQuad.width() *(float)m_Dots[i].m_tile.x)+(m_MapTexturedQuad.width()*0.20);
       _y = y-(m_MapTexturedQuad.height()*(float)m_Dots[i].m_tile.y)-(m_MapTexturedQuad.height()*0.20);
       m_MapTexturedQuad.GetVerticies( Verticies, _x, _y, m_Z );
//       TexCoords[0].glTexCoord2f();
//       Verticies[0].glVertex2f();

//       TexCoords[1].glTexCoord2f();
//       Verticies[1].glVertex2f();

//       TexCoords[2].glTexCoord2f();
//       Verticies[2].glVertex2f();


       TexCoords[0].glTexCoord2f();
       Verticies[0].glVertex2f();

       TexCoords[2].glTexCoord2f();
       Verticies[2].glVertex2f();

       TexCoords[3].glTexCoord2f();
       Verticies[3].glVertex2f();
   }
   m_TextureManager.BindTexture(  "texture\\bigdot.png" );
   m_TextureManager.GetSubTextureCoords( "texture\\bigdot.png", TexCoords );
   for( ; i<m_Dots.size(); i++  )
   {
      if( !m_Dots[i].m_bIsPowerPellet ) // some sort of weird error
         continue;
       _x = x+(m_MapTexturedQuad.width() *(float)m_Dots[i].m_tile.x);
       _y = y-(m_MapTexturedQuad.height()*(float)m_Dots[i].m_tile.y);
       m_MapTexturedQuad.GetVerticies( Verticies, _x, _y, m_Z );
       TexCoords[0].glTexCoord2f();
       Verticies[0].glVertex2f();

       TexCoords[1].glTexCoord2f();
       Verticies[1].glVertex2f();

       TexCoords[2].glTexCoord2f();
       Verticies[2].glVertex2f();


       TexCoords[0].glTexCoord2f();
       Verticies[0].glVertex2f();

       TexCoords[2].glTexCoord2f();
       Verticies[2].glVertex2f();

       TexCoords[3].glTexCoord2f();
       Verticies[3].glVertex2f();
   }
   ::glEnd();
   ::glLoadIdentity();*/
   m_Mutex.Release();
}

//***************************************************************************
//******************************* MainWindow::RenderMap *********************
// Accepts: void
//
// Purpose: Render the map
//
// Returns: void

void MainWindow::RenderMap( void )
{
   m_Mutex.Lock();
   if( m_bUseVBOs && (glGenBuffersARB != NULL) )
   {
      m_DisplayListManager.call( "map" );
   }
   else
   {
      RenderMapVBOs();
   }
   m_Mutex.Release();
}

//***************************************************************************
//******************************* MainWindow::RenderMapToBuildList **********
// Accepts: void
//
// Purpose: Render the map
//
// Returns: void

void MainWindow::RenderMapToBuildList( void )
{
   m_Mutex.Lock();
   RenderMapVBOs();
   m_Mutex.Release();
}

void MainWindow::BuildMapVBOs()
{
   m_MapMesh.DestroyClientSideData();
   if( !m_TileMap.size() )
      return;

   C_TexCoord TexCoords[4];
   C_Vert     Verticies[4];
   GLuint     Index    = 0;

   float x=m_BoundingRect.left+m_MapTexturedQuad.halfwidth(), 
         y=m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   std::vector< std::string >& maptiles = m_TileMap.GetTileSet();

   m_MapMesh.m_VertexCount = (int) ( m_TileMap.width() * m_TileMap.height() * 6 );

   // trianglestrip verticies needed = 2 + n    // 2 initial verts + 1 vert per triangle 
   // triangle      verticies needed = 3n       // 3 verts per triangle 
   // quad          verticies needed = 4n       // 4 verts per quad 
   // where in is a primative

   m_MapMesh.m_pVertices   = new C_Vert2D[ m_MapMesh.m_VertexCount ];                // Allocate Vertex Data
   m_MapMesh.m_pTexCoords  = new C_TexCoord[ m_MapMesh.m_VertexCount ];            // Allocate Tex Coord Data
   m_MapMesh.m_nTextureId  = m_TextureManager.GetSubTextureID( "texture\\maps\\"+maptiles[0]);

   for( int i=0; i<maptiles.size(); i++ ) // for each texture
   {
      m_TextureManager.BindTexture( "texture\\maps\\"+maptiles[i] );
      m_TextureManager.GetSubTextureCoords( "texture\\maps\\"+maptiles[i], TexCoords );
      int index = 0;
      for( int row=0; row<m_TileMap.height(); row++, y-=m_MapTexturedQuad.height() ) // for each row
      {
         for( int col=0; col<m_TileMap.width(); col++, x+=m_MapTexturedQuad.width() ) // for each coloum
         {
            // if the texture is in the map render it
            if( i == m_TileMap[row][col].m_TileGraphic )
            {
               m_MapTexturedQuad.GetVerticies( Verticies, x, y, m_Z );
               m_MapMesh.m_pVertices[Index].x    = Verticies[0].x;
               m_MapMesh.m_pVertices[Index].y    = Verticies[0].y;
               m_MapMesh.m_pTexCoords[Index].U   = TexCoords[0].U;
               m_MapMesh.m_pTexCoords[Index].V   = TexCoords[0].V;
               m_MapMesh.m_Indices.push_back( (GLuint) Index );

               m_MapMesh.m_pVertices[Index+1].x  = Verticies[1].x;
               m_MapMesh.m_pVertices[Index+1].y  = Verticies[1].y;
               m_MapMesh.m_pTexCoords[Index+1].U = TexCoords[1].U;
               m_MapMesh.m_pTexCoords[Index+1].V = TexCoords[1].V;
               m_MapMesh.m_Indices.push_back( (GLuint) Index+1 );

               m_MapMesh.m_pVertices[Index+2].x  = Verticies[2].x;
               m_MapMesh.m_pVertices[Index+2].y  = Verticies[2].y;
               m_MapMesh.m_pTexCoords[Index+2].U = TexCoords[2].U;
               m_MapMesh.m_pTexCoords[Index+2].V = TexCoords[2].V;
               m_MapMesh.m_Indices.push_back( (GLuint) Index+2 );

               m_MapMesh.m_pVertices[Index+3].x  = Verticies[0].x;
               m_MapMesh.m_pVertices[Index+3].y  = Verticies[0].y;
               m_MapMesh.m_pTexCoords[Index+3].U = TexCoords[0].U;
               m_MapMesh.m_pTexCoords[Index+3].V = TexCoords[0].V;
               m_MapMesh.m_Indices.push_back( (GLuint) Index+3 );

               m_MapMesh.m_pVertices[Index+4].x  = Verticies[2].x;
               m_MapMesh.m_pVertices[Index+4].y  = Verticies[2].y;
               m_MapMesh.m_pTexCoords[Index+4].U = TexCoords[2].U;
               m_MapMesh.m_pTexCoords[Index+4].V = TexCoords[2].V;
               m_MapMesh.m_Indices.push_back( (GLuint) Index+4 );

               m_MapMesh.m_pVertices[Index+5].x  = Verticies[3].x;
               m_MapMesh.m_pVertices[Index+5].y  = Verticies[3].y;
               m_MapMesh.m_pTexCoords[Index+5].U = TexCoords[3].U;
               m_MapMesh.m_pTexCoords[Index+5].V = TexCoords[3].V;
               m_MapMesh.m_Indices.push_back( (GLuint) Index+5 );

               Index += 6;
            }
         }
         x = m_BoundingRect.left+m_MapTexturedQuad.halfwidth();
      }
      y = m_BoundingRect.top-m_MapTexturedQuad.halfheight();
   }

   m_MapMesh.m_IndexCount = m_MapMesh.m_Indices.size();

   if( IsExtensionSupported( "GL_ARB_vertex_buffer_object" ) && m_bUseVBOs )
   {
      glDeleteBuffersARB( m_MapMesh.m_VertexCount, &m_MapMesh.m_VBOVertices  );
      glDeleteBuffersARB( m_MapMesh.m_VertexCount, &m_MapMesh.m_VBOTexCoords );
      glDeleteBuffersARB( m_MapMesh.m_VertexCount, &m_MapMesh.m_VBOIndices   );

      // Generate And Bind The Vertex Buffer
      glGenBuffersARB( 1, &m_MapMesh.m_VBOVertices );                  // Get A Valid Name
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_MapMesh.m_VBOVertices ); // Bind The Buffer
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_MapMesh.m_VertexCount*2*sizeof(float), 
                       m_MapMesh.m_pVertices, GL_STATIC_DRAW_ARB );

      // Generate And Bind The Texture Coordinate Buffer
      glGenBuffersARB( 1, &m_MapMesh.m_VBOTexCoords ); // Get A Valid Name
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_MapMesh.m_VBOTexCoords ); // Bind The Buffer
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_MapMesh.m_VertexCount*2*sizeof(float), 
                       m_MapMesh.m_pTexCoords, GL_STATIC_DRAW_ARB );

      glGenBuffersARB( 1, &m_MapMesh.m_VBOIndices );                  // Get A Valid Name
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_MapMesh.m_VBOIndices ); // Bind The Buffer
      glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(CMesh2D<GLushort>::MeshIndexType)*m_MapMesh.m_Indices.size(), 
                       &m_MapMesh.m_Indices[0], GL_STATIC_DRAW_ARB );

      // Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card
      delete [] m_MapMesh.m_pVertices;  m_MapMesh.m_pVertices  = NULL;
      delete [] m_MapMesh.m_pTexCoords; m_MapMesh.m_pTexCoords = NULL;
      m_MapMesh.m_Indices.clear();
   }
}

void MainWindow::BuildDotVBOs()
{
   m_Mutex.Lock();
   if( !m_Dots.size() )
      return;
   C_TexCoord TexCoords[4];
   C_Vert     Verticies[4];

//   ::glEnable( GL_TEXTURE_2D );

   float x = m_BoundingRect.left+m_MapTexturedQuad.halfwidth(),
         y = m_BoundingRect.top-m_MapTexturedQuad.halfheight(),
         _x = 0, _y = 0;
   int i = 0;

   GLuint Index = 0;
   GLuint num_bigdots = 0; // bigdots use 6 verts while normal dots use 3
   for( int c = 0; c<m_Dots.size() ; c++ )
   {
      if( m_Dots[c].m_bIsPowerPellet )
      {
         num_bigdots += 1;
      }
   }

   m_DotMesh.DestroyClientSideData();

   m_DotMesh.m_VertexCount = (int) ( (m_Dots.size() * 3) + (num_bigdots * 3) );
   m_DotMesh.m_Indices.reserve( m_DotMesh.m_VertexCount );

   // trianglestrip verticies needed = 2 + n    // 2 initial verts + 1 vert per triangle 
   // triangle      verticies needed = 3n       // 3 verts per triangle 
   // quad          verticies needed = 4n       // 4 verts per quad 
   // where in is a primative

   m_DotMesh.m_pVertices   = new C_Vert2D[ m_DotMesh.m_VertexCount ];        // Allocate Vertex Data
   m_DotMesh.m_pTexCoords  = new C_TexCoord[ m_DotMesh.m_VertexCount ];      // Allocate Tex Coord Data
   m_DotMesh.m_nTextureId  = m_TextureManager.GetSubTextureID( "texture\\dot.png" );


   m_TextureManager.BindTexture( "texture\\dot.png" );
   m_TextureManager.GetSubTextureCoords( "texture\\dot.png", TexCoords );

   ::glLoadIdentity();
   ::glTranslatef( 0, 0, m_Z );
   for( ; i<m_Dots.size(); i++ )
   {
      if( m_Dots[i].m_bIsPowerPellet )
         break; // dots sorted by if it is a power pellet or not so it 
                // is not necessiary to continue
      _x = x+(m_MapTexturedQuad.width() *(float)m_Dots[i].m_tile.x)+(m_MapTexturedQuad.width()*0.20);
      _y = y-(m_MapTexturedQuad.height()*(float)m_Dots[i].m_tile.y)-(m_MapTexturedQuad.height()*0.20);
      m_MapTexturedQuad.GetVerticies( Verticies, _x, _y, m_Z );
      m_DotMesh.m_pVertices[Index].x    = Verticies[0].x;
      m_DotMesh.m_pVertices[Index].y    = Verticies[0].y;
      m_DotMesh.m_pTexCoords[Index].U   = TexCoords[0].U;
      m_DotMesh.m_pTexCoords[Index].V   = TexCoords[0].V;
      m_DotMesh.m_Indices.push_back( (GLuint) Index );

      m_DotMesh.m_pVertices[Index+1].x  = Verticies[2].x;
      m_DotMesh.m_pVertices[Index+1].y  = Verticies[2].y;
      m_DotMesh.m_pTexCoords[Index+1].U = TexCoords[2].U;
      m_DotMesh.m_pTexCoords[Index+1].V = TexCoords[2].V;
      m_DotMesh.m_Indices.push_back( (GLuint) Index+1 );

      m_DotMesh.m_pVertices[Index+2].x  = Verticies[3].x;
      m_DotMesh.m_pVertices[Index+2].y  = Verticies[3].y;
      m_DotMesh.m_pTexCoords[Index+2].U = TexCoords[3].U;
      m_DotMesh.m_pTexCoords[Index+2].V = TexCoords[3].V;
      m_DotMesh.m_Indices.push_back( (GLuint) Index+2 );
      Index += 3;
   }

   m_TextureManager.BindTexture(  "texture\\bigdot.png" );
   m_TextureManager.GetSubTextureCoords( "texture\\bigdot.png", TexCoords );
   for( ; i<m_Dots.size(); i++  )
   {
      if( !m_Dots[i].m_bIsPowerPellet ) // some sort of weird error
         continue;
      _x = x+(m_MapTexturedQuad.width() *(float)m_Dots[i].m_tile.x);
      _y = y-(m_MapTexturedQuad.height()*(float)m_Dots[i].m_tile.y);
      m_MapTexturedQuad.GetVerticies( Verticies, _x, _y, m_Z );
      m_DotMesh.m_pVertices[Index].x    = Verticies[0].x;
      m_DotMesh.m_pVertices[Index].y    = Verticies[0].y;
      m_DotMesh.m_pTexCoords[Index].U   = TexCoords[0].U;
      m_DotMesh.m_pTexCoords[Index].V   = TexCoords[0].V;
      m_DotMesh.m_Indices.push_back( (GLuint) Index );

      m_DotMesh.m_pVertices[Index+1].x  = Verticies[1].x;
      m_DotMesh.m_pVertices[Index+1].y  = Verticies[1].y;
      m_DotMesh.m_pTexCoords[Index+1].U = TexCoords[1].U;
      m_DotMesh.m_pTexCoords[Index+1].V = TexCoords[1].V;
      m_DotMesh.m_Indices.push_back( (GLuint) Index+1 );

      m_DotMesh.m_pVertices[Index+2].x  = Verticies[2].x;
      m_DotMesh.m_pVertices[Index+2].y  = Verticies[2].y;
      m_DotMesh.m_pTexCoords[Index+2].U = TexCoords[2].U;
      m_DotMesh.m_pTexCoords[Index+2].V = TexCoords[2].V;
      m_DotMesh.m_Indices.push_back( (GLuint) Index+2 );

      m_DotMesh.m_pVertices[Index+3].x  = Verticies[0].x;
      m_DotMesh.m_pVertices[Index+3].y  = Verticies[0].y;
      m_DotMesh.m_pTexCoords[Index+3].U = TexCoords[0].U;
      m_DotMesh.m_pTexCoords[Index+3].V = TexCoords[0].V;
      m_DotMesh.m_Indices.push_back( (GLuint) Index+3 );

      m_DotMesh.m_pVertices[Index+4].x  = Verticies[2].x;
      m_DotMesh.m_pVertices[Index+4].y  = Verticies[2].y;
      m_DotMesh.m_pTexCoords[Index+4].U = TexCoords[2].U;
      m_DotMesh.m_pTexCoords[Index+4].V = TexCoords[2].V;
      m_DotMesh.m_Indices.push_back( (GLuint) Index+4 );

      m_DotMesh.m_pVertices[Index+5].x  = Verticies[3].x;
      m_DotMesh.m_pVertices[Index+5].y  = Verticies[3].y;
      m_DotMesh.m_pTexCoords[Index+5].U = TexCoords[3].U;
      m_DotMesh.m_pTexCoords[Index+5].V = TexCoords[3].V;
      m_DotMesh.m_Indices.push_back( (GLuint) Index+5 );

      Index +=6 ;
   }

   m_DotMesh.m_IndexCount = m_DotMesh.m_Indices.size();

   if( IsExtensionSupported( "GL_ARB_vertex_buffer_object" ) && m_bUseVBOs )
   {
      glDeleteBuffersARB( m_DotMesh.m_VertexCount, &m_DotMesh.m_VBOVertices  );
      glDeleteBuffersARB( m_DotMesh.m_VertexCount, &m_DotMesh.m_VBOTexCoords );
      glDeleteBuffersARB( m_DotMesh.m_VertexCount, &m_DotMesh.m_VBOIndices   );

      // Generate And Bind The Vertex Buffer
      glGenBuffersARB( 1, &m_DotMesh.m_VBOVertices );                  // Get A Valid Name
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_DotMesh.m_VBOVertices ); // Bind The Buffer
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_DotMesh.m_VertexCount*2*sizeof(float), 
                       m_DotMesh.m_pVertices, GL_STATIC_DRAW_ARB );

      // Generate And Bind The Texture Coordinate Buffer
      glGenBuffersARB( 1, &m_DotMesh.m_VBOTexCoords );                  // Get A Valid Name
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_DotMesh.m_VBOTexCoords ); // Bind The Buffer
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_DotMesh.m_VertexCount*2*sizeof(float), 
                       m_DotMesh.m_pTexCoords, GL_STATIC_DRAW_ARB );

      glGenBuffersARB( 1, &m_DotMesh.m_VBOIndices );                          // Get A Valid Name
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_DotMesh.m_VBOIndices ); // Bind The Buffer
      glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(CMesh2D<GLushort>::MeshIndexType)*m_DotMesh.m_Indices.size(), 
                       &m_DotMesh.m_Indices[0], GL_STATIC_DRAW_ARB );

      // Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card
      delete [] m_DotMesh.m_pVertices;  m_DotMesh.m_pVertices  = NULL;
      delete [] m_DotMesh.m_pTexCoords; m_DotMesh.m_pTexCoords = NULL;
      m_DotMesh.m_Indices.clear();
   }
   m_Mutex.Release();
}

void MainWindow::RenderDotVBOs()
{
   m_Mutex.Lock();
   ::glBindTexture( GL_TEXTURE_2D, m_DotMesh.m_nTextureId );
   ::glEnableClientState( GL_VERTEX_ARRAY );        // Enable Vertex Arrays
   ::glEnableClientState( GL_TEXTURE_COORD_ARRAY ); // Enable Texture Coord Arrays

   // Set Pointers To Our Data
   if( (glGenBuffersARB != NULL) && m_bUseVBOs ) // this is only valid if vbo's are supported
   {
      glBindBufferARB(   GL_ARRAY_BUFFER_ARB, m_DotMesh.m_VBOVertices        );
      ::glVertexPointer(   2, GL_FLOAT, 0, (char *) NULL                     );
   
      glBindBufferARB(   GL_ARRAY_BUFFER_ARB, m_DotMesh.m_VBOTexCoords       );
      ::glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL                     );

      glBindBufferARB(   GL_ELEMENT_ARRAY_BUFFER_ARB, m_DotMesh.m_VBOIndices );
   }
   else
   {
      ::glVertexPointer(   2, GL_FLOAT, 0, m_DotMesh.m_pVertices  ); // Set The Vertex Pointer To Our Vertex Data
      ::glTexCoordPointer( 2, GL_FLOAT, 0, m_DotMesh.m_pTexCoords ); // Set The Vertex Pointer To Our TexCoord Data
   }

   // Render
   ::glLoadIdentity();
   ::glTranslatef( 0, 0, m_Z );
   // Draw All Of The Triangles At Once
   if( (glGenBuffersARB != NULL) && m_bUseVBOs ) // this is only valid if vbo's are supported
   {
      ::glDrawElements( GL_TRIANGLES, m_DotMesh.m_IndexCount, GL_UNSIGNED_SHORT, NULL );
   }
   else
   {
      ::glDrawElements( GL_TRIANGLES, m_DotMesh.m_IndexCount, GL_UNSIGNED_SHORT, &m_DotMesh.m_Indices[0] );
   }
   ::glLoadIdentity();

   // Disable Pointers
   ::glDisableClientState( GL_VERTEX_ARRAY );        // Disable Vertex Arrays
   ::glDisableClientState( GL_TEXTURE_COORD_ARRAY ); // Disable Texture Coord Arrays
   m_Mutex.Release();
}

void MainWindow::RenderMapVBOs()
{
   ::glBindTexture( GL_TEXTURE_2D, m_MapMesh.m_nTextureId );
   ::glEnableClientState( GL_VERTEX_ARRAY );        // Enable Vertex Arrays
   ::glEnableClientState( GL_TEXTURE_COORD_ARRAY ); // Enable Texture Coord Arrays

   // Set Pointers To Our Data
   if( (glGenBuffersARB != NULL) && m_bUseVBOs ) // this is only valid if vbo's are supported
   {
      glBindBufferARB(   GL_ARRAY_BUFFER_ARB, m_MapMesh.m_VBOVertices        );
      ::glVertexPointer(   2, GL_FLOAT, 0, (char *) NULL                     );
   
      glBindBufferARB(   GL_ARRAY_BUFFER_ARB, m_MapMesh.m_VBOTexCoords       );
      ::glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL                     );

      glBindBufferARB(   GL_ELEMENT_ARRAY_BUFFER_ARB, m_MapMesh.m_VBOIndices );
   }
   else
   {
      ::glVertexPointer( 2, GL_FLOAT, (GLsizei)0, (const GLvoid*)m_MapMesh.m_pVertices );    // Set The Vertex Pointer To Our Vertex Data
      ::glTexCoordPointer( 2, GL_FLOAT, (GLsizei)0, (const GLvoid*)m_MapMesh.m_pTexCoords ); // Set The Vertex Pointer To Our TexCoord Data
      //m_MapMesh.m_VertexCount
   }

   // Render
   ::glLoadIdentity();
   ::glTranslatef( 0, 0, m_Z );
   // Draw All Of The Triangles At Once
   if( (glGenBuffersARB != NULL) && m_bUseVBOs ) // this is only valid if vbo's are supported
   {
      ::glDrawElements( GL_TRIANGLES, m_MapMesh.m_IndexCount, GL_UNSIGNED_SHORT, NULL );
   }
   else
   {
	  if( m_MapMesh.m_Indices.size() )
	  {
         ::glDrawElements( GL_TRIANGLES, m_MapMesh.m_IndexCount, GL_UNSIGNED_SHORT, &m_MapMesh.m_Indices[0] );
	  }
   }
//   if( glDrawRangeElements )
//   glDrawRangeElements( GL_TRIANGLES, 0, m_DotMesh.m_Indices.size(), m_DotMesh.m_Indices.size(), GL_UNSIGNED_INT, 
//                       (char *)NULL );

   ::glLoadIdentity();

   // Disable Pointers
   ::glDisableClientState( GL_VERTEX_ARRAY );        // Disable Vertex Arrays
   ::glDisableClientState( GL_TEXTURE_COORD_ARRAY ); // Disable Texture Coord Arrays
}

void MainWindow::DestroyVBOs()
{
   if( IsExtensionSupported( "GL_ARB_vertex_buffer_object" ) )//&& m_bUseVBOs )
   {
      glDeleteBuffersARB( m_DotMesh.m_VertexCount, &m_DotMesh.m_VBOVertices  );
      glDeleteBuffersARB( m_DotMesh.m_VertexCount, &m_DotMesh.m_VBOTexCoords );
      glDeleteBuffersARB( m_DotMesh.m_VertexCount, &m_DotMesh.m_VBOIndices   );

      glDeleteBuffersARB( m_MapMesh.m_VertexCount, &m_MapMesh.m_VBOVertices  );
      glDeleteBuffersARB( m_MapMesh.m_VertexCount, &m_MapMesh.m_VBOTexCoords );
      glDeleteBuffersARB( m_MapMesh.m_VertexCount, &m_MapMesh.m_VBOIndices   );
   }
}
