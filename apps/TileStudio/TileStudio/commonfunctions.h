#pragma once

#ifndef __commonfunctions_H
#define __commonfunctions_H

void loadmap_setupgrid( System::Windows::Forms::NumericUpDown^ numericUpDownWidth, 
					    System::Windows::Forms::NumericUpDown^ numericUpDownHeight, 
						System::Windows::Forms::NumericUpDown^ numericUpDownBitsPerTile,
						System::Windows::Forms::NumericUpDown^ numericUpDownLayers,
						System::Windows::Forms::CheckBox^  checkBoxZeroBasedIndex,
						System::Windows::Forms::OpenFileDialog^ openFileDialog,
						System::Windows::Forms::TextBox^ textBoxMapFileName,
                        System::Windows::Forms::TextBox^ textBoxCalculatedFileSize,
                        System::Windows::Forms::TextBox^ textBoxActualFileSize,
                        System::Windows::Forms::TextBox^ textBoxTilesetName,
						System::Windows::Forms::DataGridView^  dataGridView );


#endif
