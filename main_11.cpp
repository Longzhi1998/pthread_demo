#include <error.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int value;
  pthread_mutex_t mutex;
} ResourceA;

typedef struct {
  int value;
  pthread_mutex_t mutex;
} ResourceB;

typedef struct {
  ResourceA *ra;
  ResourceB *rb;
} Storage;

void *a_func(void *arg) {
  Storage *s = (Storage *)arg;
  pthread_mutex_lock(&s->ra->mutex);
  sleep(1);
  printf("0x%lx is waiting for ResourseB...\n", pthread_self());
  pthread_mutex_lock(&s->rb->mutex);
  printf("ResourceA value is:%d\n", s->ra->value);
  printf("ResourceB value is:%d\n", s->rb->value);
  pthread_mutex_unlock(&s->ra->mutex);
  pthread_mutex_unlock(&s->rb->mutex);
  return (void *)0;
}

void *b_func(void *arg) {
  Storage *s = (Storage *)arg;
  pthread_mutex_lock(&s->rb->mutex);
  sleep(1);
  printf("0x%lx is waiting for ResourseB...\n", pthread_self());
  pthread_mutex_lock(&s->ra->mutex);
  printf("ResourceA value is:%d\n", s->ra->value);
  printf("ResourceB value is:%d\n", s->rb->value);
  pthread_mutex_unlock(&s->ra->mutex);
  pthread_mutex_unlock(&s->rb->mutex);
  return (void *)0;
}

int main() {
  int err;
  ResourceA ra;
  ResourceB rb;
  ra.value = 100;
  rb.value = 200;
  pthread_mutex_init(&ra.mutex, NULL);
  pthread_mutex_init(&rb.mutex, NULL);
  Storage s = {&ra, &rb};

  pthread_t th_a, th_b;
  if ((err = pthread_create(&th_a, NULL, a_func, (void *)&s)) != 0) {
    printf("pthread_create error\n");
    return 0;
  }
  if ((err = pthread_create(&th_b, NULL, b_func, (void *)&s)) != 0) {
    printf("pthread_create error\n");
    return 0;
  }

  pthread_join(th_a, NULL);
  pthread_join(th_b, NULL);

  pthread_mutex_destroy(&ra.mutex);
  pthread_mutex_destroy(&rb.mutex);
  return 0;
}