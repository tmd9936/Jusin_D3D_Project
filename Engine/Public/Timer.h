#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() = default;

public:
	_double Get_TimeDelta(void) {
		return m_TimeDelta;
	}

public:
	HRESULT		Ready_Timer(void);
	void		Update_Timer(void);

private:
	LARGE_INTEGER			m_tFrameTime;
	LARGE_INTEGER			m_tFixTime;
	LARGE_INTEGER			m_tLastTime;
	LARGE_INTEGER			m_CpuTick;

	_double					m_TimeDelta = { 0.0 };

public:
	static CTimer* Create(void);
	virtual void		Free(void) override;
};

END
