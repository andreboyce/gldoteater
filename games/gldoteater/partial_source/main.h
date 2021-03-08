#ifndef __Main_H  // inclusion guard
#define __Main_H

// Files:
// Game Design Document.cpp : 
// gamekeys.cpp             : all key mapping done here
// handlecollisions.cpp     : collision detection done here
// load.cpp                 : loading data
// main.cpp                 : game loop
// mainfunctions.cpp        : mics stuff done here
// messagehandler.cpp       : all the messages are recieved here
// render.cpp               : all the rendering is done here
// sound.cpp                : all the sounds are played here
// updateobjects.cpp        : all the objects are ubdated here

#ifndef WIN32
#define WIN32 // VS 8 dosent do this automatically and enet needs it
#endif


#include "../../../enet/enet.h"
#include "../../../projects.h" // custom libary used in the aid of producing this game
                            // includes many custom classes that can be used for the creation of games
                            // and game tools
#include <cmath>
#include <list>

#include "resource.h"       // resources used by the game built into the exe
#include "C_GHost.h"
#include "C_DotEater.h"
//#include "../../../MathVector/Vector2D/Vector2D.h"


//#include "GhostData.h"
//#include "PlayerData.h"

#define IGNORE_CONSOLE                 true
#define INITIAL_LIVES                    12
#define MAX_NICK_LENGHT                  20
#define TICK_TIME                        87 // miliseconds
#define UPDATE_POSITION_AND_DIRECTION    87
#define UPDATE_PING                    1000
#define VERSION_STRING_LENGTH            10

// macros that determine if a key is up or down
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define IsSolid( x, y )  (( m_TileMap[y][x].m_Solidity == m_TileMap.maxsolidity() ) ? true : false )
#define mwIsSolid( x, y )  (( mw.m_TileMap[y][x].m_Solidity == mw.m_TileMap.maxsolidity() ) ? true : false )

// macro to toggle the state of a boolean
#define TOGGLE(x) ( (x) ? (x=false) : (x=true) )


