#pragma once

#ifndef __WinHFont_H
#define __WinHFont_H

#include <windows.h>
#include <string>

class WinHFont
{
   public:

      struct S_FontProperties
      {
         int nHeight;              // logical height of font 
         int nWidth;               // logical average character width 
         int nEscapement;          // angle of escapement 
         int nOrientation;         // base-line orientation angle 
         int fnWeight;             // font weight 
         DWORD fdwItalic;          // italic attribute flag 
         DWORD fdwUnderline;       // underline attribute flag 
         DWORD fdwStrikeOut;       // strikeout attribute flag 
         DWORD fdwCharSet;         // character set identifier 
         DWORD fdwOutputPrecision; // output precision 
         DWORD fdwClipPrecision;   // clipping precision 
         DWORD fdwQuality;         // output quality 
         DWORD fdwPitchAndFamily;  // pitch and family 
         LPCTSTR lpszFace;         // pointer to typeface name string 

         S_FontProperties( void ) :         
            nHeight            ( 0 ),
            nWidth             ( 0 ),
            nEscapement        ( 0 ),
            nOrientation       ( 0 ),
            fnWeight           ( 0 ),
            fdwItalic          ( 0 ),
            fdwUnderline       ( 0 ),
            fdwStrikeOut       ( 0 ),
            fdwCharSet         ( 0 ),
            fdwOutputPrecision ( 0 ),
            fdwClipPrecision   ( 0 ),
            fdwQuality         ( 0 ),
            fdwPitchAndFamily  ( 0 ),
            lpszFace( NULL )
         {         
         }

         S_FontProperties( int _nHeight, int _nWidth, int _nEscapement, int _nOrientation,
                           int _fnWeight, DWORD _fdwItalic, DWORD _fdwUnderline, DWORD _fdwStrikeOut,
                           DWORD _fdwCharSet, DWORD _fdwOutputPrecision, DWORD _fdwClipPrecision,
                           DWORD _fdwQuality, DWORD _fdwPitchAndFamily )
         :
            nHeight            ( _nHeight ),
            nWidth             ( _nWidth ),
            nEscapement        ( _nEscapement ),
            nOrientation       ( _nOrientation ),
            fnWeight           ( _fnWeight ),
            fdwItalic          ( _fdwItalic ),
            fdwUnderline       ( _fdwUnderline ),
            fdwStrikeOut       ( _fdwStrikeOut ),
            fdwCharSet         ( _fdwCharSet ),
            fdwOutputPrecision ( _fdwOutputPrecision ),
            fdwClipPrecision   ( _fdwClipPrecision ),
            fdwQuality         ( _fdwQuality ),
            fdwPitchAndFamily  ( _fdwPitchAndFamily ),
            lpszFace( NULL )
         {         
         }

         S_FontProperties( const S_FontProperties& FontPropertiesRHS ) :
            nHeight            ( FontPropertiesRHS.nHeight),
            nWidth             ( FontPropertiesRHS.nWidth),
            nEscapement        ( FontPropertiesRHS.nEscapement),
            nOrientation       ( FontPropertiesRHS.nOrientation),
            fnWeight           ( FontPropertiesRHS.fnWeight),
            fdwItalic          ( FontPropertiesRHS.fdwItalic),
            fdwUnderline       ( FontPropertiesRHS.fdwUnderline),
            fdwStrikeOut       ( FontPropertiesRHS.fdwStrikeOut),
            fdwCharSet         ( FontPropertiesRHS.fdwCharSet),
            fdwOutputPrecision ( FontPropertiesRHS.fdwOutputPrecision),
            fdwClipPrecision   ( FontPropertiesRHS.fdwClipPrecision),
            fdwQuality         ( FontPropertiesRHS.fdwQuality),
            fdwPitchAndFamily  ( FontPropertiesRHS.fdwPitchAndFamily),
            lpszFace( NULL )
         {
            if( FontPropertiesRHS.lpszFace != NULL )
            {
//               lpszFace = new char[ strlen(FontPropertiesRHS.lpszFace)+1 ];
//               ::strncpy( (char*)lpszFace, FontPropertiesRHS.lpszFace, strlen(FontPropertiesRHS.lpszFace) );
//               ((char*)lpszFace)[ strlen(FontPropertiesRHS.lpszFace) ] = '\0';
                int len = strlen(FontPropertiesRHS.lpszFace)+1;
                lpszFace = new char[ len ];
                strncpy( (char*)lpszFace, FontPropertiesRHS.lpszFace, len );
            }
         }

         ~S_FontProperties( void )
         {
            delete[] lpszFace;
         }

