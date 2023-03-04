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
		Safe_AddRef(pGameObject);
	}

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Tick(_double TimeDelta)
{
	_uint state = 0;

	//for (auto& pGameObject : m_GameObjects)
	//{
	//	if (nullptr != pGameObject)
	//	{
	//		state = pGameObject->Tick(TimeDelta);
	//	}
	//}

	for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end();)
	{
		if (nullptr != (*iter))
		{
			state = (*iter)->Tick(TimeDelta);
			if (state == OBJ_DEAD)
			{
				Remove_GameObject((*iter));
				iter = m_GameObjects.erase(iter);
				continue;
			}
			else
				++iter;
		}
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

	Safe_AddRef(pGameObject);

	m_componentStore.insert({ id, pGameObject });
	return S_OK;
}

CGameObject* CLayer::Get_Object(const _tchar* pObjectNameTag)
{
	auto iter = m_objectStore.find(pObjectNameTag);

	if (m_objectStore.end() == iter)
		return nullptr;

	return iter->second;
}

HRESULT CLayer::Remove_Component(const FamilyId& familyId, CGameObject* pObj)
{
	if (nullptr == pObj)
		return E_FAIL;

	auto iterPair = m_componentStore.equal_range(familyId);
	for (auto iter = iterPair.first; iter != iterPair.second; ++iter)
	{
		if (iter->second == pObj)
		{
			m_componentStore.erase(iter);
			return S_OK;
		}
	}

	return E_NOTIMPL;
}

void CLayer::Get_All_GameObject_In_Layer(vector<const CGameObject*>& result)
{
	for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); ++iter)
	{
		result.push_back((*iter));
	}
}

HRESULT CLayer::Erase_GameObject(CGameObject* pObj)
{
	BOOL	bFind = false;
	for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); ++iter)
	{
		if (*iter == pObj)
		{
			bFind = true;
			break;
		}
	}

	if (!bFind)
		return E_FAIL;

	for (auto objIter = pObj->m_Components.begin(); objIter != pObj->m_Components.end(); ++objIter)
	{
		auto iterPair = m_componentStore.equal_range(objIter->first);

		if (iterPair.first == iterPair.second)
			continue;

		for (auto storeIter = iterPair.first; storeIter != iterPair.second; ++storeIter)
		{
			if (storeIter->second == pObj)
			{
				Safe_Release(storeIter->second);
				m_componentStore.erase(storeIter);
				break;
			}
		}
	}

	auto objectStoreIter = m_objectStore.find(pObj->Get_NameTag().c_str());

	if (m_objectStore.end() != objectStoreIter)
	{
		Safe_Release(objectStoreIter->second);
		m_objectStore.erase(objectStoreIter);
	}

	return S_OK;
}

HRESULT CLayer::Remove_GameObject(CGameObject* pObj)
{
	if (S_OK == Erase_GameObject(pObj))
	{
		Safe_Release(pObj);
		pObj = nullptr;
		return S_OK;
	}

	return E_FAIL;
}

CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer();

	return pLayer;
}

void CLayer::Free()
{
	for (auto& iter : m_objectStore)
		Safe_Release(iter.second);
	m_objectStore.clear();

	for (auto& iter : m_componentStore)
		Safe_Release(iter.second);
	m_componentStore.clear();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);
	m_GameObjects.clear();
}

