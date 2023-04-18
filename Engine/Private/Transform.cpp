#include "Transform.h"
#include "Shader.h"

#include "Navigation.h"

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

_float CTransform::Get_NoScaleRotateValue(STATE eState) const
{
	_vector stateVector = XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	_vector originalVector = {};
	if (eState == STATE_RIGHT)
	{
		originalVector = { 1.f, 0.f, 0.f, 0.f };
	}
	else if (eState == STATE_UP)
	{
		originalVector = { 0.f, 1.f, 0.f, 0.f };
	}
	else if (eState == STATE_LOOK)
	{
		originalVector = { 0.f, 0.f, 1.f, 0.f };
	}

	_float dot = XMVectorGetX(XMVector3Dot(stateVector, originalVector));

	_float radian = acosf(dot);

	return radian;
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

void CTransform::Set_Scaled(const _float3& vOrigin, const _float4x4& matrix)
{
	Set_State(STATE_RIGHT, XMVector3Normalize((XMLoadFloat4x4(&matrix).r[0])) * vOrigin.x);
	Set_State(STATE_UP, XMVector3Normalize((XMLoadFloat4x4(&matrix).r[1])) * vOrigin.y);
	Set_State(STATE_LOOK, XMVector3Normalize((XMLoadFloat4x4(&matrix).r[2])) * vOrigin.z);
}

void CTransform::Set_Scaled_XY(const _float4x4& matrix)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * XMVector3Normalize((XMLoadFloat4x4(&matrix).r[0])));
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * XMVector3Normalize((XMLoadFloat4x4(&matrix).r[1])));
}

void CTransform::Set_ScaledX(const _float& scale)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * scale);
}

void CTransform::Set_ScaledY(const _float& scale)
{
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * scale);

}

void CTransform::Set_ScaledZ(const _float& scale)
{
	Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * scale);

}

void CTransform::Move(float x, float y, float z, CNavigation* pNavigation)
{
	_bool isMove = true;

	_vector vPosition = Get_State(STATE_POSITION);

	vPosition = vPosition + XMVectorSet(x, y, z, 0.f);

	_float fY = XMVectorGetY(vPosition);

	if (nullptr != pNavigation)
	{
		isMove = pNavigation->Move_OnNavigation_Set_Y(vPosition, fY);
	}

	if (true == isMove)
	{
		if (nullptr != pNavigation)
			Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, fY + 0.5f));
		else
			Set_State(CTransform::STATE_POSITION, vPosition);
	}
}

void CTransform::Set_PositinoX(float x, CNavigation* pNavigation)
{
	_bool isMove = true;

	_vector vPosition = Get_State(STATE_POSITION);

	vPosition = XMVectorSetX(vPosition, x);

	_float fY = XMVectorGetY(vPosition);

	if (nullptr != pNavigation)
	{
		isMove = pNavigation->Move_OnNavigation_Set_Y(vPosition, fY);
	}

	if (true == isMove)
	{
		if (nullptr != pNavigation)
			Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, fY + 0.5f));
		else
			Set_State(CTransform::STATE_POSITION, vPosition);
	}
}

void CTransform::Set_PositionY(float y, CNavigation* pNavigation)
{
	_bool isMove = true;

	_vector vPosition = Get_State(STATE_POSITION);

	vPosition = XMVectorSetY(vPosition, y);

	_float fY = XMVectorGetY(vPosition);

	if (nullptr != pNavigation)
	{
		isMove = pNavigation->Move_OnNavigation_Set_Y(vPosition, fY);
	}

	if (true == isMove)
	{
		if (nullptr != pNavigation)
			Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, fY + 0.5f));
		else
			Set_State(CTransform::STATE_POSITION, vPosition);
	}
}

void CTransform::Set_PositionZ(float z, CNavigation* pNavigation)
{
	_bool isMove = true;

	_vector vPosition = Get_State(STATE_POSITION);

	vPosition = XMVectorSetZ(vPosition, z);

	_float fY = XMVectorGetY(vPosition);

	if (nullptr != pNavigation)
	{
		isMove = pNavigation->Move_OnNavigation_Set_Y(vPosition, fY);
	}

	if (true == isMove)
	{
		if (nullptr != pNavigation)
			Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, fY + 0.5f));
		else
			Set_State(CTransform::STATE_POSITION, vPosition);
	}
}

