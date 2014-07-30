// 30초 타이머를 나타내는 쓰레드 클래스
#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include "myGlobal.h"

using namespace std;

class Timer
{
private:
    thread th;

public:
    Timer();
    ~Timer();

    void Start(chrono::milliseconds time);
    void Stop();
};

