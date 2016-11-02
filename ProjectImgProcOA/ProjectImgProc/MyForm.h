#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "opencv2\highgui\highgui.hpp"
#include <opencv2\core\core.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\features2d\features2d.hpp>
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\calib3d\calib3d.hpp"
#include <msclr/marshal_cppstd.h>
#include "opencv2/ml/ml.hpp"
#include <stdio.h>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include "CreateDictionary.h"
#include <chrono>

namespace ProjectImgProc {

	using namespace std;
	using namespace cv;
	using namespace System::IO;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;	
	using namespace System::Drawing::Drawing2D;

	Mat openImage;//Store the image used for classification

	#pragma region Declaration Field

	Mat trainingDataMat, labelsMat;
	Mat words;//#Store the vocabulary Blocks/SIFT
	Mat labels;//#Labels used in k-means algorithm
	Mat descriptor; //#Store the SIFT descriptors of images
	Mat featuresUnclustered; //Store all descriptors in a 1D Mat (unclustered)

	SiftDescriptorExtractor detector, extractor;//#SIFT feature detector and extractor
	vector<KeyPoint> keypoints; //#Store the key-points that will be extracted 

	#pragma endregion
	
	std::vector<int> LabelsTrain, LabelsTest;

public ref class MyForm : public System::Windows::Forms::Form{

	#pragma region Declaration Field

	System::Drawing::Point lastLocation;//#Last location of mouse

	Mat *ImgsTrain, *ImgsTest;//#Images used for Train/Test
	Mat **subDirImgArray;//#Mat of matrices of files under subdirectories
	Mat *ImgsResults;//#Mat of results after feature extraction	

	static int takeBy = 0;//#Take images by...
	static int *myImageFlags;//#Store the length of files in each sub-directory
	static int *timerSlide;//#Images timer sliding
	static int resultsTimerSliding;//#Used for sliding result images

	int nrGrid = 0;
	int corect = 0;
	int countCheckedClasses = 0;
	int labelForNewImage;
	int dictionarySize;//#The dictionary size
	int blockSize;//#The block size
	int nrSamples;//#Total number of _samples
	int countImgs = 0;//#Count the selected images
	int countDirs = 0;//#Count the selected directories
	int totalImages = 0;//#Store the total number of images from all sub-directories
	int checkIfPostBack = 0;//#Check if is PostBack at Create dictionary
	int checkIfPostBack1 = 0;//#Check if is PostBack at Histogram representation
	int flags = KMEANS_PP_CENTERS;//#Flags in k-means BoW Trainer
	int showSpecificHist;//#Store image for showing his hist 
	int position = 0;//#Insert the word in hist at specific position
	int minimumDistanceEuclidean;//#Store the minimum Euclidean distance
	int *distanceEuclidean;//#Calculate the Euclidean distance

	double **wordsHistTrain, **wordsHistTest;//#Store the hist of Train/Test images
	double **wordsHistImage;

	bool SiftOrBow;//#Used to see if SIFT/BoW model is selected from Menu strip (Create dictionary)
	bool mouseDown;//#Used to see if mouse is clicked (Panel movement)
	bool maximizeCheck;//#Check if window is maximized
	bool TrainOrTest;//#Used to see if Train/Test Images are selected at Histogram show
	bool OneImage;

	System::String^ raspuns;
	System::String^ _getDir;//#Store the specific directory which contains C-classes!
	array<System::String^>^ _getAllSubDir;//#Store all the sub-directories from specified directory!
	array<System::String^>^ _getDirFiles;//#Store all files of each sub-directory selected!

	array<Timer^> ^pTimers;//#Create an array of Timers
	array<PictureBox^> ^pBoxes;//#Create an array of PictureBoxes
	array<Label^> ^pLabels;//#Create an array of Labels
	array<Button^> ^pPlayButtons;//#Create an array of Play-Buttons
	array<Button^> ^pPauseButtons;//#Create an array of Pause-Buttons

	enum class Result { Zero, NeAglomerat, Aglomerat, FoarteAglomerat, ExtremDeAglomerat };

	//System::String^ getFolder;//#Store the directory path
	//array<System::String^>^ getFile;//#Store the names of each image in directory

	#pragma endregion
	
	#pragma region Constructor

	public:
		MyForm(void)
		{
			resultsTimerSliding = 1;
											
			int k = 0;//check - debug
			InitializeComponent();			
		}

	#pragma endregion

	#pragma region Destructor

	protected:
		~MyForm()
		{

			if (components)
			{
				delete components;
			}

		}

	#pragma endregion

	#pragma region Tool Declarations
	
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Panel^  panel17;
	private: System::Windows::Forms::Label^  label23;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column8;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column5;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column6;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column7;
	private: System::Windows::Forms::GroupBox^  groupBox5;
	private: System::Windows::Forms::Label^  label21;
	private: System::Windows::Forms::Label^  label22;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Panel^  panel6;
	private: System::Windows::Forms::Panel^  panel7;
	private: System::Windows::Forms::Panel^  panel8;
	private: System::Windows::Forms::Panel^  panel9;
	private: System::Windows::Forms::Panel^  panel10;
	private: System::Windows::Forms::Panel^  panel11;
	private: System::Windows::Forms::Panel^  panel12;
	private: System::Windows::Forms::Panel^  panel13;
	private: System::Windows::Forms::Panel^  panel14;
	private: System::Windows::Forms::Panel^  panel15;
	private: System::Windows::Forms::Panel^  panel16;
	private: System::Windows::Forms::Button^  button10;
	private: System::Windows::Forms::Button^  button14;
	private: System::Windows::Forms::Button^  button13;
	private: System::Windows::Forms::Button^  button12;
	private: System::Windows::Forms::Panel^  panel5;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Button^  button11;
	private: System::Windows::Forms::Panel^  panel4;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::Button^  button9;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::ToolTip^  toolTip1;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::CheckedListBox^  checkedListBox1;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox4;
	private: System::Windows::Forms::Button^  button15;
	private: System::Windows::Forms::PictureBox^  pictureBox4;
	private: System::Windows::Forms::RadioButton^  radioButton4;
	private: System::Windows::Forms::RadioButton^  radioButton3;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::RadioButton^  radioButton1;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Panel^  left_Panel;
	private: System::Windows::Forms::Panel^  logo_Panel;
	private: System::Windows::Forms::PictureBox^  logo_pictureBox;
	private: System::Windows::Forms::Panel^  header_Panel;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::CheckedListBox^  checkedListBox2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  PlayButton;
	private: System::Windows::Forms::Button^  PauseButton;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Label^  label11;

	#pragma endregion		 
	
	#pragma region Windows Form Designer generated code
		
void InitializeComponent(void){
	this->components = (gcnew System::ComponentModel::Container());
	System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
	System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
	System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
	System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
	this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
	this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
	this->textBox2 = (gcnew System::Windows::Forms::TextBox());
	this->label2 = (gcnew System::Windows::Forms::Label());
	this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
	this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
	this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
	this->label16 = (gcnew System::Windows::Forms::Label());
	this->label15 = (gcnew System::Windows::Forms::Label());
	this->button2 = (gcnew System::Windows::Forms::Button());
	this->textBox1 = (gcnew System::Windows::Forms::TextBox());
	this->label1 = (gcnew System::Windows::Forms::Label());
	this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
	this->PlayButton = (gcnew System::Windows::Forms::Button());
	this->PauseButton = (gcnew System::Windows::Forms::Button());
	this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
	this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
	this->button3 = (gcnew System::Windows::Forms::Button());
	this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
	this->checkedListBox2 = (gcnew System::Windows::Forms::CheckedListBox());
	this->button1 = (gcnew System::Windows::Forms::Button());
	this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
	this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
	this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
	this->label10 = (gcnew System::Windows::Forms::Label());
	this->label9 = (gcnew System::Windows::Forms::Label());
	this->button4 = (gcnew System::Windows::Forms::Button());
	this->label8 = (gcnew System::Windows::Forms::Label());
	this->label7 = (gcnew System::Windows::Forms::Label());
	this->label6 = (gcnew System::Windows::Forms::Label());
	this->checkedListBox1 = (gcnew System::Windows::Forms::CheckedListBox());
	this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
	this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
	this->button8 = (gcnew System::Windows::Forms::Button());
	this->button9 = (gcnew System::Windows::Forms::Button());
	this->button7 = (gcnew System::Windows::Forms::Button());
	this->left_Panel = (gcnew System::Windows::Forms::Panel());
	this->button6 = (gcnew System::Windows::Forms::Button());
	this->panel17 = (gcnew System::Windows::Forms::Panel());
	this->label23 = (gcnew System::Windows::Forms::Label());
	this->button14 = (gcnew System::Windows::Forms::Button());
	this->button13 = (gcnew System::Windows::Forms::Button());
	this->button12 = (gcnew System::Windows::Forms::Button());
	this->panel5 = (gcnew System::Windows::Forms::Panel());
	this->label14 = (gcnew System::Windows::Forms::Label());
	this->button11 = (gcnew System::Windows::Forms::Button());
	this->panel4 = (gcnew System::Windows::Forms::Panel());
	this->label13 = (gcnew System::Windows::Forms::Label());
	this->button10 = (gcnew System::Windows::Forms::Button());
	this->panel3 = (gcnew System::Windows::Forms::Panel());
	this->label12 = (gcnew System::Windows::Forms::Label());
	this->label11 = (gcnew System::Windows::Forms::Label());
	this->panel2 = (gcnew System::Windows::Forms::Panel());
	this->textBox3 = (gcnew System::Windows::Forms::TextBox());
	this->panel1 = (gcnew System::Windows::Forms::Panel());
	this->label5 = (gcnew System::Windows::Forms::Label());
	this->logo_Panel = (gcnew System::Windows::Forms::Panel());
	this->label3 = (gcnew System::Windows::Forms::Label());
	this->logo_pictureBox = (gcnew System::Windows::Forms::PictureBox());
	this->header_Panel = (gcnew System::Windows::Forms::Panel());
	this->label4 = (gcnew System::Windows::Forms::Label());
	this->panel6 = (gcnew System::Windows::Forms::Panel());
	this->panel7 = (gcnew System::Windows::Forms::Panel());
	this->panel8 = (gcnew System::Windows::Forms::Panel());
	this->panel9 = (gcnew System::Windows::Forms::Panel());
	this->panel10 = (gcnew System::Windows::Forms::Panel());
	this->panel11 = (gcnew System::Windows::Forms::Panel());
	this->panel12 = (gcnew System::Windows::Forms::Panel());
	this->panel13 = (gcnew System::Windows::Forms::Panel());
	this->panel14 = (gcnew System::Windows::Forms::Panel());
	this->panel15 = (gcnew System::Windows::Forms::Panel());
	this->panel16 = (gcnew System::Windows::Forms::Panel());
	this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
	this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
	this->label19 = (gcnew System::Windows::Forms::Label());
	this->label20 = (gcnew System::Windows::Forms::Label());
	this->label18 = (gcnew System::Windows::Forms::Label());
	this->label17 = (gcnew System::Windows::Forms::Label());
	this->button15 = (gcnew System::Windows::Forms::Button());
	this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
	this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
	this->Column8 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
	this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
	this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
	this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
	this->Column4 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
	this->Column5 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
	this->Column6 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
	this->Column7 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
	this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
	this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
	this->label21 = (gcnew System::Windows::Forms::Label());
	this->label22 = (gcnew System::Windows::Forms::Label());
	this->button5 = (gcnew System::Windows::Forms::Button());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
	this->groupBox3->SuspendLayout();
	this->groupBox1->SuspendLayout();
	this->groupBox2->SuspendLayout();
	this->left_Panel->SuspendLayout();
	this->panel17->SuspendLayout();
	this->panel5->SuspendLayout();
	this->panel4->SuspendLayout();
	this->panel3->SuspendLayout();
	this->panel2->SuspendLayout();
	this->panel1->SuspendLayout();
	this->logo_Panel->SuspendLayout();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->logo_pictureBox))->BeginInit();
	this->header_Panel->SuspendLayout();
	this->groupBox4->SuspendLayout();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
	this->groupBox5->SuspendLayout();
	this->SuspendLayout();
	// 
	// openFileDialog1
	// 
	this->openFileDialog1->Filter = L"JPEG, BMP, PNG, TIF Files|*.jpg; *.jpeg; *.bmp; *.png; *.tif";
	this->openFileDialog1->InitialDirectory = L"..\\..\\ImgTstBagOf_words\\ImgsTrn\\";
	this->openFileDialog1->Title = L"Open an Image";
	// 
	// textBox2
	// 
	this->textBox2->Enabled = false;
	this->textBox2->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->textBox2->Location = System::Drawing::Point(109, 57);
	this->textBox2->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->textBox2->Name = L"textBox2";
	this->textBox2->Size = System::Drawing::Size(68, 21);
	this->textBox2->TabIndex = 8;
	this->textBox2->Text = L"100";
	this->textBox2->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox2_TextChanged);
	// 
	// label2
	// 
	this->label2->AutoSize = true;
	this->label2->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label2->Location = System::Drawing::Point(15, 60);
	this->label2->Name = L"label2";
	this->label2->Size = System::Drawing::Size(67, 16);
	this->label2->TabIndex = 9;
	this->label2->Text = L"Block\'s Size:";
	// 
	// timer1
	// 
	this->timer1->Interval = 500;
	this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
	// 
	// chart1
	// 
	this->chart1->BackColor = System::Drawing::Color::WhiteSmoke;
	this->chart1->BorderlineColor = System::Drawing::Color::WhiteSmoke;
	chartArea1->Area3DStyle->Enable3D = true;
	chartArea1->Area3DStyle->Inclination = 10;
	chartArea1->Area3DStyle->Rotation = 15;
	chartArea1->BackColor = System::Drawing::Color::Gainsboro;
	chartArea1->Name = L"ChartArea1";
	chartArea1->ShadowColor = System::Drawing::Color::White;
	this->chart1->ChartAreas->Add(chartArea1);
	legend1->BackColor = System::Drawing::SystemColors::Control;
	legend1->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	legend1->IsTextAutoFit = false;
	legend1->Name = L"Legend1";
	this->chart1->Legends->Add(legend1);
	this->chart1->Location = System::Drawing::Point(203, 471);
	this->chart1->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->chart1->Name = L"chart1";
	this->chart1->Palette = System::Windows::Forms::DataVisualization::Charting::ChartColorPalette::Berry;
	series1->ChartArea = L"ChartArea1";
	series1->Legend = L"Legend1";
	series1->Name = L"Words occurrence";
	this->chart1->Series->Add(series1);
	this->chart1->Size = System::Drawing::Size(465, 146);
	this->chart1->TabIndex = 10;
	this->chart1->Text = L"chart1";
	this->chart1->Visible = false;
	// 
	// groupBox3
	// 
	this->groupBox3->BackColor = System::Drawing::Color::WhiteSmoke;
	this->groupBox3->Controls->Add(this->label16);
	this->groupBox3->Controls->Add(this->label15);
	this->groupBox3->Controls->Add(this->button2);
	this->groupBox3->Controls->Add(this->textBox1);
	this->groupBox3->Controls->Add(this->label1);
	this->groupBox3->Controls->Add(this->textBox2);
	this->groupBox3->Controls->Add(this->label2);
	this->groupBox3->Controls->Add(this->checkBox1);
	this->groupBox3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->groupBox3->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->groupBox3->Location = System::Drawing::Point(203, 486);
	this->groupBox3->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->groupBox3->Name = L"groupBox3";
	this->groupBox3->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->groupBox3->Size = System::Drawing::Size(356, 114);
	this->groupBox3->TabIndex = 15;
	this->groupBox3->TabStop = false;
	this->groupBox3->Text = L"Parameters";
	this->groupBox3->Visible = false;
	// 
	// label16
	// 
	this->label16->AutoSize = true;
	this->label16->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label16->Location = System::Drawing::Point(220, 91);
	this->label16->Name = L"label16";
	this->label16->Size = System::Drawing::Size(57, 16);
	this->label16->TabIndex = 22;
	this->label16->Text = L"Duration:";
	this->label16->Visible = false;
	// 
	// label15
	// 
	this->label15->AutoSize = true;
	this->label15->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label15->Location = System::Drawing::Point(278, 91);
	this->label15->Name = L"label15";
	this->label15->Size = System::Drawing::Size(47, 16);
	this->label15->TabIndex = 21;
	this->label15->Text = L"label15";
	this->label15->Visible = false;
	// 
	// button2
	// 
	this->button2->BackColor = System::Drawing::Color::Gainsboro;
	this->button2->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button2->Enabled = false;
	this->button2->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button2->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->button2->Location = System::Drawing::Point(223, 21);
	this->button2->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->button2->Name = L"button2";
	this->button2->Size = System::Drawing::Size(87, 41);
	this->button2->TabIndex = 18;
	this->button2->Text = L"Create Dictionary";
	this->button2->UseVisualStyleBackColor = false;
	this->button2->Click += gcnew System::EventHandler(this, &MyForm::bCreateDictionary_Click);
	// 
	// textBox1
	// 
	this->textBox1->Enabled = false;
	this->textBox1->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->textBox1->Location = System::Drawing::Point(109, 22);
	this->textBox1->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->textBox1->Name = L"textBox1";
	this->textBox1->Size = System::Drawing::Size(68, 21);
	this->textBox1->TabIndex = 14;
	this->textBox1->Text = L"10";
	this->textBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
	// 
	// label1
	// 
	this->label1->AutoSize = true;
	this->label1->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F));
	this->label1->Location = System::Drawing::Point(15, 25);
	this->label1->Name = L"label1";
	this->label1->Size = System::Drawing::Size(88, 16);
	this->label1->TabIndex = 14;
	this->label1->Text = L"Dictionary Size:";
	// 
	// checkBox1
	// 
	this->checkBox1->AutoSize = true;
	this->checkBox1->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->checkBox1->Location = System::Drawing::Point(223, 69);
	this->checkBox1->Name = L"checkBox1";
	this->checkBox1->Size = System::Drawing::Size(92, 19);
	this->checkBox1->TabIndex = 20;
	this->checkBox1->Text = L"Show results";
	this->checkBox1->UseVisualStyleBackColor = true;
	this->checkBox1->Visible = false;
	this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::checkBox1_CheckedChanged);
	this->checkBox1->CheckStateChanged += gcnew System::EventHandler(this, &MyForm::checkBox1_CheckStateChanged);
	// 
	// PlayButton
	// 
	this->PlayButton->BackColor = System::Drawing::Color::Transparent;
	this->PlayButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PlayButton.BackgroundImage")));
	this->PlayButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
	this->PlayButton->Cursor = System::Windows::Forms::Cursors::Hand;
	this->PlayButton->FlatAppearance->BorderSize = 0;
	this->PlayButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->PlayButton->Location = System::Drawing::Point(640, 409);
	this->PlayButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->PlayButton->Name = L"PlayButton";
	this->PlayButton->Size = System::Drawing::Size(58, 62);
	this->PlayButton->TabIndex = 16;
	this->toolTip1->SetToolTip(this->PlayButton, L"Play");
	this->PlayButton->UseVisualStyleBackColor = false;
	this->PlayButton->Visible = false;
	this->PlayButton->Click += gcnew System::EventHandler(this, &MyForm::PlayButton_Click);
	// 
	// PauseButton
	// 
	this->PauseButton->BackColor = System::Drawing::Color::Transparent;
	this->PauseButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PauseButton.BackgroundImage")));
	this->PauseButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
	this->PauseButton->Cursor = System::Windows::Forms::Cursors::Hand;
	this->PauseButton->FlatAppearance->BorderSize = 0;
	this->PauseButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->PauseButton->Location = System::Drawing::Point(640, 409);
	this->PauseButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->PauseButton->Name = L"PauseButton";
	this->PauseButton->Size = System::Drawing::Size(58, 62);
	this->PauseButton->TabIndex = 17;
	this->toolTip1->SetToolTip(this->PauseButton, L"Pause");
	this->PauseButton->UseVisualStyleBackColor = false;
	this->PauseButton->Visible = false;
	this->PauseButton->Click += gcnew System::EventHandler(this, &MyForm::PauseButton_Click);
	// 
	// groupBox1
	// 
	this->groupBox1->Controls->Add(this->radioButton4);
	this->groupBox1->Controls->Add(this->button3);
	this->groupBox1->Controls->Add(this->radioButton3);
	this->groupBox1->Controls->Add(this->checkedListBox2);
	this->groupBox1->Controls->Add(this->button1);
	this->groupBox1->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->groupBox1->Location = System::Drawing::Point(203, 291);
	this->groupBox1->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->groupBox1->Name = L"groupBox1";
	this->groupBox1->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->groupBox1->Size = System::Drawing::Size(424, 138);
	this->groupBox1->TabIndex = 22;
	this->groupBox1->TabStop = false;
	this->groupBox1->Text = L"Image Histograms";
	this->groupBox1->Visible = false;
	// 
	// radioButton4
	// 
	this->radioButton4->AutoSize = true;
	this->radioButton4->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F));
	this->radioButton4->Location = System::Drawing::Point(317, 49);
	this->radioButton4->Name = L"radioButton4";
	this->radioButton4->Size = System::Drawing::Size(88, 20);
	this->radioButton4->TabIndex = 49;
	this->radioButton4->TabStop = true;
	this->radioButton4->Text = L"Test Images";
	this->radioButton4->UseVisualStyleBackColor = true;
	this->radioButton4->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton4_CheckedChanged);
	// 
	// button3
	// 
	this->button3->BackColor = System::Drawing::Color::Gainsboro;
	this->button3->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button3->Enabled = false;
	this->button3->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->button3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button3->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->button3->Location = System::Drawing::Point(317, 81);
	this->button3->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->button3->Name = L"button3";
	this->button3->Size = System::Drawing::Size(87, 41);
	this->button3->TabIndex = 24;
	this->button3->Text = L"Show Histogram";
	this->button3->UseVisualStyleBackColor = false;
	this->button3->Visible = false;
	this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
	// 
	// radioButton3
	// 
	this->radioButton3->AutoSize = true;
	this->radioButton3->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F));
	this->radioButton3->Location = System::Drawing::Point(317, 23);
	this->radioButton3->Name = L"radioButton3";
	this->radioButton3->Size = System::Drawing::Size(93, 20);
	this->radioButton3->TabIndex = 48;
	this->radioButton3->TabStop = true;
	this->radioButton3->Text = L"Train Images";
	this->radioButton3->UseVisualStyleBackColor = true;
	this->radioButton3->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton3_CheckedChanged);
	// 
	// checkedListBox2
	// 
	this->checkedListBox2->Enabled = false;
	this->checkedListBox2->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->checkedListBox2->FormattingEnabled = true;
	this->checkedListBox2->Location = System::Drawing::Point(6, 23);
	this->checkedListBox2->Name = L"checkedListBox2";
	this->checkedListBox2->Size = System::Drawing::Size(284, 100);
	this->checkedListBox2->TabIndex = 24;
	this->toolTip1->SetToolTip(this->checkedListBox2, L"Select an image...");
	this->checkedListBox2->ItemCheck += gcnew System::Windows::Forms::ItemCheckEventHandler(this, &MyForm::checkedListBox2_ItemCheck);
	// 
	// button1
	// 
	this->button1->BackColor = System::Drawing::Color::Gainsboro;
	this->button1->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button1->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button1->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->button1->Location = System::Drawing::Point(317, 81);
	this->button1->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->button1->Name = L"button1";
	this->button1->Size = System::Drawing::Size(87, 41);
	this->button1->TabIndex = 19;
	this->button1->Text = L"Create Histograms";
	this->button1->UseVisualStyleBackColor = false;
	this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
	// 
	// radioButton2
	// 
	this->radioButton2->AutoSize = true;
	this->radioButton2->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->radioButton2->Location = System::Drawing::Point(288, 91);
	this->radioButton2->Name = L"radioButton2";
	this->radioButton2->Size = System::Drawing::Size(45, 20);
	this->radioButton2->TabIndex = 25;
	this->radioButton2->TabStop = true;
	this->radioButton2->Text = L"SIFT";
	this->radioButton2->UseVisualStyleBackColor = true;
	this->radioButton2->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton2_CheckedChanged);
	// 
	// radioButton1
	// 
	this->radioButton1->AutoSize = true;
	this->radioButton1->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->radioButton1->Location = System::Drawing::Point(213, 91);
	this->radioButton1->Name = L"radioButton1";
	this->radioButton1->Size = System::Drawing::Size(57, 20);
	this->radioButton1->TabIndex = 24;
	this->radioButton1->TabStop = true;
	this->radioButton1->Text = L"Blocks";
	this->radioButton1->UseVisualStyleBackColor = true;
	this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton1_CheckedChanged);
	// 
	// groupBox2
	// 
	this->groupBox2->Controls->Add(this->label10);
	this->groupBox2->Controls->Add(this->radioButton2);
	this->groupBox2->Controls->Add(this->label9);
	this->groupBox2->Controls->Add(this->button4);
	this->groupBox2->Controls->Add(this->radioButton1);
	this->groupBox2->Controls->Add(this->label8);
	this->groupBox2->Controls->Add(this->label7);
	this->groupBox2->Controls->Add(this->label6);
	this->groupBox2->Controls->Add(this->checkedListBox1);
	this->groupBox2->Controls->Add(this->comboBox1);
	this->groupBox2->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->groupBox2->Location = System::Drawing::Point(203, 291);
	this->groupBox2->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->groupBox2->Name = L"groupBox2";
	this->groupBox2->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->groupBox2->Size = System::Drawing::Size(356, 173);
	this->groupBox2->TabIndex = 23;
	this->groupBox2->TabStop = false;
	this->groupBox2->Text = L"Images used to create the Dictionary";
	this->groupBox2->Visible = false;
	// 
	// label10
	// 
	this->label10->AutoSize = true;
	this->label10->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label10->Location = System::Drawing::Point(199, 72);
	this->label10->Name = L"label10";
	this->label10->Size = System::Drawing::Size(156, 16);
	this->label10->TabIndex = 28;
	this->label10->Text = L"Feature extraction method:";
	// 
	// label9
	// 
	this->label9->AutoSize = true;
	this->label9->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label9->Location = System::Drawing::Point(21, 141);
	this->label9->Name = L"label9";
	this->label9->Size = System::Drawing::Size(11, 16);
	this->label9->TabIndex = 27;
	this->label9->Text = L" ";
	// 
	// button4
	// 
	this->button4->BackColor = System::Drawing::Color::Gainsboro;
	this->button4->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button4->Enabled = false;
	this->button4->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->button4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button4->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->button4->Location = System::Drawing::Point(223, 118);
	this->button4->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->button4->Name = L"button4";
	this->button4->Size = System::Drawing::Size(87, 41);
	this->button4->TabIndex = 20;
	this->button4->Text = L"Select Images";
	this->button4->UseVisualStyleBackColor = false;
	this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
	// 
	// label8
	// 
	this->label8->AutoSize = true;
	this->label8->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label8->Location = System::Drawing::Point(220, 47);
	this->label8->Name = L"label8";
	this->label8->Size = System::Drawing::Size(91, 16);
	this->label8->TabIndex = 26;
	this->label8->Text = L"from directories.";
	// 
	// label7
	// 
	this->label7->AutoSize = true;
	this->label7->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label7->Location = System::Drawing::Point(309, 23);
	this->label7->Name = L"label7";
	this->label7->Size = System::Drawing::Size(42, 16);
	this->label7->TabIndex = 25;
	this->label7->Text = L"image";
	// 
	// label6
	// 
	this->label6->AutoSize = true;
	this->label6->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label6->Location = System::Drawing::Point(199, 23);
	this->label6->Name = L"label6";
	this->label6->Size = System::Drawing::Size(75, 16);
	this->label6->TabIndex = 24;
	this->label6->Text = L"Select every";
	// 
	// checkedListBox1
	// 
	this->checkedListBox1->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->checkedListBox1->FormattingEnabled = true;
	this->checkedListBox1->Location = System::Drawing::Point(8, 23);
	this->checkedListBox1->Name = L"checkedListBox1";
	this->checkedListBox1->Size = System::Drawing::Size(185, 100);
	this->checkedListBox1->TabIndex = 16;
	this->toolTip1->SetToolTip(this->checkedListBox1, L"Select at least 2 classes.");
	this->checkedListBox1->ItemCheck += gcnew System::Windows::Forms::ItemCheckEventHandler(this, &MyForm::checkedListBox1_ItemCheck);
	// 
	// comboBox1
	// 
	this->comboBox1->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->comboBox1->FormattingEnabled = true;
	this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"2", L"3" });
	this->comboBox1->Location = System::Drawing::Point(277, 20);
	this->comboBox1->Name = L"comboBox1";
	this->comboBox1->Size = System::Drawing::Size(29, 24);
	this->comboBox1->TabIndex = 24;
	// 
	// button8
	// 
	this->button8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
	this->button8->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button8.BackgroundImage")));
	this->button8->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
	this->button8->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button8->FlatAppearance->BorderSize = 0;
	this->button8->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
	this->button8->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->button8->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button8->Location = System::Drawing::Point(810, 0);
	this->button8->Name = L"button8";
	this->button8->Size = System::Drawing::Size(25, 25);
	this->button8->TabIndex = 28;
	this->toolTip1->SetToolTip(this->button8, L"Minimize");
	this->button8->UseVisualStyleBackColor = true;
	this->button8->Click += gcnew System::EventHandler(this, &MyForm::button8_Click);
	// 
	// button9
	// 
	this->button9->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
	this->button9->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button9.BackgroundImage")));
	this->button9->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
	this->button9->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button9->FlatAppearance->BorderSize = 0;
	this->button9->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
	this->button9->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->button9->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button9->Location = System::Drawing::Point(835, 0);
	this->button9->Name = L"button9";
	this->button9->Size = System::Drawing::Size(25, 25);
	this->button9->TabIndex = 29;
	this->toolTip1->SetToolTip(this->button9, L"Maximize");
	this->button9->UseVisualStyleBackColor = true;
	this->button9->Click += gcnew System::EventHandler(this, &MyForm::button9_Click);
	// 
	// button7
	// 
	this->button7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
	this->button7->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button7.BackgroundImage")));
	this->button7->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
	this->button7->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button7->FlatAppearance->BorderSize = 0;
	this->button7->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
	this->button7->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->button7->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button7->Location = System::Drawing::Point(860, 0);
	this->button7->Name = L"button7";
	this->button7->Size = System::Drawing::Size(25, 25);
	this->button7->TabIndex = 27;
	this->toolTip1->SetToolTip(this->button7, L"Exit");
	this->button7->UseVisualStyleBackColor = true;
	this->button7->Click += gcnew System::EventHandler(this, &MyForm::button7_Click);
	// 
	// left_Panel
	// 
	this->left_Panel->BackColor = System::Drawing::Color::LightSlateGray;
	this->left_Panel->Controls->Add(this->button6);
	this->left_Panel->Controls->Add(this->panel17);
	this->left_Panel->Controls->Add(this->button14);
	this->left_Panel->Controls->Add(this->button13);
	this->left_Panel->Controls->Add(this->button12);
	this->left_Panel->Controls->Add(this->panel5);
	this->left_Panel->Controls->Add(this->button11);
	this->left_Panel->Controls->Add(this->panel4);
	this->left_Panel->Controls->Add(this->button10);
	this->left_Panel->Controls->Add(this->panel3);
	this->left_Panel->Controls->Add(this->panel2);
	this->left_Panel->Controls->Add(this->panel1);
	this->left_Panel->Controls->Add(this->logo_Panel);
	this->left_Panel->Dock = System::Windows::Forms::DockStyle::Left;
	this->left_Panel->Location = System::Drawing::Point(0, 0);
	this->left_Panel->Name = L"left_Panel";
	this->left_Panel->Size = System::Drawing::Size(185, 630);
	this->left_Panel->TabIndex = 25;
	// 
	// button6
	// 
	this->button6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(33)),
		static_cast<System::Int32>(static_cast<System::Byte>(122)));
	this->button6->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button6->Dock = System::Windows::Forms::DockStyle::Top;
	this->button6->Enabled = false;
	this->button6->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->button6->FlatAppearance->BorderSize = 0;
	this->button6->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
	this->button6->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->button6->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button6->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->button6->ForeColor = System::Drawing::Color::White;
	this->button6->Location = System::Drawing::Point(0, 548);
	this->button6->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->button6->Name = L"button6";
	this->button6->Size = System::Drawing::Size(185, 30);
	this->button6->TabIndex = 35;
	this->button6->Text = L"Reset All";
	this->button6->UseVisualStyleBackColor = false;
	this->button6->Click += gcnew System::EventHandler(this, &MyForm::button6_Click);
	// 
	// panel17
	// 
	this->panel17->BackColor = System::Drawing::Color::LightSlateGray;
	this->panel17->Controls->Add(this->label23);
	this->panel17->Dock = System::Windows::Forms::DockStyle::Top;
	this->panel17->Location = System::Drawing::Point(0, 406);
	this->panel17->Name = L"panel17";
	this->panel17->Size = System::Drawing::Size(185, 142);
	this->panel17->TabIndex = 34;
	// 
	// label23
	// 
	this->label23->AutoSize = true;
	this->label23->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label23->ForeColor = System::Drawing::Color::Black;
	this->label23->Location = System::Drawing::Point(25, 16);
	this->label23->Name = L"label23";
	this->label23->Size = System::Drawing::Size(0, 16);
	this->label23->TabIndex = 27;
	// 
	// button14
	// 
	this->button14->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(33)),
		static_cast<System::Int32>(static_cast<System::Byte>(122)));
	this->button14->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button14->Dock = System::Windows::Forms::DockStyle::Top;
	this->button14->Enabled = false;
	this->button14->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->button14->FlatAppearance->BorderSize = 0;
	this->button14->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
	this->button14->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->button14->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button14->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->button14->ForeColor = System::Drawing::Color::White;
	this->button14->Location = System::Drawing::Point(0, 376);
	this->button14->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->button14->Name = L"button14";
	this->button14->Size = System::Drawing::Size(185, 30);
	this->button14->TabIndex = 33;
	this->button14->Text = L"III. Scene Classification";
	this->button14->UseVisualStyleBackColor = false;
	this->button14->Click += gcnew System::EventHandler(this, &MyForm::button14_Click);
	// 
	// button13
	// 
	this->button13->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(33)),
		static_cast<System::Int32>(static_cast<System::Byte>(122)));
	this->button13->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button13->Dock = System::Windows::Forms::DockStyle::Top;
	this->button13->Enabled = false;
	this->button13->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->button13->FlatAppearance->BorderSize = 0;
	this->button13->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
	this->button13->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->button13->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button13->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->button13->ForeColor = System::Drawing::Color::White;
	this->button13->Location = System::Drawing::Point(0, 346);
	this->button13->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->button13->Name = L"button13";
	this->button13->Size = System::Drawing::Size(185, 30);
	this->button13->TabIndex = 32;
	this->button13->Text = L"II. Histogram Representation";
	this->button13->UseVisualStyleBackColor = false;
	this->button13->Click += gcnew System::EventHandler(this, &MyForm::button13_Click);
	// 
	// button12
	// 
	this->button12->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(33)),
		static_cast<System::Int32>(static_cast<System::Byte>(122)));
	this->button12->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button12->Dock = System::Windows::Forms::DockStyle::Top;
	this->button12->Enabled = false;
	this->button12->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->button12->FlatAppearance->BorderSize = 0;
	this->button12->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
	this->button12->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->button12->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button12->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->button12->ForeColor = System::Drawing::Color::White;
	this->button12->Location = System::Drawing::Point(0, 316);
	this->button12->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->button12->Name = L"button12";
	this->button12->Size = System::Drawing::Size(185, 30);
	this->button12->TabIndex = 31;
	this->button12->Text = L"I. Create Dictionary";
	this->button12->UseVisualStyleBackColor = false;
	this->button12->Click += gcnew System::EventHandler(this, &MyForm::button12_Click);
	// 
	// panel5
	// 
	this->panel5->BackColor = System::Drawing::Color::LightSlateGray;
	this->panel5->Controls->Add(this->label14);
	this->panel5->Dock = System::Windows::Forms::DockStyle::Top;
	this->panel5->Location = System::Drawing::Point(0, 266);
	this->panel5->Name = L"panel5";
	this->panel5->Size = System::Drawing::Size(185, 50);
	this->panel5->TabIndex = 30;
	// 
	// label14
	// 
	this->label14->AutoSize = true;
	this->label14->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label14->ForeColor = System::Drawing::Color::Black;
	this->label14->Location = System::Drawing::Point(25, 16);
	this->label14->Name = L"label14";
	this->label14->Size = System::Drawing::Size(137, 16);
	this->label14->TabIndex = 27;
	this->label14->Text = L"\"Bag-of-Words\" Model";
	// 
	// button11
	// 
	this->button11->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(33)),
		static_cast<System::Int32>(static_cast<System::Byte>(122)));
	this->button11->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button11->Dock = System::Windows::Forms::DockStyle::Top;
	this->button11->Enabled = false;
	this->button11->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->button11->FlatAppearance->BorderSize = 0;
	this->button11->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
	this->button11->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->button11->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button11->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->button11->ForeColor = System::Drawing::Color::White;
	this->button11->Location = System::Drawing::Point(0, 236);
	this->button11->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->button11->Name = L"button11";
	this->button11->Size = System::Drawing::Size(185, 30);
	this->button11->TabIndex = 29;
	this->button11->Text = L"Open Image";
	this->button11->UseVisualStyleBackColor = false;
	this->button11->Click += gcnew System::EventHandler(this, &MyForm::button11_Click);
	// 
	// panel4
	// 
	this->panel4->BackColor = System::Drawing::Color::LightSlateGray;
	this->panel4->Controls->Add(this->label13);
	this->panel4->Dock = System::Windows::Forms::DockStyle::Top;
	this->panel4->Location = System::Drawing::Point(0, 186);
	this->panel4->Name = L"panel4";
	this->panel4->Size = System::Drawing::Size(185, 50);
	this->panel4->TabIndex = 28;
	// 
	// label13
	// 
	this->label13->AutoSize = true;
	this->label13->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label13->ForeColor = System::Drawing::Color::Black;
	this->label13->Location = System::Drawing::Point(75, 16);
	this->label13->Name = L"label13";
	this->label13->Size = System::Drawing::Size(28, 16);
	this->label13->TabIndex = 27;
	this->label13->Text = L"File";
	// 
	// button10
	// 
	this->button10->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(33)),
		static_cast<System::Int32>(static_cast<System::Byte>(122)));
	this->button10->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button10->Dock = System::Windows::Forms::DockStyle::Top;
	this->button10->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->button10->FlatAppearance->BorderSize = 0;
	this->button10->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
	this->button10->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
		static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
	this->button10->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button10->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->button10->ForeColor = System::Drawing::Color::White;
	this->button10->Location = System::Drawing::Point(0, 156);
	this->button10->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->button10->Name = L"button10";
	this->button10->Size = System::Drawing::Size(185, 30);
	this->button10->TabIndex = 27;
	this->button10->Text = L"Select Path";
	this->button10->UseVisualStyleBackColor = false;
	this->button10->Click += gcnew System::EventHandler(this, &MyForm::button10_Click);
	// 
	// panel3
	// 
	this->panel3->Controls->Add(this->label12);
	this->panel3->Controls->Add(this->label11);
	this->panel3->Dock = System::Windows::Forms::DockStyle::Bottom;
	this->panel3->Location = System::Drawing::Point(0, 580);
	this->panel3->Name = L"panel3";
	this->panel3->Size = System::Drawing::Size(185, 50);
	this->panel3->TabIndex = 27;
	// 
	// label12
	// 
	this->label12->AutoSize = true;
	this->label12->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label12->ForeColor = System::Drawing::Color::Black;
	this->label12->Location = System::Drawing::Point(64, 15);
	this->label12->Name = L"label12";
	this->label12->Size = System::Drawing::Size(110, 15);
	this->label12->TabIndex = 28;
	this->label12->Text = L"Orbai Andrei-Dorel";
	// 
	// label11
	// 
	this->label11->AutoSize = true;
	this->label11->ForeColor = System::Drawing::Color::Black;
	this->label11->Location = System::Drawing::Point(12, 15);
	this->label11->Name = L"label11";
	this->label11->Size = System::Drawing::Size(46, 16);
	this->label11->TabIndex = 27;
	this->label11->Text = L"Author:";
	// 
	// panel2
	// 
	this->panel2->BackColor = System::Drawing::Color::LightSlateGray;
	this->panel2->Controls->Add(this->textBox3);
	this->panel2->Dock = System::Windows::Forms::DockStyle::Top;
	this->panel2->Location = System::Drawing::Point(0, 106);
	this->panel2->Name = L"panel2";
	this->panel2->Size = System::Drawing::Size(185, 50);
	this->panel2->TabIndex = 27;
	// 
	// textBox3
	// 
	this->textBox3->Location = System::Drawing::Point(12, 15);
	this->textBox3->Name = L"textBox3";
	this->textBox3->Size = System::Drawing::Size(164, 21);
	this->textBox3->TabIndex = 27;
	this->textBox3->Text = L"..\\\\ImgTstBagOfWords\\\\";
	this->textBox3->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox3_TextChanged);
	// 
	// panel1
	// 
	this->panel1->BackColor = System::Drawing::Color::LightSlateGray;
	this->panel1->Controls->Add(this->label5);
	this->panel1->Dock = System::Windows::Forms::DockStyle::Top;
	this->panel1->Location = System::Drawing::Point(0, 56);
	this->panel1->Name = L"panel1";
	this->panel1->Size = System::Drawing::Size(185, 50);
	this->panel1->TabIndex = 27;
	// 
	// label5
	// 
	this->label5->AutoSize = true;
	this->label5->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label5->ForeColor = System::Drawing::Color::Black;
	this->label5->Location = System::Drawing::Point(25, 16);
	this->label5->Name = L"label5";
	this->label5->Size = System::Drawing::Size(136, 16);
	this->label5->TabIndex = 27;
	this->label5->Text = L"Images Location Path:";
	// 
	// logo_Panel
	// 
	this->logo_Panel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(33)),
		static_cast<System::Int32>(static_cast<System::Byte>(122)));
	this->logo_Panel->Controls->Add(this->label3);
	this->logo_Panel->Controls->Add(this->logo_pictureBox);
	this->logo_Panel->Dock = System::Windows::Forms::DockStyle::Top;
	this->logo_Panel->Location = System::Drawing::Point(0, 0);
	this->logo_Panel->Name = L"logo_Panel";
	this->logo_Panel->Size = System::Drawing::Size(185, 56);
	this->logo_Panel->TabIndex = 27;
	this->logo_Panel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsDown);
	this->logo_Panel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsMove);
	this->logo_Panel->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsUp);
	// 
	// label3
	// 
	this->label3->AutoSize = true;
	this->label3->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9.75F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
		System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
	this->label3->ForeColor = System::Drawing::Color::Gainsboro;
	this->label3->Location = System::Drawing::Point(54, 19);
	this->label3->Name = L"label3";
	this->label3->Size = System::Drawing::Size(122, 16);
	this->label3->TabIndex = 27;
	this->label3->Text = L"Image Processing";
	this->label3->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsDown);
	this->label3->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsMove);
	this->label3->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsUp);
	// 
	// logo_pictureBox
	// 
	this->logo_pictureBox->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"logo_pictureBox.Image")));
	this->logo_pictureBox->Location = System::Drawing::Point(12, 10);
	this->logo_pictureBox->Name = L"logo_pictureBox";
	this->logo_pictureBox->Size = System::Drawing::Size(36, 35);
	this->logo_pictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
	this->logo_pictureBox->TabIndex = 0;
	this->logo_pictureBox->TabStop = false;
	this->logo_pictureBox->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsDown);
	this->logo_pictureBox->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsMove);
	this->logo_pictureBox->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsUp);
	// 
	// header_Panel
	// 
	this->header_Panel->BackColor = System::Drawing::Color::LightSlateGray;
	this->header_Panel->Controls->Add(this->button8);
	this->header_Panel->Controls->Add(this->button9);
	this->header_Panel->Controls->Add(this->button7);
	this->header_Panel->Controls->Add(this->label4);
	this->header_Panel->Dock = System::Windows::Forms::DockStyle::Top;
	this->header_Panel->Location = System::Drawing::Point(185, 0);
	this->header_Panel->Name = L"header_Panel";
	this->header_Panel->Size = System::Drawing::Size(885, 56);
	this->header_Panel->TabIndex = 26;
	this->header_Panel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsDown);
	this->header_Panel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsMove);
	this->header_Panel->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsUp);
	// 
	// label4
	// 
	this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
	this->label4->AutoSize = true;
	this->label4->Font = (gcnew System::Drawing::Font(L"Century Gothic", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label4->ForeColor = System::Drawing::Color::Black;
	this->label4->Location = System::Drawing::Point(183, 20);
	this->label4->Name = L"label4";
	this->label4->Size = System::Drawing::Size(437, 21);
	this->label4->TabIndex = 27;
	this->label4->Text = L"Sumarizarea secvențelor video de supravegere urbană";
	this->label4->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsDown);
	this->label4->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsMove);
	this->label4->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseIsUp);
	// 
	// panel6
	// 
	this->panel6->BackColor = System::Drawing::Color::Black;
	this->panel6->Location = System::Drawing::Point(0, 106);
	this->panel6->Name = L"panel6";
	this->panel6->Size = System::Drawing::Size(185, 1);
	this->panel6->TabIndex = 34;
	// 
	// panel7
	// 
	this->panel7->BackColor = System::Drawing::Color::Black;
	this->panel7->Location = System::Drawing::Point(0, 186);
	this->panel7->Name = L"panel7";
	this->panel7->Size = System::Drawing::Size(185, 1);
	this->panel7->TabIndex = 35;
	// 
	// panel8
	// 
	this->panel8->BackColor = System::Drawing::Color::Black;
	this->panel8->Location = System::Drawing::Point(0, 236);
	this->panel8->Name = L"panel8";
	this->panel8->Size = System::Drawing::Size(185, 1);
	this->panel8->TabIndex = 36;
	// 
	// panel9
	// 
	this->panel9->BackColor = System::Drawing::Color::Black;
	this->panel9->Location = System::Drawing::Point(0, 266);
	this->panel9->Name = L"panel9";
	this->panel9->Size = System::Drawing::Size(185, 1);
	this->panel9->TabIndex = 37;
	// 
	// panel10
	// 
	this->panel10->BackColor = System::Drawing::Color::Black;
	this->panel10->Location = System::Drawing::Point(0, 316);
	this->panel10->Name = L"panel10";
	this->panel10->Size = System::Drawing::Size(185, 1);
	this->panel10->TabIndex = 38;
	// 
	// panel11
	// 
	this->panel11->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
		| System::Windows::Forms::AnchorStyles::Right));
	this->panel11->BackColor = System::Drawing::Color::Black;
	this->panel11->Location = System::Drawing::Point(0, 0);
	this->panel11->Name = L"panel11";
	this->panel11->Size = System::Drawing::Size(1070, 1);
	this->panel11->TabIndex = 39;
	// 
	// panel12
	// 
	this->panel12->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
		| System::Windows::Forms::AnchorStyles::Right));
	this->panel12->BackColor = System::Drawing::Color::Black;
	this->panel12->Location = System::Drawing::Point(0, 629);
	this->panel12->Name = L"panel12";
	this->panel12->Size = System::Drawing::Size(1069, 1);
	this->panel12->TabIndex = 40;
	// 
	// panel13
	// 
	this->panel13->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
		| System::Windows::Forms::AnchorStyles::Right));
	this->panel13->BackColor = System::Drawing::Color::Black;
	this->panel13->Location = System::Drawing::Point(0, 56);
	this->panel13->Name = L"panel13";
	this->panel13->Size = System::Drawing::Size(1070, 1);
	this->panel13->TabIndex = 41;
	// 
	// panel14
	// 
	this->panel14->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
		| System::Windows::Forms::AnchorStyles::Left));
	this->panel14->BackColor = System::Drawing::Color::Black;
	this->panel14->Location = System::Drawing::Point(0, 0);
	this->panel14->Name = L"panel14";
	this->panel14->Size = System::Drawing::Size(1, 630);
	this->panel14->TabIndex = 42;
	// 
	// panel15
	// 
	this->panel15->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
		| System::Windows::Forms::AnchorStyles::Left));
	this->panel15->BackColor = System::Drawing::Color::Black;
	this->panel15->Location = System::Drawing::Point(185, 0);
	this->panel15->Name = L"panel15";
	this->panel15->Size = System::Drawing::Size(1, 630);
	this->panel15->TabIndex = 43;
	// 
	// panel16
	// 
	this->panel16->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
		| System::Windows::Forms::AnchorStyles::Right));
	this->panel16->BackColor = System::Drawing::Color::Black;
	this->panel16->Location = System::Drawing::Point(1069, 0);
	this->panel16->Name = L"panel16";
	this->panel16->Size = System::Drawing::Size(1, 630);
	this->panel16->TabIndex = 44;
	// 
	// groupBox4
	// 
	this->groupBox4->Controls->Add(this->checkBox2);
	this->groupBox4->Controls->Add(this->label19);
	this->groupBox4->Controls->Add(this->label20);
	this->groupBox4->Controls->Add(this->label18);
	this->groupBox4->Controls->Add(this->label17);
	this->groupBox4->Controls->Add(this->button15);
	this->groupBox4->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->groupBox4->Location = System::Drawing::Point(203, 291);
	this->groupBox4->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->groupBox4->Name = L"groupBox4";
	this->groupBox4->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->groupBox4->Size = System::Drawing::Size(290, 138);
	this->groupBox4->TabIndex = 45;
	this->groupBox4->TabStop = false;
	this->groupBox4->Text = L"SVM classifier";
	this->groupBox4->Visible = false;
	// 
	// checkBox2
	// 
	this->checkBox2->AutoSize = true;
	this->checkBox2->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->checkBox2->Location = System::Drawing::Point(138, 91);
	this->checkBox2->Name = L"checkBox2";
	this->checkBox2->Size = System::Drawing::Size(77, 20);
	this->checkBox2->TabIndex = 46;
	this->checkBox2->Text = L"Summary";
	this->checkBox2->UseVisualStyleBackColor = true;
	this->checkBox2->Visible = false;
	this->checkBox2->CheckStateChanged += gcnew System::EventHandler(this, &MyForm::checkBox2_CheckStateChanged);
	// 
	// label19
	// 
	this->label19->AutoSize = true;
	this->label19->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label19->Location = System::Drawing::Point(135, 52);
	this->label19->Name = L"label19";
	this->label19->Size = System::Drawing::Size(60, 16);
	this->label19->TabIndex = 46;
	this->label19->Text = L"The result:";
	// 
	// label20
	// 
	this->label20->AutoSize = true;
	this->label20->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label20->Location = System::Drawing::Point(196, 52);
	this->label20->Name = L"label20";
	this->label20->Size = System::Drawing::Size(0, 16);
	this->label20->TabIndex = 47;
	// 
	// label18
	// 
	this->label18->AutoSize = true;
	this->label18->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label18->Location = System::Drawing::Point(225, 27);
	this->label18->Name = L"label18";
	this->label18->Size = System::Drawing::Size(0, 16);
	this->label18->TabIndex = 26;
	// 
	// label17
	// 
	this->label17->AutoSize = true;
	this->label17->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label17->Location = System::Drawing::Point(135, 27);
	this->label17->Name = L"label17";
	this->label17->Size = System::Drawing::Size(90, 16);
	this->label17->TabIndex = 25;
	this->label17->Text = L"Nr. of Test imgs.:";
	// 
	// button15
	// 
	this->button15->BackColor = System::Drawing::Color::Gainsboro;
	this->button15->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button15->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->button15->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button15->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->button15->Location = System::Drawing::Point(29, 27);
	this->button15->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->button15->Name = L"button15";
	this->button15->Size = System::Drawing::Size(87, 41);
	this->button15->TabIndex = 24;
	this->button15->Text = L"Classify";
	this->button15->UseVisualStyleBackColor = false;
	this->button15->Visible = false;
	this->button15->Click += gcnew System::EventHandler(this, &MyForm::button15_Click);
	// 
	// pictureBox4
	// 
	this->pictureBox4->Location = System::Drawing::Point(710, 305);
	this->pictureBox4->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->pictureBox4->Name = L"pictureBox4";
	this->pictureBox4->Size = System::Drawing::Size(325, 275);
	this->pictureBox4->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
	this->pictureBox4->TabIndex = 21;
	this->pictureBox4->TabStop = false;
	this->pictureBox4->Visible = false;
	// 
	// dataGridView1
	// 
	this->dataGridView1->AllowUserToAddRows = false;
	this->dataGridView1->AllowUserToDeleteRows = false;
	this->dataGridView1->BackgroundColor = System::Drawing::Color::WhiteSmoke;
	this->dataGridView1->BorderStyle = System::Windows::Forms::BorderStyle::None;
	this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
	this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(8) {
		this->Column8,
			this->Column1, this->Column2, this->Column3, this->Column4, this->Column5, this->Column6, this->Column7
	});
	this->dataGridView1->Location = System::Drawing::Point(203, 456);
	this->dataGridView1->Name = L"dataGridView1";
	this->dataGridView1->ReadOnly = true;
	this->dataGridView1->Size = System::Drawing::Size(742, 144);
	this->dataGridView1->TabIndex = 48;
	this->dataGridView1->Visible = false;
	// 
	// Column8
	// 
	this->Column8->HeaderText = L"Nr.";
	this->Column8->Name = L"Column8";
	this->Column8->ReadOnly = true;
	this->Column8->Width = 30;
	// 
	// Column1
	// 
	this->Column1->HeaderText = L"Classes";
	this->Column1->Name = L"Column1";
	this->Column1->ReadOnly = true;
	this->Column1->Width = 70;
	// 
	// Column2
	// 
	this->Column2->HeaderText = L"Total Imgs.";
	this->Column2->Name = L"Column2";
	this->Column2->ReadOnly = true;
	this->Column2->Width = 80;
	// 
	// Column3
	// 
	this->Column3->HeaderText = L"Train Imgs.";
	this->Column3->Name = L"Column3";
	this->Column3->ReadOnly = true;
	this->Column3->Width = 80;
	// 
	// Column4
	// 
	this->Column4->HeaderText = L"Test Imgs.";
	this->Column4->Name = L"Column4";
	this->Column4->ReadOnly = true;
	this->Column4->Width = 80;
	// 
	// Column5
	// 
	this->Column5->HeaderText = L"Feature Extr. method";
	this->Column5->Name = L"Column5";
	this->Column5->ReadOnly = true;
	// 
	// Column6
	// 
	this->Column6->HeaderText = L"BoW (time)";
	this->Column6->Name = L"Column6";
	this->Column6->ReadOnly = true;
	this->Column6->Width = 95;
	// 
	// Column7
	// 
	this->Column7->HeaderText = L"Classification";
	this->Column7->Name = L"Column7";
	this->Column7->ReadOnly = true;
	this->Column7->Width = 90;
	// 
	// pictureBox1
	// 
	this->pictureBox1->Location = System::Drawing::Point(613, 291);
	this->pictureBox1->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->pictureBox1->Name = L"pictureBox1";
	this->pictureBox1->Size = System::Drawing::Size(160, 150);
	this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
	this->pictureBox1->TabIndex = 49;
	this->pictureBox1->TabStop = false;
	this->pictureBox1->Visible = false;
	// 
	// groupBox5
	// 
	this->groupBox5->Controls->Add(this->label21);
	this->groupBox5->Controls->Add(this->label22);
	this->groupBox5->Controls->Add(this->button5);
	this->groupBox5->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->groupBox5->Location = System::Drawing::Point(799, 305);
	this->groupBox5->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->groupBox5->Name = L"groupBox5";
	this->groupBox5->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->groupBox5->Size = System::Drawing::Size(196, 126);
	this->groupBox5->TabIndex = 50;
	this->groupBox5->TabStop = false;
	this->groupBox5->Text = L"Image Classification";
	this->groupBox5->Visible = false;
	// 
	// label21
	// 
	this->label21->AutoSize = true;
	this->label21->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label21->Location = System::Drawing::Point(6, 93);
	this->label21->Name = L"label21";
	this->label21->Size = System::Drawing::Size(60, 16);
	this->label21->TabIndex = 46;
	this->label21->Text = L"The result:";
	// 
	// label22
	// 
	this->label22->AutoSize = true;
	this->label22->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label22->ForeColor = System::Drawing::Color::Red;
	this->label22->Location = System::Drawing::Point(67, 93);
	this->label22->Name = L"label22";
	this->label22->Size = System::Drawing::Size(0, 16);
	this->label22->TabIndex = 47;
	// 
	// button5
	// 
	this->button5->BackColor = System::Drawing::Color::Gainsboro;
	this->button5->Cursor = System::Windows::Forms::Cursors::Hand;
	this->button5->FlatAppearance->BorderColor = System::Drawing::Color::Black;
	this->button5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	this->button5->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->button5->Location = System::Drawing::Point(57, 28);
	this->button5->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->button5->Name = L"button5";
	this->button5->Size = System::Drawing::Size(87, 41);
	this->button5->TabIndex = 24;
	this->button5->Text = L"Classify";
	this->button5->UseVisualStyleBackColor = false;
	this->button5->Visible = false;
	this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
	// 
	// MyForm
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(7, 16);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->BackColor = System::Drawing::Color::WhiteSmoke;
	this->ClientSize = System::Drawing::Size(1070, 630);
	this->Controls->Add(this->groupBox5);
	this->Controls->Add(this->pictureBox1);
	this->Controls->Add(this->dataGridView1);
	this->Controls->Add(this->groupBox4);
	this->Controls->Add(this->panel16);
	this->Controls->Add(this->panel15);
	this->Controls->Add(this->panel14);
	this->Controls->Add(this->panel13);
	this->Controls->Add(this->panel12);
	this->Controls->Add(this->panel11);
	this->Controls->Add(this->panel10);
	this->Controls->Add(this->panel9);
	this->Controls->Add(this->panel8);
	this->Controls->Add(this->panel7);
	this->Controls->Add(this->panel6);
	this->Controls->Add(this->header_Panel);
	this->Controls->Add(this->left_Panel);
	this->Controls->Add(this->groupBox1);
	this->Controls->Add(this->PauseButton);
	this->Controls->Add(this->groupBox3);
	this->Controls->Add(this->chart1);
	this->Controls->Add(this->groupBox2);
	this->Controls->Add(this->PlayButton);
	this->Controls->Add(this->pictureBox4);
	this->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->Name = L"MyForm";
	this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
	this->Text = L"Image Processing";
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
	this->groupBox3->ResumeLayout(false);
	this->groupBox3->PerformLayout();
	this->groupBox1->ResumeLayout(false);
	this->groupBox1->PerformLayout();
	this->groupBox2->ResumeLayout(false);
	this->groupBox2->PerformLayout();
	this->left_Panel->ResumeLayout(false);
	this->panel17->ResumeLayout(false);
	this->panel17->PerformLayout();
	this->panel5->ResumeLayout(false);
	this->panel5->PerformLayout();
	this->panel4->ResumeLayout(false);
	this->panel4->PerformLayout();
	this->panel3->ResumeLayout(false);
	this->panel3->PerformLayout();
	this->panel2->ResumeLayout(false);
	this->panel2->PerformLayout();
	this->panel1->ResumeLayout(false);
	this->panel1->PerformLayout();
	this->logo_Panel->ResumeLayout(false);
	this->logo_Panel->PerformLayout();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->logo_pictureBox))->EndInit();
	this->header_Panel->ResumeLayout(false);
	this->header_Panel->PerformLayout();
	this->groupBox4->ResumeLayout(false);
	this->groupBox4->PerformLayout();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
	this->groupBox5->ResumeLayout(false);
	this->groupBox5->PerformLayout();
	this->ResumeLayout(false);

}

	#pragma endregion

