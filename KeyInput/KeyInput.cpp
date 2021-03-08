#include "KeyInput.h"

KeyInput::KeyInput( void ) : Keys( 256, false ), // 256 keys none initally pressed
                                 m_TimeKeyWasPressed( 256, 0 ),
                                 m_RepeatCountIntervals( 256, 0 )
{
}

KeyInput::~KeyInput( void )
{
}
