#include"stdafx.h"

#include "StoneInventory.h"

#include "GameInstance.h"

#include "PartText.h"
#include "PartTexture.h"

#include "Stone.h"


CStoneInventory::CStoneInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CStoneInventory::CStoneInventory(const CStoneInventory& rhs)
	: CUI(rhs)
{
}

HRESULT CStoneInventory::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStoneInventory::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	m_stones.resize(m_InventoryNums, nullptr);

	//if (FAILED(Init_PokemonData()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CStoneInventory::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	m_stones.resize(m_InventoryNums, nullptr);

	//if (FAILED(Init_PokemonData()))
	//	return E_FAIL;

	return S_OK;
}


HRESULT CStoneInventory::Init_PokemonData(const _uint& nowMonsterNumber)
{
	if (nowMonsterNumber == 0 || nowMonsterNumber > 3)
		return E_FAIL;

	//m_PokemonSkillStone_Desc.m_nowMonsterNumber = nowMonsterNumber;

	//if (FAILED(Get_NowMonsterData()))
	//	return E_FAIL;

	//if (FAILED(Get_PokemonData()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CStoneInventory::Get_PokemonData()
{
	//if (m_TextParts.empty() || m_TextureParts.empty())
	//	return E_FAIL;

	//// 포켓몬 데이터 매니저에서 데이터 얻어오기
	//CGameObject* pPokemonData = CGameInstance::GetInstance()->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"PokemonData");
	//if (nullptr == pPokemonData)
	//	return E_FAIL;

	//// 이름 바꾸기
	//CPokemonData::POKEMONDATA_DESC desc = dynamic_cast<CPokemonData*>(pPokemonData)->Get_PokemonDesc(m_PokemonInfo_Desc.m_pokemonNo);
	//m_TextParts.at(0)->Set_Text(desc.name);
	//m_TextParts.at(1)->Set_Text(desc.type);

	//// 아이콘 바꾸기
	//wstring textureProtoType = L"Prototype_Component_Texture_Pokemon_Icon_M";
	//textureProtoType.append(to_wstring(m_PokemonInfo_Desc.m_pokemonNo));
	//if (FAILED(m_TextureParts.at(m_pokemonIconTextureIndex)->Change_Texture(textureProtoType.c_str())))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CStoneInventory::Get_NowMonsterData()
{
	//CGameInstance* pGameInstance = CGameInstance::GetInstance();

	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	//string path = "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster";

	//path.append(to_string(m_PokemonSkillStone_Desc.m_nowMonsterNumber));
	//path.append(".json");

	//FILE* fp = fopen(path.c_str(), "rb"); // non-Windows use "r"

	//if (NULL == fp)
	//{
	//	MSG_BOX("Load File Init_PokemonData() Error");
	//	return E_FAIL;
	//}
	//else
	//{
	//	char* readBuffer = new char[65536];
	//	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	//	Document doc;
	//	doc.ParseStream(is);

	//	if (doc.MemberCount() <= 0)
	//	{
	//		fclose(fp);
	//		Safe_Delete_Array(readBuffer);
	//		return E_FAIL;
	//	}

	//	/* 구현부 시작 */
	//	const Value& PokemonDesc = doc["PokemonDesc"].GetObj();
	//	const Value& skillIDs = PokemonDesc["m_skillIDs"];

	//	for (SizeType i = 0; i < skillIDs.Size(); ++i)
	//	{
	//		_int skillIndex = skillIDs[i].GetInt();
	//		CSkillInfoUI* pSkillInfoUI = nullptr;

	//		char path[MAX_PATH] = {};
	//		if (i == 0)
	//			strcpy(path, "../../Reference/Resources/Data/Scene/PokemonInfo/SkillStoneUI/SkillInfoUI01.json");
	//		else if (i == 1)
	//			strcpy(path, "../../Reference/Resources/Data/Scene/PokemonInfo/SkillStoneUI/SkillInfoUI02.json");
	//		else
	//			break;

	//		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkillInfoUI"), Get_Levelindex(), Get_LayerTag().c_str(),
	//			(CGameObject**)&pSkillInfoUI, nullptr, path, CLONE_FILEPATH)))
	//			return E_FAIL;
	//		if (nullptr == pSkillInfoUI)
	//			return E_FAIL;

	//		if (FAILED(pSkillInfoUI->Change_SkillIcon(skillIndex)))
	//			return E_FAIL;

	//		pSkillInfoUI->Set_ParentTransform(m_pTransformCom);

	//		m_skillInfoUIs.push_back(pSkillInfoUI);
	//	}

	//	const Value& m_stones = PokemonDesc["m_stones"];
	//	vector<STONE_EQUIP_DESC> stoneEquipInfos;
	//	for (SizeType i = 0; i < m_stones.Size(); ++i)
	//	{
	//		STONE_EQUIP_DESC desc{};

	//		desc.m_isOpen = m_stones[i]["m_isOpen"].GetBool();
	//		desc.m_type = (STONE_EQUIP_TYPE)m_stones[i]["m_type"].GetUint();
	//		desc.m_equip_stoneID = m_stones[i]["m_equip_stoneID"].GetUint();
	//		desc.m_state = (STONE_EQUIP_STATE)m_stones[i]["m_state"].GetUint();

	//		stoneEquipInfos.push_back(desc);
	//	}

	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StoneEquipInfoUI"), Get_Levelindex(), Get_LayerTag().c_str(),
	//		(CGameObject**)&m_pStoneEquipInfoUI, nullptr, "../../Reference/Resources/Data/Scene/PokemonInfo/SkillStoneUI/StoneEquipInfoUI.json", CLONE_FILEPATH)))
	//		return E_FAIL;
	//	if (nullptr == m_pStoneEquipInfoUI)
	//		return E_FAIL;

	//	if (FAILED(m_pStoneEquipInfoUI->Init_StoneEquipInfo(stoneEquipInfos)))
	//		return E_FAIL;

	//	m_pStoneEquipInfoUI->Set_ParentTransform(m_pTransformCom);

	//	/* 구현부 끝 */

	//	fclose(fp);
	//	Safe_Delete_Array(readBuffer);

	//	return S_OK;
	//}

	//return E_FAIL;

	return S_OK;
}

_bool CStoneInventory::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
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

_bool CStoneInventory::Load_By_JsonFile_Impl(Document& doc)
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


CStoneInventory* CStoneInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStoneInventory* pInstance = new CStoneInventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStoneInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStoneInventory::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStoneInventory* pInstance = new CStoneInventory(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStoneInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStoneInventory::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStoneInventory* pInstance = new CStoneInventory(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStoneInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStoneInventory::Free()
{
	__super::Free();

	for (auto& iter : m_stones)
	{
		Safe_Release(iter);
	}

}
