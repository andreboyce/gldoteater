#include "main.h"

//***************************************************************************
//******************************* MainWindow::HandleDownKeys ****************
// Accepts: void
//
// Purpose: performs actions when keys are pressed
//          depending on what mode the game is in
//
// Returns: void

void MainWindow::HandleDownKeys( WPARAM wparam, LPARAM lparam )
{
   switch( m_GameMode )
   {
      case TITLE_SCREEN:
      {
      } break;
      case GAME_IN_PROGRESS:
      {
      } break;
      case GAME_OVER:
      {
      } break;
      case YOU_WIN_MODE:
      {
      } break;
      default:
      {
      };
   };

   if( KEYDOWN(VK_END) )
   {
      TOGGLE(m_bGamePaused);
      SaveIntervals();
   }

   if( KEYDOWN(VK_F11) )
   {
      TOGGLE(m_bSoundOn);
   }

   if( KEYDOWN(VK_F12) )
   {
      ::PostMessage( m_WinBase, WM_TOGGLEFULLSCREEN, 0, 0 );
      // SendMessage() wont work with the game loop so you need to use PostMessage()
   }
   if( KEYDOWN(VK_ESCAPE) )
   {
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
      // SendMessage() wont work with the game loop so you need to use PostMessage()
   }
}

//***************************************************************************
//******************************* MainWindow::HandleUpKeys ******************
// Accepts: void
//
// Purpose: performs actions when keys are released
//          depending on what mode the game is in
//
// Returns: void

void MainWindow::HandleUpKeys( WPARAM wparam, LPARAM lparam )
{
   switch( m_GameMode )
   {
      case TITLE_SCREEN:
      {
         if( wparam == VK_SPACE )
         {
            m_GameMode = GAME_IN_PROGRESS;
            m_AddEnmyRowInterval.Begin();
         }
      } break;
      case GAME_IN_PROGRESS:
      {
         if( wparam == VK_RETURN )
         {
//            m_GameMode = GAME_OVER;
         }
      } break;
      case GAME_OVER:
      {
         if( wparam == VK_RETURN )
         {
            m_GameMode = TITLE_SCREEN;
            InitialiseNewGame();
         }
      } break;
      case YOU_WIN_MODE:
      {
         if( wparam == VK_RETURN )
         {
            m_GameMode = TITLE_SCREEN;
            InitialiseNewGame();
         }
      } break;
      default:
      {
      };
   };
}

