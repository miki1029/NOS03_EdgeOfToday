// 전역 영역 헤더
#pragma once
#include <cmath>

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

// 초기화 용도 또는 존재하지 않는 점을 의미함
const Point dummyPoint{ -1, -1 };

template <typename T>
inline void Swap(T& a, T& b)
{
    T tmp;
    tmp = a;
    a = b;
    b = tmp;
}

// runState는 Timer에 의해 값이 변경되며 false가 되면 프로그램이 종료 준비를 함
extern bool runState;