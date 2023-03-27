#include "stdafx.h"
#include "GoToWorldMapButton.h"

#include "GameInstance.h"

#include "Level_Loading.h"

#include "PartTexture.h"

CGoToWorldMapButton::CGoToWorldMapButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CButton(pDevice, pContext)
{
}

CGoToWorldMapButton::CGoToWorldMapButton(const CGoToWorldMapButton& rhs)
	: CButton(rhs)
{
}

HRESULT CGoToWorldMapButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

/*
	
*/
HRESULT CGoToWorldMapButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CGoToWorldMapButton::On_Idle()
{
	return 0;
}

_uint CGoToWorldMapButton::On_Press()
{
	return 0;
}

_uint CGoToWorldMapButton::On_Select()
{
	return 0;
}

_uint CGoToWorldMapButton::On_Release()
{
	if (FAILED(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_WORLDMAP))))
		return 0;
	else
		return OBJ_SCENE_CHNAGE;
}

CGoToWorldMapButton* CGoToWorldMapButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGoToWorldMapButton* pInstance = new CGoToWorldMapButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CGoToWorldMapButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoToWorldMapButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CGoToWorldMapButton* pInstance = new CGoToWorldMapButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CGoToWorldMapButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoToWorldMapButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CGoToWorldMapButton* pInstance = new CGoToWorldMapButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CGoToWorldMapButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoToWorldMapButton::Free()
{
	__super::Free();
}

