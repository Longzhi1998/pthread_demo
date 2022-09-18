#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void clean_fun(void *arg) {
  printf("clean_func: %s\n", (char *)arg);
  printf("thread %lx\n", pthread_self());
}

void *th_fn1(void *arg) {
  int arg_int = *(int *)arg;
  pthread_cleanup_push(clean_fun, (void *)"first clean fun");
  sleep(5);
  pthread_cleanup_pop(arg_int);      //pop的时候才运行clean_fun
  return (void *)0;  
}

void *th_fn2(void *arg) {
  int arg_int = *(int *)arg;
  pthread_cleanup_push(clean_fun, (void *)"second clean fun");
  pthread_cleanup_push(clean_fun, (void *)"third clean fun");
  
  pthread_cleanup_pop(arg_int);
  pthread_cleanup_pop(arg_int);
  return (void *)0;  
}

int main() {
  int err;
  pthread_t th1, th2;
  int num = 1;
  err = pthread_create(&th1, NULL, th_fn1, (void *)&num);
  if (err != 0) {
    printf("pthread_create error\n");
    return -1;
  }
  err = pthread_create(&th2, NULL, th_fn2, (void *)&num);
  if (err != 0) {
    printf("pthread_create error\n");
    return -1;
  }
  void *null;
  pthread_join(th1, (void**)&null);   //th1执行完成后才会往下执行
  pthread_join(th2, (void**)&null);
  pthread_exit((void*)0);
}