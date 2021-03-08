#ifndef __C_GLSprite_H
#define __C_GLSprite_H

#include <vector>
#include <windows.h>

#include "../../../C_CollisionObject/C_CollisionObject.h"
#include "../../C_GLTexQuad/C_GLTexQuad.h"
#include "../../C_GLLine/C_GLLine.h"
#include "../../C_GLTexTriangle/C_GLTexTriangle.h"
#include "../../C_GLPoint/C_GLPoint.h"
#include "C_GLSpriteAnimationSequence/C_GLSpriteAnimationSequence.h"
#include "../../../C_CollosionDetection/C_CollosionDetection.h"


class C_GLSprite
{
   struct S_ColObjs
   {
      public:
          S_ColObjs( void )
             {   }
          S_ColObjs( const S_ColObjs& S_ColObjsRHS )
             {   }
         ~S_ColObjs( void )
             {   }

         C_CollosionDetection::S_Rectangle m_Rect;
         C_CollosionDetection::S_Triangle  m_Tri;
   };
   
   // These 3 classes add 3 new members to The classes they inheret from
   typedef std::vector< C_GLSpriteAnimationSequence > V_Sequences;

   class C_GLTexQuadSprite: public C_GLTexQuad
   {
      public:
         C_GLTexQuadSprite( void )
            {
               x_offset = y_offset = z_offset = 0;
               currentsequence = 0;            
            }

         void AddSequences( short n=1 )
            {
               Sequences.resize( Sequences.size()+n );
            }

         void SetTextureSequence( short index, C_GLSpriteAnimationSequence::V_GLuint& Frames, const float& duration)
         {
            Sequences[index].AddTextureSequence( Frames, duration );
         }
            
         void BindCurrentTexture( DWORD currenttime )
            {
               if( Sequences.size() && ( (currentsequence+1) <= Sequences.size() ) )
               {
                  Sequences[currentsequence].Update( currenttime );
                  Bind( Sequences[currentsequence].GetCurrentTexture() );
               }
               else
               {
                  Bind(); // Bind the texture set manually or the default 0xFFFFFF
               }
            }

         void inline SetCurrentSequence( const short& newcurrentsequence )
            { currentsequence = newcurrentsequence; }

         void DrawAtOffSet( float x, float y, float z )
            { Draw( x+x_offset, y+y_offset, z+z_offset ); }

         void SetOffSet( float x, float y, float z )
            {
               x_offset = x;
               y_offset = y;
               z_offset = z;
            }
      protected:
         float x_offset, y_offset, z_offset;
         V_Sequences         Sequences;
         short               currentsequence;
         
   };

   class C_GLTexTriangleSprite: public C_GLTexTriangle
   {
      public:
         C_GLTexTriangleSprite( void )
            {
               x_offset = y_offset = z_offset = 0;
               currentsequence = 0;            
            }

         void AddSequences( short n=1 )
            {
               Sequences.resize( Sequences.size()+n );
            }
            
         void SetTextureSequence( short index, C_GLSpriteAnimationSequence::V_GLuint& Frames, const float& duration)
         {
            Sequences[index].AddTextureSequence( Frames, duration );
         }

         void BindCurrentTexture( DWORD currenttime )
            {
               if( Sequences.size() && ( (currentsequence+1) <= Sequences.size() ) )
               {
                  Sequences[currentsequence].Update( currenttime );               
                  Bind( Sequences[currentsequence].GetCurrentTexture() );
               }
               else
               {
                  Bind(); // Bind the texture set manually or the default 0xFFFFFF
               }
            }

         void inline SetCurrentSequence( const short& newcurrentsequence )
            { currentsequence = newcurrentsequence; }

         void DrawAtOffSet( float x, float y, float z )
            { Draw( x+x_offset, y+y_offset, z+z_offset ); }
            
         void SetOffSet( float x, float y, float z )
            {
               x_offset = x;
               y_offset = y;
               z_offset = z;
            }

