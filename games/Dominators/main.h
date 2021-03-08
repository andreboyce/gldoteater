#pragma once

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

// C_Tank.cpp         : tank class
// C_Shell.cpp        : shell class
// C_Projectile.cpp   : projectile class

//#define _USE_MATH_DEFINES
//#include <math.h>

#pragma comment( lib, "../../Release/projects.lib" )
//#pragma comment( lib, ".lib" )

#include "../../projects.h" // custom libary used in the ade of producing this game
                            // includes many custom classes that can be used for the creation of games
                            // and game tools

#include "resource.h"       // resources used by the game built into the exe

#include "C_Tank.h"         // holds variables specific to each tank, it is a data driven class
#include "C_Explosion.h"    // holds variables specific to each explosion

// macros that determine if a key is up or down
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

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
         BOSS_MODE,
         GAME_OVER,
         YOU_WIN_MODE,
      };

      // sound channels
      enum
      {
         MAIN_TANK_SHELL_CHANNEL=1,
         MAIN_TANK_EXPLOSION_CHANNEL,
         MAIN_TANK_EXPLOSION_CHANNEL2,
         MAIN_ATANK_SHELL_CHANNEL,
         MAIN_ATANK_EXPLOSION_CHANNEL,
         MAIN_BTANK_SHELL_CHANNEL,
         MAIN_BTANK_EXPLOSION_CHANNEL,
         MAIN_CTANK_SHELL_CHANNEL,
         MAIN_CTANK_EXPLOSION_CHANNEL,
         MAIN_DTANK_SHELL_CHANNEL,
         MAIN_DTANK_EXPLOSION_CHANNEL,
         BOSS_EXPLOSION_CHANNEL,
         MISC_CHANNEL,
         YOU_WIN_CHANNEL,
      };

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
         if( !IsInRange( Point.m_z, m_Z, 3.0 ) )
            return false;
         if( (Rect.bottom > Point.m_y) ) return false;
         if( (Rect.top    < Point.m_y) ) return false;
         if( (Rect.right  < Point.m_x) ) return false;
         if( (Rect.left   > Point.m_x) ) return false;
         return true;
      }

      // message handlers

      static LRESULT MWBaseOnCommand(          MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnRButtonU(         MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLButtonU(         MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnRDoubleClick(     MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLDoubleClick(     MainWindow &mw, WPARAM wparam, LPARAM lparam );      
      static LRESULT MWBaseOnRButtonD(         MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnLButtonD(         MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnMouseMove(        MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnCreate(           MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnSize(             MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnPaint(            MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnNotify(           MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnKeyDown(          MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnKeyUp(            MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnHScroll(          MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnVScroll(          MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnTimer(            MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnKillFocus(        MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnSetFocus(         MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnClose(            MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnSysCommand(       MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnMove(             MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnToggleFullScreen( MainWindow &mw, WPARAM wparam, LPARAM lparam );

      void   HandleUpKeys( WPARAM wparam, LPARAM lparam );   // handle the key release messages
      void   HandleDownKeys( WPARAM wparam, LPARAM lparam ); // handle the key press messages

      bool   PlayClip( long sound_clip, short channel=MISC_CHANNEL ); // play a sound clip

      void   CheckATankCollisions();                       // Handle the collisions with the main tank shells and the ATanks
	  void   CheckBTankCollisions();                       // Handle the collisions with the main tank shells and the BTanks
	  void   CheckCTankCollisions();                       // Handle the collisions with the main tank shells and the CTanks
	  void   CheckDTankCollisions();                       // Handle the collisions with the main tank shells and the DTanks

      void   HandleCollisions();                           // Handle the collisions that are detected
      void   UpdateObjects();                              // Update the objects on the screen
      void   UpdateFreeShells();                           // move the free shells
      void   UpdateEnemyTanks();                           // Make the Enemy Tanks move
      void   RestrictTankPosition( C_Tank& Tank );         // Restrict the enmy tanks to the screen
      void   MoveTank( C_Tank& Tank );                     // Make the enmy tank move
      void   MoveMainTank();                               // move the main tank
      void   MoveBossTankA();                              // move boss tank a
      void   AddNewTanks();                                // add a new row of tanks
      void   RemoveEnmyTanks();                            // remove the tanks that have been added
      void   AddExplosion( Vector2D PositionVector, double duration=100.0, float width=1.0f, float height=1.0f ); // create an explosion
      void   UpdateExplosions( void );                     // determine if the explosion should still exist

      bool   LoadMap( std::string map );  // Load the map
      bool   LoadTextures( void );        // Load the textures
      bool   LoadSounds( void );          // Load the sounds

      void   RenderTanksShells( std::deque<C_Tank> &Tanks ); // go through a list of tanks and render tham
      void   RenderTanks( std::deque<C_Tank> &Tanks );       // go through a list of tanks and render tham
      void   RenderMap();                                    // render a map to the screen
      void   RenderGameScreen();                             // render title screen
      void   RenderTitleScreen();                            // render game screen
      void   RenderGameOverScreen();                         // render game over screen
      void   RenderBossScreen();                             // render boss screen
      void   RenderYouWinScreen();                           // render you win screen
      void   RenderMainTankLife();                           // show how much life you have
      void   RenderBossTankALife();                          // show how much life the boss has
      void   RenderFreeShells();                             // show how much life the boss has
      void   RenderExplosions( void );                       // render the explosions

      void   SaveIntervals( void );                          // save the time elapsed in all the TimeInterval

      float  x_min_boundary();                             // x min boundary
      float  x_max_boundary();                             // x max boundary
      float  y_min_boundary();                             // y min boundary
      float  y_max_boundary();                             // y max boundary

      Structs::fRect MainTank_boundary();                // rectangle in which the MainTank may move
      Structs::fRect Shells_boundary();                  // rectangle to which the shells are restricted

      Structs::fRect BoundingRect( float field_of_view_y, float z, float Width, float Height );

      void   InitialiseNewGame();                          // SetUp the object positions
      void   Initialise();                                 // Initilaise API's
      void   InitialiseOpenGL();                           // Initilaise opengl state specific to this game
      bool   EnableOpenGL();                               // Enable open gl in the main window
      WPARAM RunGameLoop( void );                          // game loop
      bool   Reshape( long width, long height );           // resize the opengl viewoprt when the window is resized
      void   ShowFPSinWindow( float x, float y, float z ); // Show the frame rate in the window

   protected:

      KeyInput               m_Keys;      // Saves keyup and keydown states
      CGLRC                  m_GLRC;      // Open GL rendering context
      CFmod                  m_Fmod;      // Fmod sound system
      C_GetDC                m_WinBaseDC; // device contect of main window cliet area
      FrameRate              m_FrameRate; // class to claculate frame rate
      Console                m_Console;   // For debugging purposes allows programmer to 
                                          // capture the state of variables while program is running
      C_GLTextureManager     m_TextureManager;     // Texture manager class loads textures
      C_GLOutLinedFont       m_Font;               // Prints text to the screen
      C_GLDisplayListManager m_DisplayListManager; // Manages Opengl Display Lists
      C_GLErrorStrings       m_GLErrorStrings;     // Easy way to get errors from opengl state macheine
      C_Flash                m_Flash;              // A class to switch things on and off at timed intervals
//      C_Options              m_GameOptions;        // Class to manage wrapping of game options
      LimitFPS             m_LimitFPS;           // Limit the frames displayed per second

      Structs::fRect       m_BoundingRect, // Bounding rect for m_Z
                             m_TextBoundingRect; // text has a different z value

      bool                   m_bQuit,       // flag that will cause the program to exit if set true
                             m_bisVisible,  // true if the window is minimized
                             m_bFullScreen, // true if running in fullscreen mode
                             m_bSoundOn,    // sound on if true
                             m_bGamePaused, // true game pasued (nothing moves)
                             m_bYouWinMusicPlayed;  // true if end game music played

      double                 m_Z,           // the value of the z index where most of the drawing will be done
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
                             m_ShellDamage;          // Damage caused by shell 

      int                    m_GameMode,            // keep track of what mode the game is in
                             m_RowsOfTanksAdded,    // the amount of tanks that were added
                             m_MaxEnmyRows,         // Max number of enmy rows
                             m_MapTexturesIndex;    // where the map textures start from

      C_GLTexQuad            m_MapTexturedQuad;      // textured quad
      C_GLTexQuad            m_ExplosionTexturedQuad; // textured quads

      C_Tank                 m_MainTank,  // The main tank that you play with (The Dominator)
                             m_BossTankA; // Boss of the A tanks :)

      std::deque<C_Tank>     m_ATanks,
                             m_BTanks,
                             m_CTanks,
                             m_DTanks;

      std::deque<C_Shell>    m_FreeShells; // Shells that came from destroyed ships
      std::deque<C_Explosion> m_Explosions; // List of explosions to be rendered

      void GetFreeShells( C_Tank &Tank ); // Get Shells that came from destroyed ships


      bool AddNewATank(); // add a new a tank
      bool AddNewBTank(); // add a new a tank
      bool AddNewCTank(); // add a new a tank
      bool AddNewDTank(); // add a new a tank

      TileMap  m_TileMap; // tile map class

      TimeInterval         m_AddEnmyRowInterval; // interval that determines when to add a new row of enmies

   private:
};

#endif

