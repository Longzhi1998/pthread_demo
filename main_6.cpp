#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cerrno>

#include "inc/account.h"

typedef struct {
  char name[20];
  Account *account;
  double amt;
} OperArg;

// 定义存款操作函数
void *withdraw_fn(void *arg) {
  OperArg *oa = (OperArg *)arg;
  double amt = withdraw(oa->account, oa->amt);

  printf("%8s(Ox%lx) withdraw %f from account %d\n", oa->name, pthread_self(),
         amt, oa->account->code);
  return (void*)0;
}

//定义取款操作函数
void *deposit_fn(void *arg) {
    OperArg *oa = (OperArg*)arg;
    double amt = deposit(oa->account, oa->amt);

    printf("%8s(Ox%lx) deposit %f from account %d\n", oa->name, pthread_self(),
         amt, oa->account->code);
    return (void*)0;
}

//定义检查银行账户的线程运行函数
void *check_fn(void *arg) {
    return (void*)0;
}

int main() {
  int err;
  pthread_t boy, girl;
  Account *a = create_account(50042353, 10000.0);

  OperArg o1, o2;
  strcpy(o1.name, "boy");
  o1.account = a;
  o1.amt = 10000.0;

  strcpy(o2.name, "girl");
  o2.account = a;
  o2.amt = 10000.0;

  // 启动两个子线程（boy和girl线程）
  // 同时去操作同一个银行账户
  if ((err = pthread_create(&boy, NULL, withdraw_fn, (void *)&o1))) {
    printf("pthread_create error\n");
    return 0;
  }

  if ((err = pthread_create(&girl, NULL, withdraw_fn, (void *)&o2))) {
    printf("pthread_create error\n");
    return 0;
  }

  pthread_join(boy, NULL);
  pthread_join(girl, NULL);

  printf("account balance %f\n", get_balance(a));
  return 0;
}