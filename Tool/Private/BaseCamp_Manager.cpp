#include "stdafx.h"
#include "BaseCamp_Manager.h"

#include "GameInstance.h"

#include "Camera_Public.h"

#include <codecvt>


CBaseCamp_Manager::CBaseCamp_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CBaseCamp_Manager::CBaseCamp_Manager(const CBaseCamp_Manager& rhs)
	: CGameObject(rhs)
{

}

HRESULT CBaseCamp_Manager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBaseCamp_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	p_MainCamera = (CCamera_Public*)CGameInstance::GetInstance()->Get_Object(LEVEL_BASECAMP, L"Layer_Camera", L"Main_Camera");
	if (nullptr == p_MainCamera)
		return E_FAIL;
	Safe_AddRef(p_MainCamera);

	return S_OK;
}

HRESULT CBaseCamp_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(Add_Components_By_File()))
		return E_FAIL;

	p_MainCamera = (CCamera_Public*)CGameInstance::GetInstance()->Get_Object(LEVEL_BASECAMP, L"Layer_Camera", L"Main_Camera");
	if (nullptr == p_MainCamera)
		return E_FAIL;

	Safe_AddRef(p_MainCamera);

	return S_OK;
}

_uint CBaseCamp_Manager::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CBaseCamp_Manager::LateTick(_double TimeDelta)
{

	return _uint();
}

HRESULT CBaseCamp_Manager::Render()
{


	return S_OK;
}

_bool CBaseCamp_Manager::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	Value ManagerDesc(kObjectType);
	{
		Value pokemonFocusOffset(kObjectType);
		{
			pokemonFocusOffset.AddMember("x", m_Desc.pokemonFocusOffset.x, allocator);
			pokemonFocusOffset.AddMember("y", m_Desc.pokemonFocusOffset.y, allocator);
			pokemonFocusOffset.AddMember("z", m_Desc.pokemonFocusOffset.z, allocator);
			pokemonFocusOffset.AddMember("w", m_Desc.pokemonFocusOffset.w, allocator);
		}
		ManagerDesc.AddMember("pokemonFocusOffset", pokemonFocusOffset, allocator);

		ManagerDesc.AddMember("hideCommonUiTime", m_Desc.hideCommonUiTime, allocator);
		ManagerDesc.AddMember("visitInterval", m_Desc.visitInterval, allocator);
		ManagerDesc.AddMember("lookTime", m_Desc.lookTime, allocator);

	}
	doc.AddMember("ManagerDesc", ManagerDesc, allocator);
	

	return true;
}

_bool CBaseCamp_Manager::Load_By_JsonFile_Impl(Document& doc)
{
	const Value& ManagerDesc = doc["ManagerDesc"];

	const Value& pokemonFocusOffset = ManagerDesc["pokemonFocusOffset"];
	m_Desc.pokemonFocusOffset.x = pokemonFocusOffset["x"].GetFloat();
	m_Desc.pokemonFocusOffset.y = pokemonFocusOffset["y"].GetFloat();
	m_Desc.pokemonFocusOffset.z = pokemonFocusOffset["z"].GetFloat();
	m_Desc.pokemonFocusOffset.w = pokemonFocusOffset["w"].GetFloat();

	m_Desc.hideCommonUiTime = ManagerDesc["hideCommonUiTime"].GetFloat();
	m_Desc.lookTime = ManagerDesc["lookTime"].GetFloat();
	m_Desc.visitInterval = ManagerDesc["visitInterval"].GetFloat();


	return true;
}

void CBaseCamp_Manager::Picking()
{
	m_pPickingObject = m_pCalculator->Picking_Environment_Object(g_hWnd, L"Layer_Pokemon", LEVEL_BASECAMP);
	if (nullptr == m_pPickingObject)
		return;



}

HRESULT CBaseCamp_Manager::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CCalculator::familyId, this, LEVEL_BASECAMP, TEXT("Prototype_Component_Model_Pokemon_PM6"),
		(CComponent**)&m_pCalculator, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBaseCamp_Manager::Add_Components_By_File()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CCalculator::familyId, this, LEVEL_BASECAMP, TEXT("Prototype_Component_Model_Pokemon_PM6"),
		(CComponent**)&m_pCalculator, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBaseCamp_Manager::SetUp_ShaderResources()
{

	return S_OK;
}

CBaseCamp_Manager* CBaseCamp_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBaseCamp_Manager* pInstance = new CBaseCamp_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CBaseCamp_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBaseCamp_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CBaseCamp_Manager* pInstance = new CBaseCamp_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CBaseCamp_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBaseCamp_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CBaseCamp_Manager* pInstance = new CBaseCamp_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CBaseCamp_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaseCamp_Manager::Free()
{
	__super::Free();

	Safe_Release(p_MainCamera);

}
