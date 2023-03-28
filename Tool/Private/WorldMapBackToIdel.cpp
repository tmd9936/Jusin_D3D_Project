#include "stdafx.h"
#include "WorldMapBackToIdel.h"

#include "GameInstance.h"

#include "WorldMap_Manager.h"


CWorldMapBackToIdel::CWorldMapBackToIdel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CButton(pDevice, pContext)
{
}

CWorldMapBackToIdel::CWorldMapBackToIdel(const CWorldMapBackToIdel& rhs)
	: CButton(rhs)
{
}

HRESULT CWorldMapBackToIdel::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

/*

*/
HRESULT CWorldMapBackToIdel::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CWorldMapBackToIdel::On_Idle()
{
	return 0;
}

_uint CWorldMapBackToIdel::On_Press()
{

	return 0;
}

_uint CWorldMapBackToIdel::On_Select()
{
	return 0;
}

_uint CWorldMapBackToIdel::On_Release()
{
	CGameObject* pWorldMap_Manager = CGameInstance::GetInstance()->Get_Object(LEVEL_WORLDMAP, L"Layer_Manager", L"WorldMap_Manager");

	if (nullptr == pWorldMap_Manager)
		return 0;

	dynamic_cast<CWorldMap_Manager*>(pWorldMap_Manager)->Be_Idle();

	m_TickResult = 0;
	return 0;
}

CWorldMapBackToIdel* CWorldMapBackToIdel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorldMapBackToIdel* pInstance = new CWorldMapBackToIdel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CWorldMapBackToIdel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWorldMapBackToIdel::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CWorldMapBackToIdel* pInstance = new CWorldMapBackToIdel(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CWorldMapBackToIdel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWorldMapBackToIdel::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CWorldMapBackToIdel* pInstance = new CWorldMapBackToIdel(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CWorldMapBackToIdel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWorldMapBackToIdel::Free()
{
	__super::Free();
}

