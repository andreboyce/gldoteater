#include "main.h"

void MainWindow::HandleButtonMouseOverCollision( C_GLButton& Button, Structs::Point MousePosition )
{
   m_Mutex.Lock();
   ::glFlush();
   Button.OnMouseOver( MousePosition );
   m_Mutex.Release();
}

bool MainWindow::HandleButtonClickCollision( C_GLButton& Button, Structs::Point MousePosition )
{
   bool ret;
   m_Mutex.Lock();
   ::glFlush();
   ret = Button.OnClick( MousePosition );
   m_Mutex.Release();
   return ret;
}

void MainWindow::InitializeButtons( void )
{
   int arrow_texture = m_BackTexture;
   C_TexCoord TexCoords[4];

//   Alert( m_TextureManager["texture\\arrow.png" ) );
   m_BackButton.SetTexture( m_TextureManager.GetSubTextureID("texture\\arrow.png") );
   m_BackButton.SetPosition( m_BoundingRect.right-(m_BoundingRect.right-m_BoundingRect.left)*0.1,
                             m_BoundingRect.top-(m_BoundingRect.top-m_BoundingRect.bottom)*0.2,
                             m_Z );
   m_BackButton.SetSize( Structs::fPoint( 3.5, 3.5 ), Structs::fPoint( 2.50, 2.50 ) );
   m_BackButton.SetClickCallBack( C_GLButton::CallBackAndArgument( BackButtonClick, this ) );
   m_TextureManager.GetSubTextureCoords( "texture\\arrow.png", TexCoords );
   m_BackButton.SetTextCoords( TexCoords );

   m_WhatIsMyIPButton.SetTexture(  m_TextureManager.GetSubTextureID( "texture\\ip.png" ) );
   m_WhatIsMyIPButton.SetPosition( m_BoundingRect.right-(m_BoundingRect.right-m_BoundingRect.left)*0.6,
                                   m_BoundingRect.top-(m_BoundingRect.top-m_BoundingRect.bottom)*0.8,
                                   m_Z );
   m_WhatIsMyIPButton.SetSize( Structs::fPoint( 3.5, 3.5 ), Structs::fPoint( 2.50, 2.50 ) );
   m_WhatIsMyIPButton.SetClickCallBack( C_GLButton::CallBackAndArgument( WhatIsMyIPButtonClick, this ) );
   m_TextureManager.GetSubTextureCoords( "texture\\ip.png", TexCoords );
   m_WhatIsMyIPButton.SetTextCoords( TexCoords );

   m_StartButton.SetTexture(  m_TextureManager.GetSubTextureID( "texture\\start.png" ) );
   m_StartButton.SetPosition( m_BoundingRect.right-(m_BoundingRect.right-m_BoundingRect.left)*0.5,
                              m_BoundingRect.top-(m_BoundingRect.top-m_BoundingRect.bottom)*0.8,
                              m_Z );
   m_StartButton.SetSize( Structs::fPoint( 8.0, 4.0 ), Structs::fPoint( 7.0, 3.5 ) );
   m_StartButton.SetClickCallBack( C_GLButton::CallBackAndArgument( StartButtonClick, this ) );
   m_TextureManager.GetSubTextureCoords( "texture\\start.png", TexCoords );
   m_StartButton.SetTextCoords( TexCoords );

   m_RetryButton.SetTexture(  m_TextureManager.GetSubTextureID( "texture\\retry.png" ) );
   m_RetryButton.SetPosition( m_BoundingRect.right-(m_BoundingRect.right-m_BoundingRect.left)*0.4,
                              m_BoundingRect.top-(m_BoundingRect.top-m_BoundingRect.bottom)*0.8,
                              m_Z );
   m_RetryButton.SetSize( Structs::fPoint( 8.0, 4.0 ), Structs::fPoint( 7.0, 3.5 ) );
   m_RetryButton.SetClickCallBack( C_GLButton::CallBackAndArgument( RetryButtonClick, this ) );
   m_TextureManager.GetSubTextureCoords( "texture\\retry.png", TexCoords );
   m_RetryButton.SetTextCoords( TexCoords );

   m_ReconnectButton.SetTexture(  m_TextureManager.GetSubTextureID( "texture\\reconnect.png" ) );
   m_ReconnectButton.SetPosition( m_BoundingRect.right-(m_BoundingRect.right-m_BoundingRect.left)*0.5,
                              m_BoundingRect.top-(m_BoundingRect.top-m_BoundingRect.bottom)*0.8,
                              m_Z );
   m_ReconnectButton.SetSize( Structs::fPoint( 8.0, 4.0 ), Structs::fPoint( 7.0, 3.5 ) );
   m_ReconnectButton.SetClickCallBack( C_GLButton::CallBackAndArgument( ReconnectButtonClick, this ) );
   m_TextureManager.GetSubTextureCoords( "texture\\reconnect.png", TexCoords );
   m_ReconnectButton.SetTextCoords( TexCoords );
}

void MainWindow::ButtonMouseOverCollision()
{
   HandleButtonMouseOverCollision( m_BackButton,       m_MousePosition );
   HandleButtonMouseOverCollision( m_WhatIsMyIPButton, m_MousePosition );
   HandleButtonMouseOverCollision( m_StartButton,      m_MousePosition );
   HandleButtonMouseOverCollision( m_RetryButton,      m_MousePosition );
   HandleButtonMouseOverCollision( m_ReconnectButton,  m_MousePosition );
}

void MainWindow::ButtonClickCollision()
{
   HandleButtonClickCollision( m_BackButton,       m_MousePosition );
   HandleButtonClickCollision( m_WhatIsMyIPButton, m_MousePosition );
   HandleButtonClickCollision( m_StartButton,      m_MousePosition );
   HandleButtonClickCollision( m_RetryButton,      m_MousePosition );
   HandleButtonClickCollision( m_ReconnectButton,  m_MousePosition );
}
