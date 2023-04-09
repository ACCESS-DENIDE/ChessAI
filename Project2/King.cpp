#include "King.h"

King::King(Field* field)
{
    FigureType = FigureTypes::king;
    _field = field;
}

void King::FillMovesMap(int i, int j)
{
    //проверить все клетки вокруг корол€
    for (int x = i - 1; x <= i + 1; x++)
        for (int y = j - 1; y <= j + 1; y++)
        {
            CheckCell(x, y,false);
        }

    //–окировка
    if (state < 0)
    {
        // ороль на месте
        if (i == 7 && j == 4)
        {
            //≈сли справа до ладьи пустые клетки
            if (_field->Figures[7][6] == nullptr && _field->Figures[7][5] == nullptr)
            {
                //≈сли скраю пол€ стоит ладь€
                if (_field->Figures[7][7] != nullptr && _field->Figures[7][7]->FigureType == FigureTypes::rock)
                {
                    _field->Moves[7][6] = true;
                }

            }
            //≈сли слева до ладьи пустые клетки
            if (_field->Figures[7][3] == nullptr
                && _field->Figures[7][2] == nullptr
                && _field->Figures[7][1] == nullptr)
            {
                //≈сли скраю пол€ стоит ладь€
                if (_field->Figures[7][0] != nullptr 
                    && _field->Figures[7][0]->FigureType == FigureTypes::rock)
                {
                    _field->Moves[7][2] = true;
                }
            }
        }
    }
    else
    {
        // ороль на месте
        if (i == 0 && j == 4)
        {
            //≈сли справа до ладьи пустые клетки
            if (_field->Figures[0][6] == nullptr && _field->Figures[0][5] == nullptr)
            {
                //≈сли скраю пол€ стоит ладь€
                if (_field->Figures[0][7] != nullptr && _field->Figures[0][7]->FigureType == FigureTypes::rock)
                {
                    _field->Moves[0][6] = true;
                }

            }
            //≈сли слева до ладьи пустые клетки
            if (_field->Figures[0][3] == nullptr
                && _field->Figures[0][2] == nullptr
                && _field->Figures[0][1] == nullptr)
            {
                //≈сли скраю пол€ стоит ладь€
                if (_field->Figures[0][0] != nullptr 
                    && _field->Figures[0][0]->FigureType == FigureTypes::rock)
                {
                    _field->Moves[0][2] = true;
                }
            }
        }
    }
}

//ѕолучает все ходы, предварительно очистив ходы, записанные на доске
//≈сли вызван этот метод дл€ корол€, то он вычисл€ет все возможные ходы корол€ с учетом шахов
void King::FillMovesMapWithClear(int i, int j)
{
    //ѕолучить карту всех ходов всех фигур противоположного цвета
    _field->GetAllAttackMap(Figure::InverseColor(state));

    //булев массив возможных ходов 
    //Ќужен чтобы сохранить возможные ходы, вычисленные ранее
    bool Moves[8][8];

    // опирую массив возможных ходов в свой собственный
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Moves[i][j] = _field->Moves[i][j];
        }
    }

    //„ищу массив ходов, ведь его копи€ уже есть 
    _field->ClearMovesMap();

    //ѕолучаю все возможные ходы корол€
    FillMovesMap(i, j);

    //≈сли возможный ход корол€ совпадает с возможным ходом фигуры противника
    //“о убираю этот ход корол€
    //ставлю false везде, где карта ходов корол€ и противоположной команды совпадают
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (Moves[i][j] == _field->Moves[i][j])
                _field->Moves[i][j] = false;
        }
    }

}

//—читает карту атак фигуры
//«аполн€ет все пол€ массива Moves на которые следующим ходом может атаковать фигура
//Ётот метод нужен только чтобы определить возможные ходы корол€
void King::FillAttackMap(int i, int j)
{
    //проверить все клетки вокруг корол€
    for (int x = i - 1; x <= i + 1; x++)
        for (int y = j - 1; y <= j + 1; y++)
        {
            CheckCell(x, y, true);
        }
    //исключить себ€
    _field->Moves[i][j] = false;

   
}