#include "stdafx.h"
#include "EnemyPack.h"

#include "GameInstance.h"

CEnemyPack::CEnemyPack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CEnemyPack::CEnemyPack(const CEnemyPack& rhs)
	: CGameObject(rhs)
{

}

HRESULT CEnemyPack::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemyPack::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_Desc.m_Name = (*(ENEMY_PACK_DESC*)(pArg)).m_Name;
		m_Desc.m_registDatas = (*(ENEMY_PACK_DESC*)(pArg)).m_registDatas;
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnemyPack::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(Add_Components_By_Json()))
		return E_FAIL;

	return S_OK;
}

_uint CEnemyPack::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CEnemyPack::LateTick(_double TimeDelta)
{
	return _uint();
}

HRESULT CEnemyPack::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

_bool CEnemyPack::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	Value EnemyPackDesc(kObjectType);
	{
		Value m_NameValue;
		string name = convert.to_bytes(m_Desc.m_Name);
		m_NameValue.SetString(name.c_str(), (SizeType)name.size(), allocator);
		EnemyPackDesc.AddMember("m_Name", m_NameValue, allocator);

		Value m_registDatas(kArrayType);
		for (size_t i = 0; i < m_Desc.m_registDatas.size(); ++i)
		{
			Value registData(kObjectType);
			{
				Value m_enemyFilePath;
				m_enemyFilePath.SetString(m_Desc.m_registDatas[i].m_enemyFilePath.c_str(), (SizeType)m_Desc.m_registDatas[i].m_enemyFilePath.size(), allocator);
				registData.AddMember("m_enemyFilePath", m_enemyFilePath, allocator);

				registData.AddMember("m_enemyNumber", m_Desc.m_registDatas[i].m_enemyNumber, allocator);
				registData.AddMember("m_isBoss", m_Desc.m_registDatas[i].m_isBoss, allocator);
				registData.AddMember("m_setType", m_Desc.m_registDatas[i].m_setType, allocator);
				registData.AddMember("m_setIndex", m_Desc.m_registDatas[i].m_setIndex, allocator);
				registData.AddMember("m_spawnMode", m_Desc.m_registDatas[i].m_spawnMode, allocator);

			}
			m_registDatas.PushBack(registData, allocator);
		}
		EnemyPackDesc.AddMember("m_registDatas", m_registDatas, allocator);

	}
	doc.AddMember("EnemyPackDesc", EnemyPackDesc, allocator);

	return true;
}

_bool CEnemyPack::Load_By_JsonFile_Impl(Document& doc)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	const Value& EnemyPackDesc = doc["EnemyPackDescsss"];

	string m_Name = EnemyPackDesc["m_Name"].GetString();
	m_Desc.m_Name = convert.from_bytes(m_Name);

	const Value& m_registDatas = EnemyPackDesc["m_registDatas"].GetArray();
	assert(m_registDatas.IsArray());

	for (SizeType i = 0; i < m_registDatas.Size(); ++i)
	{
		REGIST_DATA_DESC registDataDesc{};

		registDataDesc.m_enemyFilePath = m_registDatas[i]["m_enemyFilePath"].GetString();
		registDataDesc.m_enemyNumber = m_registDatas[i]["m_enemyNumber"].GetUint();
		registDataDesc.m_isBoss = m_registDatas[i]["m_isBoss"].GetBool();
		registDataDesc.m_setType = m_registDatas[i]["m_setType"].GetUint();
		registDataDesc.m_setIndex = m_registDatas[i]["m_setIndex"].GetUint();
		registDataDesc.m_spawnMode = m_registDatas[i]["m_spawnMode"].GetUint();

		m_Desc.m_registDatas.push_back(move(registDataDesc));
	}

	return true;
}


HRESULT CEnemyPack::Add_Components()
{
	return S_OK;
}

HRESULT CEnemyPack::Add_Components_By_Json()
{
	return S_OK;
}


HRESULT CEnemyPack::SetUp_ShaderResources()
{
	return S_OK;
}

CEnemyPack* CEnemyPack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEnemyPack* pInstance = new CEnemyPack(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CEnemyPack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemyPack::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CEnemyPack* pInstance = new CEnemyPack(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CEnemyPack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemyPack::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CEnemyPack* pInstance = new CEnemyPack(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CEnemyPack");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CEnemyPack::Free()
{
	__super::Free();
}
