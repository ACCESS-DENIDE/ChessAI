#include "Pawn.h"

Pawn::Pawn(Field* field)
{
    FigureType = FigureTypes::pawn;
    _field = field;
}
void Pawn::FillMovesMap(int i, int j)
{
    //если пешка белая
    if (state <0)
    {
        if (_field->CellIsValid(i - 1, j))
            if (_field->Figures[i - 1][j] == nullptr)
            {
                _field->Moves[i - 1][j] = true;
                if (i == 6)//если пешка на том месте, где была в начале партии
                {
                    //то может пойти на 2 клетки вперед
                    if (_field->CellIsValid(i - 2, j))
                        if (_field->Figures[i - 2][j] == nullptr)
                            _field->Moves[i - 2][j] = true;
                }
            }
        //атака влево
        if (_field->CellIsValid(i - 1, j - 1))
            if (_field->Figures[i - 1][j - 1] != nullptr)
                if (_field->Figures[i - 1][j - 1]->state>=0 )
                    _field->Moves[i - 1][j - 1] = true;
        //атака вправо
        if (_field->CellIsValid(i - 1, j + 1))
            if (_field->Figures[i - 1][j + 1] != nullptr)
                if (_field->Figures[i - 1][j + 1]->state >=0)
                    _field->Moves[i - 1][j + 1] = true;
    }
    //если пешка черная
    if (state >= 0)
    {
        if (_field->CellIsValid(i + 1, j))
            if (_field->Figures[i + 1][j] == nullptr)
            {
                _field->Moves[i + 1][j] = true;
                if (i == 1)//если пешка на том месте, где была в начале партии
                {
                    //то может пойти на 2 клетки вперед
                    if (_field->CellIsValid(i + 2, j))
                        if (_field->Figures[i + 2][j] == nullptr)
                            _field->Moves[i + 2][j] = true;
                }
            }
        //атака влево
        if (_field->CellIsValid(i + 1, j + 1))
            if (_field->Figures[i + 1][j + 1] != nullptr)
                if (_field->Figures[i + 1][j + 1]->state <0 )
                    _field->Moves[i + 1][j + 1] = true;
        //атака вправо
        if (_field->CellIsValid(i + 1, j - 1))
            if (_field->Figures[i + 1][j - 1] != nullptr)
                if (_field->Figures[i + 1][j - 1]->state < 0)
                    _field->Moves[i + 1][j - 1] = true;
    }
}

//Считает карту атак фигуры
//Заполняет все поля массива Moves на которые следующим ходом может атаковать фигура
//Этот метод нужен только чтобы определить возможные ходы короля
void Pawn::FillAttackMap(int i, int j)
{
    //если пешка белая
    if (state < 0)
    {

        //атака влево
        if (_field->CellIsValid(i - 1, j - 1))
            _field->Moves[i - 1][j - 1] = true;
        //атака вправо
        if (_field->CellIsValid(i - 1, j + 1))
            _field->Moves[i - 1][j + 1] = true;
    }
    //если пешка черная
    if (state>=0)
    {

        //атака влево
        if (_field->CellIsValid(i + 1, j + 1))
            _field->Moves[i + 1][j + 1] = true;
        //атака вправо
        if (_field->CellIsValid(i + 1, j - 1))
            _field->Moves[i + 1][j - 1] = true;
    }
}