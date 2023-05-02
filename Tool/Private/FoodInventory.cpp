#include"stdafx.h"

#include "FoodInventory.h"

#include "GameInstance.h"

#include "PartText.h"
#include "PartTexture.h"

#include "Food.h"

CFoodInventory::CFoodInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CFoodInventory::CFoodInventory(const CFoodInventory& rhs)
	: CUI(rhs)
{
}

HRESULT CFoodInventory::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFoodInventory::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFoodInventory::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CFoodInventory::Tick(_double TimeDelta)
{
	for (auto& iter : m_foodInfos)
	{
		if (nullptr != iter)
		{
			iter->Tick(TimeDelta);
		}
	}

	return __super::Tick(TimeDelta);
}

_uint CFoodInventory::LateTick(_double TimeDelta)
{
	_uint result = 0;
	if (m_lateTickState)
	{
		result = __super::LateTick(TimeDelta);

		for (size_t i = 0; i < m_foodInfos.size(); ++i)
		{
			if (nullptr != m_foodInfos[i])
			{
				_float3 texturePosition = m_TextureParts[i]->Get_FinalWorldMatrixPosition();
				m_foodInfos[i]->Set_Pos({ texturePosition.x, texturePosition.y, texturePosition.z });
				m_foodInfos[i]->LateTick(TimeDelta);
			}
		}
	}

	return result;
}

HRESULT CFoodInventory::Render()
{
	return __super::Render();
}


void CFoodInventory::All_Object_RenderOff()
{
	for (auto& part : m_TextureParts)
	{
		part->Set_RenderId(RENDER_END);
	}

	for (auto& part : m_TextParts)
	{
		part->Set_RenderId(RENDER_END);
	}

	for (_uint i = 0; i < m_foodInfos.size(); ++i)
	{
		if (nullptr != m_foodInfos.at(i))
		{
			m_foodInfos.at(i)->Set_RenderId(RENDER_END);
			m_foodInfos.at(i)->All_Object_RenderOff();
		}
	}
}


_bool CFoodInventory::Check_Exist_Food_Is_In(CFood::TYPE& eFoodType, const POINT& mousePt)
{
	for (_uint i = 0; i < m_foodInfos.size(); ++i)
	{
		if (m_foodInfos.at(i)->Check_Is_In(mousePt) && m_foodInfos.at(i)->Get_FoodNum() > 0)
		{
			eFoodType = CFood::TYPE(i);
			return true;
		}
	}
	return false;
}

void CFoodInventory::Add_FoodNums(const CFood::TYPE& eFoodType, const _int& nums)
{
	_int foodNum = (_int)m_foodInfos.at(size_t(eFoodType))->Get_FoodNum() + nums;

	if (foodNum < 0)
		return;

	m_foodInfos.at(size_t(eFoodType))->Set_FoodInfo(_uint(foodNum));

	m_allFoodNums = 0;
	for (_uint i = 0; i < m_foodInfos.size(); ++i)
	{
		m_allFoodNums += m_foodInfos.at(i)->Get_FoodNum();
	}

	m_TextParts.at(m_itemNumsTextIndex)->Set_Text(to_wstring(m_allFoodNums));
}

_bool CFoodInventory::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	Value UIDesc(kObjectType);
	{
		UIDesc.AddMember("m_fX", m_UIDesc.m_fX, allocator);
		UIDesc.AddMember("m_fY", m_UIDesc.m_fY, allocator);

		UIDesc.AddMember("m_fSizeX", m_UIDesc.m_fSizeX, allocator);
		UIDesc.AddMember("m_fSizeY", m_UIDesc.m_fSizeY, allocator);
		UIDesc.AddMember("m_ShaderPass", m_UIDesc.m_ShaderPass, allocator);
		UIDesc.AddMember("m_UIType", m_UIDesc.m_UIType, allocator);

		Value m_vColor(kObjectType);
		{
			m_vColor.AddMember("x", m_UIDesc.m_vColor.x, allocator);
			m_vColor.AddMember("y", m_UIDesc.m_vColor.y, allocator);
			m_vColor.AddMember("z", m_UIDesc.m_vColor.z, allocator);
			m_vColor.AddMember("w", m_UIDesc.m_vColor.w, allocator);
		}
		UIDesc.AddMember("m_vColor", m_vColor, allocator);

		UIDesc.AddMember("m_TextureProtoTypeLevel", m_UIDesc.m_TextureProtoTypeLevel, allocator);
		Value m_TextureProtoTypeName;
		string TextureProtoTypeName = convert.to_bytes(m_UIDesc.m_TextureProtoTypeName);
		m_TextureProtoTypeName.SetString(TextureProtoTypeName.c_str(), (SizeType)TextureProtoTypeName.size(), allocator);
		UIDesc.AddMember("m_TextureProtoTypeName", m_TextureProtoTypeName, allocator);

		Value TextureParts(kArrayType);
		for (auto& iter : m_TextureParts)
		{
			Value Parts(kObjectType);
			{
				CPartTexture::UI_DESC desc = iter->Get_UIDesc();

				Parts.AddMember("m_fX", desc.m_fX, allocator);
				Parts.AddMember("m_fY", desc.m_fY, allocator);
				Parts.AddMember("m_fSizeX", desc.m_fSizeX, allocator);
				Parts.AddMember("m_fSizeY", desc.m_fSizeY, allocator);
				Parts.AddMember("m_eType", desc.m_eType, allocator);
				Parts.AddMember("m_ShaderPass", desc.m_ShaderPass, allocator);

				Value m_vColor(kObjectType);
				{
					m_vColor.AddMember("x", desc.m_vColor.x, allocator);
					m_vColor.AddMember("y", desc.m_vColor.y, allocator);
					m_vColor.AddMember("z", desc.m_vColor.z, allocator);
					m_vColor.AddMember("w", desc.m_vColor.w, allocator);
				}
				Parts.AddMember("m_vColor", m_vColor, allocator);

				Parts.AddMember("m_TextureProtoTypeLevel", desc.m_TextureProtoTypeLevel, allocator);

				Value valTextureProtoTypeName;
				string textureProtoTypeName = convert.to_bytes(desc.m_TextureProtoTypeName);
				valTextureProtoTypeName.SetString(textureProtoTypeName.c_str(), (SizeType)textureProtoTypeName.size(), allocator);
				Parts.AddMember("m_TextureProtoTypeName", valTextureProtoTypeName, allocator);

				Value valLayer;
				string layerTag = convert.to_bytes(Get_LayerTag());
				valLayer.SetString(layerTag.c_str(), (SizeType)layerTag.size(), allocator);
				Parts.AddMember("LayerTag", valLayer, allocator);
			}

			TextureParts.PushBack(Parts, allocator);
		}
		UIDesc.AddMember("m_TextureParts", TextureParts, allocator);

		Value TextParts(kArrayType);
		UIDesc.AddMember("m_TextParts", TextParts, allocator);

		Value FoodInfos(kArrayType);
		for (size_t i = 0; i < m_foodInfos.size(); ++i)
		{
			_uint num = m_foodInfos.at(i)->Get_FoodNum();

			FoodInfos.PushBack(num, allocator);
		}
		UIDesc.AddMember("m_foodInfos", FoodInfos, allocator);

	}
	doc.AddMember("UIDesc", UIDesc, allocator);

	return true;
}

