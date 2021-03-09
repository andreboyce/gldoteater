#include "main.h"

Structs::Point MainWindow::GetTile( const Vector2D& position )
{
   if( !m_TileMap.width() || !m_TileMap.height() )
      return Structs::Point();
   return Structs::Point( (long)my_roundf( ( position.m_x + m_BoundingRectHalfWidthMinMapTexturedQuadhalfwidth ) * m_InvBoundingRectWidthDivTileMapWidth ),
                             abs( (long)my_roundf( ( position.m_y - m_BoundingRectHalfHeight + m_MapTexturedQuad.halfheight() ) * m_InvBoundingRectHeightDivTileMapHeight ) ) );
}

Structs::Point MainWindow::GetTile( const Structs::dPoint3D& position )
{
   if( !m_TileMap.width() || !m_TileMap.height() )
      return Structs::Point();

//   Alert( m_TileMap.width() );
//   Alert( m_TileMap.height() );

   return Structs::Point( (long)my_roundf( ( position.m_x + m_BoundingRectHalfWidthMinMapTexturedQuadhalfwidth ) * m_InvBoundingRectWidthDivTileMapWidth ),
                             abs( (long)my_roundf( ( position.m_y - m_BoundingRectHalfHeight + m_MapTexturedQuad.halfheight() ) * m_InvBoundingRectHeightDivTileMapHeight ) ) );
}

