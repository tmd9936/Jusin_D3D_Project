#include "stdafx.h"
#include "GoToBaseCampButton.h"

#include "GameInstance.h"

#include "Level_Loading.h"

#include "PartTexture.h"

CGoToBaseCampButton::CGoToBaseCampButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CButton(pDevice, pContext)
{
}

CGoToBaseCampButton::CGoToBaseCampButton(const CGoToBaseCampButton& rhs)
	: CButton(rhs)
{
}

HRESULT CGoToBaseCampButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

/*

*/
HRESULT CGoToBaseCampButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CGoToBaseCampButton::On_Idle()
{
	return 0;
}

_uint CGoToBaseCampButton::On_Press()
{
	return 0;
}

_uint CGoToBaseCampButton::On_Select()
{
	return 0;
}

_uint CGoToBaseCampButton::On_Release()
{
	if (FAILED(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_BASECAMP))))
		return 0;
	else
		return OBJ_SCENE_CHNAGE;
}

CGoToBaseCampButton* CGoToBaseCampButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGoToBaseCampButton* pInstance = new CGoToBaseCampButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CGoToBaseCampButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoToBaseCampButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CGoToBaseCampButton* pInstance = new CGoToBaseCampButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CGoToBaseCampButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoToBaseCampButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CGoToBaseCampButton* pInstance = new CGoToBaseCampButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CGoToBaseCampButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoToBaseCampButton::Free()
{
	__super::Free();
}

