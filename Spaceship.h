// 외계 비행선을 나타내는 클래스
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
    // 직선 방정식
    inline double XEquation(double rm, int y) { return rm * (y - pos.y) + pos.x; }
    inline double YEquation(double m, int x) { return m * (x - pos.x) + pos.y; }

    // bunker로 쏘는 빔과 대지의 첫 접점을 구함
    // 산란되었으면 dummyPoint를 반환
    Point FindContactPoint(Bunker* bunker, World* world);
};