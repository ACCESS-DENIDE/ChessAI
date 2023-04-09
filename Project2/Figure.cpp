#include "Figure.h"
#include "Field.h"

//���� ����� ����� ��� ���������� �����, ������� �� � ������������ ������
    //����� ��� �������� ������ ������ �� ��������� �� ��� ��������� ��� ����
    //���������� ����� �� �������� ������
bool Figure::CheckCell(int i, int j, bool attack)
{
    if (_field->CellIsValid(i, j))
    {
        Figure* figure = _field->Figures[i][j];
        //���� ������ ���������, �� �������� �� ��������� ��� ���� � ��� �����
        if (figure == 0)
        {
            _field->Moves[i][j] = true;
            return true;
        }
        //���� ������ ������ ����� �������
        if (((figure->state>=0) && (this->state>=0)) || ((figure->state < 0) && (this->state < 0)))
        {
            //���� ������� �����, �� �������� ������
            if (attack)
            {
                _field->Moves[i][j] = true;
            }
            //���� ������� ���� �����, �� �� �������� ������

            return false;
        }
        else
        {
            //���� ������ ������ ����� �������, ��  �������� ��������� ��� ���� 
            //� ������ ��� �� �������
            _field->Moves[i][j] = true;
            return false;
        }
    }
    return false;
}

//������� ����� ���� ������
//��������� ��� ���� ������� Moves �� ������� ��������� ����� ����� ��������� ������
void Figure::FillAttackMap(int i, int j)
{
        //�� ��������� ����� ������������ ����� �����
    FillMovesMap(i, j);
}


//�������� ��� ����, �������������� ������� ����, ���������� �� �����
void Figure::FillMovesMapWithClear(int i, int j)
{
    _field->ClearMovesMap();
    FillMovesMap(i, j);
}

//���������� �������� ����
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