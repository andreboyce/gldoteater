#ifndef __C_GLListBox_H
#define __C_GLListBox_H

#include <vector>
#include <list>
#include <string>
#include "../../errorutil/errorutil.h"
#include "../C_GLTextureMappedFont/C_GLTextureMappedFont.h"
#include "../../Structs/Structs.h"

/*
   Spec:
   Has a Title
   Has a Header
   Each row is spli up into a distincenumber of cells
   Has a vertical Scroll Bar
   Vertical cursor keeps track of what line is currently selected
   A number which says how many rows to draw
   Pressing the up Button causes the vertical cursor to increment and 
      move to the next string
   
            Title
            A  <- Scroll Bar Up Button
              Col0 Title Col1 Title Col2 Title ...
              Cell 0,0   Cell 1,0   Cell 2,0     <- Row1
   Cursor ->* Cell 0,1   Cell 1,1   Cell 2,1     <- Row2
              Cell 0,2   Cell 1,2   Cell 2,2     <- Row3
              ...
            V  <- Scroll Bar Down Button

   List Of Available Games
   A
     Name    IP            Port
     me      200.36.45.12  7654
   * tct     200.36.05.12  7654
     noname  200.76.45.102 7654
   V

   cell 1,2 = 200.76.45.102
*/

class C_GLListBox
{
   public:
      C_GLListBox( std::string Title, short NumberOfStrings,
                   C_GLTextureMappedFont& OutLinedFont,
                   short VerticalIndex = 0, short SelectedIndex = 0,
                   bool wrap = true );

      ~C_GLListBox(){}

      void clear( void );
      void clearrows( void );

      short GetSelected( void ) const
      {
         return m_SelectedIndex;
      }

      void SetSelected( int index )
      {
         m_SelectedIndex = index;
         Validate();
      }

      void MoveCursorOnDownKey( void )
      {
         m_SelectedIndex++;
         if( m_SelectedIndex >= m_Strings.size() )
         {
            m_SelectedIndex = 0;
         }
         Validate();
      }

      void MoveCursorOnUpKey( void )
      {
         m_SelectedIndex--;
         if( m_SelectedIndex < 0 )
         {
            m_SelectedIndex = m_Strings.size();
         }
         Validate();
      }

      std::list< std::string > GetSelectedIndex( void )
      {
         if( m_Strings.size() )
            return m_Strings[m_SelectedIndex];
         else
            return std::list< std::string >();
      }

      std::list< std::string > GetRow( int index )
      {
         if( m_Strings.size() > index )
            return m_Strings[index];
         else
            return std::list< std::string >();
      }

      std::vector< std::list< std::string > > GetRows( void ) const
      {
         return m_Strings;
      }

      bool CheckCollide( const Structs::fRect& Rect, const Structs::fPoint& Point )
      {
         if( (Rect.bottom > Point.m_y) ) return false;
         if( (Rect.top    < Point.m_y) ) return false;
         if( (Rect.right  < Point.m_x) ) return false;
         if( (Rect.left   > Point.m_x) ) return false;
         return true;
      }

      bool UpArrowSelected( Structs::fPoint MousePosition );
      bool DownArrowSelected( Structs::fPoint MousePosition );
      bool UpArrowSelected( Structs::Point MousePosition );
      bool DownArrowSelected( Structs::Point MousePosition );
      bool SelecteRow( Structs::fPoint MousePosition );
	  bool SelecteRow( Structs::Point MousePosition );

      bool MouseOver( Structs::Point MousePosition );

      bool ClickUpArrow( Structs::fPoint MousePosition )
      {
//         if( CheckCollide( UpArrowRect(), GLMousePosition ) )
         if( UpArrowSelected( MousePosition ) )
         {
            MoveCursorUp();
            return true;
         }
         return false;
      }

      bool ClickDownArrow( Structs::fPoint MousePosition )
      {
//         if( CheckCollide( DownArrowRect(), GLMousePosition ) )
         if( DownArrowSelected( MousePosition ) )
         {
            MoveCursorDown();
            return true;
         }
         return false;
      }

