#include "..\Public\Channel.h"
#include "Model.h"

#include "Bone.h"

CChannel::CChannel()
{
}

CChannel::CChannel(const CChannel& rhs)
	: m_iNumKeyFrames(rhs.m_iNumKeyFrames)
	, m_KeyFrames(rhs.m_KeyFrames)
	, m_pBone(rhs.m_pBone)
{
	strcpy_s(m_szName, rhs.m_szName);
}	

HRESULT CChannel::Initialize(aiNodeAnim* pAIChannel, CModel* pModel)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	m_pBone = pModel->Get_BonePtr(m_szName);
	if (nullptr == m_pBone)
		return E_FAIL;

	Safe_AddRef(m_pBone);

	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	_float3		vScale;
	_float4		vRotation;
	_float3		vPosition;

	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME			KeyFrame;
		ZeroMemory(&KeyFrame, sizeof KeyFrame);

		if (pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.Time = pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumRotationKeys > i)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.Time = pAIChannel->mRotationKeys[i].mTime;
		}

		if (pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.Time = pAIChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

void CChannel::Update(_double CurrentTime)
{
	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

	_vector			vScale, vRotation, vPosition;

	if (CurrentTime >= LastKeyFrame.Time)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vPosition = XMLoadFloat3(&LastKeyFrame.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	else
	{
		if (CurrentTime >= m_KeyFrames[m_iCurrentKeyFrame + 1].Time)
			++m_iCurrentKeyFrame;

		_double		Ratio = (CurrentTime - m_KeyFrames[m_iCurrentKeyFrame].Time) /
			(m_KeyFrames[m_iCurrentKeyFrame + 1].Time - m_KeyFrames[m_iCurrentKeyFrame].Time);

		_vector		vSourScale, vDestScale;
		_vector		vSourRotation, vDestRotation;
		_vector		vSourPosition, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vScale);
		vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1].vScale);

		vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vRotation);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame + 1].vRotation);

		vSourPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vPosition);
		vDestPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, Ratio);
		vRotation = XMVectorLerp(vSourRotation, vDestRotation, Ratio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, Ratio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	if (nullptr != m_pBone)
		m_pBone->SetUp_TransformationMatrix(TransformationMatrix);
}

CChannel* CChannel::Create(aiNodeAnim* pAIChannel, CModel* pModel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	__super::Free();

	Safe_Release(m_pBone);
}

