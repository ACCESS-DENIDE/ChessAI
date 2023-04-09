#pragma once
#include "Figure.h"
#include "Field.h"

class King :
    public Figure
{
private:
    //В этом массиве собираюся все возможные ходы противника, 
    //чтобы можно было вычислить возможные ходы короля
    int Moves[8][8];
public:
    //Конструктор
    King(Field* field);

    void FillMovesMap(int i, int j);

    void FillMovesMapWithClear(int i, int j);

    void FillAttackMap(int i, int j);
};

