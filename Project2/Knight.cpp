#include "Knight.h"

Knight::Knight(Field* field)
{
    FigureType = FigureTypes::knight;
    _field = field;
}

void Knight::FillMap(int i, int j, bool attack)
{
    int x;
    int y;

    x = i + 2;
    y = j + 1;
    CheckCell(x, y, attack);

    x = i - 2;
    y = j + 1;
    CheckCell(x, y, attack);

    x = i + 2;
    y = j - 1;
    CheckCell(x, y, attack);

    x = i - 2;
    y = j - 1;
    CheckCell(x, y, attack);

    x = i + 1;
    y = j + 2;
    CheckCell(x, y, attack);

    x = i - 1;
    y = j + 2;
    CheckCell(x, y, attack);

    x = i + 1;
    y = j - 2;
    CheckCell(x, y, attack);

    x = i - 1;
    y = j - 2;
    CheckCell(x, y, attack);
}

void Knight::FillAttackMap(int i, int j)
{
    FillMap(i, j, true);
}


void Knight::FillMovesMap(int i, int j)
{
    FillMap(i, j, false);
}