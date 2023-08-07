#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include "locker.h"

// 线程池类
// 用于创建线程池，管理线程池中的线程，以及添加任务到线程池中
// 定义为模板类是为了代码复用，模板参数T是任务类
template <typename T>
class threadpool
{
public:
    threadpool(int thread_number = 8, int max_requests = 10000);
    ~threadpool();

    // 往请求队列中添加任务
    bool append(T *request);

private:
    // 工作线程运行的函数，它不断从工作队列中取出任务并执行
    static void *worker(void *arg);
    // 线程运行的函数，它不断地等待队列中的任务，然后处理
    void run();

private:
    int m_thread_number;        // 线程池中的线程数
    int m_max_requests;         // 请求队列中允许的最大请求数
    pthread_t *m_threads;       // 描述线程池的数组，其大小为m_thread_number
    std::list<T *> m_workqueue; // 请求队列
    locker m_queuelocker;       // 保护请求队列的互斥锁
    sem m_queuestat;            // 是否有任务需要处理
    bool m_stop;                // 是否结束线程
};

#endif
