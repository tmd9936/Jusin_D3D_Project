#pragma once

#include "Component.h"

/*
Debug에 경우에는 _vector를 주소체로 계산해도 상관없지만
Release에 경우에는 참조형으로 계산을 해야해서 최대한 조심해야함
*/

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_TRANSFORM;

public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float SpeedPerSec;
		_float RotationPerSec;
	} TRANSFORMDESC;

private:
	explicit CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CTransform(const CTransform& rhs, CGameObject* pOwner);
	virtual ~CTransform() = default;

public:
	virtual const FamilyId	Get_FamilyId() const { return familyId; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
		
	}

	_float3 Get_Scaled() {
		return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
	}

	_matrix Get_WorldMatrix_Inverse() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

public:
	void	Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4((_float4*)m_WorldMatrix.m[eState], vState);
	}

	void Set_TransforDesc(const TRANSFORMDESC& transformDesc) {
		m_TransformDesc = transformDesc;
	}

	void Set_Scaled(const _float3& vScale);

public:

	void Go_Straight(_float TimeDelta);
	void Go_Left(_float TimeDelta);
	void Go_Right(_float TimeDelta);
	void Go_Backward(_float TimeDelta);

	void Rotation(_fvector vAxis, _float Radian);
	void Turn(_fvector vAxis, _float TimeDelta);

	void LookAt(_fvector vTargetPos);

	void Chase(_fvector vTargetPos, _float TimeDelta, _float limitDitance = 0.2);


private:
	_float4x4	m_WorldMatrix = { };
	TRANSFORMDESC m_TransformDesc = { };

public:
	// CComponent을(를) 통해 상속됨
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg) override;

	virtual	void	Free() override;
};

END