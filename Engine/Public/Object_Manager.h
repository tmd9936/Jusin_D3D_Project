#pragma once

#include "Base.h"

/* ��Ƽ� ���� : ���� ������ ���� ����(CLayer)�� ���� ����[] ���� */
/* 1. ���� ��ü���� ��Ƽ� �����ϴ�. */
/* 1. �纻 ��ü���� ��Ƽ� �����ϴ�. */
/* 2. �纻��ü���� Tick, LateTick�������� ȣ���Ѵ�. */

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
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pObjectNameTag = nullptr, void* pArg = nullptr); /* ������ �����Ͽ� �纻�� �߰��Ѵ�. */
	void Tick(_double TimeDelta);
	void LateTick(_double TimeDelta);

public:
	HRESULT		Add_Component (const FamilyId& familyId, CGameObject* pGameObject, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg);

protected:
	HRESULT		Store_Component(const _tchar* pLayerTag, class CGameObject* pGameObject, const FamilyId& id);

private: /* ���� ��ü���� �����Ѵ�. */
	unordered_map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef unordered_map<const _tchar*, class CGameObject*>	PROTOTYPES;

private: /* ������ ��ü���� �׷����� �������� �����Ѵ�. */
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
//	/* ������ü�� �����Ͽ� �纻 ������Ʈ�� �����. */
//	pComponent = pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pGameObject, pArg);
//	if (nullptr == pComponent)
//		return E_FAIL;
//
//	//Safe_Release(pComponent_Manager);
//
//	/* �θ��� �������̳ʿ� ������ ������Ʈ�� �߰��Ѵ�. */
//	m_Components.insert({ T::familyId, pComponent });
//
//	/* �ڽĿ� �������Ե� ���������־���. */
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