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

	return 0;
}

_uint CGoToStageButton::On_Select()
{
	return 0;
}

_uint CGoToStageButton::On_Release()
{
	if (FAILED(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_STAGE))))
		return 0;
	else
		return OBJ_SCENE_CHNAGE;
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

