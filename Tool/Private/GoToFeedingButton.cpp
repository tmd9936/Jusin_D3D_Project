#include "stdafx.h"
#include "GoToFeedingButton.h"

#include "GameInstance.h"

#include "Level_Loading.h"

#include "PartTexture.h"

CGoToFeedingButton::CGoToFeedingButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CButton(pDevice, pContext)
{
}

CGoToFeedingButton::CGoToFeedingButton(const CGoToFeedingButton& rhs)
	: CButton(rhs)
{
}

HRESULT CGoToFeedingButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGoToFeedingButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CGoToFeedingButton::On_Idle()
{
	return 0;
}

_uint CGoToFeedingButton::On_Press()
{
	CGameInstance::GetInstance()->PlaySoundW(L"SE_SYS_SCENE_CHANGE_SHORT_MB.ogg", SOUND_UI);
	return 0;
}

_uint CGoToFeedingButton::On_Select()
{
	return 0;
}

_uint CGoToFeedingButton::On_Release()
{
	if (FAILED(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_FEEDING))))
		return 0;
	else
		return OBJ_SCENE_CHNAGE;
}

CGoToFeedingButton* CGoToFeedingButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGoToFeedingButton* pInstance = new CGoToFeedingButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CGoToFeedingButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoToFeedingButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CGoToFeedingButton* pInstance = new CGoToFeedingButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CGoToFeedingButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoToFeedingButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CGoToFeedingButton* pInstance = new CGoToFeedingButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CGoToFeedingButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoToFeedingButton::Free()
{
	__super::Free();
}

