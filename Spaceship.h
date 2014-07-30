#pragma once

#include "defStruct.h"
class Bunker;
class World;

class Spaceship
{
private:
    const Point pos;
    double energy;

public:
    Spaceship(int x, int y, double e);
    ~Spaceship();

    inline Point GetPos() const { return pos; }
    inline double GetEnergy() const { return energy; }
    inline double XEquation(double rm, int y) { return rm * (y - pos.y) + pos.x; }

    Point FindContactPoint(Bunker* bunker, World* world);
};