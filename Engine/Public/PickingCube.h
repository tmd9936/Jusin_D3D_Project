#pragma once

#include "VIBuffer.h"
#include "Transform.h"


BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CPickingCube : public CVIBuffer
{
public:
	static const FamilyId familyId = FAMILY_ID_PICKING_CUBE;
private:
	explicit CPickingCube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CPickingCube(const CPickingCube& rhs, CGameObject* pOwner);
	virtual ~CPickingCube();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void On_Collision(CPickingCube* pOther, const _float& fX, const _float& fY, const _float& fZ); // call on collising
	void On_CollisionEnter(CPickingCube* pOther, const _float& fX, const _float& fY, const _float& fZ);
	void On_CollisionExit(CPickingCube* pOther, const _float& fX, const _float& fY, const _float& fZ);

public:
	virtual _uint LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	_bool Picking_By_Ray(_fvector vRayOrigin, _fvector vRayDiretion);

public:
	_uint Get_ID() { return m_iID; }

public:
	_float3* Get_MinVec() { return &m_vMin; }
	_float3* Get_MaxVec() { return &m_vMax; }
	_float3* Get_Center() { return &m_vCenter; }
	_float3* Get_VtxPos() { return m_vVtx; }

public:
	void   Set_CustomScale(const _float3* pCustomScale) { m_vCustomScale = *pCustomScale; }
	void   Set_Custom(_bool bCustom) { m_bCustomScale = bCustom; }
private:
	static _uint		g_iColliderID;
	_uint				m_iID = { 0 };

	_float3				m_vMin = { };
	_float3				m_vMax = { };

	VTXWIRE*			m_pVtxWire = { nullptr };
	FACEINDICES16*		m_pIndex = { nullptr };

	_float3				m_vVtx[8];
	_float3				m_vOriginVtx[8];
	_float3				m_vCenter = {};

	_bool				m_bCustomScale = { false };
	_float3				m_vCustomScale = {};


public:
	static CPickingCube* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;

private:
	virtual void Free();

	friend class CGameObject;
};

END