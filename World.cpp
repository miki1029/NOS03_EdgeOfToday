#include <fstream>
#include <chrono>
#include <thread>
#include "World.h"

World::World(const Bitmap& bm, const char* filename)
: map(bm), groundLowY(0), groundHighY(0)
{
    if (filename)
    {
        LoadData(filename);
        CalcGroundLowHigh();
    }
}


World::~World()
{
}

void World::LoadData(const char* filename)
{
    ifstream inStream;
    inStream.open(filename);

    // Spaceship 초기화
    int numOfSpaceship = 0;
    inStream >> numOfSpaceship;
    for (int i = 0; i < numOfSpaceship; i++)
    {
        int spaceX, spaceY;
        double spaceEnergy;
        inStream >> spaceX >> spaceY >> spaceEnergy;
        spaceshipVect.push_back(make_shared<Spaceship>(spaceX, spaceY, spaceEnergy));
    }

    // Bunker 초기화
    int numOfBunker = 0;
    inStream >> numOfBunker;
    for (int i = 0; i < numOfBunker; i++)
    {
        int bunkX, bunkY;
        inStream >> bunkX >> bunkY;
        bunkerVect.push_back(make_shared<Bunker>(bunkX, bunkY));
    }

    inStream.close();
}

void World::SaveData(const char* filename)
{
    ofstream outStream;
    outStream.open(filename);

    outStream << bunkerVect.size() << endl;

    for (shared_ptr<Bunker> bunker : bunkerVect)
    {
        outStream << bunker->GetDamage() << endl;
    }

    outStream.close();
}

/*
// 이미지를 좌상단부터 무식하게 검색하는 방식(개선 전)
void World::CalcGroundLowHigh2()
{
    // y 순회
    for (int y = 0; y < map.GetHeight(); y++)
    {
        bool isGround = false, isAir = false;
        // x 순회
        for (int x = 0; x < map.GetWidth(); x++)
        {
            const RGBQurd color = map.GetPixel(x, y);
            if (color == Black) isGround = true;
            else isAir = true;
            if (isGround && isAir) break; // 대기와 대지가 혼재하는 구간. 더이상 뒷 픽셀을 확인할 필요가 없음
        }
        // 대지를 처음 만난 경우
        if (groundLowY == 0 && isGround)
        {
            groundLowY = y;
        }
        // 혼재구간이 끝난 경우
        if (isGround && !isAir)
        {
            groundHighY = y;
            break;
        }
    }
    //cout << groundLowY << ", " << groundHighY << endl;
}
*/

// 이진 탐색 방식으로 개선함
void World::CalcGroundLowHigh()
{
    int minY = 0, maxY = map.GetHeight(); // 좌표 기준이므로 max가 최저점, min이 최고점임
    int y = 0;
    
    // 1차: 대지와 대기가 혼재하는 구간 탐색
    // y 순회(이진 탐색)
    while (maxY != minY + 1)
    {
        y = (maxY + minY) / 2;
        bool isGround = false, isAir = false;

        // x 순회
        for (int x = 0; x < map.GetWidth(); x++)
        {
            const RGBQurd color = map.GetPixel(x, y);
            if (color == Black) isGround = true;
            else isAir = true;
            if (isGround && isAir) break; // 대기와 대지가 혼재하는 구간. 더이상 뒷 픽셀을 확인할 필요가 없음
        }

        // x 순회 후 판단
        // 대지와 대기 혼재
        if (isGround && isAir)
            break;
        // 대지만 있음: 윗 방향 이진 탐색
        else if (isGround && !isAir)
        {
            maxY = y;
        }
        // 대기만 있음: 아랫 방향 이진 탐색
        else if (!isGround && isAir)
        {
            minY = y;
        }

    }

    // 혼재 구간이 없는 경우
    if (maxY == minY + 1)
    {
        groundLowY = maxY;
        groundHighY = maxY;
    }
    // 혼재 구간을 찾은 경우
    else
    {
        // 쓰레드 사용
        // 2차: 혼재 구간 최고점 탐색(y 최저값)
        thread th1(&FindGroundLowY, minY, y, this);
        // 3차: 혼재 구간 최저점 탐색(y 최고값)
        thread th2(&FindGroundHighY, y, maxY, this);
        th1.join();
        th2.join();

        // 쓰레드를 안쓸 경우
        //FindGroundLowYProc(minY, y);
        //FindGroundHighYProc(y, maxY);
    }

    //cout << groundLowY << ", " << groundHighY << endl;
}

void World::FindGroundLowY(int minY, int maxY, World* world)
{
    world->FindGroundLowYProc(minY, maxY);
}

void World::FindGroundHighY(int minY, int maxY, World* world)
{
    world->FindGroundHighYProc(minY, maxY);
}

