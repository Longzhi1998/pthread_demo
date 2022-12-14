#include <error.h>
#include <pthread.h>
#include <signal.h>  //信号
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void sig_handler(int signo) {
  printf("pthread id in the sig_handler: %lx\n", pthread_self());
  if (signo == SIGALRM) {
    printf("timeout...\n");
  }
  alarm(2);  //再次设置定时器，实现循环
}

void* th_fn(void* arg) {
  // signal 函数用于注册信号处理函数，是一个回调
  if (signal(SIGALRM, sig_handler) == SIG_ERR) {
    perror("signal sigalrm err\n");
  }
  //在子线程设置定时器，信号产生后，没有设置处理函数，默认结束进程
  alarm(2);

  for (int i = 1; i <= 100; i++) {
    printf("(%lx) i: %d\n", pthread_self(), i);
    sleep(1);
  }
  return (void*)0;
}

void* th_fn2(void *arg) {
    pthread_t th1 = *(pthread_t*)arg;
    for (int i = 1; i <= 9; i++) {
        if (i%5 == 0) {
            pthread_cancel(th1);
            alarm(0); //取消定时器
        }
        printf("(%lx) i: %d\n", pthread_self(), i);
        sleep(1);
    }
    return (void*)0;
}

int main() {
  int err;
  pthread_t th, th2;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  if ((err = pthread_create(&th, &attr, th_fn, (void*)0)) != 0) {
    printf("pthread_create error\n");
    return 0;
  }

  if ((err = pthread_create(&th2, &attr, th_fn2, (void*)&th)) != 0) {
    printf("pthread_create error\n");
    return 0;
  }

  // 屏蔽信号捕获
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    pthread_sigmask(SIG_SETMASK, &set, NULL);



  while (1) {
    printf("control thread(%lx) is runnning...\n", pthread_self());
    sleep(10);   //sleep会被alarm唤醒，所以不会睡眠10s
  }
  printf("control thread oner\n");
}