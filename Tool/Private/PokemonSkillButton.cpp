#include "stdafx.h"
#include "PokemonSkillButton.h"

#include "GameInstance.h"

#include "Level_Loading.h"


CPokemonSkillButton::CPokemonSkillButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CButton(pDevice, pContext)
{
}

CPokemonSkillButton::CPokemonSkillButton(const CPokemonSkillButton& rhs)
	: CButton(rhs)
{
}

HRESULT CPokemonSkillButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

/*

*/
HRESULT CPokemonSkillButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CPokemonSkillButton::On_Idle()
{
	return 0;
}

_uint CPokemonSkillButton::On_Press()
{

	return 0;
}

_uint CPokemonSkillButton::On_Select()
{
	return 0;
}

_uint CPokemonSkillButton::On_Release()
{
	return 0;
}

CPokemonSkillButton* CPokemonSkillButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPokemonSkillButton* pInstance = new CPokemonSkillButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CPokemonSkillButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokemonSkillButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CPokemonSkillButton* pInstance = new CPokemonSkillButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CPokemonSkillButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokemonSkillButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CPokemonSkillButton* pInstance = new CPokemonSkillButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CPokemonSkillButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPokemonSkillButton::Free()
{
	__super::Free();
}

