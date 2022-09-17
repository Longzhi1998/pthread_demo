#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* th_fn(void *arg) {
  int distance = *(int*)arg;
  for (int i = 0; i <= distance; i++) {
    printf("%lx run %d\n", pthread_self(), i);
    int time = (int)(drand48() * 100000);
    usleep(time);
  }
  return (void*)0;   // 等价  pthread_exit((void*)0);
}

int main() {
  int err;
  int distance = 50;
  pthread_t rabbit, turble;  //定义线程标识符

  // 创建rabbit线程
  err = pthread_create(&rabbit, NULL, th_fn, (void*)&distance);
  if (err != 0) {
    printf("pthread_create error\n");
    return -1;
  }

  err = pthread_create(&turble, NULL, th_fn, (void*)&distance);
  if (err != 0) {
    printf("pthread_create error\n");
    return -1;
  }
 
  printf("main thread id %lx\n", pthread_self());
  pthread_join(rabbit, NULL);
  pthread_join(turble, NULL);
  printf("Finish!\n");
  return 0;
}