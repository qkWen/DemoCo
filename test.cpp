#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include "Scheduler.h"

#define MAX_DEPTH 20
void PrintfStackFunName(void)
{
	void *buff[MAX_DEPTH] = { 0 };
	unsigned int depth = backtrace(buff, MAX_DEPTH);
	char **funName = backtrace_symbols(buff, depth);
	for (int i = 0; i < MAX_DEPTH; i++) {
		if (i >= depth) {
			break;
		}
		printf("i =  %d, func = %s\n", i, funName[i]);
	}
}


struct args {
    int n;
};

static void
foo(Scheduler* S, void* ud) {
    args* arg = reinterpret_cast<args*>(ud);
    int start = arg->n;
    for(int i = 0; i < 5; i++) {  
        ::printf("coroutine %d : %d\n", S->CoRunning(), start + i);
        S->Coyield();
        PrintfStackFunName();
    }
}

static void test(Scheduler* S){
    args arg1 = { 1000 };
    args arg2 = { 2000 };
    args arg3 = { 5000 };
    ptrdiff_t co1 = S->createCo(foo,&arg1);
    ptrdiff_t co2 = S->createCo(foo,&arg2);
    ptrdiff_t co3 = S->createCo(foo,&arg3);
    printf("main start\n");

    while(S->coqueue[co1] && S->coqueue[co2]&&S->coqueue[co3]){
        S->CoResume(co1);
        S->CoResume(co2);
        S->CoResume(co3);
    }

    printf("main end\n");
}

int main(){
Scheduler* S = new Scheduler();
test(S);
return 0;
}