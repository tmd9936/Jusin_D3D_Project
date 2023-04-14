#include "stdafx.h"
#include "Skill_Manager.h"

#include "GameInstance.h"

#include "Effect_Manager.h"

#include "Effect.h"

#include "AttackEffect.h"

#include "RushAttackEffect.h"
#include "HommingAttackEffect.h"
#include "BezierAttackEffect.h"
#include "ChargeEffect.h"
#include "ConditionData.h"

#include "BuffState.h"


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

	if (skill_desc.m_isEnablePotential_Nway)
	{
		for (size_t i = 0; i < 3; ++i)
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

			_vector vPos = {};

			if (pParentBone)
				vPos = XMVectorSetW(vParentLook, 1.f);
			else
				vPos = XMVectorSetW(vParentPos, 1.f);

			_float4 pos = {};
			XMStoreFloat4(&pos, vPos);
			pEffect->Set_Pos(pos);

			pEffect->Set_AttackPower(_uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f)));

			pEffect->Set_KnockBack(skill_desc.m_isEnablePotential_Knockback);
			//pEffect->Set_Pos({ 0.f, 0.f, 0.f, 0.f });
			if (pParentBone)
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

HRESULT CSkill_Manager::CreateSkill(const _tchar* pLayerTag, _uint iLevelIndex,
	_uint skillType, _uint damage, _fmatrix vParentMatrix, 
	CBone * pBone, CTransform* pParentTransform, _fmatrix PivotMatrix, CBuffState* pBuffState)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (skillType >= m_Skill_Depend_Datas.size() || nullptr == pLayerTag || iLevelIndex >= LEVEL_END)
		return E_FAIL;

	CEffect_Manager* pEffect_Manager = dynamic_cast<CEffect_Manager*>(pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Effect_Manager"));

	if (nullptr == pEffect_Manager)
		return E_FAIL;

	CConditionData* pConditionData = dynamic_cast<CConditionData*>(pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"ConditionData"));

	if (nullptr == pConditionData)
		return E_FAIL;

	CSkillEffect* pSkillEffect = nullptr;

	CSkill::Skill_Desc skill_desc = m_Skill_Desc_Datas[skillType];

	_vector vLook = vParentMatrix.r[2];
	_vector vPos = vParentMatrix.r[3];

	if (skillType <= 35 && skillType % 2 == 1) // 원거리 공격
	{
		Create_No_ChargeEffect(m_Skill_Depend_Datas[skillType].m_effects[0], vLook, vPos, pLayerTag, iLevelIndex, pBone, pParentTransform, PivotMatrix);

		pSkillEffect = pEffect_Manager->CreateEffect(m_Skill_Depend_Datas[skillType].m_effects[1], L"Prototype_GameObject_RushAttackEffect", pLayerTag, iLevelIndex);

		CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
		if (nullptr == pTransform)
			return E_FAIL;
		pTransform->LookAt(XMVectorSetW(vLook, 1.f));

		_float4 pos = {};
		XMStoreFloat4(&pos, vPos + vLook * 0.5f);
		pSkillEffect->Set_Pos(pos);

		CAttackEffect::ATTACK_EFFECT_DESC desc{};
		Set_NormalAttackDesc(desc, skillType, pSkillEffect);

		dynamic_cast<CRushAttackEffect*>(pSkillEffect)->Set_RushSpeed(0.5);
		pSkillEffect->Init_LoopCount(2);

		Set_AttackPower(pSkillEffect, _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f)));

		Safe_Release(pSkillEffect);
	}
	else if (skillType == 50) // 하이드럼 펌프
	{
		for (size_t i = 1; i <= 4; ++i)
		{
			CChargeEffect::CHARGE_EFFECT_DESC desc{};
			desc.m_ChargeTime = skill_desc.m_chargeSecond * (0.15f * i);
			desc.m_NextEffectPrototypeTag = L"Prototype_GameObject_" + skill_desc.m_skillPath;
			desc.m_NextEffectTypeIndex = m_Skill_Depend_Datas[skillType].m_effects[1];
			desc.m_NextEffectType = EFFECT_TYPE_ATTACK;
			desc.m_NextEffectPower = _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f));
			desc.m_AttackDesc.m_bContinue = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Continue;
			desc.m_AttackDesc.m_CollisionEffectType = m_Skill_Depend_Datas[skillType].m_effects[2];
			desc.m_AttackDesc.m_bKnockBack = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Knockback;
			desc.m_AttackDesc.effectDesc.m_IsParts = false;

			pSkillEffect = pEffect_Manager->Create_Charge_Effect(m_Skill_Depend_Datas[skillType].m_effects[0], pLayerTag, iLevelIndex, desc);
			if (nullptr != pSkillEffect)
				pSkillEffect->Set_ParentNoParts(pBone, pParentTransform, PivotMatrix);

			CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
			if (nullptr == pTransform)
				return E_FAIL;
			pTransform->LookAt(XMVectorSetW(vLook, 1.f));
			
			//pTransform->Set_Scaled({ 0.5f, 0.5f, 0.5f });

			_float4 pos = {};
			XMStoreFloat4(&pos, vPos + vLook * _float(rand() % 5 + 2) * 0.4f);

			//pos.y = 1.5f;
			pos.x += (rand() % 10 - 5) * 0.6f;

			pSkillEffect->Set_Pos(pos);

			Safe_Release(pSkillEffect);
		}
	}
	else if (skillType == 57) // 10만 볼트
	{
		CChargeEffect::CHARGE_EFFECT_DESC desc{};
		desc.m_ChargeTime = skill_desc.m_chargeSecond * 0.2f;
		desc.m_NextEffectPrototypeTag = L"Prototype_GameObject_" + skill_desc.m_skillPath;
		desc.m_NextEffectTypeIndex = m_Skill_Depend_Datas[skillType].m_effects[1];
		desc.m_NextEffectType = EFFECT_TYPE_ATTACK;
		desc.m_NextEffectNum = 3;
		desc.m_NextEffectAngles = { XMConvertToRadians(0.f), XMConvertToRadians(120.f), XMConvertToRadians(240.f) };
		desc.m_NextEffectPower = _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f));
		desc.m_AttackDesc.m_bContinue = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Continue;
		desc.m_AttackDesc.m_CollisionEffectType = m_Skill_Depend_Datas[skillType].m_effects[2];
		desc.m_AttackDesc.m_bKnockBack = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Knockback;
		desc.m_AttackDesc.effectDesc.m_IsParts = true;

		pSkillEffect = pEffect_Manager->Create_Charge_Effect(m_Skill_Depend_Datas[skillType].m_effects[0], pLayerTag, iLevelIndex, desc);
		if (nullptr != pSkillEffect)
			pSkillEffect->Set_Parent(pBone, pParentTransform, PivotMatrix);

		CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
		if (nullptr == pTransform)
			return E_FAIL;
		pTransform->LookAt(XMVectorSetW(vLook, 1.f));

		Safe_Release(pSkillEffect);
	}
	else if (skillType == 58) // 볼태커
	{
		Create_No_ChargeEffect(m_Skill_Depend_Datas[skillType].m_effects[1], vLook, XMVectorSet(0.f, 0.1f, 0.f, 1.f), pLayerTag, iLevelIndex, pBone, pParentTransform, PivotMatrix);
		Create_No_ChargeEffect(m_Skill_Depend_Datas[skillType].m_effects[2], vLook, XMVectorSet(0.f, 0.1f, 0.f, 1.f), pLayerTag, iLevelIndex, pBone, pParentTransform, PivotMatrix);

		pSkillEffect = pEffect_Manager->CreateEffect(m_Skill_Depend_Datas[skillType].m_effects[0], L"Prototype_GameObject_AttackEffect", pLayerTag, iLevelIndex);
		if (nullptr != pSkillEffect)
			pSkillEffect->Set_Parent(pBone, pParentTransform, PivotMatrix);

		CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
		if (nullptr == pTransform)
			return E_FAIL;
		pTransform->LookAt(XMVectorSetW(vLook, 1.f));

		CAttackEffect::ATTACK_EFFECT_DESC desc{};
		Set_NormalAttackDesc(desc, skillType, pSkillEffect);

		Set_AttackPower(pSkillEffect, _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f)));

		Safe_Release(pSkillEffect);
	}
	else if (skillType == 72) // 냉동빔
	{
		CChargeEffect::CHARGE_EFFECT_DESC desc{};
		desc.m_ChargeTime = skill_desc.m_chargeSecond * 0.1f;
		desc.m_NextEffectPrototypeTag = L"Prototype_GameObject_" + skill_desc.m_skillPath;
		desc.m_NextEffectTypeIndex = m_Skill_Depend_Datas[skillType].m_effects[2];
		desc.m_NextEffectType = EFFECT_TYPE_ATTACK;
		desc.m_NextEffectPower = _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f));
		desc.m_AttackDesc.m_bContinue = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Continue;
		desc.m_AttackDesc.m_CollisionEffectType = m_Skill_Depend_Datas[skillType].m_effects[3];
		desc.m_AttackDesc.m_bKnockBack = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Knockback;
		desc.m_AttackDesc.effectDesc.m_IsParts = false;

		pSkillEffect = pEffect_Manager->Create_Charge_Effect(m_Skill_Depend_Datas[skillType].m_effects[0], pLayerTag, iLevelIndex, desc);
		if (nullptr != pSkillEffect)
			pSkillEffect->Set_Parent(pBone, pParentTransform, PivotMatrix);

		CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
		if (nullptr == pTransform)
			return E_FAIL;
		pTransform->LookAt(XMVectorSetW(vLook, 1.f));
		pTransform->Set_Pos({ 0.f, 0.f, -0.6f, 1.f });


		Safe_Release(pSkillEffect);
		//===============================================
		desc.m_ChargeTime = skill_desc.m_chargeSecond * 0.11f;
		desc.m_NextEffectPrototypeTag = L"Prototype_GameObject_" + skill_desc.m_skillPath;
		desc.m_NextEffectTypeIndex = m_Skill_Depend_Datas[skillType].m_effects[2];
		desc.m_NextEffectType = EFFECT_TYPE_ATTACK;
		desc.m_NextEffectPower = _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f));
		desc.m_AttackDesc.m_bContinue = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Continue;
		desc.m_AttackDesc.m_CollisionEffectType = m_Skill_Depend_Datas[skillType].m_effects[3];
		desc.m_AttackDesc.m_bKnockBack = false;
		desc.m_AttackDesc.effectDesc.m_IsParts = false;

		pSkillEffect = pEffect_Manager->Create_Charge_Effect(m_Skill_Depend_Datas[skillType].m_effects[0], pLayerTag, iLevelIndex, desc);
		if (nullptr != pSkillEffect)
			pSkillEffect->Set_Parent(pBone, pParentTransform, PivotMatrix);

		pTransform = pSkillEffect->Get_As<CTransform>();
		if (nullptr == pTransform)
			return E_FAIL;
		pTransform->LookAt(XMVectorSetW(vLook, 1.f));
		pTransform->Set_Pos({0.f, 0.f, -0.6f, 1.f});


		Safe_Release(pSkillEffect);
	}

	else if (skillType == 79) // 얼다 바람
	{
		CChargeEffect::CHARGE_EFFECT_DESC desc{};
		desc.m_ChargeTime = skill_desc.m_chargeSecond * 0.1f;
		desc.m_NextEffectPrototypeTag = L"Prototype_GameObject_" + skill_desc.m_skillPath;
		desc.m_NextEffectTypeIndex = m_Skill_Depend_Datas[skillType].m_effects[1];
		desc.m_NextEffectType = EFFECT_TYPE_ATTACK;
		desc.m_NextEffectPower = _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f));
		desc.m_AttackDesc.m_bContinue = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Continue;
		desc.m_AttackDesc.m_CollisionEffectType = m_Skill_Depend_Datas[skillType].m_effects[3];
		desc.m_AttackDesc.m_bKnockBack = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Knockback;
		desc.m_AttackDesc.effectDesc.m_IsParts = false;

		pSkillEffect = pEffect_Manager->Create_Charge_Effect(m_Skill_Depend_Datas[skillType].m_effects[0], pLayerTag, iLevelIndex, desc);
		if (nullptr != pSkillEffect)
			pSkillEffect->Set_Parent(pBone, pParentTransform, PivotMatrix);

		CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
		if (nullptr == pTransform)
			return E_FAIL;
		pTransform->LookAt(XMVectorSetW(vLook, 1.f));

		_float4 pos = {};
		XMStoreFloat4(&pos, vLook * 0.5f);
		pSkillEffect->Set_Pos(pos);

		Safe_Release(pSkillEffect);
		//===============================================
		desc.m_ChargeTime = skill_desc.m_chargeSecond * 0.1f;
		desc.m_NextEffectPrototypeTag = L"Prototype_GameObject_" + skill_desc.m_skillPath;
		desc.m_NextEffectTypeIndex = m_Skill_Depend_Datas[skillType].m_effects[2];
		desc.m_NextEffectType = EFFECT_TYPE_ATTACK;
		desc.m_NextEffectPower = _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 40) * 0.01f));
		desc.m_AttackDesc.m_bContinue = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Continue;
		desc.m_AttackDesc.m_CollisionEffectType = m_Skill_Depend_Datas[skillType].m_effects[3];
		desc.m_AttackDesc.m_bKnockBack = false;
		desc.m_AttackDesc.effectDesc.m_IsParts = false;

		pSkillEffect = pEffect_Manager->Create_Charge_Effect(m_Skill_Depend_Datas[skillType].m_effects[0], pLayerTag, iLevelIndex, desc);
		if (nullptr != pSkillEffect)
			pSkillEffect->Set_Parent(pBone, pParentTransform, PivotMatrix);

		pTransform = pSkillEffect->Get_As<CTransform>();
		if (nullptr == pTransform)
			return E_FAIL;
		pTransform->LookAt(XMVectorSetW(vLook, 1.f));

		pos = {};
		XMStoreFloat4(&pos, vLook * 0.5f);
		pSkillEffect->Set_Pos(pos);

		Safe_Release(pSkillEffect);
	}

	else if (skillType == 100) // 지진
	{
		CChargeEffect::CHARGE_EFFECT_DESC desc{};
		desc.m_ChargeTime = skill_desc.m_chargeSecond * 0.2f;
		desc.m_NextEffectPrototypeTag = L"Prototype_GameObject_" + skill_desc.m_skillPath;
		desc.m_NextEffectTypeIndex = m_Skill_Depend_Datas[skillType].m_effects[1];
		desc.m_NextEffectType = EFFECT_TYPE_ATTACK;
		desc.m_NextEffectNum = 4;
		desc.m_NextEffectAngles = { XMConvertToRadians(45.f), XMConvertToRadians(135.f), XMConvertToRadians(225.f), XMConvertToRadians(315.f) };
		desc.m_NextEffectPower = _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f));
		desc.m_vScale = _float3(2.f, 2.f, 2.f);
		desc.m_AttackDesc.m_bContinue = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Continue;
		desc.m_AttackDesc.m_CollisionEffectType = m_Skill_Depend_Datas[skillType].m_effects[2];
		desc.m_AttackDesc.m_bKnockBack = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Knockback;
		desc.m_AttackDesc.effectDesc.m_IsParts = true;

		pSkillEffect = pEffect_Manager->Create_Charge_Effect(m_Skill_Depend_Datas[skillType].m_effects[0], pLayerTag, iLevelIndex, desc);
		if (nullptr != pSkillEffect)
			pSkillEffect->Set_ParentNoParts(pBone, pParentTransform, PivotMatrix);

		CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
		if (nullptr == pTransform)
			return E_FAIL;
		pTransform->LookAt(XMVectorSetW(vLook, 1.f));

		_float4 pos = {};
		pos.y = 1.5f;
		XMStoreFloat4(&pos, vPos);

		pSkillEffect->Set_Pos(pos);

		Safe_Release(pSkillEffect);
	}

	else if (skillType == 164) // 돌진
	{
		pSkillEffect = pEffect_Manager->CreateEffect(m_Skill_Depend_Datas[skillType].m_effects[0], L"Prototype_GameObject_AttackEffect", pLayerTag, iLevelIndex);

		CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
		if (nullptr == pTransform)
			return E_FAIL;
		pTransform->LookAt(XMVectorSetW(vLook, 1.f));

		CAttackEffect::ATTACK_EFFECT_DESC desc{};
		Set_NormalAttackDesc(desc, skillType, pSkillEffect, 1);

		_float4 pos = {};
		XMStoreFloat4(&pos, vPos + vLook * 0.25f);
		pSkillEffect->Set_Pos(pos);

		Set_AttackPower(pSkillEffect, _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f)));

		Safe_Release(pSkillEffect);
	}
	else if (skillType == 168) // 메가톤 펀치
	{
		for (size_t i = 1; i <= 2; ++i)
		{
			Create_No_ChargeEffect(m_Skill_Depend_Datas[skillType].m_effects[0], vLook, XMVectorSet(0.f, 0.5f, 0.f, 1.f), pLayerTag, iLevelIndex, pBone, pParentTransform, PivotMatrix);

			pSkillEffect = pEffect_Manager->CreateEffect(m_Skill_Depend_Datas[skillType].m_effects[1], L"Prototype_GameObject_AttackEffect", pLayerTag, iLevelIndex);

			CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
			if (nullptr == pTransform)
				return E_FAIL;
			pTransform->LookAt(XMVectorSetW(vLook, 1.f));

			CAttackEffect::ATTACK_EFFECT_DESC desc{};
			Set_NormalAttackDesc(desc, skillType, pSkillEffect);

			_float4 pos = {};
			XMStoreFloat4(&pos, vPos + (vLook * 0.75f * _float(i)));
			pSkillEffect->Set_Pos(pos);

			pTransform->Set_Scaled({ 2.2f, 2.2f, 2.2f });

			Set_AttackPower(pSkillEffect, _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f)));

			Safe_Release(pSkillEffect);
		}
	}
	else if (skillType == 188) // 돌떨구기
	{
		for (size_t i = 1; i <= 2; ++i)
		{
			Create_No_ChargeEffect(m_Skill_Depend_Datas[skillType].m_effects[0], vLook, XMVectorSet(0.f, 0.f, 0.f, 1.f), pLayerTag, iLevelIndex, pBone, pParentTransform, PivotMatrix);

			pSkillEffect = pEffect_Manager->CreateEffect(m_Skill_Depend_Datas[skillType].m_effects[1], L"Prototype_GameObject_BezierAttackEffect", pLayerTag, iLevelIndex);

			CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
			if (nullptr == pTransform)
				return E_FAIL;
			pTransform->LookAt(XMVectorSetW(vLook, 1.f));
			pTransform->Set_TransforDesc({ XMConvertToRadians(10.f), XMConvertToRadians(350.f) });
			pTransform->Set_Scaled({ 2.f, 2.f, 2.f });

			_float4 pos1 = {};
			XMStoreFloat4(&pos1, vPos + (vLook * 0.25f * (_float)i));
			pSkillEffect->Set_Pos(pos1);

			_float4 pos2 = {};
			XMStoreFloat4(&pos2, vPos + (vLook * 0.75f * (_float)i));

			_float4 pos3 = {};
			XMStoreFloat4(&pos3, vPos + (vLook * 1.5f * (_float)i));

			CAttackEffect::ATTACK_EFFECT_DESC desc{};
			Set_NormalAttackDesc(desc, skillType, pSkillEffect, 3);

			static_cast<CBezierAttackEffect*>(pSkillEffect)->Set_BezierPoints(
				{ pos1.x, pos1.y, pos1.z },
				{ pos2.x, pos2.y + 2.5f, pos2.z },
				{ pos3.x, 0.f, pos3.z }
			);
			pSkillEffect->Init_LoopCount(5);

			Set_AttackPower(pSkillEffect, _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f)));

			Safe_Release(pSkillEffect);
		}
	}
	else if (skillType == 192) // 록커트
	{
		if (nullptr == pBuffState)
			return E_FAIL;

		_uint conditionDataID = m_Skill_Depend_Datas[skillType].m_conditions[0];

		CConditionData::CONDITIONDATA_DESC conditionDataDesc = pConditionData->Get_ConditonData(conditionDataID);
		_uint conditinoTypeID = conditionDataDesc.m_type;

		CConditionData::CONDITIONTYPEDATA_DESC conditionTypeDataDesc = pConditionData->Get_ConditonTypeData(conditinoTypeID);

		Create_No_ChargeEffect(conditionTypeDataDesc.m_effectType, vLook, XMVectorSet(0.f, 0.f, 0.f, 1.f), pLayerTag, iLevelIndex, pBone, pParentTransform, PivotMatrix);
		
		pBuffState->Set_BuffState(skillType, (CBuffState::BUFF_STATE)conditionTypeDataDesc.m_id,
			conditionTypeDataDesc.m_iconPath.c_str(), conditionDataDesc.m_Value_A, conditionDataDesc.m_Value_B, 
			conditionDataDesc.m_time, conditionDataDesc.m_ratio);

	}
	else if (skillType == 116) // 베리어
	{
		if (nullptr == pBuffState)
			return E_FAIL;

		_uint conditionDataID = m_Skill_Depend_Datas[skillType].m_conditions[0];

		CConditionData::CONDITIONDATA_DESC conditionDataDesc = pConditionData->Get_ConditonData(conditionDataID);
		_uint conditinoTypeID = conditionDataDesc.m_type;

		CConditionData::CONDITIONTYPEDATA_DESC conditionTypeDataDesc = pConditionData->Get_ConditonTypeData(conditinoTypeID);

		Create_No_ChargeEffect(conditionTypeDataDesc.m_effectType, vLook, XMVectorSet(0.f, 0.f, 0.f, 1.f), pLayerTag, iLevelIndex, pBone, pParentTransform, PivotMatrix);

		pBuffState->Set_BuffState(skillType, (CBuffState::BUFF_STATE)conditionTypeDataDesc.m_id,
			conditionTypeDataDesc.m_iconPath.c_str(), conditionDataDesc.m_Value_A, conditionDataDesc.m_Value_B,
			conditionDataDesc.m_time, conditionDataDesc.m_ratio);
	}
	else
	{
		Create_No_ChargeEffect(m_Skill_Depend_Datas[skillType].m_effects[1], vLook, XMVectorSet(0.f, 0.5f, 0.f, 1.f), pLayerTag, iLevelIndex, pBone, pParentTransform, PivotMatrix);

		pSkillEffect = pEffect_Manager->CreateEffect(m_Skill_Depend_Datas[skillType].m_effects[0], L"Prototype_GameObject_AttackEffect", pLayerTag, iLevelIndex);

		CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
		if (nullptr == pTransform)
			return E_FAIL;
		pTransform->LookAt(XMVectorSetW(vLook, 1.f));

		CAttackEffect::ATTACK_EFFECT_DESC desc{};
		Set_NormalAttackDesc(desc, skillType, pSkillEffect);

		_float4 pos = {};
		XMStoreFloat4(&pos, vPos + vLook * 0.5f);
		pSkillEffect->Set_Pos(pos);

		Set_AttackPower(pSkillEffect, _uint(damage * skill_desc.m_damagePercent * ((rand() % 10 + 95) * 0.01f)));

		Safe_Release(pSkillEffect);
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

CSkill* CSkill_Manager::Do_Skill(const _tchar* pLayerTag, _uint iLevelIndex, _uint skillType, _uint damage,
	_fmatrix vParentMatrix, CModel* pModel, const char* boneTag, CTransform* pParentTransform, CBuffState* pBuffState)
{
	if (nullptr == pLayerTag || nullptr == pModel || nullptr == pParentTransform)
		return nullptr;

	CSkill* pSkill = nullptr;
	if (skillType <= 35 && skillType % 2 == 1) // 원거리 공격
	{
		CreateSkill(pLayerTag, iLevelIndex, skillType, damage, 
			vParentMatrix, pModel->Get_BonePtr(boneTag), pParentTransform, pModel->Get_PivotMatrix(), pBuffState);
	}
	else if (skillType == 50) // 하이드럼 펌프
	{
		CreateSkill(pLayerTag, iLevelIndex, skillType, damage,
			vParentMatrix, pModel->Get_BonePtr(boneTag), pParentTransform, pModel->Get_PivotMatrix(), pBuffState);
	}
	else if (skillType == 57) // 10만 볼트
	{
		CreateSkill(pLayerTag, iLevelIndex, skillType, damage,
			vParentMatrix, pModel->Get_BonePtr(boneTag), pParentTransform, pModel->Get_PivotMatrix(), pBuffState);
	}
	else if (skillType == 58) // 볼테커
	{
		CreateSkill(pLayerTag, iLevelIndex, skillType, damage,
			vParentMatrix, pModel->Get_BonePtr(boneTag), pParentTransform, pModel->Get_PivotMatrix(), pBuffState);
	}
	else if (skillType == 72) // 냉동빔
	{
		CreateSkill(pLayerTag, iLevelIndex, skillType, damage,
			vParentMatrix, pModel->Get_BonePtr(boneTag), pParentTransform, pModel->Get_PivotMatrix(), pBuffState);
	}
	else if (skillType == 79) // 얼다 바람
	{
		CreateSkill(pLayerTag, iLevelIndex, skillType, damage,
			vParentMatrix, pModel->Get_BonePtr(boneTag), pParentTransform, pModel->Get_PivotMatrix(), pBuffState);
	}
	else if (skillType == 100) // 지진
	{
		CreateSkill(pLayerTag, iLevelIndex, skillType, damage,
			vParentMatrix, pModel->Get_BonePtr(boneTag), pParentTransform, pModel->Get_PivotMatrix(), pBuffState);
	}
	else if (skillType == 164) // 돌진
	{
		CreateSkill(pLayerTag, iLevelIndex, skillType, damage,
			vParentMatrix, pModel->Get_BonePtr(boneTag), pParentTransform, pModel->Get_PivotMatrix(), pBuffState);
	}
	else if (skillType == 168) // 메가톤 펀치
	{
		CreateSkill(pLayerTag, iLevelIndex, skillType, damage,
			vParentMatrix, pModel->Get_BonePtr(boneTag), pParentTransform, pModel->Get_PivotMatrix(), pBuffState);
	}
	else if (skillType == 188) // 돌떨구기
	{
		CreateSkill(pLayerTag, iLevelIndex, skillType, damage,
			vParentMatrix, pModel->Get_BonePtr(boneTag), pParentTransform, pModel->Get_PivotMatrix(), pBuffState);
	}
	else if (skillType == 192) // 록커트
	{
		// 컨디션 3
		// 컨디션 데이터 가져오고 플레이어의 버프 자리 남는 자리에 텍스쳐 생성시키고 
		// 해당 데이터에 맞는 값 증가시켜주기

		CreateSkill(pLayerTag, iLevelIndex, skillType, damage,
			vParentMatrix, pModel->Get_BonePtr(boneTag), pParentTransform, pModel->Get_PivotMatrix(), pBuffState);

	}
	else if (skillType == 116) // 베리어
	{
		// 컨디션 2

		CreateSkill(pLayerTag, iLevelIndex, skillType, damage,
			vParentMatrix, pModel->Get_BonePtr(boneTag), pParentTransform, pModel->Get_PivotMatrix(), pBuffState);
	}

	else
	{
		pSkill = Create_Skill(pLayerTag, iLevelIndex, skillType, damage,
			vParentMatrix);

		Safe_Release(pSkill);
	}

	return pSkill;
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

	m_Skill_Depend_Datas.reserve((size_t)m_datas.Size());

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

		m_Skill_Desc_Datas.reserve((size_t)m_datas.Size());

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

void CSkill_Manager::Create_No_ChargeEffect(_uint skillType, _vector vLook, _vector vPos, const _tchar* pLayerTag, _uint iLevelIndex,
	CBone* pBone, CTransform* pParentTransform, _fmatrix PivotMatrix)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CEffect_Manager* pEffect_Manager = dynamic_cast<CEffect_Manager*>(pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Effect_Manager"));

	if (nullptr == pEffect_Manager)
		return;

	CSkillEffect* pSkillEffect = nullptr;

	pSkillEffect = pEffect_Manager->CreateEffect(skillType, L"Prototype_GameObject_SkillEffect", pLayerTag, iLevelIndex);
	if (nullptr != pSkillEffect)
	{
		pSkillEffect->Set_Parent(pBone, pParentTransform, PivotMatrix);
		//pSkillEffect->Set_ParentRotateApply(true);
	}

	CTransform* pTransform = pSkillEffect->Get_As<CTransform>();
	if (nullptr == pTransform)
		return;
	pTransform->LookAt(XMVectorSetW(vLook, 1.f));

	_float4 pos = {};
	XMStoreFloat4(&pos, vPos);
	pSkillEffect->Set_Pos(pos);

	Safe_Release(pSkillEffect);
}

void CSkill_Manager::Set_NormalAttackDesc(CAttackEffect::ATTACK_EFFECT_DESC& desc, const _uint& skillType, 
	CSkillEffect* pSkillEffect, _uint collisionEffectType)
{
	desc.m_bContinue = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Continue;
	desc.m_CollisionEffectType = m_Skill_Depend_Datas[skillType].m_effects[collisionEffectType];
	desc.m_bKnockBack = m_Skill_Desc_Datas[skillType].m_isEnablePotential_Knockback;

	static_cast<CAttackEffect*>(pSkillEffect)->Set_AttackDesc(desc);
}

void CSkill_Manager::Set_AttackPower(CSkillEffect* pSkillEffect, const _uint& damage)
{
	CAttack* pAttack = pSkillEffect->Get_As<CAttack>();
	if (nullptr != pAttack)
	{
		pAttack->Set_AttackPower(damage);
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