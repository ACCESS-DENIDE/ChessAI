//Depth of predicting
#define LAYER 2

//Percent of moves (has low priority)
#define PERCENT 30

//max posibilities (if 0- no limit) (has biger priority)
#define LIMIT 10

#include "AI.h"
#include <fstream>
#include <algorithm>    // std::sort
#include <thread>
#include"SpecFunc.h"

using namespace std;

bool flag_res;
bool source;
FigureMove* AI::RandomMove(Field*field,int color)
{
	CreateMoveList(field, color);

	//���� ��� �� ���������
	//������� �� ������ ��������� ����� ��� ����
	bool Done = true;
	while (Done)
	{
		//��� ���
		if (Moves.size() == 0)
			return nullptr;

		//������� ��������� �� ��������� ���
		//��������� ��������� ����� ����� �����
		//http://cppstudio.com/post/339/
		int randomNumber = rand() % Moves.size();
		bestMove = Moves[randomNumber];

		//������� ���
		field->SelectFigure(bestMove.StartI, bestMove.StartJ);
		Done = !field->Move(bestMove.FinishI, bestMove.FinishJ);
		
		//���� ��� �� ������
		if (!Done)
		{
			//������� ��� �� �������
			Moves.erase(Moves.begin() + randomNumber);
		}
	}
	return &bestMove;


}

//��� ������� ������������ ��� ����������

bool CompareFunc(FigureMove i, FigureMove j) 
{ 
	return (i.efficiency > j.efficiency); 
}

