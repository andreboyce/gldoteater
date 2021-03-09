#pragma once

#ifndef __tiles_H
#define __tiles_H

#include <vcclr.h>
#include "../../TileSet/TileSet.h"

bool To_string( System::String^ source, std::string &target );
std::string To_string( System::String^ source );

void LoadImage( System::Windows::Forms::OpenFileDialog^ openFileDialog,
			    System::Windows::Forms::PictureBox^     pictureBox );

void SaveTiles( System::Windows::Forms::FolderBrowserDialog^ folderBrowserDialog,
			    System::Windows::Forms::PictureBox^     pictureBox,
				System::Windows::Forms::TextBox^        textBoxPrefix,
				System::Windows::Forms::NumericUpDown^  numericUpDownStartIndex,
				System::Windows::Forms::DomainUpDown^   domainUpDownWidth,
				System::Windows::Forms::DomainUpDown^   domainUpDownHeight );


#endif
