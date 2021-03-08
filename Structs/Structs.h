#pragma once

#ifndef __Structs_H
#define __Structs_H

#include <windows.h>

#include <string>
#include <sstream>

namespace Structs
{

struct Rect: public RECT
{
   Rect( void ) { left = 0; top = 0; right = 0; bottom = 0; }
   Rect( LONG _left, LONG _top, LONG _right, LONG _bottom ) { left = _left; top = _top; right = _right; bottom = _bottom; }
   Rect( const RECT &rhs ) { left = rhs.left; top = rhs.top; right = rhs.right; bottom = rhs.bottom; }
   operator std::string ( void )
   {
      std::ostringstream tmp;
      tmp << "{ left: " << left << "," << " top: " << top << "," << " right: " << right << "," << " bottom: " << bottom << " }";
      return tmp.str();
   }
};
struct dRect
{
   dRect( void ) : left(0), top(0), right(0), bottom(0) {}
   dRect( double _left, double _top, double _right, double _bottom ) { left = _left; top = _top; right = _right; bottom = _bottom; }
   double left, top, right, bottom;
   operator std::string ( void )
   {
      std::ostringstream tmp;
      tmp << "{ left: " << left << "," << " top: " << top << "," << " right: " << right << "," << " bottom: " << bottom << " }";
      return tmp.str();
   }
};
struct fRect
{
   fRect( void ) : left(0), top(0), right(0), bottom(0) {}
   fRect( float _left, float _top, float _right, float _bottom ) { left = _left; top = _top; right = _right; bottom = _bottom; }
   float left, top, right, bottom;
   operator std::string ( void )
   {
      std::ostringstream tmp;
      tmp << "{ left: " << left << "," << " top: " << top << "," << " right: " << right << "," << " bottom: " << bottom << " }";
      return tmp.str();
   }
};
struct fRect3D
{
   fRect3D( void ) : left(0.0), top(0.0), right(0.0), bottom(0.0), m_z(0.0) {}
   fRect3D( float _left, float _top, float _right, float _bottom, float z ) { left = _left; top = _top; right = _right; bottom = _bottom; m_z = z; }
   float left, top, right, bottom, m_z;
   operator std::string ( void )
   {
      std::ostringstream tmp;
      tmp << "{ left: " << left << "," << " top: " << top << "," << " right: " << right << "," << " bottom: " << bottom << "," << " z: " << m_z << " }";
      return tmp.str();
   }
};
struct Point: public POINT
{
   Point( void ){ x=0; y=0; }
   Point( LONG _x, LONG _y ){ x = _x; y = _y; }
   operator std::string ( void )
   {
      std::ostringstream tmp;
      tmp << "{ x: " << x << "," << " y: " << y << " }";
      return tmp.str();
   }
   Point& operator -= ( const Point & p )
   {
      x -= p.x;
      y -= p.y;
      return *this;
   }
   Point& operator += ( const Point & p )
   {
      x += p.x;
      y += p.y;
      return *this;
   }
   bool operator != ( const Point & p )
   {
      if( ( x != p.x ) || ( y != p.y ) )
         return true;
      return false;
   }
   bool operator == ( const Point & p )
   {
      if( ( x == p.x ) && ( y == p.y ) )
         return true;
      return false;
   }
   bool operator >= ( const long p )
   {
      if( ( x >= p ) && ( y >= p ) )
         return true;
      return false;
   }
   bool operator <= ( const long p )
   {
      if( ( x <= p ) && ( y <= p ) )
         return true;
      return false;
   }
   bool operator < ( const long p )
   {
      if( ( x < p ) && ( y < p ) )
         return true;
      return false;
   }
   bool operator > ( const long p )
   {
      if( ( x > p ) && ( y > p ) )
         return true;
      return false;
   }
};
struct dPoint
{
   dPoint(void){ m_x=0; m_y=0; }
   dPoint( double _x, double _y ){ m_x = _x; m_y = _y; }
   double m_x, m_y;
   operator std::string ( void )
   {
      std::ostringstream tmp;
      tmp << "{ x: " << m_x << "," << " y: " << m_y << " }";
      return tmp.str();
   }
};
struct fPoint
{
   fPoint(void){ m_x=0; m_y=0; }
   fPoint( float _x, float _y ){ m_x = _x; m_y = _y; }
   float m_x, m_y;
   operator std::string ( void )
   {
      std::ostringstream tmp;
      tmp << "{ x: " << m_x << "," << " y: " << m_y << " }";
      return tmp.str();
   }
};
struct Point3D: public Point
{
   Point3D(void){ x=0; y=0; z=0; }
   Point3D( LONG _x, LONG _y, LONG _z ){ x = _x; y = _y; z = _z; }
   LONG z;
   operator std::string ( void )
   {
      std::ostringstream tmp;
      tmp << "{ x: " << x << "," << " y: " << y << "," << " z: " << z << " }";
      return tmp.str();
   }
};
struct dPoint3D
{
   dPoint3D(void) : m_x( double() ), m_y( double() ) ,m_z( double() ) {}
   dPoint3D( double x, double y, double z ){ m_x = x; m_y = y; m_z = z; }
   double m_x, m_y, m_z;
   operator std::string ( void )
   {
      std::ostringstream tmp;
      tmp << "{ x: " << m_x << "," << " y: " << m_y << "," << " z: " << m_z << " }";
      return tmp.str();
   }
};

}
#endif
