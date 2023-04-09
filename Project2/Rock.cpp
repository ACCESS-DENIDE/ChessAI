#include "Rock.h"

Rock::Rock(Field* field)
{
    FigureType = FigureTypes::rock;
    _field = field;
}

void Rock::FillMap(int i, int j, bool attack)
{
    //¬низ
    for (int x = i + 1; x < 8; x++)
    {
        if (CheckCell(x, j, attack))
            continue;
        else
            break;
    }
    //¬верх
    for (int x = i - 1; x >= 0; x--)
    {
        if (CheckCell(x, j, attack))
            continue;
        else
            break;
    }
    //¬право
    for (int x = j + 1; x < 8; x++)
    {
        if (CheckCell(i, x, attack))
            continue;
        else
            break;

    }
    //¬лево
    for (int x = j - 1; x >= 0; x--)
    {
        if (CheckCell(i, x, attack))
            continue;
        else
            break;
    }
}
void Rock::FillAttackMap(int i, int j)
{
    FillMap(i, j, true);
}

void Rock::FillMovesMap(int i, int j)
{
    FillMap(i, j, false);
}