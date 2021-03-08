#include "C_SpriteMap2D.h"

namespace SpriteMap
{

C_SpriteMap2D::C_SpriteMap2D() :
   m_Hwnd( NULL )
{
}

C_SpriteMap2D::C_SpriteMap2D( const C_SpriteMap2D& rhs ) :
   m_SpriteMapFileName( rhs.m_SpriteMapFileName ),
   m_Hbitmaps( rhs.m_Hbitmaps ),
   m_SpriteMap( rhs.m_SpriteMap ),
   m_SpriteSetFileName( rhs.m_SpriteSetFileName ),
   m_SpriteSet( rhs.m_SpriteSet ),
   m_Hwnd( rhs.m_Hwnd )
{}

C_SpriteMap2D::~C_SpriteMap2D()
{
   clear();
}

bool C_SpriteMap2D::ReloadBitmap( int index )
{
   if( ( m_Hbitmaps.size() > index ) && ( m_SpriteSet.size() > index ) )
   {
      return m_Hbitmaps[index].Load( m_SpriteSet[index] );
   }
   return false;
}

bool C_SpriteMap2D::ReplaceBitmap( int index, std::string filename )
{
   if( ( m_Hbitmaps.size() > index ) && ( m_SpriteSet.size() > index ) )
   {
      m_SpriteSet[index] = filename;
      bool ret =  m_Hbitmaps[index].Load( m_SpriteSet[index] );
      m_Hbitmaps[index].SetHwnd( m_Hwnd );
      return ret;
   }
   return false;
}

bool C_SpriteMap2D::AddSprite( std::string filename )
{
   bool ret = AddBitmap( filename );
   if( ret )
   {
      try
      {
         m_SpriteSet.push_back( filename );
      }
      catch( ... )
      {
         return false;
      }
   }
   return ret;
}

bool C_SpriteMap2D::AddBitmap( std::string filename )
{
   try
   {
      m_Hbitmaps.push_back( CHBITMAP() );
      if( !m_Hbitmaps.back().Load( filename ) )
      {
         m_Hbitmaps.pop_back();
         return false;
      }
      m_Hbitmaps.back().SetHwnd( m_Hwnd );
   }
   catch( ... )
   {
      return false;
   }
   return true;
}

bool C_SpriteMap2D::LoadSpriteSet( std::string filename )
{
   std::ifstream spriteset( filename.c_str(), std::ios::in );
   try
   {
      if( !spriteset.is_open() )
         return false;
      std::string bitmapfilename;

      while( !std::getline( spriteset, bitmapfilename ).eof() )
      {
         if( AddBitmap( bitmapfilename ) )
         {
            m_SpriteSet.push_back( bitmapfilename );
         }
      }
      spriteset.close();
      return true;
   }
   catch( ... )
   {
      spriteset.close();
      return false;
   }
}

bool C_SpriteMap2D::SaveSpriteSet( std::string filename )
{
   std::ofstream spritesetfile( filename.c_str(), std::ios::out );
   if( !spritesetfile.is_open() )
      return false;
   for( int i = 0; i<m_SpriteSet.size() ; ++i )
   {
      spritesetfile << m_SpriteSet[i] << std::endl;
   }
   spritesetfile.close();
   return true;
}


bool C_SpriteMap2D::Load( std::string filename )
{
   try
   {
      m_SpriteMap.clear();
      std::ifstream spritemap( filename.c_str(), std::ios::in );
      if( !spritemap.is_open() )
         return false;

      m_SpriteMapFileName = filename;
      std::getline( spritemap, m_SpriteSetFileName );
      if( !LoadSpriteSet( m_SpriteSetFileName ) )
      {
         spritemap.close();
      }

      std::string line; // will contain a line of sprite id's
      SpriteID    sprite_id;

      while( !std::getline( spritemap, line ).eof() )
      {
         boost::escaped_list_separator<char> dlim( '\\', ',', '\"' );
         boost::tokenizer< boost::escaped_list_separator<char> > sprites( line, dlim );
         boost::tokenizer< boost::escaped_list_separator<char> >::iterator beg = sprites.begin();
         // ID
         if( beg != sprites.end() )
         {
            if( (*beg).size() )
            {
               sprite_id.id    = ::atoi( (*beg).c_str() );
            }
            else
            {
            }
            beg++;
         }
         // X coordinate in map
         if( beg != sprites.end() )
         {
            if( (*beg).size() )
            {
               sprite_id.pos.x = ::atoi( (*beg).c_str() );
            }
            else
            {
            }
            beg++;
         }
         // Y coordinate in map
         if( beg != sprites.end() )
         {
            if( (*beg).size() )
            {
               sprite_id.pos.y = ::atoi( (*beg).c_str() );
            }
            else
            {
            }
            beg++;
         }
         m_SpriteMap.push_back( sprite_id );
      }
      return true;
   }
   catch( ... )
   {
      return false;
   }
}

bool C_SpriteMap2D::Save( std::string filename )
{
   try
   {
      std::ofstream spritemap( filename.c_str(), std::ios::out );
      if( !spritemap.is_open() )
         return false;
      std::ostringstream line;

      spritemap << m_SpriteSetFileName << std::endl;
      SaveSpriteSet( m_SpriteSetFileName );

      for( int i = 0; i < m_SpriteMap.size(); i++ )
      {
         line << m_SpriteMap[i].id << "," << m_SpriteMap[i].pos.x << "," << m_SpriteMap[i].pos.y;
         spritemap << line.str() << std::endl;
         line.str( std::string() );
      }
      spritemap.close();
   }
   catch( ... )
   {
      return false;
   }
   return true;
}

};

