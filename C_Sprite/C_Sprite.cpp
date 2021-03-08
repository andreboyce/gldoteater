#include "C_Sprite.h"

C_Sprite::S_Frame& C_Sprite::GetFrame( int index )
{
   if( m_Frames.size() > index )
   {
      return m_Frames[index];
   }
}

bool C_Sprite::AddFrame( void )
{
   try
   {
      m_Frames.push_back( S_Frame() );
      return true;
   }
   catch( ... )
   {
      return false;
   }
}

