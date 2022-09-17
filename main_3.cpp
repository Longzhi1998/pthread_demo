#include<pthread.h>
#include <unistd.h>
#include <stdio.h>

pthread_once_t once = PTHREAD_ONCE_INIT;

void once_run(void)
{
    printf("once_run in thread %lx\n", pthread_self());
}

void* child1(void * arg)
{
    pthread_t tid =pthread_self();
    printf("thread %lx enter\n", tid);
    pthread_once(&once,once_run);
    printf("thread %lx return\n", tid);
    pthread_exit((void*)0);
}


void* child2(void * arg)
{
    pthread_t tid =pthread_self();
    printf("thread %lx enter\n", tid);
    pthread_once(&once,once_run);
    printf("thread %lx return\n", tid);
    pthread_exit((void*)0);
}

/**
 * @brief 只运行一次的线程
 * 
 * @return int 
 */
int main()
{
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,child1,NULL);
    pthread_create(&tid2,NULL,child2,NULL);
    sleep(10);
    printf("main thread exit.\n");
    return 0;
}
