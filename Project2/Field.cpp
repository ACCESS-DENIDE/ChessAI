#include "Field.h"
#include "Pawn.h"
#include "Rock.h"
#include "Queen.h"
#include "King.h"
#include "Knight.h"
#include "Bishop.h"
#include <stdexcept>

#include"SpecFunc.h"

Field::Field()
{
	//Очистка поля
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Figures[i][j] = 0;
		}
	}

	//Очистка ходов 
	ClearMovesMap();

	//здесь создаю и настраиваю фигуру
	Figure* figure;


	//расстановка фигур
	//черные
	//пешки
	for (int j = 0; j < 8; j++)
	{
		figure = new Pawn(this);
		figure->state = 1 * 8 + j;
		Figures[1][j] = figure;
	}

	//ладьи
	figure = new Rock(this);
	figure->state = 0;
	Figures[0][0] = figure;

	figure = new Rock(this);
	figure->state = 7;
	Figures[0][7] = figure;
	

	//кони
	figure = new Knight(this);
	figure->state =  1;
	Figures[0][1] = figure;

	figure = new Knight(this);
	figure->state = 6;
	Figures[0][6] = figure;

	//слоны
	figure = new Bishop(this);
	figure->state = 2;
	Figures[0][2] = figure;

	figure = new Bishop(this);
	figure->state = 5;
	Figures[0][5] = figure;

	//король
	figure = new King(this);
	figure->state = 4;
	Figures[0][4] = figure;
	BlackKing = figure;

	//ферзь
	figure = new Queen(this);
	figure->state = 3;
	Figures[0][3] = figure;

	//Белые
	for (int j = 0; j < 8; j++)
	{
		figure = new Pawn(this);
		figure->state = 0-(48+j);
		Figures[6][j] = figure;
	}

	//ладьи
	figure = new Rock(this);
	figure->state = -56;
	Figures[7][0] = figure;

	figure = new Rock(this);
	figure->state = -63;
	Figures[7][7] = figure;

	//кони
	figure = new Knight(this);
	figure->state = -57;
	Figures[7][1] = figure;

	figure = new Knight(this);
	figure->state = -62;
	Figures[7][6] = figure;

	//слоны
	figure = new Bishop(this);
	figure->state = -58;
	Figures[7][2] = figure;

	figure = new Bishop(this);
	figure->state = -61;
	Figures[7][5] = figure;

	//король
	figure = new King(this);
	figure->state = -60;
	Figures[7][4] = figure;
	WhiteKing = figure;

	//ферзь
	figure = new Queen(this);
	figure->state = -59;
	Figures[7][3] = figure;

	
}



Field::Field(Field *field)
{
	//Очистка поля
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Figures[i][j] = 0;
		}
	}

	//Очистка ходов 
	ClearMovesMap();

	//Копирование
	CopyToMe(field);
}


bool Field::CellIsValid(int i, int j)
{
	//проверка, попали ли координаты клетки в игровое поле
	return (i >= 0) && (j >= 0) && (i < 8) && (j < 8);
}

//чищу массив карт
void Field::ClearMovesMap()
{
	//присваивается false всем элементам массива
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Moves[i][j] = false;
		}
	}
}
//Выбрать фигуру
bool Field::SelectFigure(int i, int j)
{
	//Если координата не валидная - выходит за пределы поля
	//то не делать ничего
	if (!CellIsValid(i, j))
		return false;

	//Если в выбранном поле нет фигуры
	//то не делать ничего
	Figure* figure = Figures[i][j];
	if (figure == nullptr)
		return false;

	//если выбрана фигура того цвета, который ходит
	//то меняю карту возможных ходов
	//if (((-1 == CurrentMoveColor) && (figure->state<0))|| ((1 == CurrentMoveColor) && (figure->state >= 0)))
	{
		SelectedFigure = figure;
		figure->FillMovesMapWithClear(i,j);
		return true;
	}
	return false;
}

