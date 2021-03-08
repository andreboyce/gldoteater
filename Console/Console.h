#pragma once

#ifndef __Console_H
#define __Console_H

#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "../errorutil/errorutil.h"
#include "../TimeInterval/TimeInterval.h"

class Console
{
   friend class C_MessageQueue;
   public:

//      typedef TCHAR               u_char;
//      typedef std::string<u_char> std::string;

      Console( void ) :
         m_linesaddedindex( 0 ),
         m_maxsize( 10 ),
         m_Strings( 10, std::string() ),
         m_linelength( 100 ),
         m_LinesEverAdded( long() ),
         m_Cursor( 0 ),
         m_VerticalCursor( 0 ),
         m_OffSetFromCursor( 0 ),
         m_bLog( false ),
         m_Names( 2, std::string() ),
         m_FileName( "console.log.txt" ),
         m_Flash( true, 500 )
      {
         m_Strings.reserve( m_maxsize );
      }

      Console( unsigned short maxsize, unsigned short linelength=100, std::string filename = "console.log.txt" ) : 
         m_maxsize( maxsize ),
         m_Strings( maxsize, std::string() ),
         m_linesaddedindex( 0 ),
         m_linelength( linelength ),
         m_LinesEverAdded( long() ),
         m_Cursor( 0 ),
         m_VerticalCursor( 0 ),
         m_OffSetFromCursor( 0 ),
         m_bLog( false ),
         m_Names( 2, std::string() ),
         m_FileName( filename ),
         m_Flash( true, 500 )
      {
         m_Strings.reserve( m_maxsize );
      }

      ~Console( void ){}

      C_Flash& GetFlash()
      {
         return m_Flash;
      }

      void EnableLogging( void )
      {
         m_bLog = true;
      }
 
      void DisableLogging( void )
      {
         m_bLog = false;
      }

      bool isLogging( void ) const
      {
         return m_bLog;
      }

      int FindString( std::string str )
      {
         int index = 0;
         for( ; index < m_Strings.size() ; index++ )
         {
            if( m_Strings[index] == str )
            {
               return index;
            }
         }
         return -1;
      }

      bool ReadFile( std::string name )
      {
         try
         {
            std::ifstream fin( name.c_str(), std::ios::in );
            if( !fin.is_open() )
               return false;
            std::string tmp;
            while( !std::getline( fin, tmp ).eof() )
            {
               AddLine( tmp );
            }
         }
         catch( ... )
         {
            return false;
         }
         return true;
      }

      bool EraseLog( void )
      {
         std::ofstream of( m_FileName.c_str(), std::ios::out );
         if( !of.is_open() )
            return false;
         of.close();
         return true;
      }

      bool LogThisLine( std::string line, std::string name = "" )
      {
         if( m_bLog )
         {
            std::ofstream log( m_FileName.c_str(), std::ios::app );
            if( !log.is_open() )
            {
               //Alert( "Could not open file" + file_name + " for appending." );
               return false;
            }
            short bufferlen = ::GetTimeFormat( (LCID)NULL, 0, NULL, NULL, NULL, 0 );
            std::vector<char> time(bufferlen);
            ::GetTimeFormat( (LCID)NULL, 0, NULL, NULL, &time[0], bufferlen );
            log << "[" << &time[0] << "]" << name << ": " << line.c_str() << std::endl;
            log.close();
         }
         return true;
      }

      bool LogLine( std::string name )
      {
         if( m_bLog )
         {
            std::ofstream log( m_FileName.c_str(), std::ios::app );
            if( !log.is_open() )
            {
               //Alert( "Could not open file" + file_name + " for appending." );
               return false;
            }
            short bufferlen = ::GetTimeFormat( (LCID)NULL, 0, NULL, NULL, NULL, 0 );
            std::vector<char> time(bufferlen);
            ::GetTimeFormat( (LCID)NULL, 0, NULL, NULL, &time[0], bufferlen );      
            log << "[" << &time[0] << "]" << name << ": " << GetLastLineAdded().c_str() << std::endl;
            log.close();
         }
         return true;
      }

