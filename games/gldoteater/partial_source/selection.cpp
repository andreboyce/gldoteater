void Selection(void)                                 // This Is Where Selection Is Done
{/*
   GLuint   buffer_size = 512;
   GLuint   buffer[buffer_size];                              // Set Up A Selection Buffer
   GLint   hits;                                    // The Number Of Objects That We Selected

   // The Size Of The Viewport. [0] Is <x>, [1] Is <y>, [2] Is <length>, [3] Is <width>
   GLint   viewport[4];

   // This Sets The Array <viewport> To The Size And Location Of The Screen Relative To The Window
   glGetIntegerv(GL_VIEWPORT, viewport);
   glSelectBuffer( buffer_size, buffer);                        // Tell OpenGL To Use Our Array For Selection

   // Puts OpenGL In Selection Mode. Nothing Will Be Drawn.  Object ID's and Extents Are Stored In The Buffer.
   (void)glRenderMode( GL_SELECT );

   glInitNames();                                    // Initializes The Name Stack
   glPushName(0);                                    // Push 0 (At Least One Entry) Onto The Stack


   glMatrixMode(GL_PROJECTION);                        // Selects The Projection Matrix
   glPushMatrix();                                    // Push The Projection Matrix
   glLoadIdentity();                                 // Resets The Matrix

   // This Creates A Matrix That Will Zoom Up To A Small Portion Of The Screen, Where The Mouse Is.
   gluPickMatrix((GLdouble) mouse_x, (GLdouble) (viewport[3]-mouse_y), 1.0f, 1.0f, viewport);

   // Apply The Perspective Matrix
   gluPerspective(45.0f, (GLfloat) (viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]), 0.1f, 100.0f);
   glMatrixMode(GL_MODELVIEW);     // Select The Modelview Matrix
   DrawTargets();                  // Render The Targets To The Selection Buffer
   glMatrixMode(GL_PROJECTION);    // Select The Projection Matrix
   glPopMatrix();                  // Pop The Projection Matrix
   glMatrixMode(GL_MODELVIEW);     // Select The Modelview Matrix
   hits=glRenderMode(GL_RENDER);   // Switch To Render Mode, Find Out How Many
                                   // Objects Were Drawn Where The Mouse Was
   */
}
