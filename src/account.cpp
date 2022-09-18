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
    // //定义互斥锁属性
    // pthread_mutexattr_t mutexattr;
    // // 初始化初始化互斥锁属性
    // pthread_mutexattr_init(&mutexattr);
    // //设置互斥锁属性
    // pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_DEFAULT);
    // // 初始化互斥锁
    // pthread_mutex_init(&a->mutex, &mutexattr);
    
    // 初始化读写锁
    pthread_rwlock_init(&a->rwlock, NULL);
    return a;
}

void destroy_account(Account *a) {
    assert(a != NULL);
    // pthread_mutex_destroy(&a->mutex);
    pthread_rwlock_destroy(&a->rwlock);
    free(a);
}

double withdraw(Account *a, double amt) {
    assert(a != NULL);
    // 加锁
    // pthread_mutex_lock(&a->mutex);    // 共享资源操作的临界区
    // 加写锁
    pthread_rwlock_wrlock(&a->rwlock);
    printf("real work!\n");
    sleep(1);
    if (amt < 0 || amt > a->balance) {
        //解锁
        // pthread_mutex_unlock(&a->mutex);
        // 解写锁
        pthread_rwlock_unlock(&a->rwlock);
        return 0.0;
    }
    double balance = a->balance;
    balance -= amt;
    a->balance = balance;
    // 解锁
    // pthread_mutex_unlock(&a->mutex);
    // 解写锁
    pthread_rwlock_unlock(&a->rwlock);
    return amt;
}

double deposit(Account *a, double amt) {
    assert(a != NULL);
    // 加锁
    // pthread_mutex_lock(&a->mutex);    // 共享资源操作的临界区
    // 加写锁
    pthread_rwlock_wrlock(&a->rwlock);
    if (amt < 0) {
        // 解锁
        // pthread_mutex_unlock(&a->mutex);
        // 解写锁
        pthread_rwlock_unlock(&a->rwlock);
        return 0.0;
    }
    double balance = a->balance;
    sleep(1);
    balance += amt;
    a->balance = balance;
    // 解锁
    // pthread_mutex_unlock(&a->mutex);
    // 解写锁
    pthread_rwlock_unlock(&a->rwlock);
    return amt;
}

double get_balance(Account *a) {
    assert(a != NULL);
    // 上锁
    // pthread_mutex_lock(&a->mutex);
    // 上读锁
    pthread_rwlock_rdlock(&a->rwlock);
    double balance = a->balance;
    // 解锁
    // pthread_mutex_unlock(&a->mutex);
    //解读锁
    pthread_rwlock_unlock(&a->rwlock);
    return balance;
}