      void CheckVerticalCursorBounds( void )
      {
         if( m_VerticalCursor > m_linesaddedindex )
            m_linesaddedindex = m_linesaddedindex;
         if( m_VerticalCursor >= m_Strings.size() )
            m_VerticalCursor = m_Strings.size()-1;
         if( m_VerticalCursor < 0 )
            m_VerticalCursor = 0;
      }

      short VerticalCursorUp( void )
      {
         --m_VerticalCursor;
         CheckVerticalCursorBounds();
         return m_VerticalCursor;
      }

      short VerticalCursorDown( void )
      {
         ++m_VerticalCursor;
         CheckVerticalCursorBounds();
         return m_VerticalCursor;
      }

      std::string GetLineAtVerticalCursor( void )
      {
         CheckVerticalCursorBounds();
         return m_Strings.at( m_VerticalCursor );
      }

      std::string GetStringBehindCursor( void )
      {
         if( m_Cursor > GetCurrentLine().size() ) m_Cursor = GetCurrentLine().size();
         return GetCurrentLine().substr( 0, m_Cursor );
      }

      void IncrementOffSet( void )
      {
         m_OffSetFromCursor++;
         if( m_OffSetFromCursor < 0 )
         {
            if( m_Cursor < abs( m_OffSetFromCursor ) )
               m_OffSetFromCursor = m_Cursor*-1;
         }
         else
         {
            if( m_Cursor+m_OffSetFromCursor >= GetCurrentLine().size() )
               m_OffSetFromCursor = GetCurrentLine().size()-m_Cursor;
         }
      }

      void DecrementOffSet( void )
      {
         m_OffSetFromCursor--;
         if( m_OffSetFromCursor < 0 )
         {
            if( m_Cursor < abs( m_OffSetFromCursor ) )
               m_OffSetFromCursor = m_Cursor*-1;
         }
         else
         {
            if( m_Cursor+m_OffSetFromCursor >= GetCurrentLine().size() )
               m_OffSetFromCursor = GetCurrentLine().size()-m_Cursor;
         }
      }

      std::string SelectText( void )
      {
         int pos = 0, offset = 0;

         if( !m_OffSetFromCursor )
            return std::string();

         if( m_OffSetFromCursor > 0 )
         {
            pos    = m_Cursor;
            offset = m_Cursor+m_OffSetFromCursor;
         }
         else
         {
            pos    = m_Cursor+m_OffSetFromCursor; // offset is negative
            offset = m_Cursor;
         }

         return GetCurrentLine().substr( pos, offset );
      }

      void Reset( void )
      {
         m_Cursor           = 0;
         m_OffSetFromCursor = 0;
         for( int i=0; i<m_maxsize; ++i )
         {
            m_Strings.at(i) = "";
         }
      }

      unsigned short GetCursor( void ) const
      {
         return m_Cursor;
      }

// dosent work quite right but no biggie
      void MoveCursorHomeRight( void )
      {
         bool bNonSpaceCharFound = true;
         
         if( m_Cursor < GetCurrentLine().size() )
            bNonSpaceCharFound = (GetCurrentLine()[m_Cursor] != ' ') ? true : false;
         else if( m_Cursor >= 0 )
            bNonSpaceCharFound = (GetCurrentLine()[m_Cursor-1] != ' ') ? true : false;

         while( (GetCurrentLine()[m_Cursor] != ' ') && (m_Cursor <= GetCurrentLine().size()) && bNonSpaceCharFound )
         {
            m_Cursor++;
         }
      }

// dosent work quite right but no biggie
      void MoveCursorHomeLeft( void )
      {
         bool bNonSpaceCharFound = true;
         if( m_Cursor < GetCurrentLine().size() )
            bNonSpaceCharFound = (GetCurrentLine()[m_Cursor] != ' ') ? true : false;
         else if( m_Cursor >= 0 )
            bNonSpaceCharFound = (GetCurrentLine()[m_Cursor-1] != ' ') ? true : false;

         while( (GetCurrentLine()[m_Cursor] != ' ') && (m_Cursor >= 0) && bNonSpaceCharFound )
         {
            m_Cursor--;
         }
      }

