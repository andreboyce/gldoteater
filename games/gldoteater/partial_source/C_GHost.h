#ifndef __C_GHost_H
#define __C_GHost_H

#include "CommonSpriteAttributes.h"

class C_GHost
{
   public:
      C_GHost() :
         m_PositionVector( Vector2D() ),
         m_VelocityVector( Vector2D() ),
         m_TexQuad( C_GLTexQuad() ),
         m_direction( LEFT ),
         m_AStarPath(),
         m_bIsPoweredDown( false ),
         m_bIsEaten( false ),
         m_PoweredDownTime( 0 ),
         m_PowerUpFlash( false, 500 ),
         m_Goal( Structs::Point() ),
         m_Tile( Structs::Point() )
         {}
      C_GHost( Vector2D PositionVector, Vector2D VelocityVector, 
               C_GLTexQuad TexQuad, 
               Direction direction,
               std::vector<AStar::AStar2d::PointAndDirection> AStarPath,
               bool bIsPoweredDown,
               bool bIsEaten,
               float PoweredDownTime,
               C_Flash PowerUpFlash,
               Structs::Point Goal,
               Structs::Point Tile ) :
         m_PositionVector( PositionVector ),
         m_VelocityVector( VelocityVector ),
         m_TexQuad( TexQuad ),
         m_direction( direction ),
         m_AStarPath( AStarPath ),
         m_bIsPoweredDown( bIsPoweredDown ),
         m_bIsEaten( bIsEaten ),
         m_PoweredDownTime( PoweredDownTime ),
         m_PowerUpFlash(    PowerUpFlash ),
         m_Goal( Goal ),
         m_Tile( Tile )
         {}
      C_GHost( const C_GHost & rhs ) :
         m_VelocityVector(  rhs.m_VelocityVector ),
         m_PositionVector(  rhs.m_PositionVector ),
         m_TexQuad(         rhs.m_TexQuad ),
         m_direction(       rhs.m_direction ),
         m_AStarPath(       rhs.m_AStarPath ),
         m_bIsPoweredDown(  rhs.m_bIsPoweredDown ),
         m_bIsEaten(        rhs.m_bIsEaten ),
         m_PoweredDownTime( rhs.m_PoweredDownTime ),
         m_PowerUpFlash(    rhs.m_PowerUpFlash ),
         m_Goal(            rhs.m_Goal ),
         m_Tile(            rhs.m_Tile )
         {}
      ~C_GHost(){}

      void Draw( void )
      {
         m_TexQuad.Draw( 0, 0, 0 );
      }
      void Draw( C_TexCoord TexCoords[4] )
      {
//         m_TexQuad.Draw( 0, 0, 0, TexCoords );
         C_Vert     Verticies[4];
         m_TexQuad.GetVerticies( Verticies, 0, 0, 0 );

         ::glBegin( GL_TRIANGLES );
            TexCoords[0].glTexCoord2f();
            Verticies[0].glVertex2f();

            TexCoords[1].glTexCoord2f();
            Verticies[1].glVertex2f();

            TexCoords[2].glTexCoord2f();
            Verticies[2].glVertex2f();


            TexCoords[0].glTexCoord2f();
            Verticies[0].glVertex2f();

            TexCoords[2].glTexCoord2f();
            Verticies[2].glVertex2f();

            TexCoords[3].glTexCoord2f();
            Verticies[3].glVertex2f();
         ::glEnd();
      }
      void Draw( std::deque<Structs::Point> TextureCoordinates )
      {
         m_TexQuad.Draw2( 0, 0, 0, TextureCoordinates );
      }

      Vector2D  m_PositionVector, 
                  m_VelocityVector;
      C_GLTexQuad m_TexQuad;
      Direction   m_direction;
      std::vector<AStar::AStar2d::PointAndDirection> m_AStarPath;
      bool        m_bIsPoweredDown;
      bool        m_bIsEaten;
      float       m_PoweredDownTime;
      C_Flash     m_PowerUpFlash;
      Structs::Point m_Goal,  m_Tile;

   private:
   protected:
};

#endif

