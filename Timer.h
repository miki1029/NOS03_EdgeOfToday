// 30�� Ÿ�̸Ӹ� ��Ÿ���� ������ Ŭ����
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