FigureMove* AI::BestMove(Field* field, int color, int layer, double& master_score, bool& res_flag_not)
{
	char resolver[69];
	int cou=0;
	//����� ������
	flag_res = false;
	source = false;

	//������������ ������-��������������
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (field->Figures[i][j] != nullptr) {
				
				resolver[ cou*2] = char(field->Figures[i][j]->state + 97);
				switch (field->Figures[i][j]->FigureType)
				{
				case pawn:
					resolver[cou * 2 + 1] = 97;
					break;

				case rock:
					resolver[cou * 2 + 1] = 98;
					break;

				case bishop:
					resolver[cou * 2 + 1] = 99;
					break;

				case knight:
					resolver[cou * 2 + 1] = 100;
					break;

				case queen:
					resolver[cou * 2 + 1] = 101;
					break;
				case king:
					resolver[cou * 2 + 1] = 102;
					break;

				default:
					break;
				}
				
				cou++;
			}
		}

	}
	cou = cou * 2;
	resolver[cou] = '\0';
	
	//���� ���� �������, �� �������� ��������� ����� ��� ������ �����

	if (layer == 0) {
		thread scanRes(&AI::resolve,this, resolver);
			
			 //������ ������ ����� ����������� ��������� ������
				scanRes.detach();
		
	//resolve(resolver);
	}

	
	//���� ������� � ����� �� ����������, ������ �������
	if (!source) {
		//�������� ���������� ���� ��� �� � ��������� ��(������ 149) ��� �������(�������� � ������ ����� �����������)
		ai_sheet = new Field(field);
		ai_sheet->Check = field->Check;
		ai_sheet->Checkmate = field->Checkmate;
		ai_sheet->CurrentMoveColor = color;
		int PP = ai_sheet->CurrentMoveColor;
		//������������ ������� �����
		CreateMoveList(ai_sheet, color);

		//��� ���
		if (Moves.size() == 0)
			return nullptr;

		//������������� ������� 
		std::sort(Moves.begin(), Moves.end(), CompareFunc);

		//������ ���������� ����������� �����
		float pos;
		pos = PERCENT;
		pos = Moves.size() * (pos / 100);

		if (pos < 1) {
			pos = 1;
		}
		if (LIMIT > 0) {
			if (pos > LIMIT) {
				pos = LIMIT;
			}
		}

		AI* baby_ai = new AI;
		for (int i = 0; i < pos; i++)
		{	
			//�������� �� �������� ������� ����� ��������� �� flag_res �������� ����
			if (res_flag_not || (layer==0 && flag_res==true)) {
				

				if (layer == 0) {
					break;
				}
				delete ai_sheet;
				delete baby_ai;
				return &Moves[i];
			}
	
				//������ ����� �������� ��������� �������� ��
			ai_sheet->CopyToMe(field);
			bool flag = false;
			
				ai_sheet->CurrentMoveColor = color;
				if (ai_sheet->SelectFigure(Moves[i].StartI, Moves[i].StartJ)) {
					flag = ai_sheet->Move(Moves[i].FinishI, Moves[i].FinishJ);
				}
				if (!flag) {

					//���� ��� ���������, �� �� ������������� ��������
					Moves.erase(Moves.begin());
					if (Moves.size() == 0) {
						if (layer % 2 == 0) {
							//���� ������ ����, �� ������ ��������� ���, � ������ ���� ��� ������ � �������� �������� ��������
							master_score -= 10;
							return  &bestMove;
						}
						else {
							//������, ���� ���� ��������, �� ������� ���� �����, � � ������ ������ ���������� ����� �������� ������ ������
							master_score += 10;
							return &bestMove;
						}
						

					}
					i--;
					if (i < 0) {
						i = 0;
					}
					if (i > Moves.size()-1) {
						break;
					}

					pos = PERCENT;
					pos = Moves.size() * (pos / 100);

					if (pos < 1) {
						pos = 1;
					}
					if (LIMIT > 0) {
						if (pos > LIMIT) {
							pos = LIMIT;
						}
					}
				

			} while (!flag);

			if (layer == 0) {
				//����� ������� ���� ������� ����
				baby_ai->BestMove(ai_sheet, Figure::InverseColor(color), 1, Moves[i].efficiency, flag_res);
			}
			else {

				//master_score- ������ �� ���� ���� �� ������� ������
				if (layer % 2 == 0) {
					//����� �������, ������������� ������ ����� ������������
					master_score += Moves[i].efficiency;
				}
				else {
					//������ �������� ����� �� ����� ���� �� ������� �� ������� ����(��������� ��������� �������� ������������� ���� �����)
					//����� ���� �������� ������������������� ��������� ����� ���� � ��������� ������ �����
					if (!(LAYER % 2 ==0 && layer>LAYER)) {
						//� ����� ����������
						master_score -= Moves[i].efficiency;
					}
					else {
						int pp;
						pp = 10;
					}
				}

				if (layer <= LAYER) {
					//������� �� ����������� ������
					baby_ai->BestMove(ai_sheet, Figure::InverseColor(color), layer + 1, master_score, res_flag_not);

				}

			}

		}

		//�������� ��������� ��������
		delete ai_sheet;
		delete baby_ai;

	}
	
		
	


	if (layer == 0) {
		//���� ��� ������ ����� � �����, ������ ����� ���������� ������ ��(��� ���������� ������ ����� ������� �������� ������������� �����������)
		if (source) {
		Moves.clear();
		Moves.push_back(Readed_move);


		}
	std::sort(Moves.begin(), Moves.end(), CompareFunc);
	//���� ��� �� ���������
	//������� �� ������ ��������� ����� ��� ����
	bool Done = false;
	field->CurrentMoveColor = color;
	while (!Done)
	{
		Done = true;
				
		if (Moves.size() == 0) {
	
			return	nullptr;
		}
		bestMove = Moves[0];

		//������� ���
		field->SelectFigure(bestMove.StartI, bestMove.StartJ);
		Done = field->Move(bestMove.FinishI, bestMove.FinishJ);

		//���� ��� �� ������
		if (!Done)
		{
			//������� ��� �� �������
			Moves.erase(Moves.begin());
		}
	}
	if (!source) {
		//���� ������� ���� ������� �� � ����� �� ������ ������ ����� ���� � �������
		//����� �������� ����
		ifstream fout("Table");
		ofstream fin_b("buffer");
		do
		{
			char buff[69];
			
				fout >> buff;
				fin_b << buff<<endl;

		} while (!fout.eof());
		fout.close();
		fin_b.close();

		ofstream fin("Table");
		resolver[cou] = bestMove.StartI+97;
		resolver[cou+1] = bestMove.StartJ+97;
		resolver[cou+2] = bestMove.FinishI+97;
		resolver[cou+3] = bestMove.FinishJ+97;
		resolver[cou + 4] = '\0';
		fin << resolver<<endl;

		ifstream fout_b("buffer");

		do
		{
			char buff[69];

			fout_b >> buff;
			fin << buff<<endl;
			

		} while (!fout_b.eof());

		fin.close();

	}

	return &bestMove;

	}
	
}

//��������� ��� ��������� ���� ������ � �������
void AI::AllFigureMovesToDeque(Figure* figure,Field *field)
{
int abs_st = abs(figure->state);

	figure->FillMovesMapWithClear(DivDwnFlr(abs_st, 8), abs_st %8);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//���� � ����� ����� ���� ���� ���
			if (field->Moves[i][j])
			{
				//��������� ��� � ���������� ��� �������������
				FigureMove move;
				
				move.StartI = DivDwnFlr(abs_st, 8);
				move.StartJ = abs_st % 8;
				move.FinishI = i;
				move.FinishJ = j;

				//������� �������� ���� ������������ �������� ����
				Field* testedField = new Field(field);
				//��������� �� ��� �������
				int oldPosition = testedField->CalcPosition(abs_st);

				//������� �� ��� ���
				testedField->SelectFigure(move.StartI, move.StartJ);
				bool moveResult = testedField->Move(move.FinishI, move.FinishJ);
				if (!moveResult)
				{
					//���� ������� ��� �� ���������� - �� ��������� ���� ��� � �������
					//������� �������� ���� � ���������� ������ ������ ����
					delete testedField;
					continue;
				}
				
				//��������� ��� �������������
				int newPosition = testedField->CalcPosition(abs_st);

				move.efficiency =  newPosition - oldPosition;
				//�������� ��������� ��������� ��������, ����� ��� ���������� �������������� 
				//��������� ��������� ���. ��������� ��� ���������� ��������� ����������
				move.efficiency += (double)rand() / (RAND_MAX-1);

				//����������� �������������, ���� ��������� ������ ��������
				AttackCorrection(testedField, &move, figure);

				//������� �������� ����
				delete testedField;

				//�������� ��� � �������
				Moves.push_back(move);
			}
		}
	}
}

