#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel;
class CModel;
class CBone;

class CAnimation final : public CBase
{
	typedef struct ChangeLastFrame {
		_uint		key;
		KEYFRAME keyFrame;

	}CHANGE_LAST_FRAME;

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

public:
	void Set_Name(string szName) {
		strcpy_s(m_szName, szName.c_str());
	}

	void Set_Duration(_double Duration) {
		m_Duration = Duration;
	}

	void Set_TickPerSecond(_double TickPerSecond) {
		m_TickPerSecond = TickPerSecond;
	}

	void Set_NumChannels(_uint NumChannels) {
		m_iNumChannels = NumChannels;
	}

	void Add_Channel(CChannel* pChannel) {
		m_Channels.push_back(pChannel);
	}


	void Add_CurrentKeyFrames(_uint CurrentKeyFrame) {
		m_iCurrentKeyFrames.push_back(CurrentKeyFrame);
	}

	void Set_AnimationChangeLerp(_bool AnimationChangeLerp) {
		//m_TimeAcc = 0.0;
		m_bAnimationChangeLerp = true;
	}

	// 현재 키프레임을 현재 본들의 transMatrix값으로 가져오기
	void Set_ChangePreAnimation_LastKeyFrame(CAnimation& pAnimation);

	void Set_LerpDuration(_double LerpDuration) {
		m_LerpDuration = LerpDuration;
	}


private:
	char					m_szName[MAX_PATH] = "";
	_double					m_Duration = { 0.0 }; /* 이 애니메이션을 재생하는데 걸리는 총 시간. */
	_double					m_TickPerSecond = { 0.0 }; /* 초당 재생해야하는 속도. */
	_double					m_TimeAcc = { 0.0 };

private:  /* 뼈들 */ /* CChannel : 이 뼈가 이 애니메이션을 구동하기위한 전체 시간 안에서 세분화된 시간마다 이 뼈가 표현해야할 행렬정보를 가진다. */
	_uint			 		m_iNumChannels = { 0 };
	vector<CChannel*>		m_Channels;

	vector<_uint>	 		m_iCurrentKeyFrames;

	_bool			 		m_isLoop = { true };
	_bool			 		m_isFinished = { false };

	_bool			 		m_bAnimationChangeLerp = { false };

	vector<KEYFRAME>		m_ChangePreAnimation_LastKeyFrames;
	
public:
	static _double							m_LerpDuration; /* 이 애니메이션을 재생하는데 걸리는 총 시간. */


public:
	static	CAnimation* Create(aiAnimation* pAIAnimation, CModel* pModel);
	static	CAnimation* Create();

	CAnimation* Clone();
	virtual void Free() override;

};

END
