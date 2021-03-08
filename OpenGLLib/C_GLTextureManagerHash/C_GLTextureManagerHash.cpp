#include "C_GLTextureManagerHash.h"
/* TODO (#1#): Fix logic error in load textures cauing the last element 
               to be the first */
C_GLTextureManagerHash::C_GLTextureManagerHash( void ) : m_Textures()
{
}

C_GLTextureManagerHash::~C_GLTextureManagerHash()
{
   clear();
}

bool C_GLTextureManagerHash::BindTexture( std::string name )
{
   if( FindSubTexture( name ) )
   {
      GLenum type    = m_Textures[ m_SubTextures[name].parent_name ].type;
      GLuint texture = m_Textures[ m_SubTextures[name].parent_name ].textureID;
      ::glBindTexture( type, texture );
   }

   if( this->operator[](name) == ERROR_CNLT )
   {
      return false;
   }
   else
   {
      ::glBindTexture( m_Textures[name].type, this->operator[](name) );
      return true;
   }
   return false;
}

bool C_GLTextureManagerHash::FindSubTexture( std::string name )
{
   std::map< std::string, SubTexture >::iterator sub_texture_key = m_SubTextures.find( name );
   if( sub_texture_key != m_SubTextures.end() )
   {
      return true;
   }
   return false;
}

GLuint C_GLTextureManagerHash::GetSubTextureID( std::string name )
{
   if( !FindSubTexture(name) )
      return ERROR_CNLT;
   return m_SubTextures[ name ].texture;
}

GLuint C_GLTextureManagerHash::operator[]( std::string name )
{
   std::map< std::string, Texture >::iterator texture_key = m_Textures.find( name );
   if( texture_key != m_Textures.end() )
   {
      return m_Textures[ name ];
   }
   return ERROR_CNLT;
}

bool C_GLTextureManagerHash::AddSubTexture( std::string name, C_TexCoord TexCoord[4], GLuint parent_texture, std::string parent_name )
{
   m_SubTextures[name] = SubTexture( TexCoord, parent_texture, parent_name );
   return true;
}

bool C_GLTextureManagerHash::AddSubTexture( std::string name, SubTexture subtexture )
{
   m_SubTextures[name] = subtexture;
   return true;
}

bool C_GLTextureManagerHash::GetSubTextureCoords( std::string name, C_TexCoord TexCoord[4] )
{
   SubTexture subtexture = m_SubTextures[name];
   TexCoord[0] = subtexture.TexCoord[0];
   TexCoord[1] = subtexture.TexCoord[1];
   TexCoord[2] = subtexture.TexCoord[2];
   TexCoord[3] = subtexture.TexCoord[3];
   return true;
}

bool C_GLTextureManagerHash::GetSubTextureCoords( std::string name, Structs::fPoint TexCoords[4] )
{
   SubTexture subtexture = m_SubTextures[name];
   TexCoords[0].m_x = subtexture.TexCoord[0].U;
   TexCoords[1].m_x = subtexture.TexCoord[1].U;
   TexCoords[2].m_x = subtexture.TexCoord[2].U;
   TexCoords[3].m_x = subtexture.TexCoord[3].U;

   TexCoords[0].m_y = subtexture.TexCoord[0].V;
   TexCoords[1].m_y = subtexture.TexCoord[1].V;
   TexCoords[2].m_y = subtexture.TexCoord[2].V;
   TexCoords[3].m_y = subtexture.TexCoord[3].V;
   return true;
}

