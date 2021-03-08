//#include <unistd.h>
#include <sstream>
#include <cmath>

#include "C_Line.h"
#include "C_Dot.h"
#include "resource.h"

#include "../../projects.h"

// TODO (#1#): Get the AI TO MISS. You have to fix the 
//               MovePaddleTo function so the AI can only move the 
//               paddle by a certain amount like a human 
// TODO (#1#): Add demo option to the menu 



static double initialpaddlevelocity        = 0.5f;
static double initialballspeed             = 0.4f;

static double initialball_y_speed          = 0.4f;
static double initialball_x_speed          = 0.4f;

//static double initialpaddlespeed           = 3.0f;
static double initialballxacceleration     = 0.000055;
static double initialballyacceleration     = 0.000005;
static int    initialballxoffset           = 4;
static int    paddlekeyrepeatcountinterval = 10; // miliseconds
static int    maxscore                     = 10;

class MainWindow: public CWinMain
{
   public:

   enum DIFFICULTY
   {
      IMPOSSIBLE = 0,
      HARD,
      MEDIUM,
      EASY
   };

      MainWindow( void ) :
         m_Line1( 100 ),
         m_Line2( 100 ),
         m_ball_y_acceleration( initialballyacceleration ),
         m_ball_x_acceleration( initialballxacceleration ),
         m_ball_y_velocity( initialball_y_speed ),
         m_ball_x_velocity( initialball_x_speed ),
         m_ball_y_torque( 0.0f ), 
         m_ball_x_torque( 0.0f ),
         m_PenWidth( 5 ),
         m_P1Score( 0 ), 
         m_P2Score( 0 ),
         m_FrameDeltaTime( 0 ),
         m_FrameDeltaTimeSquared( 0 ),
         m_paddle1_y_velocity( initialpaddlevelocity ),
         m_paddle2_y_velocity( initialpaddlevelocity ),
         m_x_initialdisplacement( 0 ), 
         m_y_initialdisplacement( 0 ),
         m_TimeSinceRoundStarted( 0 ),
         m_halfClientRectright( 0 ),
         m_halfClientRectbottom( 0 ),
         m_bisVisible( false ),
         m_bQuit( false ),
         m_bGamePaused( false ),
         m_bP1Turn( true ),
         m_bGameInProgress( false ),
         m_bBallServed( false ),
         m_bP1Computer( false ), 
         m_bP2Computer( false ),
         m_TimeSincePaddleMoved( 0 ),
         m_RoundNumber( 0 ),
         m_bSoundOn( true ),
         m_bGameOver( false ),
         m_MaxScore( maxscore ),
         m_AdditionalServeVelocity( 0 ),
         m_RandomError( 0 )
      {
         AddMessageHandler( WM_SIZE,        reinterpret_cast<MessageHandler>(MWBaseOnSize)     );
         AddMessageHandler( WM_CREATE,      reinterpret_cast<MessageHandler>(MWBaseOnCreate)   );
         AddMessageHandler( WM_LBUTTONDOWN, reinterpret_cast<MessageHandler>(MWBaseOnLButtonD) );
         AddMessageHandler( WM_RBUTTONDOWN, reinterpret_cast<MessageHandler>(MWBaseOnRButtonD) );
         AddMessageHandler( WM_COMMAND,     reinterpret_cast<MessageHandler>(MWBaseOnCommand)  );
         AddMessageHandler( WM_KEYDOWN,     reinterpret_cast<MessageHandler>(MWBaseOnKeyDown)  );
         AddMessageHandler( WM_KEYUP,       reinterpret_cast<MessageHandler>(MWBaseOnKeyUp)    );
         AddMessageHandler( WM_CLOSE,       reinterpret_cast<MessageHandler>(MWBaseOnClose)    );

         m_Hpen.Create( PS_SOLID, m_PenWidth, RGB( 255, 255, 255 ) );
         LoadMenu();
         if( !m_Fmod.LoadClip( "bounce.wav" ) )
            LogError( "bounce.wav not found" );
         if( !m_Fmod.LoadClip( "newgame.wav" ) )
            LogError( "newgame.wav not found" );
         if( !m_Fmod.LoadClip( "boing.wav" ) )
            LogError( "boing.wav not found" );
         m_Keys.SetRepeatCountInterval( VK_UP,   paddlekeyrepeatcountinterval );
         m_Keys.SetRepeatCountInterval( VK_DOWN, paddlekeyrepeatcountinterval );
      }

      virtual ~MainWindow( void )
      {
      }

      static LRESULT MWBaseOnDemoClose( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         ::PostMessage( mw, WM_QUIT, 0, 0 ); // First End Demo Loop
         ::PostMessage( mw, WM_QUIT, 0, 0 ); // Next End Title Loop
         return ::PostMessage( mw, WM_QUIT, 0, 0 ); // Then  End Game  Loop
      }
      
      static LRESULT MWBaseOnGameOverClose( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         ::PostMessage( mw, WM_QUIT, 0, 0 ); // First End GameOver Loop
         ::PostMessage( mw, WM_QUIT, 0, 0 ); // Next End DemoLoop Loop
         return ::PostMessage( mw, WM_QUIT, 0, 0 ); // Then  End Game  Loop
      }

      static LRESULT MWBaseOnTitleClose( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         ::PostMessage( mw, WM_QUIT, 0, 0 ); // First End Title Loop
         ::PostMessage( mw, WM_QUIT, 0, 0 ); // First End Demo Loop
         return ::PostMessage( mw, WM_QUIT, 0, 0 ); // Then  End Game  Loop
      }

      static LRESULT MWBaseOnClose( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
//         ::DefWindowProc( mw.m_LastHwndPassed, WM_CLOSE, wparam, lparam );
         return ::PostMessage( mw, WM_QUIT, 0, 0 );
      }

