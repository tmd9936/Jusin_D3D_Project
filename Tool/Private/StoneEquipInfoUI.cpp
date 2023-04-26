#include"stdafx.h"

#include "StoneEquipInfoUI.h"

#include "GameInstance.h"

#include "PartText.h"
#include "PartTexture.h"

#include "Skill_Manager.h"

CStoneEquipInfoUI::CStoneEquipInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CStoneEquipInfoUI::CStoneEquipInfoUI(const CStoneEquipInfoUI& rhs)
	: CUI(rhs)
{
}

HRESULT CStoneEquipInfoUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStoneEquipInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	//if (FAILED(Init_PokemonData()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CStoneEquipInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	//if (FAILED(Init_PokemonData()))
	//	return E_FAIL;

	return S_OK;
}

_uint CStoneEquipInfoUI::Tick(_double TimeDelta)
{
	for (auto& part : m_TextureParts)
	{
		part->Tick(TimeDelta);
	}

	for (auto& part : m_TextParts)
	{
		part->Tick(TimeDelta);
	}

	return _uint();
}

_uint CStoneEquipInfoUI::LateTick(_double TimeDelta)
{
	if (m_SkillInfoUI_Desc.pParent)
	{
		_matrix parent = m_SkillInfoUI_Desc.pParent->Get_WorldMatrix_Matrix();
		REMOVE_SCALE(parent);

		XMStoreFloat4x4(&m_FinalWorldMatrix, XMMatrixSet(
			m_UIDesc.m_fSizeX, 0.f, 0.f, 0.f,
			0.f, m_UIDesc.m_fSizeY, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			m_UIDesc.m_fX, -m_UIDesc.m_fY, 0.f, 1.f
		));

		XMStoreFloat4x4(&m_FinalWorldMatrix, XMLoadFloat4x4(&m_FinalWorldMatrix) *
			parent);

		m_pTransformCom->Set_Matrix(XMLoadFloat4x4(&m_FinalWorldMatrix));
	}

	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	for (auto& part : m_TextureParts)
	{
		part->LateTick(TimeDelta);
	}

	for (auto& part : m_TextParts)
	{
		part->LateTick(TimeDelta);
	}

	return _uint();
}

HRESULT CStoneEquipInfoUI::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_UIDesc.m_ShaderPass);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CStoneEquipInfoUI::Change_SkillIcon(const _uint& skillIndex)
{
	//CGameInstance* pGameInstance = CGameInstance::GetInstance();

	//CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Skill_Manager");
	//if (nullptr == pGameObject)
	//	return E_FAIL;

	//CSkill_Manager* pSkill_Manager = dynamic_cast<CSkill_Manager*>(pGameObject);
	//if (nullptr == pSkill_Manager)
	//	return E_FAIL;

	//CSkill::SKILL_DESC skillDesc = pSkill_Manager->Get_Skill_Desc(skillIndex);
	//wstring prototypeTag = L"Prototype_Component_Texture_button_skill_" + skillDesc.m_iconPath;

	//if (FAILED(m_TextureParts.at(m_skillTextureIndex)->Change_Texture(prototypeTag.c_str())))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CStoneEquipInfoUI::Init_StoneEquipInfo(vector<STONE_EQUIP_DESC>& desc)
{
	for (size_t i = 0; i < desc.size(); ++i)
	{
		HRESULT hr = E_FAIL;
		if (false == desc[i].m_isOpen)
		{
			if (STONE_EQUIP_TYPE::TYPE_ATK == desc[i].m_type)
			{
				hr = m_TextureParts.at(i * 2)->Change_Texture(L"Prototype_Component_Texture_window_pcharm_hibi_ATK");
			}
			else if (STONE_EQUIP_TYPE::TYPE_HP == desc[i].m_type)
			{
				hr = m_TextureParts.at(i * 2)->Change_Texture(L"Prototype_Component_Texture_window_pcharm_hibi_HP");
			}
			else
			{
				hr = m_TextureParts.at(i * 2)->Change_Texture(L"Prototype_Component_Texture_window_pcharm_hibi_ATKHP");
			}

			m_TextureParts.at(i * 2 + 1)->Set_RenderId(RENDER_END);
		}
		else
		{
			if (STONE_EQUIP_TYPE::TYPE_ATK == desc[i].m_type)
			{
				hr = m_TextureParts.at(i * 2)->Change_Texture(L"Prototype_Component_Texture_window_pcharm_blank_ATK");
			}
			else if (STONE_EQUIP_TYPE::TYPE_HP == desc[i].m_type)
			{
				hr = m_TextureParts.at(i * 2)->Change_Texture(L"Prototype_Component_Texture_window_pcharm_blank_HP");
			}
			else
			{
				hr = m_TextureParts.at(i * 2)->Change_Texture(L"Prototype_Component_Texture_window_pcharm_blank_ATKHP");
			}

			if (STONE_EQUIP_STATE::STATE_EQUIP == desc[i].m_state)
			{
				m_TextureParts.at(i * 2 + 1)->Set_RenderId(RENDER_UI);
			}
			else
			{
				m_TextureParts.at(i * 2 + 1)->Set_RenderId(RENDER_END);
			}
		}

		if (hr == E_FAIL)
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStoneEquipInfoUI::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_FinalWorldMatrix)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", m_TextureNumber)))
		return E_FAIL;

	if (m_UIDesc.m_UIType == UI_TYPE_COLOR_TEXTURE)
	{
		_float	radius = 3.f;
		m_pShaderCom->Set_RawValue("g_Radius", &radius, sizeof(_float));

	}
	_float2	size = { m_UIDesc.m_fSizeX, m_UIDesc.m_fSizeY };
	m_pShaderCom->Set_RawValue("g_Size", &size, sizeof(_float2));

	m_pShaderCom->Set_RawValue("g_vColor", &m_UIDesc.m_vColor, sizeof(_float4));

	_float	g_Progress = 0.f;
	m_pShaderCom->Set_RawValue("g_Progress", &g_Progress, sizeof(_float));

	Safe_Release(pGameInstance);

	return S_OK;
}


CStoneEquipInfoUI* CStoneEquipInfoUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStoneEquipInfoUI* pInstance = new CStoneEquipInfoUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStoneEquipInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStoneEquipInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStoneEquipInfoUI* pInstance = new CStoneEquipInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStoneEquipInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStoneEquipInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStoneEquipInfoUI* pInstance = new CStoneEquipInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStoneEquipInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStoneEquipInfoUI::Free()
{
	__super::Free();

	Safe_Release(m_SkillInfoUI_Desc.pParent);
}
