#include"stdafx.h"

#include "StoneInfoUI.h"

#include "PartText.h"
#include "PartTexture.h"

StoneInfoUI::StoneInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

StoneInfoUI::StoneInfoUI(const StoneInfoUI& rhs)
	: CUI(rhs)
{
}

HRESULT StoneInfoUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT StoneInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT StoneInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint StoneInfoUI::Tick(_double TimeDelta)
{
	if (m_bShowStoneInfo)
	{
		for (auto& part : m_TextureParts)
		{
			part->Tick(TimeDelta);
		}

		for (auto& part : m_TextParts)
		{
			part->Tick(TimeDelta);
		}
	}

	return _uint();
}

_uint StoneInfoUI::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	if (m_bShowStoneInfo)
	{
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

HRESULT StoneInfoUI::Render()
{
	return __super::Render();
}

HRESULT StoneInfoUI::Change_StoneInfo(const CStone::STONE_DESC& desc)
{
	m_bShowStoneInfo = true;

	if (desc.m_stoneType == CStone::TYPE_ATK)
	{
		if (FAILED(m_TextureParts.at(m_StoneTextureIndex)->Change_Texture(L"Prototype_Component_Texture_UI_Pstone_attack")))
			return E_FAIL;

		m_TextureParts.at(m_NameBaseTextureIndex)->Set_Color({ 0.9294f, 0.24705f, 0.0705f, 1.f });
		m_TextParts.at(m_StoneNameText)->Set_Text(L"파워풀스톤");
		m_TextParts.at(m_StoneTypeText)->Set_Text(L"ATK");
	}
	else if (desc.m_stoneType == CStone::TYPE_HP)
	{
		if (FAILED(m_TextureParts.at(m_StoneTextureIndex)->Change_Texture(L"Prototype_Component_Texture_UI_Pstone_defense")))
			return E_FAIL;

		m_TextureParts.at(m_NameBaseTextureIndex)->Set_Color({ 0.1764f, 0.41176f, 0.76862f, 1.f });
		m_TextParts.at(m_StoneNameText)->Set_Text(L"터프니스스톤");
		m_TextParts.at(m_StoneTypeText)->Set_Text(L"HP");
	}

	m_TextParts.at(m_StoneValueText)->Set_Text(to_wstring(desc.value));
	m_TextParts.at(m_StoneValueText2)->Set_Text(L"+" + to_wstring(desc.value));

	return S_OK;
}

StoneInfoUI* StoneInfoUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	StoneInfoUI* pInstance = new StoneInfoUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created StoneInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* StoneInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	StoneInfoUI* pInstance = new StoneInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned StoneInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* StoneInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	StoneInfoUI* pInstance = new StoneInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned StoneInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void StoneInfoUI::Free()
{
	__super::Free();
}

