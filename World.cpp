#include <fstream>
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
    vector<shared_ptr<Bunker>>::iterator bunkItr = bunkerVect.begin();
    for (; bunkItr != bunkerVect.end(); bunkItr++)
    {
        (*bunkItr)->CalcDamage(spaceshipVect, this);
    }
}