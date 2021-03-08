#ifndef __CDll_H
#define __CDll_H

#include <windows.h>
#include <string>
#include "../errorutil/errorutil.h"

class CDll
{
   public:

      CDll() : m_Hinstance( NULL )
      {
      }

      CDll( std::string filename )
      {
         Load( filename );
      }

      ~CDll()
      {
         Destroy();
      }

      operator bool( void ) const
      {
         return (m_Hinstance != NULL );
      }

      operator HINSTANCE( void ) const
      {
         return m_Hinstance;
      }

      FARPROC GetProcAddress( std::string function_name )
      {
         if( m_Hinstance != NULL )
         {
            return ::GetProcAddress( (HMODULE)m_Hinstance, function_name.c_str() );
         }
         return (FARPROC)(NULL);
      }

      // something like
      // LoadLibrary( "filename.DLL" );
      bool Load( std::string filename )
      {
         Destroy();
         m_filename = filename;
         HINSTANCE m_Hinstance = ::LoadLibrary( filename.c_str() );
         if( m_Hinstance == NULL )
         {
            return false;
         }
         char module[MAX_PATH];
         ::GetModuleFileName( (HMODULE)m_Hinstance, (LPTSTR)module, MAX_PATH );
         m_filenameandpath = module;
      }

      void Destroy( void )
      {
         if( m_Hinstance )
         {
            ::FreeLibrary( (HMODULE)m_Hinstance );
         }
         m_filename.clear();
         m_filenameandpath.clear();
      }

      std::string GetFilenameAndPath( void ) const
      {
         return m_filenameandpath;
      }

      std::string GetFilename( void ) const
      {
         return m_filename;
      }

   protected:
      HINSTANCE   m_Hinstance;
      std::string m_filename;
      std::string m_filenameandpath;
      
   private:
};

#endif
