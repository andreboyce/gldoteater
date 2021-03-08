#include "CLuaState.h"

CLuaState::CLuaState( void ) //:
   // initialize Lua 
   //m_LuaState( lua_open() )
{   
}

CLuaState::~CLuaState( void )
{
   // cleanup Lua 
   //::lua_close( m_LuaState );
}

/*
// load the list of libs
void CLuaState::openlibs( luaL_reg lualibs[] )
{
   for( ; lualibs->func; lualibs++ )
   {
//      lualibs->func( m_LuaState );  // open library
//      lua_settop( m_LuaState, 0 );  // discard any results
   }
}

void CLuaState::opendefaultlibs( void )
{
   luaL_reg lualibs[] = 
   {
      { "base",    luaopen_base    },
      { "table",   luaopen_table   },
      { "io",      luaopen_io      },
      { "string",  luaopen_string  },
      { "math",    luaopen_math    },
      { "debug",   luaopen_debug   },
      { "package", luaopen_package }, 
      {NULL, NULL}
   };

   openlibs( lualibs );
}*/

