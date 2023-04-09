#include "Queen.h"

Queen::Queen(Field* field)
{
    FigureType = FigureTypes::queen;
    _field = field;
}


void Queen::FillMap(int i, int j, bool attack)
{
    //Вниз
    for (int x = i + 1; x < 8; x++)
    {
        if (CheckCell(x, j,attack))
            continue;
        else
            break;
    }
    //Вверх
    for (int x = i - 1; x >= 0; x--)
    {
        if (CheckCell(x, j, attack))
            continue;
        else
            break;
    }
    //Вправо
    for (int x = j + 1; x < 8; x++)
    {
        if (CheckCell(i, x, attack))
            continue;
        else
            break;

    }
    //Влево
    for (int x = j - 1; x >= 0; x--)
    {
        if (CheckCell(i, x, attack))
            continue;
        else
            break;
    }

    //А это по диагоналям
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
void Queen::FillAttackMap(int i, int j)
{
    FillMap(i, j, true);
}

void Queen::FillMovesMap(int i, int j)
{
    FillMap(i, j, false);
}