#pragma once
#include "Figure.h"
#include "Field.h"

class Rock :
    public Figure
{
public:
    //Конструктор
    Rock(Field* field);
    
    void FillMap(int i, int j, bool attack);
    void FillAttackMap(int i, int j);
    void FillMovesMap(int i, int j);
   
};

