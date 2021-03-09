#pragma once

#include "map.h"
#include "commonfunctions.h"

namespace TileStudio {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected: 
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDownWidth;
	private: System::Windows::Forms::NumericUpDown^  numericUpDownHeight;


	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  numericUpDownBitsPerTile;
	private: System::Windows::Forms::CheckBox^  checkBoxZeroBasedIndex;
	private: System::Windows::Forms::TextBox^  textBoxMapFileName;




	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  textBoxCalculatedFileSize;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::NumericUpDown^  numericUpDownLayers;
	private: System::Windows::Forms::TextBox^  textBoxActualFileSize;


	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Button^  buttonSaveAs;
	private: System::Windows::Forms::TextBox^  textBoxTilesetName;

	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;




	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->textBoxTilesetName = (gcnew System::Windows::Forms::TextBox());
			this->buttonSaveAs = (gcnew System::Windows::Forms::Button());
			this->textBoxActualFileSize = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->numericUpDownLayers = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBoxCalculatedFileSize = (gcnew System::Windows::Forms::TextBox());
			this->checkBoxZeroBasedIndex = (gcnew System::Windows::Forms::CheckBox());
			this->textBoxMapFileName = (gcnew System::Windows::Forms::TextBox());
			this->numericUpDownBitsPerTile = (gcnew System::Windows::Forms::NumericUpDown());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->numericUpDownHeight = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDownWidth = (gcnew System::Windows::Forms::NumericUpDown());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->menuStrip1->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownLayers))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownBitsPerTile))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownHeight))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownWidth))->BeginInit();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(741, 26);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->openToolStripMenuItem, 
				this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(40, 22);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(126, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(126, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->Filter = L"All files *.* | *.*";
			this->openFileDialog1->SupportMultiDottedExtensions = true;
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openFileDialog1_FileOk);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->FixedPanel = System::Windows::Forms::FixedPanel::Panel1;
			this->splitContainer1->IsSplitterFixed = true;
			this->splitContainer1->Location = System::Drawing::Point(0, 26);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->groupBox1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->groupBox2);
			this->splitContainer1->Size = System::Drawing::Size(741, 632);
			this->splitContainer1->SplitterDistance = 258;
			this->splitContainer1->SplitterWidth = 10;
			this->splitContainer1->TabIndex = 1;
			// 
			// groupBox1
			// 
			this->groupBox1->AutoSize = true;
			this->groupBox1->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->groupBox1->Controls->Add(this->label8);
			this->groupBox1->Controls->Add(this->textBoxTilesetName);
			this->groupBox1->Controls->Add(this->buttonSaveAs);
			this->groupBox1->Controls->Add(this->textBoxActualFileSize);
			this->groupBox1->Controls->Add(this->label7);
			this->groupBox1->Controls->Add(this->numericUpDownLayers);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->textBoxCalculatedFileSize);
			this->groupBox1->Controls->Add(this->checkBoxZeroBasedIndex);
			this->groupBox1->Controls->Add(this->textBoxMapFileName);
			this->groupBox1->Controls->Add(this->numericUpDownBitsPerTile);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->button1);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->numericUpDownHeight);
			this->groupBox1->Controls->Add(this->numericUpDownWidth);
			this->groupBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox1->Location = System::Drawing::Point(0, 0);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(741, 258);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Map Dimensions";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(318, 165);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(158, 29);
			this->label8->TabIndex = 20;
			this->label8->Text = L"Tileset Name";
			// 
			// textBoxTilesetName
			// 
			this->textBoxTilesetName->Location = System::Drawing::Point(551, 162);
			this->textBoxTilesetName->Name = L"textBoxTilesetName";
			this->textBoxTilesetName->Size = System::Drawing::Size(180, 34);
			this->textBoxTilesetName->TabIndex = 19;
			this->textBoxTilesetName->Text = L"dot.ts";
			// 
			// buttonSaveAs
			// 
			this->buttonSaveAs->Location = System::Drawing::Point(612, 210);
			this->buttonSaveAs->Name = L"buttonSaveAs";
			this->buttonSaveAs->Size = System::Drawing::Size(119, 35);
			this->buttonSaveAs->TabIndex = 18;
			this->buttonSaveAs->Text = L"Save As";
			this->buttonSaveAs->UseVisualStyleBackColor = true;
			this->buttonSaveAs->Click += gcnew System::EventHandler(this, &Form1::buttonSaveAs_Click);
			// 
			// textBoxActualFileSize
			// 
			this->textBoxActualFileSize->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->textBoxActualFileSize->Location = System::Drawing::Point(551, 118);
			this->textBoxActualFileSize->Name = L"textBoxActualFileSize";
			this->textBoxActualFileSize->ReadOnly = true;
			this->textBoxActualFileSize->Size = System::Drawing::Size(100, 34);
			this->textBoxActualFileSize->TabIndex = 17;
			this->textBoxActualFileSize->Text = L"0";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(318, 121);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(178, 29);
			this->label7->TabIndex = 16;
			this->label7->Text = L"Actual File Size";
			// 
			// numericUpDownLayers
			// 
			this->numericUpDownLayers->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->numericUpDownLayers->Location = System::Drawing::Point(182, 163);
			this->numericUpDownLayers->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 0});
			this->numericUpDownLayers->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericUpDownLayers->Name = L"numericUpDownLayers";
			this->numericUpDownLayers->Size = System::Drawing::Size(120, 34);
			this->numericUpDownLayers->TabIndex = 15;
			this->numericUpDownLayers->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(23, 165);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(84, 29);
			this->label6->TabIndex = 14;
			this->label6->Text = L"Layers";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(23, 216);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(125, 29);
			this->label5->TabIndex = 13;
			this->label5->Text = L"File Name";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(318, 70);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(227, 29);
			this->label4->TabIndex = 12;
			this->label4->Text = L"Calculated File Size";
			// 
			// textBoxCalculatedFileSize
			// 
			this->textBoxCalculatedFileSize->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->textBoxCalculatedFileSize->Location = System::Drawing::Point(551, 67);
			this->textBoxCalculatedFileSize->Name = L"textBoxCalculatedFileSize";
			this->textBoxCalculatedFileSize->ReadOnly = true;
			this->textBoxCalculatedFileSize->Size = System::Drawing::Size(100, 34);
			this->textBoxCalculatedFileSize->TabIndex = 11;
			this->textBoxCalculatedFileSize->Text = L"0";
			// 
			// checkBoxZeroBasedIndex
			// 
			this->checkBoxZeroBasedIndex->AutoSize = true;
			this->checkBoxZeroBasedIndex->Checked = true;
			this->checkBoxZeroBasedIndex->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBoxZeroBasedIndex->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->checkBoxZeroBasedIndex->Location = System::Drawing::Point(323, 23);
			this->checkBoxZeroBasedIndex->Name = L"checkBoxZeroBasedIndex";
			this->checkBoxZeroBasedIndex->Size = System::Drawing::Size(224, 33);
			this->checkBoxZeroBasedIndex->TabIndex = 10;
			this->checkBoxZeroBasedIndex->Text = L"Zero Based index";
			this->checkBoxZeroBasedIndex->UseVisualStyleBackColor = true;
			// 
			// textBoxMapFileName
			// 
			this->textBoxMapFileName->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->textBoxMapFileName->Location = System::Drawing::Point(182, 213);
			this->textBoxMapFileName->Name = L"textBoxMapFileName";
			this->textBoxMapFileName->Size = System::Drawing::Size(274, 34);
			this->textBoxMapFileName->TabIndex = 9;
			// 
			// numericUpDownBitsPerTile
			// 
			this->numericUpDownBitsPerTile->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->numericUpDownBitsPerTile->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {8, 0, 0, 0});
			this->numericUpDownBitsPerTile->Location = System::Drawing::Point(183, 115);
			this->numericUpDownBitsPerTile->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {512, 0, 0, 0});
			this->numericUpDownBitsPerTile->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {8, 0, 0, 0});
			this->numericUpDownBitsPerTile->Name = L"numericUpDownBitsPerTile";
			this->numericUpDownBitsPerTile->Size = System::Drawing::Size(120, 34);
			this->numericUpDownBitsPerTile->TabIndex = 7;
			this->numericUpDownBitsPerTile->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {8, 0, 0, 0});
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(24, 117);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(145, 29);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Bits Per Tile";
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(462, 211);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(144, 34);
			this->button1->TabIndex = 4;
			this->button1->Text = L"Load";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(24, 72);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(83, 29);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Height";
			this->label2->Click += gcnew System::EventHandler(this, &Form1::label2_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(24, 27);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(74, 29);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Width";
			// 
			// numericUpDownHeight
			// 
			this->numericUpDownHeight->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->numericUpDownHeight->Location = System::Drawing::Point(183, 70);
			this->numericUpDownHeight->Name = L"numericUpDownHeight";
			this->numericUpDownHeight->Size = System::Drawing::Size(120, 34);
			this->numericUpDownHeight->TabIndex = 1;
			this->numericUpDownHeight->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown2_ValueChanged);
			// 
			// numericUpDownWidth
			// 
			this->numericUpDownWidth->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->numericUpDownWidth->Location = System::Drawing::Point(183, 25);
			this->numericUpDownWidth->Name = L"numericUpDownWidth";
			this->numericUpDownWidth->Size = System::Drawing::Size(120, 34);
			this->numericUpDownWidth->TabIndex = 0;
			// 
			// groupBox2
			// 
			this->groupBox2->AutoSize = true;
			this->groupBox2->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->groupBox2->Controls->Add(this->dataGridView1);
			this->groupBox2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->groupBox2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox2->Location = System::Drawing::Point(0, 0);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(741, 364);
			this->groupBox2->TabIndex = 0;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Map Data";
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dataGridView1->Location = System::Drawing::Point(3, 30);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowTemplate->Height = 24;
			this->dataGridView1->Size = System::Drawing::Size(735, 331);
			this->dataGridView1->TabIndex = 0;
			// 
			// saveFileDialog1
			// 
			this->saveFileDialog1->DefaultExt = L"btm";
			this->saveFileDialog1->Filter = L"Binary Tile Map file *.btm |*.btm";
			this->saveFileDialog1->SupportMultiDottedExtensions = true;
			// 
			// Form1
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(741, 658);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"TileStudio Map Exporter";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->Panel2->PerformLayout();
			this->splitContainer1->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownLayers))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownBitsPerTile))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownHeight))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownWidth))->EndInit();
			this->groupBox2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
			 }
	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 loadmap_setupgrid( numericUpDownWidth, numericUpDownHeight, numericUpDownBitsPerTile, numericUpDownLayers, checkBoxZeroBasedIndex, openFileDialog1, textBoxMapFileName, textBoxCalculatedFileSize, textBoxActualFileSize, textBoxTilesetName, dataGridView1 );
			 }
private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }
private: System::Void groupBox1_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void numericUpDown2_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
            loadmap_setupgrid( numericUpDownWidth, numericUpDownHeight, numericUpDownBitsPerTile, numericUpDownLayers, checkBoxZeroBasedIndex, openFileDialog1, textBoxMapFileName, textBoxCalculatedFileSize, textBoxActualFileSize, textBoxTilesetName, dataGridView1 );
		 }
private: System::Void buttonSaveAs_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 save_map_as( numericUpDownWidth, numericUpDownHeight, 
				          numericUpDownBitsPerTile, numericUpDownLayers, checkBoxZeroBasedIndex, 
						  saveFileDialog1, textBoxMapFileName, textBoxCalculatedFileSize, textBoxActualFileSize, 
						  textBoxTilesetName, dataGridView1 );
		 }
};
}

