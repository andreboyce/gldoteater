#pragma once

#ifndef __TileMapSpecialData_H
#define __TileMapSpecialData_H

#include "../Structs/Structs.h"
#include <string>

struct TileMapSpecialData
{
   Structs::Point pos()
   {
      return Structs::Point( x, y );
   }
   TileMapSpecialData() : x(0), y(0), value(0)
   {
   }
   TileMapSpecialData( const TileMapSpecialData& rhs ) : x(rhs.x), y(rhs.y), value(rhs.value)
   {
   }
   TileMapSpecialData( int _x, int _y, int _value ) : x(_x), y(_y), value(_value)
   {
   }
   TileMapSpecialData& operator=( const TileMapSpecialData& rhs )
   {
      x     = rhs.x;
      y     = rhs.y; 
      value = rhs.value;
      return *this;
   }

   bool read( std::ifstream& in_file )
   {
      if( !in_file.is_open() )
         return false;
      in_file.read( (char*)&x,     sizeof(x)     );
      in_file.read( (char*)&y,     sizeof(y)     );
      in_file.read( (char*)&value, sizeof(value) );
      if( in_file.bad() )
      {
         return false;
      }
      return true;
   }
   bool write( std::ofstream& out_file )
   {
      if( !out_file.is_open() )
         return false;
      out_file.write( (char*)&x,     sizeof(x)     );
      out_file.write( (char*)&y,     sizeof(y)     );
      out_file.write( (char*)&value, sizeof(value) );
      if( out_file.bad() )
      {
         return false;
      }
      return true;
   }
   operator std::string ( void )
   {
      std::ostringstream tmp;
      tmp << "{ x: " << x << "," << " y: " << y << ", value: " << value << " }";
      return tmp.str();
   }

   int x, y, value;
};

#endif
