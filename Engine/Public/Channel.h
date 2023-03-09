#pragma once

#include "Base.h"

/* 특정 애니메이션을 구동하는데 필요한 뼈들 중, 하나. */
/* 시간에 따른 이 뼈의 상태들을 가진다. */

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	CChannel(const CChannel& rhs);
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(aiNodeAnim* pAIChannel);

private:
	char				m_szName[MAX_PATH] = "";

	_uint				m_iNumKeyFrames = { 0 };
	vector<KEYFRAME>	m_KeyFrames;
public:
	static CChannel* Create(aiNodeAnim* pAIChannel);
	virtual void Free() override;
};

END