      static LRESULT MWBaseOnDemoKeyDown( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.m_Keys.PressKey( wparam );
         mw.DoDemoKeyActions( wparam, lparam );
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYDOWN, wparam, lparam );
      }

      static LRESULT MWBaseOnGameOverKeyDown( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.m_Keys.PressKey( wparam );
         mw.DoGameOverKeyActions( wparam, lparam );
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYDOWN, wparam, lparam );
      }

      static LRESULT MWBaseOnTitleKeyDown( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.m_Keys.PressKey( wparam );
         mw.DoTitleKeyActions( wparam, lparam );
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYDOWN, wparam, lparam );
      }

      static LRESULT MWBaseOnKeyDown( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.m_Keys.PressKey( wparam );
         mw.DoGameKeyActions( wparam, lparam );
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYDOWN, wparam, lparam );
      }
      static LRESULT MWBaseOnKeyUp( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.m_Keys.ReleaseKey( wparam );
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYUP, wparam, lparam );
      }

      static LRESULT MWBaseOnGameOverCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         switch( LOWORD(wparam) )
         {
            case 1000:
            {
               ::PostMessage( mw, WM_QUIT, 0, 0 ); // End Title Loop
            } break;
            case 1001:
            {
               ::SendMessage( mw, WM_CLOSE, 0, 0 );
            } break;
            case 1007:
            {
               mw.ToggleSound();
            } break;
            case 1008:
            {
               mw.m_bGamePaused = true;
               Alert( "You need instructions on how to play pong!?" );
               mw.m_bGamePaused = false;
            } break;
            case 1009:
            {
               mw.m_bGamePaused = true;
               Alert( "   Spectial thanks to:\r\nTinyXML\r\nDev C++\r\nAfterNet #gamedev" );
               mw.m_bGamePaused = false;
            } break;
            case 1010:
            {
               mw.m_bGamePaused = true;
               Alert( "Pong! By try_catch_this" );
               mw.m_bGamePaused = false;
            } break;
         }
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
      }

      static LRESULT MWBaseOnTitleCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         switch( LOWORD(wparam) )
         {
            case 1000:
            {
               ::PostMessage( mw, WM_QUIT, 0, 0 ); // End Title Loop
            } break;
            case 1001:
            {
               ::SendMessage( mw, WM_CLOSE, 0, 0 );
            } break;
            case 1007:
            {
               mw.ToggleSound();
            } break;
            case 1008:
            {
               mw.m_bGamePaused = true;
               Alert( "You need instructions on how to play pong!?" );
               mw.m_bGamePaused = false;
            } break;
            case 1009:
            {
               mw.m_bGamePaused = true;
               Alert( "   Spectial thanks to:\r\nTinyXML\r\nDev C++\r\nAfterNet #gamedev" );
               mw.m_bGamePaused = false;
            } break;
            case 1010:
            {
               mw.m_bGamePaused = true;
               Alert( "Pong! By try_catch_this" );
               mw.m_bGamePaused = false;
            } break;
         }
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
      }

      static LRESULT MWBaseOnDemoCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         switch( LOWORD(wparam) )
         {
            case 1000:
            {
               ::PostMessage( mw, WM_QUIT, 0, 0 ); // End Title Loop
            } break;
            case 1001:
            {
               ::SendMessage( mw, WM_CLOSE, 0, 0 );
            } break;
            case 1007:
            {
               mw.ToggleSound();
            } break;
            case 1008:
            {
               mw.m_bGamePaused = true;
               Alert( "You need instructions on how to play pong!?" );
               mw.m_bGamePaused = false;
            } break;
            case 1009:
            {
               mw.m_bGamePaused = true;
               Alert( "   Spectial thanks to:\r\nTinyXML\r\nDev C++\r\nAfterNet #gamedev" );
               mw.m_bGamePaused = false;
            } break;
            case 1010:
            {
               mw.m_bGamePaused = true;
               Alert( "Pong! By try_catch_this" );
               mw.m_bGamePaused = false;
            } break;
         }
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
      }

      static LRESULT MWBaseOnCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         switch( LOWORD(wparam) )
         {
            case 1000:
            {
              mw.StartNewGame();
            } break;
            case 1001:
            {
               ::SendMessage( mw, WM_CLOSE, 0, 0 );
            } break;
            case 1007:
            {
               mw.ToggleSound();
            } break;
            case 1008:
            {
               mw.m_bGamePaused = true;
               Alert( "You need instructions on how to play pong!?" );
               mw.m_bGamePaused = false;
            } break;
            case 1009:
            {
               mw.m_bGamePaused = true;
               Alert( "   Spectial thanks to:\r\nTinyXML\r\nDev C++\r\nAfterNet #gamedev" );
               mw.m_bGamePaused = false;
            } break;
            case 1010:
            {
               mw.m_bGamePaused = true;
               Alert( "Pong! By try_catch_this" );
               mw.m_bGamePaused = false;
            } break;
         }
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
      }
      static LRESULT MWBaseOnRButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONDOWN, wparam, lparam );
      }
      static LRESULT MWBaseOnLButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDOWN, wparam, lparam );
      }
      static LRESULT MWBaseOnCreate( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         mw.Setm_WinBaseDCHWND();
         mw.Setm_MemoryDCHWND();
         mw.SetObjectsMemoryDC();
         mw.SetMenu();
         mw.Selectm_HpenTom_MemoryDC();
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_CREATE, wparam, lparam );
      }

      static LRESULT MWBaseOnDemoSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         switch( wparam )
         {
            case SIZE_MINIMIZED:
            {
               mw.m_bisVisible = false;
            } break;
            default:
            {
               mw.m_bisVisible = true;
               mw.SetClientRect();
               mw.SetScorePosition();
               mw.SetInitialPaddlePositions();
               mw.BallPositionOnSize();
               mw.m_MemoryDC.CreateBitmap( mw.m_LastHwndPassed, mw.m_ClientRect );
               mw.Selectm_HpenTom_MemoryDC();
            }
         }
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
      }

      static LRESULT MWBaseOnTitleSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         switch( wparam )
         {
            case SIZE_MINIMIZED:
            {
               mw.m_bisVisible = false;
            } break;
            default:
            {
               mw.m_bisVisible = true;
               mw.SetClientRect();
               mw.SetScorePosition();
               mw.SetInitialPaddlePositions();
               mw.m_MemoryDC.CreateBitmap( mw.m_LastHwndPassed, mw.m_ClientRect );
               mw.Selectm_HpenTom_MemoryDC();
            }
         }
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
      }

      static LRESULT MWBaseOnGameOverSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         switch( wparam )
         {
            case SIZE_MINIMIZED:
            {
               mw.m_bisVisible = false;
            } break;
            default:
            {
               mw.m_bisVisible = true;
               mw.SetClientRect();
               mw.m_MemoryDC.CreateBitmap( mw.m_LastHwndPassed, mw.m_ClientRect );
               mw.Selectm_HpenTom_MemoryDC();
            }
         }
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
      }

      static LRESULT MWBaseOnSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
      {
         switch( wparam )
         {
            case SIZE_MINIMIZED:
            {
               mw.m_bisVisible = false;
            } break;
            default:
            {
               mw.m_bisVisible = true;
               mw.SetClientRect();
               mw.SetScorePosition();
               mw.SetRoundPosition();
               mw.SetInitialPaddlePositions();
               mw.BallPositionOnSize();
               mw.m_MemoryDC.CreateBitmap( mw.m_LastHwndPassed, mw.m_ClientRect );
               mw.Selectm_HpenTom_MemoryDC();
            }
         }
         return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
      }

   void DrawAdditionalServeVelocity( void );
   int  GameOverLoop( DWORD );
   int  RunGameLoop( void );
   int  DemoLoop( void );
   int  TitleScreenLoop( void );
   void DrawServeDirections( void );
   void DrawGamePaused( void );
   void DrawGameObjects( void );
   void DrawGameOver( void );

   void DoGameKeyActions( WPARAM wparam, LPARAM lparam );
   void DoTitleKeyActions( WPARAM wparam, LPARAM lparam );
   void DoDemoKeyActions( WPARAM wparam, LPARAM lparam );
   void DoGameOverKeyActions( WPARAM wparam, LPARAM lparam );

   void ResetBallVelocityandAcceleration( void );
   void ReverseBall_x_Direction( void );
   void ReverseBall_y_Direction( void );
   bool ReverseBallDirectionOn_x_Collision( void);
   bool ReverseBallDirectionOn_y_Collision( void);
   void RestrictPaddleToGameBoundaries( C_Line& Paddle );
   void MovePaddleToBallPosition( C_Line& );
   int  paddle1_y( DWORD time );
   int  paddle2_y( DWORD time );
   void ResetGameState( void );
   void ProcessTitleKeys( void );
   void ProcessGameKeys( void );
   void DrawTitleScreen( void );
   void DrawDemoScreen( void );
   void DrawGameOverScreen();
   void DrawMainGameLoop( void );
   void DrawRound( void );
   void DrawScore( void );
   void LoadMenu( void );
   void UpdateDemoObjects( void );
   void UpdateObjects( void );
   bool HandleCollisions( void );
//   void MoveBall( bool bResetIntervalTIme = false );
   void MoveBall( void );
   void MovePaddle1( void );
   void MovePaddle2( void );
   void MovePaddle1To( const long &y_pos, const DWORD& deltaTime );
   void MovePaddle2To( const long &y_pos, const DWORD& deltaTime );
   void FollowPaddleForServe( void );
   void StartNewGame( void );
   void StartNewRound( void );
   void SetTimeSinceRoundStarted( void );
   void SetTimeSincePaddleMoved( void );
   void PlayEffect( short );
   long PredictBallPositionAtBoundary( void );
//   void InterceptBallWithPaddle1( bool bResetTimer=false );
   void InterceptBallWithPaddle1( void );
   void InterceptBallWithPaddle2( void );
//   void InterceptBallWithPaddle2( bool bResetTimer=false );
   void ProcessDemoKeys( void );
   void AIServeBall( void );
   bool CheckForEndGameCondition( void );
   void SetRandomError( void );
   void ApplyForceToBallFromPaddle1( void );
   void ApplyForceToBallFromPaddle2( void );



   void BallPositionOnSize( void )
   {
      m_Dot.m_point.x = m_halfClientRectright;
      m_Dot.m_point.y = m_halfClientRectbottom;
   }
   void ToggleSound( void )
   {
      (m_bSoundOn) ? m_bSoundOn = false: m_bSoundOn = true;
   }
   void SetMenu( void )
   {
      m_XMLMenu.SetMenu( (m_WinBase) ? m_WinBase : m_LastHwndPassed );
   }
   void RemoveMenu( void )
   {
      ::SetMenu( (m_WinBase)? m_WinBase : m_LastHwndPassed, NULL );
   }
   void Setm_MemoryDCHWND( void )
   {
      m_MemoryDC.SetHWND( (m_WinBase)? m_WinBase: m_LastHwndPassed );
   }
   void Setm_WinBaseDCHWND( void )
   {
      m_WinBaseDC.SetHWND( (m_WinBase)? m_WinBase: m_LastHwndPassed );
   }
   void SetObjectsMemoryDC( void )
   {
      m_Line1.m_memDC = &m_MemoryDC;
      m_Line2.m_memDC = &m_MemoryDC;
      m_Dot.m_memDC   = &m_MemoryDC;
   }
   void SetClientRect( void )
   {
      ::GetClientRect( m_LastHwndPassed, &m_ClientRect );
      m_halfClientRectright  = (int)(m_ClientRect.right*0.5);
      m_halfClientRectbottom = (int)(m_ClientRect.bottom*0.5);
   }
   void TextOut( const long& x, const long& y, const std::string& message )
   {
      if( !::TextOut( m_MemoryDC, x, y, message.c_str(), message.size() ) )
         throw 0;
   }
   void TextOut( const long& x, const long& y, const char* message )
   {
      if( !::TextOut( m_MemoryDC, x, y, message, strlen(message) ) )
         throw 0;
   }
   void Selectm_HpenTom_MemoryDC( void )
   {
      m_Hpen.SelectToDC( m_MemoryDC );
   }
   void TogglePause( void )
   {
      ( m_bGamePaused ) ? m_bGamePaused = false : m_bGamePaused = true;
   }
   void ToggleTurns( void )
   {
      (m_bP1Turn) ? m_bP1Turn = false: m_bP1Turn = true;
   }
   void ToggleServed( void )
   {
      (m_bBallServed) ? m_bBallServed = false : m_bBallServed = true;
   }
   void SetScorePosition()
   {
      m_P1ScorePos.y = 0;
      m_P2ScorePos.y = 0;      
      m_P1ScorePos.x = (long)(0.05*m_ClientRect.right);
      m_P2ScorePos.x = (long)(m_ClientRect.right-m_MemoryDC.GetTextDimensions( m_P2ScoreString.str() ).cx-(0.05*m_ClientRect.right));
   }
   void SetInitialObjectPositions( void )
   {
      SetInitialPaddlePositions();
      if( m_bP1Turn )
      {
         m_Dot.m_point.x = m_Line1.m_point.x+initialballxoffset;
         m_Dot.m_point.y = m_Line1.m_point.y;
      }
      else
      {
         m_Dot.m_point.x = m_Line2.m_point.x-initialballxoffset;
         m_Dot.m_point.y = m_Line2.m_point.y;
      }
   }
   void SetInitialPaddlePositions( void )
   {
      m_Line1.m_point.x = 4;
      m_Line1.m_point.y = (int)(m_ClientRect.bottom/2);
      m_Line2.m_point.x = m_ClientRect.right-4;
      m_Line2.m_point.y = (int)(m_ClientRect.bottom/2);
   }
   void SaveInitialBallDisplacement( void )
   {
      m_x_initialdisplacement = m_Dot.m_point.x;
      m_y_initialdisplacement = m_Dot.m_point.y;
   }
   bool ScoreChanged( void )
   {
      static int p1prevscore = m_P1Score;
      static int p2prevscore = m_P2Score;
      if( (p1prevscore != m_P1Score) || (p2prevscore != m_P2Score) )
         return true;
      return false;
   }
   void SetScore( void )
   {
      m_P1ScoreString.str(std::string());
      m_P2ScoreString.str(std::string());
      m_P1ScoreString << "Player 1 Score: ";
      m_P2ScoreString << "Player 2 Score: ";
      m_P1ScoreString << m_P1Score;
      m_P2ScoreString << m_P2Score;
      SetScorePosition();
   }
   void SetRoundPosition( void )
   {
      m_RoundPosition.x = m_halfClientRectright-(int)(m_MemoryDC.GetTextDimensions( m_RoundString.str() ).cx*0.5);
      m_RoundPosition.y = 0;
   }
   void SetRound( void )
   {
      m_RoundString.str(std::string());
      m_RoundString << "Round: ";
      m_RoundString << m_RoundNumber;
      SetRoundPosition();
   }


