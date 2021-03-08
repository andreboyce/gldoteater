#pragma once

#ifndef __AStar2d_H
#define __AStar2d_H

#include <vector>
#include <string>
#include <queue>
#include <list>
#include <math.h>
#include "../errorutil/errorutil.h"
#include "../Tile/Tile.h"
#include "../Structs/Structs.h"


namespace AStar
{

class AStar2d
{
   public:
      AStar2d() : m_max_search_percentage( 1.0 )
      {
         m_Nodes.reserve( 32*32 );
         m_NodesSearched.reserve( 32*32 );
      }
      AStar2d( const AStar2d& rhs ) : 
         m_max_search_percentage( rhs.m_max_search_percentage ),
         m_SearchStats( rhs.m_SearchStats ),
         m_Nodes( rhs.m_Nodes ),
         m_NodesSearched( m_NodesSearched )
         {}
      struct PointAndDirection
      {
         enum direction
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

         PointAndDirection( Structs::Point _pos, direction _oreintation ) : 
            pos( _pos ), oreintation( _oreintation ) {}
         PointAndDirection() : pos(), oreintation( UP ) {}
         PointAndDirection( const PointAndDirection& rhs ) : pos( rhs.pos ), oreintation( rhs.oreintation ) {}
         PointAndDirection& operator = ( const PointAndDirection& rhs )
         {
            pos         = rhs.pos;
            oreintation = rhs.oreintation;
            return *this;
         }

         std::string GetDirectionString( direction d )
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

         Structs::Point pos;
         direction        oreintation;

         operator std::string( void )
         {
            std::ostringstream tmp;
            tmp << "{ pos: " << std::string( pos ) << ", direction: " << GetDirectionString(oreintation) << " }";
            return tmp.str();
         }
      };

      struct Node
      {
         Node() : f(0), g(0), h(0), pos( 0, 0 ), parent( NULL ), d(PointAndDirection::UP) {}
         Node( float _f, float _g, float hyp, Structs::Point _pos, Node* _parent, PointAndDirection::direction _d ) : f(_f), g(_g), h(hyp), pos(_pos), parent( _parent ), d(_d){}
         Node( const Node& rhs ) : f(rhs.f), g(rhs.g), h(rhs.h), pos(rhs.pos), parent(rhs.parent), d(rhs.d) {}
         Node& operator = ( const Node& rhs )
         { 
            f      = rhs.f;
            g      = rhs.g; 
            h      = rhs.h;
            pos    = rhs.pos;
            parent = rhs.parent;
            d      = rhs.d;
            return *this;
         }
         bool operator == ( const Node& rhs )
         { 
            if( pos == rhs.pos )
               return true;
            return false;
         }
         bool operator < ( const Node& rhs )
         { 
            if( f < rhs.f )
               return true;
            return false;
         }
         operator float( void ) const
         {
            return f;
         }
         void setf( void )
         {
            f = g + h;
         }
         float f, g, h;
         Structs::Point  pos;
         PointAndDirection::direction d;
         Node* parent;
      };

      struct SearchStats
      {
         SearchStats() : time_taken(0), nodes_searched(0), path_length(0) {}
         SearchStats( long _time_taken, long _nodes_searched, long _path_length ) : 
            time_taken(_time_taken), nodes_searched(_nodes_searched), path_length(_path_length){}
         SearchStats( const SearchStats& rhs ) :
            time_taken( rhs.time_taken ),
            nodes_searched( rhs.nodes_searched ),
            path_length( rhs.path_length )
         {}

         SearchStats& operator= ( const SearchStats& rhs )
         { 
            time_taken     = rhs.time_taken;
            nodes_searched = rhs.nodes_searched;
            path_length    = rhs.path_length;
            return *this;
         }

         long time_taken;
         long nodes_searched;
         long path_length;
      };

      typedef std::vector< std::vector<Tile> > TileVector2D;

      std::vector<PointAndDirection> CalculatePath( TileVector2D& map, Structs::Point start_pos, Structs::Point end_pos, float maxsolidity, bool ballow_diagonal );
      float hyp( Structs::Point start_pos, Structs::Point end_pos );
      std::vector<Node> GetSurroundingNodes( const TileVector2D& map, Node node, bool ballow_diagonal );
      bool ValidIndex( const TileVector2D& map, Structs::Point pos );
      bool ValidIndex( const TileVector2D& map, Node n );

      bool LoadPath( std::string path );
      bool SavePath( std::string path );

      std::vector<PointAndDirection>& GetNodesSearched( void )
      {
         return m_NodesSearched;
      }

      void SetMaxSearchPercentage( float max_search_percentage )
      {
         m_max_search_percentage = max_search_percentage;
      }

      float GetMaxSearchPercentage( void )
      {
         return m_max_search_percentage;
      }

      //SearchStats AStar2d::GetSearchStats( void );
      SearchStats GetSearchStats( void );

   protected:
      SearchStats                    m_SearchStats;

      std::vector<PointAndDirection> m_Nodes,
                                     m_NodesSearched;
      float                          m_max_search_percentage;
   private:
};

};
#endif