      void MoveCursorRight( void )
      {
         m_Cursor++;
         if( m_Cursor > GetCurrentLine().size() )
         {
            m_Cursor = GetCurrentLine().size();
         }
      }

      void MoveCursorLeft( void )
      {
         m_Cursor--;
         if( m_Cursor < 0 )
         {
            m_Cursor = 0;
         }
      }

      void MoveCursorHome( void )
      {
         m_Cursor = 0;
      }

      void MoveCursorEnd( void )
      {
         m_Cursor = GetCurrentLine().size();
      }

      void SetCursos( unsigned short new_pos )
      {
         if( new_pos < GetCurrentLine().size() )
         {
            m_Cursor = new_pos;
         }
      }

      long GetTotalLinesEverAdded( void ) const
      {
         return m_LinesEverAdded;
      }

      std::string GetCurrentLine( void ) const
      {
         return m_Strings.at( m_linesaddedindex );
      }

      std::string GetLastLineAdded( void )
      {
         int index = m_linesaddedindex;
         if( index < 0 )
            index = 1;
         return m_Strings[ index-1 ];
      }

      void Clear( void )
      {
         Reset();
      }

      unsigned short LinesAdded( void )
      {
//         if( m_linesaddedindex >= m_Strings.size() )
//            m_linesaddedindex = ( m_Strings.size() ) ? m_Strings.size()-1 : 0;
         return m_linesaddedindex;
      }

      void EndLine( std::string Name = "" )
      {
         AddChar( '\r' );
         if( m_bLog )
         {
            LogLine( Name );
         }
      }

      void DeleteChar( void )
      {
        if( m_Cursor >=0 )
        {
           if( m_Cursor != m_Strings.at( m_linesaddedindex ).size() )
           {
              if( m_Cursor < m_Strings.at( m_linesaddedindex ).size() )
              {
                 if( m_Strings.at( m_linesaddedindex ).size() > 0 )
                    m_Strings.at( m_linesaddedindex ).erase( m_Cursor, 1 );
              }
           }

           if( (m_Cursor-1) < 0 ) m_Cursor = 0;
           if( m_Cursor > m_Strings.at( m_linesaddedindex ).size() )
              m_Cursor = m_Strings.at( m_linesaddedindex ).size();
        }
      }

      void AddString( std::string text )
      {
         if( text.size() )
         {
            for( int i=0; i<text.size(); i++ )
            {
               AddChar( text[i] );
            }
         }
      }

      void SetCurrentLine( std::string text )
      {
         m_Strings.at( m_linesaddedindex ) = text;
         MoveCursorEnd();
      }

      bool AddChar( char character )
      {
         if( m_Cursor <= GetCurrentLine().size() && ( m_Cursor >= 0 ) )
         {
            if( (character != '\r') && (character != '\n') && (character != '\0') )
            {
               if( (character == '\b') )
               {
                  BackSpaceChar();
               }
               else
               {
                  if( (GetCurrentLine().size()+1) < m_linelength )
                  {
                     std::string::iterator index = m_Strings.at( m_linesaddedindex ).begin()+m_Cursor;
                     m_Strings.at( m_linesaddedindex ).insert( index, character );   // add the character
                     ++m_Cursor;
                  }
                  else
                  {
                     return false; // the character does not fit on the line
                  }
               }
            }
            else
            {
               ++m_linesaddedindex;
               ++m_LinesEverAdded;
               m_Cursor = 0;
            }
            if( m_linesaddedindex >= m_maxsize )
            {
               pop_backLine();
            }
         }
         else
         {
            return false; // cursor error
         }
         return true;
      }

      bool AddLine( long newline, std::string name = "" )
      {
         std::ostringstream tmp;
         tmp << newline;
         return AddLine( tmp.str(), name );
      }

      bool AddLine( double newline, std::string name = "" )
      {
         std::ostringstream tmp;
         tmp << newline;
         return AddLine( tmp.str(), name );
      }

      bool AddLine( std::ostringstream newline, std::string name = "" )
      {
         return AddLine( newline.str(), name );
      }

