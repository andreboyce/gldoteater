#ifndef __Main_H
#define __Main_H

// windows 98 
//#define _WIN32_WINDOWS 0x0410
//#define         WINVER 0x0410

// windows xp
#define _WIN32_WINDOWS 0x0501
#define         WINVER 0x0501

#include "../../projects.h"
#include "C_MapEditTrackPopupMenu.h"
#include "resource.h"

// shouldnt need this but I do strange...
//#ifndef WM_MOUSEWHEEL
//#define WM_MOUSEWHEEL                   0x020A
//#endif


class MainWindow: public CWinMain
{
   friend int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                              LPSTR lpszArgument,      int nFunsterStil );
      // game modes
   enum
   {
      RENDER_MAP = 1,
   };

#define WM_TOGGLEFULLSCREEN WM_USER+1

   public:
      MainWindow( void );
      virtual ~MainWindow( void );

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
      static LRESULT MWBaseOnToggleFullScreen( MainWindow &mw, WPARAM wparam, LPARAM lparam );
      static LRESULT MWBaseOnMouseWheel(       MainWindow &mw, WPARAM wparam, LPARAM lparam );

      static BOOL CALLBACK SpecialDataTypesDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
      bool   ShowSpecialDataTypesDialog( void );
      HWND   m_HwndShowSpecialDataTypesDialog;
      bool   AddSpecialDataType( std::string name );

      static BOOL CALLBACK TilePropertiesDlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
      bool   ShowTilePropertiesDlg( void );
      HWND   m_HwndTilePropertiesDlg;
      void   DrawTileOnPropertiesDialog( C_HDC& hdc );
      Structs::Point m_TilePropertiesTile;

      bool   LoadMap( std::string map );       // Load the map
      bool   SaveMap( std::string map = "" );  // Save the map

      Structs::Point GetTile( const Vector2D& position );
      Structs::Point GetTile( const Structs::dPoint3D& position );

      void   Initialise();                                 // Initilaise API's
      void   InitialiseOpenGL();                           // Initilaise opengl state specific to this game
      bool   EnableOpenGL();                               // Enable open gl in the main window
      WPARAM RunGameLoop( void );                          // game loop
      int RunGetMessageLoop( void );
      bool   Reshape( long width, long height );           // resize the opengl viewoprt when the window is resized
      void   ShowFPSinWindow( float x, float y, float z ); // Show the frame rate in the window
      void   RenderMap( void );                            // render the map
      void   RenderMouse( void );                            // mouse position
      Structs::fRect BoundingRect( float field_of_view_y, float z, float Width, float Height );

      CWinFileHandler       m_WinFileHandler;
      C_GLTexQuad            m_MapTexturedQuad;      // textured quad
//      TileMapLib::C_TileMap  m_TileMap;   // tile map class
      TileMap              m_TileMap;
      LimitFPS             m_LimitFPS;           // Limit the frames displayed per second
      C_GLDisplayListManager m_DisplayListManager; // Manages Opengl Display Lists
      C_GLTextureManagerHash m_TextureManager;     // Texture manager class loads textures
      FrameRate            m_FrameRate;          // class to claculate frame rate
      C_GetDC                m_WinBaseDC;          // device contect of main window cliet area
      CGLRC                 m_GLRC;               // Open GL rendering context
      C_GLTextureMappedFont  m_Font;               // Prints text to the screen
//      C_GLOutLinedFont       m_Font;               // Prints text to the screen
      Structs::dPoint3D    m_GLMousePosition,
                             m_GLRButtonDownStartPoint,
                             m_GLRButtonDownEndPoint,
                             m_GLLButtonDownStartPoint,
                             m_GLLButtonDownEndPoint;
      Structs::Point       m_MousePosition;
      Console              m_Console;   // For debugging purposes allows programmer to 
                                          // capture the state of variables while program is running
      C_MapEditTrackPopupMenu m_MapEditTrackPopupMenu;

      Structs::fRect       m_BoundingRect, // Bounding rect for m_Z
                             m_TextBoundingRect; // text has a different z value

      std::ostringstream     m_MouseSStream;
      std::string            m_MapFileName;

      int                    m_GameMode,         // mode
                             m_MapTexturesIndex, // index to the first texture in the map in the texture manager
                             m_SelectedDataType,
                             m_TilePropertiesSolidity,
                             m_TilePropertiesGraphic;

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
                             m_GameSpeed;            // game speed

      bool                   m_bQuit,       // flag that will cause the program to exit if set true
                             m_bisVisible,  // true if the window is minimized
                             m_bFullScreen, // true if running in fullscreen mode
                             m_bSoundOn,    // sound on if true
                             m_bGamePaused, // true game pasued (nothing moves)
                             m_bRenderMousePos; //show the mouse pos ?

   protected:

   private:
};


#endif

