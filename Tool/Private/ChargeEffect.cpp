#include "ChargeEffect.h"

#include "Skill_Manager.h";

#include "GameInstance.h"

CChargeEffect::CChargeEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillEffect(pDevice, pContext)
{
}

CChargeEffect::CChargeEffect(const CChargeEffect& rhs)
	: CSkillEffect(rhs)
{
}

HRESULT CChargeEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CChargeEffect::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_ChargeEffectDesc.m_NextAttackEffectType = (*(Charge_Effect_Desc*)(pArg)).m_NextAttackEffectType;
		m_ChargeEffectDesc.m_NextAttackEffectPower = (*(Charge_Effect_Desc*)(pArg)).m_NextAttackEffectPower;
		m_ChargeEffectDesc.m_ChargeTime = (*(Charge_Effect_Desc*)(pArg)).m_ChargeTime;

		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &(*(Charge_Effect_Desc*)(pArg)).effectDesc)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
			return E_FAIL;
	}

	return S_OK;
}

_uint CChargeEffect::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	return _uint();
}

_uint CChargeEffect::LateTick(_double TimeDelta)
{
	return _uint();
}

CChargeEffect* CChargeEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CChargeEffect* pInstance = new CChargeEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CChargeEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChargeEffect::Charge_Time_Check(const _double& TimeDelta)
{
	if (m_ChargeTimeAcc < m_ChargeEffectDesc.m_ChargeTime)
	{
		m_ChargeTimeAcc += TimeDelta;
	}
	else
	{
		CSkill_Manager* pSkill_Mananger = dynamic_cast<CSkill_Manager*>(CGameInstance::GetInstance()->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Skill_Manager"));
		if (nullptr != pSkill_Mananger)
		{
			/*pSkill_Mananger->Do_Skill(Get_LayerTag().c_str(), m_iLevelindex, m_ChargeEffectDesc.nextAttackEffect,
				m_ChargeEffectDesc.nextAttackEffectPower, m_pTransformCom->Get_WorldMatrix_Matrix(), m_pModelCom, "RootNode", m_pTransformCom);*/
		}

		Set_Dead();
	}
}

CGameObject* CChargeEffect::Clone(const _tchar * pLayerTag, _uint iLevelIndex, void* pArg)
{
	CChargeEffect* pInstance = new CChargeEffect(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CChargeEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChargeEffect::Free()
{
	__super::Free();
}