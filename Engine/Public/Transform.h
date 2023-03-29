#pragma once

#include "Component.h"

/*
Debug에 경우에는 _vector를 주소체로 계산해도 상관없지만
Release에 경우에는 참조형으로 계산을 해야해서 최대한 조심해야함
*/

BEGIN(Engine)

class CNavigation;

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
	_vector Get_State(STATE eState) const {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
		
	}

	_float3 Get_Scaled() const {
		return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
	}

	_float4x4 Get_WorldMatrix() const {
		return m_WorldMatrix;
	}

	_matrix Get_WorldMatrix_Matrix() const {
		return  XMLoadFloat4x4(&m_WorldMatrix);
	}

	_matrix Get_WorldMatrix_Inverse() const {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

	const _float3	Get_Rotate() const { 
		return m_Rotaion; 
	}

	HRESULT Set_ShaderResource(class CShader* pShader, const char* pContantName);

public:
	void	Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4((_float4*)m_WorldMatrix.m[eState], vState);
	}

	void Set_TransforDesc(const TRANSFORMDESC& transformDesc) {
		m_TransformDesc = transformDesc;
	}

	void Set_Scaled(const _float3& vScale);
	void Set_Scaled(const _float4x4& matrix);
	void Set_Scaled(const _float3& vOrigin, const _float4x4& matrix);

	void Set_Scaled_XY(const _float4x4& matrix);

	void Set_ScaledX(const _float& scale);
	void Set_ScaledY(const _float& scale);
	void Set_ScaledZ(const _float& scale);

	void Move_Pos(float x, float y, float z) {
		m_WorldMatrix.m[3][0] += x;
		m_WorldMatrix.m[3][1] += y;
		m_WorldMatrix.m[3][2] += z;
	}

	void Set_Pos(float x, float y, float z) {
		m_WorldMatrix.m[3][0] = x;
		m_WorldMatrix.m[3][1] = y;
		m_WorldMatrix.m[3][2] = z;
	}

	void Set_PosX(float x) {
		m_WorldMatrix.m[3][0] = x;
	}

	void Set_PosY(float y) {
		m_WorldMatrix.m[3][1] = y;
	}

	void Set_PosZ(float z) {
		m_WorldMatrix.m[3][2] = z;
	}

public:

	void Go_Straight(_float TimeDelta, CNavigation* pNavigation = nullptr);
	void Go_Left(_float TimeDelta);
	void Go_Right(_float TimeDelta);
	void Go_Backward(_float TimeDelta, CNavigation* pNavigation = nullptr);
	void Go_Up(_float TimeDelta);
	void Go_Down(_float TimeDelta);

	void Go_Straight_No_Y(_float TimeDelta);

	void Go_Down_No_Y(_float TimeDelta);


	void Rotation(_fvector vAxis, _float Radian);
	void Turn(_fvector vAxis, _float TimeDelta);

	_bool TurnToTarget(_fvector vAxis, _fvector vTargetPos, _float TimeDelta);

	void LookAt(_fvector vTargetPos);

	void BackAt(_fvector vTargetPos);


	/**
	@return False 움직이는 중, True 도착
	*/
	_bool Chase(_fvector vTargetPos, _float TimeDelta, _float limitDitance = 0.2, CNavigation* pNavigation = nullptr);

	_bool TurnAndChase(_fvector vTargetPos, _float TimeDelta, _float limitDitance = 0.2);

	_bool ChaseNoLook(_fvector vTargetPos, _float TimeDelta, _float limitDitance = 0.2);

	_bool Go_BackWard_Look_Target(_fvector vLookPos, _fvector vArrivePos, _float TimeDelta, _float limitDitance = 0.2);

	void Set_Rotation(const _float3& rotaion);

	void	Set_Matrix(_fmatrix matrix) {
		XMStoreFloat4x4(&m_WorldMatrix, matrix);
	}

private:
	_float4x4	m_WorldMatrix = { };
	TRANSFORMDESC m_TransformDesc = { };

	_float3		m_Rotaion = {};

public:
	// CComponent을(를) 통해 상속됨
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg) override;

	virtual	void	Free() override;
};

END