//#(Select) - Images Location Path
private: System::Void button10_Click(System::Object^  sender, System::EventArgs^  e) {

	_getDir = textBox3->Text;//#Initialize the specific directory path
	_getAllSubDir = Directory::GetDirectories(_getDir);//#Initialize all the sub-directories paths
	myImageFlags = new int[_getAllSubDir->Length];//#Initialize the length with all sub-directories
	subDirImgArray = new Mat*[_getAllSubDir->Length];//#Initialize the Mat of Mat sub-directories images

	//#Initialize the dynamically array's - PictureBoxes,Labels,Buttons
	//-------------------------------------------------------------------
	pBoxes = gcnew array<PictureBox^>(_getAllSubDir->Length);//#Initialize PicBoxes of specific No. sub-directories
	pLabels = gcnew array<Label^>(_getAllSubDir->Length);//#Initialize Labels of specific No. sub-directories
	pPlayButtons = gcnew array<Button^>(_getAllSubDir->Length);//#Initialize Play-Buttons of specific No. sub-directories
	pPauseButtons = gcnew array<Button^>(_getAllSubDir->Length);//#Initialize Pause-Buttons of specific No. sub-directories
	pTimers = gcnew array<Timer^>(_getAllSubDir->Length);//#Initialize Timers of specific No. sub-directories
	timerSlide = new int[_getAllSubDir->Length];//#Initialize TimerSlide of specific No. sub-directories

	button10->Enabled = false;
	textBox3->Enabled = false;
	button12->Enabled = true;

}

