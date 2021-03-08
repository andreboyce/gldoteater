#ifndef __SpriteAnimation_H
#define __SpriteAnimation_H

#include <string>
#include <sstream>
#include "TextureList.h"
#include "../../TimeInterval/TimeInterval.h"

#include <cstring>

class SpriteAnimation
{
   public:
      SpriteAnimation() : m_time_elapsed( 0 ), m_current_frame( 0 ), animation_complete( false ){}
      ~SpriteAnimation(){}
      // continues on as if nothing happened if restart is false
      // resets the animation if restart is true
      void play( bool restart = false )
      {
         if( animation_complete )
            restart = true;
         if( restart )
         {
            m_current_frame = 0;
            m_time_elapsed  = 0;
            m_Interval.ResetTime();
            animation_complete = false;
         }
      }
      int update( bool loop = true )
      {
         unsigned int time_elapsed = m_Interval.TimeSinceLastCall();
         unsigned int m_elapsed_time_of_previous_frames = 0;
         for( unsigned int i=0; i<m_current_frame ; i++ )
         {
            if( i<m_Frames.size() )
            {
               m_elapsed_time_of_previous_frames += m_Frames[i].duration;
            }
         }
         m_time_elapsed += time_elapsed;
         if( m_Frames.size() > m_current_frame )
         {
            if( (m_time_elapsed-m_elapsed_time_of_previous_frames) > m_Frames[m_current_frame].duration )
            {
               m_current_frame++;
            }
            if( m_current_frame >= m_Frames.size() )
            {
               m_time_elapsed = 0;
               animation_complete = true;
               if( loop )
               {
                  m_current_frame = 0;
               }
               else
               {
                   m_current_frame--;
               }
            }
//            else
//            {
//                animation_complete = false;
//            }
         }
         m_Interval.Begin();
         return m_current_frame;
      }
      unsigned int m_time_elapsed;
      unsigned int m_current_frame;
      TimeInterval m_Interval;
      bool animation_complete;
      unsigned int total_duration()
      {
         unsigned int m_elapsed_time_of_previous_frames = 0;
         for( unsigned int i=0; i<m_Frames.size() ; i++ )
         {
            m_elapsed_time_of_previous_frames += m_Frames[i].duration;
         }
         return m_elapsed_time_of_previous_frames;
      }


      struct Frame
      {
         int duration;
         std::string texture_name;
         void clear()
         {
            duration     = 0;
            texture_name.clear();
         }
         Frame() : 
                duration( 0 )
         {}
         ~Frame(){}
         Frame( int _duration, std::string _texture_name ) : 
                duration( _duration ), texture_name( _texture_name )
         {}
         Frame( const Frame& rhs ) : 
                duration( rhs.duration ), texture_name( rhs.texture_name )
         {}
         Frame& operator=( const Frame& rhs )
         {
            duration     = rhs.duration;
            texture_name = rhs.texture_name;
            return *this;
         }
         operator std::string( void )
         {
            std::ostringstream tmp;
            tmp << "{ duration: " << duration << ", texture_name: " << texture_name << " }";
            return tmp.str();
         }
         bool read( std::ifstream& in_file )
         {
            if( !in_file.is_open() )
               return false;
            in_file.read( (char*)&duration, sizeof(duration) );
            std::string::size_type length = 0;
            in_file.read( (char*)&length, sizeof(length) );
			std::vector<char> str(length+1);
            std::memset( &str[0], 0, length+1 );
            in_file.read( (char*)&str[0], length );
            texture_name = &str[0];
            return true;
         }
         bool write( std::ofstream& out_file )
         {
            if( !out_file.is_open() )
               return false;
            out_file.write( (char*)&duration, sizeof(duration) );
            std::string::size_type length = texture_name.length();
            out_file.write( (char*)&length, sizeof(length) );
            out_file.write( (char*)texture_name.c_str(), texture_name.length() );
            return true;
         }

      };
      bool read( std::string filename )
      {
         clear();
         std::ifstream in_file( filename.c_str(), std::ios::in | std::ios::binary );
         if( !in_file.is_open() )
            return false;
         std::string::size_type length = 0;
         in_file.read( (char*)&length, sizeof(length) );
		 std::vector<char> str(length+1);
         std::memset ( &str[0], '\0', length+1 );
         in_file.read( (char*)&str[0], length );
         m_TextureListFileName = &str[0];
         m_TextureList.read( m_TextureListFileName );

         std::deque<Frame>::size_type frames = 0;
         in_file.read( (char*)&frames, sizeof(frames) );
         for( unsigned int i=0; i<frames ; i++ )
         {
            m_Frames.push_back( Frame() );
            m_Frames.back().read( in_file );
         }         
         return true;
      }
      bool write( std::string filename )
      {
         std::ofstream out_file( filename.c_str(), std::ios::out | std::ios::binary );
         if( !out_file.is_open() )
            return false;

         std::string::size_type length = m_TextureListFileName.length();
         out_file.write( (char*)&length, sizeof(length) );
         out_file.write( (char*)m_TextureListFileName.c_str(), m_TextureListFileName.length() );
         m_TextureList.write( m_TextureListFileName );
         std::deque<Frame>::size_type frames = m_Frames.size();
         out_file.write( (char*)&frames, sizeof(frames) );
         for( unsigned int i=0; i<m_Frames.size() ; i++ )
         {
            m_Frames[i].write( out_file );
         }
         return true;
      }
      void clear()
      {
         m_TextureList.clear();
         m_Frames.clear();
         m_TextureListFileName.clear();
      }
      TextureList         m_TextureList;
      std::deque< Frame > m_Frames;
      std::string         m_TextureListFileName;
   protected:
   private:
};

#endif
