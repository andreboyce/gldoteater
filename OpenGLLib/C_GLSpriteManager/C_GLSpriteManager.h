#ifndef __C_GLSpriteManager_H
#define __C_GLSpriteManager_H

#include <vector>
#include "C_GLSprite/C_GLSprite.h"

class C_GLSpriteManager
{

   typedef std::vector<C_GLSprite> V_C_GLSprite;
   typedef V_C_GLSprite::iterator  V_C_GLSpriteIterator;
   
   public:

       C_GLSpriteManager( void );
      ~C_GLSpriteManager( void );
       
       // Regular members
       
       void AddSprites( const short& n=1 )
          { Sprites.resize( Sprites.size()+n ); }
       
       void AddSprite( const C_GLSprite& NewSprite )
          { Sprites.push_back( NewSprite ); }
          
       void RemoveSprite( short index )
       {
          V_C_GLSpriteIterator SpritesIterator = Sprites.begin() + index;
          Sprites.erase( SpritesIterator );
       }
       
       short inline Size( void ) const
       {
          return Sprites.size();
       }
       
//       void Draw( float x, float y, float z )
//          {
//             for( int i=0; i < Sprites.size(); i++ )
//             {
//             
//             }
//          }
       
       bool DetectCollisions( void )
          {
//             int i;
//             for( i=0; i<Sprites.size() ; i++)
//             {
//                for( c=i+1; c<Sprites.size() ; c++)
//                {             
//                   Sprites[i].CollisionObjs
//                }
//             }
          }
          
       void Clear( void )
          { Sprites.clear(); }
       
       // Operators
       
       std::vector<C_GLSprite>::reference operator [] ( short index )
       {
          return Sprites.at(index);
       }

   protected:
   
      V_C_GLSprite Sprites;

   private:

};

#endif