//***** Done *****
	#pragma region Create Dictionary

//#Loading all Images from each Sub-directory
private: void LoadAllSubDirImages(){
	
	for (int i = 0; i < _getAllSubDir->Length; i++)
	{			
		_getDirFiles = Directory::GetFiles(_getAllSubDir[i], "*.jpg");//The .jpg file names
		subDirImgArray[i] = new Mat[_getDirFiles->Length];		

		myImageFlags[i] = _getDirFiles->Length;//#Store each sub-directory length value
		totalImages += _getDirFiles->Length;//#Store the total Images value

		for (int j = 0; j < _getDirFiles->Length; j++)
		{									
			System::String^ _fileName = _getDirFiles[j];//Store the current name of the file
			subDirImgArray[i][j] = imread(msclr::interop::marshal_as< std::string >(_fileName), CV_LOAD_IMAGE_GRAYSCALE);//#CV_LOAD_IMAGE_COLOR
			cvtColor(subDirImgArray[i][j], subDirImgArray[i][j], CV_GRAY2RGB); 
		}
			
		PopulateListBox(checkedListBox1, i, _getAllSubDir, i);
		SetLabelValue(label9, countImgs);
		comboBox1->SelectedIndex = 0;//#Set the initial value of ComboBox '1'
	}	
		
}

	#pragma region (Methods) CheckedListbox

