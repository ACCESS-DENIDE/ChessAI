#include "Figure.h"
#include "Field.h"

//Этот метод общий для нескольких фигур, поэтому он в родительском классе
    //метод для проверки каждой ячейки на отмечание ее как доступной для хода
    //возвращает нужно ли выделять дальше
bool Figure::CheckCell(int i, int j, bool attack)
{
    if (_field->CellIsValid(i, j))
    {
        Figure* figure = _field->Figures[i][j];
        //если клетка свободная, то помечаем ее доступной для хода и для атаки
        if (figure == 0)
        {
            _field->Moves[i][j] = true;
            return true;
        }
        //если клетка занята своей фигурой
        if (((figure->state>=0) && (this->state>=0)) || ((figure->state < 0) && (this->state < 0)))
        {
            //Если считаем атаку, то помечаем клетку
            if (attack)
            {
                _field->Moves[i][j] = true;
            }
            //Если считаем поле ходов, то не помечаем клетку

            return false;
        }
        else
        {
            //если клетка занята чужой фигурой, то  помечаем доступной для хода 
            //и дальше уже не считаем
            _field->Moves[i][j] = true;
            return false;
        }
    }
    return false;
}

//Считает карту атак фигуры
//Заполняет все поля массива Moves на которые следующим ходом может атаковать фигура
void Figure::FillAttackMap(int i, int j)
{
        //по умолчанию будет использовать карту ходов
    FillMovesMap(i, j);
}


//Получает все ходы, предварительно очистив ходы, записанные на доске
void Figure::FillMovesMapWithClear(int i, int j)
{
    _field->ClearMovesMap();
    FillMovesMap(i, j);
}

//Возвращает обратный цвет
int Figure::InverseColor(int state)
{
    if (state >=0)
    {
        return -1;
    }
    else
    {
        return 1;
    }

}