void CTransform::Go_Straight(_float TimeDelta, CNavigation* pNavigation)
{
	_bool	isMove = true;

	_vector vLook = Get_State(STATE_LOOK);
	_vector vPosition = Get_State(STATE_POSITION);

	_float fY = XMVectorGetY(vPosition);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.SpeedPerSec * m_SpeedPercent * TimeDelta;

	_vector vSlideLook = {};
	_vector vAxis = {};

	if (nullptr != pNavigation)
	{
		isMove = pNavigation->Move_OnNavigation_Sliding(vPosition, vLook, vSlideLook, vAxis);
	}

	if (true == isMove)
	{
		if (nullptr != pNavigation)
		{
			Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, fY));
		}
		else
			Set_State(CTransform::STATE_POSITION, vPosition);
	}
	else
	{
		if (nullptr != pNavigation)
		{
			_vector vSlidePosition = Get_State(STATE_POSITION);
			vSlidePosition += vSlideLook * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent * 1.5f;

			isMove = pNavigation->Move_OnNavigation(vSlidePosition);
			if (true == isMove)
			{
				Set_State(STATE_POSITION, XMVectorSetY(vSlidePosition, fY));
			}
			else
			{
				vSlideLook = XMVector3TransformNormal(vSlideLook, XMMatrixRotationAxis(vAxis, XMConvertToRadians(90.f)));

				_vector vSlidePosition = Get_State(STATE_POSITION);
				vSlidePosition += vSlideLook * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent * 1.5f;

				isMove = pNavigation->Move_OnNavigation(vSlidePosition);
				if (true == isMove)
				{
					Set_State(STATE_POSITION, XMVectorSetY(vSlidePosition, fY));
				}

			}
		}
	}
}

void CTransform::Go_Left(_float TimeDelta)
{
	_vector vlook = Get_State(STATE_RIGHT);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vlook) * m_TransformDesc.SpeedPerSec * m_SpeedPercent * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float TimeDelta)
{
	_vector vlook = Get_State(STATE_RIGHT);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vlook) * m_TransformDesc.SpeedPerSec * m_SpeedPercent * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float TimeDelta, CNavigation* pNavigation)
{
	_bool	isMove = true;

	_vector vLook = Get_State(STATE_LOOK);
	_vector vPosition = Get_State(STATE_POSITION);

	_float fY = XMVectorGetY(vPosition);

	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.SpeedPerSec * m_SpeedPercent * TimeDelta;

	_vector vSlideLook = {};
	_vector vAxis = {};

	if (nullptr != pNavigation)
	{
		isMove = pNavigation->Move_OnNavigation_Sliding(vPosition, vLook, vSlideLook, vAxis);
	}

	if (true == isMove)
	{
		if (nullptr != pNavigation)
		{
			Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, fY));
		}
		else
			Set_State(CTransform::STATE_POSITION, vPosition);
	}
	else
	{
		if (nullptr != pNavigation)
		{
			_vector vSlidePosition = Get_State(STATE_POSITION);
			vSlidePosition -= vSlideLook * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent * 1.5f;

			isMove = pNavigation->Move_OnNavigation(vSlidePosition);
			if (true == isMove)
			{
				Set_State(STATE_POSITION, XMVectorSetY(vSlidePosition, fY));
			}
			else
			{
				vSlideLook = XMVector3TransformNormal(vSlideLook, XMMatrixRotationAxis(vAxis, XMConvertToRadians(90.f)));

				_vector vSlidePosition = Get_State(STATE_POSITION);
				vSlidePosition -= vSlideLook * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent * 1.5f;

				isMove = pNavigation->Move_OnNavigation(vSlidePosition);
				if (true == isMove)
				{
					Set_State(STATE_POSITION, XMVectorSetY(vSlidePosition, fY));
				}

			}
		}
	}
}

