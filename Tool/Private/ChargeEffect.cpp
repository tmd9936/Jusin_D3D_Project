#include "ChargeEffect.h"

CChargeEffect::CChargeEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect(pDevice, pContext)
{
}

CChargeEffect::CChargeEffect(const CChargeEffect& rhs)
	: CEffect(rhs)
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
		m_ChargeEffectDesc.chargeTime = (*(Charge_Effect_Desc*)(pArg)).chargeTime;
		m_ChargeEffectDesc.nextAttackEffect = (*(Charge_Effect_Desc*)(pArg)).nextAttackEffect;

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
	if (m_ChargeTImeAcc > 0.0)
	{
		m_ChargeTImeAcc -= TimeDelta;
	}
	else
	{

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