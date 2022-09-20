#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// 定义线程信号量

sem_t sem1;
sem_t sem2;

void* a_fn(void *arg) {
    sem_wait(&sem1);
    printf("thread a running\n");
    sleep(1);
    return (void*)0;
}

void* b_fn(void *arg) {
    sem_wait(&sem2);
    printf("thread b running\n");
    sleep(1);
    sem_post(&sem1);
    return (void*)0;
}

void* c_fn(void *arg) {
    printf("thread c running\n");
    sleep(2);
    // 释放线程b（对线程信号量sem2作加一操作，让线程b继续运行
    sem_post(&sem2);
    return (void*)0;
} 


int main () {
    pthread_t a, b, c;
    //线程信号量初始化，初始值为0
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);

    //创建线程
    pthread_create(&a, NULL, a_fn, (void*)0);
    pthread_create(&b, NULL, b_fn, (void*)0);
    pthread_create(&c, NULL, c_fn, (void*)0);

    pthread_join(a, NULL);
    pthread_join(b, NULL);
    pthread_join(c, NULL);

    // 销毁线程信号量
    sem_destroy(&sem1);
    sem_destroy(&sem2);

}