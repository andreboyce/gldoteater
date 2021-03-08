#pragma once

#ifndef __C_GLButton_H
#define __C_GLButton_H

#include <windows.h>
#include <gl/glu.h>
#include "../../errorutil/errorutil.h"
#include "../../Structs/Structs.h"
#include "../../MathUtility/MathUtility.h"
#include "../C_GLTexQuad/C_GLTexQuad.h"
#include "../C_GLDisplayLists/C_GLDisplayLists.h"
#include "../OpenGLUtility/OpenGLUtility.h"
#include <string>

/*
   Spec:
   
   
*/

class C_GLButton
{
   enum
   {
      MOUSEON,
      MOUSEOFF,
   };

   public:

      typedef void (*CallBack)(void*);
      struct CallBackAndArgument
      {
         CallBackAndArgument() : m_function( NULL ), m_Argument( NULL ) {}
         CallBackAndArgument( CallBack function, void *Argument ) :
            m_function( function ), m_Argument( Argument )
         {}
         void call( void )
         {
            if( m_function )
            {
               m_function( m_Argument );
            }
         }
         void Set( CallBack function, void *Argument )
         {
            m_function = function;
            m_Argument = Argument;
         }
         CallBack m_function;
         void     *m_Argument;
      };

      C_GLButton(): m_MouseOverSize( 6.0, 6.0 ), m_MouseOffSize( 5.0 , 5.0 )
      {
         m_Rect          = m_TexQuad.Rect();
         m_Rect.left     += m_Position.m_x;
         m_Rect.top      += m_Position.m_y;
         m_Rect.right    += m_Position.m_x;
         m_Rect.bottom   += m_Position.m_y;
         m_TexQuad.SetSize( m_MouseOffSize.m_x, m_MouseOffSize.m_y );

         m_TexCoords[0].U = 0; m_TexCoords[0].V = 0;
         m_TexCoords[1].U = 1; m_TexCoords[1].V = 0;
         m_TexCoords[2].U = 1; m_TexCoords[2].V = 1;
         m_TexCoords[3].U = 0; m_TexCoords[3].V = 1;
         mouseon = false;
      }
      ~C_GLButton(){}

      void SetTextCoords( C_TexCoord TextCoords[4] )
      {
         m_TexCoords[0] = TextCoords[0];
         m_TexCoords[1] = TextCoords[1];
         m_TexCoords[2] = TextCoords[2];
         m_TexCoords[3] = TextCoords[3];
         BuildQuadList();
      }

      void BuildQuadList( void );

      // Texture Mapping should be enabled when calling this
      void Draw( void )
      {
         m_TexQuadList.call();
      }

      bool OnClick( const Structs::Point MousePosition );
      bool OnMouseOver( const Structs::Point MousePosition );

      bool OnClick( const Structs::dPoint3D MousePosition );
      bool OnMouseOver( const Structs::dPoint3D MousePosition );


      void SetPosition( const Structs::dPoint3D& Point )
      {
         m_Position = Point;
      }

      void SetPosition( const float x, const float y, const float z )
      {
         m_Position.m_x = x;
         m_Position.m_y = y;
         m_Position.m_z = z;
         BuildQuadList();
      }

      void SetSize( Structs::fPoint MouseOverSize, Structs::fPoint MouseOffSize )
      {
         m_MouseOverSize = MouseOverSize;
         m_MouseOffSize  = MouseOffSize;
         m_TexQuad.SetSize( m_MouseOffSize.m_x, m_MouseOffSize.m_y );
         m_Rect          = m_TexQuad.Rect();
         m_Rect.m_z      = m_Position.m_z;
         m_Rect.left     += m_Position.m_x;
         m_Rect.top      += m_Position.m_y;
         m_Rect.right    += m_Position.m_x;
         m_Rect.bottom   += m_Position.m_y;
         BuildQuadList();
      }

      void SetClickCallBack( CallBackAndArgument clickcallback )
      {
         m_ClickCallBack = clickcallback;
      }

      void SetTexture( GLuint Texture )
      {
         m_Texture = Texture;
         BuildQuadList();
      }

      Structs::fRect3D  Rect( void ) const
      {
         return m_Rect;
      }

      bool ButtonSelected( Structs::Point MousePosition );
	  bool ButtonSelected( Structs::dPoint3D MousePosition );

	  void SetTextCoordsOrder( C_TexCoord TexCoords[4] )
	  {
		  m_TexCoords[0] = TexCoords[0];
		  m_TexCoords[1] = TexCoords[1];
		  m_TexCoords[2] = TexCoords[2];
		  m_TexCoords[3] = TexCoords[3];
	  }

	  void SetTextCoordsOrder( Structs::Point TexCoords[4] )
	  {
		  m_TexCoords[0] = TexCoords[0];
		  m_TexCoords[1] = TexCoords[1];
		  m_TexCoords[2] = TexCoords[2];
		  m_TexCoords[3] = TexCoords[3];
	  }

   protected:
      Structs::fRect3D  m_Rect;
      Structs::dPoint3D m_Position;

      Structs::fPoint  m_MouseOverSize,
                         m_MouseOffSize;

      C_TexCoord         m_TexCoords[4];
      
      GLuint             m_Texture;
      C_GLTexQuad        m_TexQuad;

      C_GLDisplayList    m_TexQuadList;

      CallBackAndArgument m_ClickCallBack;

      bool CheckCollide( const Structs::fRect3D& Rect, const Structs::dPoint3D& Point )
      {
         if( !IsInRange( m_Rect.m_z, Point.m_z, 3.0 ) )
            return false;
         if( (Rect.bottom > Point.m_y) ) return false;
         if( (Rect.top    < Point.m_y) ) return false;
         if( (Rect.right  < Point.m_x) ) return false;
         if( (Rect.left   > Point.m_x) ) return false;
         return true;
      }

   private:
      bool mouseon;
};

#endif
