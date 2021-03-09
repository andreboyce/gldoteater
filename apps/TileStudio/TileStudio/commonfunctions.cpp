//#include "stdafx.h"
#include "commonfunctions.h"
#include "map.h"

void loadmap_setupgrid( System::Windows::Forms::NumericUpDown^ numericUpDownWidth, 
					    System::Windows::Forms::NumericUpDown^ numericUpDownHeight, 
						System::Windows::Forms::NumericUpDown^ numericUpDownBitsPerTile,
						System::Windows::Forms::NumericUpDown^ numericUpDownLayers,
						System::Windows::Forms::CheckBox^  checkBox1,
						System::Windows::Forms::OpenFileDialog^ openFileDialog,
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
	  map.ZeroIndexed = checkBox1->Checked;
	  map.Layers      = int(numericUpDownLayers->Value);

	  textBoxCalculatedFileSize->Text = map.calculateFileSize().ToString();
	  openFileDialog->FileName = textBoxMapFileName->Text;
      load_map( openFileDialog, dataGridView, textBoxMapFileName, textBoxActualFileSize, textBoxTilesetName, map );
      dataGridView->Refresh();
   }
   else
   {
      using namespace System::Windows::Forms;
      MessageBox::Show( "Row and Height must be Non Zero", "Map Dimensions Error",
                        MessageBoxButtons::OK, MessageBoxIcon::Exclamation );
   }
}