bool C_GLTextureManagerHash::LoadTAI( std::string path )
{
   std::ifstream tai_file( path.c_str(), std::ios::in  );
   if( !tai_file.is_open() )
      return false;

   std::string filename;
   std::string atlas_filename;
   int         atlas_id      = 0;
   GLenum      atlas_type    = GL_TEXTURE_2D;
   float       width_offset  = 0;
   float       height_offset = 0;
   float       depth_offset  = 0;
   float       width         = 0;
   float       height        = 0;

   std::string line, tmp, parentname;
   std::size_t end_index = 0;
   GLuint texture = 0;
   SubTexture st;
   while( !std::getline( tai_file, line ).eof() )
   {
      if( line.find( '#' ) != std::string::npos )
         continue;
      if( line.find( '\t' ) == std::string::npos )
         continue;
      if( line.find( ',' ) == std::string::npos )
         continue;
      if( line.size() == 0 )
         continue;
      // example atlas entry
      //ip.png		texture0.dds, 0, 2D, 0.500000, 0.000000, 0.000000, 0.007813, 0.500000
      end_index      = line.find( '\t' );
      filename       = line.substr( 0, end_index );
      line.erase( 0, end_index );
      end_index      = line.find( '\t' );
      line.erase( 0, end_index+1 );
      end_index      = line.find( '\t' );
      line.erase( 0, end_index+1 );
      end_index      = line.find( ',' );
      atlas_filename = line.substr( 0, end_index );
      parentname = "data\\texture\\" + atlas_filename;
      texture = LoadGLTextureFile( parentname );
      line.erase( 0, end_index+1 );
      end_index      = line.find( ',' );
      atlas_id       = atoi( line.substr( 0, end_index ).c_str() );
      line.erase( 0, end_index+1 );
      end_index      = line.find( ',' );
      tmp = line.substr( 0, end_index );
      if( tmp == " 2D" || tmp == "2D" )
      {
         atlas_type = GL_TEXTURE_2D;
      }
      line.erase( 0, end_index+1 );
      end_index      = line.find( ',' );
      width_offset   = atof( line.substr( 0, end_index ).c_str() );
      line.erase( 0, end_index+1 );
      end_index      = line.find( ',' );
      height_offset  = atof( line.substr( 0, end_index ).c_str() );
      line.erase( 0, end_index+1 );
      end_index      = line.find( ',' );
      depth_offset   = atof( line.substr( 0, end_index ).c_str() );
      line.erase( 0, end_index+1 );
      end_index      = line.find( ',' );
      width          = atof( line.substr( 0, end_index ).c_str() );
      line.erase( 0, end_index+1 );
      end_index      = line.find( ',' );
      height         = atof( line.substr( 0, end_index ).c_str() );
      line.erase( 0, end_index+1 );

/*      st.set( texture, parentname,
              C_TexCoord( width_offset,       height_offset ), 
              C_TexCoord( width_offset+width, height_offset ), 
              C_TexCoord( width_offset+width, height_offset+height ), 
              C_TexCoord( width_offset,       height_offset+height ) );*/
      st.set( texture, parentname,
              C_TexCoord( width_offset,       height_offset+height ), 
              C_TexCoord( width_offset+width, height_offset+height ), 
              C_TexCoord( width_offset+width, height_offset ), 
              C_TexCoord( width_offset,       height_offset ) ); //flipped

      m_SubTextures[ filename ] = st;
   }
   tai_file.close();
   return true;
}

GLuint C_GLTextureManagerHash::LoadGLBitmapHandle( HBITMAP hBMP, std::string name )
{

   if( TextureAlreadyLoaded( name ) && ::glIsTexture( m_Textures[ name ] ) )
   {
       return m_Textures[ name ];
   }
   GLuint texture = ERROR_CNLT;
   ::glGenTextures( 1, &texture ); // Generate Texture

   if( hBMP )
   {
      BITMAP BMP;

      ::GetObject( hBMP, sizeof(BMP), &BMP );
      ::glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );  // Pixel Storage Mode (Word Alignment / 4 Bytes)
      ::glBindTexture( GL_TEXTURE_2D, texture );  // Bind Our Texture
   
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); // Linear Filtering
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); // Mipmap Linear Filtering

      // Generate Mipmapped Texture (3 Bytes, Width, Height And Data From The BMP)
      ::gluBuild2DMipmaps( GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits );
      ::DeleteObject( hBMP );  // Delete The Bitmap Object
      
      m_Textures[ name ] = texture;
      
      return texture;
   }
   ::glDeleteTextures( 1, &texture );
   return ERROR_CNLT;
}

