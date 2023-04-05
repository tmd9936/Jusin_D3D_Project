#include "stdafx.h"
#include "ThreadPool.h"

IMPLEMENT_SINGLETON(CThreadPool)

CThreadPool::CThreadPool()
{
}

/*
_uint APIENTRY LoadingMain(void* pArg)
{
	if(FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	CLoader* pLoader = (CLoader*)pArg;

	EnterCriticalSection(pLoader->Get_CriticalSection());

	HRESULT			hr = { 0 };

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		hr = pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_BASECAMP:
		hr = pLoader->Loading_ForBaseCampLevel();
		break;
	case LEVEL_WORLDMAP:
		hr = pLoader->Loading_ForWorldMapLevel();
		break;
	case LEVEL_STAGE:
		hr = pLoader->Loading_ForStageLevel();
		break;
	}

	if (FAILED(hr))
	{
		LeaveCriticalSection(pLoader->Get_CriticalSection());
		return 1;
	}

	LeaveCriticalSection(pLoader->Get_CriticalSection());

	return 0;
}

*/

HRESULT CThreadPool::Initialize(_uint threadNum)
{
	workList.reserve(threadNum);
	workerThreadList.reserve(threadNum);
	workerEventList.reserve(threadNum);

	return S_OK;
}

HRESULT CThreadPool::Add_Work(WORK work)
{
	WorkerThread workThread = {};

	workThread.finished = false;
	workThread.m_CriticalSection = { new CRITICAL_SECTION };
	workThread.hTread = { 0 };
	workThread.idThread = threadIdx++;

	InitializeCriticalSection(workThread.m_CriticalSection);
	workerThreadList.push_back(workThread);

	workThread.hTread = (HANDLE)_beginthreadex(nullptr, 0, work, this, 0, nullptr);
	//	workThread.hTread = (HANDLE)_beginthreadex(nullptr, 0, work, this, 0, &threadIdx);

	if (0 == workThread.hTread)
		return E_FAIL;

	workList.push_back(work);
	workerEventList.push_back(workThread.hTread);

	return S_OK;
}

void CThreadPool::Free(void)
{
	for (auto& worker : workerThreadList)
	{
		WaitForSingleObject(worker.hTread, INFINITE);

		DeleteCriticalSection(worker.m_CriticalSection);
		DeleteObject(worker.hTread);

		Safe_Delete(worker.m_CriticalSection);
	}
}
