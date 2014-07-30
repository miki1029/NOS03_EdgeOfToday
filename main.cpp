#include "Bitmap.h"
#include "World.h"

using namespace std;

clock_t startTime = clock();
bool runState = true;

int main(void)
{

    Bitmap map("world.bmp");

    World world(map, "input.txt");

    world.CalcBunkerDamage();

    world.SaveData("output.txt");

    clock_t endTime = clock();
    std::cout << "����ð�: " << (endTime - startTime) << std::endl;

    return 0;
}