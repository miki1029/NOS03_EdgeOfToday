#pragma once
#include <cmath>
#include <ctime>

struct Point
{
    int x;
    int y;

    inline bool operator==(Point arg) const
    {
        if (x == arg.x && y == arg.y)
            return true;
        else
            return false;
    }

    inline bool operator!=(Point arg) const
    {
        return !operator==(arg);
    }

    inline double CalcDistance(Point arg) const
    {
        return sqrt(pow(arg.x - x, 2) + pow(arg.y - y, 2));
    }
};

const Point dummyPoint{ -1, -1 };

template <typename T>
inline void Swap(T& a, T& b)
{
    T tmp;
    tmp = a;
    a = b;
    b = tmp;
}

extern bool runState;