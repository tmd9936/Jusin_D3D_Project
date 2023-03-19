#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel;
class CModel;
class CBone;

class CAnimation final : public CBase
{
public:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(aiAnimation* pAIAnimation, CModel* pModel);
	_bool Update(vector<CBone*>& Bones, _double TimeDelta);

public:
	const char* Get_Name() const {
		return m_szName;
	}

	const _double Get_Duration() const {
		return m_Duration;
	}

	const _double Get_TickPerSecond() const {
		return m_TickPerSecond;
	}

	const _uint	Get_NumChannels() const {
		return m_iNumChannels;
	}

	void Get_CurrentKeyFrames(vector<_uint>& CurrentkeyFrames) const {
		for (size_t i = 0; i < m_iCurrentKeyFrames.size(); ++i)
		{
			CurrentkeyFrames.push_back(m_iCurrentKeyFrames[i]);
		}
	}

	void Get_Channels(vector<CChannel*>& Channels) const {
		for (size_t i = 0; i < m_Channels.size(); ++i)
		{
			Channels.push_back(m_Channels[i]);
		}
	}

private:
	char			m_szName[MAX_PATH] = "";
	_double			m_Duration = { 0.0 }; /* 이 애니메이션을 재생하는데 걸리는 총 시간. */
	_double			m_TickPerSecond = { 0.0 }; /* 초당 재생해야하는 속도. */
	_double			m_TimeAcc = { 0.0 };

private:  /* 뼈들 */ /* CChannel : 이 뼈가 이 애니메이션을 구동하기위한 전체 시간 안에서 세분화된 시간마다 이 뼈가 표현해야할 행렬정보를 가진다. */
	_uint								m_iNumChannels = { 0 };
	vector<CChannel*>					m_Channels;

	vector<_uint>						m_iCurrentKeyFrames;

	_bool								m_isLoop = { true };
	_bool								m_isFinished = { false };

public:
	static	CAnimation* Create(aiAnimation* pAIAnimation, CModel* pModel);
	CAnimation* Clone();
	virtual void Free() override;

};

END
