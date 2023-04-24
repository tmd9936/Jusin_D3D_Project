#include"stdafx.h"

#include "PokemonInfoUI.h"

#include "GameInstance.h"

#include "PartText.h"
#include "PartTexture.h"

#include "PokemonData.h"

CPokemonInfoUI::CPokemonInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CPokemonInfoUI::CPokemonInfoUI(const CPokemonInfoUI& rhs)
	: CUI(rhs)
{
}

HRESULT CPokemonInfoUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPokemonInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	//if (FAILED(Init_PokemonData()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CPokemonInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	//if (FAILED(Init_PokemonData()))
	//	return E_FAIL;

	return S_OK;
}


HRESULT CPokemonInfoUI::Init_PokemonData(const _uint& nowMonsterNumber)
{
	if (nowMonsterNumber == 0 || nowMonsterNumber > 3)
		return E_FAIL;

	m_PokemonInfo_Desc.m_nowMonsterNumber = nowMonsterNumber;

	if (FAILED(Get_NowMonsterData()))
		return E_FAIL;

	if (FAILED(Get_PokemonData()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPokemonInfoUI::Get_PokemonData()
{
	if (m_TextParts.empty() || m_TextureParts.empty())
		return E_FAIL;

	// 포켓몬 데이터 매니저에서 데이터 얻어오기
	CGameObject* pPokemonData = CGameInstance::GetInstance()->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"PokemonData");
	if (nullptr == pPokemonData)
		return E_FAIL;

	// 이름 바꾸기
	CPokemonData::POKEMONDATA_DESC desc = dynamic_cast<CPokemonData*>(pPokemonData)->Get_PokemonDesc(m_PokemonInfo_Desc.m_pokemonNo);
	m_TextParts.at(0)->Set_Text(desc.name);
	m_TextParts.at(1)->Set_Text(desc.type);

	// 아이콘 바꾸기
	wstring textureProtoType = L"Prototype_Component_Texture_Pokemon_Icon_M";
	textureProtoType.append(to_wstring(m_PokemonInfo_Desc.m_pokemonNo));
	if (FAILED(m_TextureParts.at(m_pokemonIconTextureIndex)->Change_Texture(textureProtoType.c_str())))
		return E_FAIL;

	return S_OK;
}

HRESULT CPokemonInfoUI::Get_NowMonsterData()
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	string path = "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster";

	path.append(to_string(m_PokemonInfo_Desc.m_nowMonsterNumber));
	path.append(".json");

	FILE* fp = fopen(path.c_str(), "rb"); // non-Windows use "r"

	if (NULL == fp)
	{
		MSG_BOX("Load File Init_PokemonData() Error");
		return E_FAIL;
	}
	else
	{
		char* readBuffer = new char[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		Document doc;
		doc.ParseStream(is);

		if (doc.MemberCount() <= 0)
		{
			fclose(fp);
			Safe_Delete_Array(readBuffer);
			return E_FAIL;
		}

		/* 구현부 시작 */
		const Value& PokemonDesc = doc["PokemonDesc"].GetObj();
		m_PokemonInfo_Desc.m_pokemonNo = PokemonDesc["m_monsterNo"].GetUint();
		m_PokemonInfo_Desc.m_pokemonHP = PokemonDesc["m_hpBasis"].GetUint();
		m_PokemonInfo_Desc.m_pokemonATK = PokemonDesc["m_attackBasis"].GetInt();
		m_PokemonInfo_Desc.m_pokemonLevel = PokemonDesc["m_level"].GetInt();
		m_PokemonInfo_Desc.m_curExp = PokemonDesc["m_level"].GetInt();

		/* 구현부 끝 */
		fclose(fp);
		Safe_Delete_Array(readBuffer);

		return S_OK;
	}

	return E_FAIL;
}

_bool CPokemonInfoUI::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
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

_bool CPokemonInfoUI::Load_By_JsonFile_Impl(Document& doc)
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

	m_PokemonInfo_Desc.m_nowMonsterNumber = UIDesc["m_nowMonsterNumber"].GetUint();

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


CPokemonInfoUI* CPokemonInfoUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPokemonInfoUI* pInstance = new CPokemonInfoUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CPokemonInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokemonInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CPokemonInfoUI* pInstance = new CPokemonInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CPokemonInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokemonInfoUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CPokemonInfoUI* pInstance = new CPokemonInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CPokemonInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPokemonInfoUI::Free()
{
	__super::Free();
}
