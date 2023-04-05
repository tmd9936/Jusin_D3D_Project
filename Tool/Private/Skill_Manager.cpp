#include "stdafx.h"
#include "Skill_Manager.h"

#include "GameInstance.h"

#include "Effect_Manager.h"

#include "Effect.h"

/*

스킬이 가지고 있다가 공격 하게 하기

*/

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

CSkill* CSkill_Manager::Create_Skill(const _tchar* pLayerTag, _uint iLevelIndex, _uint skillType, _uint damage,
	_fmatrix vParentMatrix, _float smallRotationSpeed, _float bigRotationSpeed,
	CBone* pParentBone, CTransform* pParentTransform, _fmatrix PivotMatrix, _bool bRush, _double rushSpeed, _uint extendNum)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (skillType >= m_Skill_Depend_Datas.size() || nullptr == pLayerTag || iLevelIndex >= LEVEL_END)
		return nullptr;

	CEffect_Manager* pEffect_Manager = dynamic_cast<CEffect_Manager*>(pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Effect_Manager"));

	if (nullptr == pEffect_Manager)
		return nullptr;

	CEffect* pEffect = nullptr;

	vector<CEffect*> effects;

	CSkill::Skill_Desc skill_desc = m_Skill_Desc_Datas[skillType];

	_float4x4 pivotMatrix = {};
	XMStoreFloat4x4(&pivotMatrix, PivotMatrix);


	pEffect = pEffect_Manager->Create_Effect(m_Skill_Depend_Datas[skillType].m_effects[0], pLayerTag, iLevelIndex);
	if (skill_desc.m_isEnablePotential_Charge)
	{
		pEffect->Set_Parent(pParentBone, pParentTransform, pivotMatrix);
	}
	else
	{
		_vector vPos = vParentMatrix.r[3];

		_float4 pos = {};
		XMStoreFloat4(&pos, vPos);
		pEffect->Set_Pos(pos);
	}
	Safe_Release(pEffect);
	//effects.push_back(pEffect);


	if (skill_desc.m_isEnablePotential_Nway)
	{
		for (size_t i = 1; i <= 3; ++i)
		{
			pEffect = pEffect_Manager->Create_Effect(m_Skill_Depend_Datas[skillType].m_effects[1], pLayerTag, iLevelIndex, true);

			if (nullptr == pEffect)
				return nullptr;

			_vector vParentLook = vParentMatrix.r[2];
			_vector vParentPos = vParentMatrix.r[3];

			vParentLook = XMVector3Rotate(vParentLook, XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(120.f + 120.f * i)));

			CTransform* pTransform = pEffect->Get_As<CTransform>();
			if (nullptr == pTransform)
				return nullptr;

			//pTransform->LookAt(vParentPos);

			_vector vPos = XMVectorSetW(vParentLook, 1.f);
			_float4 pos = {};
			XMStoreFloat4(&pos, vPos);
			pEffect->Set_Pos(pos);

			pEffect->Set_AttackPower(_uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f)));

			pEffect->Set_KnockBack(skill_desc.m_isEnablePotential_Knockback);
			//pEffect->Set_Pos({ 0.f, 0.f, 0.f, 0.f });

			pEffect->Set_Parent(pParentBone, pParentTransform, pivotMatrix);

			pEffect->Set_SmallRotation(smallRotationSpeed);
			pEffect->Set_BigRotation(bigRotationSpeed, 2.f);

			Safe_Release(pEffect);
			//effects.push_back(pEffect);

			for (size_t i = 2; i < m_Skill_Depend_Datas[skillType].m_effects.size() - 1; ++i)
			{
				pEffect = pEffect_Manager->Create_Effect(m_Skill_Depend_Datas[skillType].m_effects[i], pLayerTag, iLevelIndex);
				if (nullptr != pEffect)
				{
					CTransform* pTransform = pEffect->Get_As<CTransform>();
					if (nullptr == pTransform)
						continue;

					pTransform->LookAt(XMVectorSetW(vParentLook, 1.f));

					//Safe_Release(pEffect);
					_float4 pos = {};
					XMStoreFloat4(&pos, vPos);
					pEffect->Set_Pos(pos);

					if (bRush)
					{
						pEffect->Set_Rush(bRush, vParentLook, rushSpeed);
					}

					Safe_Release(pEffect);
					//effects.push_back(pEffect);
				}
			}
		}
	}
	else if (skill_desc.m_isEnablePotential_Extend)
	{
		_vector vParentLook = vParentMatrix.r[2];
		_vector vParentPos = vParentMatrix.r[3];


		for (size_t i = 1; i <= extendNum; ++i)
		{
			pEffect = pEffect_Manager->Create_Effect(m_Skill_Depend_Datas[skillType].m_effects[1], pLayerTag, iLevelIndex, true);

			if (nullptr == pEffect)
				return nullptr;

			//pTransform->LookAt(vParentPos);

			CTransform* pTransform = pEffect->Get_As<CTransform>();
			if (nullptr == pTransform)
				return nullptr;

			pTransform->LookAt(XMVectorSetW(vParentLook, 1.f));

			pEffect->Set_KnockBack(skill_desc.m_isEnablePotential_Knockback);

			_vector vPos = XMVectorSetW(vParentPos + vParentLook * (_float)i, 1.f);
			_float4 pos = {};
			XMStoreFloat4(&pos, vPos);
			pEffect->Set_Pos(pos);

			pEffect->Set_SmallRotation(smallRotationSpeed);
			pEffect->Set_BigRotation(bigRotationSpeed, 2.f);

			if (bRush)
			{
				pEffect->Set_Rush(bRush, vParentLook, rushSpeed);
			}


			pEffect->Set_AttackPower(_uint(damage* skill_desc.m_damagePercent* ((rand() % 10 + 95) * 0.01f)));

			Safe_Release(pEffect);
			//effects.push_back(pEffect);
		}

		for (size_t i = 2; i < m_Skill_Depend_Datas[skillType].m_effects.size() - 1; ++i)
		{
			pEffect = pEffect_Manager->Create_Effect(m_Skill_Depend_Datas[skillType].m_effects[i], pLayerTag, iLevelIndex);
			if (nullptr != pEffect)
			{
				CTransform* pTransform = pEffect->Get_As<CTransform>();
				if (nullptr == pTransform)
					continue;

				pTransform->LookAt(XMVectorSetW(vParentLook, 1.f));

				//Safe_Release(pEffect);
				_float4 pos = {};
				XMStoreFloat4(&pos, vParentPos);
				pEffect->Set_Pos(pos);

				if (bRush)
				{
					pEffect->Set_Rush(bRush, vParentLook, rushSpeed);
				}

				Safe_Release(pEffect);
				//effects.push_back(pEffect);
			}
		}
	}
	else
	{
		_vector vLook = vParentMatrix.r[2];
		_vector vPos = vParentMatrix.r[3];
		pEffect = pEffect_Manager->Create_Effect(m_Skill_Depend_Datas[skillType].m_effects[1], pLayerTag, iLevelIndex, true);

		CTransform* pTransform = pEffect->Get_As<CTransform>();
		if (nullptr == pTransform)
			return nullptr;

		pTransform->LookAt(XMVectorSetW(vLook, 1.f));

		pEffect->Set_AttackPower(_uint(damage* skill_desc.m_damagePercent* ((rand() % 10 + 95) * 0.01f)));

		pEffect->Set_KnockBack(skill_desc.m_isEnablePotential_Knockback);

		_float4 pos = {};
		XMStoreFloat4(&pos, vPos + vLook * 0.5f);
		pEffect->Set_Pos(pos);
		if (bRush)
		{
			pEffect->Set_Rush(bRush, vLook, rushSpeed);
		}

		Safe_Release(pEffect);
		//effects.push_back(pEffect);

		for (size_t i = 2; i < m_Skill_Depend_Datas[skillType].m_effects.size() - 1; ++i)
		{
			pEffect = pEffect_Manager->Create_Effect(m_Skill_Depend_Datas[skillType].m_effects[i], pLayerTag, iLevelIndex);
			if (nullptr != pEffect)
			{
				CTransform* pTransform = pEffect->Get_As<CTransform>();
				if (nullptr == pTransform)
					continue;

				pTransform->LookAt(XMVectorSetW(vLook, 1.f));

				//Safe_Release(pEffect);
				_float4 pos = {};
				XMStoreFloat4(&pos, vPos);
				pEffect->Set_Pos(pos);

				if (bRush)
				{
					pEffect->Set_Rush(bRush, vLook, rushSpeed);
				}

				Safe_Release(pEffect);
				//effects.push_back(pEffect);
			}
		}
	}

	vector<CEffect*> conditions;
	//for (auto& conditionsIndex : m_Skill_Depend_Datas[skillType].m_conditions)
	//{
	//	pEffect = pEffect_Manager->Create_Effect(conditionsIndex, pLayerTag, iLevelIndex);
	//	if (nullptr != pEffect)
	//	{
	//		//Safe_Release(pEffect);
	//		conditions.push_back(pEffect);
	//	}
	//}

	CSkill* pSkill = nullptr;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Skill"), iLevelIndex, pLayerTag, (CGameObject**)&pSkill, nullptr, &skill_desc)))
		return nullptr;

	//pSkill->Set_Effects(effects);
	//pSkill->Set_Conditions(conditions);

	Safe_Release(pGameInstance);

	return pSkill;
}


