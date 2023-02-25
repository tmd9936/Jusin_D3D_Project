#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject, const _tchar* pObjectNameTag)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	if (pObjectNameTag)
	{
		auto iter = m_objectStore.find(pObjectNameTag);

		if (m_objectStore.end() != iter)
			return E_FAIL;

		m_objectStore.insert({ pObjectNameTag, pGameObject });
	}

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Tick(_double TimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Tick(TimeDelta);
	}
}

void CLayer::LateTick(_double TimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->LateTick(TimeDelta);
	}
}

HRESULT CLayer::Store_Component(CGameObject* pGameObject, const FamilyId& id)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_componentStore.insert({ id, pGameObject });
	return S_OK;
}

CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer();

	return pLayer;
}

void CLayer::Free()
{
	m_objectStore.clear();
	m_componentStore.clear();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);


	m_GameObjects.clear();
}