//Сделать ход выделенной фигурой
bool Field::Move(int i, int j,bool withoutCheckmate)
{
	
	//if (SelectedFigure != nullptr) {
		if (Checkmate)
		{
			//мат уже поставлен
			return false;
		}

		//проверить поле на валидность
		if (!CellIsValid(i, j))
		{
			return false;
		}
		
		//Копирую всю игру
		Field* oldField = new Field(this);

		//Удалить фигуру, если была сбита
		if (Figures[i][j] != nullptr)
		{
			delete Figures[i][j];
		}
		//Рокировка
		//Король пойдет позже - сначала переместить ладью
		if (SelectedFigure->FigureType == FigureTypes::king)
		{
			//Если король должен пойти больше чем на одну клетку, то это рокировка
			if (abs((abs(SelectedFigure->state % 8)) - j) > 1)
			{
				//В какую сторону пойдет король
				if ((SelectedFigure->state % 8) - j > 0)
				{
					//Влево
					if (Figures[i][0] != nullptr && Figures[i][3] != nullptr) {
						Figures[i][3] = Figures[i][0];
						Figures[i][3]->state = Figures[i][3]->state - (Figures[i][3]->state % 8) + 3;
						Figures[i][0] = nullptr;
					}
				}
				else
				{
					//Вправо
					if (Figures[i][7] != nullptr && Figures[i][3] != nullptr) {
						Figures[i][5] = Figures[i][7];
						Figures[i][5]->state = Figures[i][5]->state - (Figures[i][5]->state % 8) + 5;
						Figures[i][7] = nullptr;
					}
				}
			}
		}

		//переместить фигуру
		

		//Сделать пешку ферзем
		if (SelectedFigure->FigureType == FigureTypes::pawn)
		{
			if (SelectedFigure->state < 0)
			{
				if (i == 0) {

					Figure* newQueen = new Queen(this);
					newQueen->state = SelectedFigure->state;
					delete SelectedFigure;
					SelectedFigure =newQueen;
				}
			}
			else
			{
				if (i == 7) {
				
					Figure* newQueen = new Queen(this);
					newQueen->state = SelectedFigure->state;
					delete SelectedFigure;
					SelectedFigure = newQueen;
				}
			}
		}


		Figures[i][j] = SelectedFigure;
		//очистить все переменые связанные с клеткой и выбранной фигурой
		//берутся прошлые ее координаты	
		Figures[DivDwnFlr(CurrentMoveColor * (SelectedFigure->state), 8)][((CurrentMoveColor * SelectedFigure->state) % 8)] = nullptr;
		//обновляются координаты
		SelectedFigure->state = i * 8;
		SelectedFigure->state = SelectedFigure->state + j;
		if (CurrentMoveColor == -1) {
			SelectedFigure->state = SelectedFigure->state * -1;
		}
		//Отменяется выделение
		SelectedFigure = nullptr;

		//Если все еще шах
		if (CheckTest(Figure::InverseColor(CurrentMoveColor)))
		{
			//Возвращаю из копии
			CopyToMe(oldField);

			ClearMovesMap();
			//все еще шах, нельзя так ходить
			return false;
		}

		delete oldField;

		//Выставляю шах, если он есть
		CheckTest(CurrentMoveColor);

		//Не проверять на мат
		//Это нужно для проверки на мат, чтобы не уходило в рекурсию
		if (!withoutCheckmate)
		{
			//Проверяю на мат
			CheckmateTest(CurrentMoveColor);
		}
		//переключить цвет
		CurrentMoveColor = Figure::InverseColor(CurrentMoveColor);


		ClearMovesMap();

		return true;
	//}
//	else
	//{
	//	return false;
	//}
}

//посмотреть все возможные ходы одного цвета
void Field::GetAllAttackMap(int State)
{
	//очистить все ходы
	ClearMovesMap();

	//перебрать все фигуры
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//Если фигура есть на этом поле
			if (Figures[i][j] != nullptr)
			{
				//Если фигура нужного цвета
				if (((Figures[i][j]->state <0)  && (State<0))|| ((Figures[i][j]->state >= 0) && (State >= 0)))
				{
					//Добавить все ее возможные атаки к общей карте возможных атак
					Figures[i][j]->FillAttackMap(i,j);
				}
			}
		}
	}

}

//деструктор
Field::~Field()
{
	//Удалить все фигуры
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (Figures[i][j] != nullptr)
			{
				//Удалить фигуру
				delete Figures[i][j];
			}
		}
	}
}


bool Field::CheckTest(int State)
{
	GetAllAttackMap(State);
	if (State >=0)
	{
		if (Moves[DivDwnFlr(-1*(WhiteKing->state), 8)][( - 1 * WhiteKing->state) % 8])
		{
			Check = true;
		}
		else
		{
			Check = false;
		}
	}
	else
	{
		if (Moves[DivDwnFlr(BlackKing->state, 8)][BlackKing->state % 8])
		{
			Check = true;
		}
		else
		{
			Check = false;
		}
	}
	return Check;
}


