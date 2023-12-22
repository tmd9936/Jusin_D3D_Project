#include "stdafx.h"
#include "ThreadPool.h"

IMPLEMENT_SINGLETON(CThreadPool)

CThreadPool::CThreadPool()
{
}

void CThreadPool::Start()
{
    const uint32_t num_threads = std::thread::hardware_concurrency(); // Max # of threads the system supports
    threads.resize(num_threads);

    for (uint32_t i = 0; i < num_threads; i++) {
        threads.at(i) = std::thread(&CThreadPool::ThreadLoop, this);
    }
}

void CThreadPool::QueueJob(const std::function<_uint()>& job)
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
        lock.unlock();
    }
    mutex_condition.notify_one();
}

void CThreadPool::Stop()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
        lock.unlock();
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}

_bool CThreadPool::Is_NoJobStae()
{
    bool result = false;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        result = jobs.empty();
    }
    return result;
}

_bool CThreadPool::JobEndCheck()
{
    return jobEndCheck.empty();
}

void CThreadPool::JobStart()
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    jobEndCheck.push(0);
    lock.unlock();
}

void CThreadPool::JobEnd()
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    jobEndCheck.pop();
    lock.unlock();

}

void CThreadPool::ThreadLoop()
{
    while (true) {
        std::function<_uint()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
                });
            if (should_terminate) {
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}


void CThreadPool::Free(void)
{
    Stop();
}
