#pragma once

#include <vector>
#include <memory>
#include "defStruct.h"
#include "Bitmap.h"
#include "Spaceship.h"
#include "Bunker.h"

using namespace std;

class World
{
private:
    const Bitmap& map;
    
    // low, high: 좌표값 기준
    int groundLowY;     // 대지의 최고점
    int groundHighY;    // 대지의 최저점

    vector<shared_ptr<Spaceship>> spaceshipVect;
    vector<shared_ptr<Bunker>> bunkerVect;

public:
    explicit World(const Bitmap& bm, const char* filename);
    ~World();

    inline const Bitmap& GetMap() { return map; }
    inline int GetGroundLowY() { return groundLowY; }
    inline int GetGroundHighY() { return groundHighY; }

    void LoadData(const char* filename);
    void SaveData(const char* filename);
    void CalcGroundLowHigh();

    void CalcBunkerDamage();

private:
    static void ThreadCalc(vector<shared_ptr<Bunker>>::iterator bunkItr, vector<shared_ptr<Bunker>>::iterator bunkItrEnd, World* world);
    void ThreadCalcProc(vector<shared_ptr<Bunker>>::iterator bunkItr, vector<shared_ptr<Bunker>>::iterator bunkItrEnd);
};

