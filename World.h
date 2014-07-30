// 시뮬레이션 World를 나타내는 클래스
// 비트맵 이미지, 외계 비행선의 정보, 벙커 후보의 정보가 포함되어있음
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
    
    // low, high: 좌표값 기준
    // 대기와 대지가 혼재하는 구간(BetweenZone)을 Y좌표로 표현합니다.
    int groundLowY;     // 대지의 최고점(y값 최저)
    int groundHighY;    // 대지의 최저점(y값 최고)

    vector<shared_ptr<Spaceship>> spaceshipVect;
    vector<shared_ptr<Bunker>> bunkerVect;

public:
    explicit World(const Bitmap& bm, const char* filename);
    ~World();

    // getter
    inline const Bitmap& GetMap() { return map; }
    inline int GetGroundLowY() { return groundLowY; }
    inline int GetGroundHighY() { return groundHighY; }

    // 파입 입출력, 초기화 관련
    void LoadData(const char* filename);
    void SaveData(const char* filename);
    void CalcGroundLowHigh(); // groundLowY, groundHighY의 값을 계산함

    // 각 벙커의 피해량 계산
    void CalcBunkerDamage();

private:
    // 벙커의 피해량 계산시 스레드 활용
    static void ThreadCalc(vector<shared_ptr<Bunker>>::iterator bunkItr, vector<shared_ptr<Bunker>>::iterator bunkItrEnd, World* world);
    void ThreadCalcProc(vector<shared_ptr<Bunker>>::iterator bunkItr, vector<shared_ptr<Bunker>>::iterator bunkItrEnd);
};

