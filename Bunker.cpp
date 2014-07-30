#include "Bunker.h"


Bunker::Bunker(int x, int y) : pos({ x, y }), damage(0)
{
}


Bunker::~Bunker()
{
}


void Bunker::CalcDamage(const vector<shared_ptr<Spaceship>>& spaceshipVect, World* world)
{
    vector<shared_ptr<Spaceship>>::const_iterator spaceItr = spaceshipVect.begin();
    // runState: Timer에 의해 30초후 false가 됨 -> 프로그램 종료 준비
    for (; runState && spaceItr != spaceshipVect.end(); spaceItr++)
    {
        // 빔과 대지의 첫 접점을 구함
        const Point contactPoint = (*spaceItr)->FindContactPoint(this, world);
        if (contactPoint == dummyPoint) continue; // 산란인 경우

        // 거리 계산
        double spaceToContact = (*spaceItr)->GetPos().CalcDistance(contactPoint);
        double contactToBunk = contactPoint.CalcDistance(pos);

        // 감소된 에너지 계산
        double originEnergy = (*spaceItr)->GetEnergy();
        double contactEnergy = originEnergy / spaceToContact;
        double bunkerEnergy = contactEnergy / pow(contactToBunk, 2);

        damage += bunkerEnergy;
    }
}