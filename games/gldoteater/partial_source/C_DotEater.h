#ifndef __C_DotEater_H
#define __C_DotEater_H

#include "CommonSpriteAttributes.h"
#define SCORE_FOR_LIFEUP 3000

#include <map>

class C_DotEater
{
   public:
      enum PlayerAnimation
      {
          EAT_DOT,
          DIE,
      };

      typedef std::map< int, int > ItemInventory;
      C_DotEater():
         m_PositionVector(  ),
         m_VelocityVector(  ),
         m_TexQuad( ),
         m_direction( LEFT ),
         m_AStarPath(),
         m_Lives( 3 ),
         m_bIsPoweredUp( false ),
         m_PoweredUpTime( 0 ),
         m_Score( 0 ),
         m_ControllKeys( ControllKeys() ),
         m_InvincibleFlash( false, 500 ),
         m_InvincibleTime( 0 ),
         m_bIsInvincible( false ),
         m_LastLifeUpFromPoints( 0 ),
         m_Inventory(),
         m_RequestedDirection( Direction() ),
         m_Tile( Structs::Point() ),
         m_CurrentAnimationID( EAT_DOT ),
         m_PlayerID( 0 ),
         m_TimeTillRespawn( 0 )
         {}
      C_DotEater( const C_DotEater & rhs ):
         m_PositionVector(       rhs.m_PositionVector     ),
         m_VelocityVector(       rhs.m_VelocityVector     ),
         m_TexQuad(              rhs.m_TexQuad            ),
         m_direction(            rhs.m_direction          ),
         m_AStarPath(            rhs.m_AStarPath          ),
         m_Lives(                rhs.m_Lives              ),
         m_bIsPoweredUp(         rhs.m_bIsPoweredUp       ),
         m_PoweredUpTime(        rhs.m_PoweredUpTime      ),
         m_Score(                rhs.m_Score              ),
         m_ControllKeys(         rhs.m_ControllKeys       ),
         m_InvincibleFlash(      rhs.m_InvincibleFlash    ),
         m_InvincibleTime(       rhs.m_InvincibleTime     ),
         m_bIsInvincible(        rhs.m_bIsInvincible      ),
         m_LastLifeUpFromPoints( rhs.m_LastLifeUpFromPoints ),
         m_Inventory(            rhs.m_Inventory          ),
         m_RequestedDirection(   rhs.m_RequestedDirection ),
         m_Tile(                 rhs.m_Tile               ),
         m_CurrentAnimationID(   rhs.m_CurrentAnimationID ),
         m_PlayerID(             rhs.m_PlayerID           ),
         m_TimeTillRespawn(      rhs.m_TimeTillRespawn    )
         {}
      C_DotEater( Vector2D PositionVector, Vector2D VelocityVector, 
                  C_GLTexQuad TexQuad, 
                  Direction direction,
                  std::vector<AStar::AStar2d::PointAndDirection> AStarPath,
                  int Lives,
                  bool bIsPoweredUp,
                  float PoweredUpTime,
                  long Score,
                  ControllKeys _ControllKeys,
                  C_Flash InvinsibleFlash,
                  float InvincibleTime,
                  bool bIsInvincible,
                  int LastLifeUpFromPoints,
                  C_DotEater::ItemInventory Inventory,
                  Direction RequestedDirection,
                  Structs::Point Tile,
                  PlayerAnimation AnimationID,
                  int PlayerID,
                  int TimeTillRespawn ) :
         m_PositionVector( PositionVector ),
         m_VelocityVector( VelocityVector ),
         m_TexQuad( TexQuad ),
         m_direction( direction ),
         m_AStarPath( AStarPath ),
         m_Lives( Lives ),
         m_bIsPoweredUp( bIsPoweredUp ),
         m_PoweredUpTime( PoweredUpTime ),
         m_Score( Score ),
         m_ControllKeys( _ControllKeys ),
         m_InvincibleFlash( InvinsibleFlash ),
         m_InvincibleTime( InvincibleTime ),
         m_bIsInvincible( bIsInvincible ),
         m_LastLifeUpFromPoints( LastLifeUpFromPoints ),
         m_Inventory( Inventory ),
         m_RequestedDirection( RequestedDirection ),
         m_Tile( Tile ),
         m_CurrentAnimationID( AnimationID ),
         m_PlayerID( PlayerID ),
         m_TimeTillRespawn( TimeTillRespawn )
      {}
      ~C_DotEater(){}

      SpriteAnimation& GetCurrentAnimation( void )
      {
         switch( m_CurrentAnimationID )
         {
            case EAT_DOT:
            {
            }  return m_DotEatAnimation;
            case DIE:
            {
            }  return m_DieAnimation;
            default:
            {
               return m_DotEatAnimation;
            }
         }
      }

      void play_animation( PlayerAnimation aniamtion_id, bool restart )
      {
         switch( aniamtion_id )
         {
            case EAT_DOT:
            {
               m_CurrentAnimationID = aniamtion_id;
               GetCurrentAnimation().play( restart );
            } break;
            case DIE:
            {
               m_CurrentAnimationID = aniamtion_id;
               GetCurrentAnimation().play( restart );
            } break;
            default:
            {
            }
         }
      }

      void Draw( void )
      {
         m_TexQuad.Draw( 0, 0, 0 );
      }
      void Draw( C_TexCoord TexCoords[4] )
      {
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

      Structs::Point   m_Tile;
      Vector2D         m_PositionVector, m_VelocityVector;
      C_GLTexQuad        m_TexQuad;
      Direction          m_direction, m_RequestedDirection;
      std::vector<AStar::AStar2d::PointAndDirection> m_AStarPath;
      ItemInventory      m_Inventory;
      int                m_Lives;
      bool               m_bIsPoweredUp;
      float              m_PoweredUpTime;
      long               m_Score;
      ControllKeys       m_ControllKeys;
      C_Flash            m_InvincibleFlash;
      float              m_InvincibleTime;
      bool               m_bIsInvincible;
      int                m_LastLifeUpFromPoints;
      PlayerAnimation    m_CurrentAnimationID;
      int                m_PlayerID;
      double             m_TimeTillRespawn;
      SpriteAnimation  m_DotEatAnimation,
                         m_DieAnimation;
   protected:
   private:
};

#endif
