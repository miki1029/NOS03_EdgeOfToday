// 벙커 후보를 표현하는 클래스
#pragma once

#include <memory>
#include <vector>
#include "myGlobal.h"
#include "Spaceship.h"

using namespace std;

class Bunker
{
private:
    const Point pos;
    double damage; // 빔 공격을 받은 피해 총량(초기값 0)

public:
    explicit Bunker(int x, int y);
    ~Bunker();

    // getter
    inline Point GetPos() const { return pos; }
    inline double GetDamage() const { return damage; }

    // 모든 외계 비행선으로부터 빔 공격을 받은 데미지를 총 량 계산
    void CalcDamage(const vector<shared_ptr<Spaceship>>& spaceshipVect, World* world);
};

