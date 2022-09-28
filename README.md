一个基于ucontext函数簇实现的共享栈协程库<br>
Coroutine为协程类<br>
    协程需要有执行的目标函数，协程自身运行状态，保存上下文，以及所需的栈空间

Scheduler为管理类<br>
    主要实现创建协程，启动协程，切换协程，以及关闭所有协程等方法
  
  test 为切换协程测试程序<br>
    PrintfStackFunName()  为打印函数调用栈<br>
    foo() 为协程执行的目标函数，以参数形式传入createCo方法中
