#ifndef __CLuaState_H
#define __CLuaState_H

extern "C" {
//	#include "../NonStandardHeaders/lua/lua.h"
//	#include "../NonStandardHeaders/lua/lualib.h"
//	#include "../NonStandardHeaders/lua/lauxlib.h"

//	#include <lua/lua.h>
//	#include <lua/lualib.h>
//	#include <lua/lauxlib.h>

}

#include <string>

class CLuaState
{
   public:
      CLuaState();
      ~CLuaState();

      /*void openlibs( luaL_reg [] );
      void opendefaultlibs( void );

      void LoadScript( std::string scriptname )
      {
         luaL_loadfile( m_LuaState, scriptname.c_str() );
      }

      operator lua_State*( void )
      {
         return m_LuaState;
      }*/

   protected:
      //lua_State* m_LuaState;

   private:
};

#endif

