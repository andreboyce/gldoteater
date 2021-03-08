#include "../../projects.h"
#include <windows.h>
#include <Wininet.h>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <list>
#include <stack>
#include <queue>
#include <fstream>
#include <gl\gl.h>
#include <gl\glu.h>

#include "Resource.h"
#include "PongNetwork.h"


#include "C_Ball.h"
#include "C_Paddle.h"

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

const static double ballmass                           = 0.1;
const static double initialball_x_velocity             = 0.008; //0.014
const static double initialball_y_velocity             = 0.018; // 0.024f;
const static double initialball_x_acceleration         = 0.000001f;
const static double initialball_y_acceleration         = 0.000001f; //0.000000001f;
const static double initialball_x_angular_position     = 0.1f; 
const static double initialball_y_angular_position     = 0.1f;
const static double initialball_x_angular_velocity     = 0.5f;
const static double initialball_y_angular_velocity     = 0.0f;
const static double initialball_x_angular_acceleration = 0.0;
const static double initialball_y_angular_acceleration = 0.0;
const static double initialpaddle_x_velocity           = 0.0;
const static double initialpaddle_y_velocity           = 0.03;
const static unsigned short consolelinelength          = 70;
const static unsigned short consolelines               = 13;
const static unsigned short maxnicklength              = 20;

class MainWindow: public CWinMain
{
   friend int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                              LPSTR lpszArgument,      int nFunsterStil );
   public:

      //typedef TCHAR               u_char;
      //typedef std::string<u_char> std::string;

// textures
      enum
      {
         TEXTURE_BALL = 0,
         TEXTURE_PADDLE1,
         TEXTURE_PADDLE2,
         TEXTURE_PLAYINGAREA,
         TEXTURE_ARROW,
         TEXTURE_CLOSE,
         TEXTURE_MIN,
         TEXTURE_CHAT,
         TEXTURE_HELP,
      };

//  sounds
      enum
      {
         BOUNCE = 0,
         NEWGAME,
         BOING,
         OPTION,
         CONNECT,         
      };

// difficulty
      enum
      {
        Impossible = 0,
        Hard,
        Normal,
        Easy        
      };

// Timers
      enum
      {
         CHANGE_TO_GAME_MODE = 1,
         CHANGE_TO_DEMO_MODE,
         TURNOFFP2CONSOLE,
         SEND_DATA,
         SEND_PING,
         SET_BYTES_SENT,
         SEND_CLICK_TIMER,
      };

// messages
      enum
      {
         WM_TAUNT = WM_USER+1,
         WM_TOGGLEFULLSCREEN,
         WM_ONCLIENT,
         WM_ONSERVER,
         WM_STARTSERVER,
         WM_STARTCLIENT,
         WM_CONTEXT_LBUTTONDOWN,
         WM_CONTEXT_MOUSEOVER,
      };

// taunts
      enum
      {
         INGAME = 0,
         LOSTROUND,
         WONROUND,
         LOSTGAME,
         WONGAME
      };

// game modes
      enum
      {
         GAMEMODE = 0,
         TITLEMODE,
         DEMOMODE,
         GAMEOVERMODE,
         CONNECTMODE,
         OPTIONSMODE
      };

// options
      enum
      {
         HELP = 0,
         DIFFICULTY,
         GREETZ,
         MULTIPLAYER,
         GAMEKEYS,
         DETAIL,
         VSYNC,
         SOUND,
      };

