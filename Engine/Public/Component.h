#pragma once

#include "Base.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CComponent abstract : public CBase
{
public:
	static const FamilyId familyId = 999;

protected:
	explicit CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	explicit CComponent(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	explicit CComponent(const CComponent& rhs, CGameObject* pOwner);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	virtual const FamilyId	Get_FamilyId() const { 
		return familyId; }
	virtual CGameObject* Get_Owner() const { 
		return m_pOwner; }
	virtual void Set_Owner(CGameObject* pOwner) { 
		m_pOwner = pOwner; }

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CGameObject*			m_pOwner = { nullptr };
	_bool					m_bClone = { true };

public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END