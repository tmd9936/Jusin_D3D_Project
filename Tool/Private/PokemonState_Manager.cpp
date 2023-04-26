#include "stdafx.h"
#include "PokemonState_Manager.h"

#include "GameInstance.h"

#include "Client_Utility.h"

#include "Monster.h"
#include "Player.h"
#include "StageSupportMonster.h"
#include "PokemonSkillStoneUI.h"
#include "SkillInfoUI.h"
#include "StoneEquipInfoUI.h"
#include "StoneInventory.h"
#include "PokemonInfoUI.h"

#include "Stone.h"

CPokemonState_Manager::CPokemonState_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{

}

CPokemonState_Manager::CPokemonState_Manager(const CPokemonState_Manager& rhs)
	: CUI(rhs)
{

}

HRESULT CPokemonState_Manager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPokemonState_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPokemonState_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;


	//if (FAILED(Init_NowMonster()))
	//	return E_FAIL;

	if (FAILED(Init_PokemonInfoUI()))
		return E_FAIL;

	if (FAILED(Init_PokemonSkillStoneUI()))
		return E_FAIL;

	if (FAILED(Init_PokemonSkillInfoUI()))
		return E_FAIL;

	if (FAILED(Init_StoneEquipInfoUI()))
		return E_FAIL;

	if (FAILED(Init_StoneInventory()))
		return E_FAIL;

	return S_OK;
}

_uint CPokemonState_Manager::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);
	Change_State();
	Picking();

	return _uint();
}

_uint CPokemonState_Manager::LateTick(_double TimeDelta)
{
	return _uint();
}

HRESULT CPokemonState_Manager::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}


_bool CPokemonState_Manager::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	Value UIDesc(kObjectType);
	{
		UIDesc.AddMember("m_fX", m_UIDesc.m_fX, allocator);
		UIDesc.AddMember("m_fY", m_UIDesc.m_fY, allocator);

		UIDesc.AddMember("m_fSizeX", m_UIDesc.m_fSizeX, allocator);
		UIDesc.AddMember("m_fSizeY", m_UIDesc.m_fSizeY, allocator);
		UIDesc.AddMember("m_TextureProtoTypeLevel", m_UIDesc.m_TextureProtoTypeLevel, allocator);
		UIDesc.AddMember("m_UIType", m_UIDesc.m_UIType, allocator);

		Value m_vColor(kObjectType);
		{
			m_vColor.AddMember("x", m_UIDesc.m_vColor.x, allocator);
			m_vColor.AddMember("y", m_UIDesc.m_vColor.y, allocator);
			m_vColor.AddMember("z", m_UIDesc.m_vColor.z, allocator);
			m_vColor.AddMember("w", m_UIDesc.m_vColor.w, allocator);
		}
		UIDesc.AddMember("m_vColor", m_vColor, allocator);

		Value m_TextureProtoTypeName;
		string TextureProtoTypeName = convert.to_bytes(m_UIDesc.m_TextureProtoTypeName);
		m_TextureProtoTypeName.SetString(TextureProtoTypeName.c_str(), (SizeType)TextureProtoTypeName.size(), allocator);
		UIDesc.AddMember("m_TextureProtoTypeName", m_TextureProtoTypeName, allocator);

	}
	doc.AddMember("UIDesc", UIDesc, allocator);

	return true;
}

