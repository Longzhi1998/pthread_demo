#include "../inc/account.h"

#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

Account* create_account(int code, double balance) {
    Account *a = (Account*)malloc(sizeof(Account));
    assert(a != NULL);
    a->code = code;
    a->balance = balance;
    //定义互斥锁属性
    pthread_mutexattr_t mutexattr;
    // 初始化初始化互斥锁属性
    pthread_mutexattr_init(&mutexattr);
    //设置互斥锁属性
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_DEFAULT);
    // 初始化互斥锁
    pthread_mutex_init(&a->mutex, &mutexattr);
    return a;
}

void destroy_account(Account *a) {
    assert(a != NULL);
    pthread_mutex_destroy(&a->mutex);
    free(a);
}

double withdraw(Account *a, double amt) {
    assert(a != NULL);
    // 加锁
    pthread_mutex_lock(&a->mutex);    // 共享资源操作的临界区
    if (amt < 0 || amt > a->balance) {
        //解锁
        pthread_mutex_unlock(&a->mutex);
        return 0.0;
    }
    double balance = a->balance;
    sleep(1);
    balance -= amt;
    a->balance = balance;
    // 解锁
    pthread_mutex_unlock(&a->mutex);
    return amt;
}

double deposit(Account *a, double amt) {
    assert(a != NULL);
    // 加锁
    pthread_mutex_lock(&a->mutex);    // 共享资源操作的临界区
    if (amt < 0) {
        // 解锁
        pthread_mutex_unlock(&a->mutex);
        return 0.0;
    }
    double balance = a->balance;
    sleep(1);
    balance += amt;
    a->balance = balance;
    // 解锁
    pthread_mutex_unlock(&a->mutex);
    return amt;
}

double get_balance(Account *a) {
    assert(a != NULL);
    // 上锁
    pthread_mutex_lock(&a->mutex);
    double balance = a->balance;
    // 解锁
    pthread_mutex_unlock(&a->mutex);
    return balance;
}