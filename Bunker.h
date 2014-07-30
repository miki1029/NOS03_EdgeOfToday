#pragma once

#include <memory>
#include <vector>
#include "defStruct.h"
#include "Spaceship.h"

using namespace std;

class Bunker
{
private:
    const Point pos;
    double damage;

public:
    Bunker(int x, int y);
    ~Bunker();

    inline Point GetPos() const { return pos; }
    inline double GetDamage() const { return damage; }

    void CalcDamage(const vector<shared_ptr<Spaceship>>& spaceshipVect, World* world);
};