void CTransform::Go_Up(_float TimeDelta)
{
	_vector vUp = Get_State(STATE_UP);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vUp) * m_TransformDesc.SpeedPerSec * m_SpeedPercent * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Down(_float TimeDelta)
{
	_vector vUp = Get_State(STATE_UP);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vUp) * m_TransformDesc.SpeedPerSec * m_SpeedPercent * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left_ByNavigation(_float TimeDelta, CNavigation* pNavigation)
{
	_bool	isMove = true;

	_vector vLook = Get_State(STATE_RIGHT);
	_vector vPosition = Get_State(STATE_POSITION);

	_float fY = XMVectorGetY(vPosition);

	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.SpeedPerSec * m_SpeedPercent * TimeDelta;

	_vector vSlideLook = {};
	_vector vAxis = {};

	if (nullptr != pNavigation)
	{
		isMove = pNavigation->Move_OnNavigation_Sliding(vPosition, vLook, vSlideLook, vAxis);
	}

	if (true == isMove)
	{
		if (nullptr != pNavigation)
		{
			Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, fY));
		}
		else
			Set_State(CTransform::STATE_POSITION, vPosition);
	}
	else
	{
		if (nullptr != pNavigation)
		{
			_vector vSlidePosition = Get_State(STATE_POSITION);
			vSlidePosition -= vSlideLook * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent * 1.5f;

			isMove = pNavigation->Move_OnNavigation(vSlidePosition);
			if (true == isMove)
			{
				Set_State(STATE_POSITION, XMVectorSetY(vSlidePosition, fY));
			}
			else
			{
				vSlideLook = XMVector3TransformNormal(vSlideLook, XMMatrixRotationAxis(vAxis, XMConvertToRadians(90.f)));

				_vector vSlidePosition = Get_State(STATE_POSITION);
				vSlidePosition -= vSlideLook * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent * 1.5f;

				isMove = pNavigation->Move_OnNavigation(vSlidePosition);
				if (true == isMove)
				{
					Set_State(STATE_POSITION, XMVectorSetY(vSlidePosition, fY));
				}

			}
		}
	}
}

void CTransform::Go_Right_ByNavigation(_float TimeDelta, CNavigation* pNavigation)
{
	_bool	isMove = true;

	_vector vLook = Get_State(STATE_RIGHT);
	_vector vPosition = Get_State(STATE_POSITION);

	_float fY = XMVectorGetY(vPosition);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.SpeedPerSec * m_SpeedPercent * TimeDelta;

	_vector vSlideLook = {};
	_vector vAxis = {};

	if (nullptr != pNavigation)
	{
		isMove = pNavigation->Move_OnNavigation_Sliding(vPosition, vLook, vSlideLook, vAxis);
	}

	if (true == isMove)
	{
		if (nullptr != pNavigation)
		{
			Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, fY));
		}
		else
			Set_State(CTransform::STATE_POSITION, vPosition);
	}
	else
	{
		if (nullptr != pNavigation)
		{
			_vector vSlidePosition = Get_State(STATE_POSITION);
			vSlidePosition += vSlideLook * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent * 1.5f;

			isMove = pNavigation->Move_OnNavigation(vSlidePosition);
			if (true == isMove)
			{
				Set_State(STATE_POSITION, XMVectorSetY(vSlidePosition, fY));
			}
			else
			{
				vSlideLook = XMVector3TransformNormal(vSlideLook, XMMatrixRotationAxis(vAxis, XMConvertToRadians(90.f)));

				_vector vSlidePosition = Get_State(STATE_POSITION);
				vSlidePosition += vSlideLook * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent * 1.5f;

				isMove = pNavigation->Move_OnNavigation(vSlidePosition);
				if (true == isMove)
				{
					Set_State(STATE_POSITION, XMVectorSetY(vSlidePosition, fY));
				}

			}
		}
	}
}

