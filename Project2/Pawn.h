#pragma once
#include "Field.h"
#include "Figure.h"


class Pawn :
    public Figure
{
public:
    //Конструктор
    Pawn(Field* field);

    //Считает карту ходов фигуры
    void FillMovesMap(int i, int j);

    //Считает карту атак фигуры
//Заполняет все поля массива Moves на которые следующим ходом может атаковать фигура
//Этот метод нужен только чтобы определить возможные ходы короля
    void FillAttackMap(int i, int j);
    
};

