#pragma once
#include "FigureMove.h"
#include "Field.h"
#include <deque>

//����� �������������� ���������
class AI
{
	//���������� ������� �����
	//��������� �� ��� https://metanit.com/cpp/tutorial/7.8.php
	//��� ������� �� ���� ��������� ����� ���� ����� ������ �����.
	std::deque<FigureMove> Moves;

	FigureMove bestMove;
	Field* ai_sheet;

	FigureMove Readed_move;
	AI* pre_ai;

public:
	//������� ��������� ��� �� ���� �� �������� ����
	FigureMove* RandomMove(Field* field, int color);

	//������� ��������� ��� �� ���� �� �������� ����
	FigureMove* BestMove(Field* field, int color, int layer, double& master_score, bool& res_flag_not);

	//��������� ��� ��������� ���� ������ � �������
	void AllFigureMovesToDeque(Figure* figure,Field * field);

	//������� ������ ���� ��������� �����
	//������ ����� ������ � Moves
	void CreateMoveList(Field* field, int state);

	//������������� ���� ����� ������
	//��� ��������� ������� �����
	void AI::AttackCorrection(Field* field, FigureMove* move, Figure* figure);

	//������� �������� ��������� ���� � ������
	void resolve(char inp_cond[69]);

	//������� �������� �������� ����
	void precount(Field *field, bool *signal);

	//������� ������� �������� �������� ����
	void precount_ini(Field* field, bool *signal);

	
};