// Collision Detection
   bool CheckColideWithPaddle1( void )
   {
      if( (m_Dot.m_point.y < m_Line1.m_point.y) || 
          (m_Dot.m_point.y > m_Line1.m_point.y+(m_Line1.m_LineLength)) )
         return false;
      return true;
   }

   bool CheckColideWithPaddle2( void )
   {
      if( (m_Dot.m_point.y < m_Line2.m_point.y) || 
          (m_Dot.m_point.y > (m_Line2.m_point.y+m_Line2.m_LineLength)) )
         return false;
      return true;
   }

   bool CheckCollide( const Structs::Rect& Rect1, const Structs::Rect& Rect2 )
   {
      if( (Rect1.bottom < Rect2.top) || (Rect1.top > Rect2.bottom) ) return false;
      if( (Rect1.right < Rect2.left) || (Rect1.left > Rect2.right) ) return false;
      return true;
   }

// End Collision Detection

// Path calculation
   int s_wrt_x( DWORD time, int );
   int s_wrt_y( DWORD time, int );
// End Path calculation

   protected:
      CFmod              m_Fmod;
      KeyInput           m_Keys;
      CHPen             m_Hpen;
      C_Line             m_Line1, m_Line2;
      C_Dot              m_Dot;
      FrameRate          m_FrameRate;
      CXMLMenu          m_XMLMenu;

      C_MemoryDC         m_MemoryDC;
      C_GetDC            m_WinBaseDC;

//      HDCs               m_DCs;      m_DCs.SetHwnd( m_WinBase );

      Structs::Rect    m_ClientRect, m_PlayingAreaRect;
      Structs::Point   m_P1ScorePos, m_P2ScorePos, m_RoundPosition;
      TimeInterval     m_deltaTimeBallServed;
      std::ostringstream m_P1ScoreString, m_P2ScoreString, m_RoundString;
      DWORD              m_TimeSinceRoundStarted, m_TimeSincePaddleMoved, m_FrameDeltaTime, m_FrameDeltaTimeSquared;
      int                m_PenWidth, m_RoundNumber, m_RandomError;
      double             m_ball_x_acceleration, m_ball_y_acceleration, m_paddle1_y_velocity, m_paddle2_y_velocity, m_ball_y_velocity, m_ball_x_velocity,
                         m_AdditionalServeVelocity, m_initial_ball_x_velocity, m_ball_x_velocity_at_paddle, m_ball_y_velocity_at_paddle, m_ball_y_torque, m_ball_x_torque;
      int                m_P1Score, m_P2Score, m_x_initialdisplacement, m_y_initialdisplacement, m_MaxScore, m_halfClientRectright, m_halfClientRectbottom;
      bool               m_bQuit, m_bisVisible, m_bGamePaused, m_bP1Turn, m_bGameInProgress, m_bBallServed, m_bSoundOn, m_bGameOver, m_bP1Computer, m_bP2Computer;

   private:
};

void MainWindow::MovePaddle1To( const long &y_pos, const DWORD& deltaTime )
{
   if( y_pos > (m_Line1.m_point.y+m_Line1.m_halfLineLength) )
   {
      m_Line1.m_point.y += paddle1_y(deltaTime);
   }
   else if( y_pos < (m_Line1.m_point.y+m_Line1.m_halfLineLength) )
   {
      m_Line1.m_point.y -= paddle1_y(deltaTime);
   }
}

void MainWindow::MovePaddle2To( const long &y_pos, const DWORD& deltaTime )
{
   if( y_pos > (m_Line2.m_point.y+m_Line2.m_halfLineLength) )
   {
      m_Line2.m_point.y += paddle2_y(deltaTime);
   }
   else if( y_pos < (m_Line2.m_point.y+m_Line2.m_halfLineLength) )
   {
      m_Line2.m_point.y -= paddle2_y(deltaTime);
   }
}


void MainWindow::SetRandomError( void )
{
   int error = 50;
   if( (fabsf(m_ball_x_velocity) > 0.5) || (fabsf(m_ball_y_velocity) > 0.5) )
      error = 55;
   else if( (fabsf(m_ball_x_velocity) > 0.6) || (fabsf(m_ball_y_velocity) > 0.6) )
      error = 60;
   else if( (fabsf(m_ball_x_velocity) > 0.7) || (fabsf(m_ball_y_velocity) > 0.7) )
      error = 65;
   m_RandomError = (rand()%error);

   m_RandomError *= (rand()%2) ? -1 : 1;
//      IMPOSSIBLE = 0,
//      HARD,
//      MEDIUM,
//      EASY
}

void MainWindow::RestrictPaddleToGameBoundaries( C_Line& Paddle )
{
   if( Paddle.m_point.y < 0 ) Paddle.m_point.y = 0;
   if( Paddle.m_point.y > (m_ClientRect.bottom-Paddle.m_LineLength) ) Paddle.m_point.y = m_ClientRect.bottom-Paddle.m_LineLength;
}

bool MainWindow::CheckForEndGameCondition( void )
{
   if( (m_P1Score >= m_MaxScore) || (m_P2Score >= m_MaxScore) )
   {
      return true;
   }
   return false;
}

void MainWindow::AIServeBall( void )
{
   if( !m_bBallServed )
   {
      if( m_bP1Turn && m_bP1Computer )
      {
         ::PostMessage( m_WinBase, WM_KEYDOWN, VK_RIGHT, 0 );
         ::PostMessage( m_WinBase, WM_KEYUP,   VK_RIGHT, 0 );
      }
      else if( !m_bP1Turn && m_bP2Computer )
      {
         ::PostMessage( m_WinBase, WM_KEYDOWN, VK_LEFT, 0 );
         ::PostMessage( m_WinBase, WM_KEYUP,   VK_LEFT, 0 );
      }
   }
}

void MainWindow::InterceptBallWithPaddle1( void )
{
   if( (m_ball_x_velocity < 0) )
   {
      MovePaddle1To( PredictBallPositionAtBoundary()+m_RandomError, m_FrameDeltaTime );
   }
   else // Move paddle towards center
   {
      MovePaddle1To( m_halfClientRectbottom, m_FrameDeltaTime );
   }
}

