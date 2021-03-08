#pragma once

#ifndef __InputMap_H
#define __InputMap_H

#include <string>
#include <map>

class InputMap
{
   public:
	  struct Action
	  {
	     public:
			Action(){}
			~Action(){}
	        std::string m_KeyBinding;
	     protected:
	     private:
	  };

	  InputMap( void ){}
	  ~InputMap( void ){}
	  bool Load( std::string filename );
	  bool Save( std::string filename );
	  Action getAction( int key );

   protected:
	   std::map<int,Action> m_InputMap;

   private:
};

#endif
