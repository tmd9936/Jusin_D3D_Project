#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel;

class CAnimation final : public CBase
{
public:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(aiAnimation* pAIAnimation);
	void Update(_double TimeDelta);

private:
	char			m_szName[MAX_PATH] = "";
	_double			m_Duration = { 0.0 }; /* 이 애니메이션을 재생하는데 걸리는 총 시간. */
	_double			m_TickPerSecond = { 0.0 }; /* 초당 재생해야하는 속도. */
	_double			m_TimeAcc = { 0.0 };

private:
	_uint				m_iNumChannels = { 0 };
	vector<CChannel*>	m_Channels;

public:
	static	CAnimation* Create(aiAnimation* pAIAnimation);
	virtual void Free() override;

};

END