//#Populate the ListBox with values
private: void PopulateListBox(CheckedListBox^ sender, int pozList, array<System::String^>^ dirOrfiles, int pozString){
	
	sender->Items->Insert(pozList, dirOrfiles[pozString]->ToString());//#Adding the directories to ListBox
	//sender->SetItemChecked(position, true);//#Making the state of all items, checked!
	
}

//#Method raised on checkedListBox1 ItemCheck state is changed
private: System::Void checkedListBox1_ItemCheck(System::Object^  sender, System::Windows::Forms::ItemCheckEventArgs^  e) {

	radioButton1->Checked = false;
	radioButton2->Checked = false;
	CountCheckedItems(e->Index, e->CurrentValue);
	//button4->Enabled = true;

}

//#Method used to Count the checked sub-directories Images 
private: void CountCheckedItems(int index, CheckState val){

	if (val == CheckState::Checked)
	{
		countCheckedClasses--;
		countDirs--;
		countImgs -= myImageFlags[index];
	}
	else if (val == CheckState::Unchecked)
	{
		countCheckedClasses++;
		countDirs++;
		countImgs += myImageFlags[index];
	}

	SetLabelValue(label9, countImgs);

}

	#pragma endregion

//#Method used to Display Selected Images (how many) from sub-directories to Label
private: void SetLabelValue(Label^ label, int val){

	label->Text = val.ToString() + " Images Selected!";

}

