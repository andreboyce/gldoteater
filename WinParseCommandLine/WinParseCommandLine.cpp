#include "WinParseCommandLine.h"

WinParseCommandLine::WinParseCommandLine( LPSTR lpCmdLine, char delimiters[] ) :
     m_argv(0), m_orignalcommandlinestring(lpCmdLine)
{
   char *argument = NULL;
   std::vector<char> tempstr( ::strlen(lpCmdLine)+1 );
   memset( &tempstr[0], '\n', ::strlen(lpCmdLine)+1 );
   ::strcpy( &tempstr[0], lpCmdLine );
   argument = ::strtok( &tempstr[0], delimiters );

   if( argument )
   {
      m_argv.push_back( argument );   
      while( argument = ::strtok( NULL, delimiters ) )
      {
         if( !find( argument ) )          // Remove multiple instances of the same
         {                                // argument
            m_argv.push_back( argument );
         }
      }
   }
}

