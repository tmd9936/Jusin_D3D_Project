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
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Tick(_double TimeDelta);
	virtual _uint LateTick(_double TimeDelta);
	virtual HRESULT Render();


public:
	const wstring			Get_NameTag() const { return m_strNameTag; }
	void					Set_NameTag(wstring& strName) { m_strNameTag = strName; }
	void					Set_NameTag(const _tchar* strName) { m_strNameTag = strName; }

	const RENDERGROUP		Get_RenderId() const { return m_eRenderId; }
	void					Set_RenderId(RENDERGROUP eRenderId) { m_eRenderId = eRenderId; }

	const wstring			Get_LayerTag() const { return m_strLayerTag; }
	void					Set_LayerTag(const wstring& strLayerTag) { m_strLayerTag = strLayerTag; }

	const _uint				Get_Levelindex() const { return m_iLevelindex; }
	void					Set_Levelindex(const _uint& iLevelindex) { m_iLevelindex = iLevelindex; }

public:
	_bool					Is_Dead() { return m_bDead; }
	void					Set_Alive() { m_bDead = false; }
	void					Set_Dead() { m_bDead = true; }

//protected:
//	HRESULT Add_Components(_uint iLevelIndex, const _tchar* pPrototypeTag,
//		const _tchar* pComponentTag, class CComponent** ppOut, void* pArg = nullptr);
//
//	class CComponent* Find_Component(const _tchar* pComponentTag);

public:
	void			Get_Components_FamilyId(vector<FamilyId>& vecFamilyIds);

public:
	/*template<typename T, typename = std::enable_if<is_base_of<CComponent, T>::value>>
	CComponent*		Get_Component();*/

	//template<typename T, typename = std::enable_if<is_base_of<CComponent, T>::value>>
	//HRESULT			Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg);


//private:
//	template<typename T, typename = std::enable_if<is_base_of<CComponent, T>::value>>
//	CComponent*		Find_Component();
//
//	template<typename T, typename = std::enable_if<is_base_of<CComponent, T>::value>>
//	HRESULT			Remove_Component();

	HRESULT					Add_Component(const FamilyId& familyId, CComponent* pComponent);
	CComponent*				Get_Component(const FamilyId& familyId);

	HRESULT					Remove_Component(const FamilyId& familyId);
private:
	HRESULT					Find_Component(const FamilyId& familyId);

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

protected:
	/* 해시테이블 */
	map<FamilyId, class CComponent*>				m_Components;
	wstring											m_strNameTag = { };
	wstring											m_strLayerTag = { };
	RENDERGROUP										m_eRenderId = { RENDER_END };

	_bool											m_bDead = { false };
	_uint											m_iLevelindex = { 0 };


public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

//template<typename T, typename>
//inline CComponent* CGameObject::Find_Component()
//{
//	auto	iter = m_Components.find(T::familyId);
//	if (iter == m_Components.end())
//		return nullptr;
//
//	return iter->second;
//}
//
//template<typename T, typename>
//inline CComponent* CGameObject::Get_Component()
//{
//	CComponent* pComponent = Find_Component<T>();
//
//	if (nullptr == pComponent)
//		return nullptr;
//
//	return pComponent;
//}

//template<typename T, typename>
//inline HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg)
//{
//	CComponent* pComponent = Find_Component<T>();
//
//	if (nullptr != pComponent)
//		return E_FAIL;
//
//	CComponent_Manager* pComponent_Manager = CComponent_Manager::GetInstance();
//	//Safe_AddRef(pComponent_Manager);
//
//	/* 원형객체를 복사하여 사본 컴포넌트를 만든다. */
//	pComponent = pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, this, pArg);
//	if (nullptr == pComponent)
//		return E_FAIL;
//
//	//Safe_Release(pComponent_Manager);
//
//	/* 부모의 맵컨테이너에 복제한 컴포넌트를 추가한다. */
//	m_Components.insert({ T::familyId, pComponent });
//
//	/* 자식에 변수에게도 공유시켜주었다. */
//	*ppOut = pComponent;
//
//	if (FAILED(CObject_Manager::GetInstance()->Store_Component(m_strLayerTag.c_str(), this, T::familyId)))
//		return E_FAIL;
//
//	Safe_AddRef(pComponent);
//
//	return S_OK;
//}

//template<typename T, typename>
//inline HRESULT CGameObject::Remove_Component()
//{
//	CComponent* pComponent = Find_Component<T>();
//
//	if (nullptr == pComponent)
//		return E_FAIL;
//
//	auto iter = m_Components.find(T::familyId);
//
//	if (iter == m_Components.end())
//		return E_FAIL;
//
//	Safe_Release(iter->second);
//	m_Components.erase(iter);
//
//	return S_OK;
//}


END