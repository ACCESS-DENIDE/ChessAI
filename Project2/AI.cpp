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

	//пока ход не получится
	//Сделано на случай ошибочных ходов при шахе
	bool Done = true;
	while (Done)
	{
		//Это мат
		if (Moves.size() == 0)
			return nullptr;

		//Вернуть указатель на случайный ход
		//генерация случайных чисел взята здесь
		//http://cppstudio.com/post/339/
		int randomNumber = rand() % Moves.size();
		bestMove = Moves[randomNumber];

		//Сделать ход
		field->SelectFigure(bestMove.StartI, bestMove.StartJ);
		Done = !field->Move(bestMove.FinishI, bestMove.FinishJ);
		
		//Если ход не удался
		if (!Done)
		{
			//Удалить его из вектора
			Moves.erase(Moves.begin() + randomNumber);
		}
	}
	return &bestMove;


}

//Эта функция используется для сортировки

bool CompareFunc(FigureMove i, FigureMove j) 
{ 
	return (i.efficiency > j.efficiency); 
}

FigureMove* AI::BestMove(Field* field, int color, int layer, double& master_score, bool& res_flag_not)
{
	char resolver[69];
	int cou=0;
	//сброс флагов
	flag_res = false;
	source = false;

	//формирование строки-идентификатора
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
	
	//если слой нулевой, то создаётся отдельный поток для чтения файла

	if (layer == 0) {
		thread scanRes(&AI::resolve,this, resolver);
			
			 //запуск чтения файла параллельно основному потоку
				scanRes.detach();
		
	//resolve(resolver);
	}

	
	//если решение в файле не обнаружено, запуск подбора
	if (!source) {
		//создание отдельного поля для ии и отдельных ИИ(строка 149) для решения(рекурсия в данном слчае недопустима)
		ai_sheet = new Field(field);
		ai_sheet->Check = field->Check;
		ai_sheet->Checkmate = field->Checkmate;
		ai_sheet->CurrentMoveColor = color;
		int PP = ai_sheet->CurrentMoveColor;
		//формирование очереди ходов
		CreateMoveList(ai_sheet, color);

		//Это мат
		if (Moves.size() == 0)
			return nullptr;

		//Отсортировать очередь 
		std::sort(Moves.begin(), Moves.end(), CompareFunc);

		//Расчёт количества проверяемых ходов
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
			//проверка на найденое решение через указатель на flag_res нулевого слоя
			if (res_flag_not || (layer==0 && flag_res==true)) {
				

				if (layer == 0) {
					break;
				}
				delete ai_sheet;
				delete baby_ai;
				return &Moves[i];
			}
	
				//расчёт ходов примерно идентичен прошлому ИИ
			ai_sheet->CopyToMe(field);
			bool flag = false;
			
				ai_sheet->CurrentMoveColor = color;
				if (ai_sheet->SelectFigure(Moves[i].StartI, Moves[i].StartJ)) {
					flag = ai_sheet->Move(Moves[i].FinishI, Moves[i].FinishJ);
				}
				if (!flag) {

					//если ход удаляется, то мы пересчитываем счетчики
					Moves.erase(Moves.begin());
					if (Moves.size() == 0) {
						if (layer % 2 == 0) {
							//если список пуст, то значит поставлен мат, а значит этот ход опасен и получает снижение рейтинга
							master_score -= 10;
							return  &bestMove;
						}
						else {
							//однако, если слой нечётный, ИИ считает ходы белых, и в данном случае отсутствие ходов означает победу чёрных
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
				//вызов второго слоя расчёта хода
				baby_ai->BestMove(ai_sheet, Figure::InverseColor(color), 1, Moves[i].efficiency, flag_res);
			}
			else {

				//master_score- ссылка на счёт хода на нулевом уровне
				if (layer % 2 == 0) {
					//таким образом, эффективность чёрных ходов прибавляется
					master_score += Moves[i].efficiency;
				}
				else {
					//данная проверка нужна на слуай если ИИ считает до чётного слоя(последним действием вычитают потенциальный счёт белых)
					//нужно чтоб избежать непропорционального занижения счёта хода и избежания плохих ходов
					if (!(LAYER % 2 ==0 && layer>LAYER)) {
						//а белых отнимается
						master_score -= Moves[i].efficiency;
					}
					else {
						int pp;
						pp = 10;
					}
				}

				if (layer <= LAYER) {
					//переход на последующие уровни
					baby_ai->BestMove(ai_sheet, Figure::InverseColor(color), layer + 1, master_score, res_flag_not);

				}

			}

		}

		//зачистка созданных объектов
		delete ai_sheet;
		delete baby_ai;

	}
	
		
	


	if (layer == 0) {
		//если был найден ответ в файле, список ходов заменяется только им(при нахождении ответа поиск решения подбором автоматически прерывается)
		if (source) {
		Moves.clear();
		Moves.push_back(Readed_move);


		}
	std::sort(Moves.begin(), Moves.end(), CompareFunc);
	//пока ход не получится
	//Сделано на случай ошибочных ходов при шахе
	bool Done = false;
	field->CurrentMoveColor = color;
	while (!Done)
	{
		Done = true;
				
		if (Moves.size() == 0) {
	
			return	nullptr;
		}
		bestMove = Moves[0];

		//Сделать ход
		field->SelectFigure(bestMove.StartI, bestMove.StartJ);
		Done = field->Move(bestMove.FinishI, bestMove.FinishJ);

		//Если ход не удался
		if (!Done)
		{
			//Удалить его из Очереди
			Moves.erase(Moves.begin());
		}
	}
	if (!source) {
		//если решение было найдено не в файле мы создаём запись этого хода в таблице
		//через буферный файл
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

//Добавляет все возможные ходы фигуры в очередь
void AI::AllFigureMovesToDeque(Figure* figure,Field *field)
{
int abs_st = abs(figure->state);

	figure->FillMovesMapWithClear(DivDwnFlr(abs_st, 8), abs_st %8);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//Есди в карте ходов есть этот ход
			if (field->Moves[i][j])
			{
				//заполнить ход и подсчитать его эффективность
				FigureMove move;
				
				move.StartI = DivDwnFlr(abs_st, 8);
				move.StartJ = abs_st % 8;
				move.FinishI = i;
				move.FinishJ = j;

				//Создать тестовое поле копированием текущего поля
				Field* testedField = new Field(field);
				//вычислить на нем позицию
				int oldPosition = testedField->CalcPosition(abs_st);

				//сделать на нем ход
				testedField->SelectFigure(move.StartI, move.StartJ);
				bool moveResult = testedField->Move(move.FinishI, move.FinishJ);
				if (!moveResult)
				{
					//если сделать ход не получилось - не добавлять этот ход в очередь
					//удалить тестовое поле и продолжить искать другие ходы
					delete testedField;
					continue;
				}
				
				//посчитать его эффективность
				int newPosition = testedField->CalcPosition(abs_st);

				move.efficiency =  newPosition - oldPosition;
				//Добавить случайную маленькую величину, чтобы при одинаковых эффективностях 
				//выбирался случайный ход. Случайный ход выбирается благодаря сортировке
				move.efficiency += (double)rand() / (RAND_MAX-1);

				//Пересчитать эффективность, если атакуемая фигура защищена
				AttackCorrection(testedField, &move, figure);

				//удалить тестовое поле
				delete testedField;

				//Положить ход в очередь
				Moves.push_back(move);
			}
		}
	}
}

void AI::AttackCorrection(Field* field,FigureMove * move, Figure * figure)
{
	//Сгенерировать карту ходов противника на тестовом поле
	field->GetAllAttackMap(Figure::InverseColor(figure->state));

	//Если это поле атакуется
	if (field->Moves[move->FinishI][move->FinishJ])
	{
		//Вычесть силу фигуры из эффективности
		move->efficiency -= figure->FigureType;
	}
}


void AI::CreateMoveList(Field* field, int state)
{

	if (field->Checkmate)
		return;
	Moves.clear();
	//Собрать список всех возможных ходов

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
	// сканирование файла для сравнения идентичной строки-идентификатора
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
				//при обнаружении идентичной строки мы записываем уже просчитаный ход в буферный ход
				Readed_move.StartI = chec_cr[cou]-97;
				Readed_move.StartJ = chec_cr[cou + 1] - 97;
				Readed_move.FinishI = chec_cr[cou + 2] - 97;
				Readed_move.FinishJ = chec_cr[cou + 3] - 97;
				//флаг о том что решение найдено
				flag_res = true;
				fin.close();
				//флаг об источнике решения(в данном случае источник-файл)
				source = true;
				return;
			}
		}
		else {
			break;
		}
	} while (!fin.eof());
	fin.close();
	//флаг об источнике решения(в данном случае в файле решения нет)
	source= false;
}

void AI::precount(Field *field, bool* signal)
{
	Field* prec_field = new Field(field);

	prec_field->Check = false;
	prec_field->Checkmate = false;
	CreateMoveList(prec_field, -1);

	//Это мат
	if (Moves.size() == 0)
		return ;


	//Отсортировать очередь 
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
					//выход если мат
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
			//выход, если игрок совершает ход
			delete prec_field;
			delete pre_ai;
			return;
		}
	}
	//выход, если совершили все расчёты
	delete prec_field;
	delete pre_ai;
	return;
}

void AI::precount_ini(Field* field, bool *signal)
{
	thread precounting(&AI::precount, this, field, signal);
	precounting.detach();
}