         S_FontProperties operator = ( const S_FontProperties& FontPropertiesRHS )
         {
            nHeight            = FontPropertiesRHS.nHeight;
            nWidth             = FontPropertiesRHS.nWidth;
            nEscapement        = FontPropertiesRHS.nEscapement;
            nOrientation       = FontPropertiesRHS.nOrientation;
            fnWeight           = FontPropertiesRHS.fnWeight;
            fdwItalic          = FontPropertiesRHS.fdwItalic;
            fdwUnderline       = FontPropertiesRHS.fdwUnderline;
            fdwStrikeOut       = FontPropertiesRHS.fdwStrikeOut;
            fdwCharSet         = FontPropertiesRHS.fdwCharSet;
            fdwOutputPrecision = FontPropertiesRHS.fdwOutputPrecision;
            fdwClipPrecision   = FontPropertiesRHS.fdwClipPrecision;
            fdwQuality         = FontPropertiesRHS.fdwQuality;
            fdwPitchAndFamily  = FontPropertiesRHS.fdwPitchAndFamily;
            delete [] lpszFace;
            if( FontPropertiesRHS.lpszFace != NULL )
            {
//               lpszFace = new char[ strlen(FontPropertiesRHS.lpszFace)+1 ];
//               ::strncpy( (char*)lpszFace, FontPropertiesRHS.lpszFace, strlen(FontPropertiesRHS.lpszFace) );
//               ((char*)lpszFace)[ strlen(FontPropertiesRHS.lpszFace) ] = '\0';
                int len = strlen(FontPropertiesRHS.lpszFace)+1;
                lpszFace = new char[ len ];
                strncpy( (char*)lpszFace, FontPropertiesRHS.lpszFace, len );
            }
            return *this;
         }
      };

       WinHFont( void );
      ~WinHFont( void );
       void ReCreate( void );

       void Height( int Height, bool bReCreate = true )
       {  m_FontProperties.nHeight = Height;
          if( bReCreate ){ ReCreate(); }
       }

       void Width( int Width, bool bReCreate = true )
       {  m_FontProperties.nWidth = Width;
          if( bReCreate ){ ReCreate(); }
       }

       void Escapement( int Escapement, bool bReCreate = true )
       {  m_FontProperties.nEscapement = Escapement;
          if( bReCreate ){ ReCreate(); }
       }

       void Orientation( int Orientation, bool bReCreate = true )
       {  m_FontProperties.nOrientation = Orientation;
          if( bReCreate ){ ReCreate(); }
       }

       void Weight( int Weight, bool bReCreate = true )
       {  m_FontProperties.fnWeight = Weight;
          if( bReCreate ){ ReCreate(); }
       }

       void Italic( DWORD Italic, bool bReCreate = true )
       {  m_FontProperties.fdwItalic = Italic;
          if( bReCreate ){ ReCreate(); }
       }

       void Underline( DWORD Underline, bool bReCreate = true )
       {  m_FontProperties.fdwUnderline = Underline;
          if( bReCreate ){ ReCreate(); }
       }

       void StrikeOut( DWORD StrikeOut, bool bReCreate = true )
       {  m_FontProperties.fdwStrikeOut = StrikeOut;
          if( bReCreate ){ ReCreate(); }
       }

       void CharSet( DWORD CharSet, bool bReCreate = true )
       {  m_FontProperties.fdwCharSet = CharSet;
          if( bReCreate ){ ReCreate(); }
       }

       void OutputPrecision( DWORD OutputPrecision, bool bReCreate = true )
       {  m_FontProperties.fdwOutputPrecision = OutputPrecision;
          if( bReCreate ){ ReCreate(); }
       }

       void ClipPrecision( DWORD ClipPrecision, bool bReCreate = true )
       {  m_FontProperties.fdwClipPrecision = ClipPrecision;
          if( bReCreate ){ ReCreate(); }
       }

       void Quality( DWORD Quality, bool bReCreate = true )
       {  m_FontProperties.fdwQuality = Quality;
          if( bReCreate ){ ReCreate(); }
       }

       void PitchAndFamily( DWORD PitchAndFamily, bool bReCreate = true )
       {  m_FontProperties.fdwPitchAndFamily = PitchAndFamily;
          if( bReCreate ){ ReCreate(); }
       }

       void Face( LPCTSTR Face, bool bReCreate = true )
       {  //m_FontProperties.lpszFace = Face;                      
          //            ::strcpy( lpszFace , FontPropertiesRHS.lpszFace );
          if( m_FontProperties.lpszFace )
             delete [] m_FontProperties.lpszFace;
          m_FontProperties.lpszFace = new char[ sizeof(Face) ];
          if( bReCreate ){ ReCreate(); }
       }


      operator HFONT ( void ) const
      {
         return m_HFont;
      }

      void Create( void );
      void Create( const S_FontProperties & FontProperties );

   protected:
      void Destroy( void );

      HFONT            m_HFont;
      S_FontProperties m_FontProperties;

   private:
};

#endif

