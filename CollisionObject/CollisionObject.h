#pragma once

#ifndef __CollisionObject_H
#define __CollisionObject_H

#include <vector>
#include "CollisionRectangle/CollisionRectangle.h"

class CollisionObject
{
   typedef std::vector< CollisionRectangle > V_CollisionRectangle;
   
   public:
   
       CollisionObject( void );
      ~CollisionObject( void );

   protected:
   
      V_CollisionRectangle  CollisionRectangles;

   private:

};

#endif
