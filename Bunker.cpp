#include "Bunker.h"


Bunker::Bunker(int x, int y) : pos({ x, y }), damage(0)
{
}


Bunker::~Bunker()
{
}


bool Bunker::CalcDamage(const vector<shared_ptr<Spaceship>>& spaceshipVect, World* world)
{
    vector<shared_ptr<Spaceship>>::const_iterator spaceItr = spaceshipVect.begin();
    for (; runState && spaceItr != spaceshipVect.end(); spaceItr++)
    {
        /*clock_t curTime = clock();
        if (curTime - startTime > 29900) return false;*/
        const Point contactPoint = (*spaceItr)->FindContactPoint(this, world);
        if (contactPoint == dummyPoint) continue;

        double spaceToContact = (*spaceItr)->GetPos().CalcDistance(contactPoint);
        double contactToBunk = contactPoint.CalcDistance(pos);

        double originEnergy = (*spaceItr)->GetEnergy();
        double contactEnergy = originEnergy / spaceToContact;
        double bunkerEnergy = contactEnergy / pow(contactToBunk, 2);

        damage += bunkerEnergy;
    }
    return true;
}