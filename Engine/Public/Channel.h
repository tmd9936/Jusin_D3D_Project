#pragma once

#include "Base.h"

/* Ư�� �ִϸ��̼��� �����ϴµ� �ʿ��� ���� ��, �ϳ�. */
/* �ð��� ���� �� ���� ���µ��� ������. */

BEGIN(Engine)

class CBone;
class CModel;

class CChannel final : public CBase
{
private:
	CChannel();
	CChannel(const CChannel& rhs);
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(aiNodeAnim* pAIChannel, CModel* pModel);
	void Update(_double CurrentTime);

private:
	char				m_szName[MAX_PATH] = "";

	_uint				m_iCurrentKeyFrame = { 0 };
	_uint				m_iNumKeyFrames = { 0 };
	vector<KEYFRAME>	m_KeyFrames;

	CBone*				 m_pBone = { nullptr };

public:
	static CChannel* Create(aiNodeAnim* pAIChannel, CModel* pModel);
	virtual void Free() override;
};

END