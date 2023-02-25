#pragma once

#include "Base.h"

/* 모아서 관리 : 내가 나누고 싶은 기준(CLayer)에 따라 레벨[] 별로 */
/* 1. 원형 객체들을 모아서 관리하다. */
/* 1. 사본 객체들을 모아서 관리하다. */
/* 2. 사본객체들의 Tick, LateTick무한으로 호출한다. */

BEGIN(Engine)

class CGameObject;
class CComponent;

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	void Clear(_uint iLevelIndex);

public:
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pObjectNameTag = nullptr, void* pArg = nullptr); /* 원형을 복제하여 사본을 추가한다. */
	void Tick(_double TimeDelta);
	void LateTick(_double TimeDelta);

public:
	HRESULT		Add_Component (const FamilyId& familyId, CGameObject* pGameObject, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg);

protected:
	HRESULT		Store_Component(const _tchar* pLayerTag, class CGameObject* pGameObject, const FamilyId& id);

private: /* 원형 객체들을 보관한다. */
	unordered_map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef unordered_map<const _tchar*, class CGameObject*>	PROTOTYPES;

private: /* 복제한 객체들을 그룹지어 레벨별로 보관한다. */
	unordered_map<const _tchar*, class CLayer*>* m_pLayers = { nullptr };
	typedef unordered_map<const _tchar*, class CLayer*>	LAYERS;

private:
	_uint		m_iNumLevels = { 0 };

private:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;
};

//inline HRESULT	CObject_Manager::Add_Component(const FamilyId& familyId, class CGameObject* pGameObject, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg)
//{
//	if (nullptr == pGameObject)
//		return E_FAIL;
//
//	CComponent* pComponent = Find_Component<T>();
//
//	if (nullptr != pComponent)
//		return E_FAIL;
//
//	CComponent_Manager* pComponent_Manager = CComponent_Manager::GetInstance();
//	//Safe_AddRef(pComponent_Manager);
//
//	/* 원형객체를 복사하여 사본 컴포넌트를 만든다. */
//	pComponent = pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pGameObject, pArg);
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
//	if (FAILED(Store_Component(m_strLayerTag.c_str(), pGameObject, T::familyId)))
//		return E_FAIL;
//
//	Safe_AddRef(pComponent);
//
//	return S_OK;
//}

END