#pragma once

#ifndef __TileMapSpecialDataTypes_H
#define __TileMapSpecialDataTypes_H

struct TileMapSpecialDataTypes
{
   TileMapSpecialDataTypes() : id(0)
   {
   }
   TileMapSpecialDataTypes( int _id, std::string _name ) : id( _id ), name( _name )
   {
   }
   TileMapSpecialDataTypes& operator=( const TileMapSpecialDataTypes& rhs )
   {
      id   = rhs.id;
      name = rhs.name;
      return *this;
   }

   bool read( std::ifstream& in_file )
   {
      if( !in_file.is_open() )
         return false;
      in_file.read( (char*)&id,   sizeof(id)   );
      std::string::size_type name_len = 0;
      in_file.read( (char*)&name_len, sizeof(name_len) );
	  std::vector<char> str(name_len+1);
      std::memset( &str[0], '\0', name_len+1 );
      in_file.read( (char*)&str[0], name_len );
      name = &str[0];// <--- potential bug grrrrrrrr
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
      out_file.write( (char*)&id,   sizeof(id)   );
      std::string::size_type name_len = name.length();
      out_file.write( (char*)&name_len, sizeof(name_len) );
      out_file.write( (char*)name.c_str(), name_len );
      if( out_file.bad() )
      {
         return false;
      }
      return true;
   }
   operator std::string ( void )
   {
      std::ostringstream tmp;
      tmp << "{ id: " << id << ", name: " << name << " }";
      return tmp.str();
   }

   int           id;
   std::string name;
};

#endif
