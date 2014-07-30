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
        this_thread::sleep_for(time); // time 만큼 sleep
        runState = false; // 전역 변수. false이면 실행 쓰레드들이 작업을 마침
        cout << "타이머 끝" << endl;
    });
}

void Timer::Stop()
{
    th.detach();
}