//#Select Images used to Create the Dictionary (1 mistake at divide!)
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
			 
	int x = 0;//#The x Axis value
	int countTest = 0;
	int countTrain = 0;
	takeBy = Convert::ToInt32(comboBox1->Text);//#Get the value from ComboBox
		
	//#Initialize the Train/Test Images
	//----------------------------------
	//int remain = countImgs % takeBy;
	ImgsTrain = new Mat[(int)(countImgs / takeBy)];//#Initialize the TrainImages
	ImgsTest = new Mat[(int)(countImgs - (int)(countImgs / takeBy))];//#Initialize the TestImages

	ImgsResults = new Mat[(int)(countImgs / takeBy)];//#Initialize the ResultImages

	for (int i = 0; i < _getAllSubDir->Length; i++)	
		if (checkedListBox1->GetItemCheckState(i) == CheckState::Checked)
		{		
			
			for (int j = 0; j < myImageFlags[i]; j++)
			{
				if (j % takeBy == 0)
				{
					ImgsTrain[countTrain++] = subDirImgArray[i][j];//#Count images for train					
					LabelsTrain.push_back(std::stoi(msclr::interop::marshal_as<std::string>(_getAllSubDir[i]->Substring(_getDir->Length)->Substring(1)->ToString())));					
				}			
				else
				{
					ImgsTest[countTest++] = subDirImgArray[i][j];//#Count images for test	
					LabelsTest.push_back(std::stoi(msclr::interop::marshal_as<std::string>(_getAllSubDir[i]->Substring(_getDir->Length)->Substring(1)->ToString())));
				}
			}
			
		//#Copy images
		//for (int j = 0; j < myImageFlags[i]; j++)						
			//ImgsTrain[k][j] = subDirImgArray[i][j];
		
		//#Create PictureBoxes-Labels-Buttons-Timers
		//-----------------------------------------------
		CreatePictureBoxes(205 + x, 90, 160, 150, i);//#Create PictureBoxes to display all Images
		CreateLabels(245 + x, 65, _getAllSubDir[i]->Substring(_getDir->Length)->ToString() + " - Images", i);//#Create Labels to display the name of each sub-directory
		CreatePlayButtons(265 + x, 247, i);//#Create PlayButtons to "play" each set of Images
		CreatePauseButtons(265 + x, 247, i);//#Create PauseButtons to "pause" each set of Images
		CreateTimers(i);//#Create Timers for each set of Images

		timerSlide[i] = 1;//#Start the SlidingTimer from the second image
		
		DrawImage(i, 0, subDirImgArray, pBoxes[i]);//#Draw the first Image of each set to PictureBoxes
		
		x += 165;//#Count the x Axis value
		}

		for (int i = 0; i < (int)(countImgs / takeBy); i++)
			ImgsResults[i] = ImgsTrain[i].clone();
			
	//#Enable items
	//---------------
	if (radioButton1->Checked)
	{
		groupBox3->Visible = true;
		groupBox3->Text = "Block's Parameters";		
		textBox1->Enabled = true;
		textBox2->Enabled = true;
		button2->Enabled = true;
		label2->Visible = true;
		textBox2->Visible = true;
	}

	//#Enable items
	//---------------
	if (radioButton2->Checked)
	{
		groupBox3->Visible = true;
		groupBox3->Text = "SIFT Parameters";
		textBox1->Enabled = true;
		textBox2->Enabled = true;
		button2->Enabled = true;
		label2->Visible = false;
		textBox2->Visible = false;
	}

	//#Disable items
	//----------------
	checkedListBox1->Enabled = false;
	button4->Enabled = false;
	comboBox1->Enabled = false;
	radioButton1->Enabled = false;
	radioButton2->Enabled = false;

	button12->Enabled = true;

}

	#pragma region (Dinamically - Methods) Used to Display results -> Images

