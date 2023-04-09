#pragma once
#define AI_Switch 1

#include <cmath>
#include <fstream>
#include<iostream>
#include <windows.h>
#include "Field.h"
#include "AI.h"
#include "FigureMove.h"

bool precom_flag;

//Класс окна интерфейса
//тут только интерфейс, правильность ходов и ИИ в других классах

namespace Project2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		//Размер одной клетки поля на форме
		//применяется для отрисовки и вычисления координат клика
		const int fieldSize = 70;
		MyForm(void)
		{
			InitializeComponent();
			bmp = (gcnew System::Drawing::Bitmap(1000, 1000));

			//Создание игрового поля
			field = new Field();
			ai = new AI();
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::PictureBox^ pictureBox2;


	private: System::Drawing::Bitmap^ bmp;



		   //Игровое поле для шахмат
	private: Field* field;
	

	private: AI* ai;

	private:int blackWins = 0;
	private:int whiteWins = 0;

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;


	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox2
			// 
			this->pictureBox2->Dock = System::Windows::Forms::DockStyle::Left;
			this->pictureBox2->Location = System::Drawing::Point(0, 0);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(721, 743);
			this->pictureBox2->TabIndex = 1;
			this->pictureBox2->TabStop = false;
			this->pictureBox2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::pictureBox2_Paint);
			this->pictureBox2->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBox2_MouseDown);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(741, 50);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(70, 26);
			this->label1->TabIndex = 4;
			this->label1->Text = L"label1";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(746, 94);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 37);
			this->button1->TabIndex = 5;
			this->button1->Text = L"Начать сначала";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click_1);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(746, 161);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(35, 13);
			this->label2->TabIndex = 6;
			this->label2->Text = L"label2";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(746, 178);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(35, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"label3";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1149, 743);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox2);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			pictureBox2->Invalidate();
		}
	private: System::Void pictureBox2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) 
		{
			//отметка побед белых и черных
			label2->Text = "Пoбед белых: " + whiteWins.ToString();
			label3->Text = "Пoбед черных: " + blackWins.ToString();
			//Кисть для рисования текущей клетки
			//начать рисовать надо с черного
			//	знак ^ - это особый вид указателя - управляемый указатель.
			Brush^ b = Brushes::Black;
			//Кисть для выделения
			Brush^ SelectedBrush = gcnew SolidBrush(Color::FromArgb(100, 255, 0, 0));
			//Кисть для выделения ходящей фигуры 
			Brush^ SelectedFigureBrush = gcnew SolidBrush(Color::FromArgb(170, 0, 255, 0));
			//Перебираю все строки
			for (int i = 0; i < 8; i++)
			{
				//каждую строку начинаю с переключения цвета
				if (b == Brushes::Black)
				{
					b = Brushes::White;
				}
				else
				{
					b = Brushes::Black;
				}
				//перебираю все столбцы в с троке
				for (int j = 0; j < 8; j++)
				{
					//каждый новый столбец меняю цвет, чтобы получилось чередование черного и белого
					if (b == Brushes::Black)
					{
						b = Brushes::White;
					}
					else
					{
						b = Brushes::Black;
					}
					//рисую клетку поля
					e->Graphics->FillRectangle(b, j * fieldSize, i * fieldSize, fieldSize, fieldSize);

					//РИсую выделение, если оно есть
					if (field->Moves[i][j])
					{
						e->Graphics->FillRectangle(SelectedBrush, j * fieldSize, i * fieldSize, fieldSize, fieldSize);
					}
					//РИсую выделение фигуры, если оно есть
					if (field->Figures[i][j] == field->SelectedFigure && field->SelectedFigure != nullptr)
					{
						e->Graphics->FillRectangle(SelectedFigureBrush, j * fieldSize, i * fieldSize, fieldSize, fieldSize);
					}

					//если на этом поле есть фигура, то рисую фигуру
					int sof = sizeof(Figure);
					Figure* figure = field->Figures[i][j];
					if (figure != 0)
					{
						if (figure->state< 0)
						{
							if (figure->FigureType == FigureTypes::pawn)
							{
								Bitmap^ image1 = gcnew Bitmap("img/белая пешка.png");
								//Нарисовать пешку
								e->Graphics->DrawImage(image1, j * fieldSize, i * fieldSize);
							}
							if (figure->FigureType == FigureTypes::rock)
							{
								Bitmap^ image1 = gcnew Bitmap("img/белая ладья.png");
								e->Graphics->DrawImage(image1, j * fieldSize, i * fieldSize);
							}
							if (figure->FigureType == FigureTypes::queen)
							{
								Bitmap^ image1 = gcnew Bitmap("img/белый ферзь.png");
								e->Graphics->DrawImage(image1, j * fieldSize, i * fieldSize);
							}
							if (figure->FigureType == FigureTypes::knight)
							{
								Bitmap^ image1 = gcnew Bitmap("img/белый конь.png");
								e->Graphics->DrawImage(image1, j * fieldSize, i * fieldSize);
							}
							if (figure->FigureType == FigureTypes::king)
							{
								Bitmap^ image1 = gcnew Bitmap("img/белый король.png");
								e->Graphics->DrawImage(image1, j * fieldSize, i * fieldSize);
							}
							if (figure->FigureType == FigureTypes::bishop)
							{
								Bitmap^ image1 = gcnew Bitmap("img/белый слон.png");
								e->Graphics->DrawImage(image1, j * fieldSize, i * fieldSize);
							}
						}
						else
						{
							if (figure->FigureType == FigureTypes::pawn)
							{
								Bitmap^ image1 = gcnew Bitmap("img/черная пешка.png");
								//Нарисовать пешку
								e->Graphics->DrawImage(image1, j * fieldSize, i * fieldSize);
							}
							if (figure->FigureType == FigureTypes::rock)
							{
								Bitmap^ image1 = gcnew Bitmap("img/черная ладья.png");
								e->Graphics->DrawImage(image1, j * fieldSize, i * fieldSize);
							}
							if (figure->FigureType == FigureTypes::queen)
							{
								Bitmap^ image1 = gcnew Bitmap("img/черный ферзь.png");
								e->Graphics->DrawImage(image1, j * fieldSize, i * fieldSize);
							}
							if (figure->FigureType == FigureTypes::knight)
							{
								Bitmap^ image1 = gcnew Bitmap("img/черный конь.png");
								e->Graphics->DrawImage(image1, j * fieldSize, i * fieldSize);
							}
							if (figure->FigureType == FigureTypes::king)
							{
								Bitmap^ image1 = gcnew Bitmap("img/черный король.png");
								e->Graphics->DrawImage(image1, j * fieldSize, i * fieldSize);
							}
							if (figure->FigureType == FigureTypes::bishop)
							{
								Bitmap^ image1 = gcnew Bitmap("img/черный слон.png");
								e->Graphics->DrawImage(image1, j * fieldSize, i * fieldSize);
							}
						}
					}
				}
			}
			//показать состояние игры в label1
			if (field->Checkmate)
			{
				if (field->CurrentMoveColor == -1)
				{
					label1->Text = "мат белым";
				}
				else
				{
					label1->Text = "мат черным";
				}
			}
			else if (field->Check)
			{
				if (field->CurrentMoveColor == -1)
				{
					label1->Text = "шах белым";
				}
				else
				{
					label1->Text = "шах черным";
				}
			}
			else
			{
				if (field->CurrentMoveColor ==-1)
				{
					label1->Text = "ходят белые";
				}
				else
				{
					label1->Text = "ходят черные";
				}
			}
		}

		private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e)
		{
			pictureBox2->Invalidate();
		}

		//Событие нажатия мыши
		//Координаты клика в параметре e
		private: System::Void pictureBox2_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
		{
			int x = e->X;
			int y = e->Y;
			//Вычисление поля по которому кликнули
			int j = x / fieldSize;
			int i = y / fieldSize;

			if (field->SelectedFigure != nullptr)
			{
				//если есть выделенная фигура
				if (field->Moves[i][j])
				{
					//если выделенная фигура может пойти на клетку по которой кликнули
					//сделать ход
					bool MoveResult = field->Move(i, j);
					

					if (MoveResult)
					{
						if (field->Checkmate)
						{
							if (field->CurrentMoveColor == -1)
							{
								blackWins++;
							}
							else
							{
								whiteWins++;
							}
							return;
						}
						if(AI_Switch){
							double score;
							bool idk=false;
							precom_flag = true;
						FigureMove * move = ai->BestMove(field, field->CurrentMoveColor, 0, score, idk);
							precom_flag = false;

						

							ai->precount_ini(field, &precom_flag);


						//Отметка хода
						field->Moves[move->StartI][move->StartJ] = true;
						field->Moves[move-> FinishI][move->FinishJ] = true;
						}
						
						if (field->Checkmate)
						{
							if (field->CurrentMoveColor == -1)
							{
								blackWins++;
							}
							else
							{
								whiteWins++;
							}
							return;
						}
					}
				}
				else
				{
					//если кликнули по клетке, куда нельзя пойти
					SelelectNewFigure(i, j);
				}
			}
			else
			{
				//если нет выделенной фигуры
				SelelectNewFigure(i, j);
			}

			//Перерисовать поле
			pictureBox2->Invalidate();
			return;
		}
		//метод для выделения фигуры
		void SelelectNewFigure(int i, int j)
		{
			//усли в клетке есть фигура
			if (field->Figures[i][j] != nullptr)
			{
				//если у фигуры цвет тех, кто сейча сходит
				if (((field->Figures[i][j]->state>=0) && (field->CurrentMoveColor>=0)) || ((field->Figures[i][j]->state < 0) && (field->CurrentMoveColor < 0)))
				{
					//выделить фигуру и вернуться
					field->SelectFigure(i, j);
					return;
				}
			}
			//отменить выделение фигуры
			field->ClearMovesMap();
			field->SelectedFigure = nullptr;
		}
	
		private: System::Void button1_Click_1(System::Object^ sender, System::EventArgs^ e) 
		{
			//Удаление и пересоздание игровых объектов
			delete field;
			delete ai;
			field = new Field();
			ai = new AI();
			pictureBox2->Invalidate();
		}
	};
}
