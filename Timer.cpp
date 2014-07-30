#include "Timer.h"


Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::Start(chrono::milliseconds time)
{
    th = thread([=]()
    {
        this_thread::sleep_for(time); // time ��ŭ sleep
        runState = false; // ���� ����. false�̸� ���� ��������� �۾��� ��ħ
        cout << "Ÿ�̸� ��" << endl;
    });
}

void Timer::Stop()
{
    th.detach();
}