/*
void MainWindow::InterceptBallWithPaddle1( bool bResetTimer )
{
   static DWORD t = 0;
   static TimeInterval Interval( (DWORD)m_Keys.GetRepeatCountInterval( VK_DOWN ) );

   if( bResetTimer )
   {
//      Interval.ResetTime();
      Interval.Begin();
   }
   else
   {
      if( Interval.IntervalExceeded() )
      {
         if( (m_ball_x_velocity < 0) )
         {
            t = Interval.GetInterval()*Interval.GetIntervalsPassed();
//            if( m_paddle1_y_velocity < 0 ) m_paddle1_y_velocity *= -1;
            MovePaddle1To( PredictBallPositionAtBoundary()+m_RandomError, t );
         }
         else // Move paddle towards center
         {
            if( Interval.IntervalExceeded() )
            {
               t = Interval.GetInterval()*Interval.GetIntervalsPassed();
               MovePaddle1To( m_halfClientRectbottom, t );
            }
         }
      }
//(Interval.TimeSinceLastCall() - t)
      Interval.Begin();
   }
}
*/
/*
void MainWindow::InterceptBallWithPaddle1( bool bResetTimer )
{
   static DWORD t = 0;
   static TimeInterval Interval( (DWORD)m_Keys.GetRepeatCountInterval( VK_DOWN ) );

   if( bResetTimer )
      Interval.Begin();
   else
   {
      if( Interval.IntervalExceeded() )
      {
         if( (m_ball_x_velocity < 0) )
         {
            t = Interval.GetInterval()*Interval.GetIntervalsPassed();
            if( m_paddle1_y_velocity < 0 ) m_paddle1_y_velocity *= -1;
            if( (PredictBallPositionAtBoundary()-(int)(m_Line1.m_LineLength/2)+m_RandomError) > m_Line1.m_point.y )
            {
               m_Line1.m_point.y += paddle1_y(t);
            }
            else if( (PredictBallPositionAtBoundary()-(int)(m_Line1.m_LineLength/2)+m_RandomError) < m_Line1.m_point.y )
            {
               m_Line1.m_point.y -= paddle1_y(t);
            }
         }
         else // Move paddle towards center
         {
            if( Interval.IntervalExceeded() )
            {
               t = Interval.GetInterval()*Interval.GetIntervalsPassed();
               if( (m_ClientRect.bottom/2) > (m_Line1.m_point.y+(m_Line1.m_LineLength/2)) )
               {
                  m_Line1.m_point.y += paddle1_y(t);
               }
               else if( (m_ClientRect.bottom/2) < (m_Line1.m_point.y+(m_Line1.m_LineLength/2)) )
               {
                  m_Line1.m_point.y -= paddle1_y(t);
               }
            }
         }
      }
      Interval.Begin();
   }
}
*/

void MainWindow::InterceptBallWithPaddle2( void )
{
   if( (m_ball_x_velocity > 0) ) // if the ball is moving towards this paddle
   {
      MovePaddle2To( PredictBallPositionAtBoundary()+m_RandomError, m_FrameDeltaTime );
   }
   else // Move paddle towards center
   {
      MovePaddle2To( m_halfClientRectbottom, m_FrameDeltaTime );
   }
}

/*
void MainWindow::InterceptBallWithPaddle2( bool bResetTimer )
{
   static DWORD t = 0;
   static TimeInterval Interval( (DWORD)m_Keys.GetRepeatCountInterval( VK_DOWN ) );

   if( bResetTimer )
      Interval.Begin();
   else
   {
      if( Interval.IntervalExceeded() )
      {
         if( (m_ball_x_velocity > 0) ) // if the ball is moving towards this paddle
         {
            t = Interval.GetInterval()*Interval.GetIntervalsPassed();
//            if( m_paddle2_y_velocity < 0 ) m_paddle2_y_velocity *= -1;
            MovePaddle2To( PredictBallPositionAtBoundary()+m_RandomError, t );
         }
         else // Move paddle towards center
         {
            if( Interval.IntervalExceeded() )
            {
               t = Interval.GetInterval()*Interval.GetIntervalsPassed();
               MovePaddle2To( m_halfClientRectbottom, t );
            }
         }
      }
      Interval.Begin();
   }
}
*/
/*
void MainWindow::InterceptBallWithPaddle2( bool bResetTimer )
{
   static DWORD t = 0;
   static TimeInterval Interval( (DWORD)m_Keys.GetRepeatCountInterval( VK_DOWN ) );
   if( bResetTimer )
      Interval.Begin();
   else
   {
      if( !(m_ball_x_velocity < 0) )
      {
         if( Interval.IntervalExceeded() )
         {
            t = Interval.GetInterval()*Interval.GetIntervalsPassed();
            if( m_paddle2_y_velocity < 0 ) m_paddle2_y_velocity *= -1;
            if( (PredictBallPositionAtBoundary()-(int)(m_Line2.m_LineLength/2)+m_RandomError) > m_Line2.m_point.y )
            {
               m_Line2.m_point.y += paddle2_y(t);
            }
            else if( (PredictBallPositionAtBoundary()-(int)(m_Line2.m_LineLength/2)+m_RandomError) < m_Line2.m_point.y )
            {
               m_Line2.m_point.y -= paddle2_y(t);
            }
         }
      }
      else // Move paddle towards center
      {
         if( Interval.IntervalExceeded() )
         {
            t = Interval.GetInterval()*Interval.GetIntervalsPassed();
            if( (m_ClientRect.bottom/2) > (m_Line2.m_point.y+(m_Line2.m_LineLength/2)) )
            {
               m_Line2.m_point.y += paddle2_y(t);
            }
            else if( (m_ClientRect.bottom/2) < (m_Line2.m_point.y+(m_Line2.m_LineLength/2)) )
            {
               m_Line2.m_point.y -= paddle2_y(t);
            }
         }
      }
      Interval.Begin();
   }
}
*/

long MainWindow::PredictBallPositionAtBoundary( void )
{
//   int s = m_ClientRect.right;//(int)((m_ball_speed * m_TimeSinceRoundStarted) + (0.5*m_ball_x_acceleration*m_TimeSinceRoundStarted*m_TimeSinceRoundStarted)) + m_x_initialdisplacement;
//   DWORD time = (DWORD)( ( sqrt( (m_ball_speed*m_ball_speed)+2*m_ball_x_acceleration*s ) - m_ball_speed )/m_ball_x_acceleration );
//   if( (m_Line2.m_point.x <= m_Dot.m_point.x) )
//      return s_wrt_y( time, m_y_initialdisplacement );
//  else
//      return m_Dot.m_point.y ;
//   long ball_x_displacement_to_boundary = ( m_ball_x_velocity < 0 ) ? m_Dot.m_point.x - m_Line1.m_point.x : m_Line2.m_point.x - m_Dot.m_point.x;
//   DWORD time_till_ball_collides_with_x_boundary = (DWORD)(((sqrt( (m_ball_x_velocity*m_ball_x_velocity) + (2*m_ball_x_acceleration*ball_x_displacement_to_boundary) ) * (double)((m_ball_y_velocity < 0) ? -1.0f: 1.0f))-m_ball_x_velocity)/m_ball_x_acceleration);
//   double m_ball_y_velocity_at_collision_with_x_boundary = m_ball_y_velocity + m_ball_y_acceleration*time_till_ball_collides_with_x_boundary;
//   return ((long)((m_ball_y_velocity*time_till_ball_collides_with_x_boundary) + (0.5*m_ball_y_acceleration*time_till_ball_collides_with_x_boundary*time_till_ball_collides_with_x_boundary)))%m_ClientRect.bottom;

//   long ball_x_displacement_to_boundary = ( m_ball_x_velocity < 0 ) ? m_Dot.m_point.x - m_Line1.m_point.x : m_Line2.m_point.x - m_Dot.m_point.x;
//   double m_ball_x_velocity_at_collision_with_x_boundary = sqrt( ( m_initial_ball_x_velocity*m_initial_ball_x_velocity ) + ( 2 * m_ball_x_acceleration * (m_Line2.m_point.x-m_Line1.m_point.x) ) ) * (double)((m_initial_ball_x_velocity < 0) ? -1.0f: 1.0f) ;
//   double time_till_ball_collides_with_x_boundary = fabsf(( m_ball_x_velocity_at_collision_with_x_boundary - m_initial_ball_x_velocity )/m_ball_x_acceleration);
//   long y_displacement_at_boundary = (((long)( (0.4f*time_till_ball_collides_with_x_boundary) + (0.5*fabsf(m_ball_y_acceleration)*(time_till_ball_collides_with_x_boundary*time_till_ball_collides_with_x_boundary)) ))%m_ClientRect.bottom) + 0; //(m_ClientRect.bottom - ((m_bBallServed) ? m_y_initialdisplacement : m_Dot.m_point.y))
//   return y_displacement_at_boundary;

//   long ball_x_displacement_to_boundary = ( m_ball_x_velocity < 0 ) ? m_Dot.m_point.x - m_Line1.m_point.x : m_Line2.m_point.x - m_Dot.m_point.x;
//   double m_ball_x_velocity_at_collision_with_x_boundary = sqrt( ( m_initial_ball_x_velocity*m_initial_ball_x_velocity ) + ( 2 * m_ball_x_acceleration * (m_Line2.m_point.x-m_Line1.m_point.x) ) ) * (double)((m_initial_ball_x_velocity < 0) ? -1.0f: 1.0f) ;
//   double time_till_ball_collides_with_x_boundary = fabsf(( m_ball_x_velocity_at_collision_with_x_boundary - m_initial_ball_x_velocity )/m_ball_x_acceleration);
//   long total_y_displacement = (long)((0.4f*time_till_ball_collides_with_x_boundary) + (0.5*fabsf(m_ball_y_acceleration)* (time_till_ball_collides_with_x_boundary*time_till_ball_collides_with_x_boundary) ) + ((m_bBallServed) ? m_y_initialdisplacement : m_Dot.m_point.y));
//   long y_displacement_at_boundary = 0;
//   if( ((int)(total_y_displacement/m_ClientRect.bottom))%2 )
//      y_displacement_at_boundary = m_ClientRect.bottom - (total_y_displacement%m_ClientRect.bottom);
//   else
//      y_displacement_at_boundary = total_y_displacement%m_ClientRect.bottom;
//   return y_displacement_at_boundary;

//   long   ball_x_displacement_to_boundary = ( m_ball_x_velocity < 0 ) ? m_Dot.m_point.x - m_Line1.m_point.x : m_Line2.m_point.x - m_Dot.m_point.x;
//   double m_ball_x_velocity_at_collision_with_x_boundary = sqrt( ( m_ball_x_velocity*m_ball_x_velocity ) + ( 2 * m_ball_x_acceleration * (ball_x_displacement_to_boundary) ) ) * (double)((m_ball_x_velocity < 0) ? -1.0f: 1.0f) ;
//   double time_till_ball_collides_with_x_boundary = fabsf(( m_ball_x_velocity_at_collision_with_x_boundary - m_ball_x_velocity )/m_ball_x_acceleration);
//   long   y_displacement = (long)((m_ball_y_velocity*time_till_ball_collides_with_x_boundary) + (0.5*m_ball_y_acceleration*(time_till_ball_collides_with_x_boundary*time_till_ball_collides_with_x_boundary) ) + m_Dot.m_point.y );
//   return (y_displacement);

//   long   ball_x_displacement_to_boundary = ( m_ball_x_velocity < 0 ) ? m_Dot.m_point.x - m_Line1.m_point.x : m_Line2.m_point.x - m_Dot.m_point.x;
//   double m_ball_x_velocity_at_collision_with_x_boundary = sqrt( ( m_ball_x_velocity*m_ball_x_velocity ) + ( 2 * m_ball_x_acceleration * (ball_x_displacement_to_boundary) ) ) * (double)((m_ball_x_velocity < 0) ? -1.0f: 1.0f) ;
//   double time_till_ball_collides_with_x_boundary = fabsf(( m_ball_x_velocity_at_collision_with_x_boundary - m_ball_x_velocity )/m_ball_x_acceleration);
//   long   total_y_displacement = (long)((m_ball_y_velocity*time_till_ball_collides_with_x_boundary) + (0.5*m_ball_y_acceleration*(time_till_ball_collides_with_x_boundary*time_till_ball_collides_with_x_boundary) ) + m_Dot.m_point.y ); //((m_bBallServed) ? m_y_initialdisplacement : m_Dot.m_point.y)
//   long   y_displacement_at_boundary = 0;
//   if( !((int)(total_y_displacement/m_ClientRect.bottom))%2 )
//      y_displacement_at_boundary = m_ClientRect.top + (total_y_displacement%m_ClientRect.bottom);
//   else
//      y_displacement_at_boundary = m_ClientRect.bottom - (total_y_displacement%m_ClientRect.bottom);

   long   ball_x_displacement_to_boundary = ( m_ball_x_velocity < 0 ) ? m_Dot.m_point.x - m_Line1.m_point.x : m_Line2.m_point.x - m_Dot.m_point.x;
   double m_ball_x_velocity_at_collision_with_x_boundary = sqrt( ( m_ball_x_velocity*m_ball_x_velocity ) + ( 2 * m_ball_x_acceleration * (ball_x_displacement_to_boundary) ) ) * (double)((m_ball_x_velocity < 0) ? -1.0f: 1.0f) ;
   double time_till_ball_collides_with_x_boundary = fabsf(( m_ball_x_velocity_at_collision_with_x_boundary - m_ball_x_velocity )/m_ball_x_acceleration);
   long   total_y_displacement = abs((long)((m_ball_y_velocity*time_till_ball_collides_with_x_boundary) + (0.5*m_ball_y_acceleration*(time_till_ball_collides_with_x_boundary*time_till_ball_collides_with_x_boundary) ) + m_Dot.m_point.y ));
   long   y_displacement_at_boundary = 0;
   if( !(((int)(total_y_displacement/m_ClientRect.bottom))%2) ) // if the total y displacement divided by the height of the client area is odd
      y_displacement_at_boundary = m_ClientRect.top + (total_y_displacement%m_ClientRect.bottom);
   else
      y_displacement_at_boundary = m_ClientRect.bottom - (total_y_displacement%m_ClientRect.bottom);

  return y_displacement_at_boundary;
}

