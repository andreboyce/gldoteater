#ifndef __Particle_H
#define __Particle_H

#include <queue>
#include "../../MathVector/Vector3D/Vector3D.h"

class Particle
{
   public:
      Particle():
         Acceleration(), 
         Velocity(), 
         Position(),
         PastPositions(), 
         PositionsToMoveTo(),
         TimeSinceSpawning(0), 
         TimeToDieAt(0),
         data( NULL ),
         bIsVisible( false )
         {}
      Particle( const Particle& rhs ):
         Acceleration( rhs.Acceleration ), 
         Velocity( rhs.Velocity ), 
         Position( rhs.Position ),
         PastPositions( rhs.PastPositions ), 
         PositionsToMoveTo( rhs.PositionsToMoveTo ),
         TimeSinceSpawning( rhs.TimeSinceSpawning ), 
         TimeToDieAt( rhs.TimeToDieAt ),
         data( NULL ),
         bIsVisible( rhs.bIsVisible )
      {}
      Particle& operator=( const Particle& rhs )
      {
         Acceleration      = rhs.Acceleration;
         Velocity          = rhs.Velocity; 
         Position          = rhs.Position;
         PastPositions     = rhs.PastPositions;
         PositionsToMoveTo = rhs.PositionsToMoveTo;
         TimeSinceSpawning = rhs.TimeSinceSpawning;
         TimeToDieAt       = rhs.TimeToDieAt;
         data              = NULL;
         bIsVisible        = rhs.bIsVisible;
         return *this;
      }
      ~Particle(){}
      Vector3D Acceleration, Velocity, Position;
      std::deque<Vector3D> PastPositions, PositionsToMoveTo;
      unsigned int TimeSinceSpawning, TimeToDieAt;
      void *data;
      bool bIsVisible;
   protected:
   private:
};

#endif

