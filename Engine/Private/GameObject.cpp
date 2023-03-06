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
	, m_strProtoTypeTag(rhs.m_strProtoTypeTag)
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

_bool CGameObject::Save_Args(const _tchar* filePath)
{
	HANDLE		hFile = Save_Args_Begin(filePath);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	if (false == Save_Args_Impl(hFile))
		return false;

	Save_Args_End(hFile);

	return true;
}

HANDLE CGameObject::Save_Args_Begin(const _tchar* filePath)
{
	HANDLE		hFile = CreateFile(filePath,
		GENERIC_WRITE,
		NULL, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	return hFile;
}

_bool CGameObject::Save_Args_Impl(HANDLE hFile)
{
	//DWORD   dwByte = 0;
	//int		id = 0;
	//WriteFile(hFile, &id, sizeof(int), &dwByte, NULL);

	return false;
}

_bool CGameObject::Save_Args_End(HANDLE hFile)
{
	return CloseHandle(hFile);
}

HRESULT CGameObject::Find_Component(const FamilyId& familyId) const
{
	auto	iter = m_Components.find(familyId);
	if (iter == m_Components.end())
		return E_FAIL;

	return S_OK;
}


void CGameObject::Free()
{
	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
