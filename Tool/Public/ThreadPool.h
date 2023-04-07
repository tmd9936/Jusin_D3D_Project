#pragma once

#include "Base.h"

#include "Client_Defines.h"

BEGIN(Client)

typedef _uint(APIENTRY *WORK)(void* pArg);

typedef struct __WokerThread
{
    HANDLE hTread;
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
    HRESULT Initialize(_uint threadNum);

public:
    HRESULT Add_Work(WORK work);

    const _uint   Get_WorkerThreadId() const {
        return _uint(workerThreadList.size() - 1);
    }

public:
    LPCRITICAL_SECTION Get_CurrentCriticalSection() {
        return workerThreadList[workerThreadList.size()-1].m_CriticalSection;
    }

    LPCRITICAL_SECTION Get_CriticalSection(DWORD workerID) {
        return workerThreadList[workerID].m_CriticalSection;
    }

private:
    vector<WORK> workList;

    vector<WorkerThread> workerThreadList;

    vector<HANDLE> workerEventList;

    DWORD   idOfCurrentWork = {0};

    DWORD   idOfLastAddedWork = { 0 };

    _uint   threadIdx = { 0 };

public:
    virtual void Free(void) override;

};

END