      bool AddLine( const char* newline, std::string name = "" )
      {
         return AddLine( std::string( newline ), name );
      }

      bool AddLine( std::string newline, std::string name = "" )
      {
         std::string tmp = GetCurrentLine();
         if( newline.size() > m_linelength )
         {
            for( int begin = 0;
                 begin < newline.size() ;
                 begin += m_linelength )
            {
               AddVerifiedLine( newline.substr( begin , m_linelength ), name );
            }
         }
         else
         {
            bool ret = AddVerifiedLine( newline, name );
            SetCurrentLine( tmp );
            return ret;
         }
         return false;
      }

      void pop_backLine( void )
      {
         m_Strings.erase( m_Strings.begin() ); // erase first line
         m_Strings.push_back( std::string() ); // add a black line to the end
         if( m_linesaddedindex > 0 ) --m_linesaddedindex;
      }

      std::string operator [] ( unsigned short index )
      {
         return m_Strings[ index ];
      }

      operator std::vector<std::string>&( void )
      {
         return m_Strings;
      }

   protected:

      std::vector< std::string > m_Strings;
      unsigned short          m_maxsize;
      unsigned short          m_linesaddedindex;
      unsigned short          m_linelength;
      short                   m_Cursor;
      short                   m_OffSetFromCursor;
      short                   m_VerticalCursor;
      long                    m_LinesEverAdded;
      std::ostringstream      m_templine;
      bool                    m_bLog;
      std::vector< std::string > m_Names;
      std::string                m_FileName;
      C_Flash                 m_Flash;


      void BackSpaceChar( void )
      {
        if( m_Cursor >=1 )
        {
           if( m_Cursor == m_Strings.at( m_linesaddedindex ).size() )
           {
              m_Strings.at( m_linesaddedindex ).erase( m_Cursor-1, 1 );
           }
           else
           {
              if( m_Strings.at( m_linesaddedindex ).size() > 0 )
                 m_Strings.at( m_linesaddedindex ).erase( ((m_Cursor-1)>=0)?(m_Cursor-1):0, 1 );
              --m_Cursor;
           }

           if( (m_Cursor-1) < 0 ) m_Cursor = 0;
           if( m_Cursor > m_Strings.at( m_linesaddedindex ).size() )
              m_Cursor = m_Strings.at( m_linesaddedindex ).size();
        }
      }

      bool AddVerifiedLine( std::string newline, std::string name = "" )
      {
//         Alert( name );
         m_Strings.at( m_linesaddedindex ) = newline;  // add the ned line
         ++m_linesaddedindex;
         ++m_LinesEverAdded;
         if( m_bLog )
         {
            LogLine( name );
         }
         if( m_linesaddedindex >= m_maxsize ) // only erase when the index goes out of range
         {
            pop_backLine();
         }
         m_Cursor = 0;
         return true;
      }

   private:
};

class C_MessageQueue
{
   public:
      C_MessageQueue( DWORD duration = 500, unsigned int lines_to_add = 3 ) :
         m_bAddLine( true )
      {
         for( int i=0; i<lines_to_add ; i++ )
         {
            AddLine( "" );
         }
         SetDuration( duration );
      }
      ~C_MessageQueue(){}

      void AddLine( std::string msg )
      {
         m_Console.AddLine( msg );
      }

      operator std::vector<std::string>&( void )
      {
         return m_Console.m_Strings;
      }

      void clear()
      {
         m_Console.Clear();
      }

      unsigned short LinesAdded()
      {
         return m_Console.LinesAdded();
      }

      void Update()
      {
         // add a line ever couple of miliseconds as specified by duration
         if( m_Console.GetFlash().Update() && m_bAddLine )
         {
            AddLine( "" );
            m_bAddLine = false;
         }
         else
         {
            if( !m_Console.GetFlash().Update() )
               m_bAddLine = true;
         }
      }

      void SetDuration( DWORD duration )
      {
         m_Console.GetFlash().SetDuration( duration );
      }

      Console m_Console;
      bool      m_bAddLine;
   private:
   protected:
};

#endif

