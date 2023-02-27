#pragma once

#include "Base.h"
#include "Component_Manager.h"
#include "Object_Manager.h"

BEGIN(Engine)

class CComponent;

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual _uint LateTick(_double TimeDelta);
	virtual HRESULT Render();


public:
	const wstring			Get_NameTag() const {
		return m_strNameTag; }

	void					Set_NameTag(wstring& strName) {
		m_strNameTag = strName; 
	}
	void					Set_NameTag(const _tchar* strName) {
		m_strNameTag = strName; 
	}

	const RENDERGROUP		Get_RenderId() const {
		return m_eRenderId; 
	}
	void					Set_RenderId(RENDERGROUP eRenderId) {
		m_eRenderId = eRenderId; 
	}

	const wstring			Get_LayerTag() const {
		return m_strLayerTag; 
	}

	void					Set_LayerTag(const wstring& strLayerTag) {
		m_strLayerTag = strLayerTag; 
	}

	void					Set_LayerTag(const _tchar* pLayerTag) {
		m_strLayerTag = pLayerTag;
	}

	const _uint				Get_Levelindex() const {
		return m_iLevelindex; 
	}
	void					Set_Levelindex(const _uint& iLevelindex) {
		m_iLevelindex = iLevelindex; 
	}

public:
	_bool					Is_Dead() { return m_bDead; }
	void					Set_Alive() { m_bDead = false; }
	void					Set_Dead() { m_bDead = true; }


public:
	void					Get_Components_FamilyId(vector<FamilyId>& vecFamilyIds);

	HRESULT					Add_Component(const FamilyId& familyId, CComponent* pComponent);
	CComponent*				Get_Component(const FamilyId& familyId) const;

	HRESULT					Remove_Component(const FamilyId& familyId);
private:
	HRESULT					Find_Component(const FamilyId& familyId) const;

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

protected:
	/* 해시테이블 */
	friend CLayer;
	map<FamilyId, class CComponent*>				m_Components;
	wstring											m_strNameTag;
	wstring											m_strLayerTag;
	RENDERGROUP										m_eRenderId = { RENDER_END };

	_bool											m_bDead = { false };
	_uint											m_iLevelindex = { 0 };


public:
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) = 0;
	virtual void Free() override;
};


END