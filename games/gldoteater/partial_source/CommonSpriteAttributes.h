#ifndef __CommonSpriteAttributes_H
#define __CommonSpriteAttributes_H

#include "../../../AStar/AStar2d.h"
#include "../../../MathVector/Vector2D/Vector2D.h"
#include "../../../OpenGLLib/C_GLTexQuad/C_GLTexQuad.h"
#include "../../../OpenGLLib/C_GLTextureManagerHash/C_GLTextureManagerHash.h"
#include "../../../TimeInterval/TimeInterval.h"
#include "../../../apps/SpriteAnimationEditor/SpriteAnimation.h"
#include <string>

#define POWER_UP_AND_DOWN_TIME 5500.0f
#define INVINCIBLE_TIME 2000.0f

enum Direction
{
   UP = 0,
   DOWN,
   LEFT,
   RIGHT,
   UPLEFT,
   UPRIGHT,
   DOWNLEFT,
   DOWNRIGHT,
};

std::string GetDirectionString( Direction d );

#define EAT_GHOST_SCORE      200
#define EAT_BIGDOT_SCORE     200
#define EAT_DOT_SCORE         20
#define EAT_RANDOMITEM_SCORE 200

struct ControllKeys
{
   public:
      ControllKeys() :
          m_Up( VK_UP ),
          m_Down( VK_DOWN ),
          m_Left( VK_LEFT ),
          m_Right( VK_RIGHT )
      {}
      ControllKeys( int Up, int Down, int Left, int Right ) :
          m_Up( Up ),
          m_Down( Down ),
          m_Left( Left ),
          m_Right( Right )
      {}

      ControllKeys( const ControllKeys& rhs ) :
          m_Up( rhs.m_Up ),
          m_Down( rhs.m_Down ),
          m_Left( rhs.m_Left ),
          m_Right( rhs.m_Right )
      {}
      ControllKeys& operator=( const ControllKeys& rhs )
      {
          m_Up    = rhs.m_Up;
          m_Down  = rhs.m_Down;
          m_Left  = rhs.m_Left;
          m_Right = rhs.m_Right;
      }
      ~ControllKeys(){}

      void set( int Up, int Down, int Left, int Right )
      {
          m_Up    = Up;
          m_Down  = Down;
          m_Left  = Left;
          m_Right = Right;
      }

      int m_Up,
          m_Down,
          m_Left,
          m_Right;
   protected:
   private:
};

struct PositionAndDirection
{
   Structs::Point  m_tile;
   Direction         m_direction;
};


#endif

