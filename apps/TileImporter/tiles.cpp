#include "stdafx.h"
#include "tiles.h"

void LoadImage( System::Windows::Forms::OpenFileDialog^ openFileDialog,
			    System::Windows::Forms::PictureBox^  pictureBox )
{
   System::Windows::Forms::DialogResult result = openFileDialog->ShowDialog();
   if ( result == System::Windows::Forms::DialogResult::OK )
   {
	   //System::Windows::Forms::b MyImage = new Bitmap(fileToDisplay);
	   //pictureBox->Image = ;
	   try
	   {
		   pictureBox->Load( openFileDialog->FileName );
	   }
	   catch( ... )
	   {
		   using namespace System::Windows::Forms;
           MessageBox::Show( "The specified image file is invalid", "Error",
                              MessageBoxButtons::OK, MessageBoxIcon::Exclamation );
	   }
   }
}

void SaveTiles( System::Windows::Forms::FolderBrowserDialog^ folderBrowserDialog,
			    System::Windows::Forms::PictureBox^     pictureBox,
				System::Windows::Forms::TextBox^        textBoxPrefix,
				System::Windows::Forms::NumericUpDown^  numericUpDownStartIndex,
				System::Windows::Forms::DomainUpDown^   domainUpDownWidth,
				System::Windows::Forms::DomainUpDown^   domainUpDownHeight )
{
   System::Windows::Forms::DialogResult result = folderBrowserDialog->ShowDialog();
   if ( result == System::Windows::Forms::DialogResult::OK )
   {
	   try
	   {
		  TileSet tileset;
		  System::Drawing::Point^ tile_dimensions = gcnew System::Drawing::Point;
		  tile_dimensions->X = System::Convert::ToInt32(domainUpDownWidth->Text);
		  tile_dimensions->Y = System::Convert::ToInt32(domainUpDownHeight->Text);
		  int max_width  = pictureBox->Image->Width/tile_dimensions->X;
          int max_height = pictureBox->Image->Height/tile_dimensions->Y;

		  System::Drawing::Rectangle^ rect    = gcnew System::Drawing::Rectangle( 0, 0, tile_dimensions->X, tile_dimensions->Y );
		  System::Drawing::Imaging::PixelFormat^ pixelFormat = pictureBox->Image->PixelFormat;
		  System::String^             prefix  = textBoxPrefix->Text;
		  System::String^             path    = gcnew System::String( "" );
		  int                         index   = System::Convert::ToInt32(numericUpDownStartIndex->Text);
		  System::Drawing::Bitmap^ pictureBoxBitmap = gcnew System::Drawing::Bitmap( pictureBox->Image );

          for( int i=0 ; i<max_width ; ++i )
          {
             for( int c=0 ; c<max_height ; ++c )
             {
                System::Drawing::Bitmap^ bitmap = pictureBoxBitmap->Clone( *rect, *pixelFormat );
                path = folderBrowserDialog->SelectedPath + "\\" + prefix + index.ToString() + ".bmp";
                std::string tilefilename;
				To_string( gcnew System::String( prefix + index.ToString() + ".bmp" ), tilefilename );
				tileset.AddTile( tilefilename, tileset.size() );
                bitmap->Save( path );
                index++;
				rect->Y += tile_dimensions->Y;;
				//rect->Top    += tile_dimensions->Y;
                //rect->Bottom += tile_dimensions->Y;
             }
			 rect->Y = 0;
			 rect->X += tile_dimensions->X;;
             //rect->Left   += tile_dimensions->X;
             //rect->Right  += tile_dimensions->X;
             //rect->Top    = 0;
			 //rect->Bottom = tile_dimensions->Y;
          }
		  std::string tilesetfilename;
		  System::String^ tmp = gcnew System::String( "" );
		  tmp = folderBrowserDialog->SelectedPath + "\\";
		  tmp += (prefix->Length) ? prefix : gcnew System::String( "tileset" );
		  tmp += ".ts";
		  tilesetfilename = To_string(tmp);
		  //if( prefix->Length  == 0 )
		  //{
		  //	  tilesetfilename = To_string( gcnew System::String( folderBrowserDialog->SelectedPath + "\\" + gcnew System::String("tileset") + gcnew System::String(".ts") ) );
		  //}
		  //To_string( gcnew System::String( folderBrowserDialog->SelectedPath + "\\" + prefix + ".ts" ), tilesetfilename );
		  tileset.SetTileDirectory( To_string( prefix ) );
		  tileset.write( tilesetfilename );
	   }
	   catch( ... )
	   {
		   using namespace System::Windows::Forms;
           MessageBox::Show( "You messed up bad...", "Error",
                              MessageBoxButtons::OK, MessageBoxIcon::Exclamation );
	   }
   }
}

bool To_string( System::String^ source, std::string &target )
{
	using namespace System;
    pin_ptr<const wchar_t> wch = PtrToStringChars( source );
    int len = (( source->Length+1) * 2);
    char *ch = new char[ len ];
    bool result = wcstombs( ch, wch, len ) != -1;
    target = ch;
    delete ch;
    return result;
}

std::string To_string( System::String^ source )
{
	std::string result;
	using namespace System;
    pin_ptr<const wchar_t> wch = PtrToStringChars( source );
    int len = (( source->Length+1) * 2);
    char *ch = new char[ len ];
    bool r = wcstombs( ch, wch, len ) != -1;
    result = ch;
    delete ch;
    return result;
}
