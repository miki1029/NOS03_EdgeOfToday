// �ùķ��̼� World�� ��Ÿ���� Ŭ����
// ��Ʈ�� �̹���, �ܰ� ���༱�� ����, ��Ŀ �ĺ��� ������ ���ԵǾ�����
#pragma once

#include <vector>
#include <memory>
#include "myGlobal.h"
#include "Bitmap.h"
#include "Spaceship.h"
#include "Bunker.h"

using namespace std;

class World
{
private:
    const Bitmap& map;
    
    // low, high: ��ǥ�� ����
    // ���� ������ ȥ���ϴ� ����(BetweenZone)�� Y��ǥ�� ǥ���մϴ�.
    int groundLowY;     // ������ �ְ���(y�� ����)
    int groundHighY;    // ������ ������(y�� �ְ�)

    vector<shared_ptr<Spaceship>> spaceshipVect;
    vector<shared_ptr<Bunker>> bunkerVect;

public:
    explicit World(const Bitmap& bm, const char* filename);
    ~World();

    // getter
    inline const Bitmap& GetMap() { return map; }
    inline int GetGroundLowY() { return groundLowY; }
    inline int GetGroundHighY() { return groundHighY; }

    // ���� �����, �ʱ�ȭ ����
    void LoadData(const char* filename);
    void SaveData(const char* filename);
    void CalcGroundLowHigh(); // groundLowY, groundHighY�� ���� �����

    // �� ��Ŀ�� ���ط� ���
    void CalcBunkerDamage();

private:
    // ��Ŀ�� ���ط� ���� ������ Ȱ��
    static void ThreadCalc(vector<shared_ptr<Bunker>>::iterator bunkItr, vector<shared_ptr<Bunker>>::iterator bunkItrEnd, World* world);
    void ThreadCalcProc(vector<shared_ptr<Bunker>>::iterator bunkItr, vector<shared_ptr<Bunker>>::iterator bunkItrEnd);
};

