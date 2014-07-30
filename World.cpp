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

void World::CalcGroundLowHigh()
{
    for (int y = 0; y < map.GetHeight(); y++)
    {
        bool isGround = false, isAir = false;
        for (int x = 0; x < map.GetWidth(); x++)
        {
            const RGBQurd color = map.GetPixel(x, y);
            if (color == Black) isGround = true;
            else isAir = true;
            if (isGround && isAir) break;
        }
        if (groundLowY == 0 && isGround)
        {
            groundLowY = y;
        }
        if (isGround && !isAir)
        {
            groundHighY = y;
            break;
        }
    }
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
    for (; bunkItr != bunkItrEnd; bunkItr++)
    {
        if (!(*bunkItr)->CalcDamage(spaceshipVect, this)) return;
    }
}