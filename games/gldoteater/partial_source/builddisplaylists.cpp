#include "main.h"

void MainWindow::BuildMapDisplayLists()
{
   m_Mutex.Lock();
   BuildMapVBOs();
   m_DisplayListManager.rebuild( "map" );
   RenderMapToBuildList();
   m_DisplayListManager.endrebuild();   
   m_Mutex.Release();
}

void MainWindow::BuildDotDisplayLists()
{
   m_Mutex.Lock();
   BuildDotVBOs();
   m_DisplayListManager.rebuild( "dot" );
   RenderDotVBOs();
   m_DisplayListManager.endrebuild();   
   m_Mutex.Release();
}


