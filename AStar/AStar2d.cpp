#include "AStar2d.h"

namespace AStar
{

AStar2d::SearchStats AStar2d::GetSearchStats( void )
{
   return m_SearchStats;
}

bool AStar2d::LoadPath( std::string path )
{
   return true;
}

bool AStar2d::SavePath( std::string path )
{
   return true;
}

bool AStar2d::ValidIndex( const TileVector2D& map, Structs::Point pos )
{
   if( map.size() <= 0 )
      return false;
   if( map[0].size() <= 0 )
      return false;

   if( (pos.x >= map[0].size()) || (pos.x < 0) )
      return false;
   if( (pos.y >= map.size()) || (pos.y < 0) )
      return false;

   return true;
}

bool AStar2d::ValidIndex( const TileVector2D& map, Node n )
{
   if( map.size() <= 0 )
      return false;
   if( map[0].size() <= 0 )
      return false;

   if( (n.pos.x >= map[0].size()) || (n.pos.x < 0) )
      return false;
   if( (n.pos.y >= map.size()) || (n.pos.y < 0) )
      return false;

   return true;
}

std::vector<AStar2d::Node> AStar2d::GetSurroundingNodes( const TileVector2D& map, Node n, bool ballow_diagonal )
{
   try
   {
   std::vector<Node> SurroundingNodes;

   if( !ValidIndex( map, n ) )
      return SurroundingNodes;

   Node node = n;

//  __ __ __ 
// |__|*_|__|
// |__|__|__|
// |__|__|__|
   node.pos.y -= 1;
   node.d      = PointAndDirection::UP;
   if( ValidIndex( map, node ) )
      SurroundingNodes.push_back( node );
   node = n;

//  __ __ __ 
// |__|-_|__|
// |__|__|__|
// |__|*_|__|
   node.pos.y += 1;
   node.d      = PointAndDirection::DOWN;
   if( ValidIndex( map, node ) )
      SurroundingNodes.push_back( node );
   node = n;

//  __ __ __ 
// |__|-_|*_|
// |__|__|__|
// |__|-_|__|
   if( ballow_diagonal )
   {
      node.pos.y -= 1;
      node.pos.x += 1;
      node.d      = PointAndDirection::UPRIGHT;
      if( ValidIndex( map, node ) )
        SurroundingNodes.push_back( node );
      node = n;
   }

//  __ __ __ 
// |__|-_|-_|
// |__|__|*_|
// |__|-_|__|
   node.pos.x += 1;
   node.d      = PointAndDirection::RIGHT;
   if( ValidIndex( map, node ) )
      SurroundingNodes.push_back( node );
   node = n;

//  __ __ __ 
// |__|-_|-_|
// |__|__|-_|
// |__|-_|*_|
   if( ballow_diagonal )
   {
      node.pos.y += 1;
      node.pos.x += 1;
      node.d      = PointAndDirection::DOWNRIGHT;
      if( ValidIndex( map, node ) )
         SurroundingNodes.push_back( node );
      node = n;
   }

//  __ __ __ 
// |__|-_|-_|
// |__|__|-_|
// |*_|-_|-_|
   if( ballow_diagonal )
   {
      node.pos.y += 1;
      node.pos.x -= 1;
      node.d      = PointAndDirection::DOWNLEFT;
      if( ValidIndex( map, node ) )
         SurroundingNodes.push_back( node );
      node = n;
   }

//  __ __ __ 
// |__|-_|-_|
// |*_|__|-_|
// |-_|-_|-_|
   node.pos.x -= 1;
   node.d      = PointAndDirection::LEFT;
   if( ValidIndex( map, node ) )
      SurroundingNodes.push_back( node );
   node = n;

//  __ __ __ 
// |*_|-_|-_|
// |-_|__|-_|
// |-_|-_|-_|
   if( ballow_diagonal )
   {
      node.pos.y -= 1;
      node.pos.x -= 1;
      node.d      = PointAndDirection::UPLEFT;
      if( ValidIndex( map, node ) )
         SurroundingNodes.push_back( node );
      node = n;
   }
   return SurroundingNodes;

   }
   catch( ... )
   {
      return std::vector<Node>();
   }

}

float AStar2d::hyp( Structs::Point start_pos, Structs::Point end_pos )
{
   float x = end_pos.x - start_pos.x;
   float y = end_pos.y - start_pos.y;
   return sqrt( x*x + y*y );
}

std::vector<AStar2d::PointAndDirection> AStar2d::CalculatePath( TileVector2D& map, Structs::Point start_pos, Structs::Point end_pos, float maxsolidity, bool ballow_diagonal )
{
   try
   {
      m_NodesSearched.clear();
      m_Nodes.clear();

      if( !ValidIndex( map, end_pos ) || !ValidIndex( map, start_pos ) )
      {
         return m_Nodes;
      }

      if( !map.size() )
      {
         return m_Nodes;
      }
      if( !map[0].size() )
      {
         return m_Nodes;
      }
      if( end_pos == start_pos )
         return m_Nodes;

      float inv_maxsolidity = 1.0/maxsolidity;
      float diagonalcost    = 0.75;
      float regularcost     = 0.1;
      float cost            = 0.0;
      long  nodesopened     = 0;

      std::list<Node> ClosedList;
      std::list<Node> OpenList;

      Node startnode = Node( 0, 0, hyp( start_pos, end_pos ), Structs::Point( start_pos ), NULL, PointAndDirection::UP );
      Node   endnode = Node( 0, 0, hyp( start_pos, end_pos ), Structs::Point(   end_pos ), NULL, PointAndDirection::UP );   
      startnode.setf();
      OpenList.push_front( startnode );
      Node  node_current;
      bool break_from_main_loop = false;

      long time_taken = ::GetTickCount();

      while( OpenList.size() )
      {
         if( break_from_main_loop )
         {
            break;
         }
         OpenList.sort();
         node_current = OpenList.front();
         if( node_current == endnode )
         {
            break;
         }
         std::vector<Node> SurroundingNodes = GetSurroundingNodes( map, node_current, ballow_diagonal );
         OpenList.pop_front();
         ClosedList.push_front( node_current );
         if( OpenList.size() > (map.size()*map[0].size()*m_max_search_percentage) )
         {
            break;
         }
         if( OpenList.size() )
            m_NodesSearched.push_back( PointAndDirection( node_current.pos, node_current.d ) );

         for( int i=0; i<SurroundingNodes.size() ; ++i )
         {
            if( SurroundingNodes[i] == endnode )
            {
               break_from_main_loop = true;
               break;
            }
            if( map[ SurroundingNodes[i].pos.y ][ SurroundingNodes[i].pos.x ].m_Solidity >= maxsolidity )
            {
               continue;
            }

//         SurroundingNodes[i].g = hyp( SurroundingNodes[i].pos, startnode.pos ) + (map[ SurroundingNodes[i].pos.y ][ SurroundingNodes[i].pos.x ].m_Solidity*inv_maxsolidity);
//         SurroundingNodes[i].g = node_current.g + hyp( SurroundingNodes[i].pos, node_current.pos ) + (map[ SurroundingNodes[i].pos.y ][ SurroundingNodes[i].pos.x ].m_Solidity*inv_maxsolidity);
            if( (SurroundingNodes[i].d == PointAndDirection::UPLEFT )    ||
                (SurroundingNodes[i].d == PointAndDirection::DOWNLEFT )  ||
                (SurroundingNodes[i].d == PointAndDirection::UPRIGHT )   ||
                (SurroundingNodes[i].d == PointAndDirection::DOWNRIGHT )
              )
               cost = diagonalcost;
            else
               cost = regularcost;
         
            SurroundingNodes[i].g = node_current.g + cost + (map[ SurroundingNodes[i].pos.y ][ SurroundingNodes[i].pos.x ].m_Solidity*inv_maxsolidity);
            SurroundingNodes[i].h = hyp( SurroundingNodes[i].pos, endnode.pos );
            SurroundingNodes[i].setf();
            bool found = false;
            for( std::list<Node>::iterator c = OpenList.begin(); c != OpenList.end(); ++c )
            {
               if( (*c) == SurroundingNodes[i] )
               {
//               if( (*c) < SurroundingNodes[i] )
//               {
                     found = true;
                     break;
//               }
               }
            }
            if( found )
            {
               continue;
            }
            for( std::list<Node>::iterator c = ClosedList.begin(); c != ClosedList.end() ; ++c )
            {
               if( (*c) == SurroundingNodes[i] )
               {
//               if( (*c) < SurroundingNodes[i] )
//               {               
                     found = true;
                     break;
//               }
               }
            }
            if( found )
            {
               continue;
            }
            SurroundingNodes[i].parent = &(ClosedList.front());
            OpenList.push_back( SurroundingNodes[i] );
            m_NodesSearched.push_back( PointAndDirection( SurroundingNodes[i].pos, SurroundingNodes[i].d ) );
         }
      }

      while( node_current.parent != NULL )
      {
         m_Nodes.push_back( PointAndDirection( node_current.pos, node_current.d ) );
         node_current = (*node_current.parent);
      }

      m_SearchStats.nodes_searched = m_NodesSearched.size();
      m_SearchStats.path_length    = m_Nodes.size();
      m_SearchStats.time_taken     = ::GetTickCount() - time_taken;
      return m_Nodes;

   }
   catch( std::exception& e )
   {
      m_NodesSearched.clear();
      m_Nodes.clear();
      return m_Nodes;
   }
   catch( ... )
   {
      m_NodesSearched.clear();
      m_Nodes.clear();
      return m_Nodes;
   }
}

};

