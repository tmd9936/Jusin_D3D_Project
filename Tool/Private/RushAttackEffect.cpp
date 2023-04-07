#include "RushAttackEffect.h"

#include "Skill_Manager.h"

#include "GameInstance.h"

CRushAttackEffect::CRushAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAttackEffect(pDevice, pContext)
{
}

CRushAttackEffect::CRushAttackEffect(const CRushAttackEffect& rhs)
	: CAttackEffect(rhs)
	, m_RushAttackEffectDesc(rhs.m_RushAttackEffectDesc)
{
}

HRESULT CRushAttackEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRushAttackEffect::Initialize_Prototype(RUSH_ATTACK_EFFECT_DESC& desc)
{
	m_RushAttackEffectDesc = desc;

	m_AttackEffectDesc = m_RushAttackEffectDesc.attackEffectDesc;

	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRushAttackEffect::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
			return E_FAIL;
	}

	m_EffectDesc.m_bParentRotateApply = m_RushAttackEffectDesc.attackEffectDesc.effectDesc.m_bParentRotateApply;
	m_EffectDesc.m_CurrentLoopCount = m_RushAttackEffectDesc.attackEffectDesc.effectDesc.m_CurrentLoopCount;

	m_EffectDesc.m_AnimationSpeed = m_RushAttackEffectDesc.attackEffectDesc.effectDesc.m_AnimationSpeed;
	m_EffectDesc.m_AnimationStartAcc = m_RushAttackEffectDesc.attackEffectDesc.effectDesc.m_AnimationStartAcc;

	return S_OK;
}

_uint CRushAttackEffect::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_EffectDesc.m_CurrentLoopCount < 0)
		return OBJ_DEAD;

	Rush(TimeDelta);

	Loop_Count_Check(TimeDelta);

	Attack_Time_Check(TimeDelta);

	return _uint();
}

_uint CRushAttackEffect::LateTick(_double TimeDelta)
{
	return __super::LateTick(TimeDelta);
}

void CRushAttackEffect::Rush(const _double& TimeDelta)
{
	m_pTransformCom->Go_Straight((_float)(TimeDelta * m_RushAttackEffectDesc.m_RushSpeed));
}

CRushAttackEffect* CRushAttackEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRushAttackEffect* pInstance = new CRushAttackEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CRushAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CRushAttackEffect* CRushAttackEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, RUSH_ATTACK_EFFECT_DESC& desc)
{
	CRushAttackEffect* pInstance = new CRushAttackEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(desc)))
	{
		MSG_BOX("Failed to Created CRushAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRushAttackEffect::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CRushAttackEffect* pInstance = new CRushAttackEffect(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CRushAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRushAttackEffect::Free()
{
	__super::Free();

}
