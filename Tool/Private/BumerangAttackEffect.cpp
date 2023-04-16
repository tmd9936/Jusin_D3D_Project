#include "BumerangAttackEffect.h"

#include "Skill_Manager.h"

#include "GameInstance.h"

CBumerangAttackEffect::CBumerangAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAttackEffect(pDevice, pContext)
{
}

CBumerangAttackEffect::CBumerangAttackEffect(const CBumerangAttackEffect& rhs)
	: CAttackEffect(rhs)
	, m_BumerangAttackEffectDesc(rhs.m_BumerangAttackEffectDesc)
{
}

HRESULT CBumerangAttackEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBumerangAttackEffect::Initialize_Prototype(BUMERANG_ATTACK_EFFECT_DESC& desc)
{
	m_BumerangAttackEffectDesc = desc;

	m_AttackEffectDesc = m_BumerangAttackEffectDesc.attackEffectDesc;

	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBumerangAttackEffect::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
			return E_FAIL;
	}

	m_EffectDesc.m_bParentRotateApply = m_BumerangAttackEffectDesc.attackEffectDesc.effectDesc.m_bParentRotateApply;
	m_EffectDesc.m_CurrentLoopCount = m_BumerangAttackEffectDesc.attackEffectDesc.effectDesc.m_CurrentLoopCount;

	m_EffectDesc.m_AnimationSpeed = m_BumerangAttackEffectDesc.attackEffectDesc.effectDesc.m_AnimationSpeed;
	m_EffectDesc.m_AnimationStartAcc = m_BumerangAttackEffectDesc.attackEffectDesc.effectDesc.m_AnimationStartAcc;

	return S_OK;
}

_uint CBumerangAttackEffect::Tick(_double TimeDelta)
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

_uint CBumerangAttackEffect::LateTick(_double TimeDelta)
{
	return __super::LateTick(TimeDelta);
}

void CBumerangAttackEffect::Rush(const _double& TimeDelta)
{
	if (m_eDirection == DIRECTION_FRONT)
		m_pTransformCom->Go_Straight((_float)(TimeDelta * m_BumerangAttackEffectDesc.m_RushSpeed));
	else
		m_pTransformCom->Go_Backward((_float)(TimeDelta * m_BumerangAttackEffectDesc.m_RushSpeed));
}

CBumerangAttackEffect* CBumerangAttackEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBumerangAttackEffect* pInstance = new CBumerangAttackEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CBumerangAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBumerangAttackEffect* CBumerangAttackEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BUMERANG_ATTACK_EFFECT_DESC& desc)
{
	CBumerangAttackEffect* pInstance = new CBumerangAttackEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(desc)))
	{
		MSG_BOX("Failed to Created CBumerangAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBumerangAttackEffect::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CBumerangAttackEffect* pInstance = new CBumerangAttackEffect(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CBumerangAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBumerangAttackEffect::Free()
{
	__super::Free();

}
