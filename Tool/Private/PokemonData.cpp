#include "stdafx.h"
#include "PokemonData.h"

#include "GameInstance.h"

CPokemonData::CPokemonData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CPokemonData::CPokemonData(const CPokemonData& rhs)
	: CGameObject(rhs)
	, m_PokemonData_Desc(rhs.m_PokemonData_Desc)
{

}

HRESULT CPokemonData::Initialize_Prototype(const char* pokemonnDataFilePath)
{
	if (false == Load_By_JsonFile(pokemonnDataFilePath))
		return E_FAIL;

	return S_OK;
}

HRESULT CPokemonData::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

HRESULT CPokemonData::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(Add_Components_By_File()))
		return E_FAIL;

	return S_OK;
}

_uint CPokemonData::Tick(_double TimeDelta)
{

	return _uint();
}

_uint CPokemonData::LateTick(_double TimeDelta)
{
	return _uint();
}


_bool CPokemonData::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	return true;
}

_bool CPokemonData::Load_By_JsonFile_Impl(Document& doc)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	const Value& m_datas = doc["m_datas"];
	assert(m_datas.IsArray());

	m_PokemonData_Desc.reserve((size_t)m_datas.Size());

	for (SizeType i = 0; i < m_datas.Size(); ++i)
	{
		POKEMONDATA_DESC desc{};

		desc.id = m_datas[i]["id"].GetUint();
		desc.name = convert.from_bytes(m_datas[i]["name"].GetString());
		desc.type = convert.from_bytes(m_datas[i]["type"].GetString());

		m_PokemonData_Desc.push_back(move(desc));
	}

	return true;
}


HRESULT CPokemonData::Add_Components()
{
	return S_OK;
}

HRESULT CPokemonData::Add_Components_By_File()
{
	return S_OK;
}

CPokemonData* CPokemonData::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pokemonnDataFilePath)
{
	CPokemonData* pInstance = new CPokemonData(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pokemonnDataFilePath)))
	{
		MSG_BOX("Failed to Created CPokemonData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokemonData::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CPokemonData* pInstance = new CPokemonData(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CPokemonData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokemonData::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CPokemonData* pInstance = new CPokemonData(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CPokemonData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPokemonData::Free()
{
	__super::Free();
	m_PokemonData_Desc.clear();
}
