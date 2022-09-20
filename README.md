# pthread_demo
POSIX 多线程实验代码
||.git git本地库管理
||inc 头文件/库的头文件
||src 源文件
||lib 动态/静态库
||out src输出的目标文件.o/main的.o
||build Cmake工作目录

编译指令：
//除去main_6.cpp外的其他demo编译方法
$g++ -o bin/main_xx main_xx.cpp -lpthread

//main_6.cpp 的编译方法，包含头文件account.h
$g++ -o bin/main_6 -Iinc src/account.cpp main_6.cpp -lpthread