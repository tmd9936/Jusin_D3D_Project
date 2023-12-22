#include "stdafx.h"
#include "GoToStageButton.h"

#include "GameInstance.h"

#include "WorldMap_Manager.h"

#include "Level_Loading.h"


CGoToStageButton::CGoToStageButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CButton(pDevice, pContext)
{
}

CGoToStageButton::CGoToStageButton(const CGoToStageButton& rhs)
	: CButton(rhs)
{
}

HRESULT CGoToStageButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

/*

*/
HRESULT CGoToStageButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CGoToStageButton::On_Idle()
{
	return 0;
}

_uint CGoToStageButton::On_Press()
{
	CGameInstance::GetInstance()->PlaySoundW(L"SE_WORLD_STAGE_ENTER.ogg", SOUND_UI);

	return 0;
}

_uint CGoToStageButton::On_Select()
{
	return 0;
}

_uint CGoToStageButton::On_Release()
{
	CGameInstance::GetInstance()->PlaySoundW(L"SE_GAME_START.ogg", Engine::SOUND_STAGE_UI);
	CGameObject* pGameObject = CGameInstance::GetInstance()->Get_Object(LEVEL_WORLDMAP, L"Layer_Manager", L"WorldMap_Manager");
	if (nullptr != pGameObject)
	{
		CWorldMap_Manager* pWorldMapManager = dynamic_cast<CWorldMap_Manager*>(pGameObject);
		if (nullptr != pWorldMapManager)
			pWorldMapManager->Stage_Select();
	}
	return 0;
}

CGoToStageButton* CGoToStageButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGoToStageButton* pInstance = new CGoToStageButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CGoToStageButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoToStageButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CGoToStageButton* pInstance = new CGoToStageButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CGoToStageButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoToStageButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CGoToStageButton* pInstance = new CGoToStageButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CGoToStageButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoToStageButton::Free()
{
	__super::Free();
}

