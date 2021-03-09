//#include "stdafx.h"
#include "map.h"

void load_map( System::Windows::Forms::OpenFileDialog^ openFileDialog, 
			   System::Windows::Forms::DataGridView^ gridview,
			   System::Windows::Forms::TextBox^  textBoxMapFileName,
               System::Windows::Forms::TextBox^ textBoxActualFileSize,
               System::Windows::Forms::TextBox^ textBoxTilesetName,
			   MapData& map )
{
   System::Windows::Forms::DialogResult result = openFileDialog->ShowDialog();
   if ( result == System::Windows::Forms::DialogResult::OK )
   {
      textBoxMapFileName->Text    = openFileDialog->FileName;	  
      SetupDataGridView( openFileDialog, gridview, textBoxActualFileSize, textBoxTilesetName, map );
   }
}

// Input: The DataGridView controll and the number of columns
// Output: Nothing
// Purpose: Setup the DataGridView, it will add N columns

void SetupDataGridView( System::Windows::Forms::OpenFileDialog^ openFileDialog, 
					    System::Windows::Forms::DataGridView^ gridview, 
						System::Windows::Forms::TextBox^ textBoxActualFileSize,
                        System::Windows::Forms::TextBox^ textBoxTilesetName,
						MapData& map )
{
   gridview->Columns->Clear(); // delete previous columns
   gridview->ColumnCount = map.Width;  // Number of colums

   for( int i=0 ; i<map.Width ; i++ ) // for each column in the DataGridView control
   {
	   // give each column a name, in this case 0-18
	  (gridview->Columns[i])->Name     = i.ToString();

	  // Stop the user from sorting the column
	  (gridview->Columns[i])->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;

	  // stop the user from editng the column
	  (gridview->Columns[i])->ReadOnly = true;
   }
   PopulateDataGridView( openFileDialog, gridview, textBoxActualFileSize, textBoxTilesetName, map );
}

// Input: The DataGridView controll to populate
// Output: Nothing
// Purpose: Popu;ate the DataGridView, It will add N rows,
//          The contents of the rows are determined by the make grid function

void PopulateDataGridView( System::Windows::Forms::OpenFileDialog^ openFileDialog, 
						   System::Windows::Forms::DataGridView^ gridview,
						   System::Windows::Forms::TextBox^ textBoxActualFileSize,
                           System::Windows::Forms::TextBox^ textBoxTilesetName,
						   MapData& map )
{
   gridview->Rows->Clear(); 

   // System::IO::FileAccess::Read
   System::IO::Stream^ fileStream = openFileDialog->OpenFile();

   System::IO::BinaryReader^ binaryreader = gcnew System::IO::BinaryReader( fileStream );

   array<System::String^>^ single_row = gcnew array<System::String^>( map.Width );
   int bytes_read = 0;

   // Read data from file.
   while( fileStream->CanRead && ( bytes_read < (map.Width*map.Height*map.BitsPerTile*map.Layers) ) ) //map.Layers
   {
	   read_row( map, fileStream, binaryreader, single_row, bytes_read );
	   gridview->Rows->Add( single_row );
   }

   textBoxActualFileSize->Text = fileStream->Length.ToString();
   save_map( openFileDialog->FileName, gridview, textBoxTilesetName, map );
   binaryreader->Close();
   fileStream->Close();
}

void read_row( MapData& map, System::IO::Stream^ fileStream, System::IO::BinaryReader^ binaryreader, array<System::String^>^ single_row, int &bytes_read )
{
   for( int c = 0 ; ( fileStream->CanRead ) && ( c < map.Width ) ; c++ )
   {
	  switch( map.BitsPerTile )
	  {
	      case 8:
		  {
			 unsigned char data8 = binaryreader->ReadByte();
             bytes_read += map.BitsPerTile;
			 single_row[c] = data8.ToString();
		  } break;
	      case 16:
		  {
			 short data16 = binaryreader->ReadInt16();
             bytes_read += map.BitsPerTile;
			 single_row[c] = data16.ToString();
		  } break;
	      case 32:
		  {
			 int data32 = binaryreader->ReadInt32();
             bytes_read += map.BitsPerTile;
			 single_row[c] = data32.ToString();
		  } break;
	      case 64:
		  {
			 long data64 = binaryreader->ReadInt32();
             bytes_read += map.BitsPerTile;
			 single_row[c] = data64.ToString();
		  } break;
	      default:
		  {
			 unsigned char data8 = binaryreader->ReadByte();
             bytes_read += map.BitsPerTile;
			 single_row[c] = data8.ToString();
		  };
	  }
   }
}

