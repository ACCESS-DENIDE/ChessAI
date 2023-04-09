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
	//������� ����
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Figures[i][j] = 0;
		}
	}

	//������� ����� 
	ClearMovesMap();

	//����� ������ � ���������� ������
	Figure* figure;


	//����������� �����
	//������
	//�����
	for (int j = 0; j < 8; j++)
	{
		figure = new Pawn(this);
		figure->state = 1 * 8 + j;
		Figures[1][j] = figure;
	}

	//�����
	figure = new Rock(this);
	figure->state = 0;
	Figures[0][0] = figure;

	figure = new Rock(this);
	figure->state = 7;
	Figures[0][7] = figure;
	

	//����
	figure = new Knight(this);
	figure->state =  1;
	Figures[0][1] = figure;

	figure = new Knight(this);
	figure->state = 6;
	Figures[0][6] = figure;

	//�����
	figure = new Bishop(this);
	figure->state = 2;
	Figures[0][2] = figure;

	figure = new Bishop(this);
	figure->state = 5;
	Figures[0][5] = figure;

	//������
	figure = new King(this);
	figure->state = 4;
	Figures[0][4] = figure;
	BlackKing = figure;

	//�����
	figure = new Queen(this);
	figure->state = 3;
	Figures[0][3] = figure;

	//�����
	for (int j = 0; j < 8; j++)
	{
		figure = new Pawn(this);
		figure->state = 0-(48+j);
		Figures[6][j] = figure;
	}

	//�����
	figure = new Rock(this);
	figure->state = -56;
	Figures[7][0] = figure;

	figure = new Rock(this);
	figure->state = -63;
	Figures[7][7] = figure;

	//����
	figure = new Knight(this);
	figure->state = -57;
	Figures[7][1] = figure;

	figure = new Knight(this);
	figure->state = -62;
	Figures[7][6] = figure;

	//�����
	figure = new Bishop(this);
	figure->state = -58;
	Figures[7][2] = figure;

	figure = new Bishop(this);
	figure->state = -61;
	Figures[7][5] = figure;

	//������
	figure = new King(this);
	figure->state = -60;
	Figures[7][4] = figure;
	WhiteKing = figure;

	//�����
	figure = new Queen(this);
	figure->state = -59;
	Figures[7][3] = figure;

	
}



Field::Field(Field *field)
{
	//������� ����
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Figures[i][j] = 0;
		}
	}

	//������� ����� 
	ClearMovesMap();

	//�����������
	CopyToMe(field);
}


bool Field::CellIsValid(int i, int j)
{
	//��������, ������ �� ���������� ������ � ������� ����
	return (i >= 0) && (j >= 0) && (i < 8) && (j < 8);
}

//���� ������ ����
void Field::ClearMovesMap()
{
	//������������� false ���� ��������� �������
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Moves[i][j] = false;
		}
	}
}
//������� ������
bool Field::SelectFigure(int i, int j)
{
	//���� ���������� �� �������� - ������� �� ������� ����
	//�� �� ������ ������
	if (!CellIsValid(i, j))
		return false;

	//���� � ��������� ���� ��� ������
	//�� �� ������ ������
	Figure* figure = Figures[i][j];
	if (figure == nullptr)
		return false;

	//���� ������� ������ ���� �����, ������� �����
	//�� ����� ����� ��������� �����
	//if (((-1 == CurrentMoveColor) && (figure->state<0))|| ((1 == CurrentMoveColor) && (figure->state >= 0)))
	{
		SelectedFigure = figure;
		figure->FillMovesMapWithClear(i,j);
		return true;
	}
	return false;
}

