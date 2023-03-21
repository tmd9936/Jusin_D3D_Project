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

void CChannel::Update(vector<CBone*>& Bones, _uint& pCurrentKeyFrame, _double CurrentTime)
{
	if (0.0 == CurrentTime)
		pCurrentKeyFrame = 0;
	
	_vector			vScale, vRotation, vPosition;

	Make_KeyFrame(vScale, vRotation, vPosition, pCurrentKeyFrame, CurrentTime);

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	Bones[m_iBoneIndex]->SetUp_TransformationMatrix(TransformationMatrix);
}

_bool CChannel::Update_Change_Animation_Lerp(vector<CBone*>& Bones, KEYFRAME& pPreKeyFrame, _uint& pCurrentKeyFrame, _double CurrentTime, _float LerpRatio)
{
	_bool			result = true;
	_vector			vScale, vRotation, vPosition;
	_vector			vResultScale, vResultRotation, vResultPosition;

	if (0.0 == CurrentTime)
		pCurrentKeyFrame = 0;

	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

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

	vResultScale = XMVectorLerp(XMLoadFloat3(&pPreKeyFrame.vScale), vScale, (_float)LerpRatio);
	vResultRotation = XMQuaternionSlerp(XMLoadFloat4(&pPreKeyFrame.vRotation), vRotation, (_float)LerpRatio);
	vResultPosition = XMVectorLerp(XMLoadFloat3(&pPreKeyFrame.vPosition), vPosition, (_float)LerpRatio);
	vResultPosition = XMVectorSetW(vPosition, 1.f);
	
	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vResultScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vResultRotation, vResultPosition);

	Bones[m_iBoneIndex]->SetUp_TransformationMatrix(TransformationMatrix);

	if (fabs(XMVectorGetX(vResultScale) - XMVectorGetX(vScale)) > 0.001)
	{
		return false;
	}
	if (fabs(XMVectorGetY(vResultScale) - XMVectorGetY(vScale)) > 0.001)
	{
		return false;
	}
	if (fabs(XMVectorGetZ(vResultScale) - XMVectorGetZ(vScale)) > 0.001)
	{
		return false;
	}

	if (fabs(XMVectorGetX(vResultRotation) - XMVectorGetX(vRotation)) > 0.001)
	{
		return false;
	}
	if (fabs(XMVectorGetY(vResultRotation) - XMVectorGetY(vRotation)) > 0.001)
	{
		return false;
	}
	if (fabs(XMVectorGetZ(vResultRotation) - XMVectorGetZ(vRotation)) > 0.001)
	{
		return false;
	}
	if (fabs(XMVectorGetW(vResultRotation) - XMVectorGetW(vRotation)) > 0.001)
	{
		return false;
	}

	if (fabs(XMVectorGetX(vResultPosition) - XMVectorGetX(vPosition)) > 0.001)
	{
		return false;
	}
	if (fabs(XMVectorGetY(vResultPosition) - XMVectorGetY(vPosition)) > 0.001)
	{
		return false;
	}
	if (fabs(XMVectorGetZ(vResultPosition) - XMVectorGetZ(vPosition)) > 0.001)
	{
		return false;
	}

	return true;
}

void CChannel::Make_KeyFrame(_vector& vScale, _vector& vRotation, _vector& vPosition, _uint& pCurrentKeyFrame, _double CurrentTime)
{
	if (0.0 == CurrentTime)
		pCurrentKeyFrame = 0;

	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

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
}

void CChannel::Make_KeyFrame(KEYFRAME& OutKeyFrame, _uint& pCurrentKeyFrame, _double CurrentTime)
{
	if (0.0 == CurrentTime)
		pCurrentKeyFrame = 0;

	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

	if (CurrentTime >= LastKeyFrame.Time)
	{
		XMStoreFloat3(&OutKeyFrame.vScale, XMLoadFloat3(&LastKeyFrame.vScale));
		XMStoreFloat4(&OutKeyFrame.vRotation, XMLoadFloat4(&LastKeyFrame.vRotation));
		XMStoreFloat3(&OutKeyFrame.vPosition, XMLoadFloat3(&LastKeyFrame.vPosition));
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

		XMStoreFloat3(&OutKeyFrame.vScale, XMVectorLerp(vSourScale, vDestScale, (_float)Ratio));
		XMStoreFloat4(&OutKeyFrame.vRotation, XMQuaternionSlerp(vSourRotation, vDestRotation, (_float)Ratio));
		XMStoreFloat3(&OutKeyFrame.vPosition, XMVectorLerp(vSourPosition, vDestPosition, (_float)Ratio));
	}
}

void CChannel::Make_AffineMatrix(_matrix& OutMatrix, _uint& pCurrentKeyFrame, _double CurrentTime)
{
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