void CTransform::Go_Straight_No_Y(_float TimeDelta)
{
	_vector vlook = Get_State(STATE_LOOK);
	_vector vPosition = Get_State(STATE_POSITION);

	vlook = XMVectorSetY(vlook, 0.f);

	vPosition += XMVector3Normalize(vlook) * m_TransformDesc.SpeedPerSec * m_SpeedPercent * TimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Down_No_Y(_float TimeDelta)
{
	_vector vlook = Get_State(STATE_LOOK);
	_vector vPosition = Get_State(STATE_POSITION);

	vlook = XMVectorSetY(vlook, 0.f);

	vPosition -= XMVector3Normalize(vlook) * m_TransformDesc.SpeedPerSec * m_SpeedPercent * TimeDelta;

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

void CTransform::Rotation_Current_Coordination(_fvector vAxis, _float Radian)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

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

_bool CTransform::TurnToTarget(_fvector vAxis, _fvector vTargetPos, _float TimeDelta)
{
	_vector vPos = Get_State(STATE_POSITION);
	_vector vLook = vTargetPos - vPos;
	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);

	_float3		vScale = Get_Scaled();

	//_float dot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vLook), vLookTarget));
	_vector axis = {};
	if (XMVectorGetY(vUp) > 0.f)
	{
		axis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	}
	else
	{
		axis = XMVectorSet(0.f, -1.f, 0.f, 0.f);
	}

	_matrix RotationMatrix = XMMatrixRotationAxis(axis, m_TransformDesc.RotationPerSec * TimeDelta);

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(XMVector3Normalize(vRight) * vScale.x, RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(XMVector3Normalize(vUp) * vScale.y, RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(XMVector3Normalize(vLook) * vScale.z, RotationMatrix));

	return false;
	
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

void CTransform::BackAt(_fvector vTargetPos)
{
	_vector vPos = Get_State(STATE_POSITION);
	_vector vLook = vPos - vTargetPos;
	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);

	_float3		vScale = Get_Scaled();

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);

}

_float CTransform::Get_DistanceFromTarget(_fvector vTargetPos)
{
	_vector vPosition = Get_State(STATE_POSITION);

	_vector vDir = vTargetPos - vPosition;

	_float length = XMVectorGetX(XMVector3Length(vDir));

	return length;
}

_bool CTransform::Chase(_fvector vTargetPos, _float TimeDelta, _float limitDitance, CNavigation* pNavigation)
{
	//LookAt(vTargetPos);

	TurnToTarget(XMVectorSet(0.f, 1.f, 0.f, 0.f), vTargetPos, TimeDelta);

	_bool	isMove = true;

	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);

	_vector vDir = vTargetPos - vPosition;

	_float length = XMVectorGetX(XMVector3Length(vDir));

	if (length >= limitDitance)
	{
		vPosition += XMVector3Normalize(vDir) * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent;

		_vector vSlideLook = {};
		_vector vAxis = {};

		if (nullptr != pNavigation)
		{
			isMove = pNavigation->Move_OnNavigation_Sliding(vPosition, vLook, vSlideLook, vAxis);
		}

		if (true == isMove)
		{
			Set_State(STATE_POSITION, vPosition);
		}
		else
		{
			_vector vSlidePosition = Get_State(STATE_POSITION);
			vSlidePosition += vSlideLook * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent * 1.5;

			isMove = pNavigation->Move_OnNavigation(vSlidePosition);
			if (true == isMove)
			{
				Set_State(STATE_POSITION, vSlidePosition);
			}
			else
			{
				vSlideLook = XMVector3TransformNormal(vSlideLook, XMMatrixRotationAxis(vAxis, XMConvertToRadians(90.f)));

				_vector vSlidePosition = Get_State(STATE_POSITION);
				vSlidePosition += vSlideLook * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent * 1.5f;

				isMove = pNavigation->Move_OnNavigation(vSlidePosition);
				if (true == isMove)
				{
					Set_State(STATE_POSITION, vSlidePosition);
				}

			}
		}
		return false;
		
	}

	return true;
}

_bool CTransform::TurnAndChase(_fvector vTargetPos, _float TimeDelta, _float limitDitance)
{
	_vector vLook = Get_State(STATE_LOOK);

	_vector vPosition = Get_State(STATE_POSITION);

	_vector vDir = vTargetPos - vPosition;

	_float length = XMVectorGetX(XMVector3Length(vDir));

	if (length >= limitDitance)
	{
		vPosition += XMVector3Normalize(vDir) * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent;
		Set_State(STATE_POSITION, vPosition);

		return false;
	}

	return true;
}