_bool CFoodInventory::Load_By_JsonFile_Impl(Document& doc)
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

		pTexturePart->Update_FinalMatrix();

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

	if (FAILED(Init_FoodInfoUIs()))
		return false;

	m_allFoodNums = 0;

	const Value& foodInfos = UIDesc["m_foodInfos"].GetArray();
	for (SizeType i = 0; i < foodInfos.Size(); ++i)
	{
		_uint foodNums = foodInfos[i].GetUint();
		m_allFoodNums += foodNums;
		m_foodInfos.at(i)->Set_FoodInfo(CFood::TYPE(i), foodNums);
	}

	m_TextParts.at(m_itemNumsTextIndex)->Set_Text(to_wstring(m_allFoodNums));

	return true;
}

HRESULT CFoodInventory::Init_FoodInfoUIs()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	m_foodInfos.reserve(m_maxFoodInfo);
	
	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_FEEDING, L"Layer_UI", L"FoodInfoUI01");
	if (nullptr == pObject)
		return E_FAIL;
	CFoodInfoUI* pFoodInfoUI01 = dynamic_cast<CFoodInfoUI*>(pObject);
	if (nullptr == pFoodInfoUI01)
		return E_FAIL;
	Safe_AddRef(pFoodInfoUI01);
	m_foodInfos.push_back(pFoodInfoUI01);

	pObject = pGameInstance->Get_Object(LEVEL_FEEDING, L"Layer_UI", L"FoodInfoUI02");
	if (nullptr == pObject)
		return E_FAIL;
	CFoodInfoUI* pFoodInfoUI02 = dynamic_cast<CFoodInfoUI*>(pObject);
	if (nullptr == pFoodInfoUI02)
		return E_FAIL;
	Safe_AddRef(pFoodInfoUI02);
	m_foodInfos.push_back(pFoodInfoUI02);

	pObject = pGameInstance->Get_Object(LEVEL_FEEDING, L"Layer_UI", L"FoodInfoUI03");
	if (nullptr == pObject)
		return E_FAIL;
	CFoodInfoUI* pFoodInfoUI03 = dynamic_cast<CFoodInfoUI*>(pObject);
	if (nullptr == pFoodInfoUI03)
		return E_FAIL;
	Safe_AddRef(pFoodInfoUI03);
	m_foodInfos.push_back(pFoodInfoUI03);

	pObject = pGameInstance->Get_Object(LEVEL_FEEDING, L"Layer_UI", L"FoodInfoUI04");
	if (nullptr == pObject)
		return E_FAIL;
	CFoodInfoUI* pFoodInfoUI04 = dynamic_cast<CFoodInfoUI*>(pObject);
	if (nullptr == pFoodInfoUI04)
		return E_FAIL;
	Safe_AddRef(pFoodInfoUI04);
	m_foodInfos.push_back(pFoodInfoUI04);

	return S_OK;
}


CFoodInventory* CFoodInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFoodInventory* pInstance = new CFoodInventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CFoodInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFoodInventory::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CFoodInventory* pInstance = new CFoodInventory(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CFoodInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFoodInventory::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CFoodInventory* pInstance = new CFoodInventory(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CFoodInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFoodInventory::Free()
{
	__super::Free();

	for (auto& iter : m_foodInfos)
	{
		Safe_Release(iter);
	}

}
