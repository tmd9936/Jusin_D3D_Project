#include "stdafx.h"
#include "GoToBackLevelButton.h"

#include "GameInstance.h"

#include "Level_Loading.h"

#include "PartTexture.h"

LEVEL    CGoToBackLevelButton::m_BackLevel = LEVEL_WORLDMAP;

CGoToBackLevelButton::CGoToBackLevelButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CButton(pDevice, pContext)
{
}

CGoToBackLevelButton::CGoToBackLevelButton(const CGoToBackLevelButton& rhs)
	: CButton(rhs)
{
}

HRESULT CGoToBackLevelButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGoToBackLevelButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CGoToBackLevelButton::On_Idle()
{
	return 0;
}

_uint CGoToBackLevelButton::On_Press()
{
	CGameInstance::GetInstance()->PlaySoundW(L"SE_SYS_SCENE_CHANGE_SHORT.ogg", SOUND_UI);
	return 0;
}

_uint CGoToBackLevelButton::On_Select()
{
	return 0;
}

_uint CGoToBackLevelButton::On_Release()
{
	if (FAILED(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, m_BackLevel))))
		return 0;
	else
		return OBJ_SCENE_CHNAGE;
}

CGoToBackLevelButton* CGoToBackLevelButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGoToBackLevelButton* pInstance = new CGoToBackLevelButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CGoToBackLevelButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoToBackLevelButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CGoToBackLevelButton* pInstance = new CGoToBackLevelButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CGoToBackLevelButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoToBackLevelButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CGoToBackLevelButton* pInstance = new CGoToBackLevelButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CGoToBackLevelButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoToBackLevelButton::Free()
{
	__super::Free();
}

