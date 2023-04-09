#pragma once
#include "Figure.h"
#include "Field.h"

class Knight :
    public Figure
{
public:
    //Конструктор
    Knight(Field* field);

    void FillMap(int i, int j, bool attack);

    void FillAttackMap(int i, int j);

    //
    void FillMovesMap(int i, int j);
};