void World::FindGroundLowYProc(int minY, int maxY)
{
    // minY: 대기쪽, maxY: 대지쪽
    while (maxY != minY + 1)
    {
        int y = (maxY + minY) / 2;

        bool isGround = false, isAir = false;

        // x 순회
        for (int x = 0; x < map.GetWidth(); x++)
        {
            const RGBQurd color = map.GetPixel(x, y);
            if (color == Black) isGround = true;
            else isAir = true;
            if (isGround && isAir) break; // 대기와 대지가 혼재하는 구간. 더이상 뒷 픽셀을 확인할 필요가 없음
        }

        // x 순회 후 판단
        // 대지와 대기 혼재
        if (isGround && isAir)
        {
            maxY = y;
        }
        // 대기만 있음: 아랫 방향 이진 탐색
        else if (!isGround && isAir)
        {
            minY = y;
        }

    }

    groundLowY = maxY;
}

void World::FindGroundHighYProc(int minY, int maxY)
{
    // minY: 대기쪽, maxY: 대지쪽
    while (maxY != minY + 1)
    {
        int y = (maxY + minY) / 2;

        bool isGround = false, isAir = false;

        // x 순회
        for (int x = 0; x < map.GetWidth(); x++)
        {
            const RGBQurd color = map.GetPixel(x, y);
            if (color == Black) isGround = true;
            else isAir = true;
            if (isGround && isAir) break; // 대기와 대지가 혼재하는 구간. 더이상 뒷 픽셀을 확인할 필요가 없음
        }

        // x 순회 후 판단
        // 대지와 대기 혼재
        if (isGround && isAir)
        {
            minY = y;
        }
        // 대지만 있음: 윗 방향 이진 탐색
        else if (isGround && !isAir)
        {
            maxY = y;
        }

    }

    groundHighY = maxY;
}

void World::CalcBunkerDamage()
{
    // 4 thread
    int bunkerSize = bunkerVect.size();
    int quater = bunkerSize / 4;

    vector<shared_ptr<Bunker>>::iterator bunkItr_th1End = bunkerVect.begin() + quater;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th2End = bunkerVect.begin() + quater * 2;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th3End = bunkerVect.begin() + quater * 3;

    vector<shared_ptr<Bunker>>::iterator bunkItr_th1 = bunkerVect.begin();
    vector<shared_ptr<Bunker>>::iterator bunkItr_th2 = bunkItr_th1End;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th3 = bunkItr_th2End;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th4 = bunkItr_th3End;

    thread th1(&ThreadCalc, bunkItr_th1, bunkItr_th1End, this);
    thread th2(&ThreadCalc, bunkItr_th2, bunkItr_th2End, this);
    thread th3(&ThreadCalc, bunkItr_th3, bunkItr_th3End, this);
    thread th4(&ThreadCalc, bunkItr_th4, bunkerVect.end(), this);
    
    th1.join();
    th2.join();
    th3.join();
    th4.join();

    // 8 thread
    /*int bunkerSize = bunkerVect.size();
    int quater = bunkerSize / 8;

    vector<shared_ptr<Bunker>>::iterator bunkItr_th1End = bunkerVect.begin() + quater;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th2End = bunkerVect.begin() + quater * 2;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th3End = bunkerVect.begin() + quater * 3;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th4End = bunkerVect.begin() + quater * 4;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th5End = bunkerVect.begin() + quater * 5;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th6End = bunkerVect.begin() + quater * 6;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th7End = bunkerVect.begin() + quater * 7;

    vector<shared_ptr<Bunker>>::iterator bunkItr_th1 = bunkerVect.begin();
    vector<shared_ptr<Bunker>>::iterator bunkItr_th2 = bunkItr_th1End;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th3 = bunkItr_th2End;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th4 = bunkItr_th3End;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th5 = bunkItr_th4End;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th6 = bunkItr_th5End;
    vector<shared_ptr<Bunker>>::iterator bunkItr_th7 = bunkItr_th6End;
    vector<shared_ptr<Bunker>>::iterator bunkItr = bunkItr_th7End;

    thread th1(&ThreadCalc, bunkItr_th1, bunkItr_th1End, this);
    thread th2(&ThreadCalc, bunkItr_th2, bunkItr_th2End, this);
    thread th3(&ThreadCalc, bunkItr_th3, bunkItr_th3End, this);
    thread th4(&ThreadCalc, bunkItr_th4, bunkItr_th4End, this);
    thread th5(&ThreadCalc, bunkItr_th5, bunkItr_th5End, this);
    thread th6(&ThreadCalc, bunkItr_th6, bunkItr_th6End, this);
    thread th7(&ThreadCalc, bunkItr_th7, bunkItr_th7End, this);
    ThreadCalc(bunkItr, bunkerVect.end(), this);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();
    th7.join();*/
}

void World::ThreadCalc(vector<shared_ptr<Bunker>>::iterator bunkItr, vector<shared_ptr<Bunker>>::iterator bunkItrEnd, World* world)
{
    world->ThreadCalcProc(bunkItr, bunkItrEnd);
}

void World::ThreadCalcProc(vector<shared_ptr<Bunker>>::iterator bunkItr, vector<shared_ptr<Bunker>>::iterator bunkItrEnd)
{
    // runState: Timer에 의해 30초후 false가 됨 -> 프로그램 종료 준비
    for (; runState && bunkItr != bunkItrEnd; bunkItr++)
    {
        (*bunkItr)->CalcDamage(spaceshipVect, this);
    }
}