_bool CTransform::ChaseNoLook(_fvector vTargetPos, _float TimeDelta, _float limitDitance, CNavigation* pNavigation)
{
	_bool	isMove = true;
	//LookAt(vTargetPos);

	_vector vPosition = Get_State(STATE_POSITION);

	_vector vDir = vTargetPos - vPosition;

	_float length = XMVectorGetX(XMVector3Length(vDir));

	if (length >= limitDitance)
	{
		vPosition += XMVector3Normalize(vDir) * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent;
		if (nullptr != pNavigation)
		{
			isMove = pNavigation->Move_OnNavigation(vPosition);
		}

		if (true == isMove)
		{
			Set_State(STATE_POSITION, vPosition);
		}

		return false;
	}

	return true;
}

_bool	CTransform::Chase_No_Y(_fvector vTargetPos, _float TimeDelta, _float limitDitance)
{
	_vector vPosition = Get_State(STATE_POSITION);

	_vector vDir = vTargetPos - vPosition;

	XMVectorSetY(vDir, 0.f);

	_float length = XMVectorGetX(XMVector3Length(vDir));

	if (length >= limitDitance)
	{
		vPosition += XMVector3Normalize(vDir) * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent;
		Set_State(STATE_POSITION, vPosition);

		return false;
	}

	return true;
}

_bool CTransform::Go_BackWard_Look_Pos(_fvector vLookPos, _fvector vArrivePos, _float TimeDelta, _float limitDitance, CNavigation* pNavigation)
{
	TurnToTarget(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLookPos, TimeDelta);

	_bool	isMove = true;

	_vector vPosition = Get_State(STATE_POSITION);

	_vector vDir = vArrivePos - vPosition;

	XMVectorSetX(vDir, 0.f);
	_float length = XMVectorGetX(XMVector3Length(vDir));

	if (length >= limitDitance)
	{
		vPosition += XMVector3Normalize(vDir) * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent;

		if (nullptr != pNavigation)
		{
			isMove = pNavigation->Move_OnNavigation(vPosition);
		}

		if (true == isMove)
		{
			Set_State(STATE_POSITION, vPosition);
		}

		return false;
	}

	return true;
}

_bool CTransform::Go_BackWard_Look_Target(_fvector vLookPos, _fvector vArrivePos, _float TimeDelta, _float limitDitance)
{
	//LookAt(vLookPos);

	_bool	isMove = true;

	_vector vPosition = Get_State(STATE_POSITION);

	_vector vDir = vArrivePos - vPosition;

	XMVectorSetX(vDir, 0.f);
	_float length = XMVectorGetX(XMVector3Length(vDir));

	if (length >= limitDitance)
	{
		vPosition += XMVector3Normalize(vDir) * TimeDelta * m_TransformDesc.SpeedPerSec * m_SpeedPercent;
		Set_State(STATE_POSITION, vPosition);
		
		return false;
	}

	return true;
}


void CTransform::Set_Rotation(const _float3& rotaion)
{
	m_Rotation = rotaion;

	_float3 vScale = Get_Scaled();

	_vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	_matrix		RotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMVectorSet(
		m_Rotation.x, m_Rotation.y, m_Rotation.z, 0.f
	));

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Set_SpeedPercent(_float percent)
{
	if (m_SpeedPercent > 1.8f || m_SpeedPercent < 0.5f)
		return;

	m_SpeedPercent = percent;
}

_float CTransform::Add_SpeedPercent(_float percent)
{
	_float preSpeedPercent = m_SpeedPercent;

	m_SpeedPercent += percent;
	_float returnValue = -percent;

	if (m_SpeedPercent > 1.8f)
	{
		m_SpeedPercent = 1.8f;
		returnValue = preSpeedPercent - m_SpeedPercent;
	}

	if (m_SpeedPercent < 0.5f)
	{
		m_SpeedPercent = 0.5f;
		returnValue = preSpeedPercent - m_SpeedPercent;
	}

	return returnValue;
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