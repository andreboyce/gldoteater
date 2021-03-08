#include "CommonSpriteAttributes.h"

std::string GetDirectionString( Direction d )
{
   switch( d )
   {
      case UP:
      {
         return std::string( "UP" );
      } break;
      case DOWN:
      {
         return std::string( "DOWN" );
      } break;
      case LEFT:
      {
         return std::string( "LEFT" );
      } break;
      case RIGHT:
      {
         return std::string( "RIGHT" );
      } break;
      case UPLEFT:
      {
         return std::string( "UPLEFT" );
      } break;
      case UPRIGHT:
      {
         return std::string( "UPRIGHT" );
      } break;
      case DOWNLEFT:
      {
         return std::string( "DOWNLEFT" );
      } break;
      case DOWNRIGHT:
      {
         return std::string( "DOWNRIGHT" );
      } break;
      default:
      {
         return std::string();
      }
   }
}
