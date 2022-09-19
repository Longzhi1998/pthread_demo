#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int               value;

  pthread_cond_t    rc;     //条件变量
  pthread_mutex_t   rm;     //互斥锁
  bool              r_wait; //是否等待

  pthread_cond_t    wc;     //条件变量
  pthread_mutex_t   wm;     //互斥锁
  bool              w_wait; //是否等待
} Storage;

void set_data(Storage *s, int value) {
    s->value = value;
}

int get_data(Storage *s) {
    return s->value;
}

// 读者线程执行函数
void* set_th(void *arg) {
    Storage *s = (Storage*)arg;
    for (int i = 0; i <= 100; i++) {
        pthread_mutex_lock(&s->rm);
        set_data(s, i);
        printf("0x%lx write data: %d\n", pthread_self(), i);

        while (s->r_wait == false) {
            pthread_mutex_unlock(&s->rm);
            sleep(1);
            pthread_mutex_lock(&s->rm);
        }
        s->r_wait = false;
        pthread_mutex_unlock(&s->rm);
        pthread_cond_broadcast(&s->rc);

        // 写者线程等待阻塞，等待读者线程读取完数据后通知唤醒它，然后继续写数据
        pthread_mutex_lock(&s->wm);
        s->w_wait = true;
        pthread_cond_wait(&s->wc, &s->wm);
        pthread_mutex_unlock(&s->wm); 
    }
    return (void*)0;
}

// 写者线程执行函数
void* get_th(void *arg) {
    Storage *s = (Storage*)arg;
    for (int i = 0; i <= 100; i++) {
        pthread_mutex_lock(&s->rm);
        s->r_wait = true;
        pthread_cond_wait(&s->rc, &s->rm);
        int value = get_data(s);
        printf("0x%lx(%-5d) read data: %d\n", pthread_self(), i, value);
        pthread_mutex_unlock(&s->rm);
        
        pthread_mutex_lock(&s->wm);
        //判断写者线程是否准备好
        while (s->w_wait == false)
        {
            pthread_mutex_unlock(&s->wm);
            sleep(1);
            pthread_mutex_lock(&s->wm);
        }

        s->w_wait = false;
        pthread_mutex_unlock(&s->wm);
        pthread_cond_broadcast(&s->wc);
        
    }
    return (void*)0;
}


/**
 * @brief 读写互相使用条件变量
 *
 * @return int
 */
int main() {
  int err;
  pthread_t rth, wth;

  Storage s;
  // 初始化为未等待
  s.r_wait = false;
  s.w_wait = false;
  // 初始化条件变量和互斥锁
  pthread_cond_init(&s.rc, NULL);
  pthread_mutex_init(&s.rm, NULL);
  pthread_cond_init(&s.wc, NULL);
  pthread_mutex_init(&s.wm, NULL);

  // 创建读者和写者线程
  if ((err = pthread_create(&rth, NULL, set_th, (void*)&s)) != 0) {
    printf("pthread_create error\n");
    return 0;
  }
  if ((err = pthread_create(&wth, NULL, get_th, (void*)&s)) != 0) {
    printf("pthread_create error\n");
    return 0;
  }

  pthread_join(rth, NULL);
  pthread_join(wth, NULL);

  pthread_cond_destroy(&s.rc);
  pthread_mutex_destroy(&s.rm);
  pthread_cond_destroy(&s.wc);
  pthread_mutex_destroy(&s.wm);

  return 0;
}