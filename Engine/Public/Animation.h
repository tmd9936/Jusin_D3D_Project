#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel;
class CModel;

class CAnimation final : public CBase
{
public:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(aiAnimation* pAIAnimation, CModel* pModel);
	void Update(_double TimeDelta);

private:
	char			m_szName[MAX_PATH] = "";
	_double			m_Duration = { 0.0 }; /* 이 애니메이션을 재생하는데 걸리는 총 시간. */
	_double			m_TickPerSecond = { 0.0 }; /* 초당 재생해야하는 속도. */
	_double			m_TimeAcc = { 0.0 };

private:  /* 뼈들 */ /* CChannel : 이 뼈가 이 애니메이션을 구동하기위한 전체 시간 안에서 세분화된 시간마다 이 뼈가 표현해야할 행렬정보를 가진다. */
	_uint								m_iNumChannels = { 0 };
	vector<CChannel*>					m_Channels;

	_bool								m_isLoop = { true };
	_bool								m_isFinished = { false };

public:
	static	CAnimation* Create(aiAnimation* pAIAnimation, CModel* pModel);
	virtual void Free() override;

};

END
