#include "Scheduler.h"



 ptrdiff_t Scheduler::createCo(CoFunc runfunc,void* args)
 {
    Coroutine* co = new Coroutine(this,runfunc,args);
    this->coqueue.emplace_back(std::shared_ptr<Coroutine>(co));
    return this->coqueue.size()-1;
 }

 void Scheduler::CoResume(int cid)
 {
    assert(cid >= 0 && cid < cap);
    auto Corou = coqueue[cid];
    uintptr_t ptr;
    Coroutine::status NowStatus = Corou->getStatus();
    ucontext_t NowCtx = Corou->getCtx();
    switch (NowStatus)
    {
    case Coroutine::status::COROUTINE_READY:
        getcontext(&NowCtx);
        NowCtx.uc_stack.ss_sp = stack;
        NowCtx.uc_stack.ss_size = STACK_SIZE;
        NowCtx.uc_link = &mainCtx;     // 如果协程执行完，则切换到mainCtx主协程中进行执行
        runCoId = cid;
        Corou->setStatus(Coroutine::status::COROUTINE_RUNNING);

        ptr = (uintptr_t)this;
        makecontext(&NowCtx,(void(*)(void))mainfunc,2, (uint32_t)ptr, (uint32_t)(ptr >> 32));
        swapcontext(&mainCtx, &NowCtx);   //当前的上下文内容放入mainCtx中，并将NowCtx的上下文替换到当前上下文
        break;

    case Coroutine::status::COROUTINE_SUSPEND:
        ::memcpy(stack+STACK_SIZE-Corou->size(),Corou->stack(),Corou->size());
        runCoId = cid;
        Corou->setStatus(Coroutine::status::COROUTINE_RUNNING);
        swapcontext(&mainCtx,&NowCtx);
        break;
    default:
        break;
    }
 }

void Scheduler::Coyield(){
    Coroutine& Corou = *coqueue[runCoId];
    Corou.saveStack(stack+STACK_SIZE); //保存当前栈空间
    Corou.setStatus(Coroutine::status::COROUTINE_SUSPEND);
    runCoId = -1;
    swapcontext(&Corou.getCtx(), &mainCtx);

}

void Scheduler::CoClose(){
    coqueue.erase(coqueue.begin(),coqueue.end());
    runCoId = -1;
    coNums = 0;
}


void Scheduler::mainfunc(uint32_t low32, uint32_t hi32){
    uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)hi32 << 32);
    Scheduler* S = (Scheduler*)ptr;
    int id = S->runCoId;
    std::shared_ptr<Coroutine> C = S->coqueue[id];
    C->runFuncInter(S); //协程在这个函数里面被yield了
    S->coqueue[id] = nullptr;
    --S->coNums;
	S->runCoId = -1;
}

int Scheduler::CoRunning(){
    return runCoId;
}
