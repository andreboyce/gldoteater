#include "CollisionRectangle.h"

CollisionRectangle::CollisionRectangle( void )
{
   width = height = 0;
}

CollisionRectangle::CollisionRectangle( const CollisionRectangle& CollisionRectangleRHS )
{
   width  = CollisionRectangleRHS.width;
   height = CollisionRectangleRHS.height;
}

CollisionRectangle::~CollisionRectangle( void )
{
}