void Field::CopyToMe(Field* field)
{
	//копирование цвета 
	CurrentMoveColor = field->CurrentMoveColor;

	//Очистка поля
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//Удаляю фигуры, если они есть
			if (Figures[i][j] != nullptr)
			{
				delete Figures[i][j];
				//Figures[i][j] = nullptr;
			}

			//Копирую фигуры
				if (field->Figures[i][j] != nullptr)
			{
				Figure* figure = field->Figures[i][j];
				if (figure->FigureType == FigureTypes::bishop)
				{
					Figure* newFigure = new Bishop(this);
					newFigure->state = figure->state;
					Figures[i][j] = newFigure;
				}
				if (figure->FigureType == FigureTypes::king)
				{
					Figure* newFigure = new King(this);
					newFigure->state = figure->state;
					Figures[i][j] = newFigure;

					//Настраиваю указатели на королей
					if (figure->state >= 0)
					{
						BlackKing = newFigure;
					}
					else
					{
						WhiteKing = newFigure;
					}
				}
				if (figure->FigureType == FigureTypes::knight)
				{
					Figure* newFigure = new Knight(this);
					newFigure->state = figure->state;
					Figures[i][j] = newFigure;
				}
				if (figure->FigureType == FigureTypes::pawn)
				{
					Figure* newFigure = new Pawn(this);
					newFigure->state = figure->state;
					Figures[i][j] = newFigure;
				}
				if (figure->FigureType == FigureTypes::queen)
				{
					Figure* newFigure = new Queen(this);
					newFigure->state = figure->state;
					Figures[i][j] = newFigure;
				}
				if (figure->FigureType == FigureTypes::rock)
				{
					Figure* newFigure = new Rock(this);
					newFigure->state = figure->state;
					Figures[i][j] = newFigure;
				}
			}
			else
			{
				Figures[i][j] = nullptr;
			}
		}
	}

	//Очистка ходов 
	ClearMovesMap();

	//обновление координат фигур
	
}

//ПРоверка на мат для заданного цвета
//фигуры заданного цвета поставили мат
bool Field::CheckmateTest(int color)
{
	//Алгоритм проверки на мат:
	//ПРоверить все возможные ходы всех возможных фигур выбранного цвета
	//если после хода каждой сохранятеся шах, то поставлен мат
	
	//Кому поставили мат
	int matedColor = Figure::InverseColor(color);

	//Изначально считаем, что мат
	Checkmate = true;

	//два фора и два иф - проверка всех фигур заданного цвета
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Figure* figure = Figures[i][j];
			if(figure!= nullptr)
				if (((figure->state>=0) && (matedColor>=0)) || ((figure->state < 0) && (matedColor < 0)))
				{
					//Это выполнится для каждой фигуры заданного цвета
					
					//собирает карту ходов фигуры
					figure->FillMovesMapWithClear(i, j);

					
					//Проверяет все возможные ходы фигуры на сохранение шаха
					//Здесь слишком большая вложенность, поэтому дроблю это на функции
					Checkmate = CheckAllMovesFigure(figure, color);
					
					if (!Checkmate)
					{
						//Есть спасительный ход
						return false;
					}
				}
		}
	}
	return Checkmate;
}


//Проверить все ходы одной фигуры и вернуть True, если после каждого хода будет шах
bool Field::CheckAllMovesFigure(Figure* figure,int color)
{
	//Поле для проверки одного хода
	Field* oneMoveField = new Field(this);

	//Здесь уже есть карта ходов фигуры
	//для всех ходов в карте
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (Moves[i][j])
			{
				// скопировать поле
				oneMoveField->CopyToMe(this);
				//Меняю цвет, чтобы можно было ходить
				oneMoveField->CurrentMoveColor = Figure::InverseColor(oneMoveField->CurrentMoveColor);

				//сделать ход

				if (oneMoveField->SelectFigure(DivDwnFlr(abs(figure->state), 8), abs(figure->state) % 8)) {
				oneMoveField->Move(i, j,true);
				};
				
				
				//проверить шах
				if (!oneMoveField->CheckTest(color))
				{
					delete oneMoveField;
					return false;
				}
			}
		}
	}
	delete oneMoveField;
	return true;
}

//Подсчитать позицию заданного цвета на доске
int Field::CalcPosition(int state)
{

	int position = 0;
	//Сложить все типы фигур заданного цвета
	//Вычесть все типы фигур вражеского цвета
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (Figures[i][j]!= nullptr)
			{
				if (((Figures[i][j]->state>0) && (state>0))|| ((Figures[i][j]->state < 0) && (state < 0)))
				{
					position += Figures[i][j]->FigureType;
				}
				else
				{
					position -= Figures[i][j]->FigureType;
				}

			}
		}
	}
	return position;
}
