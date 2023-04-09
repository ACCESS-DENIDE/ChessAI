#pragma once
#include "Figure.h"

//Класс игровое поле
class Field
{
public:
	///Во всех массивах первое число - номера строк
	///					второе читло - номера столбцов
	///внизу всегда белые - Белые стартуют из строк 6-7, черные стартуют из строк 0-1

	///так же для понятности буду использовать индекс i для строк
	///												  j для столбцов

	//Для облегчения доступа к королям
	Figure* WhiteKing = nullptr;
	Figure* BlackKing = nullptr;


	//массив фигур
	Figure* Figures[8][8];
	//булев массив возможных ходов
	bool Moves[8][8];

	//Выбранная фигура
	Figure* SelectedFigure = 0;


	//Какой цвет сейчас ходит
	int CurrentMoveColor = -1;

	//Состояния игры
	bool Check = false;//Шах
	bool Checkmate = false;//мат


	//Конструктор класса
	//здесь же происходит расстановка фигур
	Field();

	//Конструктор копирования
	//Создает копию текущей доски
	Field(Field*field);

	//деструктор
	~Field();

	//проверка действительно ли это поле есть на доске
	bool CellIsValid(int i, int j);

	//Очистка ходов
	void ClearMovesMap(); 

	//Выбрать фигуру
	bool SelectFigure(int i, int j);

	//Сделать ход выделенной фигурой
	bool Move(int i, int j, bool withoutCheckmate = false);

	//посмотреть все возможные ходы одного цвета
	void GetAllAttackMap(int State);

	//Проверка на шах
	//Для заданного цвета
	//Фигуры заданного цвета поставили шах 
	bool CheckTest(int State);

	//Копировать все фигуры и состояние
	void CopyToMe(Field* field);

	//ПРоверка на мат для заданного цвета
	//фигуры заданного цвета поставили мат
	bool CheckmateTest(int color);

	//Проверить все ходы фигуры и вернуть True, если после любого хода будет шах
	bool CheckAllMovesFigure(Figure*figure,int color);

	//Подсчитать позицию заданного цвета на доске
	int CalcPosition(int color);
};