      bool ClickUpArrow( Structs::Point MousePosition )
      {
//         if( CheckCollide( UpArrowRect(), GLMousePosition ) )
         if( UpArrowSelected( MousePosition ) )
         {
            MoveCursorUp();
            return true;
         }
         return false;
      }

      bool ClickDownArrow( Structs::Point MousePosition )
      {
//         if( CheckCollide( DownArrowRect(), GLMousePosition ) )
         if( DownArrowSelected( MousePosition ) )
         {
            MoveCursorDown();
            return true;
         }
         return false;
      }

      bool ClickRow( Structs::fPoint MousePosition )
      {
         int size = ( m_NumberOfStringsToDraw > m_Strings.size() ) ? m_Strings.size() : m_NumberOfStringsToDraw;
		 std::vector<Structs::fRect> rows( size );
         float h = m_FirstVisableRowRect.top-m_FirstVisableRowRect.bottom;
         Structs::fRect row_rect     = m_FirstVisableRowRect;
         for( int i = 0; i < size; i++ )
         {
//            if( CheckCollide( row_rect, point ) )
            if( SelecteRow( MousePosition ) )
            {
//               m_SelectedIndex = m_VerticalIndex+i;
               return true;
            }
            row_rect.top    -= h;
            row_rect.bottom -= h;
         }
         return false;
      }

      bool ClickRow( Structs::Point MousePosition )
      {
         int size = ( m_NumberOfStringsToDraw > m_Strings.size() ) ? m_Strings.size() : m_NumberOfStringsToDraw;
		 std::vector<Structs::fRect> rows( size );
         float h = m_FirstVisableRowRect.top-m_FirstVisableRowRect.bottom;
         Structs::fRect row_rect     = m_FirstVisableRowRect;
         for( int i = 0; i < size; i++ )
         {
//            if( CheckCollide( row_rect, point ) )
            if( SelecteRow( MousePosition ) )
            {
//               m_SelectedIndex = m_VerticalIndex+i;
               return true;
            }
            row_rect.top    -= h;
            row_rect.bottom -= h;
         }
         return false;
      }

      Structs::fRect GetFirstVisableRowRect() const
      {
         return m_FirstVisableRowRect;
      }

      void MouseHoverArrowUp( void )
      {
         m_bMouseHoveringOnUpArrow = true;
      }
      void MouseHoverOffArrowUp( void )
      {
         m_bMouseHoveringOnUpArrow = false;
      }

      void MouseHoverArrowDown( void )
      {
         m_bMouseHoveringOnDownArrow = true;
      }
      void MouseHoverOffArrowDown( void )
      {
         m_bMouseHoveringOnDownArrow = false;
      }

      // Do not call this function before the rendering context is setup
      void SetWidths( std::list< std::string >& new_string )
      {
         std::list<std::string>::iterator temp = new_string.begin();
         short counter = 1;
         for(; temp != new_string.end(); temp++, counter++ )
         {
            if( m_Widths.size() >= counter )
            {
               if( m_OutLinedFont.Length( *temp ) > m_Widths[counter-1] )
               {
                  m_Widths[counter-1] = m_OutLinedFont.Length( *temp )+m_OutLinedFont.Length( " " );
               }
            }
            else
            {
               m_Widths.push_back( m_OutLinedFont.Length( *temp ) );
            }
         }
      }

      bool AddRow( std::list< std::string > &new_string )
      {
         m_Strings.push_back( new_string );
         SetWidths( new_string );
         return true;
      }


      void SetHeader( std::list< std::string > &header )
      {
         m_ListHeader = header;
         SetWidths( header );
      }

      std::list< std::string > GetCurrentRow( void )
      {
         return (m_Strings.size()) ? m_Strings[m_VerticalIndex] : std::list< std::string >();
      }

      Structs::fRect UpArrowRect( void )
      {
         return m_Arrow1Rect;
      }

      Structs::fRect DownArrowRect( void )
      {
         return m_Arrow2Rect;
      }

      void SetArrowTexture( GLuint ArrowTexture )
      {
         m_ArrowTexture = ArrowTexture;
      }

