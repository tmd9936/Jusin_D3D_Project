#include "RushAttackEffect.h"

#include "Skill_Manager.h";

#include "GameInstance.h"

CRushAttackEffect::CRushAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CAttackEffect(pDevice, pContext)
{
}

CRushAttackEffect::CRushAttackEffect(const CRushAttackEffect& rhs)
	: CAttackEffect(rhs)
{
}

HRESULT CRushAttackEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRushAttackEffect::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_RushAttackEffectDesc.m_vRushDirection = (*(RUSH_ATTACK_EFFECT_DESC*)(pArg)).m_vRushDirection;
		m_RushAttackEffectDesc.m_RushSpeed = (*(RUSH_ATTACK_EFFECT_DESC*)(pArg)).m_RushSpeed;

		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &(*(RUSH_ATTACK_EFFECT_DESC*)(pArg)).attackEffectDesc)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
			return E_FAIL;
	}

	return S_OK;
}

_uint CRushAttackEffect::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	return _uint();
}

_uint CRushAttackEffect::LateTick(_double TimeDelta)
{
	return _uint();
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

	Safe_Release(m_pAttackCom);
	Safe_Release(m_pColliderCom);

}
