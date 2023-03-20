#include "Animation.h"

#include "Bone.h"

_double							CAnimation::m_LerpDuration = 2.7;

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& rhs)
	: m_Duration(rhs.m_Duration)
	, m_TickPerSecond(rhs.m_TickPerSecond)
	, m_TimeAcc(rhs.m_TimeAcc)
	, m_iNumChannels(rhs.m_iNumChannels)
	, m_Channels(rhs.m_Channels)
	, m_iCurrentKeyFrames(rhs.m_iCurrentKeyFrames)
	, m_isLoop(rhs.m_isLoop)
	, m_isFinished(rhs.m_isFinished)
{
	for (auto& channel : m_Channels)
		Safe_AddRef(channel);

	strcpy_s(m_szName, rhs.m_szName);
}

HRESULT CAnimation::Initialize(aiAnimation* pAIAnimation, CModel* pModel)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	m_Duration = pAIAnimation->mDuration;
	m_TickPerSecond = pAIAnimation->mTicksPerSecond;

	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i], pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	m_iCurrentKeyFrames.resize(m_iNumChannels);

	return S_OK;
}

_bool CAnimation::Update(vector<CBone*>& Bones, _double TimeDelta)
{
	if (!m_bAnimationChangeLerp)
	{
		if (m_TimeAcc < m_Duration)
			m_isFinished = false;

		/* m_TimeAcc : 현재 애니메이션이 재생된 시간. */
		m_TimeAcc += m_TickPerSecond * TimeDelta;

		if (m_TimeAcc >= m_Duration)
		{
			m_isFinished = true;

			if (true == m_isLoop)
			{
				m_TimeAcc = 0.0;
			}
		}

		/* 이 애님을 표현하는데 필요한 모든 뼈대들의 행렬을 키프레임정보로 만들어낸다. */
		for (_uint i = 0; i < m_iNumChannels; ++i)
		{
			m_Channels[i]->Update(Bones, m_iCurrentKeyFrames[i], m_TimeAcc);
		}
	}
	else
	{

		/* m_TimeAcc : 현재 애니메이션이 재생된 시간. */
		m_TimeAcc += m_TickPerSecond * TimeDelta;

		//if (m_TimeAcc >= m_LerpDuration)
		//{
		//	m_bAnimationChangeLerp = false;
		//	m_isFinished = true;
		//	m_TimeAcc = 0.0;
		//}

		/* 이 애님을 표현하는데 필요한 모든 뼈대들의 행렬을 키프레임정보로 만들어낸다. */
		for (_uint i = 0; i < m_iNumChannels; ++i)
		{
			if (m_Channels[i]->Update_Change_Animation_Lerp(Bones, m_ChangePreAnimation_LastKeyFrames[i], m_iCurrentKeyFrames[i], m_TimeAcc, m_LerpDuration))
			{
				//m_TimeAcc = 0.0;
				m_bAnimationChangeLerp = false;
			}
		}
	}

	return m_isFinished;
}

CAnimation* CAnimation::Create(aiAnimation* pAIAnimation, CModel* pModel)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Create()
{
	return new CAnimation();
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	for (auto& channel : m_Channels)
		Safe_Release(channel);

	m_Channels.clear();
}
