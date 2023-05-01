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
	m_stones.resize(m_InventoryNums, nullptr);

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;


	//if (FAILED(Init_PokemonData()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CStoneInventory::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	m_stones.resize(m_InventoryNums, nullptr);

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;


	//if (FAILED(Init_PokemonData()))
	//	return E_FAIL;

	return S_OK;
}

_uint CStoneInventory::Tick(_double TimeDelta)
{
	for (auto& iter : m_stones)
	{
		if (nullptr != iter)
		{
			iter->Tick(TimeDelta);
		}
	}

	return __super::Tick(TimeDelta);
}

_uint CStoneInventory::LateTick(_double TimeDelta)
{
	_uint result = 0;
	if (m_lateTickState)
	{
		result = __super::LateTick(TimeDelta);

		for (size_t i = 0; i < m_stones.size(); ++i)
		{
			if (nullptr != m_stones[i])
			{
				_float3 texturePosition = m_TextureParts[i]->Get_FinalWorldMatrixPosition();
				m_stones[i]->Set_Pos({ texturePosition.x, texturePosition.y, texturePosition.z });

				m_stones[i]->LateTick(TimeDelta);
			}
		}
	}

	return result;
}

HRESULT CStoneInventory::Render()
{
	return __super::Render();
}

_bool CStoneInventory::Check_Exist_Stone_Is_In(CStone::STONE_DESC& output, _uint& pickingStoneIndex, const POINT& mousePt)
{
	for (_uint i = 0; i < m_stones.size(); ++i)
	{
		if (nullptr != m_stones[i])
		{
			if (m_stones[i]->Check_Is_In(mousePt) && m_stones[i]->Get_StoneState() == CStone::STATE_NO_EQUIP_ON_INVENTORY)
			{
				output.m_stoneType = m_stones[i]->Get_StoneType();
				output.value = m_stones[i]->Get_Value();
				output.m_inventoyIndex = i;
				pickingStoneIndex = i;

				return true;
			}
		}
	}

	return false;
}

_bool CStoneInventory::Change_StoneIndex(const _uint& originIndex, const POINT& mousePt)
{
	for (_uint i = 0; i < m_TextureParts.size(); ++i)
	{
		if (m_TextureParts[i]->Check_Is_In(mousePt))
		{
			if (i != originIndex)
			{
				CStone* pTemp = m_stones[originIndex];
				m_stones[originIndex] = m_stones[i];
				m_stones[i] = pTemp;
				m_stones[i]->Set_InventoryIndex(i);

				m_TextureParts[i]->Change_Texture(m_equipTextureProtoTypeTag.c_str());
				m_TextureParts[i]->Set_Scaled(m_eqiupScale);
				if (nullptr == m_stones[originIndex])
				{
					m_TextureParts[originIndex]->Change_Texture(m_nonEquipTextureProtoTypeTag.c_str());
					m_TextureParts[originIndex]->Set_Scaled(m_nonEqiupScale);
				}
				else
				{
					m_stones[originIndex]->Set_InventoryIndex(originIndex);
				}

				return true;
			}
		}
	}
	return false;
}

_bool CStoneInventory::Change_StoneState_To_Equip(const _uint& index, const _uint& pokemonNo)
{
	if (index >= m_stones.size())
		return false;

	m_stones[index]->Set_State(CStone::STATE_EQUIP_ON_INVENTORY);
	m_stones[index]->Change_PokemonIcon(pokemonNo);

	return true;
}

_bool CStoneInventory::Change_StoneState_To_UnEquip(const _uint& index)
{
	if (index >= m_stones.size())
		return false;

	m_stones[index]->Set_State(CStone::STATE_NO_EQUIP_ON_INVENTORY);

	return true;
}

void CStoneInventory::All_Object_RenderOff()
{
	for (auto& part : m_TextureParts)
	{
		part->Set_RenderId(RENDER_END);
	}

	for (auto& part : m_TextParts)
	{
		part->Set_RenderId(RENDER_END);
	}

	for (_uint i = 0; i < m_stones.size(); ++i)
	{
		if (nullptr != m_stones.at(i))
		{
			m_stones.at(i)->Set_RenderId(RENDER_END);
			m_stones.at(i)->All_Object_RenderOff();
		}
	}
}

