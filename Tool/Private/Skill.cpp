#include "stdafx.h"
#include "Skill.h"

#include "GameInstance.h"

#include "Effect.h"

CSkill::CSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CSkill::CSkill(const CSkill& rhs)
	: CGameObject(rhs)
{

}

HRESULT CSkill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (pArg)
	{
		m_SkillDesc.m_chargeSecond = (*(SKILL_DESC*)(pArg)).m_chargeSecond;
		m_SkillDesc.m_damagePercent = (*(SKILL_DESC*)(pArg)).m_damagePercent;
		m_SkillDesc.m_iconPath= (*(SKILL_DESC*)(pArg)).m_iconPath;
		m_SkillDesc.m_isEnablePotential_BuffProbUp = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_BuffProbUp;
		m_SkillDesc.m_isEnablePotential_BuffShare = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_BuffShare;
		m_SkillDesc.m_isEnablePotential_BuffTimeUp = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_BuffTimeUp;
		m_SkillDesc.m_isEnablePotential_Charge = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_Charge;
		m_SkillDesc.m_isEnablePotential_ConditionDuration = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_ConditionDuration;
		m_SkillDesc.m_isEnablePotential_Continue = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_Continue;
		m_SkillDesc.m_isEnablePotential_Damage = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_Damage;
		m_SkillDesc.m_isEnablePotential_DebuffProbUp = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_DebuffProbUp;
		m_SkillDesc.m_isEnablePotential_Distance = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_Distance;
		m_SkillDesc.m_isEnablePotential_Extend = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_Extend;
		m_SkillDesc.m_isEnablePotential_Homing = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_Homing;
		m_SkillDesc.m_isEnablePotential_Knockback = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_Knockback;
		m_SkillDesc.m_isEnablePotential_Nway = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_Nway;
		m_SkillDesc.m_isEnablePotential_Rapid = (*(SKILL_DESC*)(pArg)).m_isEnablePotential_Rapid;
		m_SkillDesc.m_rapidDelay = (*(SKILL_DESC*)(pArg)).m_rapidDelay;
		m_SkillDesc.m_skillPath = (*(SKILL_DESC*)(pArg)).m_skillPath;
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}


_uint CSkill::Tick(_double TimeDelta)
{
	if (m_bDead)
	{
		for (auto& iter : m_effects)
		{
			if (nullptr != iter)
			{
				iter->Set_Dead();
			}
		}

		for (auto& iter : m_conditions)
		{
			if (nullptr != iter)
			{
				iter->Set_Dead();
			}
		}

		return OBJ_DEAD;
	}

	//for (auto& iter : m_effects)
	//{
	//	if (nullptr != iter)
	//	{
	//	}
	//}

	//for (auto& iter : m_conditions)
	//{
	//	if (nullptr != iter)
	//	{
	//		iter->Tick(TimeDelta);
	//	}
	//}

	if (m_CurrentTestDeadDuration >= m_TestDeadDuration)
	{
		m_bDead = true;
	}
	m_CurrentTestDeadDuration += TimeDelta;

	return _uint();
}

_uint CSkill::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);
	return _uint();
}

HRESULT CSkill::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	return S_OK;
}


void CSkill::Set_Effects_Pos(const _float4& vPos)
{
	for (auto& effect : m_effects)
	{
		if (nullptr != effect)
		{
			effect->Set_Pos(vPos);
		}
	}
}

void CSkill::Set_Conditions_Pos(const _float4& vPos)
{
	for (auto& condition : m_conditions)
	{
		if (nullptr != condition)
		{
			CTransform* pTransform = condition->Get_As<CTransform>();
			pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
		}
	}
}

void CSkill::Set_Effects_Look(const _float4& vLook)
{
	for (auto& effect : m_effects)
	{
		if (nullptr != effect)
		{
			CTransform* pTransform = effect->Get_As<CTransform>();
			pTransform->LookAt(XMLoadFloat4(&vLook));
		}
	}
}

void CSkill::Set_Conditions_Look(const _float4& vLook)
{
	for (auto& condition : m_conditions)
	{
		if (nullptr != condition)
		{
			CTransform* pTransform = condition->Get_As<CTransform>();
			pTransform->LookAt(XMLoadFloat4(&vLook));
		}
	}
}

HRESULT CSkill::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;


	return S_OK;
}


HRESULT CSkill::SetUp_ShaderResources()
{

	return S_OK;
}

CSkill* CSkill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkill* pInstance = new CSkill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CSkill* pInstance = new CSkill(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CSkill::Free()
{
	__super::Free();
	

	for (auto& effect : m_effects)
		Safe_Release(effect);

	for (auto& condition : m_conditions)
		Safe_Release(condition);


	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
}
