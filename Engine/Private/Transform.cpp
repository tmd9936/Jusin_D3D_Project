#include "Transform.h"
#include "Shader.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CTransform::CTransform(const CTransform& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
	, m_WorldMatrix(rhs.m_WorldMatrix)
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


HRESULT CTransform::Set_ShaderResource(CShader* pShader, const char* pContantName)
{
	return pShader->Set_Matrix(pContantName, &m_WorldMatrix);
}

void CTransform::Set_Scaled(const _float3& vScale)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * vScale.z);
}

void CTransform::Set_Scaled(const _float4x4& matrix)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * XMVector3Normalize((XMLoadFloat4x4(&matrix).r[0])));
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * XMVector3Normalize((XMLoadFloat4x4(&matrix).r[1])));
	Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * XMVector3Normalize((XMLoadFloat4x4(&matrix).r[2])));

}

void CTransform::Set_Scaled_XY(const _float4x4& matrix)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * XMVector3Normalize((XMLoadFloat4x4(&matrix).r[0])));
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * XMVector3Normalize((XMLoadFloat4x4(&matrix).r[1])));
}

void CTransform::Go_Straight(_float TimeDelta)
{
	_vector vlook = Get_State(STATE_LOOK);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vlook) * m_TransformDesc.SpeedPerSec * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float TimeDelta)
{
	_vector vlook = Get_State(STATE_RIGHT);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vlook) * m_TransformDesc.SpeedPerSec * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float TimeDelta)
{
	_vector vlook = Get_State(STATE_RIGHT);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vlook) * m_TransformDesc.SpeedPerSec * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float TimeDelta)
{
	_vector vLook = Get_State(STATE_LOOK);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.SpeedPerSec * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Up(_float TimeDelta)
{
	_vector vUp = Get_State(STATE_UP);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vUp) * m_TransformDesc.SpeedPerSec * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Down(_float TimeDelta)
{
	_vector vUp = Get_State(STATE_UP);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vUp) * m_TransformDesc.SpeedPerSec * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Rotation(_fvector vAxis, _float Radian)
{
	_float3 vScale = Get_Scaled();

	_vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	//_vector vRot = XMQuaternionRotationAxis(vAxis, Radian);

	//XMVector3Rotate(vRight, vRot);
	//XMVector3Rotate(vUp, vRot);
	//XMVector3Rotate(vLook, vRot);

	//Set_State(STATE_RIGHT, vRight);
	//Set_State(STATE_UP, vUp);
	//Set_State(STATE_LOOK, vLook);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, Radian);

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Turn(_fvector vAxis, _float TimeDelta)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_TransformDesc.RotationPerSec * TimeDelta);

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));

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

void CTransform::Chase(_fvector vTargetPos, _float TimeDelta, _float limitDitance)
{
	LookAt(vTargetPos);

	_vector vPosition = Get_State(STATE_POSITION);

	_vector vDir = vTargetPos - vPosition;

	_float length = XMVectorGetX(XMVector3Length(vDir));

	if (length >= limitDitance)
	{
		vPosition += XMVector3Normalize(vDir) * TimeDelta * m_TransformDesc.SpeedPerSec;
		Set_State(STATE_POSITION, vPosition);
	}

}


void CTransform::Set_Rotation(const _float3& rotaion)
{
	m_Rotaion = rotaion;

	_float3 vScale = Get_Scaled();

	_vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	_matrix		RotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMVectorSet(
		m_Rotaion.x, m_Rotaion.y, m_Rotaion.z, 0.f
	));

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(CGameObject* pOwner, void* pArg)
{
	CTransform* pInstance = new CTransform(*this, pOwner);

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