#pragma once
#include "FigureMove.h"
#include "Field.h"
#include <deque>

//Класс искусственного интелекта
class AI
{
	//Двусвязная очередь ходов
	//подробнее об ней https://metanit.com/cpp/tutorial/7.8.php
	//Это очередь из всех возможных ходов всех фигур одного цвета.
	std::deque<FigureMove> Moves;

	FigureMove bestMove;
	Field* ai_sheet;

	FigureMove Readed_move;
	AI* pre_ai;

public:
	//Выбрать случайный ход на поле за заданный цвет
	FigureMove* RandomMove(Field* field, int color);

	//Выбрать случайный ход на поле за заданный цвет
	FigureMove* BestMove(Field* field, int color, int layer, double& master_score, bool& res_flag_not);

	//Добавляет все возможные ходы фигуры в очередь
	void AllFigureMovesToDeque(Figure* figure,Field * field);

	//Собрать список всех доступных ходов
	//Список будет лежать в Moves
	void CreateMoveList(Field* field, int state);

	//Корректировка силы атаки фигуры
	//для пересчета размена фигур
	void AI::AttackCorrection(Field* field, FigureMove* move, Figure* figure);

	//Функция проверки сигнатуры поля с файлом
	void resolve(char inp_cond[69]);

	//функция просчёта будущего хода
	void precount(Field *field, bool *signal);

	//функция запуска просчёта будущего хода
	void precount_ini(Field* field, bool *signal);

	
};

