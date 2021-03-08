#include "CLuaToCFunction.h"

CLuaToCFunction::CLuaToCFunction( void )
{
}

CLuaToCFunction::~CLuaToCFunction( void )
{ 
   //DeteteReturnValues();
}

/*void CLuaToCFunction::Register( lua_State* LuaState, std::string FunctionName )
{
   m_LuaState     = LuaState;
   m_FunctionName = FunctionName;
}

CLuaToCFunction::V_LuaType CLuaToCFunction::operator()( V_LuaType& LuaTypes )
{
   if( !m_LuaState )
      return V_LuaType();
   if( !m_FunctionName.size() )
      return V_LuaType();

   DeteteReturnValues();

   // the function name 
   lua_getglobal( m_LuaState, m_FunctionName.c_str() );

   for( int i=0; i<LuaTypes.size(); i++ )
   {
      // push parameter on the stack
      LuaTypes[i]->push( m_LuaState );
   }

   // call function
   lua_call( m_LuaState, LuaTypes.size(), LUA_MULTRET );

   int number_of_results = lua_gettop( m_LuaState );

   while( lua_gettop( m_LuaState ) > 0 )
   {
      switch( lua_type( m_LuaState, lua_gettop( m_LuaState ) ) )
      {
         case LUA_TNUMBER:
         {
            Numeric *num = new Numeric;
            num->pop( m_LuaState );
            m_ReturnValues.push_back( num );
         } break;
         case LUA_TSTRING:
         {
            String *str = new String;
            str->pop( m_LuaState );
            m_ReturnValues.push_back( str );
         } break;
         case LUA_TBOOLEAN:
         {
            Boolean *bol = new Boolean;
            bol->pop( m_LuaState );
            m_ReturnValues.push_back( bol );
         } break;
         case LUA_TTABLE:
         {
         } break;
         default:
         {
            // unknown type
         } break;
      }
      lua_pop( m_LuaState, 1 );
   }

   return m_ReturnValues;
}

void CLuaToCFunction::DeteteReturnValues( void )
{
   for( int i=0; i<m_ReturnValues.size(); i++ )
      delete m_ReturnValues[i];
   m_ReturnValues.clear();
}
*/