_bool CPokemonState_Manager::Load_By_JsonFile_Impl(Document& doc)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	Value& UIDesc = doc["UIDesc"];

	m_UIDesc.m_fX = UIDesc["m_fX"].GetFloat();
	m_UIDesc.m_fY = UIDesc["m_fY"].GetFloat();
	m_UIDesc.m_fSizeX = UIDesc["m_fSizeX"].GetFloat();
	m_UIDesc.m_fSizeY = UIDesc["m_fSizeY"].GetFloat();

	m_UIDesc.m_TextureProtoTypeLevel = UIDesc["m_TextureProtoTypeLevel"].GetUint();
	m_UIDesc.m_UIType = UIDesc["m_UIType"].GetUint();

	const Value& m_vColor = UIDesc["m_vColor"];
	m_UIDesc.m_vColor = _float4(m_vColor["x"].GetFloat(), m_vColor["y"].GetFloat(), m_vColor["z"].GetFloat(), m_vColor["w"].GetFloat());

	string m_TextureProtoTypeName = UIDesc["m_TextureProtoTypeName"].GetString();

	m_UIDesc.m_ShaderPass = UIDesc["m_ShaderPass"].GetUint();

	lstrcpy(m_UIDesc.m_TextureProtoTypeName, convert.from_bytes(m_TextureProtoTypeName).c_str());

	const Value& TextureParts = UIDesc["m_TextureParts"].GetArray();
	for (SizeType i = 0; i < TextureParts.Size(); ++i)
	{
		CPartTexture* pTexturePart = nullptr;

		CPartTexture::UI_DESC desc{};
		desc.pParent = m_pTransformCom;
		desc.m_fSizeX = TextureParts[i]["m_fSizeX"].GetFloat();
		desc.m_fSizeY = TextureParts[i]["m_fSizeY"].GetFloat();
		desc.m_fX = TextureParts[i]["m_fX"].GetFloat();
		desc.m_fY = TextureParts[i]["m_fY"].GetFloat();
		desc.m_TextureProtoTypeLevel = TextureParts[i]["m_TextureProtoTypeLevel"].GetUint();

		desc.m_eType = TextureParts[i]["m_eType"].GetUint();

		const Value& m_vPartColor = TextureParts[i]["m_vColor"];
		desc.m_vColor = _float4(m_vPartColor["x"].GetFloat(), m_vPartColor["y"].GetFloat(), m_vPartColor["z"].GetFloat(), m_vPartColor["w"].GetFloat());

		desc.m_ShaderPass = TextureParts[i]["m_ShaderPass"].GetUint();

		string textureProtoTypeName = TextureParts[i]["m_TextureProtoTypeName"].GetString();
		lstrcpy(desc.m_TextureProtoTypeName, convert.from_bytes(textureProtoTypeName).c_str());

		string szLayerTag = TextureParts[i]["LayerTag"].GetString();
		wstring layerTag = convert.from_bytes(szLayerTag);

		pGameInstance->Clone_GameObject(layerTag.c_str(), m_iLevelindex, TEXT("Prototype_GameObject_PartTexture"), (CGameObject**)&pTexturePart, &desc);
		if (nullptr == pTexturePart)
			return false;

		m_TextureParts.push_back(pTexturePart);
	}

	const Value& TextParts = UIDesc["m_TextParts"].GetArray();
	for (SizeType i = 0; i < TextParts.Size(); ++i)
	{
		CPartText* pTextPart = nullptr;

		CPartText::TEXT_DESC desc{};

		desc.pParent = m_pTransformCom;
		desc.m_fX = TextParts[i]["m_fX"].GetFloat();
		desc.m_fY = TextParts[i]["m_fY"].GetFloat();

		const Value& m_vPartColor = TextParts[i]["m_vColor"];
		desc.m_vColor = _float4(m_vPartColor["x"].GetFloat(), m_vPartColor["y"].GetFloat(), m_vPartColor["z"].GetFloat(), m_vPartColor["w"].GetFloat());

		desc.m_Rotation = TextParts[i]["m_Rotation"].GetFloat();

		const Value& m_vRotationOrigin = TextParts[i]["m_vRotationOrigin"];
		desc.m_vRotationOrigin = _float2(m_vRotationOrigin["x"].GetFloat(), m_vRotationOrigin["y"].GetFloat());

		const Value& m_vScale = TextParts[i]["m_vScale"];
		desc.m_vScale = _float2(m_vScale["x"].GetFloat(), m_vScale["y"].GetFloat());

		string fontTag = TextParts[i]["m_FontTag"].GetString();
		lstrcpy(desc.m_FontTag, convert.from_bytes(fontTag).c_str());

		string m_Text = TextParts[i]["m_Text"].GetString();
		lstrcpy(desc.m_Text, convert.from_bytes(m_Text).c_str());

		string szLayerTag = TextParts[i]["LayerTag"].GetString();
		_tchar layerTag[MAX_PATH];
		lstrcpy(layerTag, convert.from_bytes(szLayerTag).c_str());

		pGameInstance->Clone_GameObject(layerTag, m_iLevelindex, TEXT("Prototype_GameObject_PartText"), (CGameObject**)&pTextPart, &desc);
		if (nullptr == pTextPart)
			return false;

		pTextPart->Set_Text(convert.from_bytes(m_Text));

		m_TextParts.push_back(pTextPart);
	}

	return true;
}

HRESULT CPokemonState_Manager::Init_NowMonster(const _uint& nowMonsterNumber)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	m_Desc.m_nowMonsterNumber = nowMonsterNumber;

	if (m_Desc.m_nowMonsterNumber == 1)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_POKEMONSTATE, L"Layer_Player", (CGameObject**)&m_pNowMonster, L"Player1", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster1.json", CLONE_FILEPATH)))
			return E_FAIL;
	}
	else if (m_Desc.m_nowMonsterNumber == 2)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_POKEMONSTATE, L"Layer_Player", L"Player1", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster1.json", CLONE_FILEPATH)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageSupportMonster"), LEVEL_POKEMONSTATE, L"Layer_Player", (CGameObject**)&m_pNowMonster, L"Player2", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster2.json", CLONE_FILEPATH)))
			return E_FAIL;
	}
	else if (m_Desc.m_nowMonsterNumber == 3)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_POKEMONSTATE, L"Layer_Player", L"Player1", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster1.json", CLONE_FILEPATH)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageSupportMonster"), LEVEL_POKEMONSTATE, L"Layer_Player", (CGameObject**)&m_pNowMonster, L"Player3", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster3.json", CLONE_FILEPATH)))
			return E_FAIL;
	}

	pGameInstance->Layer_Tick_State_Change(L"Layer_Player", LEVEL_POKEMONSTATE, false);

	return S_OK;
}