void save_map_as( System::Windows::Forms::NumericUpDown^ numericUpDownWidth, 
   			      System::Windows::Forms::NumericUpDown^ numericUpDownHeight, 
				  System::Windows::Forms::NumericUpDown^ numericUpDownBitsPerTile,
				  System::Windows::Forms::NumericUpDown^ numericUpDownLayers,
				  System::Windows::Forms::CheckBox^  checkBoxZeroBasedIndex,
				  System::Windows::Forms::SaveFileDialog ^ saveFileDialog,
				  System::Windows::Forms::TextBox^ textBoxMapFileName,
                  System::Windows::Forms::TextBox^ textBoxCalculatedFileSize,
                  System::Windows::Forms::TextBox^ textBoxActualFileSize,
                  System::Windows::Forms::TextBox^ textBoxTilesetName,
				  System::Windows::Forms::DataGridView^  dataGridView )
{
   if( ( int(numericUpDownWidth->Value) > 0 ) && ( int(numericUpDownHeight->Value) > 0 ) )
   {
	  MapData map;
	  map.BitsPerTile = int(numericUpDownBitsPerTile->Value);
	  map.Width       = int(numericUpDownHeight->Value);
	  map.Height      = int(numericUpDownWidth->Value);
	  map.ZeroIndexed = checkBoxZeroBasedIndex->Checked;
	  map.Layers      = int(numericUpDownLayers->Value);

      System::Windows::Forms::DialogResult result = saveFileDialog->ShowDialog();
	  saveFileDialog->FileName = textBoxMapFileName->Text;
      if ( result == System::Windows::Forms::DialogResult::OK )
      {
         save_map( saveFileDialog->FileName, dataGridView, textBoxTilesetName, map );
      }
   }
   else
   {
      using namespace System::Windows::Forms;
      MessageBox::Show( "Row and Height must be Non Zero", "Map Dimensions Error",
                        MessageBoxButtons::OK, MessageBoxIcon::Exclamation );
   }
}

void save_map( System::String^ FileName, 
			   System::Windows::Forms::DataGridView^ dataGridView,
			   System::Windows::Forms::TextBox^ textBoxTilesetName,
			   MapData& map )
{
   TileMap tmap;
   Tile t;
   tmap.resize( map.Width, map.Height );
   std::string TilesetName;
   To_string( textBoxTilesetName->Text, TilesetName );
   tmap.m_Header.tileset_file_name = TilesetName;

   for( int r=0 ; r<map.Height ; r++ )
   {
	   for( int c = 0 ; c<map.Width ; c++ )
	   {
		   t.m_TileGraphic = System::Convert::ToInt32( dataGridView->Rows[r]->Cells[c]->Value );
		   t.m_Solidity    = 0;
		   if( !map.ZeroIndexed ) // my btm format is zero indexed so I have to convert
		   {
			   if( t.m_TileGraphic == 0 ) // change the transparent tile from 0 to 0xffff...
			   {
                  t.m_TileGraphic = (int)pow( (double)2, (double)sizeof(t.m_TileGraphic) )-1; // 2^n-1
			   }
			   else if( t.m_TileGraphic < 0 )
			   {
				   t.m_TileGraphic = (int)pow( (double)2, (double)sizeof(t.m_TileGraphic) )-1; // 2^n-1
			   }
			   else
			   {
				   t.m_TileGraphic -= 1;
			   }
		   }
		   tmap[r][c] = t;
	   }
   }
   std::string tmp;
   To_string( FileName, tmp );
   if( tmp.find( ".btm" ) == std::string::npos )
   {
	   tmp += ".btm";
   }
   tmap.write( tmp );
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
