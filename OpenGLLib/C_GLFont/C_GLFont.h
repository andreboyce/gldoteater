#pragma once

#ifndef __C_GLFont_H
#define __C_GLFont_H

#include "../C_GLOutLinedFont/C_GLOutLinedFont.h"
#include "../C_GLBitmapFont/C_GLBitmapFont.h"
#include "../C_GLTextureMappedFont/C_GLTextureMappedFont.h"

class C_GLFont
{
   public:
       C_GLFont() :
              m_GLOutLinedFont( NULL ),
              m_GLBitmapFont( NULL ),
              m_GLTextureMappedFont( NULL )
       {}

       ~C_GLFont(){}

       operator C_GLOutLinedFont( )
       {
          if( m_GLOutLinedFont == NULL )
            m_GLOutLinedFont = new C_GLOutLinedFont();
          return *m_GLOutLinedFont;
       }
       operator C_GLBitmapFont()
       {
          if( m_GLBitmapFont == NULL )
             m_GLBitmapFont = new C_GLBitmapFont();
          return *m_GLBitmapFont;
       }
       operator C_GLTextureMappedFont()
       {
          if( m_GLTextureMappedFont == NULL )
             m_GLTextureMappedFont = new C_GLTextureMappedFont();
          return *m_GLTextureMappedFont;
       }

   private:
       C_GLOutLinedFont      *m_GLOutLinedFont;
       C_GLBitmapFont        *m_GLBitmapFont;
       C_GLTextureMappedFont *m_GLTextureMappedFont;
   protected:
};

#endif
