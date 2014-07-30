// ��Ŀ �ĺ��� ǥ���ϴ� Ŭ����
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
    double damage; // �� ������ ���� ���� �ѷ�(�ʱⰪ 0)

public:
    explicit Bunker(int x, int y);
    ~Bunker();

    // getter
    inline Point GetPos() const { return pos; }
    inline double GetDamage() const { return damage; }

    // ��� �ܰ� ���༱���κ��� �� ������ ���� �������� �� �� ���
    void CalcDamage(const vector<shared_ptr<Spaceship>>& spaceshipVect, World* world);
};

