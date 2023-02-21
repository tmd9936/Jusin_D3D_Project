#include "Transform.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
{
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof m_TransformDesc);

	return S_OK;
}


void CTransform::Go_Straight(_double TimeDelta)
{
	_vector vlook = Get_State(STATE_LOOK);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vlook) * m_TransformDesc.SpeedPerSec * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_double TimeDelta)
{
	_vector vlook = Get_State(STATE_RIGHT);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vlook) * m_TransformDesc.SpeedPerSec * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_double TimeDelta)
{
	_vector vlook = Get_State(STATE_RIGHT);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vlook) * m_TransformDesc.SpeedPerSec * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_double TimeDelta)
{
	_vector vLook = Get_State(STATE_LOOK);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.SpeedPerSec * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Rotation(_fvector vAxis, _double Radian)
{
	_float3 vScale = Get_Scaled();

	_vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	_vector vRot = XMQuaternionRotationAxis(vAxis, Radian);

	XMVector3Rotate(vRight, vRot);
	XMVector3Rotate(vUp, vRot);
	XMVector3Rotate(vLook, vRot);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);

	//_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, Radian);

	//Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	//Set_State(CTransform::STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	//Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Turn(_fvector vAxis, _double TimeDelta)
{
	_vector vRight = XMVector3Normalize(Get_State(STATE_RIGHT));
	_vector vUp = XMVector3Normalize(Get_State(STATE_UP));
	_vector vLook = XMVector3Normalize(Get_State(STATE_LOOK));


	_vector vRot = XMQuaternionRotationAxis(vAxis, m_TransformDesc.RotationPerSec  * TimeDelta);

	_float3		vScale = Get_Scaled();

	XMVector3Rotate(vRight, vRot);
	XMVector3Rotate(vUp, vRot);
	XMVector3Rotate(vLook, vRot);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);

	//_float3		vScale = Get_Scaled();

	//Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	//Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	//Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::LookAt(_fvector vTargetPos)
{
	_vector vPos = Get_State(STATE_POSITION);
	_vector vLook = vTargetPos - vPos;
	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);

	_float3		vScale = Get_Scaled();

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);

}

void CTransform::Chase(_fvector vTargetPos, _double TimeDelta, _double limitDitance)
{
	LookAt(vTargetPos);

	Go_Straight(TimeDelta);
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}