#pragma once

#ifndef __CollisionRectangle_H
#define __CollisionRectangle_H

class CollisionRectangle
{
   public:

       CollisionRectangle( void );
       CollisionRectangle( const CollisionRectangle& );
      ~CollisionRectangle( void );
      
       bool CheckCollide( const CollisionRectangle& CollisionRectangleRHS )
       {          
          return false;
       }
   
   protected:
       
       float width, height;
   
   private:
   
};

#endif
