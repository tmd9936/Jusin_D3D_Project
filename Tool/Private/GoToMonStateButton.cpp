#include "stdafx.h"
#include "GoToMonStateButton.h"

#include "GameInstance.h"

#include "Level_Loading.h"

#include "PartTexture.h"

#include "PartText.h"

#include "PokemonData.h"

CGoToMonStateButton::CGoToMonStateButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CButton(pDevice, pContext)
{
}

CGoToMonStateButton::CGoToMonStateButton(const CGoToMonStateButton& rhs)
	: CButton(rhs)
{
}

HRESULT CGoToMonStateButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Init_Monster_Info()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGoToMonStateButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (FAILED(Init_Monster_Info()))
		return E_FAIL;

	return S_OK;
}

_uint CGoToMonStateButton::On_Idle()
{
	return 0;
}

_uint CGoToMonStateButton::On_Press()
{
	return 0;
}

_uint CGoToMonStateButton::On_Select()
{
	return 0;
}

_uint CGoToMonStateButton::On_Release()
{
	return 0;
}

_bool CGoToMonStateButton::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	if (m_pTransformCom)
	{
		Value ButtonDesc(kObjectType);
		{
			ButtonDesc.AddMember("m_fX", m_ButtonDesc.m_fX, allocator);
			ButtonDesc.AddMember("m_fY", m_ButtonDesc.m_fY, allocator);
			ButtonDesc.AddMember("m_fSizeX", m_ButtonDesc.m_fSizeX, allocator);
			ButtonDesc.AddMember("m_fSizeY", m_ButtonDesc.m_fSizeY, allocator);

			ButtonDesc.AddMember("m_eModelPrototypLevel", m_ButtonDesc.m_ModelPrototypLevel, allocator);
			ButtonDesc.AddMember("m_ShaderLevelIndex", m_ButtonDesc.m_ShaderLevelIndex, allocator);

			Value m_DiffuseTexturePath;
			string tag = m_ButtonDesc.m_DiffuseTexturePath;
			m_DiffuseTexturePath.SetString(tag.c_str(), (SizeType)tag.size(), allocator);
			ButtonDesc.AddMember("m_DiffuseTexturePath", m_DiffuseTexturePath, allocator);

			Value m_MaskPrototypeTag;
			tag = convert.to_bytes(m_ButtonDesc.m_MaskPrototypeTag);
			m_MaskPrototypeTag.SetString(tag.c_str(), (SizeType)tag.size(), allocator);
			ButtonDesc.AddMember("m_MaskPrototypeTag", m_MaskPrototypeTag, allocator);

			Value m_BrushPrototypeTag;
			tag = convert.to_bytes(m_ButtonDesc.m_BrushPrototypeTag);
			m_BrushPrototypeTag.SetString(tag.c_str(), (SizeType)tag.size(), allocator);
			ButtonDesc.AddMember("m_BrushPrototypeTag", m_BrushPrototypeTag, allocator);

			Value m_ButtonName;
			tag = convert.to_bytes(m_ButtonDesc.m_ButtonName);
			m_ButtonName.SetString(tag.c_str(), (SizeType)tag.size(), allocator);
			ButtonDesc.AddMember("m_ButtonName", m_ButtonName, allocator);

			ButtonDesc.AddMember("m_UIType", m_ButtonDesc.m_UIType, allocator);

			Value m_vColor(kObjectType);
			{
				m_vColor.AddMember("x", m_ButtonDesc.m_vColor.x, allocator);
				m_vColor.AddMember("y", m_ButtonDesc.m_vColor.y, allocator);
				m_vColor.AddMember("z", m_ButtonDesc.m_vColor.z, allocator);
				m_vColor.AddMember("w", m_ButtonDesc.m_vColor.w, allocator);
			}
			ButtonDesc.AddMember("m_vColor", m_vColor, allocator);

			ButtonDesc.AddMember("m_monsterNumber", m_MonStateButtonDesc.m_monsterNumber, allocator);
		}
		doc.AddMember("ButtonDesc", ButtonDesc, allocator);

	}

	return true;
}

