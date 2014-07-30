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
    // runState: Timer�� ���� 30���� false�� �� -> ���α׷� ���� �غ�
    for (; runState && spaceItr != spaceshipVect.end(); spaceItr++)
    {
        // ���� ������ ù ������ ����
        const Point contactPoint = (*spaceItr)->FindContactPoint(this, world);
        if (contactPoint == dummyPoint) continue; // ����� ���

        // �Ÿ� ���
        double spaceToContact = (*spaceItr)->GetPos().CalcDistance(contactPoint);
        double contactToBunk = contactPoint.CalcDistance(pos);

        // ���ҵ� ������ ���
        double originEnergy = (*spaceItr)->GetEnergy();
        double contactEnergy = originEnergy / spaceToContact;
        double bunkerEnergy = contactEnergy / pow(contactToBunk, 2);

        damage += bunkerEnergy;
    }
}