      void SetTitle( const std::string& title )
      {
         m_Title = title;
      }

      void DrawArrows( const float x, const float y, const float _y, const float _z );

      void DrawUpArrow( const float x, const float y, const float _y, const float _z );
      void DrawDownArrow( const float x, const float y, const float _y, const float _z );

      void DrawRow( const float c, const float _x, const float y, const float _z, const float length, const float h );
      void Draw( const float _x, const float _y, const float _z );

      bool UpArrowMouseOver( Structs::Point MousePosition );
      bool DownArrowMouseOver( Structs::Point MousePosition );

      void Validate( void )
      {
         if( m_VerticalIndex < 0 )
         {
            m_VerticalIndex = 0;
         }
         if( m_VerticalIndex >= m_Strings.size() )
         {
            m_VerticalIndex = m_Strings.size()-1;
         }

         if( m_SelectedIndex < 0 )
         {
            m_SelectedIndex = 0;
         }
         if( m_SelectedIndex >= m_Strings.size() )
         {
            m_SelectedIndex = m_Strings.size()-1;
         }
      }

      bool MoveCursorUp( void )
      {
         m_VerticalIndex--;
         Validate();
         return true;
      }

      bool MoveCursorDown( void )
      {
         m_VerticalIndex++;
         if( (m_VerticalIndex+m_NumberOfStringsToDraw) > m_Strings.size() )
         {
            m_VerticalIndex = m_Strings.size() - m_NumberOfStringsToDraw;
         }
         Validate();
         return true;
      }

      void SetUpArrowTextCoordsOrder( Structs::Point TextCoords[4] )
      {
         m_UpArrowTextCoords[0] = TextCoords[0];
         m_UpArrowTextCoords[1] = TextCoords[1];
         m_UpArrowTextCoords[2] = TextCoords[2];
         m_UpArrowTextCoords[3] = TextCoords[3];
      }

      void SetDownArrowTextCoordsOrder( Structs::Point TextCoords[4] )
      {
         m_DownArrowTextCoords[0] = TextCoords[0];
         m_DownArrowTextCoords[1] = TextCoords[1];
         m_DownArrowTextCoords[2] = TextCoords[2];
         m_DownArrowTextCoords[3] = TextCoords[3];
      }

      void SetUpArrowTextCoordsOrder( std::deque<Structs::Point> TextCoords )
      {
         m_UpArrowTextCoords[0] = TextCoords[0];
         m_UpArrowTextCoords[1] = TextCoords[1];
         m_UpArrowTextCoords[2] = TextCoords[2];
         m_UpArrowTextCoords[3] = TextCoords[3];
      }

      void SetDownArrowTextCoordsOrder( std::deque<Structs::Point> TextCoords )
      {
         m_DownArrowTextCoords[0] = TextCoords[0];
         m_DownArrowTextCoords[1] = TextCoords[1];
         m_DownArrowTextCoords[2] = TextCoords[2];
         m_DownArrowTextCoords[3] = TextCoords[3];
      }

   protected:
      std::vector< std::list< std::string > > m_Strings; // the rows and cells

      std::vector< float >       m_Widths;

      std::list< std::string >   m_ListHeader;

      std::string                m_Title;

      bool                       m_bMouseHoveringOnUpArrow,
                                 m_bMouseHoveringOnDownArrow,
                                 m_bWrap;

      short                      m_NumberOfStringsToDraw,
                                 m_VerticalIndex,         // the top of the currently visable rows
                                 m_SelectedIndex;         // the selected row

      C_GLTextureMappedFont&     m_OutLinedFont;

      Structs::Point           m_DownArrowTextCoords[4];
      Structs::Point           m_UpArrowTextCoords[4];
      Structs::dPoint3D        m_Position, 
                                 m_DownArrowPosition,
                                 m_UpArrowPosition;

      GLuint                     m_ArrowTexture;
      float                      m_ArrowUpWidth,
                                 m_ArrowDownWidth;
      Structs::fRect           m_Arrow1Rect,
                                 m_Arrow2Rect,
                                 m_FirstVisableRowRect;

   private:
};

#endif