_bool CGoToMonStateButton::Load_By_JsonFile_Impl(Document& doc)
{
	if (m_pTransformCom)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

		const Value& ButtonDesc = doc["ButtonDesc"];

		m_ButtonDesc.m_fX = ButtonDesc["m_fX"].GetFloat();
		m_ButtonDesc.m_fY = ButtonDesc["m_fY"].GetFloat();
		m_ButtonDesc.m_fSizeX = ButtonDesc["m_fSizeX"].GetFloat();
		m_ButtonDesc.m_fSizeY = ButtonDesc["m_fSizeY"].GetFloat();

		m_ButtonDesc.m_UIType = ButtonDesc["m_UIType"].GetUint();

		const Value& m_vColor = ButtonDesc["m_vColor"];
		m_ButtonDesc.m_vColor = _float4(m_vColor["x"].GetFloat(), m_vColor["y"].GetFloat(), m_vColor["z"].GetFloat(), m_vColor["w"].GetFloat());


		m_ButtonDesc.m_ModelPrototypLevel = ButtonDesc["m_ModelPrototypLevel"].GetUint();
		m_ButtonDesc.m_ShaderLevelIndex = ButtonDesc["m_ShaderLevelIndex"].GetUint();

		string m_DiffuseTexturePath = ButtonDesc["m_DiffuseTexturePath"].GetString();
		strcpy(m_ButtonDesc.m_DiffuseTexturePath, m_DiffuseTexturePath.c_str());

		string m_MaskPrototypeTag = ButtonDesc["m_MaskPrototypeTag"].GetString();
		lstrcpy(m_ButtonDesc.m_MaskPrototypeTag, convert.from_bytes(m_MaskPrototypeTag).c_str());

		string m_BrushPrototypeTag = ButtonDesc["m_BrushPrototypeTag"].GetString();
		lstrcpy(m_ButtonDesc.m_BrushPrototypeTag, convert.from_bytes(m_BrushPrototypeTag).c_str());

		string m_ButtonName = ButtonDesc["m_ButtonName"].GetString();
		lstrcpy(m_ButtonDesc.m_ButtonName, convert.from_bytes(m_ButtonName).c_str());

		m_MonStateButtonDesc.m_monsterNumber = ButtonDesc["m_monsterNumber"].GetInt();

		const Value& TextureParts = ButtonDesc["m_TextureParts"].GetArray();
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
			_tchar layerTag[MAX_PATH];
			lstrcpy(layerTag, convert.from_bytes(szLayerTag).c_str());

			pGameInstance->Clone_GameObject(layerTag, m_iLevelindex, TEXT("Prototype_GameObject_PartTexture"), (CGameObject**)&pTexturePart, &desc);
			if (nullptr == pTexturePart)
				return false;

			m_TextureParts.push_back(pTexturePart);
		}

		const Value& TextParts = ButtonDesc["m_TextParts"].GetArray();
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

	}

	return true;
}

HRESULT CGoToMonStateButton::Get_PokemonNumber(_uint& out, const _uint& pokemonIndex)
{
	string path = "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster";

	path.append(to_string(pokemonIndex));
	path.append(".json");

	FILE* fp = fopen(path.c_str(), "rb"); // non-Windows use "r"

	if (NULL == fp)
	{
		MSG_BOX("Load File Open Error");
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
		out = PokemonDesc["m_monsterNo"].GetUint();

		/* 구현부 끝 */
		fclose(fp);
		Safe_Delete_Array(readBuffer);

		return S_OK;
	}

	return E_FAIL;
}

HRESULT CGoToMonStateButton::Init_Monster_Info()
{
	if (m_TextParts.empty() || m_TextureParts.empty())
		return E_FAIL;
	
	// 해당 포켓몬 넘버의 포켓몬 정보 가져오기
	_uint pokemonNo = { 0 };
	if (FAILED(Get_PokemonNumber(pokemonNo, m_MonStateButtonDesc.m_monsterNumber)))
		return E_FAIL;

	// 포켓몬 데이터 매니저에서 데이터 얻어오기
	CGameObject* pPokemonData = CGameInstance::GetInstance()->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"PokemonData");
	if (nullptr == pPokemonData)
		return E_FAIL;

	// 이름 바꾸기
	CPokemonData::POKEMONDATA_DESC desc = dynamic_cast<CPokemonData*>(pPokemonData)->Get_PokemonDesc(pokemonNo);
	m_TextParts.at(0)->Set_Text(desc.name);

	// 아이콘 바꾸기
	wstring textureProtoType = L"Prototype_Component_Texture_Pokemon_Icon_M";
	textureProtoType.append(to_wstring(pokemonNo));
	if (FAILED(m_TextureParts.at(0)->Change_Texture(textureProtoType.c_str())))
		return E_FAIL;

	return S_OK;
}

CGoToMonStateButton* CGoToMonStateButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGoToMonStateButton* pInstance = new CGoToMonStateButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CGoToMonStateButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoToMonStateButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CGoToMonStateButton* pInstance = new CGoToMonStateButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CGoToMonStateButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoToMonStateButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CGoToMonStateButton* pInstance = new CGoToMonStateButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CGoToMonStateButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoToMonStateButton::Free()
{
	__super::Free();
}

