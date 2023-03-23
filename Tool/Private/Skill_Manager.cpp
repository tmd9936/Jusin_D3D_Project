#include "stdafx.h"
#include "Skill_Manager.h"

#include "GameInstance.h"

#include "Effect_Manager.h"

#include "Effect.h"

CSkill_Manager::CSkill_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CSkill_Manager::CSkill_Manager(const CSkill_Manager& rhs)
	: CGameObject(rhs)
	, m_Skill_Depend_Datas(rhs.m_Skill_Depend_Datas)
	, m_Skill_Desc_Datas(rhs.m_Skill_Desc_Datas)
{

}

HRESULT CSkill_Manager::Initialize_Prototype(const char* skillDepnedDataFilePath, const char* skillDataResourceFilePath)
{
	if (false == Load_By_JsonFile(skillDepnedDataFilePath))
		return E_FAIL;

	m_skillDepnedDataFilePath = skillDepnedDataFilePath;

	if (false == Load_SkillDataResourcesSet_JsonFile(skillDataResourceFilePath))
		return E_FAIL;

	m_skillDataResourceFilePath = skillDataResourceFilePath;

	return S_OK;
}


HRESULT CSkill_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}


_uint CSkill_Manager::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CSkill_Manager::LateTick(_double TimeDelta)
{
	return _uint();
}