int MainWindow::paddle1_y( DWORD time )
{
   int s = (int)(m_paddle1_y_velocity * time);
   return s;
}

int MainWindow::paddle2_y( DWORD time )
{
   int s = (int)(m_paddle2_y_velocity * time);
   return s;
}

void MainWindow::PlayEffect( short effect_number )
{
   if( m_bSoundOn )
   {
      //m_Fmod.PlayClip( effect_number );
   }
}


void MainWindow::SetTimeSinceRoundStarted( void )
{
   static DWORD prevtime = 0;
   if( m_bGameInProgress && !m_bGamePaused )
   {
      m_TimeSinceRoundStarted = ::GetTickCount64() - prevtime;
   }
   else
   {
      prevtime = ::GetTickCount64();
      m_TimeSinceRoundStarted = 0;
   }
}

void MainWindow::SetTimeSincePaddleMoved( void )
{
   static DWORD prevtime = 0;
   if( !m_bGamePaused && (m_Keys[VK_DOWN] || m_Keys[VK_UP]) )
   {
      m_TimeSincePaddleMoved = ::GetTickCount64() - prevtime;
   }
   else
   {
      prevtime = ::GetTickCount64();
      m_TimeSincePaddleMoved = 0;
   }
}

void MainWindow::StartNewRound( void )
{
   m_bGameInProgress = false;
   m_bGamePaused     = false;
   m_bBallServed     = false;
   m_RoundNumber++;
   SetRandomError();
   ResetBallVelocityandAcceleration();
   SetInitialObjectPositions();
   SetTimeSinceRoundStarted();
   SetScore();
   SetRound();
   if( !((m_P1Score+m_P2Score)%5) && (m_P1Score+m_P2Score) != 0 )
      ToggleTurns();
}

void MainWindow::StartNewGame( void )
{
   ResetGameState();
   ToggleTurns();
}

void MainWindow::ResetBallVelocityandAcceleration( void )
{
   if( m_bP1Turn )
   {
      m_ball_y_velocity     = initialball_y_speed;
      m_ball_x_velocity     = initialball_x_speed;
      m_ball_y_acceleration = initialballyacceleration;
      m_ball_x_acceleration = initialballxacceleration;
   }
   else
   {
      m_ball_y_velocity     = initialball_y_speed*-1;
      m_ball_x_velocity     = initialball_x_speed*-1;
      m_ball_y_acceleration = initialballyacceleration*-1;
      m_ball_x_acceleration = initialballxacceleration*-1;
   }
}

void MainWindow::ResetGameState( void )
{
   StartNewRound();
   m_RoundNumber     = 1;
   m_bP1Turn         = true;
   m_bP1Computer     = false;
   m_bP2Computer     = false;
   if( m_bP1Turn )
   {
      m_bP1Computer = false;
      m_bP2Computer = true;
   }
   else 
   {
      m_bP2Computer = false;
      m_bP1Computer = true;
   }
   ResetBallVelocityandAcceleration();
   m_bGameOver       = false;
   m_paddle1_y_velocity  = initialpaddlevelocity;
   m_P1Score         = 0; 
   m_P2Score         = 0;
   PlayEffect( 1 );
}

void MainWindow::DoGameOverKeyActions( WPARAM wparam, LPARAM lparam )
{
   if( wparam == VK_RETURN )
   {
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
   }
   if( wparam == VK_ESCAPE )
   {
      m_bQuit = true;
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
   }
}

void MainWindow::DoTitleKeyActions( WPARAM wparam, LPARAM lparam )
{
   if( wparam == VK_RETURN )
   {
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
   }
   if( wparam == VK_ESCAPE )
   {
      m_bQuit = true;
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
   }
}

void MainWindow::DoDemoKeyActions( WPARAM wparam, LPARAM lparam )
{
   if( wparam == VK_RIGHT )
   {
      if( m_bP1Turn && !m_bBallServed )
      {
         m_bGameInProgress = true;
         m_bBallServed     = true;
         SaveInitialBallDisplacement();
      }
   }
   if( wparam == VK_LEFT )
   {
      if( !m_bP1Turn && !m_bBallServed )
      {
         m_bGameInProgress = true;
         m_bBallServed     = true;
         SaveInitialBallDisplacement();
      }  
   }
   if( wparam == VK_RETURN )
   {
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
   }
   if( wparam == VK_ESCAPE )
   {
      m_bQuit = true;
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
   }
}

void MainWindow::DoGameKeyActions( WPARAM wparam, LPARAM lparam )
{
   if( !m_bGamePaused )
   {
      if( wparam == VK_RIGHT )
      {
         if( m_bP1Turn && !m_bBallServed )
         {
            m_bGameInProgress = true;
            m_bBallServed     = true;
            SaveInitialBallDisplacement();
            m_deltaTimeBallServed.Begin();
         }
      }
      if( wparam == VK_LEFT )
      {
         if( !m_bP1Turn && !m_bBallServed )
         {
            m_bGameInProgress = true;
            m_bBallServed     = true;
            SaveInitialBallDisplacement();
            m_deltaTimeBallServed.Begin();
         }  
      }
   }
   if( wparam == VK_RETURN )
   {
      TogglePause();
      if( m_bGamePaused )
      {
         m_deltaTimeBallServed.SaveTime();
         m_deltaTimeBallServed.ResetTime();
      }
      else
         m_deltaTimeBallServed.Begin( m_deltaTimeBallServed.GetSavedTime() );
   }
   if( wparam == VK_ESCAPE )
   {
      m_bQuit = true;
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
   }
}