CSkill* CSkill_Manager::Create_Monster_Skill(const _tchar* pLayerTag, _uint iLevelIndex, _uint skillType, _uint damage,
	_fmatrix vParentMatrix, _float smallRotationSpeed, _float bigRotationSpeed,
	CBone* pParentBone, CTransform* pParentTransform, _fmatrix PivotMatrix, _bool bRush, _double rushSpeed, _uint extendNum)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (skillType >= m_Skill_Depend_Datas.size() || nullptr == pLayerTag || iLevelIndex >= LEVEL_END)
		return nullptr;

	CEffect_Manager* pEffect_Manager = dynamic_cast<CEffect_Manager*>(pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Effect_Manager"));

	if (nullptr == pEffect_Manager)
		return nullptr;

	CSkill::Skill_Desc skill_desc = m_Skill_Desc_Datas[skillType];

	_float4x4 pivotMatrix = {};
	XMStoreFloat4x4(&pivotMatrix, PivotMatrix);

	CSkill* pSkill = nullptr;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Skill"), iLevelIndex, pLayerTag, (CGameObject**)&pSkill, nullptr, &skill_desc)))
		return nullptr;

	vector<_uint> chargeEffects;
	vector<_uint> attackEffects;
	vector<_uint> collisionEffects;
	vector<_uint> conditionEffects;

	chargeEffects.push_back(m_Skill_Depend_Datas[skillType].m_effects[0]);

	if (m_Skill_Depend_Datas[skillType].m_effects.size() == 2)
	{
		attackEffects.push_back(m_Skill_Depend_Datas[skillType].m_effects[1]);
	}

	if (m_Skill_Depend_Datas[skillType].m_effects.size() == 3)
	{
		collisionEffects.push_back(m_Skill_Depend_Datas[skillType].m_effects[2]);
	}

	//pSkill->Set_ChargeEffectTypes(m_Skill_Depend_Datas[skillType].m_effects[0]);


}

