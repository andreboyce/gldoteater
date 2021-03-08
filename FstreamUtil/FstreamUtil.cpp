#include "FstreamUtil.h"

unsigned long FstreamFileSize( std::string str )
{
   unsigned long l,m;
   std::ifstream s( str.c_str(), std::ios::in | std::ios::binary);
   l = s.tellg();
   s.seekg( 0, std::ios::end );
   m = s.tellg();
   s.close();
   return (m-l);
}
