#include "C_WinControllBase.h"

bool C_WinControllBase::m_bCommonControlsLoaded = false;

void C_WinControllBase::LoadCommonControlls( void )
{
   if( !m_bCommonControlsLoaded )
   {
      m_bCommonControlsLoaded = true;
      INITCOMMONCONTROLSEX ice;
      ice.dwSize = sizeof(INITCOMMONCONTROLSEX);
      ice.dwICC  = ICC_INTERNET_CLASSES;
      InitCommonControlsEx( &ice );
   }
}


