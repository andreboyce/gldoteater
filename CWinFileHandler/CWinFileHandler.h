#ifndef __CWinFileHandler_H
#define __CWinFileHandler_H

//#ifdef WIN32_LEAN_AND_MEAN
//#undef WIN32_LEAN_AND_MEAN
//#endif

#include <windows.h>
#include <commdlg.h> // WIN32_LEAN_AND_MEAN stops these from being defined 
                     // OFN_FILEMUSTEXIST OFN_PATHMUSTEXIST
#include <string>
#include <queue>

class CWinFileHandler
{
   public:

       CWinFileHandler( void ): m_FileOffset(0) {}
      ~CWinFileHandler( void ){}

       std::deque< std::string > GetMultiSelectFileNames( void );
       std::string GetCurrentDirectory( void );
       void SaveMultiSelectFileNames( char* buffer, int FileOffset );

       int SaveFile( std::string & path, char filter[],
                     char defexten[], const DWORD& flags,
                     const short& filterindex, char title[] );

       int OpenFile( std::string & path, 
                     char filter[] = "All files\0*.*\0\0",
                     char defexten[] = "*.*",
                     const DWORD& flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
                     const short& filterindex = 0,
                     char title[] = "Open" );


   protected:
      int                       m_FileOffset;
      std::string               m_CurrentDirectory;
      std::deque< std::string > m_FileNames;

   private:

};

#endif

