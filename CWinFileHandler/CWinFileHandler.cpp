#include "CWinFileHandler.h"

int CWinFileHandler::SaveFile( std::string & path, char filter[],
                                char defexten[], const DWORD& flags,
                                const short& filterindex, char title[] )
{
   short buflen = MAX_PATH+1;
   std::vector<char>  buffer(buflen);
   ::ZeroMemory( &buffer[0], buflen );

   OPENFILENAME ofn;
   ::ZeroMemory( &ofn, sizeof(ofn) );

   ofn.lStructSize  = sizeof(ofn);
   ofn.hInstance    = ::GetModuleHandle( NULL );
   ofn.hwndOwner    = ::GetActiveWindow();

   ofn.lpstrFile    = &buffer[0];
   ofn.nMaxFile     = buflen;
   ofn.lpstrTitle   = title;
   ofn.nFilterIndex = filterindex;
   ofn.lpstrDefExt  = defexten;

   ofn.Flags        = flags;
   ofn.lpstrFilter  = filter;

   int iRetValue    = ::GetSaveFileName( &ofn );
   path = &buffer[0];
   return iRetValue;
}

int CWinFileHandler::OpenFile( std::string & path, char filter[],
                                char defexten[], const DWORD& flags,
                                const short& filterindex, char title[] )
{
   short buflen = MAX_PATH+1;
   std::vector<char>  buffer(buflen);
   ::ZeroMemory( &buffer[0], buflen );

   OPENFILENAME ofn;
   ::ZeroMemory( &ofn, sizeof(ofn) );

   ofn.lStructSize  = sizeof(ofn);
   ofn.hInstance    = ::GetModuleHandle( NULL );
   ofn.hwndOwner    = ::GetActiveWindow();

   ofn.lpstrFile    = &buffer[0];
   ofn.nMaxFile     = buflen;
   ofn.lpstrTitle   = title;
   ofn.nFilterIndex = filterindex;
   ofn.lpstrDefExt  = defexten;

   ofn.Flags        = flags;
   ofn.lpstrFilter  = filter;

   int iRetValue    = ::GetOpenFileName( &ofn );
   path = &buffer[0];
   if( flags & OFN_ALLOWMULTISELECT )
   {
      SaveMultiSelectFileNames( &buffer[0], ofn.nFileOffset );
   }
   return iRetValue;
}

// dont call this function unless (flags & OFN_ALLOWMULTISELECT) is true
void CWinFileHandler::SaveMultiSelectFileNames( char* buffer, int FileOffset )
{
   m_FileNames.clear();
   m_CurrentDirectory.clear();
   std::vector<char> temp( FileOffset+1 );
   temp[FileOffset] = '\0';
   std::strncpy( &temp[0], buffer, FileOffset );
   m_CurrentDirectory = &temp[0];
   m_CurrentDirectory += "\\";
   buffer += FileOffset;
   while( buffer[0] != '\0' )
   {
      m_FileNames.push_back( buffer );
      buffer += m_FileNames.back().size()+1;
   }
}

std::deque< std::string > CWinFileHandler::GetMultiSelectFileNames( void )
{
   return m_FileNames;
}

std::string CWinFileHandler::GetCurrentDirectory( void )
{
   return m_CurrentDirectory;
}

