#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int res;
  bool is_wait;           //用户给出的判断条件
  pthread_cond_t cond;    //条件变量
  pthread_mutex_t mutex;  //互斥锁
} Result;

// 计算和放置结果的线程运行函数
void* set_fn(void* arg) {
  Result* r = (Result*)arg;
  int sum;
  for (int i = 0; i <= 100; i++) {
    sum += i;
  }
  // 放置结果
  r->res = sum;

  //对共享的判断条件保护
  pthread_mutex_lock(&r->mutex);

  // 判断获取结果的线程是否准备好
  while (r->is_wait == false) {
    // 解锁
    pthread_mutex_unlock(&r->mutex);
    usleep(100);
    // 上锁
    pthread_mutex_lock(&r->mutex);
  }
  // 解锁
  pthread_mutex_unlock(&r->mutex);

  // 通知获取结果线程唤醒
  pthread_cond_broadcast(&r->cond);


  return (void*)0;
}

// 获取结果的线程运行函数
void* get_fn(void* arg) {
  Result* r = (Result*)arg;
  // 对两个线程共享的判断条件进行保护（枷锁）
  //两个线程对判断条件操作互斥
  pthread_mutex_lock(&r->mutex);

  // 代表获取结果的线程已准备好
  r->is_wait = true;

  //计算结果线程进行等待，放入等待队列，阻塞
  pthread_cond_wait(&r->cond, &r->mutex);

  //释放锁
  pthread_mutex_unlock(&r->mutex);

  // 获取计算结果（为什么先释放锁才获取结果呢？）
  /**
   * 没什么道理，但是也可以，反正都是读；
   */
  int res = r->res;
  printf("0x%lx get sum is %d\n", pthread_self(), res); 

  return (void*)0;
}

/**
 * @brief 一个线程计算，一个线程获取结果
 * 
 * @return int 
 */
int main() {
  int err;
  pthread_t cal, get;

  Result r;
  r.is_wait = false;
  pthread_cond_init(&r.cond, NULL);
  pthread_mutex_init(&r.mutex, NULL);

  // 获取结果线程
  if ((err = pthread_create(&get, NULL, get_fn, (void*)&r)) != 0) {
    printf("pthread_create error\n");
    return 0;
  }
  // 计算结果的线程
  if ((err = pthread_create(&cal, NULL, set_fn, (void*)&r)) != 0) {
    printf("pthread_create error\n");
    return 0;
  }

  pthread_join(get, NULL);
  pthread_join(cal, NULL);

  return 0;
}