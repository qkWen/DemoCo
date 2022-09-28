#include "Coroutine.h"

Coroutine::status Coroutine::getStatus(){
    return m_status;
}

void  Coroutine::setStatus(Coroutine::status st){
    this->m_status = st;
}

ucontext_t& Coroutine::getCtx(){
    return m_corouctx;
}

void Coroutine::saveStack(char* top){
    char flag = 0;
    assert(top-&flag<=STACK_SIZE);
    if(m_cap< top-&flag){
        free(m_stack);
        m_cap = top-&flag;
        m_stack = new char[m_cap];
    }
    m_size = top-&flag;
    memcpy(m_stack, &flag, m_size);
}

void Coroutine::runFuncInter(Scheduler* s) {
        m_func(s,args);
}