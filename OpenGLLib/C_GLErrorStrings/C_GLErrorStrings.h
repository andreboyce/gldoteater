#pragma once

#ifndef __C_GLErrorStrings_H
#define __C_GLErrorStrings_H

#include <windows.h>
#include <gl/glu.h>
#include <string>
#include <stack>

class C_GLErrorStrings
{
   public:
      C_GLErrorStrings(){}
      ~C_GLErrorStrings(){}
   
      static std::string GetError( void )
      {
         GLenum error_id = ::glGetError();
         return GetErrorString( error_id );
      }
   
      static std::string GetErrorString( GLenum error_id )
      {
         std::string return_value;
         switch( error_id )
         {
            case GL_NO_ERROR:
            {
               return_value = "No error has been recorded. The value of this symbolic constant is guaranteed to be zero.";
            } break;

            case GL_INVALID_ENUM:
            {
               return_value = "An unacceptable value is specified for an enumerated argument. The offending function is ignored, having no side effect other than to set the error flag. ";
            } break;

            case GL_INVALID_VALUE:
            {
               return_value = "A numeric argument is out of range. The offending function is ignored, having no side effect other than to set the error flag. ";
            } break;

            case GL_INVALID_OPERATION:
            {
               return_value = "The specified operation is not allowed in the current state. The offending function is ignored, having no side effect other than to set the error flag. ";
            } break;

            case GL_STACK_OVERFLOW:
            {
               return_value = "This function would cause a stack overflow. The offending function is ignored, having no side effect other than to set the error flag. ";
            } break;

            case GL_STACK_UNDERFLOW:
            {
               return_value = "This function would cause a stack underflow. The offending function is ignored, having no side effect other than to set the error flag. ";
            } break;

            case GL_OUT_OF_MEMORY:
            {
               return_value = "There is not enough memory left to execute the function. The state of OpenGL is undefined, except for the state of the error flags, after this error is recorded.";
            } break;

            default:
            {
            }
         }   

         return return_value;
      }

   protected:
//      std::stack< GLenum > m_ErrorStack;

   private:
};

#endif