CSkill* CSkill_Manager::Create_Test_Skill(const _tchar* pLayerTag, _uint iLevelIndex, _uint skillType, _fmatrix vParentMatrix)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (skillType >= m_Skill_Depend_Datas.size() || nullptr == pLayerTag || iLevelIndex >= LEVEL_END)
		return nullptr;

	CEffect_Manager* pEffect_Manager = dynamic_cast<CEffect_Manager*>(pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Effect_Manager"));

	if (nullptr == pEffect_Manager)
		return nullptr;

	CEffect* pEffect = nullptr;

	vector<CEffect*> effects;

	CSkill::Skill_Desc skill_desc = m_Skill_Desc_Datas[skillType];

	_vector vLook = vParentMatrix.r[2];
	_vector vPos = vParentMatrix.r[3];

	size_t i = 0;
	for (auto& effectIndex : m_Skill_Depend_Datas[skillType].m_effects)
	{
		pEffect = pEffect_Manager->Create_Effect(effectIndex, pLayerTag, iLevelIndex);
		if (nullptr != pEffect)
		{
			CTransform* pTransform = pEffect->Get_As<CTransform>();
			if (nullptr == pTransform)
				continue;

			pTransform->LookAt(XMVectorSetW(vLook, 1.f));

			//Safe_Release(pEffect);
			_float4 pos = {};
			XMStoreFloat4(&pos, vPos);
			pEffect->Set_Pos(pos);
			effects.push_back(pEffect);
		}
	}

	vector<CEffect*> conditions;
	//for (auto& conditionsIndex : m_Skill_Depend_Datas[skillType].m_conditions)
	//{
	//	pEffect = pEffect_Manager->Create_Effect(conditionsIndex, pLayerTag, iLevelIndex);
	//	if (nullptr != pEffect)
	//	{
	//		_float4 pos = {};
	//		XMStoreFloat4(&pos, vPos);
	//		pEffect->Set_Pos(pos);

	//		//Safe_Release(pEffect);
	//		conditions.push_back(pEffect);
	//	}
	//}

	CSkill* pSkill = nullptr;

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

		/* 구현부 시작 */
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

		/* 구현부 끝 */


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

	m_Skill_Desc_Datas.clear();
	m_Skill_Depend_Datas.clear();
}