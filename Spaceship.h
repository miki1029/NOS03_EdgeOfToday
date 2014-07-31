// �ܰ� ���༱�� ��Ÿ���� Ŭ����
#pragma once

#include "myGlobal.h"
class Bunker;
class World;

class Spaceship
{
private:
    const Point pos;
    const double energy;

public:
    explicit Spaceship(int x, int y, double e);
    ~Spaceship();

    // getter
    inline Point GetPos() const { return pos; }
    inline double GetEnergy() const { return energy; }
    // ���� ������
    inline double XEquation(double rm, int y) { return rm * (y - pos.y) + pos.x; }
    inline double YEquation(double m, int x) { return m * (x - pos.x) + pos.y; }

    // bunker�� ��� ���� ������ ù ������ ����
    // ����Ǿ����� dummyPoint�� ��ȯ
    Point FindContactPoint(Bunker* bunker, World* world);
};