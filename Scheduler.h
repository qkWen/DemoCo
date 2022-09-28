#pragma once
#include <ucontext.h>
#include <vector>
#include <memory>
#include "Coroutine.h"

using std::vector;

class Scheduler{
public:

    Scheduler():coqueue(cap,nullptr){}
    ~Scheduler() { CoClose(); }

    //创建一个协程
    ptrdiff_t createCo(CoFunc runfunc,void* args);
    //切换协程
    void Coyield();
    //启动协程
    void CoResume(int cid);
    //关闭所有协程
    void CoClose();
    int CoRunning();
    static void mainfunc(uint32_t low32, uint32_t hi32);
    vector<std::shared_ptr<Coroutine>> coqueue;
    
private:
    
    ucontext_t mainCtx;
    int coNums;
    int cap = MAXCOROU;
    int runCoId = -1;
    char stack[STACK_SIZE];

};