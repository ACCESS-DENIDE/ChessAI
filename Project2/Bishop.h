#pragma once
#include "Figure.h"
#include "Field.h"

class Bishop :
    public Figure
{
public:
    //Конструктор
    Bishop(Field* field);

    void FillMap(int i, int j,bool attack);

    void FillMovesMap(int i, int j);
    void FillAttackMap(int i, int j);
   
};

