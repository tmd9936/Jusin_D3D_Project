#include"stdafx.h"

#include "SkillInfoUI.h"

#include "GameInstance.h"

#include "PartText.h"
#include "PartTexture.h"

#include "Skill_Manager.h"

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

_uint CSkillInfoUI::Tick(_double TimeDelta)
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

_uint CSkillInfoUI::LateTick(_double TimeDelta)
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
}

HRESULT CSkillInfoUI::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_UIDesc.m_ShaderPass);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CSkillInfoUI::Change_SkillIcon(const _uint& skillIndex)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Skill_Manager");
	if (nullptr == pGameObject)
		return E_FAIL;

	CSkill_Manager* pSkill_Manager = dynamic_cast<CSkill_Manager*>(pGameObject);
	if (nullptr == pSkill_Manager)
		return E_FAIL;

	CSkill::SKILL_DESC skillDesc = pSkill_Manager->Get_Skill_Desc(skillIndex);
	wstring prototypeTag = L"Prototype_Component_Texture_button_skill_" + skillDesc.m_iconPath;

	if (FAILED(m_TextureParts.at(m_skillTextureIndex)->Change_Texture(prototypeTag.c_str())))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkillInfoUI::SetUp_ShaderResources()
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
