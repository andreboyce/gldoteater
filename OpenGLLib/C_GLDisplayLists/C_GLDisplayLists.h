#pragma once

#ifndef __C_GLDisplayLists_H
#define __C_GLDisplayLists_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <algorithm>
#include <map>
#include <vector>
#include <string>

class C_GLDisplayList
{
   public:
      C_GLDisplayList() : m_list( 0 ) {}
      ~C_GLDisplayList(){ destroy(); }

      bool begin( GLenum mode = GL_COMPILE )
      {
         if( m_list )
            return false;

         m_list = ::glGenLists( 1 );
         if( m_list == GL_INVALID_VALUE )
            return false;

         if( ::glIsList( m_list ) == GL_FALSE )
            return false;

         ::glNewList( m_list, mode );
         return true;
      }

      void end( void )
      {
         ::glEndList();
      }

      void call( void )
      {
         ::glCallList( m_list );	 
      }

      void destroy( void )
      {
         if( ::glIsList( m_list ) != GL_FALSE )
         {
            ::glDeleteLists( m_list, 1 );
         }
         m_list = 0;
      }

      operator bool ( void ) const
      {
         if( ::glIsList( m_list ) == GL_FALSE )
            return false;
         return true;
      }

   protected:
      GLuint m_list;

   private:
};

class C_GLDisplayListManagerHash
{
   public:
      C_GLDisplayListManagerHash() : m_Index() {}
      ~C_GLDisplayListManagerHash(){ destroy(); }

      void clear( void )
      {
         destroy();
      }

      bool rebuild( std::string index, GLenum mode = GL_COMPILE )
      {
         if( m_DisplayListMap.find( m_Index ) != m_DisplayListMap.end() )
         {
            m_Index = index;
            if( m_DisplayListMap[ index ] != NULL )
            {
               if( m_DisplayListMap[ m_Index ] )
                  m_DisplayListMap[ m_Index ]->destroy();
               if( !m_DisplayListMap[ m_Index ]->begin( mode ) )
                  return false;
               return true;
            }
         }
         else
         {
            m_Index = index;
            try
            {
               m_DisplayListMap[index] = new C_GLDisplayList;
               if( m_DisplayListMap.find( index ) != m_DisplayListMap.end() )
               {
                  if( m_DisplayListMap[ index ] != NULL )
                  {
                     if( !m_DisplayListMap[index]->begin( mode ) )
                        return false;
                     return true;
                  }
               }
            }
            catch( ... )
            {
            }
         }
         return false;
      }

      void endrebuild( void )
      {
         if( m_DisplayListMap.find( m_Index ) != m_DisplayListMap.end() )
         {
            if( m_DisplayListMap[ m_Index ] != NULL )
            {
               m_DisplayListMap[ m_Index ]->end();
            }
         }
      }

      bool begin( std::string index, GLenum mode = GL_COMPILE )
      {
         try
         {
            m_DisplayListMap[index] = new C_GLDisplayList;

            if( m_DisplayListMap.find( index ) != m_DisplayListMap.end() )
            {
               if( m_DisplayListMap[ index ] != NULL )
               {
                  if( !m_DisplayListMap[index]->begin( mode ) )
                     return false;
                  return true;
               }
            }
         }
         catch( ... )
         {
         }
         return false;
      }

      void end()
      {
         if( m_DisplayListMap.find( m_Index ) != m_DisplayListMap.end() )
         {
            if( m_DisplayListMap[ m_Index ] != NULL )
            {
               m_DisplayListMap[m_Index]->end();
            }
         }
      }

      void call( std::string index )
      {
         if( m_DisplayListMap.find( index ) != m_DisplayListMap.end() )
         {
            if( m_DisplayListMap[ index ] != NULL )
            {
               m_DisplayListMap[ index ]->call();
            }
         }
      }

   protected:

      std::string m_Index;
      std::map< std::string, C_GLDisplayList* >                   m_DisplayListMap;
      typedef std::map< std::string, C_GLDisplayList* >::iterator DisplayListMapIterator;

      void destroy( void )
      {
         m_Index.clear();
         for( DisplayListMapIterator i = m_DisplayListMap.begin(); 
              i != m_DisplayListMap.end() ; i++ )
         {
            delete i->second;
            i->second = NULL;
         }
         m_DisplayListMap.clear();
      }

   private:
};

class C_GLDisplayListManager
{
   public:
      C_GLDisplayListManager() : m_Index( 0 ) {}
      ~C_GLDisplayListManager(){ destroy(); }

      void clear( void )
      {
         destroy();
      }

      bool rebuild( unsigned short index, GLenum mode = GL_COMPILE )
      {
         if( index < m_DisplayLists.size() ) 
         {
            m_Index = index;
            if( m_DisplayLists[ m_Index ] )
               m_DisplayLists[ m_Index ]->destroy();
//            return m_DisplayLists[ m_Index ]->begin( mode );
            if( !m_DisplayLists[ m_Index ]->begin( mode ) )
               return false;

         }
         return false;
      }

      void endrebuild( void )
      {
         if( m_DisplayLists.size() )
            m_DisplayLists[ m_Index ]->end();
      }

      bool begin( GLenum mode = GL_COMPILE )
      {
         try
         {
            m_DisplayLists.push_back( new C_GLDisplayList );
            if( !m_DisplayLists.back()->begin( mode ) )
               return false;
         }
         catch( ... )
         {
         }
         return true;
      }

      void end()
      {
         m_DisplayLists.back()->end();
      }

      void call( unsigned short index )
      {
         if( index < m_DisplayLists.size() ) 
            m_DisplayLists[ index ]->call();
      }

   protected:

      int m_Index;

      std::vector<C_GLDisplayList*> m_DisplayLists;

      template <class T> class deleteOne
      {
         public:
            void operator ()( T *& t )
            {
               delete t;
            }
      };

      void destroy( void )
      {
         deleteOne<C_GLDisplayList> deleteList;
         std::for_each( m_DisplayLists.begin(), m_DisplayLists.end(), deleteList );
         m_DisplayLists.clear();
      }

   private:
};

#endif

