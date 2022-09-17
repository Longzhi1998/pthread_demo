#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int g_data = 0;

typedef struct {
    char ch;
    int var;
    char str[64];
}th_t;

void *thrd_func(void *arg){
    th_t *retvar = (th_t *)arg;

    // retvar->ch = 'm';
    // retvar->var = 200;
    // strcpy(retvar->str, "my thread");
	g_data = 344;
	//线程的退出可以将return换成pthread_exit
    pthread_exit((void *)retvar);
}

/**
 * @brief 线程间的数据交互测试（动态内存、栈、全局变量）
 * 
 * @return int 
 */
int main(void){
	
    pthread_t tid;
    int ret;
    g_data = 3999;
    // th_t *retval = NULL;
	// retval = (th_t *)malloc(sizeof(th_t));
    th_t retval = {'L', 300, "longzhi"};

    ret = pthread_create(&tid, NULL, thrd_func, (void *)&retval);
    if (ret != 0) {
        fprintf(stderr, "pthread_create error:%s\n", strerror(ret));
        exit(1);//整个进程退出
    }
	
	//阻塞并获取返回值
    th_t *retval_p = NULL;
    pthread_join(tid, (void **)&retval_p);
    printf("ch = %c, var = %d, str = %s\n", retval_p->ch, retval_p->var, retval_p->str);
    printf("g_data:%d\n", g_data);
	// if(retval != NULL){
	// 	free(retval);
	// 	retval = NULL;
	// }
    
	//注意：这里只是将主线程退出，若还有其它子线程在运行扔会运行，而return将会使整个进程结束
    pthread_exit((void *)1);
}