GLuint C_GLTextureManagerHash::LoadGLTextureBitmapResource( DWORD resource, std::string name  )
{
   if( TextureAlreadyLoaded( name ) && ::glIsTexture( m_Textures[ name ] ) )
   {
       return m_Textures[ name ];
   }
   GLuint texture = ERROR_CNLT;
   ::glGenTextures( 1, &texture ); // Generate Texture

   HBITMAP hBMP;
   BITMAP  BMP;
   
   hBMP = (HBITMAP)::LoadImage( ::GetModuleHandle( NULL ), MAKEINTRESOURCE( resource ), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );
   if( hBMP )
   {
      ::GetObject( hBMP, sizeof(BMP), &BMP );
      ::glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );  // Pixel Storage Mode (Word Alignment / 4 Bytes)
      ::glBindTexture( GL_TEXTURE_2D, texture );  // Bind Our Texture
   
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); // Linear Filtering
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); // Mipmap Linear Filtering

      // Generate Mipmapped Texture (3 Bytes, Width, Height And Data From The BMP)
      ::gluBuild2DMipmaps( GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits );
      ::DeleteObject( hBMP );  // Delete The Bitmap Object      
      
      m_Textures[ name ] = texture;
      
      return texture;
   }
   ::glDeleteTextures( 1, &texture );
   return ERROR_CNLT;
}

GLuint C_GLTextureManagerHash::LoadGLTextureBitmapFile( std::string path )
{
   return LoadGLTextureBitmapFile( (char*) path.c_str() );
}

GLuint C_GLTextureManagerHash::LoadGLTextureBitmapFile( char path[] )
{
   if( TextureAlreadyLoaded( path ) && ::glIsTexture( m_Textures[ path ] ) )
   {
       return m_Textures[ path ];
   }

   GLuint texture = ERROR_CNLT;
   ::glGenTextures( 1, &texture ); // Generate Texture

   HBITMAP hBMP;
   BITMAP  BMP;

   hBMP = (HBITMAP)::LoadImage( NULL, path, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );
   if( hBMP )
   {
      ::GetObject( hBMP, sizeof(BMP), &BMP );
      ::glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ); // Pixel Storage Mode (Word Alignment / 4 Bytes)
      ::glBindTexture( GL_TEXTURE_2D, texture ); // Bind Our Texture

      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); // Linear Filtering
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); // Mipmap Linear Filtering

      // Generate Mipmapped Texture (3 Bytes, Width, Height And Data From The BMP)
      ::gluBuild2DMipmaps( GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits );
      ::DeleteObject( hBMP );  // Delete The Bitmap Object

      m_Textures[ path ] = texture;

      return texture;
   }
   ::glDeleteTextures( 1, &texture );
   return ERROR_CNLT;
}

GLuint C_GLTextureManagerHash::LoadGLTextureBitmapFile( std::string path, COLORREF transparent_color )
{
   return LoadGLTextureBitmapFile( (char*) path.c_str(), transparent_color );
}

GLuint C_GLTextureManagerHash::LoadGLTextureFile( std::string path, COLORREF transparent_color )
{
   GLuint texture = ERROR_CNLT;
   if( path.find( ".bmp" ) != std::string::npos )
   {
      texture = LoadGLTextureBitmapFile( path, transparent_color );
   }
   else if( path.find( ".png" ) != std::string::npos )
   {
      texture = LoadGLTexturePNGFile( path );
   }
   else if( path.find( ".dds" ) != std::string::npos )
   {
      texture = LoadGLTextureDDSFile( path );
   }
   else if( path.find( ".tga" ) != std::string::npos )
   {
      texture = LoadGLTextureTGAFile( path );
   }
   return texture;
}