#pragma region Dynamically (Create)Methods - PictureBox,Label,Button,Timer

//#Dynamically create PictureBoxes for each sub-directory
private: void CreatePictureBoxes(int xPoint, int yPoint, int width, int height, int count){
	
	PictureBox ^pBox = gcnew PictureBox();

	pBox->Parent = this; // 'this' is pointing to our form (the parent)
	pBox->Location = System::Drawing::Point(xPoint, yPoint);
	pBox->Size = System::Drawing::Size(width, height);
	pBox->BackColor = System::Drawing::Color(Color::White);
	pBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
	
	//pBox->Image = gcnew Bitmap(img.cols, img.rows, img.step, Imaging::PixelFormat::Format24bppRgb, IntPtr(img.data));
	//DrawImage(count, 0, subDirImgArray, pBox);
	pBoxes[count] = pBox;
	//DrawImage(count, 0, subDirImgArray, pBoxes[count]);
	
}

//#Dynamically create Labels for each sub-directory
private: void CreateLabels(int xPoint, int yPoint, System::String^ name, int count){

	Label ^pLab = gcnew Label();

	pLab->Parent = this; // 'this' is pointing to our form (the parent)
	pLab->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	pLab->AutoSize = true;
	pLab->Location = System::Drawing::Point(xPoint, yPoint);
	pLab->Name = L"label10";
	pLab->Size = System::Drawing::Size(23, 16);
	pLab->TabIndex = 25;
	pLab->Text = name;

	pLabels[count] = pLab;
}

//#Dynamically create PlayButtons for each sub-directory
private: void CreatePlayButtons(int xPoint, int yPoint, int count){

	Button ^pPlayBut = gcnew Button();
	System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));

	pPlayBut->Parent = this; // 'this' is pointing to our form (the parent)
	pPlayBut->BackColor = System::Drawing::Color::Transparent;
	pPlayBut->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PlayButton.BackgroundImage")));
	pPlayBut->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
	pPlayBut->Cursor = System::Windows::Forms::Cursors::Hand;
	pPlayBut->FlatAppearance->BorderSize = 0;
	pPlayBut->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	pPlayBut->Location = System::Drawing::Point(xPoint, yPoint);
	pPlayBut->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	pPlayBut->Name = L"PlayButton" + count.ToString();
	pPlayBut->Size = System::Drawing::Size(40, 40);
	pPlayBut->TabIndex = count;
	pPlayBut->UseVisualStyleBackColor = false;
	pPlayBut->Visible = true;
	this->toolTip1->SetToolTip(pPlayBut, L"Play");
	pPlayBut->Click += gcnew System::EventHandler(this, &MyForm::PlayButtons_Click);
	
	pPlayButtons[count] = pPlayBut;
}

//#Dynamically create PauseButtons for each sub-directory
private: void CreatePauseButtons(int xPoint, int yPoint, int count){

	Button ^pPauseBut = gcnew Button();
	System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));

	pPauseBut->Parent = this; // 'this' is pointing to our form (the parent)
	pPauseBut->BackColor = System::Drawing::Color::Transparent;
	pPauseBut->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PauseButton.BackgroundImage")));
	pPauseBut->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
	pPauseBut->Cursor = System::Windows::Forms::Cursors::Hand;
	pPauseBut->FlatAppearance->BorderSize = 0;
	pPauseBut->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
	pPauseBut->Location = System::Drawing::Point(xPoint, yPoint);
	pPauseBut->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	pPauseBut->Name = L"PauseButton" +count.ToString();
	pPauseBut->Size = System::Drawing::Size(40, 40);
	pPauseBut->TabIndex = 10 + count;
	pPauseBut->UseVisualStyleBackColor = false;
	pPauseBut->Visible = false;
	this->toolTip1->SetToolTip(pPauseBut, L"Pause");
	pPauseBut->Click += gcnew System::EventHandler(this, &MyForm::PauseButtons_Click);

	pPauseButtons[count] = pPauseBut;
}

//#Dynamically create Timers for each sub-directory
private: void CreateTimers(int count){

	Timer ^pTimer = gcnew Timer();

	pTimer->Interval = 500;
	pTimer->Tick += gcnew System::EventHandler(this, &MyForm::Timers_Tick);

	pTimers[count] = pTimer;
}

#pragma endregion

#pragma region Dynamically (OnClick - Event)Methods - Play/Pause Buttons, Timers

//#Dynamically (OnClick - Event)Method used to Play specific sub-directory images
private: System::Void PlayButtons_Click(System::Object^  sender, System::EventArgs^  e) {
		 
	//Button^ button = (Button^)sender;
	//IEnumerator ^but = button->Name->GetEnumerator();

	System::String^ buttonName = safe_cast<Button^>(sender)->Name;

	for (int i = 0; i < _getAllSubDir->Length; i++)
		if (checkedListBox1->GetItemCheckState(i) == CheckState::Checked)
		{
			if (Object::ReferenceEquals(pPlayButtons[i]->Name, buttonName))
			{
				pPlayButtons[i]->Visible = false;
				pPauseButtons[i]->Visible = true;

				pTimers[i]->Enabled = true;//Start the timer
			}
		}


	/*for (int i = 0; i < pPlayButtons->Length; i++)
	{
		//Object::ReferenceEquals(pPlayButtons[i]->Name, buttonName);
		if (Object::ReferenceEquals(pPlayButtons[i]->Name, buttonName))
		{

			pPlayButtons[i]->Visible = false;
			pPauseButtons[i]->Visible = true;

			pTimers[i]->Enabled = true;//Start the timer
			int k = 0;
		}
		
	}*/

}

//#Dynamically (OnClick - Event)Method used to Pause specific sub-directory images
private: System::Void PauseButtons_Click(System::Object^  sender, System::EventArgs^  e) {

	System::String^ buttonName = safe_cast<Button^>(sender)->Name;

	for (int i = 0; i < _getAllSubDir->Length; i++)
		if (checkedListBox1->GetItemCheckState(i) == CheckState::Checked)
		{
			if (Object::ReferenceEquals(pPauseButtons[i]->Name, buttonName))
			{
				pPauseButtons[i]->Visible = false;
				pPlayButtons[i]->Visible = true;

				pTimers[i]->Enabled = false;//Start the timer
			}
		}

}

//#Dynamically (OnClick - Event)Method used to Draw(in picBoxes, at specified Interval) specific sub-directory images
private: System::Void Timers_Tick(System::Object^  sender, System::EventArgs^  e) {

	Timer^ timer = (Timer^)sender;
			 
	for (int i = 0; i < _getAllSubDir->Length; i++)	
		if (checkedListBox1->GetItemCheckState(i) == CheckState::Checked)
		{
			if (Object::ReferenceEquals(pTimers[i], timer))
			{
				if (timerSlide[i] == myImageFlags[i])
				{
					//pTimers[i]->Enabled = false;
					timerSlide[i] = 0;			
				}							
					DrawImage(i, timerSlide[i], subDirImgArray, pBoxes[i]);
					timerSlide[i]++;
			}			
		}
							
}
		 
#pragma endregion

//#Draw specific images to PictureBox
private: void DrawImage(int x, int y, Mat **image, PictureBox^  sender){

	//cvtColor(image[x][y], image[x][y], CV_GRAY2RGB); 
	sender->Image = gcnew Bitmap(image[x][y].cols, image[x][y].rows, image[x][y].step, Imaging::PixelFormat::Format24bppRgb, IntPtr(image[x][y].data));

}

	#pragma endregion

//Selecting the method for Creating the Dictionary
private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (radioButton1->Checked && countImgs != 0 && countCheckedClasses >= 2)	button4->Enabled = true;
	else
	{
		button4->Enabled = false;
		radioButton1->Checked = false;
	}

	SiftOrBow = true;//#Set true if Block's Method

}

private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (radioButton2->Checked && countImgs != 0 && countCheckedClasses >= 2)	button4->Enabled = true;
	else
	{ 				
		button4->Enabled = false;
		radioButton2->Checked = false;
	}

	SiftOrBow = false;//#Set false if SIFT Method

}

//Create Dictionary Button
private: System::Void bCreateDictionary_Click(System::Object^  sender, System::EventArgs^  e){
	
	for (int i = 0; i < _getAllSubDir->Length; i++)
	if (checkedListBox1->GetItemCheckState(i) == CheckState::Checked)
	{
		pTimers[i]->Enabled = false;
		pPauseButtons[i]->Visible = false;
		pPlayButtons[i]->Visible = true;
	}
		
	auto start_time = std::chrono::high_resolution_clock::now();

	Dictionary objDictionary;
	//System::Int32^ buttonIndex = safe_cast<Button^>(sender)->TabIndex;//Get the sender object TabIndex

	switch (SiftOrBow)
	{
		//#(SIFT) Button - Create Dictionary 
		//------------------------------------
		case false:
		{
										   
			try
			{
				
				dictionarySize = System::Convert::ToInt32(textBox1->Text);

				for (int j = 0; j < (int)(countImgs / takeBy); j++)
				{

						//objDictionary.ExtractFeatures(ImgsTrain[j], _featuresUnclustered);
						//_featuresUnclustered.push_back(_descriptor); //Put the all feature descriptors in a single Mat object 
					
					detector.detect(ImgsTrain[j], keypoints); //Detect feature key-points
					drawKeypoints(ImgsTrain[j], keypoints, ImgsResults[j], Scalar::all(-1), DrawMatchesFlags::DEFAULT);
					extractor.compute(ImgsTrain[j], keypoints, descriptor); //Compute the descriptors for each key-point
					featuresUnclustered.push_back(descriptor); //Put the all feature descriptors in a single Mat object
				}

				kmeans(featuresUnclustered, dictionarySize, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 100, 0.0001), 1, flags, words);
											
				//#The Dictionary Name
				//------------------------	
				System::String^ ymlName = "DictionarySIFT.yml";//#The .yml name
				objDictionary.SaveFeaturesInYml(ymlName, words);						

				//Visualizations
				//button3->Enabled = false;
				//textBox4->Enabled = false;
				//textBox5->Enabled = false;
				//checkBox2->Visible = true;
				radioButton1->Enabled = false;
				radioButton2->Enabled = false;
				checkBox1->Visible = true;	
				button13->Enabled = true;
				button12->Enabled = false;
				textBox1->Enabled = false;
				button2->Enabled = false;
				label15->Visible = true;
				label16->Visible = true;

			}

			catch (System::Exception ^ex)
			{
				MessageBox::Show(ex->Message->ToString(), "Error!", MessageBoxButtons::OK);
			}

		}
			break;
		//#(Blocks) Button - Create Dictionary 
		//---------------------------------------
		case true:
		{

			try
			{

				blockSize = System::Convert::ToInt32(textBox2->Text);
				dictionarySize = System::Convert::ToInt32(textBox1->Text);
				cv::Size _Size(blockSize, blockSize);//The block size					

				//#Define the dimension of Samples Mat
				//-----------------------------------------
				nrSamples = (ImgsTrain[0].rows / _Size.height) * (ImgsTrain[0].cols / _Size.width) * (countImgs / takeBy);//#Count the number of samples per all selected sub-directories
				Mat _samples(nrSamples, _Size.height * _Size.width, CV_32F);//#Initialize the samples Mat for selected sub-directories

				for (int j = 0; j < (int) (countImgs / takeBy); j++)
				{
					objDictionary.ExtractFeatures(ImgsTrain[j], _Size, _samples);
				}

					/*for (int x = 0; x < (ImgsTrain[j].rows - _Size.height); x += _Size.height)
						   for (int y = 0; y < (ImgsTrain[j].cols - _Size.width); y += _Size.width)
						   {

						   _imageBlocks = ImgsTrain[j](cv::Rect(y, x, _Size.width, _Size.height)).clone();

						   for (int z = 0; z < _Size.height; z++)
						   for (int w = 0; w < _Size.width; w++)
						   _samples.at<float>(_count_samples, z *_Size.width + w) = _imageBlocks.at<uchar>(z, w);

						   _count_samples++;
						   }

						   //_count_samples = 0;*/

				//objDictionary.ClusterFeatures(_samples, _labels, _dictionarySize, _words);
				kmeans(_samples, dictionarySize, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 100, 0.0001), 1, flags, words);

				//#The Dictionary Name
				//------------------------
				System::String^ ymlName = "DictionaryBlocks.yml";//#The .yml name
				objDictionary.SaveFeaturesInYml(ymlName, words);					
				
				//Enable/Disable items
				//------------------------
				button2->Enabled = false;
				textBox1->Enabled = false;
				textBox2->Enabled = false;
				checkBox1->Visible = true;
				radioButton2->Enabled = false;
				radioButton1->Enabled = true;
				button13->Enabled = true;
				button12->Enabled = false;
				label15->Visible = true;
				label16->Visible = true;

			}

			catch (System::Exception ^ex)
			{
				MessageBox::Show(ex->Message->ToString(), "Error!", MessageBoxButtons::OK);
			}

		}
			break;
	}

	//Timer implementation
	double d;
	auto current_time = std::chrono::high_resolution_clock::now();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
	
	if (millis >= 1000)
	{
		d = (double)millis / 1000;
		label15->Text = d.ToString() + " s";
	}
	else label15->Text = millis.ToString() + " ms";

}

	#pragma endregion