CSkill* CSkill_Manager::Create_Skill(const _tchar* pLayerTag, _uint iLevelIndex, _uint skillType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (skillType >= m_Skill_Depend_Datas.size() || nullptr == pLayerTag || iLevelIndex >= LEVEL_END)
		return nullptr;

	CEffect_Manager* pEffect_Manager = dynamic_cast<CEffect_Manager*>(pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Effect_Manager"));

	if (nullptr == pEffect_Manager)
		return nullptr;;

	CEffect* pEffect = nullptr;

	vector<CEffect*> effects;

	for (auto& effectIndex : m_Skill_Depend_Datas[skillType].m_effects)
	{
		pEffect = pEffect_Manager->Create_Effect(effectIndex, pLayerTag, iLevelIndex);
		if (nullptr != pEffect)
		{
			effects.push_back(pEffect);
		}
	}

	vector<CEffect*> conditions;
	for (auto& effectIndex : m_Skill_Depend_Datas[skillType].m_conditions)
	{
		pEffect = pEffect_Manager->Create_Effect(effectIndex, pLayerTag, iLevelIndex);
		if (nullptr != pEffect)
		{
			conditions.push_back(pEffect);
		}
	}

	CSkill* pSkill = nullptr;

	CSkill::Skill_Desc skill_desc = m_Skill_Desc_Datas[skillType];

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Skill"), iLevelIndex, pLayerTag, (CGameObject**)&pSkill, nullptr, &skill_desc)))
		return nullptr;

	pSkill->Set_Effects(effects);
	pSkill->Set_Conditions(conditions);

	Safe_Release(pGameInstance);

	return pSkill;
}


HRESULT CSkill_Manager::Reload_Datas()
{
	m_Skill_Depend_Datas.clear();
	m_Skill_Desc_Datas.clear();

	if (false == Load_By_JsonFile(m_skillDepnedDataFilePath.c_str()))
		return E_FAIL;

	if (false == Load_SkillDataResourcesSet_JsonFile(m_skillDataResourceFilePath.c_str()))
		return E_FAIL;

	return S_OK;
}

_bool CSkill_Manager::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{

	return true;
}

_bool CSkill_Manager::Load_By_JsonFile_Impl(Document& doc)
{
	const Value& m_datas = doc["m_datas"];
	assert(m_datas.IsArray());

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	for (SizeType i = 0; i < m_datas.Size(); ++i)
	{
		SKILL_DEPEND_DATA m_desc{};

		const Value& m_effects = m_datas[i]["m_effects"];
		assert(m_effects.IsArray());
		for (SizeType j = 0; j < m_effects.Size(); ++j)
		{
			m_desc.m_effects.push_back(m_effects[j].GetUint());
		}

		const Value& m_conditions = m_datas[i]["m_conditions"];
		assert(m_conditions.IsArray());
		for (SizeType j = 0; j < m_conditions.Size(); ++j)
		{
			m_desc.m_conditions.push_back(m_conditions[j].GetUint());

		}

		m_Skill_Depend_Datas.push_back(m_desc);
	}

	return true;
}

bool CSkill_Manager::Load_SkillDataResourcesSet_JsonFile(const char* filePath)
{
	FILE* fp = fopen(filePath, "rb"); // non-Windows use "r"

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

		for (SizeType i = 0; i < m_datas.Size(); ++i)
		{
			CSkill::SKILL_DESC m_desc{};

			m_desc.m_skillPath = convert.from_bytes(m_datas[i]["m_skillPath"].GetString());
			m_desc.m_iconPath = convert.from_bytes(m_datas[i]["m_iconPath"].GetString());
			m_desc.m_damagePercent = m_datas[i]["m_damagePercent"].GetFloat();
			m_desc.m_chargeSecond = m_datas[i]["m_chargeSecond"].GetFloat();
			m_desc.m_rapidDelay = m_datas[i]["m_rapidDelay"].GetFloat();
			m_desc.m_isEnablePotential_Nway = (_bool)m_datas[i]["m_isEnablePotential_Nway"].GetUint();
			m_desc.m_isEnablePotential_Distance = (_bool)m_datas[i]["m_isEnablePotential_Distance"].GetUint();
			m_desc.m_isEnablePotential_Extend = (_bool)m_datas[i]["m_isEnablePotential_Extend"].GetUint();
			m_desc.m_isEnablePotential_Homing = (_bool)m_datas[i]["m_isEnablePotential_Homing"].GetUint();
			m_desc.m_isEnablePotential_Continue = (_bool)m_datas[i]["m_isEnablePotential_Continue"].GetUint();
			m_desc.m_isEnablePotential_Rapid = (_bool)m_datas[i]["m_isEnablePotential_Rapid"].GetUint();
			m_desc.m_isEnablePotential_Charge = (_bool)m_datas[i]["m_isEnablePotential_Charge"].GetUint();
			m_desc.m_isEnablePotential_ConditionDuration = (_bool)m_datas[i]["m_isEnablePotential_ConditionDuration"].GetUint();
			m_desc.m_isEnablePotential_BuffProbUp = (_bool)m_datas[i]["m_isEnablePotential_BuffProbUp"].GetUint();
			m_desc.m_isEnablePotential_DebuffProbUp = (_bool)m_datas[i]["m_isEnablePotential_DebuffProbUp"].GetUint();
			m_desc.m_isEnablePotential_BuffTimeUp = (_bool)m_datas[i]["m_isEnablePotential_BuffTimeUp"].GetUint();
			m_desc.m_isEnablePotential_BuffShare = (_bool)m_datas[i]["m_isEnablePotential_BuffShare"].GetUint();
			m_desc.m_isEnablePotential_Knockback = (_bool)m_datas[i]["m_isEnablePotential_Knockback"].GetUint();
			m_desc.m_isEnablePotential_Damage = (_bool)m_datas[i]["m_isEnablePotential_Damage"].GetUint();

			m_Skill_Desc_Datas.push_back(m_desc);
		}

		/* 备泅何 场 */


		fclose(fp);
		Safe_Delete_Array(readBuffer);

		return true;
	}
}


HRESULT CSkill_Manager::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	return S_OK;
}


HRESULT CSkill_Manager::SetUp_ShaderResources()
{

	return S_OK;
}

CSkill_Manager* CSkill_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* skillDepnedDataFilePath, const char* skillDepnedResourceFilePath)
{
	CSkill_Manager* pInstance = new CSkill_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(skillDepnedDataFilePath, skillDepnedResourceFilePath)))
	{
		MSG_BOX("Failed to Created CSkill_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CSkill_Manager* pInstance = new CSkill_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CSkill_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CSkill_Manager::Free()
{
	__super::Free();
}