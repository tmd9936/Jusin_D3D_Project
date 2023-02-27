#include "..\Public\GameObject.h"

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr == pLayerTag)
		return E_FAIL;

	m_iLevelindex = iLevelIndex;
	m_strLayerTag = pLayerTag;

	return S_OK;
}

_uint CGameObject::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	return _uint();
}

_uint CGameObject::LateTick(_double TimeDelta)
{
	return _uint();
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

void CGameObject::Get_Components_FamilyId(vector<FamilyId>& vecFamilyIds)
{
	for (auto iter = m_Components.begin(); iter != m_Components.end(); ++iter)
	{
		vecFamilyIds.push_back(iter->first);
	}
}

HRESULT CGameObject::Add_Component(const FamilyId& familyId, CComponent* pComponent)
{
	CComponent* pGetComponent = Get_Component(familyId);

	if (nullptr != pGetComponent)
		return E_FAIL;

	m_Components.insert({ familyId, pComponent });

	return S_OK;
}

CComponent* CGameObject::Get_Component(const FamilyId& familyId) const
{
	auto	iter = m_Components.find(familyId);
	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Remove_Component(const FamilyId& familyId)
{
	auto iter = m_Components.find(familyId);

	if (iter == m_Components.end())
		return E_FAIL;

	Safe_Release(iter->second);
	m_Components.erase(iter);

	return S_OK;
}

HRESULT CGameObject::Find_Component(const FamilyId& familyId) const
{
	auto	iter = m_Components.find(familyId);
	if (iter == m_Components.end())
		return E_FAIL;

	return S_OK;
}

//HRESULT CGameObject::Add_Components(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg)
//{
//	if (nullptr != Find_Component(pComponentTag))
//		return E_FAIL;
//
//	CComponent_Manager* pComponent_Manager = CComponent_Manager::GetInstance();
//	Safe_AddRef(pComponent_Manager);
//
//	/* 원형객체를 복사하여 사본 컴포넌트를 만든다. */
//	CComponent* pComponent = pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
//	if (nullptr == pComponent)
//		return E_FAIL;
//
//	Safe_Release(pComponent_Manager);
//
//	/* 부모의 맵컨테이너에 복제한 컴포넌트를 추가한다. */
//	m_Components.emplace(pComponentTag, pComponent);
//
//	/* 자식에 변수에게도 공유시켜주었다. */
//	*ppOut = pComponent;
//
//	Safe_AddRef(pComponent);
//
//	return S_OK;
//}
//
//CComponent* CGameObject::Find_Component(const _tchar* pComponentTag)
//{
//	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));
//
//	if (iter == m_Components.end())
//		return nullptr;
//
//	return iter->second;
//}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
