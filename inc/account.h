#pragma once

#include <pthread.h>

typedef struct 
{
    int code;
    double balance;

    //定义互斥锁，用来对多线程共享资源加锁（保护）
    //一般不要把锁初始化为全局变量
    // pthread_mutex_t mutex;
    //定义读写锁
    pthread_rwlock_t rwlock;
}Account;

/**
 * @brief 创建账户
 * 
 * @param code 
 * @param balance 
 * @return Account* 
 */
Account* create_account(int code, double balance);

/**
 * @brief 销毁账户
 * 
 * @param a 
 */
void destroy_account(Account *a);

/**
 * @brief 取款
 * 
 * @param a 
 * @param amt 
 * @return double 
 */
double withdraw(Account *a, double amt);

/**
 * @brief 存钱
 * 
 * @param a 
 * @param amt 
 * @return double 
 */
double deposit(Account *a, double amt);

/**
 * @brief 查看余额
 * 
 * @param a 
 * @return double 
 */
double get_balance(Account *a);