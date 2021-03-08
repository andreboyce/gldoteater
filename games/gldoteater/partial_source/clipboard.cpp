#include "main.h"

void MainWindow::CopyTextToClipBoard( std::string text )
{
   LPTSTR  lptstrCopy = NULL;
   HGLOBAL hglbCopy   = NULL;
   int     length     = (text.size()+1) * sizeof(TCHAR);

   try
   {
      if( !::OpenClipboard( m_WinBase ) )
         throw std::string( "Could Not Open ClipBoard " );

      if( !::EmptyClipboard() )
         throw std::string( "Could Not Empty Clip Board " );

      hglbCopy = ::GlobalAlloc( GMEM_MOVEABLE, length );
      if( hglbCopy == NULL )
         throw std::string( "Not Enough Memory to Copy String to ClipBoard " );
       
      if( ( lptstrCopy = (TCHAR*)::GlobalLock( (HGLOBAL)hglbCopy ) ) == NULL )
         throw std::string( "Unable to LockMemory" );

      memcpy( lptstrCopy, text.c_str(), length-1 );
      lptstrCopy[length] = 0;

      ::GlobalUnlock( (HGLOBAL)hglbCopy );
//      if( !::GlobalUnlock( (HGLOBAL)hglbCopy ) )
//         throw std::string( "Unable to UnLockMemory " );

      if( ::SetClipboardData( CF_TEXT, (HANDLE)hglbCopy ) == NULL )
         throw std::string( "Unable To Set ClipBoard Data " );

      if( !::CloseClipboard() )
         throw std::string( "Could Not Close ClipBoard " );
   }
   catch ( std::string error_msg )
   {
      m_Console.AddLine( std::string( "Error : " ) + error_msg + GetLastSystemError() );
      ::CloseClipboard();
   }
}

std::string MainWindow::GetTextFromClipBoard( bool bAddTextToConsole )
{
   std::string text;
   LPTSTR  lptstrCopy = NULL;
   HGLOBAL hglbCopy   = NULL;

   try
   {
      if( !::IsClipboardFormatAvailable( CF_TEXT ) )
         throw 0;

      if( !::OpenClipboard( m_WinBase ) )
         throw std::string( "Could Not Open ClipBoard " );

      if( (hglbCopy = ::GetClipboardData( CF_TEXT )) == NULL )
         throw std::string( "Unable To Get ClipBoard Data " );

      if( (lptstrCopy = (LPTSTR)::GlobalLock(hglbCopy)) == NULL )
         throw std::string( "Clould Not Lock ClipBoard Data " );

      text = lptstrCopy;
//      Alert( text );
      if( bAddTextToConsole )
      {
         m_Console.AddString( text );
      }

      ::GlobalUnlock( (HGLOBAL)hglbCopy );
//      if( !::GlobalUnlock( (HGLOBAL)hglbCopy ) )
//         throw std::string( "Unable to UnLockMemory " );

      if( !::CloseClipboard() )
         throw std::string( "Could Not Close ClipBoard " );
   }
   catch ( std::string error_msg )
   {
      m_Console.AddLine( std::string( "Error : " ) + error_msg + GetLastSystemError() );
      ::CloseClipboard();
   }
   catch( int i )
   {
      MsgBeep( 0xFFFFFFFF );
   }
   return text;
}


