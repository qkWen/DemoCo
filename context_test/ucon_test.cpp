#include <ucontext.h>
#include <stdio.h>

void func1(void* args ){
    puts("There is context!\n");
}

void context_test(){
    char stack[1024*128];
    ucontext_t child,main;

    getcontext(&child);//初始化一个上下文结构体
    child.uc_stack.ss_sp=stack;
    child.uc_stack.ss_size = sizeof(stack);
    child.uc_stack.ss_flags=0;
    child.uc_link = &main; //设置后继上下文

    makecontext(&child,(void (*)(void))func1,0);
    swapcontext(&main,&child);
    puts("context come back\n");


}

int main(){
    context_test();

    return 0;
}