class MainWindow: public CWinMain
{
   // let WinMain access the private and protected variables so I dont have to write accessors
   friend int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                              LPSTR lpszArgument,      int nFunsterStil );
   public:
      MainWindow( void );
      virtual ~MainWindow( void );

      typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC)    ( GLenum target, GLuint buffer         );
      typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) ( GLsizei n,     const GLuint *buffers );
      typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC)    ( GLsizei n,     GLuint *buffers       );
      typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC)    ( GLenum target, int size, const GLvoid *data, GLenum usage );
      typedef void (APIENTRY * PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);

      PFNGLGENBUFFERSARBPROC     glGenBuffersARB;     // VBO Name Generation Procedure
      PFNGLBINDBUFFERARBPROC     glBindBufferARB;     // VBO Bind Procedure
      PFNGLBUFFERDATAARBPROC     glBufferDataARB;     // VBO Data Loading Procedure
      PFNGLDELETEBUFFERSARBPROC  glDeleteBuffersARB;  // VBO Deletion Procedure
      PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements; // DrawRangeElements

      // custom messages
      enum
      {
         WM_TOGGLEFULLSCREEN=WM_USER+1, // send a message that causes the game to toggle between fullscreen and windowed mode
      };

      // game modes
      enum
      {
         TITLE_SCREEN = 0,
         GAME_IN_PROGRESS,
         GAME_OVER,
         YOU_WIN_MODE,
         TWO_PLAYER_MODE,
         CHOOSE_CLIENT_OR_SERVER_MODE,
         CONNECT_MODE,
         GET_SERVER_INFORMATION_FROM_USER,
         LISTEN_MODE,
         VIEW_HIGH_SCORES,
         VIEW_STATS,
      };

      // sound channels
      enum
      {
         MISC_CHANNEL,
         DIE_CHANNEL,
         YOU_WIN_CHANNEL,
      };

      enum
      {
         SOUND_PACMAN,
         SOUND_DIE,
         SOUND_CHOMP,
         SOUND_CHOMP2,
         SOUND_EATGHOST,
         SOUND_LIFEUP,
         SOUND_CONNECT,
         SOUND_RANDOMITEM,
      };


      // maps
      enum
      {
         TITLE_MAP,
         YOUWIN_MAP,
         GAMEOVER_MAP,
         STATS_MAP,
         HIGH_SCORES,
         FIRST_LEVEL_MAP,
      };

      enum CONSOLE_INPUT
      {
         GET_NOTHING = 0,
         GET_PLAYER1_NAME,
         GET_PLAYER2_NAME,
         GET_PLAYER_NAME,
         GET_SERVER_IP,
         GET_SERVER_PORT,
      } m_ConsoleInput;

      // network channels
      enum 
      {
         ENET_CHANNEL_TEXT = 0,

         ENET_CHANNEL_DOTS_EATEN,

         ENET_CHANNEL_COMMAND,

         ENET_CHANNEL_SEND_PING,
         ENET_CHANNEL_RECV_SEND_PING,
         ENET_CHANNEL_SEND_REPLY_PING,
         ENET_CHANNEL_RECV_REPLY_PING,

         ENET_CHANNEL_RANDOMITEM_DATA,
         ENET_CHANNEL_RANDOMITEMS,

         ENET_CHANNEL_GAMESTATE,

         ENET_CHANNEL_P1_POSITION_AND_DIRECTION,
         ENET_CHANNEL_P2_POSITION_AND_DIRECTION,
         ENET_CHANNEL_P3_POSITION_AND_DIRECTION,
         ENET_CHANNEL_P4_POSITION_AND_DIRECTION,

         ENET_CHANNEL_GHOST_DATA,
         ENET_CHANNEL_GHOST_POSITION_AND_DIRECTION,

         ENET_CHANNEL_PLAYERINFO,
         ENET_CHANNEL_KEEPALIVE,

         ENET_CHANNEL_SIZEMINUS1,
         ENET_CHANNEL_SIZE,
      };

      enum NetorkCommands
      {
         COMMAND_START = 1,
         COMMAND_GAME_MODE,
         COMMAND_LOAD_MAP,
         COMMAND_NEW_ROUND,
         COMMAND_VIEW_STATS,
         COMMAND_PAUSE,
         COMMAND_PLAYER1_DIED,
         COMMAND_PLAYER2_DIED,
         COMMAND_RESET_PLAYER_POSITION,
         COMMAND_YOU_WIN,
         COMMAND_GAME_OVER,
         COMMAND_RANDOMITEM_EATEN,
         COMMAND_REQUEST_START,
      };
      std::deque<NetorkCommands> NetorkCommandsQueue;


      class NetorkData
      {
         public:
            NetorkData() : channel(0), size_of_data(0), data(NULL) {}
            NetorkData( int _channel, unsigned long _size_of_data, void*_data ) :
               channel( _channel ), size_of_data( _size_of_data ), data( _data )
            {}
            NetorkData( const NetorkData& rhs ) :
               channel( rhs.channel ), size_of_data( rhs.size_of_data ), data( rhs.data )
            {}
            NetorkData& operator=( const NetorkData& rhs )
            {
               channel      = rhs.channel; 
               size_of_data = rhs.size_of_data;
               data         = rhs.data;
               return *this;
            }
            ~NetorkData(){}
            int channel;
            unsigned long size_of_data;
            void *data;
      };
      std::deque<NetorkData> m_NetorkDataQueue;

      struct Ping
      {
         Ping() : m_RemoteTime( 0 ), m_LocalTime( 0 ) {}
         Ping( const Ping& rhs ) : m_RemoteTime( rhs.m_RemoteTime ), m_LocalTime( rhs.m_LocalTime ) {}
         Ping( DWORD RemoteTime, DWORD LocalTime ) :
            m_RemoteTime( RemoteTime ), m_LocalTime( LocalTime )
         {}
         ~Ping(){}
         Ping& operator=( const Ping& rhs )
         {
            m_RemoteTime = rhs.m_RemoteTime;
            m_LocalTime  = rhs.m_LocalTime;
            return *this;
         }
         DWORD m_RemoteTime;
         DWORD m_LocalTime;
      };

      void SendPing();
      void HandleENET_CHANNEL_SEND_PING( char* data, unsigned long length );
      void HandleENET_CHANNEL_RECV_SEND_PING( char* data, unsigned long length );
