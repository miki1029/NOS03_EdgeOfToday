#include "Bitmap.h"
#include "World.h"
#include "Timer.h"

using namespace std;

bool runState = true;

int main(void)
{
    clock_t startTime = clock();

    Timer timer;
    timer.Start(chrono::milliseconds(29900));

    Bitmap map("world.bmp");
    World world(map, "input.txt");
    world.CalcBunkerDamage();
    world.SaveData("output.txt");

    timer.Stop();

    clock_t endTime = clock();
    std::cout << "수행시간: " << (endTime - startTime) << std::endl;

    return 0;
}