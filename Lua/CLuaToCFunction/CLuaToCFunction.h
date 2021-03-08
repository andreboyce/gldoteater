#ifndef __CLuaToCFunction_H
#define __CLuaToCFunction_H

#include <string>
#include <vector>
#include "../../errorutil/errorutil.h"

extern "C" {
	//#include "../NonStandardHeaders/lua/lua.h"
	//#include "../NonStandardHeaders/lua/lualib.h"
	//#include "../NonStandardHeaders/lua/lauxlib.h"

//#include <lua/lua.h>
//#include <lua/lualib.h>
//#include <lua/lauxlib.h>
}

class CLuaToCFunction
{
   public:

      /*struct LuaType
      {
         virtual void push( lua_State* LuaState ) = 0;
         virtual void pop(  lua_State* LuaState ) = 0;
      };

      struct String: public LuaType
      {
         ~String(){ delete data; }
         String() : data( NULL ){}
         String( char* _data )
         {
            data = new char[ strlen( _data )+1 ];
            strcpy( data, _data );//, strlen( _data )+1 );
         }
         String& operator = ( const String& rhs )
         {
            data = new char[ strlen( rhs.data )+1 ];
            strcpy( data, rhs.data );//, strlen( rhs.data )+1 );
            return *this;
         }
         void push( lua_State* LuaState )
         {
            ::lua_pushstring( LuaState, data );
         }
         void pop( lua_State* LuaState )
         {
            if( lua_type( LuaState, lua_gettop( LuaState ) ) == LUA_TSTRING )
            {
               char* str = (char*)lua_tostring( LuaState, lua_gettop( LuaState ) );
               data      = new char[ strlen(str)+1 ];
               strcpy( data, str ); //, strlen(str)+1 );
               lua_pop( LuaState, 1 );
            }
         }
         char *data;
      };

      struct Numeric: public LuaType
      {
         Numeric() : data( 0 ){}
         Numeric( const Numeric & rhs ) : data( rhs.data ){}
         Numeric( long _data ) : data( _data ){}
         Numeric & operator = ( const Numeric & rhs )
         {
            data = rhs.data;
            return *this;
         }

         void push( lua_State* LuaState )
         {
            ::lua_pushnumber( LuaState, data );
         }
         void pop( lua_State* LuaState )
         {
            if( lua_type( LuaState, lua_gettop( LuaState ) ) == LUA_TNUMBER )
            {
               data = (int)lua_tonumber( LuaState, lua_gettop( LuaState ) );
               lua_pop( LuaState, 1 );
            }
         }
         long data;
      };

      struct Boolean: public LuaType
      {
         Boolean() : data( 0 ){}
         Boolean( bool _data ) : data( _data ){}
         Boolean( const Boolean & rhs ) : data( rhs.data ) {}
         Boolean & operator = ( const Boolean & rhs )
         {
            data = rhs.data;
            return *this;
         }
         void push( lua_State* LuaState )
         {
            ::lua_pushboolean( LuaState, data );
         }
         void pop( lua_State* LuaState )
         {
            if( lua_type( LuaState, lua_gettop( LuaState ) ) == LUA_TBOOLEAN )
            {
               data = (int)lua_toboolean( LuaState, lua_gettop( LuaState ) );
               lua_pop( LuaState, 1 );
            }
         }
         bool data;
      };

      typedef std::vector< LuaType *> V_LuaType;
	  */

      CLuaToCFunction( void );
      ~CLuaToCFunction( void );

      //V_LuaType operator()( V_LuaType& LuaTypes );
      //void Register( lua_State* LuaState, std::string FunctionName );
      //void DeteteReturnValues( void );

   protected:
      //lua_State*  m_LuaState;
      //std::string m_FunctionName;
      //V_LuaType   m_ReturnValues;

   private:
};

#endif