GLuint C_GLTextureManagerHash::LoadGLTextureBitmapFile( char path[], COLORREF transparent_color )
{
   if( !IsExtensionSupported( "GL_EXT_bgra" ) )
   {
      return ERROR_CNLT;
   }

   if( TextureAlreadyLoaded( path ) && ::glIsTexture( m_Textures[ path ] ) )
   {
       return m_Textures[ path ];
   }

   GLuint texture = ERROR_CNLT;
   ::glGenTextures( 1, &texture ); // Generate Texture

   HBITMAP hBMP;
   BITMAP  BMP;
   
   hBMP = (HBITMAP)::LoadImage( NULL, path, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );
   if( hBMP )
   {
      ::GetObject( hBMP, sizeof(BMP), &BMP );
      ::glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );  // Pixel Storage Mode (Word Alignment / 4 Bytes)
      ::glBindTexture( GL_TEXTURE_2D, texture );  // Bind Our Texture
   
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); // Linear Filtering
      ::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      unsigned char *data = NULL;
      try
      {
         data = new unsigned char[ BMP.bmWidth * BMP.bmHeight * 4 ];
      }
      catch( ... )
      {      
         ::DeleteObject( hBMP );
         ::glDeleteTextures( 1, &texture );
         return ERROR_CNLT;
      }
      unsigned int  pixels   = BMP.bmWidth * BMP.bmHeight;
      unsigned char color[3] = {0,0,0};
      unsigned char *bits    = (unsigned char*)BMP.bmBits;
      for( int i=0; i<pixels ; ++i )
      {
         color[0] = *(bits+(i*3));
         color[1] = *(bits+(i*3)+1);
         color[2] = *(bits+(i*3)+2);
         if( color[0] == GetBValue(transparent_color) &&
             color[1] == GetGValue(transparent_color) &&
             color[2] == GetRValue(transparent_color)
           )
         {
            data[ i*4    ] = color[0];
            data[ i*4 +1 ] = color[1];
            data[ i*4 +2 ] = color[2];
            data[ i*4 +3 ] = 0;
         }
         else
         {
            data[ i*4    ] = color[0];
            data[ i*4 +1 ] = color[1];
            data[ i*4 +2 ] = color[2];
            data[ i*4 +3 ] = 255;
         }
      }

      if( IsExtensionSupported( "GL_EXT_texture_compression_s3tc" ) &&
          IsExtensionSupported( "GL_ARB_texture_compression" )
        )
      {
         //::gluBuild2DMipmaps( GL_TEXTURE_2D, 4, BMP.bmWidth, BMP.bmHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data );
         //::gluBuild2DMipmaps( GL_TEXTURE_2D, GL_COMPRESSED_RGBA_ARB, BMP.bmWidth, BMP.bmHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data );
         //::gluBuild2DMipmaps( GL_TEXTURE_2D, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, BMP.bmWidth, BMP.bmHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data );
         ::glTexImage2D( GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, BMP.bmWidth, BMP.bmHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data );
      }
      else
      {
         // Generate Mipmapped Texture (4 Bytes, Width, Height And Data From The BMP)
         //::gluBuild2DMipmaps( GL_TEXTURE_2D, 4, BMP.bmWidth, BMP.bmHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data );
         ::glTexImage2D( GL_TEXTURE_2D, 0, 4, BMP.bmWidth, BMP.bmHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data );
      }

      ::DeleteObject( hBMP );  // Delete The Bitmap Object
      
      m_Textures[ path ] = texture;
      delete[] data;
      return texture;
   }
   ::glDeleteTextures( 1, &texture );
   return ERROR_CNLT;
}

GLuint C_GLTextureManagerHash::LoadGLTextureRAW( const char * filename, int wrap, int width, int height )
{
   if( TextureAlreadyLoaded( filename ) && ::glIsTexture( m_Textures[ filename ] ) )
   {
       return m_Textures[ filename ];
   }

   BYTE * data;
   FILE * file;  

   // open texture data
   file = ::fopen( filename, "rb" );
   if ( file == NULL )
   {
      return ERROR_CNLT;
   }

   // allocate buffer
   data = (BYTE*) ::malloc( width * height * 3 );

   // read texture data
   ::fread( data, width * height * 3, 1, file );
   ::fclose( file );

   // allocate a texture name
   GLuint texture = ERROR_CNLT;
   ::glGenTextures( 1, &texture ); // Generate Texture

   // select our current texture
   ::glBindTexture( GL_TEXTURE_2D, texture );

   // select modulate to mix texture with color for shading
   ::glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

   // when texture area is small, bilinear filter the closest MIP map
   ::glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );

   // when texture area is large, bilinear filter the first MIP map
   ::glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   // if wrap is true, the texture wraps over at the edges (repeat)
   //       ... false, the texture ends at the edges (clamp)
   ::glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                      wrap ? GL_REPEAT : GL_CLAMP );
   ::glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                      wrap ? GL_REPEAT : GL_CLAMP );

   // build our texture MIP maps
   ::gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );

   ::glFlush();

   // free buffer
   ::free( data );
  
   m_Textures[ filename ] = texture;
     
  return texture;
}

GLuint C_GLTextureManagerHash::LoadGLTextureRAW( std::string filename, int wrap, int width, int height )
{
   return LoadGLTextureRAW( filename.c_str() , wrap, width, height );
}

