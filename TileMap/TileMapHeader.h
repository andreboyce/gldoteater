#pragma once

#ifndef __TileMapHeader_H
#define __TileMapHeader_H

struct TileMapHeader
{
   int         width;
   int         height;
   int         max_solidity;
   std::string tileset_file_name;

   void clear()
   {
      width        = 0;
      height       = 0;
      max_solidity = 1000;
      tileset_file_name.clear();
   }

   TileMapHeader() : width(0), height(0), max_solidity(1000), tileset_file_name() {}
   TileMapHeader( const TileMapHeader& rhs ) : width(rhs.width), height(rhs.height), max_solidity( rhs.max_solidity ), tileset_file_name(rhs.tileset_file_name) {}
   TileMapHeader& operator = ( const TileMapHeader& rhs )
   { 
      width             = rhs.width;
      height            = rhs.height;
      max_solidity      = rhs.max_solidity;
      tileset_file_name = rhs.tileset_file_name;
      return *this;
   }
   ~TileMapHeader(){}

   bool read( std::ifstream& in_file )
   {
      if( !in_file.is_open() )
         return false;
      std::ofstream::pos_type pos = in_file.tellg();
      in_file.read( (char*)&width,        sizeof(width)  );
      in_file.read( (char*)&height,       sizeof(height) );
      in_file.read( (char*)&max_solidity, sizeof(max_solidity) );
      std::string::size_type length = 0;
      in_file.read( (char*)&length, sizeof(length) );
	  std::vector<char> str(length+1);
      std::memset ( &str[0], '\0', length+1 );
      in_file.read( (char*)&str[0], length );
      tileset_file_name = &str[0]; // <--- potential bug grrrrrrrr
      if( in_file.bad() )
      {
         in_file.seekg( pos );
         return false;
      }
      return true;
   }
   bool write( std::ofstream& out_file )
   {
      if( !out_file.is_open() )
         return false;
      std::ofstream::pos_type pos = out_file.tellp();
      out_file.write( (char*)&width,        sizeof(width)  );
      out_file.write( (char*)&height,       sizeof(height) );
      out_file.write( (char*)&max_solidity, sizeof(max_solidity) );
      std::string::size_type length = tileset_file_name.length();
      out_file.write( (char*)&length, sizeof(length) );
      out_file.write( (char*)tileset_file_name.c_str(), tileset_file_name.length() );
      if( out_file.bad() )
      {
         out_file.seekp( pos );
         return false;
      }
      return true;
   }
   bool writeXTM( std::ofstream& out_file, std::string tabs )
   {
      if( !out_file.is_open() )
         return false;
      std::ofstream::pos_type pos = out_file.tellp();

	  std::string temp = tabs + "<header>" + "\n";
	  out_file.write( temp.c_str(), temp.size() );

	  temp = tabs + tabs + "<FileFormatVersion value=\"1.0\"/>" + "\n";
	  out_file.write( temp.c_str(), temp.size() );

      std::stringstream ss;
      std::string str;
      ss << width;
	  ss >> str;
	  temp = tabs + tabs + "<Width value=\"" + str + "\"/>" + "\n";
      out_file.write( temp.c_str(), temp.size() );

	  ss.clear();
	  str.clear();
      ss << height;
	  ss >> str;
      temp = tabs + tabs + "<Height value=\"" + str + "\"/>" + "\n";
	  out_file.write( temp.c_str(), temp.size() );

	  ss.clear();
	  str.clear();
      ss << max_solidity;
	  ss >> str;
      temp = tabs + tabs + "<MaxDesnity value=\"" + str + "\"/>" + "\n";
	  out_file.write( temp.c_str(), temp.size() );

      temp = tabs + tabs + "<TileSetFileName value=\"" + tileset_file_name + "\"/>" + "\n";
	  out_file.write( temp.c_str(), temp.size() );

	  temp = tabs + "</header>" + "\n";
	  out_file.write( temp.c_str(), temp.size() );

      /*out_file.write( (char*)&width,        sizeof(width)  );
      out_file.write( (char*)&height,       sizeof(height) );
      out_file.write( (char*)&max_solidity, sizeof(max_solidity) );
      std::string::size_type length = tileset_file_name.length();
      out_file.write( (char*)&length, sizeof(length) );
      out_file.write( (char*)tileset_file_name.c_str(), tileset_file_name.length() );*/
      if( out_file.bad() )
      {
         out_file.seekp( pos );
         return false;
      }
      return true;
   }
};

#endif
