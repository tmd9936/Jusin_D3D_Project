#include"stdafx.h"

#include "StoneInfoUI.h"

#include "PartText.h"
#include "PartTexture.h"

CStoneInfoUI::CStoneInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CStoneInfoUI::CStoneInfoUI(const CStoneInfoUI& rhs)
	: CUI(rhs)
{
}

HRESULT CStoneInfoUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStoneInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStoneInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CStoneInfoUI::Tick(_double TimeDelta)
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

_uint CStoneInfoUI::LateTick(_double TimeDelta)
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

HRESULT CStoneInfoUI::Render()
{
	return __super::Render();
}

HRESULT CStoneInfoUI::Change_StoneInfo(const CStone::STONE_DESC& desc)
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

void CStoneInfoUI::Set_ShowStoneInfo(const _bool& bShowInfo)
{
	m_bShowStoneInfo = bShowInfo;
}

CStoneInfoUI* CStoneInfoUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStoneInfoUI* pInstance = new CStoneInfoUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStoneInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStoneInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStoneInfoUI* pInstance = new CStoneInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStoneInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStoneInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStoneInfoUI* pInstance = new CStoneInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStoneInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStoneInfoUI::Free()
{
	__super::Free();
}

