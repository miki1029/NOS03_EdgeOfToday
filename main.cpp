#include "Bitmap.h"
#include "World.h"

using namespace std;

int main(void)
{
    clock_t begin, end;
    begin = clock();
    Bitmap map("world.bmp");

    World world(map, "input.txt");

    world.CalcBunkerDamage();

    world.SaveData("output.txt");

    end = clock();
    std::cout << "수행시간: " << (end - begin) / CLOCKS_PER_SEC << std::endl;

    return 0;
}