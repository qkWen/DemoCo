#pragma once
#include <functional>
#include <string.h>
#include <ucontext.h>
#include <assert.h>
class Scheduler;

//*
typedef std::function<void (Scheduler*,void*)> CoFunc;
static const int MAXCOROU = 16;
static const int STACK_SIZE = 1024*1024;

class Coroutine
{   
public:

typedef enum class STATE{
    COROUTINE_FREE  = 0,
    COROUTINE_READY = 1,
    COROUTINE_RUNNING = 2,
    COROUTINE_SUSPEND  = 3
    
}status;
//*
    Coroutine( Scheduler*, CoFunc runfunc, void* args): m_func(runfunc), args(args),m_cap(0),m_size(0) ,m_status(status::COROUTINE_READY), m_stack(nullptr)
    {::memset(&m_corouctx, 0, sizeof(m_corouctx));}
    ~Coroutine() {free(m_stack);}
    status  getStatus();
    void    setStatus(Coroutine::status);
    ucontext_t& getCtx();
    void saveStack(char* );

    inline ptrdiff_t size() { return m_size; }
    inline char* stack() { return m_stack; }

    void runFuncInter(Scheduler* s);

private:
    CoFunc m_func;
    void* args;
    ucontext_t m_corouctx;
    status m_status; // 协程当前状态
    char* m_stack;
    ptrdiff_t m_cap;
    ptrdiff_t m_size;

};

