#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cerrno>
#include <string.h>

int sum;
void out_state(pthread_attr_t * attr) {
    int state;
    if(pthread_attr_getdetachstate(attr, &state) != 0) {
        printf("getdetachstate error\n");
    } else {
        if (state == PTHREAD_CREATE_DETACHED) {
            printf("detacged state\n");
        } else {
            printf("joinable state\n");
        }
    }
}

void* th_fn(void* arg) { 
    sum = 0;
    for (int i = 0; i <= 100; i++) {
        sum += i;
    }
    return (void*)0; 
    }

int main() {
  int err;
  int distance = 50;
  pthread_t th_default, th_detach;             //定义线程标识符
  pthread_attr_t attr;       //定义线程属性
  pthread_attr_init(&attr);  // 初始化线程属性
  out_state(&attr);
  
  // 去默认值分离属性，创建线程
  err = pthread_create(&th_default, &attr, th_fn, (void*)&distance);
  if (err != 0) {
    printf("pthread_create error\n");
    return -1;
  }

  int *res;
  if((err = pthread_join(th_default, (void**)&res)) != 0) {
    printf("pthread join error\n");
  } else {
    printf("th_default return is %d\n", sum);
  }

  printf("---------------------------------------------\n");

  //分离状态属性创建线程
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  out_state(&attr);

  err = pthread_create(&th_detach, &attr, th_fn, (void*)&distance);
  if (err != 0) {
    printf("pthread_create error\n");
    return -1;
  }

    if((err = pthread_join(th_detach, (void**)&res)) != 0) {
    printf("pthread join error\n");
    printf("error code is %s\n", strerror(err));
  } else {
    printf("th_detach return is %d\n", sum);
  }

  // 销毁线程属性
  pthread_attr_destroy(&attr);

  return 0;
}