void MainWindow::FollowPaddleForServe( void )
{
   if( m_bP1Turn )
   {
      m_Dot.m_point.x = m_Line1.m_point.x+m_PenWidth;
      m_Dot.m_point.y = (long)(m_Line1.m_point.y+(m_Line1.m_LineLength/2));
      if( (m_paddle1_y_velocity < 0) && (m_ball_y_velocity > 0) ) m_ball_y_velocity *= -1.0f;
   }
   else
   {
      m_Dot.m_point.x = m_Line2.m_point.x-m_PenWidth;
      m_Dot.m_point.y = (long)(m_Line2.m_point.y+(m_Line2.m_LineLength/2));
      if( (m_paddle2_y_velocity < 0) && (m_ball_y_velocity > 0) ) m_ball_y_velocity *= -1.0f;
   }
   m_initial_ball_x_velocity = m_ball_x_velocity;
}

void MainWindow::ReverseBall_y_Direction( void )
{
   m_ball_y_velocity     *= -1;
   m_ball_y_acceleration *= -1;
}

void MainWindow::ReverseBall_x_Direction( void )
{
   m_ball_x_velocity     *= -1;
   m_ball_x_acceleration *= -1;
}

bool MainWindow::ReverseBallDirectionOn_x_Collision( void )
{
   ReverseBall_x_Direction();
   return true;
}

bool MainWindow::ReverseBallDirectionOn_y_Collision( void )
{
   ReverseBall_y_Direction();
   return true;
}

void MainWindow::MoveBall( void )
{

   if( (m_Dot.m_point.x == m_Line2.m_point.x-1) || (m_Dot.m_point.x == m_Line1.m_point.x+1) )
   m_ball_y_acceleration = 0;

   double y_displacementsincelastcall = (m_ball_y_velocity*m_FrameDeltaTime) + (0.5*m_ball_y_acceleration*m_FrameDeltaTimeSquared); // u*t+0.5*a*t^2
   double x_displacementsincelastcall = (m_ball_x_velocity*m_FrameDeltaTime) + (0.5*m_ball_x_acceleration*m_FrameDeltaTimeSquared); // u*t+0.5*a*t^2
   m_ball_y_velocity       = (m_ball_y_velocity + (m_ball_y_acceleration*m_FrameDeltaTime)); //v = u+at
   m_ball_x_velocity       = (m_ball_x_velocity + (m_ball_x_acceleration*m_FrameDeltaTime)); //v = u+at
   m_Dot.m_point.x        += (long)x_displacementsincelastcall;
   m_Dot.m_point.y        += (long)y_displacementsincelastcall;
}

void MainWindow::MovePaddle1( void )
{
   if( m_Keys[VK_UP] )
   {
      if( m_paddle1_y_velocity > 0 ) m_paddle1_y_velocity *= -1;
      m_Line1.m_point.y += paddle1_y( m_FrameDeltaTime );
   }
   if( m_Keys[VK_DOWN] )
   {
      if( m_paddle1_y_velocity < 0 ) m_paddle1_y_velocity *= -1;
      m_Line1.m_point.y += paddle1_y( m_FrameDeltaTime );
   }
}


/*
void MainWindow::MovePaddle1( void )
{
   static unsigned long lastrepeatcount  = m_Keys.GetRepeatCount( VK_UP   );
   static unsigned long lastrepeatcount2 = m_Keys.GetRepeatCount( VK_DOWN );

   if( m_Keys[VK_UP] )
   {
      if( !(m_Line1.m_point.y <= m_ClientRect.top) )
      {
         if( m_paddle1_y_velocity > 0 ) m_paddle1_y_velocity *= -1;
          m_Line1.m_point.y += paddle1_y( ((m_Keys.GetRepeatCount( VK_UP )-lastrepeatcount)*m_Keys.GetRepeatCountInterval( VK_UP )) );
      }
   }

   if( m_Keys[VK_DOWN] )
   {
      if( !(m_Line1.m_point.y >= (m_ClientRect.bottom-m_Line1.m_LineLength)) )
      {
         if( m_paddle1_y_velocity < 0 ) m_paddle1_y_velocity *= -1;
         m_Line1.m_point.y += paddle1_y( ((m_Keys.GetRepeatCount( VK_DOWN )-lastrepeatcount2)*m_Keys.GetRepeatCountInterval( VK_DOWN )) );
      }
   }
   lastrepeatcount  = m_Keys.GetRepeatCount( VK_UP );
   lastrepeatcount2 = m_Keys.GetRepeatCount( VK_DOWN );
}
*/

void MainWindow::MovePaddle2( void )
{
   if( m_Keys[VK_UP] )
   {
      if( m_paddle2_y_velocity > 0 ) m_paddle2_y_velocity *= -1;
      m_Line2.m_point.y += paddle2_y( m_FrameDeltaTime );
   }
   if( m_Keys[VK_DOWN] )
   {
      if( m_paddle2_y_velocity < 0 ) m_paddle2_y_velocity *= -1;
      m_Line2.m_point.y += paddle2_y( m_FrameDeltaTime );
   }
}

/*
void MainWindow::MovePaddle2( void )
{
   static unsigned long lastrepeatcount  = m_Keys.GetRepeatCount( VK_UP   );
   static unsigned long lastrepeatcount2 = m_Keys.GetRepeatCount( VK_DOWN );

   if( m_Keys[VK_UP] )
   {
      if( !(m_Line2.m_point.y <= m_ClientRect.top) )
     {
         if( m_paddle2_y_velocity > 0 ) m_paddle2_y_velocity *= -1;
          m_Line2.m_point.y += paddle2_y( ((m_Keys.GetRepeatCount( VK_UP )-lastrepeatcount)*m_Keys.GetRepeatCountInterval( VK_UP )) );
      }
   }

   if( m_Keys[VK_DOWN] )
   {
      if( !(m_Line2.m_point.y >= (m_ClientRect.bottom-m_Line2.m_LineLength)) )
      {
         if( m_paddle2_y_velocity < 0 ) m_paddle2_y_velocity *= -1;
         m_Line2.m_point.y += paddle2_y( ((m_Keys.GetRepeatCount( VK_DOWN )-lastrepeatcount2)*m_Keys.GetRepeatCountInterval( VK_DOWN )) );
      }
   }
   lastrepeatcount  = m_Keys.GetRepeatCount( VK_UP );
   lastrepeatcount2 = m_Keys.GetRepeatCount( VK_DOWN );
}
*/

void MainWindow::ApplyForceToBallFromPaddle1( void )
{
//   m_ball_y_velocity -= 0.5*m_paddle1_y_velocity;

}

void MainWindow::ApplyForceToBallFromPaddle2( void )
{
//   m_ball_y_velocity -= 0.5*m_paddle2_y_velocity;
}


bool MainWindow::HandleCollisions( void )
{
   if( (m_Dot.m_point.y <= m_ClientRect.top) )
   {
      ReverseBallDirectionOn_y_Collision();
      m_Dot.m_point.y = m_ClientRect.top+1;
      PlayEffect( 0 );
   }
   else if( (m_Dot.m_point.y >= m_ClientRect.bottom) )
   {
      ReverseBallDirectionOn_y_Collision();
      m_Dot.m_point.y = m_ClientRect.bottom-1;
      PlayEffect( 0 );
   }

   if( (m_Line1.m_point.x >= m_Dot.m_point.x) )
   {
      if( CheckColideWithPaddle1() )
      {
         ApplyForceToBallFromPaddle1();
         ReverseBallDirectionOn_x_Collision();
         m_Dot.m_point.x = m_Line1.m_point.x+1;
         SetRandomError();
         PlayEffect( 0 );
         return true;
      }
      PlayEffect( 2 );
      m_P2Score++;
      StartNewRound();
   }
   else if( (m_Line2.m_point.x <= m_Dot.m_point.x) )
   {
      if( CheckColideWithPaddle2() )
      {
         ApplyForceToBallFromPaddle2();
         ReverseBallDirectionOn_x_Collision();
         m_Dot.m_point.x = m_Line2.m_point.x-1;
         SetRandomError();
         PlayEffect( 0 );
         return true;
      }
      PlayEffect( 2 );
      m_P1Score++;
      StartNewRound();
   }
   return false;
}

void MainWindow::UpdateDemoObjects( void )
{
   if( !m_bGamePaused )
   {
      if( m_bBallServed )
      {
         MoveBall();
      }
      else
      {
         FollowPaddleForServe();
         AIServeBall();
         m_deltaTimeBallServed.ResetTime();
      }
      if( m_bP1Computer )
         InterceptBallWithPaddle1();
      else
         MovePaddle1();
      if( m_bP2Computer )
         InterceptBallWithPaddle2();
      else
         MovePaddle2();
   }
   RestrictPaddleToGameBoundaries( m_Line1 );
   RestrictPaddleToGameBoundaries( m_Line2 );
}

void MainWindow::UpdateObjects( void )
{
   if( !m_bGamePaused )
   {
      if( m_bBallServed )
      {
         MoveBall();
      }
      else
      {
         FollowPaddleForServe();
         AIServeBall();
         m_deltaTimeBallServed.ResetTime();
      }
      if( m_bP1Computer )
         InterceptBallWithPaddle1();
      else
         MovePaddle1();
      if( m_bP2Computer )
         InterceptBallWithPaddle2();
      else
         MovePaddle2();
   }
   RestrictPaddleToGameBoundaries( m_Line1 );
   RestrictPaddleToGameBoundaries( m_Line2 );
}

