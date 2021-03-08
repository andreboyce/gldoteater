#ifndef __C_GLSpriteAnimationSequence_H
#define __C_GLSpriteAnimationSequence_H

#include <windows.h>
#include <vector>
#include <gl/glu.h>

#include "../../../C_GLTextureManager/C_GLTextureManager.h"

// TODO (#1#): Add some code for interpolated rotation 


class C_GLSpriteAnimationSequence
{

   public:
   
       typedef std::vector<GLuint> V_GLuint;
       typedef V_GLuint::iterator  V_GLuintIterator;
   

       C_GLSpriteAnimationSequence( void );
      ~C_GLSpriteAnimationSequence( void );
      
       void Update( DWORD ); // All sprites get the same time       
       void AddTextureSequence( V_GLuint&, const float & );
       
       V_GLuint::reference inline GetCurrentTexture( void )
          {
             GLuint a = ERROR_CNLT;
             V_GLuint::reference t = a;
             if( textureIDs.at(textureIndex) )
             {
                return textureIDs.at(textureIndex);
             }
             return t;
          }

       void SetDuration( const float& newduration )
       {
          duration = newduration;
       }       

   protected:

      float     duration;
      short     frames;
      short     textureIndex;
      DWORD     prevtime;
      V_GLuint  textureIDs;

   private:

};

#endif

