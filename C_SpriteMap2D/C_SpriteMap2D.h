#ifndef __C_SpriteMap2D_H
#define __C_SpriteMap2D_H

#include <queue>
#include <string>

#include "../errorutil/errorutil.h"
#include "../Structs/Structs.h"
#include "../CHBITMAP/CHBITMAP.h"

#include "../NonStandardHeaders/boost/tokenizer.hpp"
//#include <boost/tokenizer.hpp>


namespace SpriteMap
{

struct SpriteID
{
   SpriteID() : id (0), pos() {}
   SpriteID( const SpriteID& rhs ) : id( rhs.id ), pos( rhs.pos ) {}
   SpriteID( short _id, Structs::Point _pos ) : id( _id ), pos( _pos ) {}
   SpriteID& operator = ( const SpriteID& rhs )
   {
      id  = rhs.id;
      pos = rhs.pos;
      return *this;
   }
   short             id;
   Structs::Point pos;
};

typedef std::deque< SpriteID > D_Sprite;
typedef std::deque< CHBITMAP > D_HBitmaps;

class C_SpriteMap2D
{
   public:
      C_SpriteMap2D();
      C_SpriteMap2D( const C_SpriteMap2D& rhs );
      ~C_SpriteMap2D();

      bool ReloadBitmap( int index );
      bool ReplaceBitmap( int index, std::string filename );


      bool Load( std::string filename );
      bool Save( std::string filename );

      bool LoadSpriteSet( std::string filename );
      bool SaveSpriteSet( std::string filename );

      bool AddSprite( std::string filename );
      bool AddBitmap( std::string filename );

      void SetHwnd( HWND Hwnd )
      {
         m_Hwnd = Hwnd;
      }

      long size( void )
      {
         return m_SpriteMap.size();
      }
      SpriteID& operator[]( int index )
      {
         return m_SpriteMap[index];
      }
      D_Sprite& GetSprites( void )
      {
         return m_SpriteMap;
      }
      std::string GetFileName( void )
      {
         return m_SpriteMapFileName;
      }
      void SetFileName( std::string FileName )
      {
         m_SpriteMapFileName = FileName;
      }

      D_HBitmaps& GetBitmaps( void )
      {
         return m_Hbitmaps;
      }

      void clear()
      {
         m_Hbitmaps.clear();
         m_SpriteMap.clear();
         m_SpriteMapFileName.clear();
         m_SpriteSetFileName.clear();
         m_SpriteSet.clear();
         m_Hwnd = NULL;
      }

   protected:
      D_HBitmaps  m_Hbitmaps;
      D_Sprite    m_SpriteMap;
      std::string m_SpriteMapFileName;
      std::string m_SpriteSetFileName;
      std::deque< std::string >  m_SpriteSet;
      HWND        m_Hwnd;

   private:
};

};
#endif

