#include"stdafx.h"

#include "SkillInfoUI.h"

#include "GameInstance.h"

#include "PartText.h"
#include "PartTexture.h"


CSkillInfoUI::CSkillInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CSkillInfoUI::CSkillInfoUI(const CSkillInfoUI& rhs)
	: CUI(rhs)
{
}

HRESULT CSkillInfoUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkillInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	//if (FAILED(Init_PokemonData()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CSkillInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	//if (FAILED(Init_PokemonData()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CSkillInfoUI::Change_SkillIcon(const _uint& skillIndex)
{
	return S_OK;
}


CSkillInfoUI* CSkillInfoUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkillInfoUI* pInstance = new CSkillInfoUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CSkillInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkillInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CSkillInfoUI* pInstance = new CSkillInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CSkillInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkillInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CSkillInfoUI* pInstance = new CSkillInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CSkillInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkillInfoUI::Free()
{
	__super::Free();
}
