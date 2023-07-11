#include "threadpool.h"

// 线程池类构造函数
template <typename T>
threadpool<T>::threadpool(int thread_number, int max_requests) :
    m_thread_number(thread_number), m_max_requests(max_requests),
    m_stop(false), m_threads(NULL) {

    // 参数检查
    if ((thread_number <= 0) || (max_requests <= 0)) {
        throw std::exception();
    }

    // 创建thread_number个线程，并将它们都设置为脱离线程
    m_threads = new pthread_t[m_thread_number];
    if (!m_threads) {
        throw std::exception();
    }
    for (int i = 0; i < thread_number; ++i) {
        printf("create the %dth thread\n", i);

        if (pthread_create(m_threads + i, NULL, worker, this) != 0) {
            delete[] m_threads;
            throw std::exception();
        }

        if (pthread_detach(m_threads[i])) {
            delete[] m_threads;
            throw std::exception();
        }
    }
}

// 线程池类析构函数
template <typename T>
threadpool<T>::~threadpool() {
    delete[] m_threads;
    m_stop = true;
}

// 往请求队列中添加任务
template <typename T>
bool threadpool<T>::append(T* request) {
    // 操作工作队列时一定要加锁，因为它被所有线程共享
    m_queuelocker.lock();

    // 如果请求队列的大小大于允许的最大请求数，就解锁并返回false
    if (m_workqueue.size() > m_max_requests) {
        m_queuelocker.unlock();
        return false;
    }

    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();
    
    return true;
}

// 工作线程运行的函数，它不断从工作队列中取出任务并执行
template <typename T>
void* threadpool<T>::worker(void* arg) {
    threadpool* pool = (threadpool*)arg;
    pool->run();
    return pool;
}

// 线程运行的函数，它不断地等待队列中的任务，然后处理
template <typename T>
void threadpool<T>::run() {
    while (!m_stop) {
        m_queuestat.wait();
        m_queuelocker.lock();
        if (m_workqueue.empty()) {
            m_queuelocker.unlock();
            continue;
        }

        T* request = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();
        if (!request) {
            continue;
        }

        request->process();
    }
}