void AI::AttackCorrection(Field* field,FigureMove * move, Figure * figure)
{
	//������������� ����� ����� ���������� �� �������� ����
	field->GetAllAttackMap(Figure::InverseColor(figure->state));

	//���� ��� ���� ���������
	if (field->Moves[move->FinishI][move->FinishJ])
	{
		//������� ���� ������ �� �������������
		move->efficiency -= figure->FigureType;
	}
}


void AI::CreateMoveList(Field* field, int state)
{

	if (field->Checkmate)
		return;
	Moves.clear();
	//������� ������ ���� ��������� �����

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Figure* figure = field->Figures[i][j];
			
			if (figure != nullptr)
			{
				if (((figure->state >= 0) && (state >= 0)) || ((figure->state < 0) && (state < 0)))
				{
					AllFigureMovesToDeque(figure, field);
				}
			}
		}
	}
	return;
}


void AI::resolve(char inp_cond[69]) {

	ifstream fin("Table");
	char chec_cr[69];
	// ������������ ����� ��� ��������� ���������� ������-��������������
	do{
		fin >> chec_cr;
		if (!flag_res) {
			bool ck_flg = true;
			int cou = -1;
			do
			{
				cou++;
				if (!(inp_cond[cou] == '\0')) {

					if (chec_cr[cou] == '\0')
					{
						ck_flg = false;
						break;
					}

					if (!(inp_cond[cou] == chec_cr[cou]))
					{
						ck_flg = false;
						break;
					}
				}

			} while (!(inp_cond[cou] == '\0'));

			if (ck_flg) {
				//��� ����������� ���������� ������ �� ���������� ��� ����������� ��� � �������� ���
				Readed_move.StartI = chec_cr[cou]-97;
				Readed_move.StartJ = chec_cr[cou + 1] - 97;
				Readed_move.FinishI = chec_cr[cou + 2] - 97;
				Readed_move.FinishJ = chec_cr[cou + 3] - 97;
				//���� � ��� ��� ������� �������
				flag_res = true;
				fin.close();
				//���� �� ��������� �������(� ������ ������ ��������-����)
				source = true;
				return;
			}
		}
		else {
			break;
		}
	} while (!fin.eof());
	fin.close();
	//���� �� ��������� �������(� ������ ������ � ����� ������� ���)
	source= false;
}

void AI::precount(Field *field, bool* signal)
{
	Field* prec_field = new Field(field);

	prec_field->Check = false;
	prec_field->Checkmate = false;
	CreateMoveList(prec_field, -1);

	//��� ���
	if (Moves.size() == 0)
		return ;


	//������������� ������� 
	std::sort(Moves.begin(), Moves.end(), CompareFunc);
	bool flag=false;

	pre_ai = new AI;
	for (int i = 0; i < Moves.size(); i++)
	{
		if (!(*signal)) {

			prec_field->CopyToMe(field);
			prec_field->CurrentMoveColor = -1;

		
			if (prec_field->SelectFigure(Moves[i].StartI, Moves[i].StartJ)) {
				flag = prec_field->Move(Moves[i].FinishI, Moves[i].FinishJ);

			}
			if (!flag) {

				Moves.erase(Moves.begin());
				i--;
				if (i < 0) {
					i = 0;
				}
				if (i > Moves.size()-1) {
					//����� ���� ���
					delete prec_field;
					delete pre_ai;
					return;
				}
				
			}
			else {
				
				double mpty_i;
				bool mpty_b;
				pre_ai->BestMove(prec_field, 1, 0, mpty_i,*signal);

			}
		}
		else {
			//�����, ���� ����� ��������� ���
			delete prec_field;
			delete pre_ai;
			return;
		}
	}
	//�����, ���� ��������� ��� �������
	delete prec_field;
	delete pre_ai;
	return;
}

void AI::precount_ini(Field* field, bool *signal)
{
	thread precounting(&AI::precount, this, field, signal);
	precounting.detach();
}


