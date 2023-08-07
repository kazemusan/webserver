#ifndef _LOCKER_H_
#define _LOCKER_H_

#include <exception>
#include <pthread.h>
#include <semaphore.h>

// 线程同步机制封装类

// 互斥锁类
class locker
{
public:
    locker();
    ~locker();

    bool lock();
    bool unlock();
    pthread_mutex_t *get();

private:
    pthread_mutex_t m_mutex;
};

// 信号量类
class sem
{
public:
    sem();
    sem(int num);
    ~sem();

    bool wait();
    bool post();

private:
    sem_t m_sem;
};

// 条件变量类
class cond
{
public:
    cond();
    ~cond();

    bool wait(pthread_mutex_t *m_mutex);
    bool timewait(pthread_mutex_t *m_mutex, struct timespec t);
    bool signal();
    bool broadcast();

private:
    pthread_cond_t m_cond;
};

#endif