//������� ��� ���������� �������
bool Field::Move(int i, int j,bool withoutCheckmate)
{
	
	//if (SelectedFigure != nullptr) {
		if (Checkmate)
		{
			//��� ��� ���������
			return false;
		}

		//��������� ���� �� ����������
		if (!CellIsValid(i, j))
		{
			return false;
		}
		
		//������� ��� ����
		Field* oldField = new Field(this);

		//������� ������, ���� ���� �����
		if (Figures[i][j] != nullptr)
		{
			delete Figures[i][j];
		}
		//���������
		//������ ������ ����� - ������� ����������� �����
		if (SelectedFigure->FigureType == FigureTypes::king)
		{
			//���� ������ ������ ����� ������ ��� �� ���� ������, �� ��� ���������
			if (abs((abs(SelectedFigure->state % 8)) - j) > 1)
			{
				//� ����� ������� ������ ������
				if ((SelectedFigure->state % 8) - j > 0)
				{
					//�����
					if (Figures[i][0] != nullptr && Figures[i][3] != nullptr) {
						Figures[i][3] = Figures[i][0];
						Figures[i][3]->state = Figures[i][3]->state - (Figures[i][3]->state % 8) + 3;
						Figures[i][0] = nullptr;
					}
				}
				else
				{
					//������
					if (Figures[i][7] != nullptr && Figures[i][3] != nullptr) {
						Figures[i][5] = Figures[i][7];
						Figures[i][5]->state = Figures[i][5]->state - (Figures[i][5]->state % 8) + 5;
						Figures[i][7] = nullptr;
					}
				}
			}
		}

		//����������� ������
		

		//������� ����� ������
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
		//�������� ��� ��������� ��������� � ������� � ��������� �������
		//������� ������� �� ����������	
		Figures[DivDwnFlr(CurrentMoveColor * (SelectedFigure->state), 8)][((CurrentMoveColor * SelectedFigure->state) % 8)] = nullptr;
		//����������� ����������
		SelectedFigure->state = i * 8;
		SelectedFigure->state = SelectedFigure->state + j;
		if (CurrentMoveColor == -1) {
			SelectedFigure->state = SelectedFigure->state * -1;
		}
		//���������� ���������
		SelectedFigure = nullptr;

		//���� ��� ��� ���
		if (CheckTest(Figure::InverseColor(CurrentMoveColor)))
		{
			//��������� �� �����
			CopyToMe(oldField);

			ClearMovesMap();
			//��� ��� ���, ������ ��� ������
			return false;
		}

		delete oldField;

		//��������� ���, ���� �� ����
		CheckTest(CurrentMoveColor);

		//�� ��������� �� ���
		//��� ����� ��� �������� �� ���, ����� �� ������� � ��������
		if (!withoutCheckmate)
		{
			//�������� �� ���
			CheckmateTest(CurrentMoveColor);
		}
		//����������� ����
		CurrentMoveColor = Figure::InverseColor(CurrentMoveColor);


		ClearMovesMap();

		return true;
	//}
//	else
	//{
	//	return false;
	//}
}

//���������� ��� ��������� ���� ������ �����
void Field::GetAllAttackMap(int State)
{
	//�������� ��� ����
	ClearMovesMap();

	//��������� ��� ������
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//���� ������ ���� �� ���� ����
			if (Figures[i][j] != nullptr)
			{
				//���� ������ ������� �����
				if (((Figures[i][j]->state <0)  && (State<0))|| ((Figures[i][j]->state >= 0) && (State >= 0)))
				{
					//�������� ��� �� ��������� ����� � ����� ����� ��������� ����
					Figures[i][j]->FillAttackMap(i,j);
				}
			}
		}
	}

}

//����������
Field::~Field()
{
	//������� ��� ������
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (Figures[i][j] != nullptr)
			{
				//������� ������
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
	//����������� ����� 
	CurrentMoveColor = field->CurrentMoveColor;

	//������� ����
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//������ ������, ���� ��� ����
			if (Figures[i][j] != nullptr)
			{
				delete Figures[i][j];
				//Figures[i][j] = nullptr;
			}

			//������� ������
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

					//���������� ��������� �� �������
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

	//������� ����� 
	ClearMovesMap();

	//���������� ��������� �����
	
}

//�������� �� ��� ��� ��������� �����
//������ ��������� ����� ��������� ���
bool Field::CheckmateTest(int color)
{
	//�������� �������� �� ���:
	//��������� ��� ��������� ���� ���� ��������� ����� ���������� �����
	//���� ����� ���� ������ ����������� ���, �� ��������� ���
	
	//���� ��������� ���
	int matedColor = Figure::InverseColor(color);

	//���������� �������, ��� ���
	Checkmate = true;

	//��� ���� � ��� �� - �������� ���� ����� ��������� �����
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Figure* figure = Figures[i][j];
			if(figure!= nullptr)
				if (((figure->state>=0) && (matedColor>=0)) || ((figure->state < 0) && (matedColor < 0)))
				{
					//��� ���������� ��� ������ ������ ��������� �����
					
					//�������� ����� ����� ������
					figure->FillMovesMapWithClear(i, j);

					
					//��������� ��� ��������� ���� ������ �� ���������� ����
					//����� ������� ������� �����������, ������� ������ ��� �� �������
					Checkmate = CheckAllMovesFigure(figure, color);
					
					if (!Checkmate)
					{
						//���� ������������ ���
						return false;
					}
				}
		}
	}
	return Checkmate;
}


//��������� ��� ���� ����� ������ � ������� True, ���� ����� ������� ���� ����� ���
bool Field::CheckAllMovesFigure(Figure* figure,int color)
{
	//���� ��� �������� ������ ����
	Field* oneMoveField = new Field(this);

	//����� ��� ���� ����� ����� ������
	//��� ���� ����� � �����
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (Moves[i][j])
			{
				// ����������� ����
				oneMoveField->CopyToMe(this);
				//����� ����, ����� ����� ���� ������
				oneMoveField->CurrentMoveColor = Figure::InverseColor(oneMoveField->CurrentMoveColor);

				//������� ���

				if (oneMoveField->SelectFigure(DivDwnFlr(abs(figure->state), 8), abs(figure->state) % 8)) {
				oneMoveField->Move(i, j,true);
				};
				
				
				//��������� ���
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

//���������� ������� ��������� ����� �� �����
int Field::CalcPosition(int state)
{

	int position = 0;
	//������� ��� ���� ����� ��������� �����
	//������� ��� ���� ����� ���������� �����
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
