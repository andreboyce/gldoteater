#ifndef __C_WinSockErrors_H
#define __C_WinSockErrors_H

#include <string>

class C_WinSockErrors
{
   public:
      static std::string GetLastErrorString( void );
      static std::string GetLastErrorString( int errorcode );

   protected:
   private:
};

#endif
