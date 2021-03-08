#include "C_GLSpriteAnimationSequence.h"

C_GLSpriteAnimationSequence::C_GLSpriteAnimationSequence( void ): textureIDs(0)
{
   textureIndex = 0;
   duration     = 1.0f;
   frames       = 1;
   prevtime     = 0;
}

C_GLSpriteAnimationSequence::~C_GLSpriteAnimationSequence( void )
{
}

void C_GLSpriteAnimationSequence::Update( DWORD currenttime )
{
   if( ( (currenttime - prevtime) ) >= ((1.0f/(float)(frames))*1000.0f*duration) )
   {
      if( (textureIndex+1) < textureIDs.size() )  // +1 for zero indexing
      {      
         textureIndex++;
      }
      else
      {
         textureIndex = 0;      
      }
      prevtime = currenttime;      
   }
}

void C_GLSpriteAnimationSequence::AddTextureSequence( V_GLuint& newtextureIDs, 
                                                      const float& newduration )
{
   textureIDs = newtextureIDs;
   frames     = textureIDs.size();
   duration   = newduration;
}