void MainWindow::LoadMenu( void )
{
//   HRSRC r = ::FindResource( m_Hinstance, MAKEINTRESOURCE(IDS_STRING1), RT_STRING );
//   if(!r)
//   {
//     LogError( GetLastSystemError() );
//   }

   DWORD stringsize = 0;
//   stringsize = ::SizeofResource( m_Hinstance, r );
   stringsize = 2000;
   char *str = new char[stringsize];
   ::LoadString( m_Hinstance, IDS_STRING1, str, stringsize );
   std::string menu = str;
   delete [] str;
   m_XMLMenu.LoadMenuFromString( menu, 1000 );
//   m_XMLMenu.LoadMenu( "menu.xml", 1000 );
}

void MainWindow::DrawAdditionalServeVelocity( void )
{
   if( !m_bBallServed )
   {
      static std::ostringstream s1;
      s1 << m_AdditionalServeVelocity;
      if( !m_bP1Turn )
      {
         TextOut( (m_ClientRect.right)-(int)(m_MemoryDC.GetTextDimensions( s1.str() ).cx*1.5f), 0, s1.str() );
      }
      else
      {
         TextOut( (int)(m_MemoryDC.GetTextDimensions( s1.str() ).cx*1.5f), 0, s1.str() );
      }
      s1.str(std::string());
   }
}

void MainWindow::DrawScore( void )
{
   TextOut( m_P1ScorePos.x, m_P1ScorePos.y, m_P1ScoreString.str() );
   TextOut( m_P2ScorePos.x, m_P2ScorePos.y, m_P2ScoreString.str() );
}

void MainWindow::DrawGameOver( void )
{
   static std::string gameover = "Game Over";
   TextOut( m_halfClientRectright-(int)(m_MemoryDC.GetTextDimensions( gameover ).cx/2), m_halfClientRectbottom, gameover );

   static std::string WhoWonTheGame;
   if( m_P1Score > m_P2Score )
   {
      WhoWonTheGame = "Player 1 Wins";
      TextOut( m_halfClientRectright-(int)(m_MemoryDC.GetTextDimensions( WhoWonTheGame ).cx/2), m_halfClientRectbottom+(m_MemoryDC.GetTextDimensions( WhoWonTheGame ).cy*2), WhoWonTheGame );
   }
   else
   {
      WhoWonTheGame = "Player 2 Wins";
      TextOut( m_halfClientRectright-(int)(m_MemoryDC.GetTextDimensions( WhoWonTheGame ).cx/2), m_halfClientRectbottom+(int)(m_MemoryDC.GetTextDimensions( WhoWonTheGame ).cy*4), WhoWonTheGame );
   }
}

void MainWindow::DrawRound( void )
{
   TextOut( m_RoundPosition.x, m_RoundPosition.y, m_RoundString.str() );
}

void MainWindow::DrawGamePaused( void )
{
   static std::string paused( "Game Paused" );
   static Structs::Point p( (int)(m_MemoryDC.GetTextDimensions( paused ).cx*0.5), (int)(m_MemoryDC.GetTextDimensions( paused ).cy*2) );
   TextOut( m_halfClientRectright-p.x, m_halfClientRectbottom-p.y, paused );
}

void MainWindow::DrawGameObjects( void )
{
   m_Line1.Draw();
   m_Line2.Draw();
   m_Dot.Draw();
}

void MainWindow::DrawServeDirections( void )
{
   if( !m_bGameInProgress )
   {
      static std::string serveright   = "Press Right to Serve";
      static std::string serveleft    = "Press Left to Serve";
      static int         serveleft_x  = (int)(m_MemoryDC.GetTextDimensions( serveleft.c_str()  ).cx*0.5);
      static int         serveright_x = (int)(m_MemoryDC.GetTextDimensions( serveright.c_str() ).cx*0.5);
      if( m_bP1Turn )
         TextOut( m_halfClientRectright-serveright_x, m_halfClientRectbottom, serveright );
      else
         TextOut(  m_halfClientRectright-serveleft_x, m_halfClientRectbottom,  serveleft );         
   }
}

void MainWindow::DrawMainGameLoop( void )
{
   try
   {
      DrawGameObjects();
      DrawScore();
      DrawRound();
      DrawServeDirections();
      if( m_bGamePaused )
         DrawGamePaused();
//      DrawAdditionalServeVelocity();

/*::TextOut(
    m_WinBaseDC,
    400,	// x-coordinate of starting position  
    100,	// y-coordinate of starting position  
    "100010104140",	// address of string 
    12
   );	
*/
      if( !::BitBlt( m_WinBaseDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right-m_ClientRect.left, m_ClientRect.bottom-m_ClientRect.top, m_MemoryDC, m_ClientRect.left, m_ClientRect.top, SRCCOPY ) )
         throw;
   }
   catch(...)
   {
      LogError( GetLastSystemError() );
   }
}

void MainWindow::DrawDemoScreen( void )
{
   try
   {
      DrawGameObjects();
      DrawScore();
      DrawRound();
      static std::string demomode( "Demo Mode" );
      static std::string pressenter( "Press Enter/Return To Continue" );
      static int messageheight = m_MemoryDC.GetTextDimensions( pressenter ).cy;
      static int halfpressenter_x = (int)(m_MemoryDC.GetTextDimensions( pressenter ).cx*0.5);
      static int halfdemomode_x   = (int)(m_MemoryDC.GetTextDimensions( demomode   ).cx*0.5);
      TextOut(   m_halfClientRectright-halfdemomode_x, m_halfClientRectbottom-(messageheight*4),   demomode );
      TextOut( m_halfClientRectright-halfpressenter_x, m_halfClientRectbottom-(messageheight*2), pressenter );

      if( !::BitBlt( m_WinBaseDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right-m_ClientRect.left, m_ClientRect.bottom-m_ClientRect.top, m_MemoryDC, m_ClientRect.left, m_ClientRect.top, SRCCOPY ) )
         throw;
   }
   catch(...)
   {
      LogError( GetLastSystemError() );
   }
}

void MainWindow::DrawTitleScreen( void )
{
   try
   {
      static std::string Welcome( "Welcome To Ver 0.1.8.6 of my pong clone - milestone First good AI" );
      static std::string pressenter( "Press Enter/Return To continue" );
      static int halfpressenter_x = (int)(m_MemoryDC.GetTextDimensions( pressenter ).cx/2);
      static int halfWelcome_x    = (int)(m_MemoryDC.GetTextDimensions(    Welcome ).cx/2);
      static int messageheight = m_MemoryDC.GetTextDimensions( Welcome ).cy;
      TextOut(    m_halfClientRectright-halfWelcome_x, m_halfClientRectbottom-messageheight, Welcome );
      TextOut( m_halfClientRectright-halfpressenter_x, m_halfClientRectbottom+messageheight, pressenter );
      if( !::BitBlt( m_WinBaseDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right-m_ClientRect.left, m_ClientRect.bottom-m_ClientRect.top, m_MemoryDC, m_ClientRect.left, m_ClientRect.top, SRCCOPY ) )
         throw;
   }
   catch(...)
   {
      LogError( GetLastSystemError() );
   }
}

void MainWindow::DrawGameOverScreen( void )
{
   try
   {
      DrawGameOver();
      if( !::BitBlt( m_WinBaseDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right-m_ClientRect.left, m_ClientRect.bottom-m_ClientRect.top, m_MemoryDC, m_ClientRect.left, m_ClientRect.top, SRCCOPY ) )
         throw;
   }
   catch(...)
   {
      LogError( GetLastSystemError() );
   }
}

int MainWindow::TitleScreenLoop( void )
{
   MSG msg;
   bool isMessagePumpActive = true;

   AddMessageHandler( WM_CLOSE,   reinterpret_cast<MessageHandler>(MWBaseOnTitleClose)   );
   AddMessageHandler( WM_SIZE,    reinterpret_cast<MessageHandler>(MWBaseOnTitleSize)    );
   AddMessageHandler( WM_COMMAND, reinterpret_cast<MessageHandler>(MWBaseOnTitleCommand) );
   AddMessageHandler( WM_KEYDOWN, reinterpret_cast<MessageHandler>(MWBaseOnTitleKeyDown) );

   while( isMessagePumpActive ) // While The Message Pump Is Active
   {
      if( ::PeekMessage( &msg, m_WinBase, 0, 0, PM_REMOVE ) )
      {
         // Check For WM_QUIT Message
         if( (msg.message != WM_QUIT) && !m_bQuit ) // Is The Message A WM_QUIT Message?
         {
            ::TranslateMessage( &msg );
            ::DispatchMessage(  &msg );
         }
         else // Otherwise (If Message Is WM_QUIT)
         {
            isMessagePumpActive = false; // Terminate The Message Pump
         }
      }
      else // If There Are No Messages
      {
         if( !m_bisVisible ) // If Window Is Not Visible
         {
            ::WaitMessage(); // Application Is Minimized Wait For A Message
         }
         else // If Window Is Visible
         {
            m_MemoryDC.Clear( m_ClientRect );
            DrawTitleScreen();
         }
      }
   }
   AddMessageHandler( WM_CLOSE,   reinterpret_cast<MessageHandler>(MWBaseOnClose)   );
   AddMessageHandler( WM_SIZE,    reinterpret_cast<MessageHandler>(MWBaseOnSize)    );
   AddMessageHandler( WM_COMMAND, reinterpret_cast<MessageHandler>(MWBaseOnCommand) );
   AddMessageHandler( WM_KEYDOWN, reinterpret_cast<MessageHandler>(MWBaseOnKeyDown) );
   return msg.wParam;
}

