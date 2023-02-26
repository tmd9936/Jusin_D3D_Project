#include "..\Public\Object_Manager.h"
#include "GameObject.h"
#include "Camera_Dynamic.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

HRESULT CObject_Manager::Reserve_Manager(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();
}

HRESULT CObject_Manager::Add_Layer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

/* 원형객체를 찾아 복제하여 레이어에 추가한다. */
HRESULT CObject_Manager::Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex, 
	const _tchar* pLayerTag, const _tchar* pObjectNameTag, void* pArg)
{
	/* 원형을 찾는다. */
	CGameObject* pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	/* 사본을 생성한다. */
	CGameObject* pGameObject = pPrototype->Clone(pLayerTag, iLevelIndex, pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pGameObject, pObjectNameTag)))
			return E_FAIL;

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
	{
		if (FAILED(pLayer->Add_GameObject(pGameObject, pObjectNameTag)))
			return E_FAIL;
	}

	if (pObjectNameTag)
	{
		pGameObject->Set_NameTag(pObjectNameTag);
	}

	return S_OK;
}

void CObject_Manager::Tick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Tick(TimeDelta);
		}
	}
}

void CObject_Manager::LateTick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->LateTick(TimeDelta);
		}
	}
}

CComponent* CObject_Manager::Get_Component(const FamilyId& familyId, CGameObject* pObj) const
{
	{
		if (nullptr == pObj)
			return nullptr;

		return pObj->Get_Component(familyId);
	}
}

CComponent* CObject_Manager::Get_Component(const FamilyId& familyId, _uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pObjectTag) const
{
	if (nullptr == pObjectTag || nullptr == pLayerTag)
		return nullptr;

	CGameObject* pObj = Get_Object(iLevelIndex, pLayerTag, pObjectTag);
	if (nullptr == pObj)
		return nullptr;

	return pObj->Get_Component(familyId);
}

HRESULT CObject_Manager::Remove_Component(const FamilyId& familyId, CGameObject* pObj)
{
	if (nullptr == pObj)
		return E_FAIL;

	if (FAILED(pObj->Remove_Component(familyId)))
	{
		return E_FAIL;
	}

	CLayer* pLayer = Find_Layer(pObj->Get_Levelindex(), pObj->Get_LayerTag().c_str());

	if (nullptr == pLayer)
		return E_FAIL;

	if (FAILED(pLayer->Remove_Component(familyId, pObj)))
		return E_FAIL;

	return S_OK;
}

CGameObject* CObject_Manager::Get_Object(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pObjectTag) const
{
	if (nullptr == pObjectTag || nullptr == pLayerTag)
		return nullptr;

	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	CGameObject* pResult = pLayer->Get_Object(pObjectTag);

	return pResult;
}

HRESULT CObject_Manager::Add_Component(const FamilyId& familyId, CGameObject* pGameObject, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	CComponent* pComponent = pGameObject->Get_Component(familyId);

	if (nullptr != pComponent)
		return E_FAIL;

	CComponent_Manager* pComponent_Manager = CComponent_Manager::GetInstance();
	//Safe_AddRef(pComponent_Manager);

	/* 원형객체를 복사하여 사본 컴포넌트를 만든다. */
	pComponent = pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pGameObject, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	//Safe_Release(pComponent_Manager);

	/* 부모의 맵컨테이너에 복제한 컴포넌트를 추가한다. */
	//m_Components.insert({ T::familyId, pComponent });
	if (FAILED(pGameObject->Add_Component(familyId, pComponent)))
		return E_FAIL;


	if (FAILED(Store_Component(pGameObject->Get_LayerTag().c_str(), pGameObject, familyId)))
		return E_FAIL;

	/* 자식에 변수에게도 공유시켜주었다. */
	if (nullptr != ppOut)
	{
		*ppOut = pComponent;
		Safe_AddRef(pComponent);
	}

	return S_OK;
}

HRESULT CObject_Manager::Store_Component(const _tchar* pLayerTag, class CGameObject* pGameObject, const FamilyId& id)
{
	if (nullptr == pLayerTag || nullptr == pGameObject)
		return E_FAIL;

	_uint objectiLevelIndex = pGameObject->Get_Levelindex();

	if (objectiLevelIndex >= m_iNumLevels)
		return E_FAIL;

	auto iter = Find_Layer(objectiLevelIndex, pLayerTag);

	if (nullptr == iter)
		return E_FAIL;

	HRESULT hr = iter->Store_Component(pGameObject, id);

	return hr;
}

CGameObject* CObject_Manager::Find_Prototype(const _tchar* pPrototypeTag) const
{
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder(pPrototypeTag));

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

CLayer* CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag) const
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	wstring tag = pLayerTag;
	auto	iter = m_pLayers[iLevelIndex].find(tag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);
		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);
	m_Prototypes.clear();

}
