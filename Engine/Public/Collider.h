#pragma once

#include "Component.h"
#include "Transform.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CCollider : public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_COLLISION;
private:
	explicit CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CCollider(const CCollider& rhs, CGameObject* pOwner);
	virtual ~CCollider();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ); // call on collising
	void On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);
	void On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ);

public:
	HRESULT Ready_Collider();
	virtual	_int Update_Component(const _float& fTimeDelta);
	virtual	void LateUpdate_Component();
	virtual void Render_Collider();

public:
	void Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }
	_uint Get_ID() { return m_iID; }
	HRESULT Ready_Buffer(void);

public:
	_float3* Get_MinVec() { return &m_vMin; }
	_float3* Get_MaxVec() { return &m_vMax; }
	_float3* Get_Center() { return &m_vCenter; }
	_float3* Get_VtxPos() { return m_vVtx; }

public:
	void   Set_CustomScale(const _float3* pCustomScale) { m_vCustomScale = *pCustomScale; }
	void   Set_Custom(_bool bCustom) { m_bCustomScale = bCustom; }
private:
	static _uint g_iColliderID;
	_uint m_iID = { 0 };

	CGameObject* m_pOwner = { nullptr };

	_float3 m_vMin = { };
	_float3 m_vMax = { };

	ID3D11Buffer*	m_pVB = { nullptr };			// 수많은 정점들을 한데 묶어서 보관 또는 관리하기 위해 사용되는 객체
	ID3D11Buffer*	m_pIB = { nullptr };			// 정점들의 그리기 순서를 인덱스화 하여 보관 또는 관리하기 위해 사용되는 객체

	_ulong			m_dwVtxSize = { 0 };
	_ulong			m_dwVtxCnt = { 0 };
	_ulong			m_dwTriCnt = { 0 };
	_ulong			m_dwFVF = { 0 };

	_ulong			m_dwIdxSize;
	DXGI_FORMAT		m_IdxFmt;

	VTXWIRE*		m_pVtxWire;
	FACEINDICES16*	m_pIndex;

	_float3		m_vVtx[8] = {};
	_float3		m_vOriginVtx[8] = {};
	_float3		m_vCenter = {};

	_bool		m_bCustomScale = { false };

	_float3		m_vCustomScale = {};


public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;

private:
	virtual void Free();

	friend class CGameObject;
};

END