      protected:
         float x_offset, y_offset, z_offset;
         V_Sequences         Sequences;
         short               currentsequence;
   };

   class C_GLLineSprite: public C_GLLine
   {
      public:
         C_GLLineSprite( void )
            {
               x_offset = y_offset = z_offset = 0;
               currentsequence = 0;
            }

         void AddSequences( short n=1 )
            {
               Sequences.resize( Sequences.size()+n );
            }

         void DrawAtOffSet( float x, float y, float z )
            { Draw( x+x_offset, y+y_offset, z+z_offset ); }
            
         void inline SetCurrentSequence( const short& newcurrentsequence )
            { currentsequence = newcurrentsequence; }
            
         void SetOffSet( float x, float y, float z )
            { 
               x_offset = x;
               y_offset = y;
               z_offset = z;
            }            

      protected:
         float x_offset, y_offset, z_offset;
         V_Sequences         Sequences;
         short               currentsequence;
   };
   
   class C_GLPointSprite: public C_GLPoint
   {
      public:
         C_GLPointSprite( void )
            {
               x_offset = y_offset = z_offset = 0;
               currentsequence = 0;
            }
            
         void AddSequences( short n=1 )
            {
               Sequences.resize( Sequences.size()+n );
            }
            
         void DrawAtOffSet( float x, float y, float z )
            { Draw( x+x_offset, y+y_offset, z+z_offset ); }

         void inline SetCurrentSequence( const short& newcurrentsequence )
            { currentsequence = newcurrentsequence; }
         
         void SetOffSet( float x, float y, float z )
            { 
               x_offset = x;
               y_offset = y;
               z_offset = z;
            }

      protected:
         float x_offset, y_offset, z_offset;
         V_Sequences         Sequences;
         short               currentsequence;
   };   
   
   /////////////////////////////////////////////////////////////////////
   
      typedef std::vector< S_ColObjs >              V_CollisionObj;
      typedef std::vector< C_GLTexQuadSprite >      V_TexQuadSprite;
      typedef std::vector< C_GLTexTriangleSprite >  V_TexTriangleSprite;
      typedef std::vector< C_GLLineSprite >         V_LineSprite;
      typedef std::vector< C_GLPointSprite >        V_PointSprite;

   public:

       C_GLSprite( void );
       C_GLSprite( const C_GLSprite& );
      ~C_GLSprite( void );

       //Regular members
       
       void AddQuads( short n=1 )
          {
             TexQuads.resize( TexQuads.size()+n );
          }
          
       void AddLines( short n=1 )
          {
             Lines.resize( Lines.size()+n );
          }

       void AddTriangles( short n=1 )
          {
             TexTriangles.resize( TexTriangles.size()+n );
          }
          
       void AddPoints( short n=1 )
          {
             Points.resize( Points.size()+n );
          }
          
       void AddCollisionObjects( short n=1 )
          {
             CollisionObjs.resize( CollisionObjs.size()+n );
          }

          
       void Draw( float, float, float, DWORD );
       void SetCollisionObjectType( C_CollisionObject );
       
       C_GLSprite& operator = ( const C_GLSprite & C_GLSpriteRHS )
       {
          x_pos = C_GLSpriteRHS.x_pos;
          y_pos = C_GLSpriteRHS.y_pos;

          CollisionObjs   = C_GLSpriteRHS.CollisionObjs;
          TexQuads        = C_GLSpriteRHS.TexQuads;
          TexTriangles    = C_GLSpriteRHS.TexTriangles;
          Lines           = C_GLSpriteRHS.Lines;          
          return *this;
       }

//   protected:
   
      float   x_pos;
      float   y_pos;
      
      V_CollisionObj      CollisionObjs;
      V_TexQuadSprite     TexQuads;
      V_TexTriangleSprite TexTriangles;
      V_LineSprite        Lines;
      V_PointSprite       Points;
      
   private:

};

#endif

