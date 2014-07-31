#include "Spaceship.h"
#include "Bunker.h"
#include "World.h"

Spaceship::Spaceship(int x, int y, double e)
: pos({ x, y }), energy(e)
{
}


Spaceship::~Spaceship()
{
}

Point Spaceship::FindContactPoint(Bunker* bunker, World* world)
{
    // 기울기
    double m = static_cast<double>(bunker->GetPos().y - pos.y) / (bunker->GetPos().x - pos.x);
    double rm = static_cast<double>(bunker->GetPos().x - pos.x) / (bunker->GetPos().y - pos.y);

    // low point(그림 상 위)
    int y1 = world->GetGroundLowY();
    int x1 = static_cast<int>(XEquation(rm, y1)); // 버림.. 오차 발생

    // high point(그림 상 아래)
    int y2 = world->GetGroundHighY();
    int x2 = static_cast<int>(XEquation(rm, y2)); // 버림.. 오차 발생

    // 대지 접점을 위한 변수들
    Point contactPoint = dummyPoint; // 초기값이면서 이 값이 유지되면 산란상태
    bool find = false; // 대지를 찾는 순간 true가 됨

    // x축이 독립축
    if (-1 < m && m < 1)
    {
        // 외계 비행선이 좌측, 벙커가 우측
        if (x1 < x2)
        {
            double y = YEquation(m, x1); // 초기값
            // 직선 탐색
            for (int x = x1; x <= x2; x++)
            {
                RGBQurd color = world->GetMap().GetPixel(x, static_cast<int>(y));
                // 첫 접점을 찾은 경우
                if (!find && color == Black)
                {
                    find = true;
                    contactPoint = Point{ x, static_cast<int>(y) };
                }
                // 접점을 찾았는데 대기를 또 만나는 경우->산란
                else if (find && color != Black)
                {
                    contactPoint = dummyPoint;
                    break;
                }
                y += m;
            }
        }
        // 외계 비행선이 우측, 벙커가 좌측
        else
        {
            double y = YEquation(m, x1) + 1; // 초기값(+1은 오차 최소화)
            // 직선 탐색
            for (int x = x1; x >= x2; x--)
            {
                RGBQurd color = world->GetMap().GetPixel(x, static_cast<int>(y));
                // 첫 접점을 찾은 경우
                if (!find && color == Black)
                {
                    find = true;
                    contactPoint = Point{ x, static_cast<int>(y) };
                }
                // 접점을 찾았는데 대기를 또 만나는 경우->산란
                else if (find && color != Black)
                {
                    contactPoint = dummyPoint;
                    break;
                }
                y -= m;
            }
        }
    }
    // y축이 독립축
    else
    {
        // y1이 항상 작음
        double x = XEquation(rm, y1); // 초기값
        // 직선 탐색
        for (int y = y1; y <= y2; y++)
        {
            RGBQurd color = world->GetMap().GetPixel(static_cast<int>(x), y);
            // 첫 접점을 찾은 경우
            if (!find && color == Black)
            {
                find = true;
                contactPoint = Point{ static_cast<int>(x), y };
            }
            // 접점을 찾았는데 대기를 또 만나는 경우->산란
            else if (find && color != Black)
            {
                contactPoint = dummyPoint;
                break;
            }
            x += rm;
        }
    }

    return contactPoint;
}