int MainWindow::DemoLoop( void )
{
   MSG msg;
   bool isMessagePumpActive = true;
   TimeInterval Interval;

   m_bP1Computer = true;
   m_bP2Computer = true;

   AddMessageHandler( WM_CLOSE,   reinterpret_cast<MessageHandler>(MWBaseOnDemoClose)   );
   AddMessageHandler( WM_SIZE,    reinterpret_cast<MessageHandler>(MWBaseOnDemoSize)    );
   AddMessageHandler( WM_COMMAND, reinterpret_cast<MessageHandler>(MWBaseOnDemoCommand) );
   AddMessageHandler( WM_KEYDOWN, reinterpret_cast<MessageHandler>(MWBaseOnDemoKeyDown) );

   while( isMessagePumpActive ) // While The Message Pump Is Active
   {
      if( ::PeekMessage( &msg, m_WinBase, 0, 0, PM_REMOVE ) )
      {
         // Check For WM_QUIT Message
         if( (msg.message != WM_QUIT) && !m_bQuit ) // Is The Message A WM_QUIT Message?
         {
            ::TranslateMessage( &msg );
            ::DispatchMessage(  &msg );
         }
         else // Otherwise (If Message Is WM_QUIT)
         {
            isMessagePumpActive = false; // Terminate The Message Pump
         }
      }
      else // If There Are No Messages
      {
         if( !m_bisVisible ) // If Window Is Not Visible
         {
            ::WaitMessage(); // Application Is Minimized Wait For A Message
         }
         else // If Window Is Visible
         {
            m_MemoryDC.Clear( m_ClientRect );
            if( !m_bGamePaused )
            {
               m_FrameDeltaTime        = Interval.TimeSinceLastCall();
               m_FrameDeltaTimeSquared = m_FrameDeltaTime*m_FrameDeltaTime;
            }
            else
            {
               m_FrameDeltaTime        = 0;
               m_FrameDeltaTimeSquared = 0;
            }
            Interval.Begin();
            if( m_bGameInProgress && !m_bGamePaused && m_bBallServed )
               HandleCollisions();
            UpdateDemoObjects();
//            if( !m_bBallServed )
//            {
//               AIServeBall();
//            }
//            SetTimeSinceRoundStarted();
//            SetTimeSincePaddleMoved();
            DrawDemoScreen();
            if( CheckForEndGameCondition() )
            {
               GameOverLoop( 10000 );
               AddMessageHandler( WM_CLOSE,   reinterpret_cast<MessageHandler>(MWBaseOnDemoClose)   );
               AddMessageHandler( WM_SIZE,    reinterpret_cast<MessageHandler>(MWBaseOnDemoSize)    );
               AddMessageHandler( WM_COMMAND, reinterpret_cast<MessageHandler>(MWBaseOnDemoCommand) );
               AddMessageHandler( WM_KEYDOWN, reinterpret_cast<MessageHandler>(MWBaseOnDemoKeyDown) );
               ResetGameState();
            }
         }
      }
   }
   AddMessageHandler( WM_CLOSE,   reinterpret_cast<MessageHandler>(MWBaseOnClose)   );
   AddMessageHandler( WM_SIZE,    reinterpret_cast<MessageHandler>(MWBaseOnSize)    );
   AddMessageHandler( WM_COMMAND, reinterpret_cast<MessageHandler>(MWBaseOnCommand) );
   AddMessageHandler( WM_KEYDOWN, reinterpret_cast<MessageHandler>(MWBaseOnKeyDown) );
   m_Keys.Reset();
   return msg.wParam;
}

int MainWindow::RunGameLoop( void )
{
   MSG msg;
   bool isMessagePumpActive = true;
   ResetGameState();
   TimeInterval Interval;
         
   while( isMessagePumpActive ) // While The Message Pump Is Active
   {
      if( ::PeekMessage( &msg, m_WinBase, 0, 0, PM_REMOVE ) )
      {
         // Check For WM_QUIT Message
         if( (msg.message != WM_QUIT) && !m_bQuit ) // Is The Message A WM_QUIT Message?
         {
            ::TranslateMessage( &msg );
            ::DispatchMessage(  &msg );
         }
         else // Otherwise (If Message Is WM_QUIT)
         {
            isMessagePumpActive = false; // Terminate The Message Pump
         }
      }
      else // If There Are No Messages
      {
         if( !m_bisVisible ) // If Window Is Not Visible
         {
            ::WaitMessage(); // Application Is Minimized Wait For A Message
         }
         else // If Window Is Visible
         {
            m_MemoryDC.Clear( m_ClientRect );
            if( !m_bGamePaused )
            {
               m_FrameDeltaTime        = Interval.TimeSinceLastCall();
               m_FrameDeltaTimeSquared = m_FrameDeltaTime*m_FrameDeltaTime;
            }
            else
            {
               m_FrameDeltaTime        = 0;
               m_FrameDeltaTimeSquared = 0;
            }
            Interval.Begin();
            // Process Application Loop
            if( CheckForEndGameCondition() )
            {
               GameOverLoop(10000);
               ResetGameState();
               DemoLoop();
               ResetGameState();
            }
            else
            {
               UpdateObjects();
               if( m_bGameInProgress && !m_bGamePaused && m_bBallServed )
                  HandleCollisions();
//               SetTimeSinceRoundStarted();
//               SetTimeSincePaddleMoved();
            }
            DrawMainGameLoop();
            m_FrameRate.ShowFPSinTitle( m_WinBase );
         }
      }
   }
   return msg.wParam;
}

int MainWindow::GameOverLoop( DWORD timeoutvalue )
{
   m_bGameOver = true;
   TimeInterval Interval;
   Interval.Begin();

   MSG msg;
   bool isMessagePumpActive = true;

   AddMessageHandler( WM_CLOSE,   reinterpret_cast<MessageHandler>(MWBaseOnGameOverClose)   );
   AddMessageHandler( WM_SIZE,    reinterpret_cast<MessageHandler>(MWBaseOnGameOverSize)    );
   AddMessageHandler( WM_COMMAND, reinterpret_cast<MessageHandler>(MWBaseOnGameOverCommand) );
   AddMessageHandler( WM_KEYDOWN, reinterpret_cast<MessageHandler>(MWBaseOnGameOverKeyDown) );
         
   while( isMessagePumpActive ) // While The Message Pump Is Active
   {
      if( ::PeekMessage( &msg, m_WinBase, 0, 0, PM_REMOVE ) )
      {
         // Check For WM_QUIT Message
         if( (msg.message != WM_QUIT) && !m_bQuit ) // Is The Message A WM_QUIT Message?
         {
            ::TranslateMessage( &msg );
            ::DispatchMessage(  &msg );
         }
         else // Otherwise (If Message Is WM_QUIT)
         {
            isMessagePumpActive = false; // Terminate The Message Pump
         }
      }
      else // If There Are No Messages
      {
         if( !m_bisVisible ) // If Window Is Not Visible
         {
            ::WaitMessage(); // Application Is Minimized Wait For A Message
         }
         else // If Window Is Visible
         {
            m_Fmod.Update();
            m_MemoryDC.Clear( m_ClientRect );
            DrawGameOverScreen();
            m_FrameRate.ShowFPSinTitle( m_WinBase );
            if( (Interval.TimeSinceLastCall() >= timeoutvalue) && (timeoutvalue > 0) )
            {
               ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
            }
         }
      }
   }
   AddMessageHandler( WM_CLOSE,   reinterpret_cast<MessageHandler>(MWBaseOnGameOverClose)   );
   AddMessageHandler( WM_SIZE,    reinterpret_cast<MessageHandler>(MWBaseOnSize)            );
   AddMessageHandler( WM_COMMAND, reinterpret_cast<MessageHandler>(MWBaseOnGameOverCommand) );
   AddMessageHandler( WM_KEYDOWN, reinterpret_cast<MessageHandler>(MWBaseOnGameOverKeyDown) );
   return msg.wParam;
}

int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,      int nFunsterStil )
{
   MainWindow App;
   try
   {
      App.m_WinBase.SetStyle( WS_VISIBLE|WS_CAPTION );
      int x_pos = (int)(::GetSystemMetrics( SM_CXSCREEN )*0.1),
          y_pos = (int)(::GetSystemMetrics( SM_CYSCREEN )*0.1);
      int width  = ::GetSystemMetrics( SM_CXSCREEN )-x_pos*2,
          height = ::GetSystemMetrics( SM_CYSCREEN )-y_pos*2;

      if( App.Create( "A pong clone!", NULL, x_pos, y_pos, 550, 450 ) )
      {
//         App.m_WinBase.Show( SW_MAXIMIZE );
         App.TitleScreenLoop();
         App.DemoLoop();
         return App.RunGameLoop();
      }
   }
   catch(...)
   {
   }
   return -1;
}

