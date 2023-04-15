#pragma once

#include "Base.h"

#include "Client_Defines.h"

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>

BEGIN(Client)

typedef _uint(APIENTRY *WORK)(void* pArg);

typedef struct __WokerThread
{
    HANDLE hThread;
    DWORD idThread;
    LPCRITICAL_SECTION m_CriticalSection;
    _bool finished;
} WorkerThread;

class CThreadPool :
    public CBase
{
    DECLARE_SINGLETON(CThreadPool)

private:
    explicit CThreadPool();
    virtual ~CThreadPool() = default;

public:
    void Start();
    void QueueJob(const std::function<_uint()>& job);
    void Stop();
    _bool Is_NoJobStae();

private:
    void ThreadLoop();

    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
    std::vector<std::thread> threads;
    std::queue<std::function<_uint()>> jobs;

public:
    virtual void Free(void) override;

};

END
