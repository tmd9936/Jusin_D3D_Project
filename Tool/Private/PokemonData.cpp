#include "stdafx.h"
#include "PokemonData.h"

#include "GameInstance.h"

CPokemonData::CPokemonData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CPokemonData::CPokemonData(const CPokemonData& rhs)
	: CGameObject(rhs)
	, m_ConditionData_Desc(rhs.m_ConditionData_Desc)
	, m_ConditionTypeData_Desc(rhs.m_ConditionTypeData_Desc)
	, m_ConditionParameter(rhs.m_ConditionParameter)
{

}

HRESULT CPokemonData::Initialize_Prototype(
	const char* conditionDataFilePath,
	const char* conditionTypeDataFilePath,
	const char* conditionParameterFilePath)
{
	if (false == Load_By_JsonFile(conditionDataFilePath))
		return E_FAIL;

	if (false == Load_ConditionTypeData_JsonFile(conditionTypeDataFilePath))
		return E_FAIL;

	if (false == Load_ConditionParameter_JsonFile(conditionParameterFilePath))
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
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	const Value& m_datas = doc["m_datas"];
	assert(m_datas.IsArray());

	m_ConditionData_Desc.reserve((size_t)m_datas.Size());

	for (SizeType i = 0; i < m_datas.Size(); ++i)
	{
		CONDITIONDATA_DESC desc{};

		desc.m_id = m_datas[i]["m_id"].GetUint();
		desc.m_type = m_datas[i]["m_type"].GetUint();
		desc.m_Value_A = m_datas[i]["m_Value_A"].GetFloat();
		desc.m_Value_B = m_datas[i]["m_Value_B"].GetFloat();
		desc.m_time = m_datas[i]["m_time"].GetFloat();
		desc.m_ratio = m_datas[i]["m_ratio"].GetFloat();

		m_ConditionData_Desc.insert({ desc.m_id , move(desc) });
	}

	return true;
}

_bool CPokemonData::Load_ConditionParameter_JsonFile(const char* conditionParameterFilePath)
{
	FILE* fp = fopen(conditionParameterFilePath, "rb"); // non-Windows use "r"

	if (NULL == fp)
	{
		MSG_BOX("Load File Open Error");
		return false;
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
			return false;
		}

		/* 备泅何 矫累 */
		m_ConditionParameter.m_attackRatioClampMin = doc["m_attackRatioClampMin"].GetFloat();
		m_ConditionParameter.m_attackRatioClampMax = doc["m_attackRatioClampMax"].GetFloat();

		m_ConditionParameter.m_damageRatioClampMin = doc["m_damageRatioClampMin"].GetFloat();
		m_ConditionParameter.m_damageRatioClampMax = doc["m_damageRatioClampMax"].GetFloat();

		m_ConditionParameter.m_moveSpeedClampMin = doc["m_moveSpeedClampMin"].GetFloat();
		m_ConditionParameter.m_moveSpeedClampMax = doc["m_moveSpeedClampMax"].GetFloat();

		m_ConditionParameter.m_resistBadConditionClampMin = doc["m_resistBadConditionClampMin"].GetFloat();
		m_ConditionParameter.m_resistBadConditionClampMax = doc["m_resistBadConditionClampMax"].GetFloat();

		/* 备泅何 场 */
		fclose(fp);
		Safe_Delete_Array(readBuffer);

		return true;
	}
}

_bool CPokemonData::Load_ConditionTypeData_JsonFile(const char* conditionTypeDataFilePath)
{
	FILE* fp = fopen(conditionTypeDataFilePath, "rb"); // non-Windows use "r"

	if (NULL == fp)
	{
		MSG_BOX("Load File Open Error");
		return false;
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
			return false;
		}

		/* 备泅何 矫累 */
		const Value& m_datas = doc["m_datas"];
		assert(m_datas.IsArray());

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

		m_ConditionTypeData_Desc.reserve((size_t)m_datas.Size());

		for (SizeType i = 0; i < m_datas.Size(); ++i)
		{
			CONDITIONTYPEDATA_DESC desc{};

			desc.m_id = m_datas[i]["m_id"].GetUint();
			desc.m_category = m_datas[i]["m_category"].GetUint();
			desc.m_iconPath = convert.from_bytes(m_datas[i]["m_iconPath"].GetString());
			desc.m_iconLSizePath = convert.from_bytes(m_datas[i]["m_iconLSizePath"].GetString());
			desc.m_effectType = m_datas[i]["m_effectType"].GetUint();
			desc.m_resistType_1 = m_datas[i]["m_resistType_1"].GetUint();
			desc.m_resistType_2 = m_datas[i]["m_resistType_2"].GetUint();
			desc.m_groupID = m_datas[i]["m_groupID"].GetUint();
			desc.m_force = m_datas[i]["m_force"].GetUint();

			m_ConditionTypeData_Desc.push_back(move(desc));
		}

		/* 备泅何 场 */

		fclose(fp);
		Safe_Delete_Array(readBuffer);

		return true;
	}
}

HRESULT CPokemonData::Add_Components()
{

	return S_OK;
}

HRESULT CPokemonData::Add_Components_By_File()
{
	return S_OK;
}

CPokemonData* CPokemonData::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const char* conditionDataFilePath, const char* conditionTypeDataFilePath,
	const char* conditionParameterFilePath)
{
	CPokemonData* pInstance = new CPokemonData(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(conditionDataFilePath,
		conditionTypeDataFilePath, conditionParameterFilePath)))
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
	m_ConditionData_Desc.clear();
	m_ConditionTypeData_Desc.clear();
}
