#pragma once

#ifndef __Vector2D_H
#define __Vector2D_H

//#include "../C_UnitVector/C_UnitVector.h"

#include <sstream>

class Vector2D
{
   public:
      Vector2D( void ) : m_x( 0.0f ), m_y( 0.0f ){}

      Vector2D( const double darray[2] ) :
         m_x( darray[0] ),
         m_y( darray[1] )
      {
      }

      Vector2D( const Vector2D & rhs ) :
         m_x( rhs.m_x ),
         m_y( rhs.m_y )
      {
      }

      Vector2D( double x, double y ) :
         m_x ( x ),
         m_y ( y )
      {
      }

      ~Vector2D( void ){}

      inline void SetX ( const double x )
      {
         m_x = x;
      }

      inline void SetY ( const double y )
      {
         m_y = y;
      }


      inline Vector2D & operator = ( const Vector2D & rhs )
      {
         m_x = rhs.m_x;
         m_y = rhs.m_y;
         return *this;
      }

      inline bool operator == ( const Vector2D & rhs )
      {
         if( (m_x == rhs.m_x) && (m_y == rhs.m_y) )
            return true;
         return false;
      }

      inline Vector2D & operator += ( const Vector2D & rhs )
      {
         m_x += rhs.m_x;
         m_y += rhs.m_y;
         return *this;
      }

      inline Vector2D & operator += ( const double & scalar )
      {
         m_x += scalar;
         m_y += scalar;
         return *this;
      }

      inline Vector2D & operator -= ( const Vector2D & rhs )
      {
         m_x -= rhs.m_x;
         m_y -= rhs.m_y;
         return *this;
      }

      inline Vector2D & operator -= ( const double & scalar )
      {
         m_x -= scalar;
         m_y -= scalar;
         return *this;
      }

      inline const Vector2D  operator - ( const Vector2D & rhs ) const
      {
         return Vector2D( m_x - rhs.m_x,  m_y - rhs.m_y );
      }

      inline const Vector2D operator - ( const double & scalar ) const
      {
         return Vector2D( m_x - scalar,  m_y - scalar );
      }

      inline const Vector2D operator - ( void ) const
      {
         return Vector2D( -m_x, -m_y );
      }

      inline const Vector2D  operator + ( const Vector2D & rhs ) const
      {
         return Vector2D( m_x + rhs.m_x,  m_y + rhs.m_y );
      }

      inline const Vector2D operator + ( const double & scalar ) const
      {
         return Vector2D( m_x + scalar,  m_y + scalar );
      }

      inline const Vector2D operator + ( void ) const
      {
         return Vector2D(*this);
      }

      inline operator bool ( void )
      {
         return true;
      }

      // Vector multiplication
      inline const Vector2D operator * ( const Vector2D& rhs ) const
      {
         return Vector2D( m_x * rhs.m_x, m_y * rhs.m_y );
      }

      // Vector multiplication
      inline const Vector2D operator * ( double scalar ) const
      {
         return Vector2D( m_x * scalar, m_y * scalar );
      }

      // Vector multiplication
      inline const Vector2D & operator *= ( const Vector2D& rhs )
      {
         m_x *= rhs.m_x;
         m_y *= rhs.m_y;
         return *this;
      }

      // Vector multiplication
      inline const Vector2D & operator *= ( double scalar )
      {
         m_x *= scalar;
         m_y *= scalar;
         return *this;
      }

      // Vector division
      inline const Vector2D operator / ( const Vector2D& rhs ) const
      {
         return Vector2D( m_x / rhs.m_x, m_y / rhs.m_y );
      }

      // Vector division
      inline const Vector2D operator / ( float scalar ) const
      {
         return Vector2D( m_x / scalar, m_y / scalar );
      }

      // Vector division
      inline const Vector2D& operator /= ( const Vector2D & rhs )
      {
         m_x /= rhs.m_x;
         m_y /= rhs.m_y;
         return *this;
      }

      // Vector division
      inline const Vector2D& operator /= ( double scalar )
      {
         m_x /= scalar;
         m_y /= scalar;
         return *this;
      }

      // Return component
      inline double operator [] ( const long index ) const
      {
         if( index == 0 ) return m_x;
         if( index == 1 ) return m_y;
      }

		// Reset coordinate
      inline void Zero( void )
      {
         m_x = 0.0f;
         m_y = 0.0f;
      }

      operator std::string ( void )
      {
         std::ostringstream tmp;
         tmp << "{ x: " << m_x << "," << " y: " << m_y << " }";
         return tmp.str();
      }

      double m_x, m_y;
};

#endif
