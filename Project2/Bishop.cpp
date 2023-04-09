#include "Bishop.h"

Bishop::Bishop(Field* field)
{
    FigureType = FigureTypes::bishop;
    _field = field;
}


void Bishop::FillAttackMap(int i, int j)
{
    FillMap(i, j, true);
}

void Bishop::FillMap(int i, int j,bool attack)
{
    for (int x = i + 1, y = j + 1; x < 8; x++, y++)
    {
        if (CheckCell(x, y, attack))
            continue;
        else
            break;
    }
    for (int x = i - 1, y = j + 1; x < 8; x--, y++)
    {
        if (CheckCell(x, y, attack))
            continue;
        else
            break;
    }
    for (int x = i + 1, y = j - 1; x < 8; x++, y--)
    {
        if (CheckCell(x, y, attack))
            continue;
        else
            break;
    }
    for (int x = i - 1, y = j - 1; x < 8; x--, y--)
    {
        if (CheckCell(x, y, attack))
            continue;
        else
            break;
    }
}


void Bishop::FillMovesMap(int i, int j)
{
    FillMap(i, j, false);
}