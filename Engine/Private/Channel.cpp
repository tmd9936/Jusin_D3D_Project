#include "..\Public\Channel.h"
#include "Model.h"

#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(aiNodeAnim* pAIChannel, CModel* pModel)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	m_iBoneIndex = pModel->Get_BoneIndex(m_szName);

	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	_float3		vScale{};
	_float4		vRotation{};
	_float3		vPosition{};

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

void CChannel::Update(vector<CBone*>& Bones, _uint& pCurrentKeyFrame, _double CurrentTime, KEYFRAME& OutKeyFrame)
{
	if (0.0 == CurrentTime)
		pCurrentKeyFrame = 0;

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
		if (pCurrentKeyFrame + 1 < m_KeyFrames.size())
		{
			while (CurrentTime >= m_KeyFrames[pCurrentKeyFrame + 1].Time)
				++pCurrentKeyFrame;
		}

		_double		Ratio = (CurrentTime - m_KeyFrames[pCurrentKeyFrame].Time) /
			(m_KeyFrames[pCurrentKeyFrame + 1].Time - m_KeyFrames[pCurrentKeyFrame].Time);

		_vector		vSourScale, vDestScale;
		_vector		vSourRotation, vDestRotation;
		_vector		vSourPosition, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames[pCurrentKeyFrame].vScale);
		vDestScale = XMLoadFloat3(&m_KeyFrames[pCurrentKeyFrame + 1].vScale);

		vSourRotation = XMLoadFloat4(&m_KeyFrames[pCurrentKeyFrame].vRotation);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[pCurrentKeyFrame + 1].vRotation);

		vSourPosition = XMLoadFloat3(&m_KeyFrames[pCurrentKeyFrame].vPosition);
		vDestPosition = XMLoadFloat3(&m_KeyFrames[pCurrentKeyFrame + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, (_float)Ratio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, (_float)Ratio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, (_float)Ratio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	memcpy(&OutKeyFrame.vScale, &vScale, sizeof vScale);
	memcpy(&OutKeyFrame.vRotation, &vRotation, sizeof vRotation);
	memcpy(&OutKeyFrame.vPosition, &vPosition, sizeof vPosition);

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	Bones[m_iBoneIndex]->SetUp_TransformationMatrix(TransformationMatrix);
}

_bool CChannel::Update_Change_Animation_Lerp(vector<CBone*>& Bones, KEYFRAME& pPreKeyFrame, _uint& pCurrentKeyFrame, _double CurrentTime, _double EndTime)
{
	_vector			vScale, vRotation, vPosition;

	KEYFRAME	FrontKeyFrame = m_KeyFrames.front();

	_double		Ratio = CurrentTime / EndTime;
	if (Ratio >= 1.0)
	{
		vScale = XMLoadFloat3(&FrontKeyFrame.vScale);
		vRotation = XMLoadFloat4(&FrontKeyFrame.vRotation);
		vPosition = XMLoadFloat3(&FrontKeyFrame.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}
	else
	{
		_vector		vSourScale, vDestScale;
		_vector		vSourRotation, vDestRotation;
		_vector		vSourPosition, vDestPosition;

		vSourScale = XMLoadFloat3(&pPreKeyFrame.vScale);
		vDestScale = XMLoadFloat3(&FrontKeyFrame.vScale);

		vSourRotation = XMLoadFloat4(&pPreKeyFrame.vRotation);
		vDestRotation = XMLoadFloat4(&FrontKeyFrame.vRotation);

		vSourPosition = XMLoadFloat3(&pPreKeyFrame.vPosition);
		vDestPosition = XMLoadFloat3(&FrontKeyFrame.vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, (_float)Ratio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, (_float)Ratio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, (_float)Ratio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}
	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	Bones[m_iBoneIndex]->SetUp_TransformationMatrix(TransformationMatrix);

	return false;
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

CChannel* CChannel::Create()
{
	return new CChannel();
}

void CChannel::Free()
{
	m_KeyFrames.clear();
}

