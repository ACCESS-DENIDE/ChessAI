#pragma once
#include "Figure.h"
#include "Field.h"

class Queen :
    public Figure
{
public:
    //Конструктор
    Queen(Field* field);

    void FillMap(int i, int j,bool attack);
    void FillAttackMap(int i, int j);
    void FillMovesMap(int i, int j);
};