HRESULT CStoneInventory::Add_StoneData(CStone::STONE_DESC& stoneData)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CStone* pStone = nullptr;

	_int emptyIndex = -1;

	for (size_t i = 0; i < m_stones.size(); ++i)
	{
		if (nullptr == m_stones.at(i))
		{
			emptyIndex = (_int)i;
			break;
		}
	}

	if (-1 == emptyIndex)
		return E_FAIL;

	pGameInstance->Clone_GameObject(Get_LayerTag().c_str(), m_iLevelindex, 
		TEXT("Prototype_GameObject_Stone"), (CGameObject**)&pStone, &stoneData);
	if (nullptr == pStone)
		return E_FAIL;

	pStone->Set_InventoryIndex(emptyIndex);
	m_stones[emptyIndex] = pStone;

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

		Value Stones(kArrayType);
		for (size_t i = 0; i < m_stones.size(); ++i)
		{
			if (nullptr == m_stones[i])
				continue;

			Value stone(kObjectType);
			{
				CStone::Stone_Desc stoneDesc = m_stones[i]->Get_StoneDesc();
				stone.AddMember("m_inventoyIndex", stoneDesc.m_inventoyIndex, allocator);
				stone.AddMember("m_eCurState", (_uint)stoneDesc.m_eCurState, allocator);
				stone.AddMember("m_stoneType", (_uint)stoneDesc.m_stoneType, allocator);
				stone.AddMember("value", stoneDesc.value, allocator);
				stone.AddMember("m_pokemonIconNumber", stoneDesc.m_pokemonIconNumber, allocator);
			}

			Stones.PushBack(stone, allocator);
		}
		UIDesc.AddMember("m_stones", Stones, allocator);

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

	const Value& stones = UIDesc["m_stones"].GetArray();
	for (SizeType i = 0; i < stones.Size(); ++i)
	{
		CStone* pStone = nullptr;

		CStone::STONE_DESC stoneDesc{};

		stoneDesc.m_inventoyIndex = stones[i]["m_inventoyIndex"].GetUint();
		stoneDesc.m_eCurState = (CStone::STATE)stones[i]["m_eCurState"].GetUint();
		stoneDesc.m_stoneType = (CStone::TYPE)stones[i]["m_stoneType"].GetUint();
		stoneDesc.value = stones[i]["value"].GetUint();
		stoneDesc.m_pokemonIconNumber = stones[i]["m_pokemonIconNumber"].GetUint();

		stoneDesc.m_UIDesc.m_fSizeX = 45.f;
		stoneDesc.m_UIDesc.m_fSizeY = 45.f;
		stoneDesc.m_UIDesc.m_fX = 0.f;
		stoneDesc.m_UIDesc.m_fY = 0.f;
		stoneDesc.m_UIDesc.m_ShaderPass = 0;
		stoneDesc.m_UIDesc.m_TextureProtoTypeLevel = 0;
		stoneDesc.m_UIDesc.m_UIType = 0;
		lstrcpy(stoneDesc.m_UIDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_UI_Pstone_attack");

		pGameInstance->Clone_GameObject(Get_LayerTag().c_str(), m_iLevelindex, TEXT("Prototype_GameObject_Stone"), (CGameObject**)&pStone, &stoneDesc);
		if (nullptr == pStone)
			return false;

		CTransform* pStoneTransform = pStone->Get_As<CTransform>();

		if (nullptr == pStoneTransform)
			return false;

		_float3 texturePosition = m_TextureParts[stoneDesc.m_inventoyIndex]->Get_FinalWorldMatrixPosition();
		pStoneTransform->Set_Pos(texturePosition.x + 5.f, texturePosition.y - 5.f, texturePosition.z);

		m_TextureParts[stoneDesc.m_inventoyIndex]->Change_Texture(m_equipTextureProtoTypeTag.c_str());
		m_TextureParts[stoneDesc.m_inventoyIndex]->Set_Scaled(m_eqiupScale);

		m_stones[stoneDesc.m_inventoyIndex] = pStone;
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