//***** Done *****
	#pragma region See Results (After Creating Dictionary)

//#Draw images to PictureBox
private: void DrawImage(int x, Mat *image, PictureBox^  sender){

	//cvtColor(image[x][y], image[x][y], CV_GRAY2RGB); 
	sender->Image = gcnew Bitmap(image[x].cols, image[x].rows, image[x].step, Imaging::PixelFormat::Format24bppRgb, IntPtr(image[x].data));

}

private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (SiftOrBow == false)
	{
		DrawImage(0, ImgsResults, pictureBox4);
	}
	else
	{
		cv::Size _Size(blockSize, blockSize);
		DrawBlocks(_Size, ImgsResults);
		DrawImage(0, ImgsResults, pictureBox4);
	}

}

private: System::Void checkBox1_CheckStateChanged(System::Object^  sender, System::EventArgs^  e) {

	if (checkBox1->Checked)
	{
		pictureBox4->Visible = true;
		PlayButton->Visible = true;
	}
	else
	{
		pictureBox4->Visible = false;
		PlayButton->Visible = false;
		PauseButton->Visible = false;
	}

}

//Play Slides Button
private: System::Void PlayButton_Click(System::Object^  sender, System::EventArgs^  e) {

	PlayButton->Visible = false;
	PauseButton->Visible = true;

	timer1->Enabled = true;//#Start the timer

}

//Pause Slides Button
private: System::Void PauseButton_Click(System::Object^  sender, System::EventArgs^  e) {

	timer1->Enabled = false;//#Stop the timer

	PlayButton->Visible = true;
	PauseButton->Visible = false;

}

//Timer - Used to display the result images
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {

	if (resultsTimerSliding == (int)(countImgs / takeBy)) resultsTimerSliding = 0;

	switch (SiftOrBow)
	{

		case true:

			DrawImage(resultsTimerSliding, ImgsResults, pictureBox4);
			
			break;

		case false:

			DrawImage(resultsTimerSliding, ImgsResults, pictureBox4);
			
			break;

	}

	resultsTimerSliding++;

}

//#Drawing image blocks method
private: void DrawBlocks(cv::Size size, Mat *img)
{
	
	for (int i = 0; i < (int)(countImgs / takeBy); i++)
	{
		for (int x = 0; x < img[i].rows; x += size.height)
		for (int y = 0; y < img[i].cols; y += size.width)
		{
			cv::rectangle(img[i], cv::Point(y, x), cv::Point(size.width, size.height), cv::Scalar(0, 0, 0), 1);
		}
	}

}

	#pragma endregion

//***** Done *****
	#pragma region Histogram Representation

//#Loading all Test Images to ListBox
private: void LoadTrainImages(bool trainOrTest){

	int add = 0;

	if (trainOrTest == true)
	{
		for (int i = 0; i < _getAllSubDir->Length; i++)
		{
			if (checkedListBox1->GetItemCheckState(i) == CheckState::Checked)
			{
				_getDirFiles = Directory::GetFiles(_getAllSubDir[i], "*.jpg");//The .jpg file names	
				for (int j = 0; j < _getDirFiles->Length; j++)
				if (j % takeBy == 0)	PopulateListBox(checkedListBox2, add++, _getDirFiles, j);//(modified) j % takeBy != 0
			}
		}
	}
	else
	{
		for (int i = 0; i < _getAllSubDir->Length; i++)
		{
			if (checkedListBox1->GetItemCheckState(i) == CheckState::Checked)
			{
				_getDirFiles = Directory::GetFiles(_getAllSubDir[i], "*.jpg");//The .jpg file names	
				for (int j = 0; j < _getDirFiles->Length; j++)
				if (j % takeBy != 0)	PopulateListBox(checkedListBox2, add++, _getDirFiles, j);//(modified) j % takeBy != 0
			}
		}
	}
}

private: void AllocateHists(bool TrOrTst)
{

	if (TrOrTst == false)
	{
		wordsHistTest = new double*[(int)(countImgs - (int)(countImgs / takeBy))];
		for (int i = 0; i < (int)(countImgs - (int)(countImgs / takeBy)); i++)
		{
			wordsHistTest[i] = new double[words.rows];
			for (int j = 0; j < words.rows; j++)  wordsHistTest[i][j] = 0;
		}
	}
	else
	{
		wordsHistTrain = new double*[(int)(countImgs / takeBy)];
		for (int i = 0; i < (int)(countImgs / takeBy); i++)
		{
			wordsHistTrain[i] = new double[words.rows];
			for (int j = 0; j < words.rows; j++)  wordsHistTrain[i][j] = 0;
		}
	}

}

//#Euclidean Distance
private: void L2Norm(Mat samples, Mat words, double *hist){

	for (int i = 0; i < samples.rows; i++)
	{
		minimumDistanceEuclidean = 999999;
		for (int k = 0; k < words.rows; k++)
		{
			distanceEuclidean[k] = norm(samples.row(i), words.row(k), NORM_L2);
			if (distanceEuclidean[k] < minimumDistanceEuclidean)
			{
				minimumDistanceEuclidean = distanceEuclidean[k];
				position = k;
			}
		}
		hist[position] ++;
	}
}

//#Display the histogram to Chart
private: void ShowHistogram(double *hist){

	//Show histogram in chart			 					 
	chart1->Series["Words occurrence"]->Points->Clear();
	for (int i = 0; i < words.rows; i++)
		chart1->Series["Words occurrence"]->Points->AddXY(i, hist[i]);
	chart1->Visible = true;
				 	
}

//#(Button) - Create Histograms
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
	
	for (int i = 0; i < _getAllSubDir->Length; i++)
	if (checkedListBox1->GetItemCheckState(i) == CheckState::Checked)
	{
		pTimers[i]->Enabled = false;
		pPauseButtons[i]->Visible = false;
		pPlayButtons[i]->Visible = true;
	}

	LoadTrainImages(TrainOrTest);

	AllocateHists(TrainOrTest);

	distanceEuclidean = new int[dictionarySize];//Initialize the (int*)_dist

	CreateHistogram(TrainOrTest);

	//#Enable/Disable items
	//-----------------------
	button1->Visible = false;
	checkedListBox2->Enabled = true;
	button3->Visible = true;
	button13->Enabled = true;
	button14->Enabled = true;
	radioButton3->Enabled = false;
	radioButton4->Enabled = false;
}

//#Allow us to check only 1 item from ListBox
private: System::Void checkedListBox2_ItemCheck(System::Object^  sender, System::Windows::Forms::ItemCheckEventArgs^  e) {

	for (int i = 0; i < checkedListBox2->Items->Count; i++)
	{
		if (i != e->Index)
		{
			checkedListBox2->SetItemChecked(i, false);
			//button3->Enabled = true;
		}
		else showSpecificHist = i;			
	}

	if (checkedListBox2->GetItemCheckState(e->Index) != CheckState::Checked) button3->Enabled = true;
	else button3->Enabled = false;

}

//#(Button) - Show Histogram
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {

	for (int i = 0; i < _getAllSubDir->Length; i++)
	if (checkedListBox1->GetItemCheckState(i) == CheckState::Checked)
	{
		pTimers[i]->Enabled = false;
		pPauseButtons[i]->Visible = false;
		pPlayButtons[i]->Visible = true;
	}

	if (TrainOrTest == false)
	{
		ShowHistogram(wordsHistTest[showSpecificHist]);
		button3->Enabled = false;
	}
	else
	{
		ShowHistogram(wordsHistTrain[showSpecificHist]);
		button3->Enabled = false;
	}
	
}

//#Train Images
private: System::Void radioButton3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (radioButton3->Checked)	button1->Enabled = true;
	else
	{
		button1->Enabled = false;
		radioButton3->Checked = false;
	}

	TrainOrTest = true;//#Set true if Train Images

}

//#Test Images
private: System::Void radioButton4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

	if (radioButton4->Checked)	button1->Enabled = true;
	else
	{
		button1->Enabled = false;
		radioButton4->Checked = false;
	}

	TrainOrTest = false;//#Set false if Test Images

}

private: void CreateHistogram(bool TrOrTst)
{

	Dictionary objDictionary;

	switch (SiftOrBow)
	{

		case true:
		{
			cv::Size _Size(blockSize, blockSize);//The block size	

			if (TrOrTst == false)
			{

				//#Define the dimension of Samples Mat
							  //-----------------------------------------
				nrSamples = (ImgsTest[0].rows / _Size.height) * (ImgsTest[0].cols / _Size.width) * ((int)(countImgs - (int)(countImgs / takeBy)));//#Count the number of samples per all selected sub-directories
				Mat _samples2(nrSamples, _Size.height * _Size.width, CV_32F);//#Initialize the samples Mat for selected sub-directories

				_count_samples = 0;
				for (int j = 0; j < (int)(countImgs - (int)(countImgs / takeBy)); j++)
				{
					objDictionary.ExtractFeatures(ImgsTest[j], _Size, _samples2);
					L2Norm(_samples2, words, wordsHistTest[j]);
				}
				//Normalize hist
				double norm;
				for (int i = 0; i < (int)(countImgs - (int)(countImgs / takeBy)); i++)
				{
					for (int j = 0; j < words.rows; j++)
					{
						norm = (double)(wordsHistTest[i][j] / nrSamples);
						wordsHistTest[i][j] = norm;
					}
				}
			}
			else
			{
				//#Define the dimension of Samples Mat
							  //-----------------------------------------
				nrSamples = (ImgsTrain[0].rows / _Size.height) * (ImgsTrain[0].cols / _Size.width) * ((int)(countImgs / takeBy));//#Count the number of samples per all selected sub-directories
				Mat _samples2(nrSamples, _Size.height * _Size.width, CV_32F);//#Initialize the samples Mat for selected sub-directories

				_count_samples = 0;
				for (int j = 0; j < (int)(countImgs / takeBy); j++)
				{
					objDictionary.ExtractFeatures(ImgsTrain[j], _Size, _samples2);
					L2Norm(_samples2, words, wordsHistTrain[j]);
				}
				//Normalize hist
				double norm;
				for (int i = 0; i < ((int)(countImgs / takeBy)); i++)
				{
					for (int j = 0; j < words.rows; j++)
					{
						norm = (double)(wordsHistTrain[i][j] / nrSamples);
						wordsHistTrain[i][j] = norm;
					}
				}
			}
		}
		break;

		case false:
		{
			if (TrOrTst == false)
			{
				for (int j = 0; j < (int)(countImgs - (int)(countImgs / takeBy)); j++)
				{
					//objDictionary.ExtractFeatures(ImgsTrain[j], _featuresUnclustered);
								   //_featuresUnclustered.push_back(_descriptor); 

					detector.detect(ImgsTest[j], keypoints);
					extractor.compute(ImgsTest[j], keypoints, descriptor);
					featuresUnclustered.push_back(descriptor);
					L2Norm(featuresUnclustered, words, wordsHistTest[j]);
				}
			}
			else
			{
				for (int j = 0; j < (int)(countImgs / takeBy); j++)
				{
					//objDictionary.ExtractFeatures(ImgsTrain[j], _featuresUnclustered);
								   //_featuresUnclustered.push_back(_descriptor); 

					detector.detect(ImgsTrain[j], keypoints);
					extractor.compute(ImgsTrain[j], keypoints, descriptor);
					featuresUnclustered.push_back(descriptor);
					L2Norm(featuresUnclustered, words, wordsHistTrain[j]);
					//Normalize hist not working
					//double norm;
					//double rnd;
					//for (int i = 0; i < ((int)(countImgs / takeBy)); i++)
					//{
					//	for (int j = 0; j < words.rows; j++)
					//	{
					//		
					//		norm = (double)(wordsHistTrain[i][j] / nrSamples);
					//		rnd = round(norm * 1000000000.0) / 1000000000.0;
					//		wordsHistTrain[i][j] = rnd;
					//		norm = 0;
					//	}
					//}
				}
			}
		}
		break;

	}

}

	#pragma endregion

//***** Done *****
	#pragma region Scene Classification

private: void Summary()
{

	array<System::String^>^ dirs = gcnew array<System::String^>(countDirs);
	System::String^ joins = "";
	System::String^ method;

	if (SiftOrBow == true) method = radioButton1->Text;
	else method = radioButton2->Text;

	int k = 0;
	for (int i = 0; i < _getAllSubDir->Length; i++)
	{
		if (checkedListBox1->GetItemCheckState(i) == CheckState::Checked)
		{
			dirs[k] = (_getAllSubDir[i]->Substring(_getDir->Length))->ToString();
			k++;
		}
	}

	joins = System::String::Join(",", dirs);//Put "," between directors

	dataGridView1->Rows->Add(nrGrid.ToString(), joins, countImgs.ToString(), ((int)(countImgs / takeBy)).ToString(), ((int)(countImgs - (int)(countImgs / takeBy))).ToString(), method, label15->Text, label20->Text);

}