HRESULT CPokemonState_Manager::Init_PokemonInfoUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_POKEMONSTATE, L"Layer_UI", L"PokemonInfoUI");
	if (nullptr == pObject)
		return E_FAIL;

	m_pPokemonInfoUI = dynamic_cast<CPokemonInfoUI*>(pObject);
	if (nullptr == m_pPokemonInfoUI)
		return E_FAIL;

	Safe_AddRef(m_pPokemonInfoUI);

	return S_OK;
}

HRESULT CPokemonState_Manager::Init_PokemonSkillStoneUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_POKEMONSTATE, L"Layer_UI", L"PokemonSkillStoneUI");
	if (nullptr == pObject)
		return E_FAIL;

	m_pPokemonSkillStoneUI = dynamic_cast<CPokemonSkillStoneUI*>(pObject);
	if (nullptr == m_pPokemonSkillStoneUI)
		return E_FAIL;

	Safe_AddRef(m_pPokemonSkillStoneUI);

	return S_OK;
}

HRESULT CPokemonState_Manager::Init_PokemonSkillInfoUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject1 = pGameInstance->Get_Object(LEVEL_POKEMONSTATE, L"Layer_UI", L"SkillInfoUI01");
	if (nullptr == pObject1)
		return E_FAIL;

	m_pSkillInfoUI01 = dynamic_cast<CSkillInfoUI*>(pObject1);
	if (nullptr == m_pSkillInfoUI01)
		return E_FAIL;
	Safe_AddRef(m_pSkillInfoUI01);

	CGameObject* pObject2 = pGameInstance->Get_Object(LEVEL_POKEMONSTATE, L"Layer_UI", L"SkillInfoUI02");
	if (nullptr == pObject2)
		return E_FAIL;

	m_pSkillInfoUI02 = dynamic_cast<CSkillInfoUI*>(pObject2);
	if (nullptr == m_pSkillInfoUI02)
		return E_FAIL;
	Safe_AddRef(m_pSkillInfoUI02);

	return S_OK;
}

HRESULT CPokemonState_Manager::Init_StoneEquipInfoUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_POKEMONSTATE, L"Layer_UI", L"StoneEquipInfoUI");
	if (nullptr == pObject)
		return E_FAIL;

	m_pStoneEquipInfoUI = dynamic_cast<CStoneEquipInfoUI*>(pObject);
	if (nullptr == m_pStoneEquipInfoUI)
		return E_FAIL;
	Safe_AddRef(m_pStoneEquipInfoUI);

	return S_OK;
}

HRESULT CPokemonState_Manager::Init_StoneInventory()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_POKEMONSTATE, L"Layer_UI", L"StoneInvetory");
	if (nullptr == pObject)
		return E_FAIL;

	m_pStoneInventory = dynamic_cast<CStoneInventory*>(pObject);
	if (nullptr == m_pStoneInventory)
		return E_FAIL;
	Safe_AddRef(m_pStoneInventory);

	return S_OK;
}

void CPokemonState_Manager::State_Tick(const _double& TimeDelta)
{


}

void CPokemonState_Manager::Change_State()
{
	if (m_eCurState != m_ePreState)
	{

		m_ePreState = m_eCurState;
	}
}

void CPokemonState_Manager::Picking()
{
	if (MOUSE_TAB(MOUSE::LBTN) && CClient_Utility::Mouse_Pos_In_Platform() && m_eCurState == MANAGER_IDLE)
	{
		// 마우스가 인벤토리 안에 있는지 확인
		// 존재하는 스톤칸에 있는지 확인
		// 맞으면 피킹상태로 변환
		// 스톤이랑 마우스 따라다니는 텍스쳐 만들기
		// 텍스쳐 바꾸기
	}
}

HRESULT CPokemonState_Manager::SetUp_ShaderResources()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CPokemonState_Manager* CPokemonState_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPokemonState_Manager* pInstance = new CPokemonState_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CPokemonState_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokemonState_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CPokemonState_Manager* pInstance = new CPokemonState_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CPokemonState_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokemonState_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CPokemonState_Manager* pInstance = new CPokemonState_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CPokemonState_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPokemonState_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pNowMonster);
	Safe_Release(m_pPokemonSkillStoneUI);
	Safe_Release(m_pSkillInfoUI01);
	Safe_Release(m_pSkillInfoUI02);
	Safe_Release(m_pStoneEquipInfoUI);
	Safe_Release(m_pStoneInventory);
	Safe_Release(m_pPokemonInfoUI);
}
