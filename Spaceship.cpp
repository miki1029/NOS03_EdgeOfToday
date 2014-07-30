#include "Spaceship.h"
#include "Bunker.h"
#include "World.h"

Spaceship::Spaceship(int x, int y, double e)
: pos({ x, y }), energy(e)
{
}


Spaceship::~Spaceship()
{
}

Point Spaceship::FindContactPoint(Bunker* bunker, World* world)
{
    // ����
    double m = static_cast<double>(bunker->GetPos().y - pos.y) / (bunker->GetPos().x - pos.x);
    double rm = static_cast<double>(bunker->GetPos().x - pos.x) / (bunker->GetPos().y - pos.y);

    // low point
    int y1 = world->GetGroundLowY();
    int x1 = static_cast<int>(XEquation(rm, y1)); // ����.. ���� �߻�

    // high point
    int y2 = world->GetGroundHighY();
    int x2 = static_cast<int>(XEquation(rm, y2)); // ����.. ���� �߻�

    // ���� ������ ���� ������
    Point contactPoint = dummyPoint; // �ʱⰪ�̸鼭 �� ���� �����Ǹ� �������
    bool find = false;

    // x���� ������
    if (-1 < m && m < 1)
    {
        if (x1 > x2)
        {
            Swap(x1, x2);
            Swap(y1, y2);
        }
        double y = static_cast<double>(y1); // �ʱⰪ
        // ���� Ž��
        for (int x = x1; x <= x2; x++)
        {
            RGBQurd color = world->GetMap().GetPixel(x, static_cast<int>(y));
            if (!find && color == Black)
            {
                find = true;
                contactPoint = Point{ x, static_cast<int>(y) };
            }
            else if (find && color != Black)
            {
                contactPoint = dummyPoint;
                break;
            }
            y += m;
        }
    }
    // y���� ������
    else
    {
        /* y1�� �׻� ����
        if (y1 > y2)
        {
            Swap(x1, x2);
            Swap(y1, y2);
        }*/
        double x = static_cast<double>(x1); // �ʱⰪ
        // ���� Ž��
        for (int y = y1; y <= y2; y++)
        {
            RGBQurd color = world->GetMap().GetPixel(static_cast<int>(x), y);
            if (!find && color == Black)
            {
                find = true;
                contactPoint = Point{ static_cast<int>(x), y };
            }
            else if (find && color != Black)
            {
                contactPoint = dummyPoint;
                break;
            }
            x += rm;
        }
    }

    return contactPoint;
}