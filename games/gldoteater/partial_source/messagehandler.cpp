#include "main.h"

LRESULT MainWindow::MWBaseOnCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( LOWORD(wparam) )
   {
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRDoubleClick(     MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONDBLCLK, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONDOWN, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRButtonU( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONUP, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnLButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   MWBaseOnContextLbtn( mw, wparam, lparam );
   /*
   Structs::Point tile = mw.GetTile( Vector2D( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
   if( !mwIsSolid( tile.x, tile.y ) )
   {
      mw.m_PlayerGoal = tile;
      mw.GetplayerGoal( mw.m_PlayerControlledByMouse );
   }*/
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDOWN, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnLButtonU( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONUP, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnCreate( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_CREATE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( wparam )
   {
      case SIZE_MINIMIZED:
      {
         mw.m_Mutex.Lock();
         mw.m_bGamePaused = true;
         mw.m_bisVisible = false;

         if( mw.m_bNetworkActive )
         {
            if( mw.IamServer() )
            {
//               std::string text = mw.m_Nick + ": " + "player minimized";
//               mw.SendToClient( (char*)text.c_str(), text.length(), 0,
//                             ENET_CHANNEL_TEXT, ENET_PACKET_FLAG_RELIABLE );
               mw.NetorkCommandsQueue.push_back( COMMAND_PAUSE );
            }
            else
            {
//               std::string text = mw.m_Nick + ": " + "player minimized";
//               mw.SendToHost( (char*)text.c_str(), text.length(),
//                           ENET_CHANNEL_TEXT, ENET_PACKET_FLAG_RELIABLE );
            }
         }
         mw.m_Mutex.Release();

      } break;
      case SIZE_MAXIMIZED:
      {
         if( mw.m_bPauseOnSize )
            mw.m_bGamePaused = false;
         mw.m_bisVisible  = true;
         mw.Reshape( LOWORD(lparam), HIWORD(lparam) );

         if( mw.m_bNetworkActive )
         {
            if( mw.IamServer() )
            {         
               mw.NetorkCommandsQueue.push_back( COMMAND_PAUSE );
            }
         }
      } break;

      case SIZE_RESTORED:
      {
         if( mw.m_bPauseOnSize )
            mw.m_bGamePaused = false;
         mw.m_bisVisible  = true;
         mw.Reshape( LOWORD(lparam), HIWORD(lparam) );
         if( mw.m_bNetworkActive )
         {
            if( mw.IamServer() )
            {         
               mw.NetorkCommandsQueue.push_back( COMMAND_PAUSE );
            }
         }
      } break;
      default:
      {
         mw.m_bisVisible        = true;
         mw.Reshape( LOWORD(lparam), HIWORD(lparam) );
      } break;
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnNotify( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   NMHDR *lpnmhdr = (NMHDR*)(lparam);
   switch( lpnmhdr->code )
   {
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_NOTIFY, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnHScroll( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{   
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_HSCROLL, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnVScroll( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_VSCROLL, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnKeyDown( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_Keys.PressKey( wparam );
   mw.HandleDownKeys( wparam, lparam );
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYDOWN, wparam, lparam );
}     

LRESULT MainWindow::MWBaseOnKeyUp( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_Keys.ReleaseKey( wparam );
   mw.HandleUpKeys( wparam, lparam );
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYUP, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnPaint( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_PAINT, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnTimer( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( wparam )
   {
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_TIMER, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnMove( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_x_pos = LOWORD(lparam);
   mw.m_y_pos = HIWORD(lparam);
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_MOVE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnKillFocus( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_bHasKeyboardFocus = false;
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KILLFOCUS, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnSetFocus( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_bHasKeyboardFocus = true;
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_SETFOCUS, wparam, lparam );
}

//***************************************************************************
//******************************* MainWindow::MWBaseOnToggleFullScreen ******
// Accepts: void
//
// Purpose: switch between fullscreen and windowed mode
//
// Returns: void

LRESULT MainWindow::MWBaseOnToggleFullScreen( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_Mutex.Lock();
   mw.m_bGamePaused = true;
   if( mw.m_bFullScreen ) // go to windowed mode
   {
      int x_offset = (int)(::GetSystemMetrics( SM_CXSCREEN )*0.01),
          y_offset = (int)(::GetSystemMetrics( SM_CYSCREEN )*0.01);
      int width  = ::GetSystemMetrics( SM_CXSCREEN )-x_offset,
          height = ::GetSystemMetrics( SM_CYSCREEN )-y_offset;

      if( !::SetWindowLong( mw.m_WinBase, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE ) )
      {
//         mw.m_Console.AddLine( "Call to SetWindowLong Failed" );
      }
      if( !::SetWindowLong( mw.m_WinBase, GWL_EXSTYLE, WS_EX_APPWINDOW ) )
      {
//         mw.m_Console.AddLine( "Call to SetWindowLong Failed" );
      }
      if( ::ChangeDisplaySettings ( NULL, 0 ) != DISP_CHANGE_SUCCESSFUL )
      {
         mw.m_Console.AddLine( "Call to ChangeDisplaySettings Failed" );
      }
      if( mw.m_WinBase.Show( SW_NORMAL ) )
      {
      }
      if( ::SetWindowPos( mw.m_WinBase, HWND_TOPMOST, (int)(x_offset/2), (int)(y_offset/2), width, height, 0 ) )
      {
//         mw.m_Console.AddLine( "Call to SetWindowPos Failed" );
      }
      if( !::SetForegroundWindow( mw.m_WinBase ) )
      {
         mw.m_Console.AddLine( "Call to SetForegroundWindow Failed" );
      }
      mw.m_bFullScreen = false;
      mw.m_Console.AddLine( "FullScreen Off" );
   }      
   else // go to full screen mode
   {
      int width  = ::GetSystemMetrics( SM_CXSCREEN ),
          height = ::GetSystemMetrics( SM_CYSCREEN );
      int bits_per_pixel = 32;

      if( !::SetWindowLong( mw.m_WinBase, GWL_STYLE, WS_POPUP | WS_VISIBLE ) )
      {
//         mw.m_Console.AddLine( "Call to SetWindowLong Failed" );
      }
      if( !::SetWindowLong( mw.m_WinBase, GWL_EXSTYLE, WS_EX_TOPMOST ) )
      {
//         mw.m_Console.AddLine( "Call to SetWindowLong Failed" );
      }
      if( !ChangeScreenResolution( width, height, bits_per_pixel, CDS_FULLSCREEN ) )
      {
         mw.m_Console.AddLine( "Call to ChangeScreenResolution Failed" );
      }
      if( !mw.m_WinBase.Show( SW_NORMAL ) )
      {
      }
      if( ::SetWindowPos( mw.m_WinBase, HWND_TOPMOST, 0, 0, width, height, 0 ) )
      {
//         mw.m_Console.AddLine( "Call to SetWindowPos Failed" );
//         mw.m_Console.AddLine( GetLastSystemError() );
      }
      mw.m_bFullScreen = true;
      mw.m_Console.AddLine( "FullScreen On" );
   }
   mw.m_bGamePaused = false;
   mw.m_Mutex.Release();
   return 0;
}

LRESULT MainWindow::MWBaseOnClose( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   ::glFlush();
   mw.m_GLRC.DisableOpenGL();
   mw.StopServerThread();
   mw.StopClientThread();
//   exit(0);

   return ::PostMessage( mw, WM_QUIT, 0, 0 );
}

LRESULT MainWindow::MWBaseOnSysCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( wparam )
   {
      case SC_CLOSE:
      {
         ::glFlush();
         mw.m_GLRC.DisableOpenGL();
         mw.StopServerThread();
         mw.StopClientThread();

      } return ::PostMessage( mw, WM_QUIT, 0, 0 );
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_SYSCOMMAND, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnContextMouseOver( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_Mutex.Lock();
   if( mw.m_GameMode == TITLE_SCREEN )
   {
      mw.HandleListBoxMouseOverCollision( mw.m_TitleScreenOptionsListBox, mw.m_MousePosition );
   }
   if( mw.m_GameMode == CHOOSE_CLIENT_OR_SERVER_MODE )
   {
      mw.HandleListBoxMouseOverCollision( mw.m_ChooseClientOrServerListBox, mw.m_MousePosition );
   }
   if( mw.m_GameMode == VIEW_HIGH_SCORES )
   {
      mw.HandleListBoxMouseOverCollision( mw.m_HighScoresListBox, mw.m_MousePosition );
   }
   if( mw.m_GameMode == GET_SERVER_INFORMATION_FROM_USER )
   {
      mw.HandleListBoxMouseOverCollision( mw.m_GamesListBox, mw.m_MousePosition );
   }
   mw.ButtonMouseOverCollision();
   mw.m_Mutex.Release();
   return 0;
}

LRESULT MainWindow::MWBaseOnLDoubleClick( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   if( mw.m_GameMode == TITLE_SCREEN )
   {
      if( mw.HandleListBoxClickCollision( mw.m_TitleScreenOptionsListBox, mw.m_MousePosition ) )
      {
         mw.HandleTitleScreenKeys( VK_RETURN, 0, IGNORE_CONSOLE );
      }
   }
   if( mw.m_GameMode == CHOOSE_CLIENT_OR_SERVER_MODE )
   {
      if( mw.HandleListBoxClickCollision( mw.m_ChooseClientOrServerListBox, mw.m_MousePosition ) )
      {
         mw.HandleChooseClientOrServerKeys( VK_RETURN, 0, IGNORE_CONSOLE );
      }
   }
   if( mw.m_GameMode == VIEW_HIGH_SCORES )
   {
      if( mw.HandleListBoxClickCollision( mw.m_HighScoresListBox, mw.m_MousePosition ) )
      {
      }
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDBLCLK, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnContextLbtn(  MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   if( mw.m_GameMode == TITLE_SCREEN )
   {
      if( mw.HandleListBoxClickCollision( mw.m_TitleScreenOptionsListBox, mw.m_MousePosition ) )
      {
      }
   }
   if( mw.m_GameMode == CHOOSE_CLIENT_OR_SERVER_MODE )
   {
      if( mw.HandleListBoxClickCollision( mw.m_ChooseClientOrServerListBox, mw.m_MousePosition ) )
      {
      }
   }
   if( mw.m_GameMode == VIEW_HIGH_SCORES )
   {
      if( mw.HandleListBoxClickCollision( mw.m_HighScoresListBox, mw.m_MousePosition ) )
      {
      }
   }
   if( mw.m_GameMode == GET_SERVER_INFORMATION_FROM_USER )
   {
      if( mw.HandleListBoxClickCollision( mw.m_GamesListBox, mw.m_MousePosition ) )
      {
         std::list< std::string > row = mw.m_GamesListBox.GetSelectedIndex();
         //    0        1      2     3     4
         //  version   nick   ip   port   ping
         int index = 0;
         for( std::list< std::string >::iterator i = row.begin(); 
                i != row.end(); 
                i++, index++ )
         {
            if( index == 2 ) // IP
            {
               mw.m_RemoteHostAddress = *i;
               mw.m_Console.SetCurrentLine( *i );
            }
            else if( index == 3 ) // port
            {
               mw.m_Port = atoi( (*i).c_str() );
            }
         }
      }
   }
   mw.ButtonClickCollision();

   return 0;
}

LRESULT MainWindow::MWBaseOnMouseMove( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_Mutex.Lock();
   ::GetCursorPos( &mw.m_MousePosition );
   ::ScreenToClient( mw.m_WinBase, &mw.m_MousePosition );
   mw.m_MouseSStream.str( std::string() );
   Vector3D temp = WindowToGLCoordinates( mw.m_MousePosition.x, mw.m_MousePosition.y );

   mw.m_GLMousePosition.m_x = temp[0];
   mw.m_GLMousePosition.m_y = temp[1];
   mw.m_GLMousePosition.m_z = temp[2];
   mw.m_MouseSStream << "{ " << mw.m_GLMousePosition.m_x << ", " << mw.m_GLMousePosition.m_y << ", " << mw.m_GLMousePosition.m_z << " }";
   mw.m_Mutex.Release();

   mw.MWBaseOnContextMouseOver( mw, wparam, lparam );
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_MOUSEMOVE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnChar( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   if( wparam == '`' )
   {
      TOGGLE( mw.m_bShowTheConsole );
      if( !mw.m_bShowTheConsole )
      {
         mw.m_bTheConsoleAcceptsTyping = false;
      }
      else
      {
         mw.m_bTheConsoleAcceptsTyping = true;
      }
   }
   else if( (wparam == '\r') && mw.m_bTheConsoleAcceptsTyping )
   {
      mw.m_Mutex.Lock();
      mw.m_Console.EndLine( mw.m_Nick );
      if( mw.m_bNetworkActive && mw.m_Console.GetLastLineAdded()[0] != '/' )
      {
         std::string text = mw.m_Nick + ": " + mw.m_Console.GetLastLineAdded();

         if( mw.m_Console.GetLastLineAdded().size() )
         {
            if( mw.m_Server != NULL )
            {
               mw.SendToClient( (char*)text.c_str(), text.length(), 0,
                                ENET_CHANNEL_TEXT, ENET_PACKET_FLAG_RELIABLE );
            }
            else// if( mw.m_Client != NULL )
            {
               mw.SendToHost( (char*)text.c_str(), text.length(), 
                              ENET_CHANNEL_TEXT, ENET_PACKET_FLAG_RELIABLE );
            }
         }
      }
      mw.CheckCommand( mw.m_Console.GetLastLineAdded() );
      if( mw.m_ConsoleInput != GET_NOTHING )
         mw.GetText( mw.m_Console.GetLastLineAdded() );
      mw.m_Mutex.Release();
   }
   else if( (wparam != '\n') && mw.m_bTheConsoleAcceptsTyping )
   {
      mw.m_Mutex.Lock();
      if( !mw.m_Console.AddChar( (char)wparam ) )
      {
         MsgBeep( 0xFFFFFFFF );
      }
      mw.m_Mutex.Release();
   }

   return ::DefWindowProc( mw.m_LastHwndPassed, WM_CHAR, wparam, lparam );
}