private: System::Void checkBox2_CheckStateChanged(System::Object^  sender, System::EventArgs^  e) {

	if (checkBox2->Checked) dataGridView1->Visible = true;
	else dataGridView1->Visible = false;

}

private: System::Void button15_Click(System::Object^  sender, System::EventArgs^  e) {

	Mat sampleMat(1, words.rows, CV_32FC1);

	for (int i = 0; i < (int)(countImgs - (int)(countImgs / takeBy)); i++)
	{
		for (int j = 0; j < words.rows; j++)
		{
			sampleMat.at<float>(0, j) = wordsHistTest[i][j];
		}
		SVMclassif(sampleMat, i, OneImage);
	}

	double a = (double)(corect * 100) / (int)(countImgs - (int)(countImgs / takeBy));
	System::String^ procent = a.ToString();

	if (procent->Length > 5) label20->Text = procent->Remove(5) + " %";
	else label20->Text = procent + " %";

	button15->Enabled = false;
	checkBox2->Visible = true;
	button11->Enabled = true;
	button6->Enabled = false;//Reset not working
	nrGrid++;

	Summary();

}

private: System::Void SVMclassif(Mat toClassify, int poz, bool image) {
	
	// Set up SVM's parameters
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	
	// Train the SVM
	CvSVM SVM;
	SVM.train(trainingDataMat, labelsMat, Mat(), Mat(), params);

	float response = 0;		
	//Result
	response = SVM.predict(toClassify);

	if (image)
	{
		//System::String^ test = Enum::GetName(Result::typeid, 3);
		raspuns = Enum::GetName(Result::typeid, (Convert::ToInt32(response)));
	}
	else
	{
		if (LabelsTest[poz] == response)	corect++;
	}
	
}

	#pragma endregion


//#Draw a single image to pictureBox
private: void DrawImage(Mat image, PictureBox^  sender){

	//cvtColor(image, _copyImage, CV_GRAY2RGB);
	sender->Image = gcnew Bitmap(image.cols, image.rows, image.step, Imaging::PixelFormat::Format24bppRgb, IntPtr(image.data));

}

private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {

	//Allocate hist 
	wordsHistImage = new double*[1];			 
	wordsHistImage[1] = new double[words.rows];
	for (int j = 0; j < words.rows; j++)  wordsHistImage[1][j] = 0;
			 

	if (SiftOrBow)
	{
		Dictionary objDictionary;
		cv::Size _Size(blockSize, blockSize);//The block size

		nrSamples = ((openImage.rows / _Size.height) * (openImage.cols / _Size.width) * 1);//#Count the number of samples 
		Mat _samples2(nrSamples, _Size.height * _Size.width, CV_32F);//#Initialize the samples Mat 

		_count_samples = 0;
		
		objDictionary.ExtractFeatures(openImage, _Size, _samples2);
		L2Norm(_samples2, words, wordsHistImage[1]);

		//Normalize hist
		double norm;	
		for (int j = 0; j < words.rows; j++)
		{
			norm = (double)(wordsHistTrain[1][j] / nrSamples);
			wordsHistTrain[1][j] = norm;
		}
		
		//Classify
		Mat sampleMat(1, words.rows, CV_32FC1);

		for (int j = 0; j < words.rows; j++)
		{
			sampleMat.at<float>(0, j) = wordsHistImage[1][j];
		}
		SVMclassif(sampleMat, 1, OneImage);
		label22->Text = raspuns;
	}
	else
	{
		detector.detect(openImage, keypoints);
		extractor.compute(openImage, keypoints, descriptor);
		featuresUnclustered.push_back(descriptor);
		L2Norm(featuresUnclustered, words, wordsHistImage[1]);
		
		//Classify
		Mat sampleMat(1, words.rows, CV_32FC1);

		for (int j = 0; j < words.rows; j++)
		{
			sampleMat.at<float>(0, j) = wordsHistImage[1][j];
		}
		SVMclassif(sampleMat, 1, OneImage);

		label22->Text = raspuns;
	}

	nrGrid++;
	button5->Enabled = false;
}

//***** Done *****
	#pragma region (MenuStrip) - Methods

//#(MenuStrip) - Create Dictionary
private: System::Void button12_Click(System::Object^  sender, System::EventArgs^  e) {

	//#If PostBack!
	//-------------
	if (checkIfPostBack >= 1)
	{
		for (int i = 0; i < _getAllSubDir->Length; i++)
		{
			//#Delete items 
			//--------------					
			delete pBoxes[i];
			delete pLabels[i];
			delete pPlayButtons[i];
			delete pPauseButtons[i];
			delete pTimers[i];

			LabelsTrain.clear();
			LabelsTest.clear();

			for (int w = 0; w < (int)(countImgs / takeBy); w++)
			{
				ImgsTrain[w].deallocate();
				ImgsResults[w].deallocate();
			}

			for (int k = 0; k < (int)(countImgs - (int)(countImgs / takeBy)); k++)
				ImgsTest[k].deallocate();

			//for (int i = 0; i < (int)(countImgs - (int)(countImgs / takeBy)); i++)			
			//for (int j = 0; j < _words.rows; j++)
			delete wordsHistTrain;

			//delete[i] _timerSlide;

			checkedListBox1->Items->Clear();
			//#Reset items
					 //-------------
			countDirs = 0;
			countImgs = 0;
			countCheckedClasses = 0;
			SetLabelValue(label9, countImgs);
		}
	}

	groupBox2->Visible = true;
	checkedListBox1->Enabled = true;
	comboBox1->Enabled = true;
	groupBox3->Visible = false;
	groupBox1->Visible = false;
	radioButton1->Checked = false;
	radioButton2->Checked = false;
	radioButton1->Enabled = true;
	radioButton2->Enabled = true;
	button12->Enabled = false;

	LoadAllSubDirImages();//#Load Images from all sub-directories
	checkIfPostBack++;//#Counting if pressed

}

//#(MenuStrip) - Histogram Representation
private: System::Void button13_Click(System::Object^  sender, System::EventArgs^  e) {

	//#If PostBack!
	//-------------
	if (checkIfPostBack1 >= 1)
	{
		for (int i = 0; i < _getAllSubDir->Length; i++)
		if (checkedListBox1->GetItemCheckState(i) == CheckState::Checked)
		{
			pTimers[i]->Enabled = false;
			pPauseButtons[i]->Visible = false;
			pPlayButtons[i]->Visible = true;
		}

		if (TrainOrTest == false)
		{
			delete wordsHistTest;
		}
		else
		{
			delete wordsHistTrain;			
		}
			
		checkedListBox2->Items->Clear();
		delete distanceEuclidean;
		
		button3->Visible = false;
		button1->Visible = true;
		chart1->Visible = false;
	}

	button1->Enabled = false;
	button13->Enabled = false;
	groupBox1->Visible = true;
	groupBox2->Visible = false;
	groupBox3->Visible = false;
	checkedListBox2->Enabled = false;
	radioButton3->Enabled = true;
	radioButton4->Enabled = true;
	radioButton3->Checked = false;
	radioButton4->Checked = false;
	checkBox1->Checked = false;

	checkIfPostBack1++;//#Counting if pressed
	
}

//#(MenuStrip) - Scene Classification
private: System::Void button14_Click(System::Object^  sender, System::EventArgs^  e) {

	AllocateHists(!TrainOrTest);
	CreateHistogram(!TrainOrTest);//Create the other set of histograms

	//#Allocate the Training/Label Mat
	trainingDataMat = Mat((int)(countImgs / takeBy), words.rows, CV_32FC1);
	labelsMat = Mat((int)(countImgs / takeBy), 1, CV_32FC1);

	for (int i = 0; i < (int)(countImgs / takeBy); i++)
	{
		for (int j = 0; j < words.rows; j++)
		{
			trainingDataMat.at<float>(i, j) = wordsHistTrain[i][j];
		}
	}

	for (int i = 0; i < (int)(countImgs / takeBy); i++)
	{
		labelsMat.at<float>(i, 0) = LabelsTrain[i];
	}

	button13->Enabled = false;
	groupBox1->Visible = false;
	groupBox4->Visible = true;
	chart1->Visible = false;
	button15->Visible = true;
	button14->Enabled = false;
	label18->Text = ((int)(countImgs - (int)(countImgs / takeBy))).ToString();
	OneImage = false;
}

//#(MenuStrip) - Open Image
private: System::Void button11_Click(System::Object^  sender, System::EventArgs^  e) {

	this->openFileDialog1->InitialDirectory = _getDir;//#OpenFileDialog will open at specified directory

	if (this->openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		openImage = imread(msclr::interop::marshal_as< std::string >(openFileDialog1->FileName), CV_LOAD_IMAGE_GRAYSCALE);
		cvtColor(openImage, openImage, CV_GRAY2RGB);
		DrawImage(openImage, pictureBox1);
		pictureBox1->Visible = true;
		//groupBox1->Visible = true;
		//labelForNewImage =
	}

	button5->Enabled = true;
	button5->Visible = true;
	groupBox5->Visible = true;
	OneImage = true;
	label22->Text = "";

}

//#(MenuStrip) - Open Multiple Images

	#pragma endregion 

//***** Done *****
	#pragma region Interface Design

//Form Movement Methods
private: System::Void MouseIsDown(System::Object^ sender, MouseEventArgs^ e){
	
	mouseDown = true;
	lastLocation = e->Location;

}

private: System::Void MouseIsMove(System::Object^ sender, MouseEventArgs^ e){
			 
	if (mouseDown)
	{
		this->System::Windows::Forms::Form::Location = System::Drawing::Point((this->Location.X - lastLocation.X) + e->X, (this->Location.Y - lastLocation.Y) + e->Y);
		this->Update();
	}

}

private: System::Void MouseIsUp(System::Object^ sender, MouseEventArgs^ e){
			 
	mouseDown = false;
		 
}

//#Exit application
private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {

	Application::Exit();

}

//#Maximize application
private: System::Void button9_Click(System::Object^  sender, System::EventArgs^  e) {

	if (maximizeCheck == false)	
	{
		this->WindowState = FormWindowState::Maximized;
		maximizeCheck = true;
	}
	else 
	{
		this->WindowState = FormWindowState::Normal;
		maximizeCheck = false;
	}
}

//#Minimize application
private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) {

	this->WindowState = FormWindowState::Minimized;

}

//#Check if numbers in textBoxes
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {

	System::String^ tString = textBox1->Text;
	ErrorMessage(tString, textBox1);
	
}

private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {

	System::String^ tString = textBox2->Text;
	ErrorMessage(tString, textBox2);

}

private: System::Void textBox3_TextChanged(System::Object^  sender, System::EventArgs^  e) {

	System::String^ tString = textBox3->Text;
	ErrorMessage(tString, textBox3);

}

private: System::Void ErrorMessage(System::String^ string, TextBox^ tb)
{

	if (string->Trim() == "") return;
	for (int i = 0; i < string->Length; i++)
	{
		if (!System::Char::IsNumber(string[i]))
		{
			std::string test1 = "Please enter a valid number";
			MessageBoxA(NULL, test1.c_str(), "Error", MB_OK);
			tb->Text = "";
			return;
		}
	}

}

#pragma endregion

//Reset all items!
private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {

	//#Delete items 
			 //--------------					
	for (int i = 0; i < _getAllSubDir->Length; i++)
	{
		delete pBoxes[i];
		delete pLabels[i];
		delete pPlayButtons[i];
		delete pPauseButtons[i];
		delete pTimers[i];
		delete _getAllSubDir[i];
	}

	for (int i = 0; i < _getDirFiles->Length; i++)
	{
		delete _getDirFiles[i];
	}

	LabelsTrain.clear();
	LabelsTest.clear();
	keypoints.clear();

	for (int w = 0; w < (int)(countImgs / takeBy); w++)
	{
		ImgsTrain[w].deallocate();
		ImgsResults[w].deallocate();
	}

	for (int k = 0; k < (int)(countImgs - (int)(countImgs / takeBy)); k++)
		ImgsTest[k].deallocate();

	delete wordsHistTrain;
	delete wordsHistTest;
	delete wordsHistImage;
	delete distanceEuclidean;
	delete myImageFlags;
	delete timerSlide;

	words.deallocate();
	trainingDataMat.deallocate();
	labelsMat.deallocate();
	labels.deallocate();

	if (!SiftOrBow)
	{
		descriptor.deallocate();
		featuresUnclustered.deallocate();
	}

	if (OneImage) openImage.deallocate();

	checkedListBox1->Items->Clear();
	checkedListBox2->Items->Clear();
	
	dictionarySize = 0;
	blockSize = 0;
	nrSamples = 0;
	totalImages = 0;
	flags = KMEANS_PP_CENTERS;
	showSpecificHist = 0;
	position = 0;
	minimumDistanceEuclidean = 0;
	resultsTimerSliding = 0;
	corect = 0;
	takeBy = 0;
	countDirs = 0;
	countImgs = 0;
	countCheckedClasses = 0;
	checkIfPostBack = 0;
	checkIfPostBack1 = 0;

	groupBox4->Visible = false;
	groupBox5->Visible = false;
	pictureBox1->Visible = false;
	checkBox2->Checked = false;
	button11->Enabled = false;
	button10->Enabled = true;
	button6->Enabled = false;
	textBox3->Enabled = true;
	label15->Visible = false;
	label16->Visible = false;
	checkBox1->Visible = false;
}

};
}