#include "Timer_Manager.h"
#include "Timer.h"

IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager()
{
}

_double Engine::CTimer_Manager::Get_Timer(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);
	if (nullptr == pTimer)
		return 0.0;

	return pTimer->Get_TimeDelta();
}

void CTimer_Manager::Set_Timer(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);
	if (nullptr == pTimer)
		return;

	pTimer->Update_Timer();
}

CTimer* Engine::CTimer_Manager::Find_Timer(const _tchar* pTimerTag)
{
	auto		iter = find_if(m_umapTimers.begin(), m_umapTimers.end(), [&](unordered_map<const _tchar*, class CTimer*>::value_type Pair)->bool
		{
			if (false == lstrcmp(pTimerTag, Pair.first))
			return true;

	return false;
		});

	if (iter == m_umapTimers.end())
		return nullptr;

	return iter->second;
}

HRESULT Engine::CTimer_Manager::Ready_Timer(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	m_umapTimers.insert({ pTimerTag, pTimer });

	return S_OK;
}

void Engine::CTimer_Manager::Free(void)
{
	for (auto& Pair : m_umapTimers)
		Safe_Release(Pair.second);

	m_umapTimers.clear();
}
