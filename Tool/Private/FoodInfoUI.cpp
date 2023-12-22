#include"stdafx.h"

#include "FoodInfoUI.h"

#include "GameInstance.h"

CFoodInfoUI::CFoodInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CFoodInfoUI::CFoodInfoUI(const CFoodInfoUI& rhs)
	: CUI(rhs)
{
}

HRESULT CFoodInfoUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFoodInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_FoodInfoUIDesc = (*(FOODINFOUI_DESC*)(pArg));
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &m_FoodInfoUIDesc.m_UIDesc)))
		return E_FAIL;

	m_eRenderId = RENDER_UI;

	return S_OK;
}

HRESULT CFoodInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	m_eRenderId = RENDER_UI;

	return S_OK;
}

_uint CFoodInfoUI::Tick(_double TimeDelta)
{

	return __super::Tick(TimeDelta);
}

_uint CFoodInfoUI::LateTick(_double TimeDelta)
{
	if (m_bLateTick)
	{
		m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

		for (auto& part : m_TextureParts)
		{
			part->LateTick(TimeDelta);
		}

		for (auto& part : m_TextParts)
		{
			part->LateTick(TimeDelta);
		}
	}

	return _uint();
}

HRESULT CFoodInfoUI::Render()
{
	return __super::Render();
}

HRESULT CFoodInfoUI::Set_FoodInfo(CFood::TYPE eType, const _uint& foodNum)
{
	m_foodNum = foodNum;

	switch (eType)
	{
	case Client::CFood::TYPE_BLUE:
		m_TextureParts.at(m_FoodTextureIndex)->Change_Texture(L"Prototype_Component_Texture_Item_Blue_UC");
		break;
	case Client::CFood::TYPE_RED:
		m_TextureParts.at(m_FoodTextureIndex)->Change_Texture(L"Prototype_Component_Texture_Item_Red_UC");
		break;
	case Client::CFood::TYPE_GREY:
		m_TextureParts.at(m_FoodTextureIndex)->Change_Texture(L"Prototype_Component_Texture_Item_Grey_UC");
		break;
	case Client::CFood::TYPE_YELLOW:
		m_TextureParts.at(m_FoodTextureIndex)->Change_Texture(L"Prototype_Component_Texture_Item_Yellow_UC");
		break;
	}

	m_TextParts.at(m_FoodNumTextIndex)->Set_Text(to_wstring(foodNum));

	return S_OK;
}

HRESULT CFoodInfoUI::Set_FoodInfo(const _uint& foodNum)
{
	m_foodNum = foodNum;

	m_TextParts.at(m_FoodNumTextIndex)->Set_Text(to_wstring(foodNum));

	return S_OK;
}

void CFoodInfoUI::All_Object_RenderOff()
{
	m_bLateTick = false;

	for (auto& part : m_TextureParts)
	{
		part->Set_RenderId(RENDER_END);
	}

	for (auto& part : m_TextParts)
	{
		part->Set_RenderId(RENDER_END);
	}
}

CFoodInfoUI* CFoodInfoUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFoodInfoUI* pInstance = new CFoodInfoUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CFoodInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFoodInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CFoodInfoUI* pInstance = new CFoodInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CFoodInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFoodInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CFoodInfoUI* pInstance = new CFoodInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CFoodInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFoodInfoUI::Free()
{
	__super::Free();
}