//      void HandleENET_CHANNEL_SEND_REPLY_PING( char* data, unsigned long length );
//      void HandleENET_CHANNEL_RECV_REPLY_PING( char* data, unsigned long length );

      void SendPlayerInfo();
      void CheckPlayerInfo( char* data, unsigned long length );
      struct PlayerInfo
      {
         PlayerInfo() :
            ip( 0 )
         {
            std::memset( name,    '\0', MAX_NICK_LENGHT       );
            std::memset( version, '\0', VERSION_STRING_LENGTH );
         }
         ~PlayerInfo(){}
         PlayerInfo( char _name[MAX_NICK_LENGHT], unsigned __int32 _ip, char _version[VERSION_STRING_LENGTH] ) :
            ip( _ip )
         {
            std::strncpy( name,       _name, MAX_NICK_LENGHT-1       );
            std::strncpy( version, _version, VERSION_STRING_LENGTH-1 );
            name[ MAX_NICK_LENGHT-1 ]          = '\0';
            version[ VERSION_STRING_LENGTH-1 ] = '\0';
         }
         PlayerInfo( const PlayerInfo& rhs ) :
            ip( rhs.ip )
         {
            std::strncpy( name,       rhs.name, MAX_NICK_LENGHT-1       );
            std::strncpy( version, rhs.version, VERSION_STRING_LENGTH-1 );
            name[ MAX_NICK_LENGHT-1 ]          = '\0';
            version[ VERSION_STRING_LENGTH-1 ] = '\0';
         }
         PlayerInfo& operator=( const PlayerInfo& rhs )
         {
            ip      = rhs.ip;
            std::strncpy( name,       rhs.name, MAX_NICK_LENGHT-1       );
            std::strncpy( version, rhs.version, VERSION_STRING_LENGTH-1 );
            name[ MAX_NICK_LENGHT-1 ]          = '\0';
            version[ VERSION_STRING_LENGTH-1 ] = '\0';
            return *this;
         }
         char name[ MAX_NICK_LENGHT ];
         char version[ VERSION_STRING_LENGTH ];
         unsigned __int32 ip;
         operator std::string( void )
         {
            name[ MAX_NICK_LENGHT-1 ]          = '\0';
            version[ VERSION_STRING_LENGTH-1 ] = '\0';
            std::ostringstream tmp;
            tmp << "{ name: " << name << ", ip: " << ip << ", version: " << version << " }";
            return tmp.str();
         }
      };
      struct PositionDirectionAndRequestedDirection
      {
         Structs::Point m_Tile;
         Direction m_RequestedDirection, m_Direction;
         int id;
         unsigned long timestamp;
      };

      struct HighestFirst
      {
         bool operator()( int i1, int i2 ) const
         {
            return i1 >= i2;
         }
      };

      // Png's are loaded upside down!
      // so you have to give them different text coords than bitmaps

      std::deque<Structs::Point> LeftTextureCoordinates( void )
      {
         std::deque<Structs::Point> TextureCoordinates(4);
         TextureCoordinates[0] = Structs::Point( 1 , 0 );
         TextureCoordinates[1] = Structs::Point( 0 , 0 );
         TextureCoordinates[2] = Structs::Point( 0 , 1 );
         TextureCoordinates[3] = Structs::Point( 1 , 1 );
         return TextureCoordinates;
      }
      std::deque<Structs::Point> RightTextureCoordinates( void )
      {
         std::deque<Structs::Point> TextureCoordinates(4);
         TextureCoordinates[0] = Structs::Point( 0 , 0 );
         TextureCoordinates[1] = Structs::Point( 1 , 0 );
         TextureCoordinates[2] = Structs::Point( 1 , 1 );
         TextureCoordinates[3] = Structs::Point( 0 , 1 );
         return TextureCoordinates;
      }
      std::deque<Structs::Point> UpTextureCoordinates( void )
      {
         std::deque<Structs::Point> TextureCoordinates(4);
         TextureCoordinates[0] = Structs::Point( 0 , 0 );
         TextureCoordinates[1] = Structs::Point( 0 , 1 );
         TextureCoordinates[2] = Structs::Point( 1 , 1 );
         TextureCoordinates[3] = Structs::Point( 1 , 0 );
         return TextureCoordinates;
      }
      std::deque<Structs::Point> DownTextureCoordinates( void )
      {
         std::deque<Structs::Point> TextureCoordinates(4);
         TextureCoordinates[0] = Structs::Point( 1 , 1 );
         TextureCoordinates[1] = Structs::Point( 1 , 0 );
         TextureCoordinates[2] = Structs::Point( 0 , 0 );
         TextureCoordinates[3] = Structs::Point( 0 , 1 );
         return TextureCoordinates;
      }

      // rect to rect collision detection
      // true it the point overlaps with the rect

      bool CheckCollide( const Structs::fRect& Rect1, const Structs::fRect& Rect2 )
      {
         if( (Rect1.bottom > Rect2.top) ) return false;
         if( (Rect1.top < Rect2.bottom) ) return false;
         if( (Rect1.right < Rect2.left) ) return false;
         if( (Rect1.left > Rect2.right) ) return false;
         return true;
      }

      // rect to point collision detection
      // true it the point overlaps with the rect

      bool CheckCollide( const Structs::fRect& Rect, const Structs::dPoint3D& Point )
      {
//         if( !IsInRange( Point.m_z, m_Z, 3.0 ) )
//            return false;
         if( (Rect.bottom > Point.m_y) ) return false;
         if( (Rect.top    < Point.m_y) ) return false;
         if( (Rect.right  < Point.m_x) ) return false;
         if( (Rect.left   > Point.m_x) ) return false;
         return true;
      }

      // message handlers

      static LRESULT MWBaseOnCommand(                 MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnRButtonU(                MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLButtonU(                MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnRDoubleClick(            MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLDoubleClick(            MainWindow &mw, WPARAM wparam, LPARAM lparam );      
      static LRESULT MWBaseOnRButtonD(                MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLButtonD(                MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnMouseMove(               MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnCreate(                  MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnSize(                    MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnPaint(                   MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnNotify(                  MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnKeyDown(                 MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnKeyUp(                   MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnHScroll(                 MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnVScroll(                 MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnTimer(                   MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnKillFocus(               MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnSetFocus(                MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnClose(                   MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnSysCommand(              MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnMove(                    MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnToggleFullScreen(        MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnContextMouseOver(        MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnContextLbtn(             MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnChar(                    MainWindow &mw, WPARAM wparam, LPARAM lparam );

      void EnumerateLocalAddresses( std::deque<std::string>& Ipaddresses );

      static void BackButtonClick( void * argument );
      static void WhatIsMyIPButtonClick( void * argument );
      static void StartButtonClick( void * argument );
      static void RetryButtonClick( void * argument );
      static void ReconnectButtonClick( void * argument );

      void ButtonMouseOverCollision();
      void ButtonClickCollision();

      std::string GetGamemodeString( int GameMode = -1 );
      std::string GetMapString( int Map = -1 );

      bool EraseGameFromMasterServer( void );
      bool SaveGameOnMasterServer( void );
      bool GetGamesFromMasterServer( void );
      bool ParseGamesFromMasterServer( std::string& Games );

      void GetHighScorePlayer1Name();
      void GetHighScorePlayer2Name();
      void GetServerIP();
      void GetPort();
      void GetPlayerName( bool bReplaceNick = false );

      int CheckCommand( std::string command );

      void   SendNetworkCommands();
      void   SendNetworkData();

      void   SendPlayer2Data();
      void   SendDotsEatenToClient( int index );
      void   SendDotsEatenToHost();
      void   SendPositionAndDirectionToClient( int client );
      void   SendPositionAndDirectionToHost();
      void   CreateServer();
      void   CreateClient();
      void   ConnectToHost();
      void   SendToHost(   char *data, long length, int channel, int flag=ENET_PACKET_FLAG_RELIABLE );
      void   SendToClient( char *data, long length, int index, int channel, int flag=ENET_PACKET_FLAG_RELIABLE );
      void   ManageEnetServer();
      void   ManageEnetClient();
      void   InitializeEnet();
      void   DisconnectClient( int index );
      void   DisconnectFromHost( );
      void   DestroyServer();
      void   DestroyClient();

      ENetHost    *m_Server,
                  *m_Client;
      ENetPeer    *m_HostPeer;

      // gamestate
      void RetreivePlayerData( std::vector<char>& packet, C_DotEater& Player );
      void InsertPlayerData( std::vector<char>& packet, C_DotEater& Player );

      void RetreiveGhostData( std::vector<char>& packet, C_GHost& Ghost );
      void InsertGhostData( std::vector<char>& packet, C_GHost& Ghost );

      void InsertData( std::vector<char>& packet, char *data, unsigned long sizeofdata );
      void RetreiveData( std::vector<char>& packet, char *data, unsigned long sizeofdata );

      void GenerateGameState( std::vector< char >& packet );
      void ReadGameState( std::vector< char >& packet );
      // gamestate

      Structs::Point GetTile( const Vector2D& position );
      Structs::Point GetTile( const Structs::dPoint3D& position );

      Structs::fPoint GetTilePos( int x, int y );
      Vector2D GetTilePos( Structs::Point point );

      Structs::Point GetRandomTile();

      void GetText( std::string TextFromConsole );
      std::string GetQuickMessage( int index );

      void GetDotsEaten( char* data, long length );
//      void GetGhostData( char* data, long length );
      void GetGhostPositionAndDirection( char* data, long length );

      void SendGhostPositionAndDirectionToClient( int index );
//      void SendGhostDataToClient( int index );

      void SendRandomItems( int index );
      void GetRandomItemsFromPacket( std::vector< char >& packet );

      void AddHighScore( std::string name, int score );
      void SortHighScores();

      bool AllPlayersAreDead();

      void   HandleListenModeKeys( WPARAM wparam, LPARAM lparam, bool ignore_console = false );
      void   HandleConnectModeKeys( WPARAM wparam, LPARAM lparam, bool ignore_console = false );
      void   HandleTitleScreenKeys( WPARAM wparam, LPARAM lparam, bool ignore_console = false );
      void   HandleChooseClientOrServerKeys( WPARAM wparam, LPARAM lparam, bool ignore_console = false );
      void   HandleUpKeys( WPARAM wparam, LPARAM lparam, bool ignore_console = false );   // handle the key release messages
      void   HandleDownKeys( WPARAM wparam, LPARAM lparam, bool ignore_console = false ); // handle the key press messages

      bool   PlayClip( long sound_clip, short channel=MISC_CHANNEL ); // play a sound clip

      void   HandleCollisions();       // Handle the collisions that are detected

      void   HandlePlayerCollisions( C_DotEater& Player, int index );       // Handle the collisions that are detected
      void   HandleOnlinePlayerCollisions( C_DotEater& Player, int index );

      void   UpdateObjects();                              // Update the objects on the screen

      bool   WinCondition();

      bool   LoadAnimations( void );
      bool   LoadFileNames( std::string filename, std::deque<std::string>& container );
      bool   LoadMapsFilenames( void );   // Load the names of the maps
      bool   LoadMap( std::string map );  // Load the map
      bool   LoadTextures( void );        // Load the textures
      bool   LoadSounds( void );          // Load the sounds
      bool   LoadHighScores( void );
      bool   LoadConfig( void );
      bool   LoadQuickMessages( void );

      bool   SaveHighScores( void );
      bool   SaveConfig( void );

      void   init_depth_buffer_trick( void );
      void   depth_buffer_trick( void );

      void   BuildMapDisplayLists();
      void   BuildMapVBOs();
      void   RenderMapVBOs();

      void   BuildDotDisplayLists();
      void   BuildDotVBOs();
      void   RenderDotVBOs();

      void   DestroyVBOs();

      void   RenderRandomItems();
      void   RenderMousePos();
      void   RenderDirtyTiles();
      void   RenderStatsMode();
      void   RenderMessages();
      void   RenderTransferRate();
      void   RenderTotalDataSent();
      void   RenderPing();
      void   RenderButtons();
      void   RenderChooseClientOrServer();
      void   RenderListenMode();
      void   RenderConnectMode();
      void   RenderPlayer1Inventory();
      void   RenderPlayer2Inventory();
      void   RenderPlayer( C_DotEater& player );             // render the players
      void   RenderGhosts();                                 // render the ghosts
      void   RenderDots();                                   // render the dots

      void   RenderMapToBuildList();
      void   RenderMap();                                    // render a map to the screen
      void   RenderGameScreen();                             // render title screen
      void   RenderTitleScreen();                            // render game screen
      void   RenderGameOverScreen();                         // render game over screen
      void   RenderYouWinScreen();                           // render you win screen
      void   RenderListboxes();                              // render List Boxes
      void   RenderConsole();

      void   UpdateAllClients();
      void   SendStateToServer();

      void   StartServerThread();
      void   StartClientThread();
      void   StopServerThread();
      void   StopClientThread();

      bool   IamServer();

      void   HandleListBoxMouseOverCollision( C_GLListBox& ListBox, Structs::Point MousePosition );
      bool   HandleListBoxClickCollision(     C_GLListBox& ListBox, Structs::Point MousePosition );

      void   HandleButtonMouseOverCollision( C_GLButton& Button, Structs::Point MousePosition );
      bool   HandleButtonClickCollision(     C_GLButton& Button, Structs::Point MousePosition );

      void   ResetPlayer1Position();
      void   ResetPlayer2Position();

      void   ResetRGhostPosition();
      void   ResetBGhostPosition();
      void   ResetYGhostPosition();
      void   ResetWGhostPosition();

      void   ResetPlayerPosition( int index );
      void   MovePlayer( C_DotEater& Player );
      void   MovePlayer( C_DotEater& Player, Structs::Point goal );
      void   MovePlayerOnNetworkUpdate( C_DotEater& Player );

      void GetplayerGoal( int PlayerControlledByMouse );

      Vector2D AlignToHorizontialAxis( Vector2D position );
      Vector2D AlignToVerticalAxis( Vector2D position );
      void   AlignToVerticalAxis( C_DotEater& Player );
      void   AlignToHorizontialAxis( C_DotEater& Player );
      void   AlignToVerticalAxis( C_GHost& ghost );
      void   AlignToHorizontialAxis( C_GHost& ghost );

      void   MoveOffSolidTile( C_GHost& ghost );
      void   MoveOffSolidTile( C_DotEater& Player );

      bool LeftTileIsSolid( Vector2D position );
      bool RightTileIsSolid( Vector2D position );
      bool UpTileIsSolid( Vector2D position );
      bool DownTileIsSolid( Vector2D position );
      bool IsValidTile( Structs::Point tile );

      void SaveSpriteTiles();
      void RestoreSpriteTiles();
      void SetDirection( C_GHost& ghost );

      void   MoveGhost( C_GHost& ghost, Structs::Point goal );

      void   CopyTextToClipBoard( std::string text );

      std::string GetTextFromClipBoard( bool bAddTextToConsole = true );

      void   AddNewPlayer();
      C_DotEater MakePlayer1();
      C_DotEater MakePlayer2();

      Structs::Point GetWhiteGhostSpawn();
      Structs::Point GetBlueGhostSpawn();
      Structs::Point GetYellowGhostSpawn();
      Structs::Point GetRedGhostSpawn();
      void   MoveWhiteGhost();
      void   MoveBlueGhost();
      void   MoveRedGhost();
      void   MoveYellowGhost();

      std::string GetQuickMessage();

      void   UpdateGhostPowerDownTime( C_GHost& ghost );
      void   UpdatePlayerPowerUpTime( C_DotEater& player );
      void   UpdatePlayerInvincibleTime( C_DotEater& player );
      void   CheckForLifeUpFromPoints( C_DotEater& Player );
      void   UpdateTimeTillRespawn( C_DotEater& Player );

      Direction  GetDirectionToNextTile( Structs::Point start, Structs::Point end );

      void   GetPathAndDirection( C_DotEater& Player, Structs::Point end );
      void   GetPathAndDirection( C_GHost& ghost, Structs::Point end );

      Structs::fRect BoundingRect( float field_of_view_y, float z, float Width, float Height );


      void   SaveIntervals( void );                          // save the time elapsed in all the TimeInterval

      float  x_min_boundary();                             // x min boundary
      float  x_max_boundary();                             // x max boundary
      float  y_min_boundary();                             // y min boundary
      float  y_max_boundary();                             // y max boundary

      void   IncrementLevel( void );

      void   InitialiseNewRound();                         // SetUp the object positions
      void   InitialiseNewOnlineRound();                   // new round online

      void   InitialiseServerOnlineGame();
      void   InitialiseClientOnlineGame();

      void   InitializeButtons( );
      void   InitialiseNewGame();                          // SetUp the object positions
      void   Initialise();                                 // Initilaise API's
      void   InitialiseOpenGL();                           // Initilaise opengl state specific to this game
      void   InitializeListBoxes( void );

      bool   EnableOpenGL();                               // Enable open gl in the main window
      bool   DisableOpenGL();                              // Enable open gl in the main window

      WPARAM RunGameLoop( void );                          // game loop
      bool   Reshape( long width, long height );           // resize the opengl viewoprt when the window is resized
      void   ShowFPSinWindow( float x, float y, float z ); // Show the frame rate in the window

      bool CollideWithSolidTile( Structs::fRect Rect );
      Structs::fRect GetTileRect( int x, int y );
      void KillPlayerAndStartNewRound();
      void KillPlayer( C_DotEater& Player, int index );
      void GetPlayerDirection( WPARAM wparam, C_DotEater& Player );

      bool DotOnTile( Structs::Point tile );
      void AddDots( std::deque<Structs::Point> PowerPellets );
      void GetBigDots( std::deque<Structs::Point>& big_dots );
      void GetRandomItems( void );
      bool IsRandomItemTile( Structs::Point tile );
      bool IsRandomItemTile( int x, int y );
      int  GetRandomItemIndex( int x, int y );
      int  GetRandomItemIndex( Structs::Point tile );
      void EatRandomItem( C_DotEater& Player, int index );


      bool ValidateIP( std::string ip );

      void ShowConsole();

      int  EnumerateLocalAddresses();
      void HandleCommandLine( HINSTANCE hThisInstance, HINSTANCE hPrevInstance, 
                              LPSTR lpszArgument,      int nFunsterStil );

   protected:

      RandomNum            m_Rand;
      KeyInput             m_Keys;      // Saves keyup and keydown states
      CGLRC               m_GLRC;      // Open GL rendering context
      CFmod               m_Fmod;      // Fmod sound system
      C_GetDC              m_WinBaseDC; // device contect of main window cliet area
      FrameRate            m_FrameRate; // class to claculate frame rate
      Console              m_Console;   // For debugging purposes allows programmer to 
                                        // capture the state of variables while program is running
      C_MessageQueue             m_MessageQueue;
      C_GLTextureManagerHash     m_TextureManager;     // Texture manager class loads textures
      C_GLTextureMappedFont      m_Font;
      C_GLOutLinedFont           m_OFont;               // Prints text to the screen
      C_GLDisplayListManagerHash m_DisplayListManager; // Manages Opengl Display Lists
      C_GLErrorStrings           m_GLErrorStrings;     // Easy way to get errors from opengl state macheine
      C_Flash                    m_Flash;              // A class to switch things on and off at timed intervals
      C_GLVBO                    m_MapVbos;

      LimitFPS               m_LimitFPS;           // Limit the frames displayed per second

      TileMap              m_TileMap; // tile map class

      AStar::AStar2d         m_Astar;

      CMutex                m_Mutex;

      WinThread             m_EnetClientThread;
      static DWORD   WINAPI  EnetClientThreadFunc( LPVOID mw );
      WinThread            m_EnetServerThread;
      static DWORD   WINAPI  EnetServerThreadFunc( LPVOID mw );

//      C_RainParticles        m_RainParticles;
      Ping                   m_LastPingSent, 
                             m_LastPingRecv;
      PlayerInfo             m_RemotePlayerInfo;

      Structs::fRect       m_BoundingRect, // Bounding rect for m_Z
                             m_TextBoundingRect; // text has a different z value
      float                  m_BoundingRectWidth, 
                             m_BoundingRectHeight,
                             m_BoundingRectHalfWidth,
                             m_BoundingRectHalfHeight,
                             m_TextBoundingRectWidth,
                             m_TextBoundingRectHeight,
                             m_TextBoundingRectHalfWidth,
                             m_TextBoundingRectHalfHeight,
                             m_InvBoundingRectWidthDivTileMapWidth,
                             m_InvBoundingRectHeightDivTileMapHeight,
                             m_BoundingRectHalfWidthMinMapTexturedQuadhalfwidth,
                             m_BoundingRectHalfHeightMinMapTexturedQuadhalfheight;


      volatile bool          m_bQuit,       // flag that will cause the program to exit if set true
                             m_bisVisible,  // true if the window is minimized
                             m_bFullScreen, // true if running in fullscreen mode
                             m_bSoundOn,    // sound on if true
                             m_bGamePaused, // true game pasued (nothing moves)
                             m_bRenderMousePos, // show the mouse position
                             m_bShowTheConsole,
                             m_bShowMessageQueue,
                             m_bTheConsoleAcceptsTyping,
                             m_bIgnoreVK_RETURN,
                             m_bPauseOnSize,
                             m_bNetworkActive,
                             m_bProcessServerThread,
                             m_bProcessClientThread,
                             m_bNetworkThreadActive,
                             m_bRemoteVersionReceived,
                             m_bMoveGhosts,
                             m_bInitializeOnlineGame,
                             m_bInitializeNewOnlineRound,
                             m_bContinueWithNextPlayersCollisionDetection,
                             m_bShowOnlineGameMode,
                             m_bGotToGameOverAfterStats,
                             m_bRenderRandomItems,
                             m_bConnectionFailed,
                             m_bServerCreationFailed,
                             m_bDepthEvenFlag,
                             m_bUseVBOs;

      volatile double                 m_Z,           // the value of the z index where most of the drawing will be done
                             m_TextZ,       // the value of the z index where text should be rendered
                             m_Width,       // width of client area
                             m_Height,      // height of client area
                             m_FieldOfViewY, // field of view of the opengl view port in degrees
                             m_FieldOfViewX,
                             m_bHasKeyboardFocus,    // true if the windows has keyboard focus
                             m_x_pos,                // x position of window
                             m_y_pos,                // y position of window
                             m_deltaGameTime,        // time elapsed sice last frame
                             m_deltaGameTimeSquared, // time elapsed sice last frame squared
                             m_GameSpeed,            // game speed
                             m_bGameSavedOnServer,
//                             m_deltaDataRecv,
                             m_deltaKeepAlive,
                             m_deltaTickTime, // last time since all clients were updated simelteanously
                             m_deltaSendPing,     // time since ping was sent
                             m_deltaPositionAndDirectionUptateTime;

      volatile int           m_GameMode,            // keep track of what mode the game is in
                             m_GhostTexturesIndex,  // where the ghost textures start from
                             m_Level,               // the level that you are in
                             m_Port,
                             m_ArrowTexture,
                             m_BackTexture,
                             m_MapToLoad,
                             m_CurrentMap,
                             m_PlayerControlledByMouse,
                             m_EatenRandomItemIndex,
                             m_InventoryTextureIndex,
                             m_NumberOfInventoryTextures,
                             m_LastGameStateID,
                             m_PositionAndDirectionToHostID;

      unsigned long          m_TimeSinceConnect,
                             m_TimeSinceRoundStarted,
                             m_DeltaClientServerRoundStart;

      double                 m_TotalDataSent,
                             m_TotalDataSentThisSecond;

//      SpriteAnimation      m_P1Animation;

      TimeInterval         m_TotalDataSentThisSecondInterval,
                             m_TimeSinceConnectInterval;

      C_GLTexQuad            m_MapTexturedQuad;      // textured quad

      C_GHost                m_WhiteGhost,
                             m_BlueGhost,
                             m_RedGhost,
                             m_YellowGhost;

      CMesh2D<GLushort>      m_MapMesh, 
                             m_DotMesh;

      C_GLListBox            m_TitleScreenOptionsListBox,
                             m_ChooseClientOrServerListBox,
                             m_GamesListBox,
                             m_HighScoresListBox;

      C_GLButton             m_BackButton, m_WhatIsMyIPButton, 
                             m_StartButton, m_RetryButton, m_ReconnectButton;

      Structs::dPoint3D    m_GLMousePosition;
      Structs::Point       m_MousePosition,
                             m_PlayerGoal;

      std::ostringstream     m_MouseSStream,
                             m_PingSStream,           // last ping time
                             m_TotalDataSentSStream,
                             m_TransferRateSStream;

      std::string            m_MasterServerHost,
                             m_MasterServerPath,
                             m_Nick,
                             m_RemoteHostAddress,
                             m_AppWorkingDirectory,
                             m_DataDirectory,
                             m_Version;

     std::deque<std::string> m_PlayerNames,
                             m_Ipaddresses,
                             m_QuickMessages;


      std::deque<std::string> m_MapFilenames, // 0 title
                                              // 1 you win
                                              // 2 gameover
                                              // 3 and up stages
                              m_MultiPlayerMaps,
                              m_SinglePlayerMaps;
                                              

      std::deque<C_DotEater> m_Players;

      class C_Dot
      {
         public:
            C_Dot(): 
               m_tile(),
               m_bIsPowerPellet(false) {}
            C_Dot( int col, int row, bool bIsPowerPellet = false ) : 
               m_tile( col, row ),
               m_bIsPowerPellet( bIsPowerPellet )
               {}
            C_Dot( Structs::Point tile, bool bIsPowerPellet = false ) : 
               m_tile( tile )
               {}
            C_Dot( const C_Dot& rhs ):
               m_tile( rhs.m_tile ),
               m_bIsPowerPellet( rhs.m_bIsPowerPellet )
               {}
            ~C_Dot(){}
            C_Dot& operator=( const C_Dot& rhs )
            {
               m_tile           = rhs.m_tile;
               m_bIsPowerPellet = rhs.m_bIsPowerPellet;
               return *this;
            }
            bool operator<( const C_Dot& rhs )
            {
               if( rhs.m_bIsPowerPellet && m_bIsPowerPellet )
               {
                   return false;
               }
               if( !rhs.m_bIsPowerPellet && m_bIsPowerPellet )
               {
                   return false;
               }
               return true;
            }
            Structs::Point m_tile; // which tile the dot is on
            bool             m_bIsPowerPellet;
         protected:
         private:
      };
      class RandomItems: public TileMapSpecialData
      {
         public:
            RandomItems() : id( 0 ), TileMapSpecialData(){}
            RandomItems( const RandomItems& rhs ) : id( rhs.id ), TileMapSpecialData( rhs ){}
            RandomItems( const TileMapSpecialData& rhs ) : id( 0 ), TileMapSpecialData( rhs ){}
            RandomItems& operator=( const RandomItems& rhs )
            { 
               id = rhs.id; 
               TileMapSpecialData::operator=( rhs );
               return *this;
            }
            TileMapSpecialData& operator=( const TileMapSpecialData& rhs )
            {
               return TileMapSpecialData::operator=( rhs );
            }
            ~RandomItems(){}
            int id;
            operator std::string ( void )
            {
               std::ostringstream tmp;
               tmp << "{ x: " << x << "," << " y: " << y << ", value: " << value << ", id: " << id << " }";
               return tmp.str();
            }

      };

      std::map< int, std::string >   m_RandomItemTextureMap;
      std::deque< RandomItems >      m_RandomItems;
      std::deque<C_Dot>              m_Dots;
      std::deque<Structs::Point>     m_DotsEaten;
      std::deque<Structs::Point>     m_WalkableTiles;
      std::deque<Structs::Point>     m_SpriteTiles;

   private:
};

#endif

