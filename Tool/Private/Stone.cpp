#include"stdafx.h"

#include "Stone.h"

#include "GameInstance.h"

CStone::CStone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CStone::CStone(const CStone& rhs)
	: CUI(rhs)
{
}

HRESULT CStone::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStone::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_Desc = (*(STONE_DESC*)pArg);
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &m_Desc.m_UIDesc)))
		return E_FAIL;

	if (FAILED(Init_Stone(m_Desc)))
		return E_FAIL;
		
	return S_OK;
}

HRESULT CStone::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CStone::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);

	Change_State();

	return __super::Tick(TimeDelta);
}

_uint CStone::LateTick(_double TimeDelta)
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

	return _uint();
}

HRESULT CStone::Render()
{
	return __super::Render();
}

HRESULT CStone::Init_Stone(const STONE_DESC& stoneDesc)
{
	m_Desc.value = stoneDesc.value;
	m_Desc.m_stoneType = stoneDesc.m_stoneType;

	if (FAILED(Init_Text(stoneDesc)))
		return E_FAIL;

	if (FAILED(Init_Texture(stoneDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStone::Init_Text(const STONE_DESC& stoneDesc)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CPartText* pTextPart = nullptr;

	CPartText::TEXT_DESC partTextDesc{};

	partTextDesc.pParent = m_pTransformCom;

	partTextDesc.m_fX = 10.f;
	partTextDesc.m_fY = 28.5f;

	partTextDesc.m_vColor = _float4(1.f, 1.f, 1.f, 1.f);

	partTextDesc.m_Rotation = 0.f;
	partTextDesc.m_vRotationOrigin = _float2(0.f, 0.f);
	partTextDesc.m_vScale = _float2(0.32f, 0.26f);

	lstrcpy(partTextDesc.m_FontTag, L"Font_NanumBarunGothic");
	//lstrcpy(partTextDesc.m_Text, to_wstring(stoneDesc.value).c_str());

	pGameInstance->Clone_GameObject(Get_LayerTag().c_str(), m_iLevelindex, TEXT("Prototype_GameObject_PartText"), (CGameObject**)&pTextPart, &partTextDesc);
	if (nullptr == pTextPart)
		return E_FAIL;

	pTextPart->Set_Text(to_wstring(stoneDesc.value));

	m_TextParts.push_back(pTextPart);

	return S_OK;
}

HRESULT CStone::Init_Texture(const STONE_DESC& stoneDesc)
{
	if (stoneDesc.m_stoneType == TYPE_ATK)
	{
		if (FAILED(Change_Texture(L"Prototype_Component_Texture_UI_Pstone_attack")))
			return E_FAIL;
	}
	else if (stoneDesc.m_stoneType == TYPE_HP)
	{
		if (FAILED(Change_Texture(L"Prototype_Component_Texture_UI_Pstone_defense")))
			return E_FAIL;
	}

	if (FAILED(Init_MaskTexture(stoneDesc)))
		return E_FAIL;

	if (FAILED(Init_PokemonIconTexture(stoneDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStone::Init_MaskTexture(const STONE_DESC& stoneDesc)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CPartTexture* pTexturePart = nullptr;

	CPartTexture::UI_DESC maskDesc{};
	maskDesc.pParent = m_pTransformCom;
	maskDesc.m_fSizeX = m_UIDesc.m_fSizeX;
	maskDesc.m_fSizeY = m_UIDesc.m_fSizeY;
	maskDesc.m_fX = 0.f;
	maskDesc.m_fY = 0.f;
	maskDesc.m_TextureProtoTypeLevel = LEVEL_STATIC;
	maskDesc.m_eType = 1;
	maskDesc.m_vColor = _float4(0.2f, 0.2f, 0.2f, 0.5f);

	maskDesc.m_ShaderPass = 5;
	lstrcpy(maskDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_Rectangle_Base");

	pGameInstance->Clone_GameObject(Get_LayerTag().c_str(), m_iLevelindex, TEXT("Prototype_GameObject_PartTexture"), (CGameObject**)&pTexturePart, &maskDesc);
	if (nullptr == pTexturePart)
		return E_FAIL;

	if (stoneDesc.m_eCurState == STATE_EQUIP_ON_INVENTORY)
	{
		pTexturePart->Set_RenderId(RENDER_UI);
	}
	else
	{
		pTexturePart->Set_RenderId(RENDER_END);
	}

	m_TextureParts.push_back(pTexturePart);

	return S_OK;
}

HRESULT CStone::Init_PokemonIconTexture(const STONE_DESC& stoneDesc)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CPartTexture* pTexturePart = nullptr;

	CPartTexture::UI_DESC pokemonIconDesc{};
	pokemonIconDesc.pParent = m_pTransformCom;
	pokemonIconDesc.m_fSizeX = m_UIDesc.m_fSizeX * 0.4f;
	pokemonIconDesc.m_fSizeY = m_UIDesc.m_fSizeY * 0.4f;
	pokemonIconDesc.m_fX = -12.5f;
	pokemonIconDesc.m_fY = -12.5f;
	pokemonIconDesc.m_TextureProtoTypeLevel = LEVEL_STATIC;
	pokemonIconDesc.m_eType = 0;
	pokemonIconDesc.m_vColor = _float4(0.2f, 0.2f, 0.2f, 0.5f);

	pokemonIconDesc.m_ShaderPass = 0;
	lstrcpy(pokemonIconDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_Pokemon_Icon_M");
	lstrcat(pokemonIconDesc.m_TextureProtoTypeName, to_wstring(stoneDesc.m_pokemonIconNumber).c_str());

	pGameInstance->Clone_GameObject(Get_LayerTag().c_str(), m_iLevelindex, TEXT("Prototype_GameObject_PartTexture"), (CGameObject**)&pTexturePart, &pokemonIconDesc);
	if (nullptr == pTexturePart)
		return E_FAIL;

	if (stoneDesc.m_eCurState == STATE_EQUIP_ON_INVENTORY)
	{
		pTexturePart->Set_RenderId(RENDER_UI);
	}
	else
	{
		pTexturePart->Set_RenderId(RENDER_END);
	}

	m_TextureParts.push_back(pTexturePart);

	return S_OK;
}

void CStone::Change_Value(const wstring& text)
{
	if (m_TextParts.empty())
		return;

	m_Desc.value = stoi(text);

	m_TextParts.at(0)->Set_Text(text);
}

HRESULT CStone::Change_StoneType(TYPE eType)
{
	m_Desc.m_stoneType = eType;
	if (m_Desc.m_stoneType == TYPE_ATK)
	{
		if (FAILED(Change_Texture(L"Prototype_Component_Texture_UI_Pstone_attack")))
			return E_FAIL;
	}
	else if (m_Desc.m_stoneType == TYPE_HP)
	{
		if (FAILED(Change_Texture(L"Prototype_Component_Texture_UI_Pstone_defense")))
			return E_FAIL;
	}

	return S_OK;
}

void CStone::Set_State(STATE eState)
{
	m_Desc.m_eCurState = eState;
}

void CStone::Change_State()
{
	if (m_Desc.m_eCurState != m_Desc.m_ePreState)
	{
		switch (m_Desc.m_eCurState)
		{
		case STATE_NO_EQUIP_ON_INVENTORY:
			break;
		case STATE_EQUIP_ON_INVENTORY:
			break;
		case STATE_PICKING_ON_INVENTORY:
			break;
		case STATE_EQUIP_ON_EQUIPINFO:
			m_eRenderId = RENDER_UI;
			m_TextParts.at(0)->Set_RenderId(RENDER_UI);
			break;
		case STATE_SHOW_ON_INFO_UI:
			break;
		case STATE_PICKING_FOLLOW_MOUSE:
			m_eRenderId = RENDER_UI;
			m_TextParts.at(0)->Set_RenderId(RENDER_UI);
			break;
		case STATE_NO_SHOW:
			m_eRenderId = RENDER_END;
			m_TextParts.at(0)->Set_RenderId(RENDER_END);
			break;
		}

		m_Desc.m_ePreState = m_Desc.m_eCurState;
	}
}

void CStone::State_Tick(const _double& TimeDelta)
{
	switch (m_Desc.m_eCurState)
	{
	case STATE_NO_EQUIP_ON_INVENTORY:
		break;
	case STATE_EQUIP_ON_INVENTORY:
		break;
	case STATE_PICKING_ON_INVENTORY:
		break;
	case STATE_EQUIP_ON_EQUIPINFO:
		break;
	case STATE_SHOW_ON_INFO_UI:
		break;
	case STATE_PICKING_FOLLOW_MOUSE:
		break;
	case STATE_NO_SHOW:
		break;
	}
}

CStone* CStone::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStone* pInstance = new CStone(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStone::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStone* pInstance = new CStone(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))

	{
		MSG_BOX("Failed to Cloned CStone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStone::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStone* pInstance = new CStone(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStone::Free()
{
	__super::Free();

	Safe_Release(m_Desc.pParent);
}