// display lists
      enum
      {
         GL_LIST_CLEAR = 0,
         GL_LIST_TEXTCOLOR,
         GL_LIST_TEXTURECOLOR,
         GL_LIST_BG,
         GL_LIST_BALL,
         GL_LIST_TRAIL,
         GL_LIST_PADDLE1,
         GL_LIST_PADDLE2,
         GL_LIST_TITLE_STR1,
         GL_LIST_TITLE_STR2,
         GL_LIST_OPTIONS_INSTRUCTIONS,
         GL_LIST_HELP,
         GL_LIST_GREETZ,
         GL_LIST_OPTIONS,
         GL_LIST_COMMANDLIST,
         GL_LIST_DEMO_STR1,
         GL_LIST_DEMO_STR2,
         GL_LIST_DIFFICULTY,
         GL_LIST_MULTIPLAYER,
         GL_LIST_OPTIONS_CURSOR,
         GL_LIST_GAMEKEY_SUBOPTIONS_STR1,
         GL_LIST_GAMEKEY_SUBOPTIONS_STR2,
         GL_LIST_GAMEKEY_SUBOPTIONS_STR3,
         GL_LIST_GAMEKEY_SUBOPTIONS_STR4,
         GL_LIST_CONNECT,
         GL_LIST_DETAIL,
         GL_LIST_P1CONSOLE,
         GL_LIST_P1CONSOLE_CURSOR,
         GL_LIST_P2CONSOLE,
         GL_LIST_P2CONSOLE_CURSOR,
         GL_LIST_OPTIONS_SCORE,
         GL_LIST_OPTIONS_ROUND,
         GL_LIST_GAME_SPEED,
         GL_LIST_GAME_DIFFICULTY,
         GL_LIST_PLAYER_1_NICK,
         GL_LIST_PLAYER_2_NICK,
         GL_LIST_CLIENT_IP,
         GL_LIST_PING,
         GL_LIST_BYTES_SENT,
         GL_LIST_P2CHATTING,
         GL_LIST_BUFFERSIZE,
      };

      MainWindow( LPSTR lpszArgument );

      ~MainWindow( void )
      {
      }

      std::string SetVSync( int interval = 0 )
      {
         typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
         PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
         const unsigned char *extensions = ::glGetString( GL_EXTENSIONS );

         if( ::strstr( (const char *)extensions, "WGL_EXT_swap_control" ) == 0 )
         {
            return std::string( "WGL_EXT_swap_control extension not supported on your computer." );
         }
         else
         {
            wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)::wglGetProcAddress( "wglSwapIntervalEXT" );
            if( wglSwapIntervalEXT )
            {
                wglSwapIntervalEXT( interval );
                return std::string( "VSync change successfull" );
            }
            else
            {
                return std::string( "wglGetProcAddress failed" );
            }
         }
      }

      static LRESULT MWBaseOnRButtonD(         MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLButtonD(         MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLDoubleClick(     MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnRDoubleClick(     MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnRButtonUP(        MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLButtonUP(        MainWindow &mw, WPARAM wparam, LPARAM lparam );

      static LRESULT MWBaseOnKeyDown(          MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnKeyUp(            MainWindow &mw, WPARAM wparam, LPARAM lparam );

      static LRESULT MWBaseOnContextMouseOver( MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnContextLbtn(      MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnMove(             MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnSize(             MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnClose(            MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnChar(             MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnTimer(            MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnTaunt(            MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnCreate(           MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnCommand(          MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnClient(           MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnServer(           MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnStartClient(      MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnStartServer(      MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnToggleFullScreen( MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnMouseMove(        MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnSysCommand(       MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnKillFocus(        MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnSetFocus(         MainWindow &mw, WPARAM wparam, LPARAM lparam );

      static void OptionsButtonClick(     void * argument );
      static void LeftButtonClick(        void * argument );
      static void RightButtonClick(       void * argument );
      static void MinButtonClick(         void * argument );
      static void CloseButtonClick(       void * argument );
      static void FullScreenButtonClick(  void * argument );
      static void ChatButtonClick(        void * argument );
      static void CommandsButtonClick(    void * argument );

      std::queue<std::string> m_string_buffer_queue;

      struct S_BallPosition
      {
         S_BallPosition( void ){}
         S_BallPosition( const S_BallPosition& rhs ) :
            m_BallPositionVector     ( rhs.m_BallPositionVector ),
            m_BallVelocityVector     ( rhs.m_BallVelocityVector )
         {}
         S_BallPosition& operator = ( const S_BallPosition& rhs )
         {
            m_BallPositionVector     = rhs.m_BallPositionVector;
            m_BallVelocityVector     = rhs.m_BallVelocityVector;
            return *this;
         }
         Structs::fPoint m_BallPositionVector,
                           m_BallVelocityVector;
      };
      S_BallPosition m_BallPosition;

      void CopyBallPositionForSending( void )
      {
         if( m_bNetworkActive )
         {
            m_BallPosition.m_BallPositionVector.m_x = -m_Ball.m_PositionVector.m_x;
            m_BallPosition.m_BallPositionVector.m_y = m_Ball.m_PositionVector.m_y;
            m_BallPosition.m_BallVelocityVector.m_x = -m_Ball.m_VelocityVector.m_x;
            m_BallPosition.m_BallVelocityVector.m_y = m_Ball.m_VelocityVector.m_y;
         }
      }
      void CopyBallPositionFromNetwork( void )
      {
         if( m_bNetworkActive )
         {
            m_Ball.m_PositionVector.m_x  = m_BallPosition.m_BallPositionVector.m_x;
            m_Ball.m_PositionVector.m_y  = m_BallPosition.m_BallPositionVector.m_y;
            m_Ball.m_VelocityVector.m_x  = m_BallPosition.m_BallVelocityVector.m_x;
            m_Ball.m_VelocityVector.m_y  = m_BallPosition.m_BallVelocityVector.m_y;
        }
      }

      struct S_PaddlePosition
      {
         S_PaddlePosition( void ) : m_PaddlePositionVector( float() ) {}
         S_PaddlePosition( const S_PaddlePosition& rhs ) :
            m_PaddlePositionVector     ( rhs.m_PaddlePositionVector )
         {}
         S_PaddlePosition& operator = ( const S_PaddlePosition& rhs )
         {
            m_PaddlePositionVector     = rhs.m_PaddlePositionVector;
            return *this;
         }
         float m_PaddlePositionVector;
      };
      S_PaddlePosition m_PaddlePosition;

      void CopyPaddlePositionForSending( void )
      {
         if( m_bNetworkActive )
         {
            m_PaddlePosition.m_PaddlePositionVector     = m_Paddle1.m_PositionVector.m_y;
         }
      }
      void CopyPaddlePositionFromNetwork( void )
      {
         if( m_bNetworkActive )
         {
            m_Paddle2.m_PositionVector.m_y     = m_PaddlePosition.m_PaddlePositionVector;
        }
      }

      struct S_GameStatus
      {
         S_GameStatus( void ) : 
            m_RoundNumber( 0 ), 
            m_P1Score( 0 ), 
            m_P2Score( 0 ),
            m_bBallServed( false ),
            m_bServerServes( true )
         {}
         S_GameStatus( const S_GameStatus& rhs ) : 
            m_RoundNumber  ( rhs.m_RoundNumber ), 
            m_P1Score      ( rhs.m_P1Score ), 
            m_P2Score      ( rhs.m_P2Score ),
            m_bBallServed  ( rhs.m_bBallServed ),
            m_bServerServes( rhs.m_bServerServes )
         {}
         S_GameStatus& operator =( const S_GameStatus& rhs )
         {
            m_RoundNumber   = rhs.m_RoundNumber;
            m_P1Score       = rhs.m_P1Score;
            m_P2Score       = rhs.m_P2Score;
            m_bBallServed   = rhs.m_bBallServed;
            m_bServerServes = rhs.m_bServerServes;
         }

         unsigned char m_RoundNumber, m_P1Score, 
                       m_P2Score, m_bBallServed, 
                       m_bServerServes;
      };
      S_GameStatus m_GameStatus;

      void CopyGameStatusFromNetwork( void )
      {
         m_RoundNumber   = m_GameStatus.m_RoundNumber;
         m_P1Score       = m_GameStatus.m_P1Score;
         m_P2Score       = m_GameStatus.m_P2Score;
         m_bBallServed   = m_GameStatus.m_bBallServed;
         m_bServerServes = m_GameStatus.m_bServerServes;
      }
      void CopyGameStatusForSending( void )
      {
         m_GameStatus.m_RoundNumber   = m_RoundNumber;
         m_GameStatus.m_P1Score       = m_P1Score;
         m_GameStatus.m_P2Score       = m_P2Score;
         m_GameStatus.m_bBallServed   = m_bBallServed;
         m_GameStatus.m_bServerServes = m_bServerServes;
      }

      void StartOnlineGame( void )
      {
         SetGameModeTrue();
         StartNewGame();
         if( m_bisServer )
         {
            m_bP1Turn = true;
         }
         else
         {
            m_bP1Turn = false;
         }
         m_bP1Computer = false;
         m_bP2Computer = false;
         m_bGamePaused = false;
      }

      void ShutDownNetork( void )
      {
//         m_SendStream.flush();
         m_Server.close();
         m_Client.close();
         m_bNetworkActive = false;
      }

      void EndOnlineGame( void )
      {
         SetConnectModeTrue();
         ShutDownNetork();
      }

      void GetDataFromNetWork( void );

      void SendData( void )
      {
         if( m_bNetworkActive )
         {
            if( m_bisServer )
            {
               m_SendStream.send( m_Server.GetSocket( 0 ) );
            }
            else
            {
               m_SendStream.send( m_Client.GetSocket() );
            }
         }
      }

      void CompensateGameSpeedForLag( void );

      bool EraseGameFromMasterServer( void );
      bool SaveGameOnMasterServer( void );
      bool GetGamesFromMasterServer( void );
      bool ParseGamesFromMasterServer( std::string& Games );

      void BuildBufferSizeList();
      void BuildBytesSentList();
      void BuildPingList( void );
      void BuildPlayer2IPList( void );
      void BuildP1NickList( void );
      void BuildP2NickList( void );
      void BuildGameDifficultyList( void );
      void BuildGameSpeedList( void );
      void BuildRoundList( void );
      void BuildScoreList( void );
      void BuildP1ConsoleList( void );
      void BuildP2ConsoleList( void );

      Structs::dPoint BoundingBox( const double z );

      bool EnableOpenGL( void );

      WPARAM RunGameLoop( void );
      void   Initialise( void );

      void   Draw( void );

      void   DrawP1Console( void );
      void   DrawP2Console( void );

      void   InitLists( void );

      void   DrawBytesSent( void );
      void   DrawPing( void );
      void   DrawPlayer2IP( void );
      void   DrawGameDifficulty( void );
      void   DrawGameSpeed( void );
      void   DrawMousePosition( void );
      void   DrawTitle( void );
      void   DrawConnect( void );
      void   DrawGame( void );
      void   DrawDemo( void );
      void   DrawGameOverMode( void );
      void   DrawOptionsCursor( void );
      void   DrawOptions( void );
      void   DrawScore( void );
      void   DrawRound( void );
      void   DrawServeInstructions( void );
      void   DrawGreetz( void );
      void   DrawHelp( void );
      void   DrawDifficulty( void );
      void   DrawMultiPlayerSubOption( void );
      void   DrawGameKeySubOptions( void );
      void   DrawDetailSubOptions( void );
      void   DrawCommandList( void );

      void   ShowFPSinWindow( float x, float y, float z );
      bool   Reshape( long width, long height );
      bool   HandleCollisions( void );
      bool   ChangeScreenResolution( int width, int height, int bitsPerPixel, DWORD dwflags );

      void   FollowPaddleForServe( void );
      void   MoveBall( void );
      void   MovePaddle1WithKeyPad( void );
      void   MovePaddle2WithKeyPad( void );
      void   MovePaddle1WithMouse( void );
      void   MovePaddle2WithMouse( void );

      void   AIServeBall( void );
      void   ApplyForceToBallFromPaddle1( void );
      void   ApplyForceToBallFromPaddle2( void );

      void   InterceptBallWithPaddle1( void );
      void   InterceptBallWithPaddle2( void );
      void   MovePaddle1_y_To( const double );
      void   MovePaddle2_y_To( const double );
      void   UpdatePaddle1Position( void );
      void   UpdatePaddle2Position( void );

      void   ReverseBall_y_Direction( void );
      void   ReverseBall_x_Direction( void );

      void   DoGameKeyActions(         WPARAM wparam, LPARAM lparam );
      void   DoTitleKeyActions(        WPARAM wparam, LPARAM lparam );
      void   DoDemoKeyActions(         WPARAM wparam, LPARAM lparam );
      void   DoGameOverModeKeyActions( WPARAM wparam, LPARAM lparam );
      void   DoOptionsKeyActions(      WPARAM wparam, LPARAM lparam );
      void   DoConnectKeyActions(      WPARAM wparam, LPARAM lparam );

      void   ParseCommandLine( void );
      void   PushGameState( void );
      void   PopGameState( void );
      void   StartNewGame( void );
      void   StartNewRound( void );
      void   ResetGameState( void );
      void   ResetBallAndPaddleVectors( void );
      void   SetRandomError( void );

      void   GetHostIp( std::string ipstring );

      void   StartServer( void );
      void   StartClient( void );

      int    CheckCommand( std::string = "" );
      std::string GetVariable( std::string );

      void   EndLine( void );

      void UpdatePlayer1ToNetwork( void );
      void UpdatePlayer2FromNetwork( void );

      void CopyTextToClipBoard( std::string text );
      std::string GetTextFromClipBoard( bool bAddTextToConsole = true );

      double Ball_y_PositionAt_x_Boundary( void );

      void RestrictPaddleToGameBoundaries( C_Paddle& Paddle );

      bool LoadConfig( void );
      bool SaveConfig( void );

      void InitializeListBoxes( void );
      void InitializeListButtons( void );

      Vector3D WindowToGLCoordinates( int y, int x );
      Vector2D GLToWindowCoordinates( double y, double x, double z );

      void   UpdateObjects( void );

      std::string BoolValue( bool bval );

      bool ValidateIP( std::string ip );

      void EnableLogging( void )
      {
         m_p1Console.EnableLogging();
         m_p2Console.EnableLogging();
      }

      void DisableLogging( void )
      {
         m_p1Console.DisableLogging();
         m_p2Console.DisableLogging();
      }

      void EraseLog( void )
      {
         m_p1Console.EraseLog();
         m_p2Console.EraseLog();
      }

      bool GameKeyPressed( WPARAM wparam )
      {
         if( ( wparam == VK_LEFT ) || ( wparam == VK_RIGHT ) || ( wparam == VK_UP ) || ( wparam == VK_DOWN ))
            return true;
         return false;
      }

      bool GameKeyReleased( WPARAM wparam )
      {
         return GameKeyPressed( wparam );
      }

      inline bool CheckForEndGameCondition( void )
      {
         if( (m_P1Score >= m_MaxScore) || (m_P2Score >= m_MaxScore) )
         {
            return true;
         }
         return false;
      }

      void SetPlayingAreaRect( void )
      {
         Structs::dPoint pt = BoundingBox( m_Z );
         m_PlayingAreaRect.left   = -18;//pt.m_x * -0.5;//-18;
         m_PlayingAreaRect.right  = +18;//pt.m_x * +0.5;//+18;
         m_PlayingAreaRect.top    = +12;//pt.m_y * +0.5;//+12;
         m_PlayingAreaRect.bottom = -12;//pt.m_y * -0.5;//-12;
         m_halfplayingarea        = fabs((m_PlayingAreaRect.top-m_PlayingAreaRect.bottom)*0.5);
      }

      void SetInitialObjectPositions( void )
      {
         SetInitialPaddlePositions();
         if( m_bP1Turn )
         {
            m_Ball.m_PositionVector.m_x = m_Paddle1.m_PositionVector.m_x+(m_Ball.m_Radius+m_Paddle1.m_TexQuad.halfwidth());
            m_Ball.m_PositionVector.m_y = m_Paddle1.m_PositionVector.m_y;
         }
         else
         {
            m_Ball.m_PositionVector.m_x = m_Paddle2.m_PositionVector.m_x-(m_Ball.m_Radius+m_Paddle1.m_TexQuad.halfwidth());
            m_Ball.m_PositionVector.m_y = m_Paddle2.m_PositionVector.m_y;
         }
         m_Ball.m_Z = m_Z;
      }
      void SetInitialPaddlePositions( void )
      {
         m_Paddle1.m_PositionVector.m_x =  m_PlayingAreaRect.left;//-18;//m_PlayingAreaRect.left;//-35.5f;
         m_Paddle1.m_PositionVector.m_y =  0.0f;
         m_Paddle2.m_PositionVector.m_x =  m_PlayingAreaRect.right;//18;//m_PlayingAreaRect.right;//+35.5f;
         m_Paddle2.m_PositionVector.m_y =  0.0f;

         m_Paddle1.m_Z = m_Z;
         m_Paddle2.m_Z = m_Z;
      }

      void SetBufferSizeSStream( void )
      {
         m_SendBufferSizeSStream.str(std::string());
         m_RecvBufferSizeSStream.str(std::string());

         std::size_t rbuf = m_SendStream.Buffer_Size();
         std::size_t sbuf = m_RecvStream.Buffer_Size();

         m_SendBufferSizeSStream << "Send buffer size: " << sbuf << "Bytes";
         m_RecvBufferSizeSStream << "Recv buffer size: " << rbuf << "Bytes";
/*         if( m_SendStream.Bytes_Sent() < 1000 )
         {
            m_BytesSentSStream << m_SendStream.Bytes_Sent() << "bytes" << " Sent";
         }
         else if( m_SendStream.Bytes_Sent() >= 1000 )
         {
            double bytes = ((double)m_SendStream.Bytes_Sent())/1000;
            m_BytesSentSStream << bytes << "Kbytes" << " Sent";
         }
         else if( m_SendStream.Bytes_Sent() >= 1000000 )
         {
            double bytes = ((double)m_SendStream.Bytes_Sent())/1000000;
            m_BytesSentSStream << bytes << "Mbytes" << " Sent";
         } 
*/       BuildBufferSizeList();
      }

      void SetBytesSentSStream( void )
      {
         m_BytesSentSStream.str( std::string() );
         if( m_SendStream.Bytes_Sent() < 1000 )
         {
            m_BytesSentSStream << m_SendStream.Bytes_Sent() << "bytes" << " Sent";
         }
         else if( m_SendStream.Bytes_Sent() >= 1000 )
         {
            double bytes = ((double)m_SendStream.Bytes_Sent())/1000;
            m_BytesSentSStream << bytes << "Kbytes" << " Sent";
         }
         else if( m_SendStream.Bytes_Sent() >= 1000000 )
         {
            double bytes = ((double)m_SendStream.Bytes_Sent())/1000000;
            m_BytesSentSStream << bytes << "Mbytes" << " Sent";
         }
         BuildBytesSentList();
      }

      void SetPingStringStream( void )
      {
         m_PingStringStream.str(std::string());
         m_PingStringStream << "Ping: " << m_Ping << "ms";
         BuildPingList();
      }

      void SetScore( void )
      {
         m_P1ScoreSStream.str(std::string());
         m_P2ScoreSStream.str(std::string());
         m_P1ScoreSStream << "Player 1 Score: ";
         m_P2ScoreSStream << "Player 2 Score: ";
         m_P1ScoreSStream << m_P1Score;
         m_P2ScoreSStream << m_P2Score;
         BuildScoreList();
      }

      void SetGameSpeed( void )
      {
         m_GameSpeedSStream.str(std::string());
         m_GameSpeedSStream << "GameSpeed: ";
         m_GameSpeedSStream << m_GameSpeed;
         BuildGameSpeedList();
      }

      void SetDifficultySStream( void )
      {
         m_DifficultySStream.str( std::string() );
         if( m_DifficultyOptions[ Easy ] )
            m_DifficultySStream << "Difficulty: Easy";
         else if( m_DifficultyOptions[ Normal ] )
            m_DifficultySStream << "Difficulty: Normal";
         else if( m_DifficultyOptions[ Hard ] )
            m_DifficultySStream << "Difficulty: Hard";
         else if( m_DifficultyOptions[ Impossible ] )
            m_DifficultySStream << "Difficulty: Impossible";
         else
         {
            m_DifficultyOptions.SetOnlyTrue( Normal );
            m_DifficultySStream << "Difficulty: Normal";
         }
         BuildGameDifficultyList();
      }

      void SetRound( void )
      {
         m_RoundSStream.str(std::string());
         m_RoundSStream << "Round: ";
         m_RoundSStream << m_RoundNumber;
         BuildRoundList();
      }

      void SetOptionsModeTrue( void )
      {
         m_GameMode[ DEMOMODE ]     = false;
         m_GameMode[ TITLEMODE ]    = false;
         m_GameMode[ GAMEMODE ]     = false;
         m_GameMode[ GAMEOVERMODE ] = false;
         m_GameMode[ OPTIONSMODE ]  = true;
         m_GameMode[ CONNECTMODE ]  = false;
         m_bOptionsModeWasVisited   = true;
         m_ChangeToGameMode.Kill();
         m_ChangeToDemoMode.Kill();
         m_ChangeToConnectMode.Kill();
         m_bP2DrawConsole           = false;
      }
      void SetTitleModeTrue( void )
      {
         m_GameMode[ DEMOMODE ]     = false;
         m_GameMode[ TITLEMODE ]    = true;
         m_GameMode[ GAMEMODE ]     = false;
         m_GameMode[ GAMEOVERMODE ] = false;
         m_GameMode[ OPTIONSMODE ]  = false;
         m_GameMode[ CONNECTMODE ]  = false;

         m_bP1Computer = true;
         m_bP2Computer = true;
      }
      void SetDemoModeTrue( void )
      {
         ResetGameState();
         m_GameMode[ DEMOMODE ]     = true;
         m_GameMode[ TITLEMODE ]    = false;
         m_GameMode[ GAMEMODE ]     = false;
         m_GameMode[ GAMEOVERMODE ] = false;
         m_GameMode[ OPTIONSMODE ]  = false;
         m_GameMode[ CONNECTMODE ]  = false;
         m_p2Console.Clear();
         BuildP2ConsoleList();
         m_bP1Computer    = true;
         m_bP2Computer    = true;
         m_bP2DrawConsole = true;
         m_ChangeToGameMode.Kill();
         m_ChangeToDemoMode.Kill();
         m_ChangeToConnectMode.Kill();
      }
      void SetGameModeTrue( void )
      {
         m_GameMode[ DEMOMODE ]     = false;
         m_GameMode[ TITLEMODE ]    = false;
         m_GameMode[ GAMEMODE ]     = true;
         m_GameMode[ GAMEOVERMODE ] = false;
         m_GameMode[ OPTIONSMODE ]  = false;
         m_GameMode[ CONNECTMODE ]  = false;
         ResetGameState();
         PopGameState();
         m_p2Console.Clear();
         m_ChangeToDemoMode.Kill();
         m_bGamePaused = false;
      }
      void SetGameOverModeTrue( void )
      {
         m_GameMode[ DEMOMODE ]     = false;
         m_GameMode[ TITLEMODE ]    = false;
         m_GameMode[ GAMEMODE ]     = false;
         m_GameMode[ GAMEOVERMODE ] = true;
         m_GameMode[ OPTIONSMODE ]  = false;
         m_GameMode[ CONNECTMODE ]  = false;

         m_bP1Computer = true;
         m_bP2Computer = true;
      }
      void SetConnectModeTrue( void )
      {
         m_GameMode[ DEMOMODE ]     = false;
         m_GameMode[ TITLEMODE ]    = false;
         m_GameMode[ GAMEMODE ]     = false;
         m_GameMode[ GAMEOVERMODE ] = false;
         m_GameMode[ OPTIONSMODE ]  = false;
         m_GameMode[ CONNECTMODE ]  = true;
         m_bP2DrawConsole = false;
      }

      void SetdeltaTimeAIServBall( void )
      {
         double max_time = (m_PlayingAreaRect.top/initialpaddle_y_velocity)*2.0; // maximum time it will take to move to the extents of the playing area *2
         if( max_time == 0 ) max_time = 1;
         m_deltaTimeAIServBall =  max_time - double(rand()%((long)max_time));
      }
      void SetRandomServePosition( void )
      {
         if( m_PlayingAreaRect.top == 0 ) m_PlayingAreaRect.top = 1;
         m_RandomServePosition = ((double)(rand()%((int)m_PlayingAreaRect.top)));
         if( ((bool)(rand()%2)) ) m_RandomServePosition *= -1.0;
      }

      bool MainWindow::PlayEffect( short effect_number )
      {
         if( m_bSoundOn )
         {
            return m_Fmod.PlayClip( effect_number );
         }
         return 0;
      }

      void ToggleDrawP1Console( void )
      {
         ( m_bP1DrawConsole ) ? m_bP1DrawConsole = false : m_bP1DrawConsole = true;
         SetDrawP1Console( m_bP1DrawConsole );
      }
      void SetDrawP1Console( bool value )
      {
         if( m_bNetworkActive )
         {
            if( value )
               m_SendStream.AddPacket( NULL, 0, packet::PACKET_FLAG_CHATTING_ON );
            else
               m_SendStream.AddPacket( NULL, 0, packet::PACKET_FLAG_CHATTING_OFF );
         }
         else
         {
            m_bP1DrawConsole = value;
         }
      }

      void ToggleDrawP2Console( void )
      {
         ( m_bP2DrawConsole ) ? m_bP2DrawConsole = false : m_bP2DrawConsole = true;
      }
      void TogglePause( void )
      {
         ( m_bGamePaused ) ? m_bGamePaused = false : m_bGamePaused = true;
      }
      void ToggleTurns( void )
      {
         if( m_bNetworkActive )
         {
            (m_bServerServes) ? m_bServerServes = false  : m_bServerServes = true;
            (m_bP1Turn) ? m_bP1Turn = false: m_bP1Turn = true;
         }
         else
         {
            (m_bP1Turn) ? m_bP1Turn = false: m_bP1Turn = true;
         }
      }
      void ToggleSound( void )
      {
         (m_bSoundOn) ? m_bSoundOn = false: m_bSoundOn = true;
      }
      void ToggleShowOptionsSubMenu( void )
      {
         (m_bShowOptionsSubMenu) ? m_bShowOptionsSubMenu = false : m_bShowOptionsSubMenu = true;
      }
      void ToggleP1MoveWithKeyPad( void )
      {
         (m_bP1MoveWithKeyPad) ? m_bP1MoveWithKeyPad=false: m_bP1MoveWithKeyPad = true;
      }
      void ToggleP2MoveWithKeyPad( void )
      {
         (m_bP2MoveWithKeyPad) ? m_bP2MoveWithKeyPad=false: m_bP2MoveWithKeyPad = true;
      }
      void ToggleIsServer( void )
      {
         (m_bisServer) ? m_bisServer = false : m_bisServer = true;
      }
      void ToggleTrackMouse( void )
      {
         (m_bTrackMouse) ? m_bTrackMouse = false : m_bTrackMouse = true;
      }
      void ToggleDrawCommandList( void )
      {
         ( m_bDrawCommandList ) ? m_bDrawCommandList = false: m_bDrawCommandList = true;
      }

// Collision Detection
      bool CheckColideWithPaddle1( void )
      {
         if( ( (m_Ball.m_PositionVector.m_y-m_Ball.m_Radius) < (m_Paddle1.m_PositionVector.m_y+m_Paddle1.m_TexQuad.halfheight()) )  && 
             ( (m_Ball.m_PositionVector.m_y-m_Ball.m_Radius) > (m_Paddle1.m_PositionVector.m_y-m_Paddle1.m_TexQuad.halfheight()) )     )
            return true;
         return false;
      }

      bool CheckColideWithPaddle2( void )
      {
         if( ( (m_Ball.m_PositionVector.m_y-m_Ball.m_Radius) < (m_Paddle2.m_PositionVector.m_y+m_Paddle2.m_TexQuad.halfheight()) )  && 
             ( (m_Ball.m_PositionVector.m_y-m_Ball.m_Radius) > (m_Paddle2.m_PositionVector.m_y-m_Paddle2.m_TexQuad.halfheight()) )     )
            return true;
         return false;
      }

      bool CheckCollide( const Structs::fRect& Rect1, const Structs::fRect& Rect2 )
      {
         if( (Rect1.bottom < Rect2.top) ) return false;
         if( (Rect1.top > Rect2.bottom) ) return false;
         if( (Rect1.right < Rect2.left) ) return false;
         if( (Rect1.left > Rect2.right) ) return false;
         return true;
      }

      bool CheckCollide( const Structs::fRect& Rect, const Structs::dPoint3D& Point )
      {
         if( !IsInRange( Point.m_z, m_Z, 3.0 ) )
            return false;
         if( (Rect.bottom > Point.m_y) ) return false;
         if( (Rect.top    < Point.m_y) ) return false;
         if( (Rect.right  < Point.m_x) ) return false;
         if( (Rect.left   > Point.m_x) ) return false;
         return true;
      }

//      bool CheckCollide( const Structs::Rect& Rect1, const Structs::Rect& Rect2 )
//      {
//         if( (Rect1.bottom < Rect2.top) || (Rect1.top > Rect2.bottom) ) return false;
//         if( (Rect1.right < Rect2.left) || (Rect1.left > Rect2.right) ) return false;
//         return true;
//      }

// End Collision Detection

   protected:

      KeyInput             m_Keys;
      FrameRate            m_FrameRate;

      C_GLTextureManager     m_TextureManager;
      C_GLTextureMappedFont  m_Font;
      C_GLDisplayListManager m_DisplayListManager;
      C_GLErrorStrings       m_GLErrorStrings;

      C_GLButton             m_LeftButton,
                             m_RightButton,
                             m_OptionsButton,
                             m_MinButton,
                             m_CloseButton,
                             m_FullScreenButton,
                             m_ChatButton,
                             m_CommandsButton;

      C_GLListBox            m_GamesListBox,
                             m_GameOptionsListBox,
                             m_MultiplayerOptionsListBox,
                             m_DifficultyOptionsListBox,
                             m_DetailOptionsListBox,
                             m_GameKeysListBox,
                             m_ConnectModeListBox,
                             m_VSyncListBox,
                             m_SoundListBox;

      C_GLTexQuad         m_TexQuad;
      CGLRC              m_GLRC;
      CFmod               m_Fmod;
      C_GetDC             m_WinBaseDC;
      Structs::Rect     m_ClientRect;
      Structs::dRect    m_PlayingAreaRect;

      Structs::Point    m_P1ScorePos, 
                          m_P2ScorePos, 
                          m_RoundPosition,
                          m_MousePosition;

      Structs::fPoint   m_P1ConsolePos, 
                          m_P2ConsolePos,
                          m_GoToOptions;

      Structs::dPoint3D m_GLMousePosition;

      TimeInterval      m_deltaTimeBallServed,
                          m_P2PingInterval,
                          SendInterval;

      C_Flash             m_Flash, 
                          m_CursorFlash;

      Console             m_p1Console, 
                          m_p2Console;

      Taunts              m_IngameTaunts, 
                          m_WinGameTaunts, 
                          m_LoseGameTaunts,
                          m_LoseRoundTaunts,
                          m_WinRoundTaunts;

      Options           m_GameOptions, 
                          m_DifficultyOptions, 
                          m_MultiplayerOptions,
                          m_GameKeyOptions,
                          m_GameMode,
                          m_Detail;

      Timer               m_ChangeToGameMode, 
                          m_ChangeToDemoMode, 
                          m_ChangeToConnectMode,
                          m_TurnOffP2Console,
                          m_SendData,
                          m_PingTimer,
                          m_BytesSentTimer,
                          m_SendClickTimer;

      C_WinSockServer     m_Server;
      C_WinSockClient     m_Client;

      C_SAsyncSockStream   m_SendStream;
      C_RAsyncSockStream   m_RecvStream;

      SideScrollingText m_HelpSideScrollingText;

      std::ostringstream  m_RoundSStream, 
                          m_P1ScoreSStream, 
                          m_P2ScoreSStream,
                          m_GameSpeedSStream,
                          m_DifficultySStream,
                          m_MouseSStream,
                          m_PingStringStream,
                          m_BytesSentSStream,
                          m_SendBufferSizeSStream,
                          m_RecvBufferSizeSStream;

      std::string            m_RemoteHostAddress, 
                          m_LoadingErrors,
                          m_P1Nick,
                          m_P2Nick,
                          m_ConfigFileName,
                          m_Player2IP,
                          m_LastIP,
                          m_MasterServerHost,
                          m_MasterServerPath;

      short               m_PingFrequency,
                          m_SendTimeOut,
                          m_RecvTimeOut;

      long                m_TotalLinesAddedToP1Console,
                          m_RoundNumber,
                          m_P1Score,
                          m_P2Score,
                          m_GamesPlayed,
                          m_MaxScore,
                          m_ServeChangeScore,
                          m_Port,
                          m_TCPWindowSize,
                          m_Difficulty,
                          m_TestList,
                          m_send_interval;

      float               m_GameSpeed,
                          m_ArrowLWidth,
                          m_ArrowRWidth,
                          m_GoToOptionsWidth;

      double              m_deltaGameTime,
                          m_deltaGameTimeSquared,
                          m_RandomError,
                          m_halfPlayingAreaRectbottom,
                          m_halfPlayingAreaRectright,
                          m_halfplayingarea,
                          m_deltaTimeAIServBall,
                          m_RandomServePosition,
                          m_TileRotateAngle,
                          m_Z,
                          m_FieldOfView;

      short               m_x_pos;
      short               m_y_pos;
      short               m_Width;
      short               m_Height;


      DWORD               m_FrameDeltaTime,
                          m_FrameDeltaTimeSquared,
                          m_Ping;

      C_Ball              m_Ball;

      C_Paddle            m_Paddle1,
                          m_Paddle2;

      WinParseCommandLine m_CommandLine;

      LimitFPS            m_LimitFPS;

      bool                m_bFullScreen,
                          m_bisVisible,
                          m_bGamePaused, 
                          m_bP1Turn, 
                          m_bBallServed, 
                          m_bSoundOn,
                          m_bP1Computer, 
                          m_bP2Computer,
                          m_bShowOptionsSubMenu,
                          m_bDoOptionAction,
                          m_bP1MoveWithKeyPad,
                          m_bP2MoveWithKeyPad,
                          m_bOptionsModeWasVisited,
                          m_bisServer,
                          m_bP1DrawConsole,
                          m_bP2DrawConsole,
                          m_bNetworkActive,
                          m_bCaptureHostIP,
                          m_bDataChanged,
                          m_bDataSent,
                          m_bDataRecved,
                          m_bTrackMouse,
                          m_bP1Cheat,
                          m_bP2Cheat,
                          m_bDrawCommandList,
                          m_bHasKeyboardFocus,
                          m_bPositionChanged,
                          m_bPingRecved,
                          m_bServerServes,
                          m_bQuit,
                          m_bP2Chatting,
                          m_bAutoChangeGameSpeed,
                          m_bIgnoreReturn,
                          m_bVSyncOn,
                          m_bWindowPropertiesOverided,
                          m_bServerStarted,
                          m_bGameSavedOnServer;

      struct S_GameState
      {
         bool  m_bisVisible,   m_bGamePaused, m_bP1Turn, 
               m_bBallServed,  m_bSoundOn, m_bGameOver, 
               m_bP1Computer,  m_bP2Computer, m_bDemoMode,
               m_bTitleMode,   m_bGameMode, m_bGameOverMode,
               m_bOptionsMode, m_bShowOptionsSubMenu, m_bDoOptionAction;

         long  m_RoundNumber,  m_P1Score, m_P2Score, 
               m_GamesPlayed, m_MaxScore;
         S_GameState& operator = ( const S_GameState& rhs )
         {
            std::memcpy( this, &rhs, sizeof(S_GameState) );
			return *this;
         }
      };

      S_GameState m_GameState;
      std::stack< S_GameState > m_GameStateStack;

   private:
};

MainWindow::MainWindow( LPSTR lpszArgument ) :
   m_DisplayListManager(),
   m_GamesListBox(              "List of Available Games", 7, m_Font, 0 ),
   m_GameOptionsListBox(        "Options",                 10, m_Font, 0 ),
   m_MultiplayerOptionsListBox( "Online Play Options",     5, m_Font, 0 ),
   m_GameKeysListBox(           "GameKeys",                6, m_Font, 0 ),
   m_DifficultyOptionsListBox(  "Difficulty Options",      5, m_Font, 0, Normal ),
   m_DetailOptionsListBox(      "Detail Options",          5, m_Font, 0 ),
   m_ConnectModeListBox(        "Network Options",         5, m_Font, 0 ),
   m_VSyncListBox(              "Vsync Options",           5, m_Font, 0 ),
   m_SoundListBox(              "Sound Options",           5, m_Font, 0 ),
   m_MasterServerHost( "www.raptor85.com" ),
   m_MasterServerPath( "/try_catch_this/server/" ),
   m_GLRC( 32 ),
   m_ArrowLWidth( 1.5 ),
   m_ArrowRWidth( 1.5 ),
   m_GoToOptionsWidth( 1.5 ),
   m_P1ConsolePos( -28, 0 ),
   m_P2ConsolePos(   7, 0 ),
   m_CommandLine( lpszArgument, " -\"" ),
   m_SendTimeOut( 5000 ),
   m_RecvTimeOut( 5000 ),
   m_PingFrequency( 1000 ),
   m_send_interval( 50 ),
   m_Ping( 0 ),
   m_y_pos( 0 ),
   m_x_pos( 0 ),
   m_ConfigFileName( "pong.cfg.txt" ),
   m_P1Nick( "Computer1" ),
   m_P2Nick( "Computer2" ),
   m_HelpSideScrollingText( 55, 150 ),
   m_Width( 0 ),
   m_Height( 0 ),
   m_FieldOfView( 45 ),
   m_TestList( -1 ),
   m_TCPWindowSize( 3 ),
   m_Port( 7654 ),
   m_RemoteHostAddress( "127.0.0.1" ),
   m_TotalLinesAddedToP1Console( 0 ),
   m_Flash( true, 1000 ),
   m_CursorFlash( true,  500 ),
   m_bGameSavedOnServer( false ),
   m_bServerStarted( false ),
   m_bWindowPropertiesOverided( false ),
   m_bVSyncOn( true ),
   m_bIgnoreReturn( false ),
   m_bAutoChangeGameSpeed( true ),
   m_bP2Chatting( false ),
   m_bQuit( false ),
   m_bServerServes( true ),
   m_bPingRecved( false ),
   m_bPositionChanged( false ),
   m_bHasKeyboardFocus( false ),
   m_bDrawCommandList( false ),
   m_bP1Cheat( false ),
   m_bP2Cheat( false ),
   m_bTrackMouse( false ),
   m_bFullScreen( false ),
   m_bDataRecved( false ),
   m_bDataSent( false ),
   m_bDataChanged( false ),
   m_bCaptureHostIP( false ),
   m_bNetworkActive(  false ),
   m_bP1DrawConsole(  false ),
   m_bP2DrawConsole(  false ),
   m_bisServer( true ),
   m_bOptionsModeWasVisited( false ),
   m_bisVisible(  false ),
   m_bGamePaused( false ),
   m_bP1Turn(     false ),
   m_bBallServed( false ),
   m_bP1Computer( false ),
   m_bP2Computer( false ),
   m_bSoundOn(    true  ),
   m_bDoOptionAction( false ),
   m_bShowOptionsSubMenu( false ),
   m_bP1MoveWithKeyPad( true ),
   m_bP2MoveWithKeyPad( false ),
   m_TileRotateAngle( 0 ),
   m_Z( -60.0 ),
   m_p1Console( consolelines, consolelinelength ),
   m_p2Console( consolelines, consolelinelength ),
   m_halfPlayingAreaRectright( 0 ),
   m_halfPlayingAreaRectbottom( 0 ),
   m_GameSpeed(   1.0f ),
   m_deltaGameTime( 0 ),
   m_deltaGameTimeSquared( 0 ),
   m_RoundNumber( 0 ),
   m_GamesPlayed( 0 ),
   m_halfplayingarea( 0 ),
   m_P1Score( 0 ),
   m_P2Score( 0 ),
   m_Difficulty( Normal ),
   m_GameMode( 6 ),
   m_GameOptions( SOUND+1 ),
   m_Detail( 0 ),
   m_DifficultyOptions( 4 ),
   m_MultiplayerOptions( 3 ),
   m_GameKeyOptions( 2 ),
   m_ServeChangeScore( 2 ),
   m_RandomServePosition( 0 ),
   m_deltaTimeAIServBall( 0 ),
   m_MaxScore( 5 ),
   m_RandomError( 0 ),
   m_Ball( 0.3f, ballmass ),
   m_Paddle1( 0.5f, 5.0f ),
   m_Paddle2( 0.5f, 5.0f ),
   m_TexQuad( 17.5*2.0, 12.5*2.0 )
{
//   C_WinParseCommandLine CommandLine( lpszArgument, " -\"" );
//   for( int i=0; i<CommandLine.size(); i++ )
//   {
//      Alert( CommandLine(i) );
//   }

   AddMessageHandler( WM_LBUTTONUP,           reinterpret_cast<MessageHandler>(MWBaseOnLButtonUP)        );
   AddMessageHandler( WM_RBUTTONUP,           reinterpret_cast<MessageHandler>(MWBaseOnRButtonUP)        );
   AddMessageHandler( WM_LBUTTONDOWN,         reinterpret_cast<MessageHandler>(MWBaseOnLButtonD)         );
   AddMessageHandler( WM_RBUTTONDOWN,         reinterpret_cast<MessageHandler>(MWBaseOnRButtonD)         );
   AddMessageHandler( WM_LBUTTONDBLCLK,       reinterpret_cast<MessageHandler>(MWBaseOnLDoubleClick)     );
   AddMessageHandler( WM_RBUTTONDBLCLK,       reinterpret_cast<MessageHandler>(MWBaseOnRDoubleClick)     );

   AddMessageHandler( WM_MOVE,                reinterpret_cast<MessageHandler>(MWBaseOnMove)             );
   AddMessageHandler( WM_SIZE,                reinterpret_cast<MessageHandler>(MWBaseOnSize)             );
   AddMessageHandler( WM_CREATE,              reinterpret_cast<MessageHandler>(MWBaseOnCreate)           );
   AddMessageHandler( WM_COMMAND,             reinterpret_cast<MessageHandler>(MWBaseOnCommand)          );
   AddMessageHandler( WM_CHAR,                reinterpret_cast<MessageHandler>(MWBaseOnChar)             );
   AddMessageHandler( WM_KEYDOWN,             reinterpret_cast<MessageHandler>(MWBaseOnKeyDown)          );
   AddMessageHandler( WM_KEYUP,               reinterpret_cast<MessageHandler>(MWBaseOnKeyUp)            );
   AddMessageHandler( WM_CLOSE,               reinterpret_cast<MessageHandler>(MWBaseOnClose)            );
   AddMessageHandler( WM_TIMER,               reinterpret_cast<MessageHandler>(MWBaseOnTimer)            );
   AddMessageHandler( WM_TAUNT,               reinterpret_cast<MessageHandler>(MWBaseOnTaunt)            );
   AddMessageHandler( WM_MOUSEMOVE,           reinterpret_cast<MessageHandler>(MWBaseOnMouseMove)        );
   AddMessageHandler( WM_SYSCOMMAND,          reinterpret_cast<MessageHandler>(MWBaseOnSysCommand)       );
   AddMessageHandler( WM_ONSERVER,            reinterpret_cast<MessageHandler>(MWBaseOnServer)           );
   AddMessageHandler( WM_ONCLIENT,            reinterpret_cast<MessageHandler>(MWBaseOnClient)           );
   AddMessageHandler( WM_STARTSERVER,         reinterpret_cast<MessageHandler>(MWBaseOnStartServer)      );
   AddMessageHandler( WM_STARTCLIENT,         reinterpret_cast<MessageHandler>(MWBaseOnStartClient)      );
   AddMessageHandler( WM_TOGGLEFULLSCREEN,    reinterpret_cast<MessageHandler>(MWBaseOnToggleFullScreen) );
   AddMessageHandler( WM_SETFOCUS,            reinterpret_cast<MessageHandler>(MWBaseOnSetFocus)         );
   AddMessageHandler( WM_KILLFOCUS,           reinterpret_cast<MessageHandler>(MWBaseOnKillFocus)        );
   AddMessageHandler( WM_CONTEXT_LBUTTONDOWN, reinterpret_cast<MessageHandler>(MWBaseOnContextLbtn)      );
   AddMessageHandler( WM_CONTEXT_MOUSEOVER,   reinterpret_cast<MessageHandler>(MWBaseOnContextMouseOver) );

//   InitializeListBoxes();

   if( !m_Fmod.LoadClip( "bounce.wav" ) )
   {
      m_LoadingErrors += "bounce.wav not found\n";
      LogError( "bounce.wav not found" );
   }
   if( !m_Fmod.LoadClip( "newgame.wav" ) )
   {
      m_LoadingErrors += "newgame.wav not found\n";
      LogError( "newgame.wav not found" );
   }
   if( !m_Fmod.LoadClip( "boing.wav" ) )
   {
      m_LoadingErrors += "boing.wav not found\n";
      LogError( "boing.wav not found" );
   }
   if( !m_Fmod.LoadClip( "option.wav" ) )
   {
      m_LoadingErrors += "option.wav not found\n";
      LogError( "option.wav not found" );
   }
   if( !m_Fmod.LoadClip( "connect.wav" ) )
   {
      m_LoadingErrors += "connect.wav not found\n";
      LogError( "connect.wav not found" );
   }

   if( !m_IngameTaunts.LoadStringTaunts(    "taunts/ingametaunts.txt"   ) )
   {
      m_LoadingErrors += "taunts/ingametaunts.txt not found\n";
      m_p1Console.AddLine( "taunts/ingametaunts.txt not found" );
   }
   if( !m_WinGameTaunts.LoadStringTaunts(   "taunts/wingametaunts.txt"  ) )
   {
      m_LoadingErrors += "taunts/wingametaunts.txt not found\n";
      m_p1Console.AddLine( "taunts/wingametaunts.txt not found" );
   }
   if(! m_LoseGameTaunts.LoadStringTaunts(  "taunts/losegametaunts.txt" ) )
   {
      m_LoadingErrors += "taunts/losegametaunts.txt not found\n";
      m_p1Console.AddLine( "taunts/losegametaunts.txt not found" );
   }
   if( !m_LoseRoundTaunts.LoadStringTaunts( "taunts/loseroundtaunts.txt" ) )
   {
      m_LoadingErrors += "taunts/loseroundtaunts.txt not found\n";
      m_p1Console.AddLine( "taunts/loseroundtaunts.txt not found" );
   }
   if( !m_WinRoundTaunts.LoadStringTaunts(  "taunts/winroundtaunts.txt"  ) )
   {
      m_LoadingErrors += "taunts/winroundtaunts.txt not found\n";
      m_p1Console.AddLine( "taunts/winroundtaunts.txt not found" );
   }

   m_WinBase.SetStyle( WS_OVERLAPPEDWINDOW );
   m_WinBase.SetExStyle( WS_EX_TOPMOST );

   ResetBallAndPaddleVectors();
   ::srand( (unsigned int)(::GetTickCount()%10000) );
   SetDifficultySStream();

   m_DifficultyOptions.SetOnlyTrueAtCursor();
   m_GameOptions.SetOnlyTrueAtCursor();
   m_MultiplayerOptions.SetOnlyTrueAtCursor();
   m_GameKeyOptions.SetOnlyTrueAtCursor();

   m_WinBase.SetLargeIcon( ::LoadIcon( ::GetModuleHandle(NULL), MAKEINTRESOURCE( IDI_LARGEICON ) ) );
   m_WinBase.SetSmallIcon( ::LoadIcon( ::GetModuleHandle(NULL), MAKEINTRESOURCE( IDI_SMALLICON ) ) );

   m_p1Console.AddLine( " Type Here" );
   m_p1Console.AddLine( " /list for commands" );
   m_p1Console.AddLine( " At any time you can press the ~ key to toggle the console" );

   LoadConfig();
   EraseLog();
   EnableLogging();

   SendInterval.Begin();

   m_HelpSideScrollingText.AddText( " Press F12 to toggle fullscreen. "
                                    "Press F4 to turn off sound. "
                                    "Move Paddles With Up and Down or With the Mouse (see Options/GameKeys). "
                                    "Press tab while playing to show Options. "
                                    "Serve the ball With Left or Right ( when using Keyboard ) "
                                    "or Left Click (when using Mouse). " );
}

LRESULT MainWindow::MWBaseOnLDoubleClick( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   MWBaseOnContextLbtn( mw, wparam, lparam );
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDBLCLK, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRDoubleClick(     MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONDBLCLK, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRButtonUP(        MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONUP, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnLButtonUP(        MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_SendClickTimer.Kill();
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONUP, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnLButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_SendClickTimer.Set( mw.m_WinBase, SEND_CLICK_TIMER, 200, NULL );
   if( !mw.m_bGamePaused && !mw.m_bBallServed && ( mw.m_GameMode[ GAMEMODE ] || mw.m_GameMode[ DEMOMODE ] ) )
   {
      if( mw.m_bNetworkActive )
      {
      }
      else
      {
         if( !mw.m_bP1MoveWithKeyPad && !mw.m_bP1Computer )
            mw.m_bBallServed = true;
         else if( !mw.m_bP2MoveWithKeyPad && !mw.m_bP2Computer )
            mw.m_bBallServed = true;
      }
   }

   ::PostMessage( mw.m_WinBase, WM_CONTEXT_LBUTTONDOWN, 0, 0 );
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_LBUTTONDOWN, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnRButtonD( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_RBUTTONDOWN, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnMouseMove( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
//   if( mw.m_bTrackMouse )
//   {
      ::GetCursorPos( &mw.m_MousePosition );
      ::ScreenToClient( mw.m_WinBase, &mw.m_MousePosition );
      mw.m_MouseSStream.str( std::string() );
      Vector3D temp = mw.WindowToGLCoordinates( mw.m_MousePosition.x, mw.m_MousePosition.y );

//      GLint viewport[4];
//      ::glGetIntegerv( GL_VIEWPORT, viewport );
//      viewport
      if( IsInRange( temp[2], mw.m_Z, 3.0 ) )
      {
         mw.m_GLMousePosition.m_x = temp[0];
         mw.m_GLMousePosition.m_y = temp[1];
         mw.m_GLMousePosition.m_z = temp[2];
         mw.m_MouseSStream << "{ ";
         mw.m_MouseSStream << mw.m_GLMousePosition.m_x;
         mw.m_MouseSStream << ", ";
         mw.m_MouseSStream << mw.m_GLMousePosition.m_y;
         mw.m_MouseSStream << ", ";
         mw.m_MouseSStream << mw.m_GLMousePosition.m_z;
         mw.m_MouseSStream << " }";
      }   
//   }
   mw.MWBaseOnContextMouseOver( mw, wparam, lparam );
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_MOUSEMOVE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnContextMouseOver( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_OptionsButton.OnMouseOver(    mw.m_GLMousePosition );
   mw.m_CloseButton.OnMouseOver(      mw.m_GLMousePosition );
   mw.m_MinButton.OnMouseOver(        mw.m_GLMousePosition );
   mw.m_FullScreenButton.OnMouseOver( mw.m_GLMousePosition );
   mw.m_LeftButton.OnMouseOver(       mw.m_GLMousePosition );
   mw.m_RightButton.OnMouseOver(      mw.m_GLMousePosition );
   mw.m_ChatButton.OnMouseOver(       mw.m_GLMousePosition );
   mw.m_CommandsButton.OnMouseOver(   mw.m_GLMousePosition );

   if( mw.m_GameMode[ CONNECTMODE ] )
   {
      if( mw.CheckCollide( mw.m_GamesListBox.UpArrowRect(), mw.m_GLMousePosition ) )
      {
         mw.m_GamesListBox.MouseHoverArrowUp();
      }
      else
      {
         mw.m_GamesListBox.MouseHoverOffArrowUp();
      }
      if( mw.CheckCollide( mw.m_GamesListBox.DownArrowRect(), mw.m_GLMousePosition ) )
      {
         mw.m_GamesListBox.MouseHoverArrowDown();
      }
      else
      {
         mw.m_GamesListBox.MouseHoverOffArrowDown();
      }
   }
   if( mw.m_GameOptions[ OPTIONSMODE ] )
   {
      if( mw.m_bDoOptionAction )
      {
         if( mw.CheckCollide( mw.m_GameOptionsListBox.UpArrowRect(), mw.m_GLMousePosition ) )
         {
            mw.m_GameOptionsListBox.MouseHoverArrowUp();
         }
         else
         {
            mw.m_GameOptionsListBox.MouseHoverOffArrowUp();
         }
         if( mw.CheckCollide( mw.m_GameOptionsListBox.DownArrowRect(), mw.m_GLMousePosition ) )
         {
            mw.m_GameOptionsListBox.MouseHoverArrowDown();
         }
         else
         {
            mw.m_GameOptionsListBox.MouseHoverOffArrowDown();
         }
      }
      else
      {
      }
   }
   return 0;
}

LRESULT MainWindow::MWBaseOnContextLbtn(      MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_OptionsButton.OnClick(    mw.m_GLMousePosition );
   mw.m_CloseButton.OnClick(      mw.m_GLMousePosition );
   mw.m_MinButton.OnClick(        mw.m_GLMousePosition );
   mw.m_FullScreenButton.OnClick( mw.m_GLMousePosition );
   mw.m_LeftButton.OnClick(       mw.m_GLMousePosition );
   mw.m_RightButton.OnClick(      mw.m_GLMousePosition );
   mw.m_ChatButton.OnClick(       mw.m_GLMousePosition );
   mw.m_CommandsButton.OnClick(   mw.m_GLMousePosition );

   if( mw.m_GameMode[ GAMEMODE ] || mw.m_GameMode[ CONNECTMODE ] || mw.m_GameMode[ DEMOMODE ] )
   {
   }

   if( mw.m_GameMode[ CONNECTMODE ] )
   {
      if( mw.m_ConnectModeListBox.ClickRow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) ) )
      {
         if( mw.m_ConnectModeListBox.GetSelected() && mw.m_bisServer ) // Change to client
         {
            mw.ToggleIsServer();
         }
         if( !mw.m_ConnectModeListBox.GetSelected() && !mw.m_bisServer ) // Change to server
         {
            mw.ToggleIsServer();
         }
         mw.SetDrawP1Console( false );
         if( mw.m_bGameSavedOnServer )
         {
            mw.EraseGameFromMasterServer();
         }
      }
      mw.m_ConnectModeListBox.ClickUpArrow(   Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
      mw.m_ConnectModeListBox.ClickDownArrow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );

      if( mw.m_GamesListBox.ClickRow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) ) )
      {
         if( mw.m_bCaptureHostIP )
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
                  mw.m_p1Console.SetCurrentLine( *i );
                  mw.BuildP1ConsoleList();
               }
               else if( index == 3 ) // port
               {
                  mw.m_Port = atoi( (*i).c_str() );
               }
            }
         }
         else
         {
            mw.m_p1Console.SetCurrentLine( "Click Join Game To Start Client" );
            mw.BuildP1ConsoleList();
         }
      }
      mw.m_GamesListBox.ClickUpArrow(   Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
      mw.m_GamesListBox.ClickDownArrow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
   }
   if( mw.m_GameMode[ OPTIONSMODE ] )
   {
      if( !mw.m_bDoOptionAction )
      {
         if( mw.m_GameOptionsListBox.ClickRow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) ) )
         {
            mw.m_GameOptions.SetOnlyTrue( (long)mw.m_GameOptionsListBox.GetSelected() );
         }
         mw.m_GameOptionsListBox.ClickUpArrow(   Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
         mw.m_GameOptionsListBox.ClickDownArrow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
      }
      else
      {
         if( mw.m_GameOptions[ DIFFICULTY ] )
         {
            if( mw.m_DifficultyOptionsListBox.ClickRow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) ) )
            {
               mw.m_DifficultyOptions.SetOnlyTrue( (long)mw.m_DifficultyOptionsListBox.GetSelected() );
               mw.SetDifficultySStream();
            }
            mw.m_DifficultyOptionsListBox.ClickUpArrow(   Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
            mw.m_DifficultyOptionsListBox.ClickDownArrow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
//            Alert( (long)mw.m_DifficultyOptionsListBox.GetSelected() );
//            Alert( (long)mw.m_DifficultyOptions.GetCorsorPosition() );
         }
         else if( mw.m_GameOptions[ MULTIPLAYER ] )
         {
            if( mw.m_MultiplayerOptionsListBox.ClickRow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) ) )
            {
               mw.m_MultiplayerOptions.SetOnlyTrue( (long)mw.m_MultiplayerOptionsListBox.GetSelected() );
            }
            mw.m_MultiplayerOptionsListBox.ClickUpArrow(   Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
            mw.m_MultiplayerOptionsListBox.ClickDownArrow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
         }
         else if( mw.m_GameOptions[ GAMEKEYS ] )
         {
            if( mw.m_GameKeysListBox.ClickRow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) ) )
            {
               mw.m_GameKeyOptions.SetOnlyTrue( (long)mw.m_GameKeysListBox.GetSelected() );
            }
            mw.m_GameKeysListBox.ClickUpArrow(   Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
            mw.m_GameKeysListBox.ClickDownArrow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
         }
         else if( mw.m_GameOptions[ DETAIL ] )
         {
            if( mw.m_DetailOptionsListBox.ClickRow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) ) )
            {
               // Do something with this later
//               mw..SetOnlyTrue( (long)mw.m_DetailOptionsListBox.GetSelected() );
            }
            mw.m_DetailOptionsListBox.ClickUpArrow(   Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
            mw.m_DetailOptionsListBox.ClickDownArrow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
         }
         else if( mw.m_GameOptions[ VSYNC ] )
         {
            if( mw.m_VSyncListBox.ClickRow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) ) )
            {
               if( mw.m_VSyncListBox.GetSelected() )
               {
                  mw.SetVSync( 0 );
               }
               else
               {
                  mw.SetVSync( 1 );
               }
               // Do something with this later
            }
            mw.m_VSyncListBox.ClickUpArrow(   Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
            mw.m_VSyncListBox.ClickDownArrow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
         }
         else if( mw.m_GameOptions[ SOUND ] )
         {
            if( mw.m_SoundListBox.ClickRow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) ) )
            {
               if( !mw.m_SoundListBox.GetSelected() )
               {
                  if( !mw.m_bSoundOn )
                  {
                     mw.ToggleSound();
                  }
               }
               else
               {
                  if( mw.m_bSoundOn )
                  {
                     mw.ToggleSound();
                  }
               }
            }
            mw.m_SoundListBox.ClickUpArrow(   Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
            mw.m_SoundListBox.ClickDownArrow( Structs::fPoint( mw.m_GLMousePosition.m_x, mw.m_GLMousePosition.m_y ) );
         }
      }
   }
   return 0;
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

LRESULT MainWindow::MWBaseOnToggleFullScreen( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   if( mw.m_bFullScreen ) // go to windowed mode
   {
      int x_offset = (int)(::GetSystemMetrics( SM_CXSCREEN )*0.01),
          y_offset = (int)(::GetSystemMetrics( SM_CYSCREEN )*0.01);
      int width  = ::GetSystemMetrics( SM_CXSCREEN )-x_offset,
          height = ::GetSystemMetrics( SM_CYSCREEN )-y_offset;

      if( !::SetWindowLong( mw.m_WinBase, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE ) )
      {
         mw.m_p1Console.AddLine( "Call to SetWindowLong Failed" );
      }
      if( !::SetWindowLong( mw.m_WinBase, GWL_EXSTYLE, WS_EX_APPWINDOW ) )
      {
         mw.m_p1Console.AddLine( "Call to SetWindowLong Failed" );
      }
      if( ::ChangeDisplaySettings ( NULL, 0 ) != DISP_CHANGE_SUCCESSFUL )
      {
         mw.m_p1Console.AddLine( "Call to ChangeDisplaySettings Failed" );
      }
      if( mw.m_WinBase.Show( SW_NORMAL ) )
      {
      }
      if( ::SetWindowPos( mw.m_WinBase, HWND_TOP, (int)(x_offset/2), (int)(y_offset/2), width, height, 0 ) )
      {
//         mw.m_p1Console.AddLine( "Call to SetWindowPos Failed" );
      }
      if( !::SetForegroundWindow( mw.m_WinBase ) )
      {
         mw.m_p1Console.AddLine( "Call to SetForegroundWindow Failed" );
      }
      mw.m_bFullScreen = false;
      mw.m_p1Console.AddLine( "FullScreen Off" );
   }      
   else // go to full screen mode
   {
      int width  = ::GetSystemMetrics( SM_CXSCREEN ),
          height = ::GetSystemMetrics( SM_CYSCREEN );
      int bits_per_pixel = 32;

      if( !::SetWindowLong( mw.m_WinBase, GWL_STYLE, WS_POPUP | WS_VISIBLE ) )
      {
         mw.m_p1Console.AddLine( "Call to SetWindowLong Failed" );
      }
      if( !::SetWindowLong( mw.m_WinBase, GWL_EXSTYLE, WS_EX_TOPMOST ) )
      {
         mw.m_p1Console.AddLine( "Call to SetWindowLong Failed" );
      }
      if( !mw.ChangeScreenResolution( width, height, bits_per_pixel, CDS_FULLSCREEN ) )
      {
         mw.m_p1Console.AddLine( "Call to ChangeScreenResolution Failed" );
      }
      if( !mw.m_WinBase.Show( SW_NORMAL ) )
      {
      }
      if( ::SetWindowPos( mw.m_WinBase, HWND_TOP, 0, 0, width, height, 0 ) )
      {
//         mw.m_p1Console.AddLine( "Call to SetWindowPos Failed" );
//         mw.m_p1Console.AddLine( GetLastSystemError() );
      }
      mw.m_bFullScreen = true;
      mw.m_p1Console.AddLine( "FullScreen On" );
   }
   mw.BuildP1ConsoleList();
   return 0;
}

LRESULT MainWindow::MWBaseOnStartServer( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_bisServer      = true;
//   mw.m_bP1DrawConsole = true;
   mw.SetDrawP1Console( true );
   mw.m_bCaptureHostIP = false;
   mw.m_Client.close();
//   mw.m_Server.close();
   mw.m_SendData.Kill();
   mw.m_p1Console.Clear();
   mw.m_p2Console.Clear();

//  mw.m_Server.EnumerateLocalAddresses();
//  if( mw.m_Client.SetWindowSize( mw.m_TCPWindowSize ) )
//  {
//     mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError() );
//     return -1;
//  }

   if( mw.m_Server.GetServerSocket() == INVALID_SOCKET )
   {
      if( mw.m_Server.open() == INVALID_SOCKET )
      {
         mw.m_p1Console.AddLine( "Could Not Open Socket" );
         mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError() );
         mw.BuildP1ConsoleList();
         return -1;
      }
   }

   mw.m_p1Console.AddLine( std::string("Your IP is : ") + mw.m_Server.GetHostIpAddress() );
   mw.CopyTextToClipBoard( mw.m_Server.GetHostIpAddress() );
   mw.m_p1Console.AddLine( std::string("Your IP has been copied to the clip board") );
  
   if( mw.m_Server.Async( mw.m_LastHwndPassed, WM_ONSERVER ) == SOCKET_ERROR )
   {
      mw.m_p1Console.AddLine( "Call to WSAAsyncSelect failed" );
      mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError() );
      mw.BuildP1ConsoleList();
      return -1;
   }

   bool optval = true;
   if( mw.m_Server.setsockopt( SOL_SOCKET, SO_KEEPALIVE, (char*) &optval, sizeof(optval) ) == SOCKET_ERROR )
   {
      mw.m_p1Console.AddLine( "Call to setsockopt( SOL_SOCKET, SO_KEEPALIVE... failed" );
      mw.m_p1Console.AddLine( mw.m_Server.GetLastWinSockError() );
   }
//   if( mw.m_Server.setsockopt( SOL_SOCKET, SO_SNDTIMEO, (char*) &mw.m_SendTimeOut, sizeof(mw.m_SendTimeOut) ) == SOCKET_ERROR )
//   {
//      mw.m_p1Console.AddLine( "Call to setsockopt( SOL_SOCKET, SO_SNDTIMEO... failed" );
//      mw.m_p1Console.AddLine( mw.m_Server.GetLastWinSockError() );
//   }
//   if( mw.m_Server.setsockopt( SOL_SOCKET, SO_RCVTIMEO,  (char*) &mw.m_RecvTimeOut, sizeof(mw.m_RecvTimeOut) ) == SOCKET_ERROR )
//   {
//      mw.m_p1Console.AddLine( "Call to setsockopt( SOL_SOCKET, SO_RCVTIMEO... failed" );
//      mw.m_p1Console.AddLine( mw.m_Server.GetLastWinSockError() );
//   }

   if( mw.m_Server.listen( mw.m_Port ) == SOCKET_ERROR )
   {
      int nError = ::WSAGetLastError();
      if( nError == WSAEWOULDBLOCK )
      {
//         mw.m_p1Console.AddLine( "Wating for connection..." );
      }
      else
      {
         mw.m_p1Console.AddLine( mw.m_Server.GetLastWinSockError() );
         mw.m_p1Console.AddLine( "Listen failed: Port probally in use" );
         mw.m_Server.close();
         mw.BuildP1ConsoleList();
         return -1;
      }
   }

   mw.m_p1Console.AddLine( "Connecting To Master Server" );
   mw.SaveGameOnMasterServer();
   mw.m_p1Console.AddLine( "Wating for connection..." );
   mw.m_bServerStarted = true;

   mw.m_WinBase.SetText( "Server " + mw.m_WinBase.GetTitle() );
   mw.m_FrameRate.SetTitle( mw.m_WinBase.GetTitle() );
   mw.BuildP1ConsoleList();
   return 0;
}

LRESULT MainWindow::MWBaseOnStartClient( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_bisServer      = false;
   mw.m_bP1DrawConsole = true;
   mw.SetDrawP1Console( true );
//   mw.m_Client.close();
   mw.m_Server.close();
   mw.m_SendStream.Clear();
   mw.m_RecvStream.Clear();

//   if( mw.m_Client.SetWindowSize( mw.m_TCPWindowSize ) )
//   {
//      mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError() );
//      return -1;
//   }
   mw.m_SendData.Kill();
   mw.m_p1Console.Clear();
   mw.m_p2Console.Clear();

   if( mw.m_Client.GetSocket() == INVALID_SOCKET )
   {
      if( mw.m_Client.open() == INVALID_SOCKET )
      {
         mw.m_p1Console.AddLine( "Could Not Open Socket" );
         mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError() );
         mw.BuildP1ConsoleList();
         return -1;
      }
   }

   if( mw.m_Client.Async( mw.m_LastHwndPassed, WM_ONCLIENT ) == SOCKET_ERROR )
   {
      mw.m_p1Console.AddLine( "Call to WSAAsyncSelect failed" );
      mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError() );
      mw.BuildP1ConsoleList();
      return -1;
   }

   bool optval = true;
   if( mw.m_Client.setsockopt( SOL_SOCKET, SO_KEEPALIVE, (char*) &optval, sizeof(optval) ) == SOCKET_ERROR )
   {
      mw.m_p1Console.AddLine( "Call to setsockopt( SOL_SOCKET, SO_KEEPALIVE... failed" );
      mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError() );
   }
//   if( mw.m_Client.setsockopt( SOL_SOCKET, SO_SNDTIMEO, (char*) &mw.m_SendTimeOut, sizeof(mw.m_SendTimeOut) ) == SOCKET_ERROR )
//   {
//      mw.m_p1Console.AddLine( "Call to setsockopt( SOL_SOCKET, SO_SNDTIMEO... failed" );
//      mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError() );
//   }
//   if( mw.m_Client.setsockopt( SOL_SOCKET, SO_RCVTIMEO,  (char*) &mw.m_RecvTimeOut, sizeof(mw.m_RecvTimeOut) ) == SOCKET_ERROR )
//   {
//      mw.m_p1Console.AddLine( "Call to setsockopt( SOL_SOCKET, SO_RCVTIMEO... failed" );
//      mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError() );
//   }

   std::ostringstream temp;
   temp << "Connecting to : " << mw.m_RemoteHostAddress << " : " << mw.m_Port;
   mw.m_p1Console.AddLine( temp.str() );

   std::string ip = mw.m_RemoteHostAddress;
//   if( mw.m_LastIP.size() )
//   {
//      ip = mw.m_LastIP;
//   }
//   else
//   {
//      ip = mw.m_RemoteHostAddress;
//   }
   mw.m_LastIP = mw.m_RemoteHostAddress;

   if( mw.m_Client.connect( mw.m_Port, ip ) == SOCKET_ERROR )
   {
      int nError = ::WSAGetLastError();
      if( nError == WSAEWOULDBLOCK )
      {
         mw.m_p1Console.AddLine( "Asynchronous connection attempt started." );
      }
      else
      {
         mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError() );
         mw.m_p1Console.AddLine( "Graceful shutdown in progress." );
         mw.EndOnlineGame();
         mw.m_SendStream.Clear();
         mw.m_Client.close();
         mw.m_p1Console.AddLine( "Async connect failed" );
         mw.BuildP1ConsoleList();
         return -1;
      }
   }
//   mw.m_p1Console.AddLine( "Asynchronous connection attempt started." );

   mw.m_WinBase.SetText( "Client " + mw.m_WinBase.GetTitle() );
   mw.m_FrameRate.SetTitle( mw.m_WinBase.GetTitle() );
   mw.BuildP1ConsoleList();
   return 0;
}


LRESULT MainWindow::MWBaseOnServer( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   int nError = WSAGETASYNCERROR( lparam );  // If an error occurred,
   if( nError )
   {
      switch( nError )
      {
         case WSAECONNRESET:
         {
//            REPORT_PROBLEM("Connection was aborted.");
            mw.m_p1Console.AddLine( mw.m_Server.GetLastWinSockError( (int)WSAGETSELECTERROR( lparam ) ) );
         } break;
//         case WSANETDOWN:
//         {
//            mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError( (int)WSAGETSELECTERROR( lparam ) ) );
//            mw.m_Client.close();
//            mw.m_bNetworkActive = false;
//         } return -1;
//         case WSAECONNREFUSED:
//         {
//            REPORT_PROBLEM("Connection was refused.");
//            mw.m_p1Console.AddLine( mw.Server.GetLastWinSockError( (int)WSAGETSELECTERROR( lparam ) ) );
//            mw.m_Server.close();
//         } return -1;
         default:
         {
            mw.m_p1Console.AddLine( mw.m_Server.GetLastWinSockError( (int)WSAGETSELECTERROR( lparam ) ) );
         }
      }
      mw.m_Server.close();
      mw.m_bNetworkActive = false;
      mw.BuildP1ConsoleList();
      return -1;
   }

   switch( WSAGETSELECTEVENT( lparam ) ) // Differentiate between the events
   {
      case FD_READ:
      {
         // This message indicates that you have just received data, and you must use the recv() function to get it.
         if( mw.m_RecvStream.recv( wparam ) == WSAEDISCON )
         {
            mw.m_p1Console.AddLine( "Graceful shutdown in progress." );
            mw.m_bNetworkActive = false;
         }
      } break;
      case FD_WRITE:
      {
         // This means that there is room in the buffer to send, and we must send using the send() function.
         mw.m_SendStream.send( wparam );
      } break;
      case FD_ACCEPT:
      {
         // This message indicates the receipt of the connect() request and you must send back an accept().
         if( mw.m_bNetworkActive )
         {
            packet p;
            p.size  = sizeof(packet);
            p.flags = packet::PACKET_FLAG_REJECT;
            ::send( wparam, (char*)&p, p.size , 0 );
            ::closesocket( wparam );
         }
         else
         {
            mw.m_Server.accept( wparam );
            mw.m_p1Console.AddLine( "Connection established " );
            mw.BuildP1ConsoleList();
            mw.StartOnlineGame();
            mw.m_bNetworkActive = true;
            mw.m_bPingRecved    = true; //start sending pings
            mw.m_bP2Chatting    = true;
            mw.m_bP2DrawConsole = true;
            mw.m_SendData.Set( mw.m_WinBase, SEND_DATA, 500, NULL );
            mw.m_PingTimer.Set( mw.m_WinBase, SEND_PING, mw.m_PingFrequency, NULL );
            mw.m_BytesSentTimer.Set( mw.m_WinBase, SET_BYTES_SENT, 1000, NULL );
            std::string text = GetFileVersionValue( NULL, "ProductVersion" );
            mw.m_SendStream.AddPacket( (char*)text.c_str(), text.size()+1, packet::PACKET_FLAG_VERSION );
            mw.m_SendStream.AddPacket( (char*)mw.m_P1Nick.c_str(), mw.m_P1Nick.size()+1, packet::PACKET_FLAG_NIC );
            mw.PlayEffect( CONNECT );
            mw.EraseGameFromMasterServer();
         }
      } break;
      case FD_CLOSE:
      {
         // This message indicates that the other side’s socket has shut down. This can be for several reasons.
         mw.m_p1Console.AddLine( "Disconnected " );
         mw.m_p1Console.AddLine( mw.m_Server.GetLastWinSockError() );
         mw.EndOnlineGame();
         mw.m_bNetworkActive = false;
         mw.m_bP2Chatting    = false;
         mw.BuildP1ConsoleList();
         mw.m_SendData.Kill();
         mw.m_PingTimer.Kill();
         mw.m_BytesSentTimer.Kill();
      } break;
   }
   return 0;
}

LRESULT MainWindow::MWBaseOnClient( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   int nError = WSAGETASYNCERROR( lparam ); // If an error occurred
   if( nError )
   {
      switch( nError )
      {
         case WSAECONNRESET:
         {
//            REPORT_PROBLEM("Connection was aborted.");
            mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError( (int)WSAGETSELECTERROR( lparam ) ) );
            mw.m_Client.close();
            mw.m_bNetworkActive = false;
            mw.BuildP1ConsoleList();
//            Alert( "WSAECONNRESET" );
         } break;
         case WSAECONNREFUSED:
         {
//            REPORT_PROBLEM("Connection was refused.");
            mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError( (int)WSAGETSELECTERROR( lparam ) ) );
            mw.m_Client.close();
            mw.m_bNetworkActive = false;
            mw.BuildP1ConsoleList();
//            Alert( "WSAECONNREFUSED" );
         } break;
//         case WSANETDOWN:
//         {
//            mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError( (int)WSAGETSELECTERROR( lparam ) ) );
//            mw.m_Client.close();
//            mw.m_bNetworkActive = false;
//         } return -1;
         default:
         {
            mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError( (int)WSAGETSELECTERROR( lparam ) ) );
         }
      }
      mw.m_p1Console.AddLine( "Disconnected " );
      mw.EndOnlineGame();
      mw.m_bNetworkActive = false;
      mw.m_bP2Chatting    = false;
      mw.m_SendData.Kill();
//         mw.m_PingTimer.Kill();
      mw.m_BytesSentTimer.Kill();
      mw.BuildP1ConsoleList();
      return -1;      
   }

   switch( WSAGETSELECTEVENT( lparam ) ) // Differentiate between the events
   {
      case FD_READ:
      {
         // This message indicates that you have just received data, and you must use the recv() function to get it.
         if( mw.m_RecvStream.recv( wparam ) == WSAEDISCON )
         {
            mw.m_p1Console.AddLine( "Graceful shutdown in progress." );
            mw.m_bNetworkActive = false;
         }
      } break;
      case FD_WRITE:
      {
         // This means that there is room in the buffer to send, and we must send using the send() function.
         mw.m_SendStream.send( wparam );
      } break;
      case FD_CONNECT:
      {
         // This message indicates that the server has received the connection request and has accepted.
         // Begin the connection attempt.  This will almost certainly
         // not complete immediately.
         mw.m_p1Console.AddLine( "Connection established " );
         mw.m_bNetworkActive = true;
         mw.m_bPingRecved    = true; //start sending pings
         mw.m_bP2Chatting    = true;
         mw.m_bP2DrawConsole = true;
         mw.StartOnlineGame();
         mw.BuildP1ConsoleList();
         mw.m_SendData.Set( mw.m_WinBase, SEND_DATA, 500, NULL );
         mw.m_PingTimer.Set( mw.m_WinBase, SEND_PING, mw.m_PingFrequency, NULL );
         mw.m_BytesSentTimer.Set( mw.m_WinBase, SET_BYTES_SENT, 1000, NULL );
         std::string text = mw.m_Client.GetHostIpAddress();
         mw.m_SendStream.AddPacket( (char*)text.c_str(), text.size()+1, packet::PACKET_FLAG_IP );
         text = GetFileVersionValue( NULL, "ProductVersion" );
         mw.m_SendStream.AddPacket( (char*)text.c_str(), text.size()+1, packet::PACKET_FLAG_VERSION );
         mw.m_SendStream.AddPacket( (char*)mw.m_P1Nick.c_str(), mw.m_P1Nick.size()+1, packet::PACKET_FLAG_NIC );
         mw.PlayEffect( CONNECT );
         mw.BuildPlayer2IPList();
      } break;
      case FD_CLOSE:
      {
         // This message indicates that the other side’s socket has shut down. This can be for several reasons.
         mw.m_Client.shutdown( SD_SEND );
         mw.m_p1Console.AddLine( "Disconnected " );
         mw.m_p1Console.AddLine( mw.m_Client.GetLastWinSockError() );
         mw.m_p1Console.AddLine( "User probally aborted" );
         mw.BuildP1ConsoleList();
         mw.EndOnlineGame();
         mw.m_bNetworkActive = false;
         mw.m_bP2Chatting    = false;
         mw.m_SendData.Kill();
         mw.m_PingTimer.Kill();
         mw.m_BytesSentTimer.Kill();
      } break;
   }

   return 0;
}

LRESULT MainWindow::MWBaseOnCreate( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_FrameRate.SetTitle( mw.m_WinBase.GetTitle().c_str() );
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_CREATE, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( LOWORD(wparam) )
   {
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_COMMAND, wparam, lparam );
}


LRESULT MainWindow::MWBaseOnTaunt( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( wparam )
   {
      case INGAME:
      {
         mw.m_p2Console.AddLine( mw.m_IngameTaunts.RandomTaunt(), mw.m_P2Nick );
      } break;
      case LOSTROUND:
      {
         mw.m_p2Console.AddLine( mw.m_LoseRoundTaunts.RandomTaunt(), mw.m_P2Nick );
      } break;
      case WONROUND:
      {
         mw.m_p2Console.AddLine( mw.m_WinRoundTaunts.RandomTaunt(), mw.m_P2Nick );
      } break;
      case LOSTGAME:
      {
//         mw.m_p2Console.AddLine( "Good Game!" );
//         mw.m_p2Console.AddLine( "You beat me!" );
         mw.m_p2Console.AddLine( mw.m_LoseGameTaunts.RandomTaunt(), mw.m_P2Nick );
         mw.m_TurnOffP2Console.Set( mw.m_WinBase, TURNOFFP2CONSOLE, 1000, 0 );
      } break;
      case WONGAME:
      {
//         mw.m_p2Console.AddLine( "Good Game!" );
//         mw.m_p2Console.AddLine( "Try harder" );
         mw.m_p2Console.AddLine( mw.m_WinGameTaunts.RandomTaunt(), mw.m_P2Nick );
         mw.m_TurnOffP2Console.Set( mw.m_WinBase, TURNOFFP2CONSOLE, 1000, 0 );
      } break;
   }

   mw.BuildP2ConsoleList();
   return 0;
}

LRESULT MainWindow::MWBaseOnTimer( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( wparam )
   {
      case SEND_CLICK_TIMER:
      {
         ::GetCursorPos(   &mw.m_MousePosition );
         ::ScreenToClient( mw.m_WinBase, &mw.m_MousePosition );
         mw.MWBaseOnContextLbtn( mw, 0, 0 );
//         ::PostMessage(    mw.m_WinBase, WM_KEYDOWN, 0, MAKELPARAM( (WORD)mw.m_MousePosition.x, (WORD)mw.m_MousePosition.y ) );
//         ::PostMessage(    mw.m_WinBase, WM_KEYUP,   0, MAKELPARAM( (WORD)mw.m_MousePosition.x, (WORD)mw.m_MousePosition.y ) );
      } break;
      case CHANGE_TO_DEMO_MODE:
      {
         mw.SetDemoModeTrue();
         mw.m_ChangeToDemoMode.Kill();
      } break;
      case TURNOFFP2CONSOLE:
      {
         mw.m_bP2DrawConsole = false;
         mw.m_TurnOffP2Console.Kill();
      } break;
      case SEND_DATA:
      {
         if( mw.m_string_buffer_queue.size() )
         {
            int size = mw.m_string_buffer_queue.front().size()+1;
			std::vector<char> str( size );
            std::strcpy( &str[0], mw.m_string_buffer_queue.front().c_str() );
            str[size-1] = '\0';
            mw.m_SendStream.AddPacket( &str[0], size, packet::PACKET_FLAG_TEXT );
            mw.m_string_buffer_queue.pop();
         }
         mw.SendData();
      } break;
      case SEND_PING:
      {
         if( mw.m_bisServer && mw.m_bNetworkActive && mw.m_bPingRecved )
         {
            mw.m_SendStream.AddPacket( NULL, 0, packet::PACKET_FLAG_SPING );
            mw.m_P2PingInterval.Begin();
            mw.m_bPingRecved = false;
            mw.CompensateGameSpeedForLag();
         }
      } break;
      case SET_BYTES_SENT:
      {
         mw.SetBytesSentSStream();
         mw.SetBufferSizeSStream();
      } break;
      default:
      {
      
      }
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_TIMER, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnKeyUp( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_Keys.ReleaseKey( wparam );
   if( mw.m_bNetworkActive )
   {
      if( mw.GameKeyReleased( wparam ) )
      {
         mw.m_bPositionChanged = false;
         if( mw.m_bNetworkActive )
         {
            if( (mw.m_bisServer && mw.m_bServerServes) || (!mw.m_bisServer && !mw.m_bServerServes) )
            {
               mw.CopyBallPositionForSending();
//               mw.m_SendStream.AddPacket( (char*)&mw.m_BallPosition, sizeof(S_BallPosition), packet::PACKET_FLAG_BALL_POSITION );
            }
            mw.CopyPaddlePositionForSending();
//            mw.m_SendStream.AddPacket( (char*)&mw.m_PaddlePosition, sizeof(S_PaddlePosition), packet::PACKET_FLAG_PADDLE_POSITION );
            mw.SendInterval.Begin();
         }
      }
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYUP, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnKeyDown( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   mw.m_Keys.PressKey( wparam );

   if( mw.m_bNetworkActive )
   {
      if( mw.GameKeyPressed( wparam ) )
      {
         if( mw.SendInterval.TimeSinceLastCall() >= mw.m_send_interval )
         {
            if( (mw.m_bisServer && mw.m_bServerServes) || (!mw.m_bisServer && !mw.m_bServerServes) )
            {
               mw.CopyBallPositionForSending();
//               mw.m_SendStream.AddPacket( (char*)&mw.m_BallPosition, sizeof(S_BallPosition), packet::PACKET_FLAG_BALL_POSITION );
            }
            mw.CopyPaddlePositionForSending();
//            mw.m_SendStream.AddPacket( (char*)&mw.m_PaddlePosition, sizeof(S_PaddlePosition), packet::PACKET_FLAG_PADDLE_POSITION );
            mw.SendInterval.Begin();
         }
         mw.m_bPositionChanged = true;
      }
   }

   if( wparam == VK_F3)
   {
      mw.ToggleDrawP2Console();
      mw.BuildP2ConsoleList();
   }

   if( KEYDOWN(VK_F12) )
   {
      ::PostMessage( mw.m_WinBase, WM_TOGGLEFULLSCREEN, 0, 0 );
   }

   if( mw.m_bP1DrawConsole )
   {
//      if( mw.m_GameMode[ CONNECTMODE ] )
//         mw.DoConnectKeyActions( wparam, lparam );

      if( KEYDOWN( VK_UP ) )
      {
//         std::vector< std::string > &temp = mw.m_p1Console;
//         int size = mw.m_p1Console.LinesAdded();
//         for( int i=0; i <= size ; i++ )
//         {
//            Alert( temp[i] );
//         }

//         Alert( (long)mw.m_p1Console.LinesAdded() );
//         Alert( (long) mw.m_p1Console.VerticalCursorUp() );
//         mw.m_p1Console.VerticalCursorUp();
//         mw.m_p1Console.AddString( mw.m_p1Console.GetLineAtVerticalCursor() );
//         mw.m_p1Console.AddString( mw.m_p1Console.GetLastLineAdded() );
         mw.m_p1Console.SetCurrentLine( mw.m_p1Console.GetLastLineAdded() );
      }
      else if( KEYDOWN( VK_DOWN ) )
      {
//         mw.m_p1Console.VerticalCursorDown();
//         mw.m_p1Console.AddString( mw.m_p1Console.GetLineAtVerticalCursor() );
//         mw.m_p1Console.AddString( mw.m_p1Console.GetLastLineAdded() );
         mw.m_p1Console.SetCurrentLine( mw.m_p1Console.GetLastLineAdded() );
      }

      if( KEYDOWN( VK_DELETE ) )
      {
         mw.m_p1Console.DeleteChar();
      }

      if( KEYDOWN(VK_SHIFT) )
      {
         if( KEYDOWN(VK_LEFT) )
            mw.m_p1Console.DecrementOffSet();
         else if( KEYDOWN(VK_RIGHT) )
            mw.m_p1Console.IncrementOffSet();
      }
      else
      {   
         if( KEYDOWN(VK_CONTROL) )
         {
//            if( KEYDOWN(VK_LEFT) )
//               mw.m_p1Console.MoveCursorHomeLeft();
               mw.m_p1Console.MoveCursorLeft();
//            else if( KEYDOWN(VK_RIGHT) )
//               mw.m_p1Console.MoveCursorHomeRight();
               mw.m_p1Console.MoveCursorRight();

         }
         else
         {
            if( KEYDOWN(VK_LEFT) )
               mw.m_p1Console.MoveCursorLeft();
            else if( KEYDOWN(VK_RIGHT) )
               mw.m_p1Console.MoveCursorRight();
         }

         if( KEYDOWN(VK_END) )
            mw.m_p1Console.MoveCursorEnd();
         else if( KEYDOWN(VK_HOME) )
            mw.m_p1Console.MoveCursorHome();

         if( (KEYDOWN(VK_CONTROL) && (wparam == 'V')) || (KEYDOWN(VK_SHIFT) && KEYDOWN(VK_INSERT)) )
         {
            mw.GetTextFromClipBoard();
         }
         else if( (KEYDOWN(VK_CONTROL) && (wparam == 'C')) || (KEYDOWN(VK_CONTROL) && KEYDOWN(VK_INSERT)) )
         {
            mw.CopyTextToClipBoard( mw.m_p1Console.SelectText() );
         }
         mw.BuildP1ConsoleList();
      }
   }
   else
   {
      if( mw.m_GameMode[ DEMOMODE ] )
         mw.DoDemoKeyActions( wparam, lparam );
      else if( mw.m_GameMode[ TITLEMODE ] )
         mw.DoTitleKeyActions( wparam, lparam );
      else if( mw.m_GameMode[ GAMEMODE ] )
         mw.DoGameKeyActions( wparam, lparam );
      else if( mw.m_GameMode[ GAMEOVERMODE ] )
         mw.DoGameOverModeKeyActions( wparam, lparam );
      else if( mw.m_GameMode[ OPTIONSMODE ] )
         mw.DoOptionsKeyActions( wparam, lparam );
      else if( mw.m_GameMode[ CONNECTMODE ] )
         mw.DoConnectKeyActions( wparam, lparam );
   }

   return ::DefWindowProc( mw.m_LastHwndPassed, WM_KEYDOWN, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnChar( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   if( wparam == '`' )
   {
      mw.ToggleDrawP1Console();
   }
   else if( mw.m_bP1DrawConsole )
   {
      if( wparam == '\r' && !mw.m_bIgnoreReturn )
      {
         mw.EndLine();
         mw.CheckCommand();

         if( mw.m_bNetworkActive )
         {
            if( mw.m_string_buffer_queue.size() )
            {
               int size = mw.m_string_buffer_queue.front().size()+1;
			   std::vector<char> str( size );
               std::strcpy( &str[0], mw.m_string_buffer_queue.front().c_str() );
               str[size-1] = '\0';
               mw.m_SendStream.AddPacket( &str[0], size, packet::PACKET_FLAG_TEXT );
               mw.m_string_buffer_queue.pop();
            }
            mw.SendData();
         }
      }
      else
      {
         if( mw.m_bIgnoreReturn )
         {
            mw.m_bIgnoreReturn = false;
         }
         else
         {
            if( (wparam != '\n') )
            {
               if( !mw.m_p1Console.AddChar( (char)wparam ) )
               {
                  MsgBeep( 0xFFFFFFFF );
               }
            }
         }
      }
      mw.BuildP1ConsoleList();
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_CHAR, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnSysCommand( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( wparam )
   {
      case SC_CLOSE:
      {
         ::glFlush();
         if( mw.m_bServerStarted )
         {
            mw.EraseGameFromMasterServer();
         }
         mw.m_GLRC.DisableOpenGL();
      } return ::PostMessage( mw, WM_QUIT, 0, 0 );
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_SYSCOMMAND, wparam, lparam );
}

LRESULT MainWindow::MWBaseOnClose( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   ::glFlush();
   mw.m_GLRC.DisableOpenGL();
   if( mw.m_bServerStarted )
   {
      mw.EraseGameFromMasterServer();
   }
   if( mw.m_bNetworkActive )
   {
      mw.m_Client.close();
      mw.m_Server.close();
   }
   return ::PostMessage( mw, WM_QUIT, 0, 0 );
}

LRESULT MainWindow::MWBaseOnSize( MainWindow &mw, WPARAM wparam, LPARAM lparam )
{
   switch( wparam )
   {
      case SIZE_MINIMIZED:
      {
         mw.m_bisVisible = false;
      } break;
      case SIZE_MAXIMIZED:
      {
         mw.m_bisVisible        = true;
         mw.Reshape( LOWORD(lparam), HIWORD(lparam) );
      } break;

      case SIZE_RESTORED:
      {
         mw.m_bisVisible        = true;
         mw.Reshape( LOWORD(lparam), HIWORD(lparam) );
      } break;
      default:
      {
         mw.m_bisVisible        = true;
         mw.Reshape( LOWORD(lparam), HIWORD(lparam) );
      } break;
   }
   return ::DefWindowProc( mw.m_LastHwndPassed, WM_SIZE, wparam, lparam );
}

void MainWindow::OptionsButtonClick(   void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      mw->SetOptionsModeTrue();
      mw->m_SendClickTimer.Kill();
   }   
}
void MainWindow::LeftButtonClick(   void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      ::PostMessage( mw->m_WinBase, WM_KEYDOWN, VK_ESCAPE, 0 );
//      ::PostMessage( mw->m_WinBase, WM_KEYUP,   VK_ESCAPE, 0 );
      mw->m_SendClickTimer.Kill();
   }
}
void MainWindow::RightButtonClick(       void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      ::PostMessage( mw->m_WinBase, WM_KEYDOWN, VK_RETURN, 0 );
      mw->m_SendClickTimer.Kill();
   }
}
void MainWindow::MinButtonClick(         void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      ::ShowWindow( mw->m_WinBase, SW_MINIMIZE );
      mw->m_SendClickTimer.Kill();
   }
}
void MainWindow::CloseButtonClick(       void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      ::PostMessage( mw->m_WinBase, WM_QUIT, 0, 0 );
      mw->m_SendClickTimer.Kill();
   }
}
void MainWindow::FullScreenButtonClick(  void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      ::PostMessage( mw->m_WinBase, WM_TOGGLEFULLSCREEN, 0, 0 );
      mw->m_SendClickTimer.Kill();
   }
}
void MainWindow::ChatButtonClick(        void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      mw->ToggleDrawP1Console();
      mw->m_SendClickTimer.Kill();
   }
}
void MainWindow::CommandsButtonClick(        void * argument )
{
   if( argument )
   {
      MainWindow *mw = reinterpret_cast<MainWindow*>( argument );
      mw->CheckCommand( "/list" );
      mw->m_SendClickTimer.Kill();
   }
}

/*
   if( mw.m_GameMode[ CONNECTMODE ] || mw.m_GameMode[ GAMEOVERMODE ] || mw.m_GameMode[ OPTIONSMODE ] )
   {
      if( mw.CheckCollide( Rect1, mw.m_GLMousePosition ) )
      {
         if( !mw.m_bP1DrawConsole )
         {
            ::PostMessage( mw.m_WinBase, WM_KEYDOWN, VK_ESCAPE, 0 );
            ::PostMessage( mw.m_WinBase, WM_KEYUP,   VK_ESCAPE, 0 );
         }
         else
         {
            MsgBeep( 0xFFFF );
         }
      }
      if( mw.CheckCollide( Rect2, mw.m_GLMousePosition ) )
      {
         if( !mw.m_bP1DrawConsole )
         {
            ::PostMessage( mw.m_WinBase, WM_KEYDOWN, VK_RETURN, 0 );
            ::PostMessage( mw.m_WinBase, WM_KEYUP,   VK_RETURN, 0 );
         }
         else
         {
            MsgBeep( 0xFFFF );
         }
      }
   }

      if( mw.CheckCollide( GoToOptionsRect, mw.m_GLMousePosition ) )
      {
         if( mw.m_GameMode[ DEMOMODE ] )
         {
//            ::PostMessage( mw.m_WinBase, WM_KEYDOWN, VK_RETURN, 0 );
//            ::PostMessage( mw.m_WinBase, WM_KEYUP,   VK_RETURN, 0 );
//            ::PostMessage( mw.m_WinBase, WM_KEYDOWN, VK_TAB,    0 );
//            ::PostMessage( mw.m_WinBase, WM_KEYUP,   VK_TAB,    0 );
            mw.SetOptionsModeTrue();
         }
         else if( mw.m_GameMode[ GAMEMODE ] )
         {
            mw.m_bGamePaused = true;
            mw.PushGameState();
            mw.ResetGameState();
            mw.SetOptionsModeTrue();
            mw.m_bGamePaused = true;
         }
         else if( mw.m_GameMode[ CONNECTMODE ] )
         {
            mw.SetOptionsModeTrue();
         }
         mw.BuildGotoOptionsList();
         return 0;
      }
*/

void MainWindow::InitializeListButtons( void )
{
   Structs::Point TextCoords[4];

//   ::glTexCoord2f(0.0f, 0.0f); // Bottom Left  Of The Texture
//   ::glTexCoord2f(1.0f, 0.0f); // Bottom Right Of The Texture
//   ::glTexCoord2f(1.0f, 1.0f); // Top    Right Of The Texture
//   ::glTexCoord2f(0.0f, 1.0f); // Top    Left  Of The Texture

   // Left Button
   m_LeftButton.SetTexture( m_TextureManager[TEXTURE_ARROW] );
   m_LeftButton.SetPosition( -8.0, -22.0, m_Z );
   m_LeftButton.SetSize( Structs::fPoint( 2.5, 2.5 ), Structs::fPoint( 1.50, 1.50 ) );
   m_LeftButton.SetClickCallBack( C_GLButton::CallBackAndArgument( LeftButtonClick, this ) );
//   m_LeftButton.SetTextCoordsOrder( TextCoords );

   // Right Button
   m_RightButton.SetTexture( m_TextureManager[TEXTURE_ARROW] );
   m_RightButton.SetPosition( 8.0, -22.0, m_Z );
   m_RightButton.SetSize( Structs::fPoint( 2.5, 2.5 ), Structs::fPoint( 1.50, 1.50 ) );
   m_RightButton.SetClickCallBack( C_GLButton::CallBackAndArgument( RightButtonClick, this ) );
   TextCoords[0].x = 1; TextCoords[0].y = 0;
   TextCoords[1].x = 0; TextCoords[1].y = 0;
   TextCoords[2].x = 0; TextCoords[2].y = 1;
   TextCoords[3].x = 1; TextCoords[3].y = 1;
   m_RightButton.SetTextCoordsOrder( TextCoords );

   // FullScreen Button
   m_FullScreenButton.SetTexture( m_TextureManager[TEXTURE_ARROW] );
   m_FullScreenButton.SetPosition( 20, 23.0, m_Z );
   m_FullScreenButton.SetSize( Structs::fPoint( 2.5, 2.5 ), Structs::fPoint( 1.50, 1.50 ) );
   m_FullScreenButton.SetClickCallBack( C_GLButton::CallBackAndArgument( FullScreenButtonClick, this ) );
   TextCoords[0].x = 1; TextCoords[0].y = 1;
   TextCoords[1].x = 1; TextCoords[1].y = 0;
   TextCoords[2].x = 0; TextCoords[2].y = 0;
   TextCoords[3].x = 0; TextCoords[3].y = 1;
   m_FullScreenButton.SetTextCoordsOrder( TextCoords );

   // Min Button
   m_MinButton.SetTexture( m_TextureManager[TEXTURE_MIN] );
   m_MinButton.SetPosition( 22, 23, m_Z );
   m_MinButton.SetSize( Structs::fPoint( 2.5, 2.5 ), Structs::fPoint( 1.50, 1.50 ) );
   m_MinButton.SetClickCallBack( C_GLButton::CallBackAndArgument( MinButtonClick, this ) );
//   m_MinButton.SetTextCoordsOrder( TextCoords );

   // Close Button
   m_CloseButton.SetTexture( m_TextureManager[TEXTURE_CLOSE] );
   m_CloseButton.SetPosition( 24, 23.0, m_Z );
   m_CloseButton.SetSize( Structs::fPoint( 2.5, 2.5 ), Structs::fPoint( 1.50, 1.50 ) );
   m_CloseButton.SetClickCallBack( C_GLButton::CallBackAndArgument( CloseButtonClick, this ) );
//   m_CloseButton.SetTextCoordsOrder( TextCoords );

   // Options
   m_OptionsButton.SetTexture( m_TextureManager[TEXTURE_ARROW] );
   m_OptionsButton.SetPosition( 0, -22.0, m_Z );
   m_OptionsButton.SetSize( Structs::fPoint( 2.5, 2.5 ), Structs::fPoint( 1.50, 1.50 ) );
   m_OptionsButton.SetClickCallBack( C_GLButton::CallBackAndArgument( OptionsButtonClick, this ) );
   TextCoords[0].x = 0; TextCoords[0].y = 0;
   TextCoords[1].x = 0; TextCoords[1].y = 1;
   TextCoords[2].x = 1; TextCoords[2].y = 1;
   TextCoords[3].x = 1; TextCoords[3].y = 0;
   m_OptionsButton.SetTextCoordsOrder( TextCoords );

  //  Chat Button
   m_ChatButton.SetTexture( m_TextureManager[TEXTURE_CHAT] );
   m_ChatButton.SetPosition( -15.0, -22.0, m_Z );
   m_ChatButton.SetSize( Structs::fPoint( 2.5, 2.5 ), Structs::fPoint( 1.50, 1.50 ) );
   m_ChatButton.SetClickCallBack( C_GLButton::CallBackAndArgument( ChatButtonClick, this ) );
   TextCoords[0].x = 1; TextCoords[0].y = 0;
   TextCoords[1].x = 0; TextCoords[1].y = 0;
   TextCoords[2].x = 0; TextCoords[2].y = 1;
   TextCoords[3].x = 1; TextCoords[3].y = 1;
   m_ChatButton.SetTextCoordsOrder( TextCoords );

   // Command Button
   m_CommandsButton.SetTexture( m_TextureManager[TEXTURE_HELP] );
   m_CommandsButton.SetPosition( -19.0, -22.0, m_Z );
   m_CommandsButton.SetSize( Structs::fPoint( 2.5, 2.5 ), Structs::fPoint( 1.50, 1.50 ) );
   m_CommandsButton.SetClickCallBack( C_GLButton::CallBackAndArgument( CommandsButtonClick, this ) );
   TextCoords[0].x = 0; TextCoords[0].y = 0;
   TextCoords[1].x = 1; TextCoords[1].y = 0;
   TextCoords[2].x = 1; TextCoords[2].y = 1;
   TextCoords[3].x = 0; TextCoords[3].y = 1;
   m_CommandsButton.SetTextCoordsOrder( TextCoords );
}

void MainWindow::InitializeListBoxes( void )
{
   m_GamesListBox.clear();
   m_GameOptionsListBox.clear();
   m_DifficultyOptionsListBox.clear();
   m_MultiplayerOptionsListBox.clear();
   m_GameKeysListBox.clear();
   m_DetailOptionsListBox.clear();
   m_ConnectModeListBox.clear();

   m_GamesListBox.SetArrowTexture( m_TextureManager[TEXTURE_ARROW] );
   m_GameOptionsListBox.SetArrowTexture( m_TextureManager[TEXTURE_ARROW] );
   m_DifficultyOptionsListBox.SetArrowTexture( m_TextureManager[TEXTURE_ARROW] );
   m_MultiplayerOptionsListBox.SetArrowTexture( m_TextureManager[TEXTURE_ARROW] );
   m_GameKeysListBox.SetArrowTexture( m_TextureManager[TEXTURE_ARROW] );
   m_DetailOptionsListBox.SetArrowTexture( m_TextureManager[TEXTURE_ARROW] );
   m_ConnectModeListBox.SetArrowTexture( m_TextureManager[TEXTURE_ARROW] );
   m_VSyncListBox.SetArrowTexture( m_TextureManager[TEXTURE_ARROW] );
   m_SoundListBox.SetArrowTexture( m_TextureManager[TEXTURE_ARROW] );

//   if( m_GLRC )
//   {
      std::list< std::string > Header;
      std::list< std::string > Row;

      // Online Game List
      Header.push_back( "Version  " );
      Header.push_back( "Nick     " );
      Header.push_back( "IP       " );
      Header.push_back( "Port     " );
      Header.push_back( "Ping     " );
      m_GamesListBox.SetHeader( Header );
      // Game Options
      Header.clear();
      Row.push_back( "Help" );                   m_GameOptionsListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Difficulty" );             m_GameOptionsListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Greetz" );                 m_GameOptionsListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Multiplayer And Online" ); m_GameOptionsListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Game Keys" );              m_GameOptionsListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Game Detail" );            m_GameOptionsListBox.AddRow( Row ); Row.clear();
      Row.push_back( "VSync" );                  m_GameOptionsListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Sound" );                  m_GameOptionsListBox.AddRow( Row ); Row.clear();
      // Difficulty
      Header.clear();
      Row.push_back( "Impossible" ); m_DifficultyOptionsListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Hard" );       m_DifficultyOptionsListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Normal" );     m_DifficultyOptionsListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Easy" );       m_DifficultyOptionsListBox.AddRow( Row ); Row.clear();
      // connect Options
      Header.clear();
      Row.push_back( "Single Player (vs Computer)" );                                m_MultiplayerOptionsListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Two Player (One Player Uses Mouse Other Uses Keyboard)" );     m_MultiplayerOptionsListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Two Player Online (Play Against Someone Over The Internet)" ); m_MultiplayerOptionsListBox.AddRow( Row ); Row.clear();
      // Game Keys
      Header.clear();
      Row.push_back( "Player  1 Uses Mouse Player 2 Uses Keyboard" ); m_GameKeysListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Player  2 Uses Mouse Player 1 Uses Keyboard" ); m_GameKeysListBox.AddRow( Row ); Row.clear();
      // Game Detail
      Header.clear();
      Row.push_back( "Detail potions go here (Not Implemented)" ); m_DetailOptionsListBox.AddRow( Row ); Row.clear();
      // Connect Mode
      Header.clear();
      Row.push_back( "Wait For Connection (Act as a Server)" );      m_ConnectModeListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Join Game           (Connect To a Server)" );  m_ConnectModeListBox.AddRow( Row ); Row.clear();
      // VSync
      Row.push_back( "VSync On" );   m_VSyncListBox.AddRow( Row ); Row.clear();
      Row.push_back( "VSync Off" );  m_VSyncListBox.AddRow( Row ); Row.clear();
      // Sound
      Row.push_back( "Sound On" );   m_SoundListBox.AddRow( Row ); Row.clear();
      Row.push_back( "Sound Off" );  m_SoundListBox.AddRow( Row ); Row.clear();
//   }   
}

void MainWindow::ParseCommandLine( void )
{
   if( m_CommandLine.find( "fs" ) )
   {
      ::PostMessage( m_WinBase, WM_TOGGLEFULLSCREEN, 0, 0 );
   }

   if( m_CommandLine.find( "vs" ) )
   {
      SetVSync( 0 );
   }

   if( m_CommandLine.find( "connect" ) )
   {
      SetConnectModeTrue();
      return;
   }
   SetTitleModeTrue();
}

bool MainWindow::EraseGameFromMasterServer( void )
{
   short buffer_length = 256;
   std::vector<TCHAR> error_msg_buffer( buffer_length );
   DWORD error_id      = 0;
   ::ZeroMemory( &error_msg_buffer[0], buffer_length );

   HINTERNET hInternet = ::InternetOpen( m_WinBase.GetTitle().c_str(), INTERNET_OPEN_TYPE_PRECONFIG,
                                         NULL, NULL, 0 ); // INTERNET_FLAG_ASYNC

   if( !hInternet )
   {
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_p1Console.AddLine( "Call to InternetOpen Failed:" );
      m_p1Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   std::string GetGamesURL   = m_MasterServerPath;
   std::string SaveGamesURL  = m_MasterServerPath;
   std::ostringstream    temp;
   SaveGamesURL          += "erasegame.php?";
   SaveGamesURL          += "game=pong";
   temp << m_Port;
   SaveGamesURL          += "&port=" + temp.str();
   SaveGamesURL          += "&nick=" + m_P1Nick;
   temp.str(std::string());
   temp << GetFileVersionValue( NULL, "ProductVersion" );
   SaveGamesURL          += "&version=" + temp.str();

   HINTERNET
   hConnection = ::InternetConnect( hInternet, m_MasterServerHost.c_str(), 80, 
                                  " "," ", INTERNET_SERVICE_HTTP, 0, 0 );

   if ( !hConnection )
   {
      ::InternetCloseHandle( hInternet );
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_p1Console.AddLine( "Call to InternetConnect Failed:" );
      m_p1Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   // Get data
   HINTERNET 
   hData = ::HttpOpenRequest( hConnection, "GET", SaveGamesURL.c_str(),
                              NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0 );

   if ( !hData )
   {
      ::InternetCloseHandle( hConnection);
      ::InternetCloseHandle( hInternet );
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_p1Console.AddLine( "Call to HttpOpenRequest Failed:" );
      m_p1Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   ::HttpSendRequest( hData, NULL, 0, NULL, 0 );

   CHAR buffer[2048];
   DWORD dwRead;
   std::string Contents;

   while( ::InternetReadFile( hData, buffer, 255, &dwRead ) )
   {
      if ( dwRead == 0 )
      {
         if( !Contents.size() )
         { 
            m_p1Console.AddLine( "No responce from Master Server" );
         }
         else
         {
            m_p1Console.AddLine( "Game Has Been Erased from the Master Server List" );
            if( Contents.find_first_of( "\r" ) != std::string::npos )
            {
               Contents.erase( Contents.find_first_of( "\r" ), 1 );
            }
            if( Contents.find_first_of( "\n" ) != std::string::npos )
            {
               Contents.erase( Contents.find_first_of( "\n" ), 1 );
            }
            m_p1Console.AddLine( "Reply from Master Server: " + Contents );
         }
         break;
      }
      buffer[dwRead] = 0;
      Contents += buffer;
   }

   ::InternetCloseHandle( hConnection );
   ::InternetCloseHandle( hInternet );
   ::InternetCloseHandle( hData );

   m_bGameSavedOnServer = false;
   return true;
}

bool MainWindow::SaveGameOnMasterServer( void )
{
   short buffer_length = 256;
   std::vector<TCHAR> error_msg_buffer( buffer_length );
   DWORD error_id      = 0;
   ::ZeroMemory( &error_msg_buffer[0], buffer_length );

   HINTERNET hInternet = ::InternetOpen( m_WinBase.GetTitle().c_str(), INTERNET_OPEN_TYPE_PRECONFIG,
                                         NULL, NULL, 0 ); // INTERNET_FLAG_ASYNC

   if( !hInternet )
   {
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_p1Console.AddLine( "Call to InternetOpen Failed:" );
      m_p1Console.AddLine( &error_msg_buffer[0] );
      return false;
   }


   std::string GetGamesURL   = m_MasterServerPath;
   std::string SaveGamesURL  = m_MasterServerPath;
   std::ostringstream    temp;
   SaveGamesURL          += "savegame.php?";
   SaveGamesURL          += "game=pong";
   temp << m_Port;
   SaveGamesURL          += "&port=" + temp.str();
   SaveGamesURL          += "&nick=" + m_P1Nick;
   temp.str(std::string());
   temp << GetFileVersionValue( NULL, "ProductVersion" );
   SaveGamesURL          += "&version=" + temp.str();

   HINTERNET
   hConnection = ::InternetConnect( hInternet, m_MasterServerHost.c_str(), 80, 
                                  " "," ", INTERNET_SERVICE_HTTP, 0, 0 );

   if ( !hConnection )
   {
      ::InternetCloseHandle( hInternet );
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_p1Console.AddLine( "Call to InternetConnect Failed:" );
      m_p1Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   // Get data
   HINTERNET 
   hData = ::HttpOpenRequest( hConnection, "GET", SaveGamesURL.c_str(),
                              NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0 );

   if ( !hData )
   {
      ::InternetCloseHandle( hConnection);
      ::InternetCloseHandle( hInternet );
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_p1Console.AddLine( "Call to HttpOpenRequest Failed:" );
      m_p1Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   ::HttpSendRequest( hData, NULL, 0, NULL, 0 );

   CHAR buffer[2048];
   DWORD dwRead;
   std::string Contents;

   while( ::InternetReadFile( hData, buffer, 255, &dwRead ) )
   {
      if ( dwRead == 0 )
      {
//         Alert( Contents );
         if( !Contents.size() )
         { 
            m_p1Console.AddLine( "No responce from Master Server" );
         }
         else
         {
            m_p1Console.AddLine( "Game Has Been Added to the Master Server List Of Games" );
            if( Contents.find_first_of( "\r" ) != std::string::npos )
            {
               Contents.erase( Contents.find_first_of( "\r" ), 1 );
            }
            if( Contents.find_first_of( "\n" ) != std::string::npos )
            {
               Contents.erase( Contents.find_first_of( "\n" ), 1 );
            }
            m_p1Console.AddLine( "Reply from Master Server: " + Contents );
         }
         break;
      }
      buffer[dwRead] = 0;
      Contents += buffer;
   }

   ::InternetCloseHandle( hConnection );
   ::InternetCloseHandle( hInternet );
   ::InternetCloseHandle( hData );
   m_bGameSavedOnServer = true;
   return true;
}

bool MainWindow::GetGamesFromMasterServer( void )
{
   short buffer_length = 256;
   std::vector<TCHAR> error_msg_buffer( buffer_length );
   DWORD error_id = 0;
   CHAR buffer[2048];
   DWORD dwRead;

   ::ZeroMemory( &error_msg_buffer[0], buffer_length );
   HINTERNET hInternet = ::InternetOpen( m_WinBase.GetTitle().c_str(), INTERNET_OPEN_TYPE_PRECONFIG,
                                         NULL, NULL, 0 ); // INTERNET_FLAG_ASYNC

   if( !hInternet )
   {
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_p1Console.AddLine( "Call to InternetOpen Failed:" );
      m_p1Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   HINTERNET
   hConnection = ::InternetConnect( hInternet, m_MasterServerHost.c_str(), 80, 
                                    " "," ", INTERNET_SERVICE_HTTP, 0, 0 );

   if ( !hConnection )
   {
      ::InternetCloseHandle( hInternet );
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_p1Console.AddLine( "Call to InternetConnect Failed:" );
      m_p1Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   std::string GetGamesURL   = m_MasterServerPath;
   GetGamesURL           += "getgames.php";

   HINTERNET
   hData = ::HttpOpenRequest( hConnection, "GET", GetGamesURL.c_str(),
                              NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, 0 );

   if ( !hData )
   {
      ::InternetCloseHandle( hConnection);
      ::InternetCloseHandle( hInternet );
      ::InternetGetLastResponseInfo( &error_id, &error_msg_buffer[0], (LPDWORD)&buffer_length );
      m_p1Console.AddLine( "Call to HttpOpenRequest Failed:" );
      m_p1Console.AddLine( &error_msg_buffer[0] );
      return false;
   }

   ::HttpSendRequest( hData, NULL, 0, NULL, 0 );

   ::ZeroMemory( buffer, 2048 );
   std::string Games;
   while( ::InternetReadFile( hData, buffer, 255, &dwRead ) )
   {
      if ( dwRead == 0 )
      {
//         Alert( Games );
         if( !Games.size() )
         {
            m_p1Console.AddLine( "No responce from Master Server" );
            m_p1Console.AddLine( "Your Internet Connection may presently be in offline mode" );
         }
         break;
      }
      buffer[dwRead] = 0;
      Games += buffer;
      if( Games.size() > 2048 )
         break;
   }

   ::InternetCloseHandle( hConnection );
   ::InternetCloseHandle( hInternet );
   ::InternetCloseHandle( hData );

   if( ParseGamesFromMasterServer( Games ) )
   {
      return true;
   }
//   std::ifstream games_buffer( "gamesbuffer.txt", std::ios::out );
//   if( !games_buffer.is_open() )
//   {
//      m_p1Console.AddLine( GetLastSystemError() );
//   }
//   else
//   {
//      games_buffer << buffer;
//   }
//   games_buffer.close();
   return false;
}


bool MainWindow::ParseGamesFromMasterServer( std::string& Games )
{
   std::list< std::string > NickIPAndPort;
   std::string temp;

   // Parses a string like this
   // game=pong+version=0.4.5.3+nick=try_catch_this+IP=127.0.0.1+port=7654
   while( (Games.find( "game=pong+" ) == 0) )
   {
      temp = "game=pong+";
      if( Games.find( temp ) == 0 )
      {
         Games.erase( 0, temp.size() );
      }

      temp  = "version=";

      if( Games.find( temp ) == 0 )
      {
         Games.erase( 0, temp.size() );
         NickIPAndPort.push_back( Games.substr( 0, Games.find_first_of( "+" ) ) );
         Games.erase( 0, Games.find_first_of( "+" )+1 );
      }

      temp = "nick=";

      if( Games.find( temp ) == 0 )
      {
         Games.erase( 0, temp.size() );
         NickIPAndPort.push_back( Games.substr( 0, Games.find_first_of( "+" ) ) );
         Games.erase( 0, Games.find_first_of( "+" )+1 );
      }

      temp = "IP=";

      if( Games.find( temp ) == 0 )
      {
         Games.erase( 0, temp.size() );
         NickIPAndPort.push_back( Games.substr( 0, Games.find_first_of( "+" ) ) );
         Games.erase( 0, Games.find_first_of( "+" )+1 );
      }

      temp = "port=";

      if( Games.find( temp ) == 0 )
      {
         Games.erase( 0, temp.size() );
         NickIPAndPort.push_back( Games.substr( 0, Games.find_first_of( "+" ) ) );
         Games.erase( 0, Games.find_first_of( "+" )+1 );
      }

      Games.erase( 0, 1 );
      Games.erase( 0, 1 );
      NickIPAndPort.push_back( "-" );
      m_GamesListBox.AddRow( NickIPAndPort );
      NickIPAndPort.clear();
   }
   return true;

/*
   std::string IP;
   std::string Port;
   std::list< std::string > row = m_GamesListBox.GetSelectedIndex();
   //    0        1      2     3     4
   //  version   nick   ip   port   ping
   int index = 0;
   for( std::list< std::string >::iterator i = row.begin();
       i != row.end();
       i++, index++ )
   {
      if( index == 2 ) // IP
      {
         IP = *i;
         m_p1Console.SetCurrentLine( *i );
         BuildP1ConsoleList();
      }
      else if( index == 3 ) // port
      {
         Port = (*i);
      }
   }

   if( ValidateIP( IP ) )
   {
      m_RemoteHostAddress = IP;
      m_Port              = atoi( Port.c_str() );
      return true;
   }
   else
   {
      m_p1Console.AddLine( "Invalid Reply from Master Server:" );
   }
*/
   return false;
}

bool MainWindow::LoadConfig( void )
{
   std::ifstream config( m_ConfigFileName.c_str(), std::ios::in );
   if( !config.is_open() )
      return false;
   std::string tmp;
   while( !std::getline( config, tmp ).eof() )
   {
      if( tmp.find( "Nick=" ) == 0 )
      {
         m_P1Nick = tmp.erase( 0, ::strlen( "Nick=" ) );
      }
      else if( tmp.find( "Port=" ) == 0 )
      {
         m_Port = ::atoi( tmp.erase( 0, ::strlen( "Port=" ) ).c_str() );
      }
      else if( tmp.find( "LastIP=" ) == 0 )
      {
         m_LastIP = tmp.erase( 0, ::strlen( "LastIP=" ) );
      }
      else if( tmp.find( "FPSLimit=" ) == 0 )
      {
         m_LimitFPS.SetFPS( ::atof( tmp.erase( 0, ::strlen( "FPSLimit=" ) ).c_str() ) );
      }
      else if( tmp.find( "VSync=" ) == 0 )
      {
         m_bVSyncOn =  ::atol( tmp.erase( 0, ::strlen( "VSync=" ) ).c_str() );
      }
      else if( tmp.find( "Height=" ) == 0 )
      {
         std::string temp = tmp.erase( 0, ::strlen( "Height=" ) );
         if( temp.size() )
         {
            m_bWindowPropertiesOverided = true;
            m_Height =  ::atol( temp.c_str() );
         }
      }
      else if( tmp.find( "Width=" ) == 0 )
      {
         std::string temp = tmp.erase( 0, ::strlen( "Width=" ) );
         if( temp.size() )
         {
            m_bWindowPropertiesOverided = true;
            m_Width =  ::atol( temp.c_str() );
         }
      }
      else if( tmp.find( "y_pos=" ) == 0 )
      {
         std::string temp = tmp.erase( 0, ::strlen( "y_pos=" ) );
         if( temp.size() )
         {
            m_bWindowPropertiesOverided = true;
            m_y_pos =  ::atol( temp.c_str() );
         }
      }
      else if( tmp.find( "x_pos=" ) == 0 )
      {
         std::string temp = tmp.erase( 0, ::strlen( "x_pos=" ) );
         if( temp.size() )
         {
            m_bWindowPropertiesOverided = true;
            m_x_pos =  ::atol( temp.c_str() );
         }
      }
      else if( tmp.find( "trackmouse=" ) == 0 )
      {
         m_bTrackMouse =  ::atol( tmp.erase( 0, ::strlen( "trackmouse=" ) ).c_str() );
      }
      else if( tmp.find( "masterserverhost=" ) == 0 )
      {
         m_MasterServerHost = tmp.erase( 0, ::strlen( "masterserverhost=" ) );
      }
      else if( tmp.find( "masterserverpath=" ) == 0 )
      {
         m_MasterServerPath = tmp.erase( 0, ::strlen( "masterserverpath=" ) );
      }
      else if( tmp.find( "fullscreen=" ) == 0 )
      {
         m_bFullScreen = ::atol( tmp.erase( 0, ::strlen( "fullscreen=" ) ).c_str() );
      }
   }
   config.close();
   return true;
}

bool MainWindow::SaveConfig( void )
{
   std::ofstream config( m_ConfigFileName.c_str(), std::ios::out );
   if( !config.is_open() )
      return false;
   config << "Nick="         << m_P1Nick                         << std::endl;
   config << "Port="         << m_Port                           << std::endl;
   config << "LastIP="       << m_LastIP                         << std::endl;
   config << "FPSLimit="     << m_LimitFPS.fps()                 << std::endl;
   config << "VSync="        << ((m_bVSyncOn) ? true : false)    << std::endl;
   config << "trackmouse="   << ((m_bTrackMouse) ? true : false) << std::endl;
   config << "fullscreen="   << ((m_bFullScreen) ? true : false) << std::endl;
   config << "masterserverhost=" << m_MasterServerHost           << std::endl;
   config << "masterserverpath=" << m_MasterServerPath           << std::endl;

   config << "# Begin: Leave these values alone, for debugging purposes only!" << std::endl;
   if( m_bWindowPropertiesOverided )
   {
      config << "Width="    << m_Width                       << std::endl;
      config << "Height="   << m_Height                      << std::endl;
      config << "x_pos="    << m_x_pos                       << std::endl;
      config << "y_pos="    << m_y_pos                       << std::endl;
   }
   else
   {
      config << "Width="    << std::endl;
      config << "Height="   << std::endl;
      config << "x_pos="    << std::endl;
      config << "y_pos="    << std::endl;
   }
   config << "# End" << std::endl;
   config.close();
   return true;
}

void MainWindow::GetDataFromNetWork( void )
{
   if( m_bNetworkActive )
   {
      if( m_RecvStream.PacketReceived() )
      {
         std::size_t size = m_RecvStream.GetPacketSize();
		 std::vector<char> temp( size+1 );
         ::ZeroMemory( &temp[0], size+1 );
         m_RecvStream.GetDataFromPacket( &temp[0] );
         packet * pack = reinterpret_cast<packet*>(&temp[0]);

         switch ( pack->flags )
         {
             case packet::PACKET_FLAG_CHATTING_ON:
             {
                m_bP2Chatting = true;
             } break;
             case packet::PACKET_FLAG_CHATTING_OFF:
             {
                m_bP2Chatting = false;
             } break;
             case packet::PACKET_FLAG_GAME_SPEED:
             {
                m_GameSpeed = *((float*)pack->data);
                SetGameSpeed();
             } break;
             case packet::PACKET_FLAG_BALL_POSITION:
             {
                std::memcpy( &m_BallPosition, pack->data, sizeof(S_BallPosition) );
                CopyBallPositionFromNetwork();
             } break;
             case packet::PACKET_FLAG_PADDLE_POSITION:
             {
                std::memcpy( &m_PaddlePosition, pack->data, sizeof(S_PaddlePosition) );
                CopyPaddlePositionFromNetwork();
             } break;
             case packet::PACKET_FLAG_TOGGLE_SERVES:
             {
//               if( m_bisServer )
//                  Alert( "Server PACKET_FLAG_TOGGLE_SERVES" );
//               else
//                  Alert( "Client PACKET_FLAG_TOGGLE_SERVES" );
//
                ToggleTurns();
             } break;
             case packet::PACKET_FLAG_STATUS:
             {
                if( (pack->size - sizeof(packet)) >= sizeof( S_GameStatus ) )
                {
                   std::memcpy( &m_GameStatus, pack->data, sizeof(S_GameStatus) );
                   CopyGameStatusFromNetwork();
                }
                SetRound();
                SetDifficultySStream();
                SetScore();
                SetGameSpeed();
             } break;
             case packet::PACKET_FLAG_TEXT:
             {
                 m_p2Console.AddLine( pack->data, m_P2Nick );
                 BuildP2ConsoleList();
                 if( !m_bHasKeyboardFocus )
                 {
                    ::FlashWindow( m_WinBase, true );
                    MsgBeep( 0xFFFFFFFF );
                 }
             } break;
             case packet::PACKET_FLAG_NIC:
             {
                 m_P2Nick = pack->data;
                 BuildP2NickList();
                 BuildP2ConsoleList();
             } break;
             case packet::PACKET_FLAG_VERSION:
             {
                std::string temp;
                if( m_bisServer )
                {
                   temp = "Client Version: ";
                }
                else
                {
                   temp = "Server Version: ";
                }
                if( ::strcmp( pack->data, GetFileVersionValue( NULL, "ProductVersion" ).c_str() ) )
                {
                   m_p1Console.AddLine( "Versions do not match this could cause serious errors." );
                   ShutDownNetork();
                }

                temp += pack->data;
                m_p1Console.AddLine( temp );
                m_p1Console.AddLine( "Press ~ to toggle console so you can serv" );
                BuildP1ConsoleList();
            } break;
            case packet::PACKET_FLAG_IP:
            {
               std::string temp ( "Client IP: " );
               temp += pack->data;
               m_Player2IP = pack->data;
               m_p1Console.AddLine( temp );
               BuildPlayer2IPList();
               BuildP1ConsoleList();
               m_LastIP = pack->data;
            } break;
            case packet::PACKET_FLAG_COMMAND:
            {
               CheckCommand( pack->data );
               BuildP1ConsoleList();
               BuildP2ConsoleList();
            } break;
            case packet::PACKET_FLAG_CURRENTPING: // get ping from server
            {
               if( !m_bisServer ) // just to make sure that this is the client
               {
                  m_Ping = *((unsigned long*)pack->data);
                  SetPingStringStream();
               }
            } break;
            case packet::PACKET_FLAG_RPING: // get the reply to ping
            {
               if( m_bisServer ) // just to make sure that this is the server
                                  // only the server should get this message
               {
                  m_bPingRecved = true;
                  m_Ping = m_P2PingInterval.TimeSinceLastCall(); // you send the ping in WM_TIMER
                  SetPingStringStream();
                  m_SendStream.AddPacket( (char*)&m_Ping, sizeof(m_Ping), packet::PACKET_FLAG_CURRENTPING );
               }
            } break;
            case packet::PACKET_FLAG_SPING: // send a reply to ping
            {
               if( !m_bisServer ) // just to make sure that this is the client
                                  // only the client should get this message
               {
                  m_SendStream.AddPacket( NULL, 0, packet::PACKET_FLAG_RPING );
                  SendData();
               }
            } break;
            case packet::PACKET_FLAG_REJECT:
            {
                m_p1Console.AddLine( "The server refused you connection, a game is already in progress" );
            } break;
            default:
            {
                m_p1Console.DisableLogging();
                m_p1Console.AddLine( "Wow some sort of packet error" );
                m_p1Console.EnableLogging();
            }
         }
      }
   }
}

void MainWindow::CompensateGameSpeedForLag( void )
{
   if( m_bAutoChangeGameSpeed )
   {
      if( m_Ping > 300 && m_Ping < 400 )
      {
         m_GameSpeed = 0.8;
      }
      else if( m_Ping > 400 && m_Ping < 500 )
      {
         m_GameSpeed = 0.7;
      }
      if( m_Ping > 500 && m_Ping < 600 )
      {
         m_GameSpeed = 0.6;
      }
   }
}

std::string MainWindow::BoolValue( bool bval )
{
   return ( bval ) ? std::string( "true" ) : std::string( "false" );
}

std::string MainWindow::GetVariable( std::string variable_name )
{
   std::string return_value;

/*
                          m_bisVisible,
                          m_bGamePaused, 
                          m_bP1Turn, 
                          m_bBallServed, 
                          m_bSoundOn,
                          m_bP1Computer, 
                          m_bP2Computer,
                          m_bShowOptionsSubMenu,
                          m_bDoOptionAction,
                          m_bP1MoveWithKeyPad,
                          m_bP2MoveWithKeyPad,
                          m_bOptionsModeWasVisited,
                          m_bisServer,
                          m_bP1DrawConsole,
                          m_bP2DrawConsole,
                          m_bNetworkActive,
                          m_bCaptureHostIP,
                          m_bDataChanged,
                          m_bDataSent,
                          m_bDataRecved,
                          m_bTrackMouse,
                          m_bP1Cheat,
                          m_bP2Cheat,
                          m_bDrawCommandList,
                          m_bHasKeyboardFocus,
                          m_bPositionChanged,
                          m_bPingRecved,
                          m_bServerServes,
                          m_bQuit;

*///sc /getvar m_bFullScreen
   if( variable_name.find( "m_bFullScreen" ) == 0 )
   {
      return_value = BoolValue( m_bFullScreen );
   }
   else if( variable_name.find( "m_bP1MoveWithKeyPad" ) == 0 )
   {
      return_value = BoolValue( m_bP1MoveWithKeyPad );
   }
   else if( variable_name.size() == 0 )
   {
      return_value = "No Variable Name Given";
   }
   else if( variable_name.find( "m_bServerServes" ) == 0 )
   {
      return_value = BoolValue( m_bServerServes );
   }
   else if( variable_name.find( "path" ) == 0 )
   {
      char lpFilename[ MAX_PATH ];
      ::ZeroMemory( lpFilename, MAX_PATH );
      ::GetModuleFileName( NULL, (LPTSTR) lpFilename, MAX_PATH );
      return_value = lpFilename;
   }
   else
   {
      return_value = "Variable name not found";
   }

   if( m_bNetworkActive )
   {
      m_string_buffer_queue.push( return_value );
   }

   return return_value;
}

int MainWindow::CheckCommand( std::string command )
{
   if( command.size() == 0  )
      command = m_p1Console.GetLastLineAdded();

   if( command[0] == '/' )
   {
      if( command.find( "/getvar " ) == 0 )
      {
         std::string var_name = GetVariable( command.erase( 0, ::strlen( "/getvar " ) ) );
         if( !m_bNetworkActive )
         {
            m_p1Console.AddLine( var_name );
         }
      }
      else if( command.find( "/loadconfig" ) == 0 )
      {
         LoadConfig();
      }
      else if( (command.find( "/exit" ) == 0) || (command.find("/quit") == 0) )
      {
         if( m_WinBase )
         {
            ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
         }
         else
         {
            m_bQuit = true;
         }
      }
      else if( command == "/lamer" )
      {
         if( m_GameMode[ GAMEMODE ] )
         {
            if( !m_bP1Computer )
            {
               m_bP1Computer = true;
               m_bP1Cheat    = true;
            }
            else if( !m_bP2Computer )
            {
               m_bP2Computer = true;
               m_bP2Cheat    = true;
            }
            m_p1Console.AddLine( "Cheat activated" );
         }
      }
      else if( (command == "/nolamer") )
      {
         if( m_GameMode[ GAMEMODE ] )
         {
            if( m_bP1Computer )
            {
               m_bP1Computer = false;
               m_bP1Cheat    = false;
            }
            else if( m_bP2Computer )
            {
               m_bP2Computer = false;
               m_bP2Cheat    = false;
            }
            m_p1Console.AddLine( "Cheat deactivated" );
         }
      }
      else if( (command == "/clear") )
      {
         m_p1Console.Reset();
      }
      else if( (command == "/clearall") )
      {
         m_p1Console.Reset();
         m_p2Console.Reset();
      }
      else if( command.find("/connect ") == 0 )
      {
         std::string ipstring = command.erase( 0, ::strlen("/connect ") );
         SetConnectModeTrue();
         GetHostIp( ipstring );
//         ::PostMessage( m_WinBase, WM_STARTCLIENT, 0, 0 );
      }
      else if( command.find("/listen ") == 0 )
      {
         m_Port = atoi( command.erase( 0, sizeof("/listen ")-1 ).c_str() );
         SetConnectModeTrue();
         ::PostMessage( m_WinBase, WM_STARTSERVER, 0, 0 );
      }
      else if( command.find("/ip") == 0 )
      {
         m_Server.EnumerateLocalAddresses();
         for( int i=0; i < m_Server.GetNumberofHostIpAddress() ; ++i )
            m_p1Console.AddLine( m_Server.GetHostIpAddress( i ) );
      }
      else if( command == "/port"  )
      {
         std::ostringstream portstringstream;
         portstringstream << m_Port;
         m_p1Console.AddLine( portstringstream.str() );
      }
      else if( command == "/lastsyserror"  )
      {
         m_p1Console.AddLine( GetLastSystemError() );
      }
      else if( command == "/lastwserror"  )
      {
         m_p1Console.AddLine( m_Server.GetLastWinSockError() );
      }
      else if( command == "/wserror"  )
      {
         std::ostringstream str;
         if( command.size() == ::strlen("/wserror") )
         {
            m_p1Console.AddLine( m_Server.GetLastWinSockError() );
         }
         else
         {
            m_p1Console.AddLine( m_Server.GetLastWinSockError( atoi( command.erase( 0, ::strlen("/wserror") ).c_str() ) ) );
         }
      }
      else if( command == "/taunt"  )
      {
         command.erase( 0, sizeof("/taunt") ).c_str();
         std::string taunt_type = command;
//         m_p1Console.AddLine( m_Server.GetLastWinSockError( atoi( command.erase( 0, sizeof("/taunt")-1 ).c_str() ) ) );
      }
      else if( command.find("/playeffect") == 0  )
      {
         if( m_bSoundOn )
         {
            if( !PlayEffect( atoi( command.erase( 0, ::strlen("/playeffect") ).c_str() ) ) )
               m_p1Console.AddLine( "Invalid effect number" );
         }
         else
         {
            m_p1Console.AddLine( "sound is off" );
         }
      }
      else if( command == "/path"  )
      {
         char lpFilename[ MAX_PATH ];
         ::ZeroMemory( lpFilename, MAX_PATH );
         ::GetModuleFileName( NULL, (LPTSTR) lpFilename, MAX_PATH );
         m_p1Console.AddLine( lpFilename );
      }
      else if( command.find( "/z " ) == 0 )
      {
         std::ostringstream str;
         m_Z = atof( command.erase( 0, ::strlen("/z ") ).c_str() );
         str << m_Z;

         m_Ball.m_Z    = m_Z;
         m_Paddle1.m_Z = m_Z;
         m_Paddle2.m_Z = m_Z;

         m_DisplayListManager.clear();
         InitLists();

         m_p1Console.AddLine( str.str() );
      }
      else if( command.find( "/z" ) == 0 )
      {
         std::ostringstream str;
         str << m_Z;
         m_p1Console.AddLine( str.str() );
      }
      else if( command.find( "/ballpos" ) == 0 )
      {
         std::ostringstream str;
         str << "{ x: ";
         str << m_Ball.m_PositionVector.m_x;
         str << ", y: ";
         str << m_Ball.m_PositionVector.m_y;
         str << ", z: ";
         str << m_Ball.m_Z;
         str << " }";
         m_p1Console.AddLine( str.str() );
      }
      else if( command.find( "/paddle1pos" ) == 0 )
      {
         std::ostringstream str;
         str << "{ x: ";
         str << m_Paddle1.m_PositionVector.m_x;
         str << ", y: ";
         str << m_Paddle1.m_PositionVector.m_y;
         str << ", z: ";
         str << m_Paddle1.m_Z;
         str << " }";
         m_p1Console.AddLine( str.str() );
      }
      else if( command.find( "/paddle2pos" ) == 0 )
      {
         std::ostringstream str;
         str << "{ x: ";
         str << m_Paddle2.m_PositionVector.m_x;
         str << ", y: ";
         str << m_Paddle2.m_PositionVector.m_y;
         str << ", z: ";
         str << m_Paddle2.m_Z;
         str << " }";
         m_p1Console.AddLine( str.str() );
      }
      else if( command.find( "/trackmouse" ) == 0 )
      {
         ToggleTrackMouse();
      }
      else if( (command.find( "/list" ) == 0) && (command.size() == ::strlen("/list") ) )
      {
         ToggleDrawCommandList();
      }
      else if( command.find( "/help" ) == 0 )
      {
         ToggleDrawCommandList();
      }
      else if( command.find( "/ver " ) == 0 )
      {
         std::string version = GetFileVersionValue( NULL, command.erase( 0, ::strlen("/ver ") ).c_str() );
         m_p1Console.AddLine( version );
      }
      else if( command.find( "/fullscreen" ) == 0 )
      {
         ::PostMessage( m_WinBase, WM_TOGGLEFULLSCREEN, 0, 0 );
      }
      else if( command.find( "/playingarea" ) == 0 )
      {
         std::ostringstream str;
         str << "{ left: ";
         str << m_PlayingAreaRect.left;
         str << ", top: ";
         str << m_PlayingAreaRect.top;
         str << ", right: ";
         str << m_PlayingAreaRect.right;
         str << ", bottom: ";
         str << m_PlayingAreaRect.bottom;
         str << " }";
         m_p1Console.AddLine( str.str() );
      }
      else if( command.find( "/ballserved" ) == 0 )
      {
         if( m_bBallServed )
         {
            m_p1Console.AddLine( "true" );
         }
         else
         {
            m_p1Console.AddLine( "false" );
         }
      }
      else if( command.find( "/testlist " ) == 0 )
      {
         m_TestList = atoi( command.erase( 0, ::strlen("/testlist ") ).c_str() );
      }
      else if( command.find( "/viewport" ) == 0 )
      {
         GLdouble viewport[4];
         ::glGetDoublev( GL_VIEWPORT, viewport );
         std::ostringstream str;
         str << "{ x: ";
         str << viewport[0];
         str << ", y: ";
         str << viewport[1];
         str << ", width: ";
         str << viewport[2];
         str << ", height: ";
         str << viewport[3];
         str << " }";
         m_p1Console.AddLine( str.str() );
      }
      else if( command.find( "/difficulty" ) == 0 )
      {
         std::ostringstream str;
         str << m_Difficulty;
         m_p1Console.AddLine( str.str() );

      }
      else if( command.find( "/bbox " ) == 0 )
      {
         Structs::dPoint pt = BoundingBox( fabsf( atoi( command.erase( 0, sizeof("/bbox ")-1 ).c_str() ) * ::sin( m_FieldOfView ) ) );
         std::ostringstream str;
         str << " x: ";
         str << pt.m_x;
         str << ", y: ";
         str << pt.m_y;
         m_p1Console.AddLine( str.str() );
      }
      else if( command.find( "/vsync " ) == 0 )
      {         
         m_p1Console.AddLine( SetVSync( atoi( command.erase( 0, sizeof("/vsync ")-1 ).c_str() ) ) );
      }
      else if( command.find( "/nick " ) == 0 )
      {
         m_P1Nick = command.erase( 0, sizeof("/nick ")-1 );
         if( m_P1Nick.size() > maxnicklength )
            m_P1Nick = m_P1Nick.substr( 0, maxnicklength );
         BuildP1NickList();
         m_p1Console.AddLine( std::string( "Your new nick is: " ) + m_P1Nick );
         if( m_bNetworkActive )
         {
            m_SendStream.AddPacket( (char*)m_P1Nick.c_str(), m_P1Nick.size()+1, packet::PACKET_FLAG_NIC );
         }
      }
      else if( command.find( "/sc " ) == 0 )
      {
         if( m_bNetworkActive )
         {
            std::string tmp = command.erase( 0, strlen("/sc ") );
            m_SendStream.AddPacket( (char*)tmp.c_str(), tmp.size()+1, packet::PACKET_FLAG_COMMAND );
         }
         else
         {
            m_p1Console.AddLine( std::string( "Network not Active" ) );
         }
      }
      else if( command.find( "/squeue" ) == 0 )
      {
//         for( std::queue<std::string>::iterator str = m_string_buffer_queue.begin(); str < m_string_buffer_queue.end() ; ++str )
//            m_p1Console.AddLine( str );
         if( m_string_buffer_queue.size() )
            m_p1Console.AddLine( m_string_buffer_queue.front() );
      }
      else if( command.find( "/startgame" ) == 0 )
      {
         SetGameModeTrue();
      }
      else if( command.find( "/sserv" ) == 0 )
      {
         if( m_bServerServes )
         {
            m_p1Console.AddLine( "true" );
         }
         else
         {
            m_p1Console.AddLine( "false" );
         }
      }
      else if( command.find( "/turn" ) == 0 )
      {
         if( m_bP1Turn )
         {
            m_p1Console.AddLine( "true" );
         }
         else
         {
            m_p1Console.AddLine( "false" );
         }
      }
      else if( command.find( "/sendint" ) == 0 )
      {
         std::ostringstream str;
         if( command.size() == ::strlen("/sendint") )
         {
            str << m_send_interval;
            m_p1Console.AddLine( std::string( "Send Interval set to: " ) + str.str() + " ms" );
         }
         else
         {
            m_send_interval = ::atoi( command.erase( 0, sizeof("/sendint")-1 ).c_str() );
            str << m_send_interval;
            m_p1Console.AddLine( std::string( "Send Interval set to: " ) + str.str() + " ms" );
         }
      }
      else if( command.find( "/log" ) == 0 )
      {
         std::ostringstream str;
         if( command.size() == ::strlen("/log") )
         {
            if( m_p1Console.isLogging() )
               m_p1Console.AddLine( "true" );
            else
               m_p1Console.AddLine( "false" );
         }
         else
         {
            if( ::atoi( command.erase( 0, ::strlen("/log") ).c_str() ) )
            {
               EnableLogging();
               m_p1Console.AddLine( "Logging On" );
            }
            else
            {
               DisableLogging();
               m_p1Console.AddLine( "Logging Off" );
            }
         }
      }
      else if( command.find( "/pingf" ) == 0 )
      {
         std::ostringstream str;
         if( command.size() == ::strlen("/pingf") )
         {
            str << m_PingFrequency;
            m_p1Console.AddLine( str.str() );
         }
         else
         {
            str << ::atoi( command.erase( 0, ::strlen("/pingf") ).c_str() );
            m_p1Console.AddLine( str.str() );
         }
      }
      else
      {
         m_p1Console.AddLine( "Unknown Command" );
//         Alert( "..." );
      }
   }
   return 0;
}

Structs::dPoint MainWindow::BoundingBox( const double z )
{
   Structs::dPoint pt;
   pt.m_x = fabsf( z * ::sin( m_FieldOfView ) );
   pt.m_y = pt.m_x * (m_Width/m_Height);              // x * aspecratio = y
//   pt.m_y = 2 * z * tan( 0.5*m_FieldOfView );
//   pt.m_x = pt.m_y * (m_Width/m_Height);
   return pt;
}

void MainWindow::EndLine( void )
{
   m_p1Console.EndLine( m_P1Nick );
   if( m_GameMode[ CONNECTMODE ] && m_bCaptureHostIP )
   {
      std::string temp = m_p1Console.GetLastLineAdded();
      if( temp.size() )
      {
         GetHostIp( temp );
      }
   }
   if( m_bNetworkActive )
   {
      std::string temp = m_p1Console.GetLastLineAdded();
      if( temp.size() )
      {
         if( temp[0] != '/' )
            m_string_buffer_queue.push( temp );
      }
   }
}

bool MainWindow::ValidateIP( std::string ipstring )
{
   int count = 0;
   for( int i = 0; i < ipstring.size(); ++i )
      if( ipstring[i] == '.') count++;

   unsigned long ip = ::inet_addr( ipstring.c_str() );

   if( (ip != INADDR_NONE) && (ip != 0) && (count == 3) )
      return true;

   return false;
}

void MainWindow::GetHostIp( std::string ipstring )
{
//   int count = 0;
//   for( int i = 0; i < ipstring.size(); ++i )
//      if( ipstring[i] == '.') count++;

//   unsigned long ip = ::inet_addr( ipstring.c_str() );

//   if( (ip != INADDR_NONE) && (ip != 0) && (count == 3) )
   if( ValidateIP( ipstring ) )
   {
      m_RemoteHostAddress = m_p1Console.GetLastLineAdded();
      ::PostMessage( m_WinBase, WM_STARTCLIENT, 0, 0 );
      m_bCaptureHostIP = false;
      m_Player2IP = m_RemoteHostAddress;
   }
   else
   {
      m_p1Console.AddLine( "Invalid address please try again" );
   }
   BuildP1ConsoleList();
}

std::string MainWindow::GetTextFromClipBoard( bool bAddTextToConsole )
{
   std::string text;
   LPTSTR  lptstrCopy = NULL;
   HGLOBAL hglbCopy   = NULL;

   try
   {
      if( !::IsClipboardFormatAvailable( CF_TEXT ) )
         throw 0;

      if( !::OpenClipboard( m_WinBase ) )
         throw std::string( "Could Not Open ClipBoard " );

      if( (hglbCopy = ::GetClipboardData( CF_TEXT )) == NULL )
         throw std::string( "Unable To Get ClipBoard Data " );

      if( (lptstrCopy = (LPTSTR)::GlobalLock(hglbCopy)) == NULL )
         throw std::string( "Clould Not Lock ClipBoard Data " );

      text = lptstrCopy;
      if( bAddTextToConsole )
      {
         m_p1Console.AddString( text );
      }

      ::GlobalUnlock( (HGLOBAL)hglbCopy );
//      if( !::GlobalUnlock( (HGLOBAL)hglbCopy ) )
//         throw std::string( "Unable to UnLockMemory " );

      if( !::CloseClipboard() )
         throw std::string( "Could Not Close ClipBoard " );
   }
   catch ( std::string error_msg )
   {
      m_p1Console.AddLine( std::string( "Error : " ) + error_msg + GetLastSystemError() );
      ::CloseClipboard();
   }
   catch( int i )
   {
      MsgBeep( 0xFFFFFFFF );
   }
   return text;
}

void MainWindow::CopyTextToClipBoard( std::string text )
{
   LPTSTR  lptstrCopy = NULL;
   HGLOBAL hglbCopy   = NULL;
   int     length     = (text.size()+1) * sizeof(TCHAR);

   try
   {
      if( !::OpenClipboard( m_WinBase ) )
         throw std::string( "Could Not Open ClipBoard " );

      if( !::EmptyClipboard() )
         throw std::string( "Could Not Empty Clip Board " );

      hglbCopy = ::GlobalAlloc( GMEM_MOVEABLE, length );
      if( hglbCopy == NULL )
         throw std::string( "Not Enough Memory to Copy String to ClipBoard " );
       
      if( ( lptstrCopy = (TCHAR*)::GlobalLock( (HGLOBAL)hglbCopy ) ) == NULL )
         throw std::string( "Unable to LockMemory" );

      memcpy( lptstrCopy, text.c_str(), length-1 );
      lptstrCopy[length] = 0;

      ::GlobalUnlock( (HGLOBAL)hglbCopy );
//      if( !::GlobalUnlock( (HGLOBAL)hglbCopy ) )
//         throw std::string( "Unable to UnLockMemory " );

      if( ::SetClipboardData( CF_TEXT, (HANDLE)hglbCopy ) == NULL )
         throw std::string( "Unable To Set ClipBoard Data " );

      if( !::CloseClipboard() )
         throw std::string( "Could Not Close ClipBoard " );
   }
   catch ( std::string error_msg )
   {
      m_p1Console.AddLine( std::string( "Error : " ) + error_msg + GetLastSystemError() );
      ::CloseClipboard();
   }
}

//void MainWindow::StartServer( void )
//{
//   m_bP1DrawConsole = true;
//   m_p1Console.AddLine( "Starting Server" );
//}

//void MainWindow::StartClient( void )
//{
//   m_bP1DrawConsole = true;
//   m_p1Console.AddLine( "Starting Client" );
//}

void MainWindow::PushGameState( void )
{
   m_GameState.m_bisVisible          = m_bisVisible;
   m_GameState.m_bGamePaused         = m_bGamePaused;
   m_GameState.m_bP1Turn             = m_bP1Turn;
   m_GameState.m_bBallServed         = m_bBallServed;
   m_GameState.m_bSoundOn            = m_bSoundOn;
   m_GameState.m_bP1Computer         = m_bP1Computer;
   m_GameState.m_bP2Computer         = m_bP2Computer;
   m_GameState.m_bDemoMode           = m_GameMode[ DEMOMODE ];
   m_GameState.m_bTitleMode          = m_GameMode[ TITLEMODE ];
   m_GameState.m_bGameMode           = m_GameMode[ GAMEMODE ];
   m_GameState.m_bGameOverMode       = m_GameMode[ GAMEOVERMODE ];
   m_GameState.m_bOptionsMode        = m_GameMode[ OPTIONSMODE ];
   m_GameState.m_bShowOptionsSubMenu = m_bShowOptionsSubMenu; 
   m_GameState.m_bDoOptionAction     = m_bDoOptionAction;

   m_GameState.m_RoundNumber = m_RoundNumber;
   m_GameState.m_P1Score     = m_P1Score;
   m_GameState.m_P2Score     = m_P2Score;
   m_GameState.m_GamesPlayed = m_GamesPlayed;
   m_GameState.m_MaxScore    = m_MaxScore;

   m_GameStateStack.push( m_GameState );
}

void MainWindow::PopGameState( void )
{
   if( m_GameStateStack.size() > 0 )
   {
      m_GameState = m_GameStateStack.top();
      m_GameStateStack.pop();

      m_bisVisible               = m_GameState.m_bisVisible;
      m_bGamePaused              = m_GameState.m_bGamePaused;
      m_bP1Turn                  = m_GameState.m_bP1Turn;
      m_bBallServed              = m_GameState.m_bBallServed;
      m_bSoundOn                 = m_GameState.m_bSoundOn;
      m_bP1Computer              = m_GameState.m_bP1Computer;
      m_bP2Computer              = m_GameState.m_bP2Computer;
      m_GameMode[ DEMOMODE ]     = m_GameState.m_bDemoMode;
      m_GameMode[ TITLEMODE ]    = m_GameState.m_bTitleMode;
      m_GameMode[ GAMEMODE ]     = m_GameState.m_bGameMode;
      m_GameMode[ GAMEOVERMODE ] = m_GameState.m_bGameOverMode;
      m_GameMode[ OPTIONSMODE ]  = m_GameState.m_bOptionsMode;
      m_bShowOptionsSubMenu      = m_GameState.m_bShowOptionsSubMenu; 
      m_bDoOptionAction          = m_GameState.m_bDoOptionAction;
      m_RoundNumber              = m_GameState.m_RoundNumber;
      m_P1Score                  = m_GameState.m_P1Score;
      m_P2Score                  = m_GameState.m_P2Score;
      m_GamesPlayed              = m_GameState.m_GamesPlayed;
      m_MaxScore                 = m_GameState.m_MaxScore;
   }
   SetScore();
   SetRound();
}

double MainWindow::Ball_y_PositionAt_x_Boundary( void )
{
   // This code is supposed to predict the position of the ball when it is supposed to hit the paddle

   // +y
   // ^
   // | 
   //  -->+x

   // first calculate the distance the ball will have to move in the x axis to the paddle
   double ball_x_distance_to_paddle = 
   fabs( 
        ( m_Ball.m_VelocityVector.m_x < 0 ) ? // if the ball is moving to the left
        (m_Ball.m_PositionVector.m_x-m_Ball.m_Radius) - (m_Paddle1.m_PositionVector.m_x+m_Paddle2.m_TexQuad.halfwidth()) :  // ball_x_pos    - paddle1_x_pos
        (m_Paddle2.m_PositionVector.m_x-m_Paddle2.m_TexQuad.halfwidth()) - (m_Ball.m_PositionVector.m_x+m_Ball.m_Radius) ); // paddle2_x_pos - ball_x_pos

   // next calculate the x component of the velocity of the ball when it hits the paddle

   double ball_x_speed_at_x_paddle =  // speed because we dont care about direction
   // from v^2 = u^2 + 2*a*s
   // v = sqrt                               u^2                            +   2 *                a                *            s    
          sqrt( ( m_Ball.m_VelocityVector.m_x*m_Ball.m_VelocityVector.m_x ) + ( 2 * m_Ball.m_AccelerationVector.m_x * (ball_x_distance_to_paddle) ) );

   double collision_delta_time = 
   fabs( 
   // from v = u+at
   // t =          v                -           u                        /         a 
          (ball_x_speed_at_x_paddle - fabs(m_Ball.m_VelocityVector.m_x) )/fabs(m_Ball.m_AccelerationVector.m_x));

   // next find the total distance that the ball will trave; in the y direction
   // so we can chop it into chuncks and finally get the displacement at the paddle

   double total_y_distance = 
   // from s =                            u*t                    + 0.5*                 a             *                    t^2
   fabs(       (m_Ball.m_VelocityVector.m_y*collision_delta_time)+(0.5*m_Ball.m_AccelerationVector.m_y*(collision_delta_time*collision_delta_time))) ;

   double y_displacement_at_boundary = 0;

   if( (fabs(total_y_distance) < m_halfplayingarea) )
   {
      if( m_Ball.m_VelocityVector.m_y >= 0 ) // if the ball is moving up
      {
         if(total_y_distance > (m_PlayingAreaRect.top-(m_Ball.m_PositionVector.m_y+m_Ball.m_Radius)) ) 
         {
            // if the distance the ball has left to travel is grater than the distance between the ball and the top of the game area the ball will hit the game area and reverse direction
            // so then                        top              -   total distacne - distance to top from ball
            y_displacement_at_boundary = m_PlayingAreaRect.top - (total_y_distance-(m_PlayingAreaRect.top-m_Ball.m_PositionVector.m_y+m_Ball.m_Radius));
         }
         else
            y_displacement_at_boundary = m_Ball.m_PositionVector.m_y + total_y_distance;
      }
      else
      {
         if( total_y_distance > ((m_Ball.m_PositionVector.m_y-m_Ball.m_Radius)-m_PlayingAreaRect.bottom) ) 
         {
            // if the distance the ball has left to travel is grater than the distance between the ball and the bottom of the game area the ball will hit the game area and reverse direction
            // so then                        top                 +   total distacne - distance to bottom from ball
            y_displacement_at_boundary = m_PlayingAreaRect.bottom + (total_y_distance-(m_PlayingAreaRect.top-m_Ball.m_PositionVector.m_y+m_Ball.m_Radius));
         }
         else
            y_displacement_at_boundary = m_Ball.m_PositionVector.m_y - total_y_distance;

      }
   }

   return y_displacement_at_boundary;
}

void MainWindow::RestrictPaddleToGameBoundaries( C_Paddle& Paddle )
{
   if( Paddle.m_PositionVector.m_y > m_PlayingAreaRect.top )      Paddle.m_PositionVector.m_y = m_PlayingAreaRect.top;
   if( (Paddle.m_PositionVector.m_y) < m_PlayingAreaRect.bottom ) Paddle.m_PositionVector.m_y = m_PlayingAreaRect.bottom;
}

void MainWindow::InterceptBallWithPaddle1( void )
{
   if( (m_Ball.m_VelocityVector.m_x < 0) )
   {
      MovePaddle1_y_To( Ball_y_PositionAt_x_Boundary()+(( m_bP1Cheat ) ? 0.0 : m_RandomError) );
   }
   else // Move paddle towards center
   {
      MovePaddle1_y_To( 0 );
   }
}

void MainWindow::InterceptBallWithPaddle2( void )
{
   if( (m_Ball.m_VelocityVector.m_x > 0) ) // if the ball is moving towards this paddle
   {
      MovePaddle2_y_To( Ball_y_PositionAt_x_Boundary()+(( m_bP2Cheat ) ? 0.0 : m_RandomError ));
   }
   else // Move paddle towards center
   {
      MovePaddle2_y_To( 0 );
   }
}

void MainWindow::MovePaddle1_y_To( const double y_pos )
{

   if( !IsInRange( y_pos, m_Paddle1.m_PositionVector.m_y, m_Paddle1.m_VelocityVector.m_y * m_deltaGameTime  ) )
   {
      if( y_pos > m_Paddle1.m_PositionVector.m_y )
      {
         if( m_Paddle1.m_VelocityVector.m_y < 0) m_Paddle1.m_VelocityVector.m_y *= -1.0f;
         UpdatePaddle1Position();
      }
      else
      {
         if( m_Paddle1.m_VelocityVector.m_y > 0) m_Paddle1.m_VelocityVector.m_y *= -1.0f;
         UpdatePaddle1Position();
      }
   }
}

void MainWindow::MovePaddle2_y_To( const double y_pos )
{

   if( !IsInRange( y_pos, m_Paddle2.m_PositionVector.m_y, m_Paddle2.m_VelocityVector.m_y * m_deltaGameTime  ) )
   {
      if( y_pos > m_Paddle2.m_PositionVector.m_y )
      {
         if( m_Paddle2.m_VelocityVector.m_y < 0) m_Paddle2.m_VelocityVector.m_y *= -1.0f;
         UpdatePaddle2Position();
      }
      else
      {
         if( m_Paddle2.m_VelocityVector.m_y > 0) m_Paddle2.m_VelocityVector.m_y *= -1.0f;
         UpdatePaddle2Position();
      }
   }
}

void MainWindow::UpdatePaddle1Position( void )
{
   m_Paddle1.m_PositionVector += (m_Paddle1.m_VelocityVector * m_deltaGameTime);
}

void MainWindow::UpdatePaddle2Position( void )
{
   m_Paddle2.m_PositionVector += (m_Paddle2.m_VelocityVector * m_deltaGameTime);
}

void MainWindow::ReverseBall_y_Direction( void )
{
   m_Ball.m_VelocityVector.m_y     *= -1.0f;
   m_Ball.m_AccelerationVector.m_y *= -1.0f;
}

void MainWindow::ReverseBall_x_Direction( void )
{
   m_Ball.m_VelocityVector.m_x     *= -1.0f;
   m_Ball.m_AccelerationVector.m_x *= -1.0f;
}

void MainWindow::StartNewGame( void )
{
   ResetGameState();
   if( m_GamesPlayed > 1 )
      ToggleTurns();
   SetScore();
   SetRound();
   PlayEffect( NEWGAME );
}

void MainWindow::StartNewRound( void )
{
   m_bGamePaused = false;
   m_bBallServed = false;
   m_bDoOptionAction = false;
   m_RoundNumber++;
   SetRandomError();
   SetInitialObjectPositions();
   SetScore();
   SetRound();
   SetdeltaTimeAIServBall();
   SetRandomServePosition();
   SetGameSpeed();
//   m_GameSpeed = 1.0f;
   if( !((m_P1Score+m_P2Score)%m_ServeChangeScore) && (m_P1Score+m_P2Score) != 0 )
   {
      if( m_bNetworkActive )
      {
         if( (m_bisServer && m_bServerServes) || (!m_bisServer && !m_bServerServes) )
         {
            CopyGameStatusForSending();
            m_SendStream.AddPacket( (char*)&m_GameStatus, sizeof(m_GameStatus), packet::PACKET_FLAG_STATUS );
            m_bPositionChanged = false;
            m_SendStream.AddPacket( NULL, 0, packet::PACKET_FLAG_TOGGLE_SERVES );
            ToggleTurns();
         }
      }
      else
      {
         ToggleTurns();
      }
   }
   ResetBallAndPaddleVectors();

   if( m_bNetworkActive )
   {
      if( CheckForEndGameCondition() )
      {
         if( m_P1Score > m_P2Score )
         {
            m_p1Console.AddLine( m_P1Nick+" wins!" );            
         }
         else
         {
            m_p1Console.AddLine( m_P2Nick+" wins!" );
         }
         BuildP1ConsoleList();
         m_RoundNumber = 0;
         m_P1Score     = 0;
         m_P2Score     = 0;
         SetInitialObjectPositions();
         SetScore();
         SetRound();
      }
   }
   else
   {
      if( CheckForEndGameCondition() )
      {
         SetGameOverModeTrue();
         m_ChangeToDemoMode.Set( m_WinBase, CHANGE_TO_DEMO_MODE, 15*1000, NULL );
      }
   }

   if( m_bNetworkActive )
   {
      SendInterval.ResetTime();
      if( (m_bisServer && m_bServerServes) || (!m_bisServer && !m_bServerServes) )
      {
         CopyGameStatusForSending();
         m_SendStream.AddPacket( (char*)&m_GameStatus, sizeof(m_GameStatus), packet::PACKET_FLAG_STATUS );
         m_bPositionChanged = false;
      }
   }
}

void MainWindow::ResetGameState( void )
{
   StartNewRound();
   m_GameSpeed      = 1.0f;
   m_RoundNumber    = 1;
   m_bP1Turn        = true;
   m_bP1Computer    = false;
   m_bP2Computer    = false;
   if( m_GameMode[ GAMEMODE ] )
   {
      if( m_bP1Turn )
      {
         m_bP1Computer = false;
         if( m_MultiplayerOptions[1] ) // m_MultiplayerOptions[1] = 2 player mode
            m_bP2Computer = false;
         else
            m_bP2Computer = true;         
      }
      else 
      {
         m_bP2Computer = false;
         if( m_MultiplayerOptions[1] )
            m_bP1Computer = false;
         else
            m_bP1Computer = true;
      }

      if( m_bP1MoveWithKeyPad )      // Only one person can use the mouse at a time
         m_bP2MoveWithKeyPad = false;
      else
         m_bP2MoveWithKeyPad = true;
   }
   else if( m_GameMode[ DEMOMODE ] )
   {
      m_bP1Computer = true;
      m_bP2Computer = true;
   }
   m_GameMode[ GAMEOVERMODE ]   = false;
   m_P1Score         = 0;
   m_P2Score         = 0;
   SetScore();
   SetRound();
}

void MainWindow::ResetBallAndPaddleVectors( void )
{
   if( m_bP1Turn )
   {
      m_Ball.m_VelocityVector        = Vector2D( initialball_x_velocity,         initialball_y_velocity         );
      m_Ball.m_AccelerationVector    = Vector2D( initialball_x_acceleration,     initialball_y_acceleration     );
      m_Ball.m_AngularPositionVector = Vector2D( initialball_x_angular_position, initialball_y_angular_position );
      m_Ball.m_AngularVelocityVector = Vector2D( initialball_x_angular_velocity, initialball_y_angular_velocity );
   }
   else
   {
      m_Ball.m_VelocityVector        = Vector2D( -1.0*fabs( initialball_x_velocity),         -1.0*fabs( initialball_y_velocity)         );
      m_Ball.m_AccelerationVector    = Vector2D( -1.0*fabs( initialball_x_acceleration),     -1.0*fabs( initialball_y_acceleration)     );
      m_Ball.m_AngularPositionVector = Vector2D( -1.0*fabs( initialball_x_angular_position), -1.0*fabs( initialball_y_angular_position) );
      m_Ball.m_AngularVelocityVector = Vector2D( -1.0*fabs( initialball_x_angular_velocity), -1.0*fabs( initialball_y_angular_velocity) );
   }

   m_Ball.m_AngularAccelerationVector = Vector2D( initialball_x_angular_acceleration, initialball_y_angular_acceleration );
   m_Paddle1.m_VelocityVector.m_x = initialpaddle_x_velocity;
   m_Paddle1.m_VelocityVector.m_y = initialpaddle_y_velocity;
   m_Paddle2.m_VelocityVector.m_x = initialpaddle_x_velocity;
   m_Paddle2.m_VelocityVector.m_y = initialpaddle_y_velocity;

//   m_Paddle1.m_PositionVector.m_x = 0.0f; m_PlayingAreaRecy.left
   m_Paddle1.m_PositionVector.m_y = 0.0f;
//   m_Paddle2.m_PositionVector.m_x = 0.0f; m_PlayingAreaRecy.right
   m_Paddle2.m_PositionVector.m_y = 0.0f;
}

void MainWindow::SetRandomError( void )
{
   if( m_Difficulty == Impossible )
   {
      m_RandomError = 0;
   }
   else
   {
      double error = 1.0f;
      if( (fabsf(m_Ball.m_VelocityVector.m_x) > 0.012) || (fabsf(m_Ball.m_VelocityVector.m_y) > 0.022) )
         error = 3.0f + m_Difficulty;
      else if( (fabsf(m_Ball.m_VelocityVector.m_x) > 0.014) || (fabsf(m_Ball.m_VelocityVector.m_y) > 0.024) )
         error = 3.5f + m_Difficulty;
      else if( (fabsf(m_Ball.m_VelocityVector.m_x) > 0.016) || (fabsf(m_Ball.m_VelocityVector.m_y) > 0.026) )
         error = 4.0f + m_Difficulty;
   
      m_RandomError = (rand()%((long)error));
      m_RandomError *= (rand()%2) ? -1.0f : 1.0f;
   }
}

Vector3D MainWindow::WindowToGLCoordinates( int x, int y )
{
   ::glLoadIdentity();
   GLint    viewport[4];
   GLdouble modelview[16];
   GLdouble projection[16];
   GLfloat  winX, winY, winZ;
   GLdouble posX, posY, posZ;

   ::glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
   ::glGetDoublev( GL_PROJECTION_MATRIX, projection );
   ::glGetIntegerv( GL_VIEWPORT, viewport );

   winX = (float)x;
   winY = (float)viewport[3] - (float)y;
   ::glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

   ::gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

   return Vector3D( posX, posY, posZ );
}

Vector2D MainWindow::GLToWindowCoordinates( double x, double y, double z )
{
   GLdouble *model = NULL, *proj = NULL, *win[3];
   GLint    *view  = NULL;
   ::glGetDoublev(  GL_MATRIX_MODE,       model );
   ::glGetDoublev(  GL_PROJECTION_MATRIX, proj  );
   ::glGetIntegerv( GL_VIEWPORT,          view  );

   ::gluProject( (GLdouble)x, (GLdouble)y, z, model, proj, view, win[0], win[1], win[2] );
   return Vector2D( (double)*(win[0]), (double)*(win[1]) );
}

bool MainWindow::HandleCollisions( void )
{
   if( (m_Ball.m_PositionVector.m_y <= m_PlayingAreaRect.bottom-m_Ball.m_Radius) )//-25 //(m_PlayingAreaRect.bottom-m_Ball.m_Radius)
   {
      ReverseBall_y_Direction();
      m_Ball.m_PositionVector.m_y = m_PlayingAreaRect.bottom+m_Ball.m_Radius;//-24 // (m_PlayingAreaRect.bottom+m_Ball.m_Radius)
      PlayEffect( BOUNCE );
   }
   else if( (m_Ball.m_PositionVector.m_y >= m_PlayingAreaRect.top+m_Ball.m_Radius) )//26
   {
      ReverseBall_y_Direction();
      m_Ball.m_PositionVector.m_y = m_PlayingAreaRect.top-m_Ball.m_Radius;//25 //(m_PlayingAreaRect.bottom-m_Ball.m_Radius)
      PlayEffect( BOUNCE );
   }

   if( (m_Paddle1.m_PositionVector.m_x >= m_Ball.m_PositionVector.m_x) )
   {
      if( CheckColideWithPaddle1() )
      {
         ApplyForceToBallFromPaddle1();
         ReverseBall_x_Direction();
         m_Ball.m_PositionVector.m_x = m_Paddle1.m_PositionVector.m_x+(m_Ball.m_Radius+m_Paddle1.m_TexQuad.halfwidth());
         SetRandomError();
         PlayEffect( BOUNCE );
         return true;
      }
      if( m_bP2Computer )
         ::PostMessage( m_WinBase, WM_TAUNT, WONROUND, 0 );
      PlayEffect( BOING );
      if( m_bNetworkActive )
      {
         if( (m_bisServer && m_bServerServes) || (!m_bisServer && !m_bServerServes) )
         {
            m_P2Score++;
            StartNewRound();
         }
      }
      else
      {
         m_P2Score++;
         StartNewRound();
      }
   }
   else if( (m_Paddle2.m_PositionVector.m_x <= m_Ball.m_PositionVector.m_x) )
   {
      if( CheckColideWithPaddle2() )
      {
         if( m_bP2Computer )
            ::PostMessage( m_WinBase, WM_TAUNT, INGAME, 0 );
         ApplyForceToBallFromPaddle2();
         ReverseBall_x_Direction();
         m_Ball.m_PositionVector.m_x = m_Paddle2.m_PositionVector.m_x-(m_Ball.m_Radius+m_Paddle1.m_TexQuad.halfwidth());
         SetRandomError();
         PlayEffect( BOUNCE );
         return true;
      }
      if( m_bP2Computer )
         ::PostMessage( m_WinBase, WM_TAUNT, LOSTROUND, 0 );
      PlayEffect( BOING );
      if( m_bNetworkActive )
      {
         if( (m_bisServer && m_bServerServes) || (!m_bisServer && !m_bServerServes) )
         {
            m_P1Score++;
            StartNewRound();
         }
      }
      else
      {
         m_P1Score++;
         StartNewRound();
      }
   }
   return false;
}

void MainWindow::DoConnectKeyActions(  WPARAM wparam, LPARAM lparam )
{
   if( wparam == VK_ESCAPE )
   {
      ResetGameState();
      SetDemoModeTrue();
   }

   if( wparam == VK_RETURN )
   {
      if( m_bisServer )
      {
         ::PostMessage( m_WinBase, WM_STARTSERVER, 0, 0 );
      }
      else
      {
         SetDrawP1Console( true );
         m_bP2DrawConsole = false;
         m_p1Console.AddLine( "Connecting To Master Server..." );
         m_bIgnoreReturn  = true; // stop it from connceting before the use presses enter again
         m_bCaptureHostIP = true;

         if( GetGamesFromMasterServer() )
         {
            m_p1Console.AddLine( "Click the Games List To the Right" );
            m_p1Console.AddLine( "Press Enter to accept: " );
            m_p1Console.SetCurrentLine( m_RemoteHostAddress );
         }
         else   // Manually get the ip frrst check from clipboard then from config file
         {
            m_p1Console.AddLine( "Enter Host IP address : " );

            std::string temp = GetTextFromClipBoard( 0 );
            if( ValidateIP( temp ) ) // ip from clipboard
            {
               m_RemoteHostAddress = temp;
               m_p1Console.SetCurrentLine( m_RemoteHostAddress );
            }
            else if( ValidateIP( m_LastIP ) ) // ip in config file
            {
               m_RemoteHostAddress = m_LastIP;
               m_p1Console.SetCurrentLine( m_RemoteHostAddress );
            }
         }
         BuildP1ConsoleList();
      }
   }

   if( wparam == VK_UP )
   {
      m_ConnectModeListBox.MoveCursorOnUpKey();
      ToggleIsServer();
   }

   if( wparam == VK_DOWN )
   {
      m_ConnectModeListBox.MoveCursorOnDownKey();
      ToggleIsServer();
   }
}

void MainWindow::DoOptionsKeyActions(  WPARAM wparam, LPARAM lparam )
{
   if( wparam == VK_ESCAPE )
   {
      if( !m_bShowOptionsSubMenu ) // if you didnt press enter
      {
         SetGameModeTrue();
      }
      else
      {
         m_bDoOptionAction     = false;
         m_bShowOptionsSubMenu = false;
      }
   }

   if( wparam == VK_RETURN )
   {
      if( !m_bShowOptionsSubMenu )
      {
         m_bDoOptionAction     = true;
         m_bShowOptionsSubMenu = true;
      }
      else
      {
         m_bShowOptionsSubMenu = false;
         if( m_GameOptions[MULTIPLAYER] && m_MultiplayerOptions[2] ) // Two Player Online
         {
            SetConnectModeTrue();
         }
         else
         {
            SetGameModeTrue();
            PlayEffect( NEWGAME );
         }
         if( m_GameOptions[DIFFICULTY] )
         {
            if( m_DifficultyOptions[ Impossible ] )
               m_Difficulty = Impossible;
            else if( m_DifficultyOptions[ Hard ] )
               m_Difficulty = Hard;
            else if( m_DifficultyOptions[ Normal ] )
               m_Difficulty = Normal;
            else if( m_DifficultyOptions[ Easy ] )
               m_Difficulty = Easy;
         }
      }
   }

   if( wparam == VK_F4 )
   {
      ToggleSound();
   }

   if( wparam == VK_RIGHT )
   {
//      if( m_GameOptions[GAMEKEYS] )
//      {
//         if( m_GameKeyOptions[0] )
//            ToggleP1MoveWithKeyPad();
//         else
//            ToggleP2MoveWithKeyPad();
//      }
   }
   else if( wparam == VK_LEFT )
   {
//      if( m_GameOptions[GAMEKEYS] )
//      {
//         if( m_GameKeyOptions[0] )
//            ToggleP1MoveWithKeyPad();
//         else
//            ToggleP2MoveWithKeyPad();
//      }
   }

   if( wparam == VK_DOWN )
   {
      if( m_bShowOptionsSubMenu )
      {
         if( m_GameOptions[DIFFICULTY] )
         {
            ++m_DifficultyOptions;
            m_DifficultyOptions.SetOnlyTrueAtCursor();
            SetDifficultySStream();
            m_DifficultyOptionsListBox.MoveCursorOnDownKey();
         }
         else if( m_GameOptions[MULTIPLAYER] )
         {
            ++m_MultiplayerOptions;
            m_MultiplayerOptions.SetOnlyTrueAtCursor();
            m_MultiplayerOptionsListBox.MoveCursorOnDownKey();
         }
         else if( m_GameOptions[GAMEKEYS] )
         {
            ++m_GameKeyOptions;
            m_GameKeyOptions.SetOnlyTrueAtCursor();
            m_GameKeysListBox.MoveCursorOnDownKey();

            if( m_GameKeyOptions[0] )
            {
               m_bP1MoveWithKeyPad = true;
               m_bP2MoveWithKeyPad = false;
            }
            else
            {
               m_bP1MoveWithKeyPad = false;
               m_bP2MoveWithKeyPad = true;
            }
         }
         else if( m_GameOptions[ DETAIL ] )
         {
            m_DetailOptionsListBox.MoveCursorOnDownKey();
         }
         else if( m_GameOptions[ VSYNC ] )
         {
            m_VSyncListBox.MoveCursorOnDownKey();
            if( m_VSyncListBox.GetSelected() )
            {
               SetVSync( 0 );
            }
            else
            {
               SetVSync( 1 );
            }
         }
         else if( m_GameOptions[ SOUND ] )
         {
            m_SoundListBox.MoveCursorOnDownKey();
            if( !m_SoundListBox.GetSelected() )
            {
               if( !m_bSoundOn )
               {
                  ToggleSound();
               }
            }
            else
            {
               if( m_bSoundOn )
               {
                  ToggleSound();
               }
            }
         }
      }
      else //if( !m_bShowOptionsSubMenu )
      {
         ++m_GameOptions;
         m_GameOptions.SetOnlyTrueAtCursor();
         m_GameOptionsListBox.MoveCursorOnDownKey();
      }
      PlayEffect( OPTION );
   }
   else if( wparam == VK_UP )
   {
      if( m_bShowOptionsSubMenu )
      {
         if( m_GameOptions[DIFFICULTY] )
         {
            --m_DifficultyOptions;
            m_DifficultyOptions.SetOnlyTrueAtCursor();
            SetDifficultySStream();
            m_DifficultyOptionsListBox.MoveCursorOnUpKey();
         }
         else if( m_GameOptions[MULTIPLAYER] )
         {
            --m_MultiplayerOptions;
            m_MultiplayerOptions.SetOnlyTrueAtCursor();
            m_MultiplayerOptionsListBox.MoveCursorOnUpKey();
         }
         else if( m_GameOptions[GAMEKEYS] )
         {
            --m_GameKeyOptions;
            m_GameKeyOptions.SetOnlyTrueAtCursor();
            m_GameKeysListBox.MoveCursorOnUpKey();
            if( m_GameKeyOptions[0] )
            {
               m_bP1MoveWithKeyPad = true;
               m_bP2MoveWithKeyPad = false;
            }
            else
            {
               m_bP1MoveWithKeyPad = false;
               m_bP2MoveWithKeyPad = true;
            }
         }
         else if( m_GameOptions[ DETAIL ] )
         {
            m_DetailOptionsListBox.MoveCursorOnUpKey();
         }
         else if( m_GameOptions[ VSYNC ] )
         {
            m_VSyncListBox.MoveCursorOnUpKey();
            if( m_VSyncListBox.GetSelected() )
            {
               SetVSync( 0 );
            }
            else
            {
               SetVSync( 0 );
            }
         }
         else if( m_GameOptions[ SOUND ] )
         {
            m_SoundListBox.MoveCursorOnUpKey();
            if( !m_SoundListBox.GetSelected() )
            {
               if( !m_bSoundOn )
               {
                  ToggleSound();
               }
            }
            else
            {
               if( m_bSoundOn )
               {
                  ToggleSound();
               }
            }
         }
      }
      else//( !m_bShowOptionsSubMenu )
      {
         --m_GameOptions;
         m_GameOptions.SetOnlyTrueAtCursor();
         m_GameOptionsListBox.MoveCursorOnUpKey();
      }
      PlayEffect( OPTION );
   }
}

void MainWindow::DoGameOverModeKeyActions( WPARAM wparam, LPARAM lparam )
{
   if( wparam == VK_RETURN )
   {
      ResetGameState();
      SetDemoModeTrue();
   }
}

void MainWindow::DoTitleKeyActions( WPARAM wparam, LPARAM lparam )
{
   if( wparam == VK_RETURN )
   {
      SetDemoModeTrue();
      ResetGameState();
   }
   if( wparam == VK_F4 )
   {
      ToggleSound();
   }
}

void MainWindow::DoDemoKeyActions( WPARAM wparam, LPARAM lparam )
{
   if( wparam == VK_F4 )
   {
      ToggleSound();
   }
   if( wparam == VK_RETURN ) // end the demo loop
   {
      if( !m_bOptionsModeWasVisited )
      {
         ResetGameState();
         SetOptionsModeTrue();
      }
      else
      {
         ResetGameState();
         SetGameModeTrue();
         StartNewGame();
      }
   }
}

void MainWindow::DoGameKeyActions( WPARAM wparam, LPARAM lparam )
{
   if( !m_bGamePaused )
   {
      if( wparam == VK_RIGHT ) // Set the served flag to true which will tell the AI to serve the ball for player 1
      {
         if( m_bP1Turn && !m_bBallServed && !m_bP1Computer && m_bP1MoveWithKeyPad )
         {
            if( m_bNetworkActive )
            {
               if( (m_bisServer && m_bServerServes) || (!m_bisServer && !m_bServerServes) )
               {
                  m_bBallServed     = true;
                  CopyGameStatusForSending();
                  m_SendStream.AddPacket( (char*)&m_GameStatus, sizeof(S_GameStatus), packet::PACKET_FLAG_STATUS );
                  SendData();
                  SendInterval.Begin();
                  m_deltaTimeBallServed.Begin();
               }
            }
            else
            {
               m_bBallServed     = true;
               m_deltaTimeBallServed.Begin();
            }
         }
      }
      if( wparam == VK_LEFT ) // Set the served flag to true which will tell the AI to serve the ball for player 2
      {
         if( !m_bP1Turn && !m_bBallServed && !m_bP2Computer && m_bP2MoveWithKeyPad )
         {
            m_bBallServed     = true;
            m_deltaTimeBallServed.Begin();
         }  
      }
   }
   if( wparam == VK_F4 )
   {
      ToggleSound();
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
//   if( wparam == VK_ESCAPE )
//   {
//      PostMessage( m_WinBase, WM_QUIT, 0, 0 );
//   }

   if( GetAsyncKeyState( VK_TAB ) )
   {
      m_bGamePaused = true;
      PushGameState();
      ResetGameState();
      SetOptionsModeTrue();
      m_bGamePaused = true;
   }

   if( GetAsyncKeyState( VK_LSHIFT ) )
   {
      m_GameSpeed -= 0.02f;
      SetGameSpeed();
      if( m_bNetworkActive )
      {
         if( (m_bisServer && m_bServerServes) || (!m_bisServer && !m_bServerServes) )
         {
            m_SendStream.AddPacket( (char*)&m_GameSpeed, sizeof(m_GameSpeed), packet::PACKET_FLAG_GAME_SPEED );
         }
      }
   }
   else if( GetAsyncKeyState( VK_RSHIFT ) )
   {
      m_GameSpeed += 0.02f;
      SetGameSpeed();
      if( m_bNetworkActive )
      {
         if( (m_bisServer && m_bServerServes) || (!m_bisServer && !m_bServerServes) )
         {
            m_SendStream.AddPacket( (char*)&m_GameSpeed, sizeof(m_GameSpeed), packet::PACKET_FLAG_GAME_SPEED );
         }
      }
   }
}

void MainWindow::AIServeBall( void )
{
   if( m_bP1Turn && m_bP1Computer )
      MovePaddle1_y_To( m_RandomServePosition );
   else if ( !m_bP1Turn && m_bP2Computer )
      MovePaddle2_y_To( m_RandomServePosition );

   if( (m_deltaTimeAIServBall <= 0) )
   {
         if( !m_bBallServed )
         {
            if( m_bP1Turn && m_bP1Computer && IsInRange( m_Paddle1.m_PositionVector.m_y, m_RandomServePosition, 4.0 ) )
            {
               m_bBallServed = true;
               m_deltaTimeBallServed.Begin();
            }
            else if( !m_bP1Turn && m_bP2Computer && IsInRange( m_Paddle2.m_PositionVector.m_y, m_RandomServePosition, 4.0 ) )
            {
               m_bBallServed = true;
               m_deltaTimeBallServed.Begin();
            }
         }
   }
   else
   {
      m_deltaTimeAIServBall -= m_deltaGameTime;
   }
}

void MainWindow::MovePaddle1WithMouse( void )
{
   POINT mouse_pos;
   ::GetCursorPos( &mouse_pos );
   ::ScreenToClient( m_WinBase, &mouse_pos );
   
   Vector3D glmouse_pos = WindowToGLCoordinates( mouse_pos.x, mouse_pos.y );

   if( !IsInRange( (glmouse_pos[1]*2.0), m_Paddle1.m_PositionVector.m_y, m_Paddle1.m_VelocityVector.m_y * m_deltaGameTime  ) )
   {
      if( ( (glmouse_pos[1]*2.0) > m_Paddle1.m_PositionVector.m_y) )
      {
         if( m_Paddle1.m_VelocityVector.m_y < 0 ) m_Paddle1.m_VelocityVector.m_y *= -1.0f;
         m_Paddle1.m_PositionVector += m_Paddle1.m_VelocityVector*m_deltaGameTime;
      }
      else if( ( (glmouse_pos[1]*2.0) < m_Paddle1.m_PositionVector.m_y) )
      {
         if( m_Paddle1.m_VelocityVector.m_y > 0 ) m_Paddle1.m_VelocityVector.m_y *= -1.0f;
         m_Paddle1.m_PositionVector += m_Paddle1.m_VelocityVector*m_deltaGameTime;
      }
   }
}

void MainWindow::MovePaddle2WithMouse( void )
{
   POINT mouse_pos;
   ::GetCursorPos( &mouse_pos );
   ::ScreenToClient( m_WinBase, &mouse_pos );

   Vector3D glmouse_pos = WindowToGLCoordinates( mouse_pos.x, mouse_pos.y );

   if( !IsInRange( (glmouse_pos[1]*2.0), m_Paddle2.m_PositionVector.m_y, m_Paddle2.m_VelocityVector.m_y * m_deltaGameTime  ) )
   {
      if( ( (glmouse_pos[1]*2.0) > m_Paddle2.m_PositionVector.m_y) )
      {
         if( m_Paddle2.m_VelocityVector.m_y < 0 ) m_Paddle2.m_VelocityVector.m_y *= -1.0f;
         m_Paddle2.m_PositionVector += m_Paddle2.m_VelocityVector*m_deltaGameTime;
      }
      else if( ( (glmouse_pos[1]*2.0) < m_Paddle2.m_PositionVector.m_y) )
      {
         if( m_Paddle2.m_VelocityVector.m_y > 0 ) m_Paddle2.m_VelocityVector.m_y *= -1.0f;
         m_Paddle2.m_PositionVector += m_Paddle2.m_VelocityVector*m_deltaGameTime;
      }
   }
}

void MainWindow::MovePaddle1WithKeyPad( void )
{
   if( m_Keys[VK_UP] )
   {
      if( m_Paddle1.m_VelocityVector.m_y < 0 ) m_Paddle1.m_VelocityVector.m_y *= -1.0f;
      m_Paddle1.m_PositionVector += m_Paddle1.m_VelocityVector*m_deltaGameTime;
   }
   if( m_Keys[VK_DOWN] )
   {
      if( m_Paddle1.m_VelocityVector.m_y > 0 ) m_Paddle1.m_VelocityVector.m_y *= -1.0f;
      m_Paddle1.m_PositionVector += m_Paddle1.m_VelocityVector*m_deltaGameTime;
   }
}

void MainWindow::MovePaddle2WithKeyPad( void )
{
   if( m_Keys[VK_UP] )
   {
      if( m_Paddle2.m_VelocityVector.m_y < 0 ) m_Paddle2.m_VelocityVector.m_y *= -1.0f;
      m_Paddle2.m_PositionVector += m_Paddle2.m_VelocityVector*m_deltaGameTime;
   }
   if( m_Keys[VK_DOWN] )
   {
      if( m_Paddle2.m_VelocityVector.m_y > 0 ) m_Paddle2.m_VelocityVector.m_y *= -1.0f;
      m_Paddle2.m_PositionVector += m_Paddle2.m_VelocityVector*m_deltaGameTime;
   }
}

void MainWindow::MoveBall( void )
{
   m_Ball.m_PositionVector += ( (m_Ball.m_VelocityVector*m_deltaGameTime) + (m_Ball.m_AccelerationVector*(m_deltaGameTimeSquared*0.5)) ); // u*t+0.5*a*t^2
   m_Ball.m_VelocityVector += (m_Ball.m_AccelerationVector*m_deltaGameTime);                                                              // v = u+at
   if( m_Ball.m_AngularPositionVector.m_x > 360 )
      m_Ball.m_AngularPositionVector.m_x = 0;
   m_Ball.m_AngularPositionVector += (m_Ball.m_AngularVelocityVector*m_deltaGameTime) + (m_Ball.m_AngularAccelerationVector*(m_deltaGameTimeSquared*0.5));
}

void MainWindow::FollowPaddleForServe( void )
{
   if( m_bP1Turn )
   {
      m_Ball.m_PositionVector.m_x = m_Paddle1.m_PositionVector.m_x+(m_Ball.m_Radius+m_Paddle1.m_TexQuad.halfwidth());
      m_Ball.m_PositionVector.m_y = m_Paddle1.m_PositionVector.m_y;
      if( (m_Paddle1.m_VelocityVector.m_y < 0) && (m_Ball.m_VelocityVector.m_y > 0) ) // Make the velocity negative if the paddle is moving down
         m_Ball.m_VelocityVector.m_y *= -1.0f;
      else if( (m_Paddle1.m_VelocityVector.m_y > 0) && (m_Ball.m_VelocityVector.m_y < 0) ) 
         m_Ball.m_VelocityVector.m_y *= -1.0f;
   }
   else
   {
      m_Ball.m_PositionVector.m_x = m_Paddle2.m_PositionVector.m_x-(m_Ball.m_Radius+m_Paddle1.m_TexQuad.halfwidth());
      m_Ball.m_PositionVector.m_y = m_Paddle2.m_PositionVector.m_y;
      if( (m_Paddle2.m_VelocityVector.m_y < 0) && (m_Ball.m_VelocityVector.m_y > 0) )  // Make the velocity negative if the paddle is moving down
         m_Ball.m_VelocityVector.m_y *= -1.0f;
      else if( (m_Paddle2.m_VelocityVector.m_y > 0) && (m_Ball.m_VelocityVector.m_y < 0) ) 
         m_Ball.m_VelocityVector.m_y *= -1.0f;
   }
}

void MainWindow::ApplyForceToBallFromPaddle1( void )
{
//   m_Ball.ApplyForce( Vector2D( 0, m_Paddle1.m_VelocityVector.m_y*0.0000001*m_deltaGameTime ), Vector2D() ) ;
}

void MainWindow::ApplyForceToBallFromPaddle2( void )
{
//   m_Ball.ApplyForce( Vector2D( 0, m_Paddle2.m_VelocityVector.m_y*0.0000001*m_deltaGameTime ), Vector2D() ) ;
}

void MainWindow::UpdateObjects( void )
{
   if( !m_bGamePaused && (m_GameMode[ DEMOMODE ] || m_GameMode[ GAMEMODE ]) )
   {
      if( m_bBallServed )
      {
         if( m_bNetworkActive )
         {
            if( (m_bisServer && m_bServerServes) || (!m_bisServer && !m_bServerServes) )
            {
               MoveBall();
               if( m_bP1Computer )
                  InterceptBallWithPaddle1();
               if( m_bP2Computer )
                  InterceptBallWithPaddle2();
            }
         }
         else
         {
            MoveBall();
            if( m_bP1Computer )
               InterceptBallWithPaddle1();
            if( m_bP2Computer )
               InterceptBallWithPaddle2();
         }
      }
      else
      {
         FollowPaddleForServe();
         AIServeBall();
         m_deltaTimeBallServed.ResetTime();
      }
      if( !m_bP1Computer )
      {
         if( m_bP1MoveWithKeyPad )
            MovePaddle1WithKeyPad();
         else
            MovePaddle1WithMouse();
      }
      if( !m_bP2Computer )
      {
         if( !m_bNetworkActive )
         {
            if( m_bP2MoveWithKeyPad )
               MovePaddle2WithKeyPad();
            else
               MovePaddle2WithMouse();
         }
      }
   }
   RestrictPaddleToGameBoundaries( m_Paddle1 );
   RestrictPaddleToGameBoundaries( m_Paddle2 );
   CopyPaddlePositionForSending();
   if( (m_bisServer && m_bServerServes) || (!m_bisServer && !m_bServerServes) )
   {
      CopyBallPositionForSending();
   }
}

void MainWindow::DrawTitle( void )
{
//   if( m_TileRotateAngle > 360 )
//       m_TileRotateAngle = 0;
//   m_TileRotateAngle += 0.01*m_deltaGameTime;
//   ::glRotatef( m_TileRotateAngle, 0.0f, 1.0f, 0.0 );

   m_DisplayListManager.call( GL_LIST_TITLE_STR1 );
   if( m_Flash.Update() )
      m_DisplayListManager.call( GL_LIST_TITLE_STR2 );
}

void MainWindow::DrawGame( void )
{
   ::glBlendFunc( GL_ONE, GL_ZERO ); // Set Blending Mode (Cheap / Quick)
   ::glEnable( GL_BLEND );
   ::glEnable( GL_TEXTURE_2D );

   m_DisplayListManager.call( GL_LIST_TEXTURECOLOR );

   ::glLoadIdentity();
   m_DisplayListManager.call( GL_LIST_BG );

   ::glLoadIdentity();
   ::glTranslatef( m_Ball.m_PositionVector.m_x, m_Ball.m_PositionVector.m_y, m_Ball.m_Z );
   m_Ball.OGLDraw();
   m_DisplayListManager.call( GL_LIST_BALL  );

   ::glLoadIdentity();
   ::glTranslatef( m_Ball.m_PositionVector.m_x, m_Ball.m_PositionVector.m_y, m_Ball.m_Z );

// Trail
   if( m_Ball.m_VelocityVector.m_x > 0 )
      ::glTranslatef( -m_Ball.Diameter(), 0, 0 );
   else
      ::glTranslatef( m_Ball.Diameter(), 0, 0 );
   if( m_Ball.m_VelocityVector.m_y > 0 )
      ::glTranslatef( 0, -m_Ball.Diameter()*2.0, 0 );
   else
      ::glTranslatef( 0, m_Ball.Diameter()*2.0, 0 );

   ::glRotatef( ::atan2( m_Ball.m_VelocityVector.m_y, m_Ball.m_VelocityVector.m_x) * 180.0 * M_1_PI, 0, 0, 1.0 );
   m_DisplayListManager.call( GL_LIST_TRAIL );
// end Trail

   ::glLoadIdentity();
   ::glTranslatef( m_Paddle1.m_PositionVector.m_x, m_Paddle1.m_PositionVector.m_y, m_Paddle1.m_Z );
   m_DisplayListManager.call( GL_LIST_PADDLE1 );

   ::glLoadIdentity();
   ::glTranslatef( m_Paddle2.m_PositionVector.m_x, m_Paddle2.m_PositionVector.m_y, m_Paddle2.m_Z );
   m_DisplayListManager.call( GL_LIST_PADDLE2 );

   ::glDisable( GL_TEXTURE_2D );

   m_DisplayListManager.call( GL_LIST_TEXTCOLOR );
   ::glDisable( GL_BLEND );

   if( m_bGamePaused )
      m_Font.Print( "Game Paused!", -3, 0.5f, m_Z );

   DrawGameSpeed();
   DrawGameDifficulty();
}

void MainWindow::DrawGameSpeed( void )
{
   m_DisplayListManager.call( GL_LIST_GAME_SPEED );
}

void MainWindow::BuildGameSpeedList( void )
{
   m_DisplayListManager.rebuild( GL_LIST_GAME_SPEED );
   m_Font.Print( m_GameSpeedSStream.str(), -15.0f, -13.5f, m_Z );
   m_DisplayListManager.endrebuild();
}

void MainWindow::DrawGameDifficulty( void )
{
   m_DisplayListManager.call( GL_LIST_GAME_DIFFICULTY );
}

void MainWindow::BuildGameDifficultyList( void )
{
   m_DisplayListManager.rebuild( GL_LIST_GAME_DIFFICULTY );
   m_Font.Print( m_DifficultySStream.str(),  5.0f, -13.5f, m_Z );
   m_DisplayListManager.endrebuild();
}

void MainWindow::DrawDemo( void )
{
   DrawGame();
   m_DisplayListManager.call( GL_LIST_DEMO_STR1 );

   if( m_Flash.Update() )
      m_DisplayListManager.call( GL_LIST_DEMO_STR2 );

   m_Font.Print( m_HelpSideScrollingText.ScrollLeft(), -22, -17.5, m_Z );
}

void MainWindow::DrawGameOverMode( void )
{
   m_Font.Print( "Game Over", -10, 10, m_Z );

   if( m_P1Score > m_P2Score )
   {
      m_Font.Print( "Player 1 Wins", -10, 8, m_Z );
      m_Font.Print( "Player 2 You Got Owned", -10, 6, m_Z );   }
   else
   {
      m_Font.Print( "Player 2 Wins", -10, 8, m_Z );
      m_Font.Print( "Player 1 You Got Owned", -10, 6, m_Z );
   }

   if( m_Flash.Update() )
      m_Font.Print( "Press Enter/Return To Continue", -10, 4, m_Z );
}

void MainWindow::DrawOptionsCursor( void )
{
   if( m_GameOptions[ HELP ] )
   {
      ::glTranslatef( -11, 10, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
   if( m_GameOptions[ DIFFICULTY ] )
   {
      ::glTranslatef( -11,  8, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
   if( m_GameOptions[ GREETZ ] )
   {
      ::glTranslatef( -11,  6, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
   if( m_GameOptions[ MULTIPLAYER ] )
   {
      ::glTranslatef( -11,  4, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
   if( m_GameOptions[ GAMEKEYS ] )
   {
      ::glTranslatef( -11,  2, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
   if( m_GameOptions[ DETAIL ] )
   {
      ::glTranslatef( -11,  0, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
}

void MainWindow::DrawOptions( void )
{
   m_Font.Print( "Help",                    -10, 10, m_Z );
   m_Font.Print( "Difficulty",              -10,  8, m_Z );
   m_Font.Print( "Greetz",                  -10,  6, m_Z );
   m_Font.Print( "Mulitiplayer And Online", -10,  4, m_Z );
   m_Font.Print( "Game Keys",               -10,  2, m_Z );
   m_Font.Print( "Game Detail",             -10,  0, m_Z );
   m_Font.Print( "Press Enter/Return To Select", -5, -2, m_Z );
   m_Font.Print( "Press Escape to exit Options", -5, -4, m_Z );
}

void MainWindow::DrawServeInstructions( void )
{
   if( m_bP1Turn )
   {
      if( m_bP1MoveWithKeyPad )
      {
         m_Font.Print( "Press Right To Serve", -15, 0, m_Z );
         if( m_bNetworkActive )
         {
            m_Font.Print( "Rember to move the chat box (~) to serv", -15, -2, m_Z );
            m_Font.Print( "First to 5 wins!", -15, -4, m_Z );
         }
      }
      else
      {
         m_Font.Print( "Left Click To Serve",  -15, 0, m_Z );
      }
   }
   else
   {
      if( m_bP2MoveWithKeyPad )
      {
         m_Font.Print( "Press Left To Serve", 0, 0, m_Z );
      }
      else
      {
         m_Font.Print( "Left Click To Serve", 0, 0, m_Z );
      }
   }
}

void MainWindow::DrawScore( void )
{
   m_DisplayListManager.call( GL_LIST_OPTIONS_SCORE );
}

void MainWindow::BuildScoreList( void )
{
   m_DisplayListManager.rebuild( GL_LIST_OPTIONS_SCORE );
   m_Font.Print( m_P1ScoreSStream.str(), -15, 13, m_Z );
   m_Font.Print( m_P2ScoreSStream.str(),  +5, 13, m_Z );
   m_DisplayListManager.endrebuild();
}

void MainWindow::DrawRound( void )
{
   m_DisplayListManager.call( GL_LIST_OPTIONS_ROUND );
}

void MainWindow::BuildRoundList( void )
{
   m_DisplayListManager.rebuild( GL_LIST_OPTIONS_ROUND );
   m_Font.Print( m_RoundSStream.str(), -2, 13, m_Z );
   m_DisplayListManager.endrebuild();
}

void MainWindow::DrawGreetz( void )
{
   m_Font.Print( "Programmed by try_catch_this look me up in AfterNet #gamedev", -10, 5, m_Z );
   m_Font.Print( "Thanks to Keyboard help with vectors and magnus forces",       -10, 4, m_Z );
   m_Font.Print( "Thanks to Hoda     encouraged me to make a GUI",               -10, 3, m_Z );
   m_Font.Print( "Thanks to Washu    #gamdev Network guru",                      -10, 2, m_Z );
   m_Font.Print( "The    www.gamedev.net community",                             -10, 1, m_Z );
}

void MainWindow::DrawHelp( void )
{
   m_Font.Print( "F12 to toggle fullscreen",                      -10, 12, m_Z );
   m_Font.Print( "F4  to toggle sound",                           -10, 11, m_Z );
   m_Font.Print( "Move paddles with UP and DOWN keys",            -10, 10, m_Z );
   m_Font.Print( "Move paddles with Mouse when selected",         -10,  9, m_Z );
   m_Font.Print( "` to turn on/off  player1 console",             -10,  8, m_Z );
   m_Font.Print( "1 to turn on/off player2 console",              -10,  7, m_Z );
   m_Font.Print( "/list or /help for a list of console commands", -10,  6, m_Z );
   m_Font.Print( "Enter to pause game",                           -10,  5, m_Z );
   m_Font.Print( "First to 5 points win",                         -10,  4, m_Z );
}

void MainWindow::DrawConnect( void )
{
   if( m_bisServer )
   {
      ::glTranslatef( -6, 2, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
   else
   {
      ::glTranslatef( -6, 0, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }

   m_DisplayListManager.call( GL_LIST_CONNECT );
}

void MainWindow::DrawDifficulty( void )
{
   m_DisplayListManager.call( GL_LIST_DIFFICULTY );

   if( m_DifficultyOptions[ Impossible ] )
   {
      ::glTranslatef( -6, 8, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
   else if( m_DifficultyOptions[ Hard ] )
   {
      ::glTranslatef( -6, 6, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
   else if( m_DifficultyOptions[ Normal ] )
   {
      ::glTranslatef( -6, 4, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
   else if( m_DifficultyOptions[ Easy ] )
   {
      ::glTranslatef( -6, 2, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
}

void MainWindow::DrawMultiPlayerSubOption( void )
{
   if( m_MultiplayerOptions[ 0 ] )
   {
      ::glTranslatef( -6, 6, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
   else if( m_MultiplayerOptions[ 1 ] )
   {
      ::glTranslatef( -6, 4, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
   else if( m_MultiplayerOptions[ 2 ] )
   {
      ::glTranslatef( -6, 2, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }

   m_DisplayListManager.call( GL_LIST_MULTIPLAYER );
}

void MainWindow::DrawGameKeySubOptions( void )
{
   if( m_GameKeyOptions[0] )
   {
      ::glTranslatef( -6, 10, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );
   }
   else
   {
      ::glTranslatef( -6, 8, m_Z );
      m_DisplayListManager.call( GL_LIST_OPTIONS_CURSOR );

   }

   if( m_bP1MoveWithKeyPad )
      m_DisplayListManager.call( GL_LIST_GAMEKEY_SUBOPTIONS_STR1 );
   else
      m_DisplayListManager.call( GL_LIST_GAMEKEY_SUBOPTIONS_STR2 );

   if( m_bP2MoveWithKeyPad )
      m_DisplayListManager.call( GL_LIST_GAMEKEY_SUBOPTIONS_STR3 );
   else
      m_DisplayListManager.call( GL_LIST_GAMEKEY_SUBOPTIONS_STR4 );
}

void MainWindow::DrawDetailSubOptions( void )
{
   m_DisplayListManager.call( GL_LIST_DETAIL );
}

void MainWindow::DrawP1Console( void )
{
   m_DisplayListManager.call( GL_LIST_P1CONSOLE );

   if( m_CursorFlash.Update() )
      m_DisplayListManager.call( GL_LIST_P1CONSOLE_CURSOR );
}

void MainWindow::BuildP1ConsoleList( void )
{
   m_DisplayListManager.rebuild( GL_LIST_P1CONSOLE );

   float y = m_P1ConsolePos.m_y;
   float x = m_P1ConsolePos.m_x;
//   float y = -5.0;
//   float x = -27.0;

   std::vector< std::string > &temp = m_p1Console;
   int size = m_p1Console.LinesAdded();
   for( int i=0; i <= size ; i++, y-- )
   {
      m_Font.Print( temp[i],  x,  y, m_Z );
   }

   m_Font.Print( ">", x-1, y+1, m_Z );
   m_DisplayListManager.endrebuild();

   m_DisplayListManager.rebuild( GL_LIST_P1CONSOLE_CURSOR );
   m_Font.Print( "|", x+m_Font.Length( m_p1Console.GetStringBehindCursor() ), y+1, m_Z );
   m_DisplayListManager.endrebuild();
}

void MainWindow::DrawP2Console( void )
{
   m_DisplayListManager.call( GL_LIST_P2CONSOLE );

   if( m_CursorFlash.Update() )
      m_DisplayListManager.call( GL_LIST_P2CONSOLE_CURSOR );
}

void MainWindow::BuildP2ConsoleList( void )
{
   m_DisplayListManager.rebuild( GL_LIST_P2CONSOLE );

   float y = m_P2ConsolePos.m_y;
   float x = m_P2ConsolePos.m_x;
//   float y = -5.0;
//   float x = 8.0;

   for( int i=0; i <= m_p2Console.LinesAdded() ; i++, y-- )
   {
      m_Font.Print( m_p2Console[i].c_str(), x,  y, m_Z );
   }
   m_Font.Print( ">", x-1, y+1, m_Z );
   m_DisplayListManager.endrebuild();

   m_DisplayListManager.rebuild( GL_LIST_P2CONSOLE_CURSOR );
   m_Font.Print( "|", x+m_Font.Length( m_p2Console.GetStringBehindCursor() ), y+1, m_Z );
   m_DisplayListManager.endrebuild();
}

void MainWindow::BuildP1NickList( void )
{
   m_DisplayListManager.rebuild( GL_LIST_PLAYER_1_NICK );
   m_Font.Print( m_P1Nick, -15, -15, m_Z );
   m_DisplayListManager.endrebuild();
}

void MainWindow::BuildP2NickList( void )
{
   m_DisplayListManager.rebuild( GL_LIST_PLAYER_2_NICK );
   m_Font.Print( m_P2Nick, 5, -15, m_Z );
   m_DisplayListManager.endrebuild();
}

void MainWindow::DrawPlayer2IP( void )
{
   m_DisplayListManager.call( GL_LIST_CLIENT_IP );
}

void MainWindow::BuildPlayer2IPList( void )
{
   m_DisplayListManager.rebuild( GL_LIST_CLIENT_IP );
   std::string temp = "@"+m_Player2IP;
   m_Font.Print( temp, 5, -16, m_Z );
   m_DisplayListManager.endrebuild();
}

void MainWindow::DrawPing( void )
{
   m_DisplayListManager.call( GL_LIST_PING );
}

void MainWindow::BuildPingList( void )
{
   m_DisplayListManager.rebuild( GL_LIST_PING );
   m_Font.Print( m_PingStringStream.str(), 5, -17, m_Z );
   m_DisplayListManager.endrebuild();
}

void MainWindow::DrawBytesSent( void )
{
   m_DisplayListManager.call( GL_LIST_BYTES_SENT );
}

void MainWindow::BuildBytesSentList( void )
{
   m_DisplayListManager.rebuild( GL_LIST_BYTES_SENT );
   m_Font.Print( m_BytesSentSStream.str(), 5, -18, m_Z );
   m_DisplayListManager.endrebuild();
}

void MainWindow::BuildBufferSizeList( void )
{
   m_DisplayListManager.rebuild( GL_LIST_BUFFERSIZE );
   m_Font.Print( m_SendBufferSizeSStream.str(), -15, -16, m_Z );
   m_Font.Print( m_RecvBufferSizeSStream.str(), -15, -17, m_Z );
   m_DisplayListManager.endrebuild();
}

void MainWindow::DrawMousePosition( void )
{
   m_Font.Print( m_MouseSStream.str(), m_GLMousePosition.m_x, m_GLMousePosition.m_y, m_Z );
}

void MainWindow::DrawCommandList( void )
{
   float z = -55;
   float x = -12;

   m_Font.Print( "/exit or /quit close program",                     x,  16, z );
   m_Font.Print( "/lamer cheat on",                                  x,  15, z );
   m_Font.Print( "/nolamer cheat off",                               x,  14, z );
   m_Font.Print( "/clear clear player 1 console",                    x,  13, z );
   m_Font.Print( "/clearall clear both player 1 and 2 console",      x,  12, z );
   m_Font.Print( "/connect connect to server",                       x,  11, z );
   m_Font.Print( "/listen start server",                             x,  10, z );
   m_Font.Print( "/ip emumerate host ip addresses",                  x,   9, z );
   m_Font.Print( "/port port you use to connect to",                 x,   8, z );
   m_Font.Print( "/lastsyserror last error reported by windows",     x,   7, z );
   m_Font.Print( "/wserror last winsock error",                      x,   6, z );
   m_Font.Print( "/taunt dunno",                                     x,   5, z );
   m_Font.Print( "/playeffect play effect by index",                 x,   4, z );
   m_Font.Print( "/path path to this program",                       x,   3, z );
   m_Font.Print( "/z zoom in and out",                               x,   2, z );
   m_Font.Print( "/ballpos show ball position",                      x,   1, z );
   m_Font.Print( "/paddle1pos show paddle1 position",                x,   0, z );
   m_Font.Print( "/paddle2pos show paddle2 position",                x,  -1, z );
   m_Font.Print( "/trackmouse follow mouse position",                x,  -2, z );
   m_Font.Print( "/list show this list of commands",                 x,  -3, z );
   m_Font.Print( "/help show this list of commands",                 x,  -4, z );
   m_Font.Print( "/ver get version information about this program",  x,  -5, z );
   m_Font.Print( "/fullscreen toggle fullscreen",                    x,  -6, z );
   m_Font.Print( "/playingarea get",                                 x,  -7, z );
   m_Font.Print( "/ballserved true if ballserved false otherwise",   x,  -8, z );
   m_Font.Print( "/testlist call a display list indicated by index", x,  -9, z );
   m_Font.Print( "/bbox calculate the bounzing box at z",            x, -10, z );
   m_Font.Print( "/vsync <number> change the value of vsync",        x, -11, z );
   m_Font.Print( "/nick  <newnick> change your nick",                x, -12, z );
}

void MainWindow::Draw( void )
{
   m_DisplayListManager.call( GL_LIST_CLEAR );

   if( m_bDrawCommandList )
   {
      m_DisplayListManager.call( GL_LIST_COMMANDLIST );
   }
   else
   {
      if( m_GameMode[ DEMOMODE ] )
      {
         DrawDemo();
         DrawRound();
         DrawScore();
      }
      else if( m_GameMode[ CONNECTMODE ] )
      {
//         DrawConnect();
         m_ConnectModeListBox.Draw( -12.0, 12.0, m_Z );
         
      }
      else if( m_GameMode[ TITLEMODE ] )
      {
         DrawTitle();
//         ::glEnable( GL_TEXTURE_2D );
//         m_OptionsButton.Draw();
//         ::glDisable( GL_TEXTURE_2D );
         m_DisplayListManager.call( GL_LIST_TEXTCOLOR );

//         Structs::fRect3D  Rect = m_OptionsButton.Rect();
//         std::ostringstream t;
//         t << "{ left= " << Rect.left << ", " << "top= " << Rect.top << ", right= " << Rect.right << ", bottom= " << Rect.bottom << ", z= " << Rect.m_z << " }";
//         m_Font.Print( t.str(), 0.0, -10.0, m_Z );
      }
      else if( m_GameMode[ GAMEMODE ] )
      {
         DrawGame();
         DrawRound();
         DrawScore();
         if( !m_bBallServed && !m_bGamePaused )
         {
            DrawServeInstructions();
         }
      }
      else if( m_GameMode[ GAMEOVERMODE ] )
      {
         DrawGameOverMode();
      }
      else if( m_GameMode[ OPTIONSMODE ] )
      {
         if( !m_bDoOptionAction )
         {
//            m_DisplayListManager.call( GL_LIST_OPTIONS );
//            DrawOptionsCursor();
            m_GameOptionsListBox.Draw( -2.0, 5.0, m_Z );
            m_DisplayListManager.call( GL_LIST_TEXTCOLOR );
         }
         else
         {
            if( m_GameOptions[ HELP ] )
            {
               m_DisplayListManager.call( GL_LIST_HELP );
//               m_DisplayListManager.call( GL_LIST_OPTIONS_INSTRUCTIONS );
            }
            if( m_GameOptions[ DIFFICULTY ] )
            {
               m_DifficultyOptionsListBox.Draw( -2.0, 5.0, m_Z );
//               DrawDifficulty();
//               m_DisplayListManager.call( GL_LIST_OPTIONS_INSTRUCTIONS );
            }
            if( m_GameOptions[ GREETZ ] )
            {
               m_DisplayListManager.call( GL_LIST_GREETZ );
//               m_DisplayListManager.call( GL_LIST_OPTIONS_INSTRUCTIONS );
            }
            if( m_GameOptions[ MULTIPLAYER ] )
            {
                 m_MultiplayerOptionsListBox.Draw( -5.0, 5.0, m_Z );
//               DrawMultiPlayerSubOption();
//               m_DisplayListManager.call( GL_LIST_OPTIONS_INSTRUCTIONS );
            }
            if( m_GameOptions[ GAMEKEYS ] )
            {
               m_GameKeysListBox.Draw( -5.0, 5.0, m_Z );
//               DrawGameKeySubOptions();
//              m_DisplayListManager.call( GL_LIST_OPTIONS_INSTRUCTIONS );
            }
            if( m_GameOptions[ DETAIL ] )
            {
               m_DetailOptionsListBox.Draw( -5.0, 5.0, m_Z );
//               DrawDetailSubOptions();
//               m_DisplayListManager.call( GL_LIST_OPTIONS_INSTRUCTIONS );
            }
            if( m_GameOptions[ VSYNC ] )
            {
               m_VSyncListBox.Draw( -5.0, 5.0, m_Z );
//               DrawDetailSubOptions();
//               m_DisplayListManager.call( GL_LIST_OPTIONS_INSTRUCTIONS );
            }
            if( m_GameOptions[ SOUND ] )
            {
               m_SoundListBox.Draw( -5.0, 5.0, m_Z );
//               DrawDetailSubOptions();
//               m_DisplayListManager.call( GL_LIST_OPTIONS_INSTRUCTIONS );
            }
         }
      }

      if( m_bP2DrawConsole )
         DrawP2Console();
      if( m_bTrackMouse )
         DrawMousePosition();

      if( m_TestList >= 0 )
      {
         ::glLoadIdentity();
         m_DisplayListManager.call( m_TestList );
      }

      if( m_GameMode[ GAMEMODE ] || m_GameMode[ CONNECTMODE ] || m_GameMode[ DEMOMODE ] )
      {
         ::glLoadIdentity();
         m_DisplayListManager.call( GL_LIST_PLAYER_1_NICK );
         m_DisplayListManager.call( GL_LIST_PLAYER_2_NICK );
         if( m_bP2Chatting )
            m_DisplayListManager.call( GL_LIST_P2CHATTING );

         if( m_GameMode[ CONNECTMODE ] )
         {
            m_GamesListBox.Draw( 10, 18.5, m_Z );
            m_DisplayListManager.call( GL_LIST_TEXTCOLOR );
         }
      }

      if( m_GameMode[ CONNECTMODE ] || m_GameMode[ GAMEOVERMODE ] || m_GameMode[ OPTIONSMODE ] ) // m_GameMode[ GAMEMODE ] || m_GameMode[ DEMOMODE ]
      {
         m_DisplayListManager.call( GL_LIST_TEXTCOLOR );
      }

      if( m_bNetworkActive )
      {
         DrawPlayer2IP();
         DrawPing();
         DrawBytesSent();
         m_DisplayListManager.call( GL_LIST_BUFFERSIZE );
      }
   }

   // Buttons
   ::glEnable( GL_TEXTURE_2D );
   m_OptionsButton.Draw();
   m_RightButton.Draw();
   m_LeftButton.Draw();
   m_CloseButton.Draw();
   m_MinButton.Draw();
   m_FullScreenButton.Draw();
   m_ChatButton.Draw();
   m_CommandsButton.Draw();
   ::glDisable( GL_TEXTURE_2D );
   m_DisplayListManager.call( GL_LIST_TEXTCOLOR );

   if( m_bP1DrawConsole )
      DrawP1Console();
   if( m_bFullScreen )
   {
      ShowFPSinWindow( -25.7, 23.0, m_Z );
   }   
}

void MainWindow::ShowFPSinWindow( const float x, const float y, const float z )
{
   char str[25];
   ::_snprintf_s( str, 25, 25, "%.2f fps", m_FrameRate.FPS() );
   m_Font.Print( str, x, y, z );
}

bool MainWindow::ChangeScreenResolution( int width, int height, int bitsPerPixel, DWORD dwflags )
{
   DEVMODE     dmScreenSettings;											
   ::ZeroMemory( &dmScreenSettings, sizeof( DEVMODE ) );
   dmScreenSettings.dmSize       = sizeof( DEVMODE );
   dmScreenSettings.dmPelsWidth  = width;
   dmScreenSettings.dmPelsHeight = height;
   dmScreenSettings.dmBitsPerPel = bitsPerPixel;
   dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
   return ( ::ChangeDisplaySettings ( &dmScreenSettings, dwflags ) != DISP_CHANGE_SUCCESSFUL )
   ? false : true;
}

bool MainWindow::EnableOpenGL( void )
{
   return m_GLRC.EnableOpenGL( m_WinBase );
}

void MainWindow::Initialise( void )
{
   int ret = IDRETRY;
   do
   {
      if( !EnableOpenGL() )
      {
         ret = Retry( std::string("Unable to initialize OpenGL\n") + GetLastSystemError() + "Would you like to retry" );
      }
      else
      {
         break;
      }
   } while( ret == IDRETRY );

   if( ret == IDCANCEL )
   {
      Alert( "Program exiting" );
      ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
   }
   else
   {
      SetVSync( (m_bVSyncOn) ); // ? true : false 
      ::glDepthFunc( GL_LEQUAL );   // The Type Of Depth Testing (Less Or Equal)
      ::glEnable( GL_DEPTH_TEST );
//      glBegin( GL_QUADS );
//      ::glEnable( GL_DEPTH_TEST );
//      glEnd();
//      m_p1Console.LogThisLine( " glEnable( GL_DEPTH_TEST ) " + m_GLErrorStrings.GetError(), " gl" );
//      ::glShadeModel( GL_SMOOTH );  // Select Smooth Shading
//      ::glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
//      ::glBlendFunc( GL_ONE, GL_SRC_ALPHA ); // Set Blending Mode (Cheap / Quick)
//      ::glEnable( GL_BLEND );
      ::glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );

      m_Font.BuildFont( "font.dds", RGB( 0, 0, 0 ), 0.65, 16.0, 20 );

      if( ret == IDCANCEL )
      {
         Alert( "Program exiting" );
         ::PostMessage( m_WinBase, WM_QUIT, 0, 0 );
      }
      else
      {
         if( ERROR_CNLT == m_TextureManager.LoadGLTextureBitmapFile( "ball.bmp" )      )
         {
            m_LoadingErrors += "Could not Load Texture ball.bmp\n";
         }
         if( ERROR_CNLT == m_TextureManager.LoadGLTextureBitmapFile( "paddle1.bmp" )     )
         {
            m_LoadingErrors += "Couldnot Load Texture paddle1.bmp\n";
         }
         if( ERROR_CNLT == m_TextureManager.LoadGLTextureBitmapFile( "paddle2.bmp" )     )
         {
            m_LoadingErrors += "Could not Load Texture paddle2.bmp\n";
         }
         if( ERROR_CNLT == m_TextureManager.LoadGLTextureBitmapFile( "PlayingArea.bmp" ) )
         {
            m_LoadingErrors += "Could not Load Texture PlayingArea.bmp\n";
         }
         if( ERROR_CNLT == m_TextureManager.LoadGLTextureBitmapFile( "arrow.bmp" ) )
         {
            m_LoadingErrors += "Could not Load Texture arrow.bmp\n";
         }
         if( ERROR_CNLT == m_TextureManager.LoadGLTextureBitmapFile( "close.bmp" ) )
         {
            m_LoadingErrors += "Could not Load Texture close.bmp\n";
         }
         if( ERROR_CNLT == m_TextureManager.LoadGLTextureBitmapFile( "min.bmp" ) )
         {
            m_LoadingErrors += "Could not Load Texture min.bmp\n";
         }
         if( ERROR_CNLT == m_TextureManager.LoadGLTextureBitmapFile( "chat.bmp" ) )
         {
            m_LoadingErrors += "Could not Load Texture chat.bmp\n";
         }
         if( ERROR_CNLT == m_TextureManager.LoadGLTextureBitmapFile( "help.bmp" ) )
         {
            m_LoadingErrors += "Could not Load Texture help.bmp\n";
         }

         if( m_LoadingErrors.size() )
         {
            Alert( m_LoadingErrors );
            m_LoadingErrors.clear();
         }

         InitializeListBoxes();
         InitializeListButtons();

         InitLists();
      }
   }
}

void MainWindow::InitLists( void )
{
   try
   {
//         GL_LIST_CLEAR
      m_DisplayListManager.begin();
      ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//      ::glClearDepth( 1.0f );
      ::glLoadIdentity();
      ::glColor4f( 1.0, 0.502, 0, 0.0 );
      m_DisplayListManager.end();
//         GL_LIST_TEXTCOLOR 
      m_DisplayListManager.begin();
      ::glColor4f( 1.0, 0.502, 0, 0.0 );
      m_DisplayListManager.end();
//         GL_LIST_TEXTURECOLOR 
      m_DisplayListManager.begin();
      ::glColor4f( 1.0, 1.0, 1.0, 0.5 );
      m_DisplayListManager.end();
//     GL_LIST_BG 
      m_DisplayListManager.begin();
      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[3] );
      ::glBegin( GL_QUADS );
      m_TexQuad.Draw( 0, 0, m_Z-0.25 );
      ::glEnd();
      m_DisplayListManager.end();
//         GL_LIST_BALL
      m_DisplayListManager.begin();
      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[0] );
      m_Ball.DrawConstant();
      m_DisplayListManager.end();
//     GL_LIST_TRAIL
      m_DisplayListManager.begin();
      m_Ball.DrawTrail();
      m_DisplayListManager.end();
//         GL_LIST_PADDLE1
      m_DisplayListManager.begin();
      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[1] );
      ::glBegin( GL_QUADS );
      m_Paddle1.OGLDraw();
      ::glEnd();
      m_DisplayListManager.end();
//         GL_LIST_PADDLE2
      m_DisplayListManager.begin();
      ::glBindTexture( GL_TEXTURE_2D, m_TextureManager[2] );
      ::glBegin( GL_QUADS );
      m_Paddle2.OGLDraw();
      ::glEnd();
      m_DisplayListManager.end();
//         GL_LIST_TITLE_STR1
      std::string temp  = "Pong Version ";
      temp          += GetFileVersionValue( NULL, "ProductVersion" );
      temp          += " by try_catch_this";
      m_DisplayListManager.begin();
      ::glTranslatef( -10, 10, m_Z );
      m_Font.Print( temp.c_str() );
      m_Font.Print( "Pardon the lack of a better looking GUI", -11,  6, m_Z );
      m_DisplayListManager.end();
//         GL_LIST_TITLE_STR2
      m_DisplayListManager.begin();
      m_Font.Print( "< Perss Enter to Continue >",  -10, -2, m_Z );
      m_DisplayListManager.end();
//GL_LIST_OPTIONS_INSTRUCTIONS
      m_DisplayListManager.begin();
      m_Font.Print( "<- Escape | Enter ->", -5, -2, m_Z );
      m_DisplayListManager.end();
// GL_LIST_HELP
      m_DisplayListManager.begin();
      DrawHelp();
      m_DisplayListManager.end();
// GL_LIST_GREETZ
      m_DisplayListManager.begin();
      DrawGreetz();
      m_DisplayListManager.end();
// GL_LIST_OPTIONS
      m_DisplayListManager.begin();
      DrawOptions();
      m_DisplayListManager.end();
// GL_LIST_COMMANDLIST
      m_DisplayListManager.begin();
      DrawCommandList();
      m_DisplayListManager.end();
// GL_LIST_DEMO_STR1
      m_DisplayListManager.begin();
      m_Font.Print( "DemoMode", -10, 8, m_Z );
      m_DisplayListManager.end();
// GL_LIST_DEMO_STR2
      m_DisplayListManager.begin();
      m_Font.Print( "Press Enter/Return To Continue", -10, 6, m_Z );
      m_DisplayListManager.end();
//     GL_LIST_DIFFICULTY
      m_DisplayListManager.begin();
      m_Font.Print( "Difficulty", -8, 10, m_Z );
      m_Font.Print( "Impossible", -5,  8, m_Z );
      m_Font.Print( "Hard",       -5,  6, m_Z );
      m_Font.Print( "Normal",     -5,  4, m_Z );
      m_Font.Print( "Easy",       -5,  2, m_Z );
      m_DisplayListManager.end();
//    GL_LIST_MULTIPLAYER 
      m_DisplayListManager.begin();
      m_Font.Print( "One Player",        -5,  6, m_Z );
      m_Font.Print( "Two Player",        -5,  4, m_Z );
      m_Font.Print( "Two Player Online", -5,  2, m_Z );
      m_DisplayListManager.end();
//    GL_LIST_OPTIONS_CURSOR
      m_DisplayListManager.begin();
      m_Font.Print( "*" );
      m_DisplayListManager.end();
//   GL_LIST_GAMEKEY_SUBOPTIONS_STR1 
      m_DisplayListManager.begin();
      m_Font.Print( "Player One Uses KeyPad", -5,  10, m_Z );
      m_DisplayListManager.end();
//    GL_LIST_GAMEKEY_SUBOPTIONS_STR2
      m_DisplayListManager.begin();
      m_Font.Print( "Player One Uses Mouse",  -5,  10, m_Z );
      m_DisplayListManager.end();
//     GL_LIST_GAMEKEY_SUBOPTIONS_STR3
      m_DisplayListManager.begin();
      m_Font.Print( "Player Two Uses KeyPad", -5,   8, m_Z );
      m_DisplayListManager.end();
//     GL_LIST_GAMEKEY_SUBOPTIONS_STR4
      m_DisplayListManager.begin();
      m_Font.Print( "Player Two Uses Mouse",  -5,   8, m_Z );
      m_DisplayListManager.end();
//     GL_LIST_CONNECT
      m_DisplayListManager.begin();
      m_Font.Print( "Wait For Connection - ( Act as Server )",    -5,  2, m_Z );
      m_Font.Print( "Connect To Server",                          -5,  0, m_Z );
      m_Font.Print( "<- Escape | Enter ->",                       -5, -4, m_Z );
      m_DisplayListManager.end();
//    GL_LIST_DETAIL
      m_DisplayListManager.begin();
      m_Font.Print( "Detail Options Goes Here", -5, 0, m_Z );
      m_DisplayListManager.end();
//         GL_LIST_P1CONSOLE
      m_DisplayListManager.begin();
      ::glLoadIdentity();
      m_DisplayListManager.end();
//         GL_LIST_P1CONSOLE_CURSOR
      m_DisplayListManager.begin();
      ::glLoadIdentity();
      m_DisplayListManager.end();
      BuildP1ConsoleList();
//         GL_LIST_P2CONSOLE
      m_DisplayListManager.begin();
      ::glLoadIdentity();
      m_DisplayListManager.end();
//         GL_LIST_P2CONSOLE_CURSOR
      m_DisplayListManager.begin();
      ::glLoadIdentity();
      m_DisplayListManager.end();
      BuildP2ConsoleList();
// GL_LIST_OPTIONS_SCORE
      m_DisplayListManager.begin();
      m_Font.Print( m_P1ScoreSStream.str(), -15, 13, m_Z );
      m_Font.Print( m_P2ScoreSStream.str(),  +5, 13, m_Z );
      m_DisplayListManager.end();
//  GL_LIST_OPTIONS_ROUND
      m_DisplayListManager.begin();
      m_Font.Print( m_RoundSStream.str(), -2, 13, m_Z );
      m_DisplayListManager.end();
//   GL_LIST_GAME_SPEED
      m_DisplayListManager.begin();
      m_Font.Print( m_GameSpeedSStream.str(), -15.0f, -13.5f, m_Z );
      m_DisplayListManager.end();
//   GL_LIST_GAME_DIFFICULTY 
      m_DisplayListManager.begin();
      m_Font.Print( m_DifficultySStream.str(),  5.0f, -13.5f, m_Z );
      m_DisplayListManager.end();
//   GL_LIST_PLAYER_1_NICK
      m_DisplayListManager.begin();
      m_Font.Print( m_P1Nick, -15, -15, m_Z );
      m_DisplayListManager.end();
//   GL_LIST_PLAYER_2_NICK
      m_DisplayListManager.begin();
      m_Font.Print( m_P2Nick, 5, -15, m_Z );
      m_DisplayListManager.end();
//         GL_LIST_CLIENT_IP
      m_DisplayListManager.begin();
      m_Font.Print( m_Player2IP, 5, -16, m_Z );
      m_DisplayListManager.end();
//         GL_LIST_PING
      m_DisplayListManager.begin();
      m_Font.Print( m_PingStringStream.str(), 5, -17, m_Z );
      m_DisplayListManager.end();
//         GL_LIST_BYTES_SENT
      m_DisplayListManager.begin();
      m_Font.Print( m_BytesSentSStream.str(), 5, -18, m_Z );
      m_DisplayListManager.end();
// GL_LIST_P2CHATTING
      m_DisplayListManager.begin();
      m_Font.Print( "P2 Chatting", 18, 12, m_Z );
      m_DisplayListManager.end();
// GL_LIST_BUFFERSIZE
      m_DisplayListManager.begin();
      m_Font.Print( "", 18, 12, m_Z );
      m_Font.Print( "", 18, 12, m_Z );
      m_DisplayListManager.end();
   }
   catch(...)
   {
   }
}

bool MainWindow::Reshape( long width, long height )
{
   m_Width  = width;
   m_Height = height;
   std::ostringstream temp;
   temp << "{ width="  << width;
   temp << ", height=" << height << " }";
   m_p1Console.AddLine( temp.str() );
   if( height < 1 ) // Prevent A Divide By Zero By
      height=1;
   ::glViewport( 0, 0, (GLsizei)(width), (GLsizei)(height) );
   ::glMatrixMode( GL_PROJECTION );
   ::glLoadIdentity();                                          // Reset The Projection Matrix
   ::gluPerspective( m_FieldOfView, (GLfloat)(width)/(GLfloat)(height), // Calculate The Aspect Ratio Of The Window
                     0.010f, 1000.0f );
   ::glMatrixMode( GL_MODELVIEW );                              // Select The Modelview Matrix
   ::glLoadIdentity();
   SetPlayingAreaRect();
   SetInitialPaddlePositions();
   return false;
}

WPARAM MainWindow::RunGameLoop( void )
{
   MSG msg;
   bool isMessagePumpActive = true;
   int  iReturn             = 0;
   TimeInterval Interval;

   while( isMessagePumpActive ) // While The Message Pump Is Active
   {
      if( m_bQuit )
      {
         isMessagePumpActive = false;
      }
      if( ::PeekMessage( &msg, m_WinBase, 0, 0, PM_REMOVE ) )
      {
         if( m_bNetworkActive )
         {
            GetDataFromNetWork();
            SendData();
         }
         // Check For WM_QUIT Message
         if( msg.message != WM_QUIT ) // Is The Message A WM_QUIT Message?
         {
            ::TranslateMessage( &msg );
            ::DispatchMessage(  &msg );
         }
         else // Otherwise ( If Message Is WM_QUIT )
         {
            isMessagePumpActive = false; // Terminate The Message Pump
         }
      }
      else // If There Are No Messages
      {
         if( !m_bisVisible ) // If Window Is Not Visible
         {
            if( !m_bNetworkActive )
            {
               ::WaitMessage(); // Application Is Minimized Wait For A Message
            }
            else
            {
               GetDataFromNetWork();
               ::WaitMessage();
               SendData();
            }
         }
         else// If Window Is Visible
         {
            GetDataFromNetWork();
//            if( m_LimitFPS )
//            {
               if( (m_GameMode[ GAMEMODE ] || m_GameMode[ DEMOMODE ] ) )
               {
                  m_deltaGameTime        = Interval.TimeSinceLastCall()*m_GameSpeed;
                  m_deltaGameTimeSquared = m_deltaGameTime*m_deltaGameTime;
                  Interval.Begin();
                  if( !m_GameMode[ TITLEMODE ] && !m_GameMode[ GAMEOVERMODE ] )
                  {
                     UpdateObjects();
                  }
                  if( m_bBallServed )
                     HandleCollisions();
               }
               if( m_bNetworkActive )
               {
                  if( SendInterval.TimeSinceLastCall() > m_send_interval )
                  {
                    if( (m_bisServer && m_bServerServes) || (!m_bisServer && !m_bServerServes) )
                    {
                        CopyBallPositionForSending();
                        m_SendStream.AddPacket( (char*)&m_BallPosition,   sizeof(S_BallPosition),   packet::PACKET_FLAG_BALL_POSITION   );
                     }
                     CopyPaddlePositionForSending();
                     m_SendStream.AddPacket( (char*)&m_PaddlePosition, sizeof(S_PaddlePosition), packet::PACKET_FLAG_PADDLE_POSITION );
                     SendInterval.Begin();
                  }

               }
			   m_Fmod.Update();
               Draw();
               ::SwapBuffers( m_GLRC );            
               if( !m_bFullScreen )
                  m_FrameRate.ShowFPSinTitle( m_WinBase );
//            }
            SendData();
         }
      }
   }
   SaveConfig();
   return msg.wParam;
}

int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,      int nFunsterStil )
{
//   Alert( GetSystemProcessorInfo() );
//struct _packet{ char id[0]; };
//Alert( (long) sizeof(_packet) );
//Alert( (long) (sizeof(char)*2+sizeof(std::size_t)) );

   try
   {
      MainWindow App( lpszArgument );
      int x_pos = (int)(::GetSystemMetrics( SM_CXSCREEN )*0.005),
          y_pos = (int)(::GetSystemMetrics( SM_CYSCREEN )*0.005);
      int width  = ::GetSystemMetrics( SM_CXSCREEN )-x_pos*2,
          height = ::GetSystemMetrics( SM_CYSCREEN )-y_pos*2;

      if( App.m_bWindowPropertiesOverided )
      {
         x_pos   = App.m_x_pos;
         y_pos   = App.m_y_pos;
         width   = App.m_Width;
         height  = App.m_Height;
      }

      if( App.Create( "OpenGL Pong Online!", HWND_DESKTOP, x_pos, y_pos, width, height ) )
      {
         App.Initialise();
         App.SetTitleModeTrue();
         App.ParseCommandLine();
         if( App.m_bFullScreen )
         {
            App.m_bFullScreen = false;
            ::PostMessage( App.m_WinBase, MainWindow::WM_TOGGLEFULLSCREEN, 0, 0 );
         }

         App.m_WinBase.Show( SW_SHOWNORMAL ); // Send a WM_SIZE Message
//         ::SetWindowPos( App, HWND_TOP, x_pos, y_pos, width, height, SWP_SHOWWINDOW );
         return App.RunGameLoop();
      }
   }
   catch(...)
   {
      return -1;
   }
   return 0;
}

