#pragma once

#include "Base.h"

/* 특정 애니메이션을 구동하는데 필요한 뼈들 중, 하나. */
/* 시간에 따른 이 뼈의 상태들을 가진다. */

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
	void Update(vector<CBone*>& Bones, _uint& pCurrentKeyFrame, _double CurrentTime);

	_bool Update_Change_Animation_Lerp(vector<CBone*>& Bones, KEYFRAME& pPreKeyFrame, _uint& pCurrentKeyFrame, _double CurrentTime, _float LerpRatio);

	void Make_KeyFrame(_vector& vScale, _vector& vRotation, _vector& vPosition, _uint& pCurrentKeyFrame, _double CurrentTime);
	void Make_KeyFrame(KEYFRAME& OutKeyFrame, _uint& pCurrentKeyFrame, _double CurrentTime);

public:
	const char* Get_Name() const {
		return m_szName;
	}

	const _uint Get_NumKeyFrames() const {
		return m_iNumKeyFrames;
	}

	const _int	Get_BoneIndex() const {
		return m_iBoneIndex;
	}

	void Get_KeyFrames(vector<KEYFRAME>& keyFrames) {
		for (size_t i = 0; i < m_KeyFrames.size(); ++i)
		{
			keyFrames.push_back(m_KeyFrames[i]);
		}
	}

	const KEYFRAME Get_LastKeyFrame() const {
		return m_KeyFrames.back();
	}

public:
	void	Set_Name(string szName) {
		strcpy_s(m_szName, szName.c_str());
	}

	void	Set_NumKeyFrames(_uint NumKeyFrames) {
		m_iNumKeyFrames = NumKeyFrames;
	}

	void	Add_KeyFrame(KEYFRAME keyFrame) {
		m_KeyFrames.push_back(keyFrame);
	}

	void	Set_BoneIndex(_uint BoneIndex) {
		m_iBoneIndex = BoneIndex;
	}

private:
	char				m_szName[MAX_PATH] = "";

	_uint				m_iNumKeyFrames = { 0 };
	vector<KEYFRAME>	m_KeyFrames;

	_int				m_iBoneIndex = { 0 };

public:
	static CChannel* Create(aiNodeAnim* pAIChannel, CModel* pModel);

	static CChannel* Create();

	virtual void Free() override;
};

END