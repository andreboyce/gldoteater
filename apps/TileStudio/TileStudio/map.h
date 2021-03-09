#pragma once

#ifndef __loadmap_H
#define __loadmap_H

#include <stdlib.h>
#include <vcclr.h>
#include <string>
#include <cmath>

#pragma warning( disable : 4561 ) 
#pragma warning( disable : 4018 ) 
#include "../../../TileMap/TileMap.h"

struct MapData
{
	int  Width;       // number of columns
	int  Height;      // number of rows
	int  BitsPerTile; // Bits Per Tile e.g. 8, 16, 32, 64
	int  Layers;      // The Number of layers
	bool ZeroIndexed; // If true then transparent tiles are 0xffff...
	                  // If false transparent tiles are 0x0000

	MapData(): ZeroIndexed( false ), Width( 0 ),
	                Height( 0 ), BitsPerTile( 0 ), Layers( 0 )
	{
	}
	MapData( const MapData& rhs )
	{
		ZeroIndexed = rhs.ZeroIndexed;
		Width       = rhs.Width;
		Height      = rhs.Height;
		BitsPerTile = rhs.BitsPerTile;
		Layers      = rhs.Layers;
	}
	const MapData& operator = ( const MapData& rhs )
	{
		ZeroIndexed = rhs.ZeroIndexed;
		Width       = rhs.Width;
		Height      = rhs.Height;
		BitsPerTile = rhs.BitsPerTile;
		Layers      = rhs.Layers;
		return *this;
	}
	unsigned long calculateFileSize() // bytes
	{
		return ((Width*Height*BitsPerTile*Layers)/8);
	}
};

void load_map( System::Windows::Forms::OpenFileDialog^ openFileDialog1, 
			   System::Windows::Forms::DataGridView^ dataGridView1,
			   System::Windows::Forms::TextBox^  textBoxMapFileName,
               System::Windows::Forms::TextBox^ textBoxActualFileSize,
               System::Windows::Forms::TextBox^ textBoxTilesetName,
			   MapData& map );

void SetupDataGridView( System::Windows::Forms::OpenFileDialog^ openFileDialog1, 
					    System::Windows::Forms::DataGridView^ gridview, 
						System::Windows::Forms::TextBox^ textBoxActualFileSize,
                        System::Windows::Forms::TextBox^ textBoxTilesetName,
						MapData& map );

void PopulateDataGridView( System::Windows::Forms::OpenFileDialog^ openFileDialog1, 
						   System::Windows::Forms::DataGridView^ gridview,
						   System::Windows::Forms::TextBox^ textBoxActualFileSize,
                           System::Windows::Forms::TextBox^ textBoxTilesetName,
						   MapData& map );

void read_row( MapData& map, System::IO::Stream^ fileStream, System::IO::BinaryReader^ binaryreader, array<System::String^>^ single_row, int &bytes_read );

void save_map( System::String^ FileName,
			   System::Windows::Forms::DataGridView^ dataGridView1,
			   System::Windows::Forms::TextBox^ textBoxTilesetName,
			   MapData& map );

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
				  System::Windows::Forms::DataGridView^  dataGridView );

bool To_string( System::String^ source, std::string &target );


#endif