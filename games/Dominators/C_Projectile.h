#pragma once

#ifndef __C_Projectile_H
#define __C_Projectile_H

#include "../../MathVector/Vector2D/Vector2D.h"

class C_Projectile
{
   public:
      C_Projectile(){}
      C_Projectile( const C_Projectile& rhs ) :
         m_PositionVector(     rhs.m_PositionVector ),
         m_VelocityVector(     rhs.m_VelocityVector ),
         m_AccelerationVector( rhs.m_AccelerationVector )
      {}
      ~C_Projectile(){}

      Vector2D  m_PositionVector,     // Position
                m_VelocityVector,     // Velocity
                m_AccelerationVector; // Alleceration
   protected:
   private:
};

#endif

