#include "HP.h"
#include "GameObject.h"

CHP::CHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CHP::CHP(const CHP& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
{
}


HRESULT CHP::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHP::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof m_Desc);
	}
	else
	{
		m_Desc.m_MaxHp = 100;
		m_Desc.m_HpChangeTick = 1;
		m_Desc.bDeadAfterOwnerDead = false;
	}
	m_MaxHP = m_Desc.m_MaxHp;
	m_CurrentHP = m_Desc.m_MaxHp;

	return S_OK;
}

_uint CHP::Tick(const _double& Timedelta)
{
	m_GetDamageEvent = false;
	m_DamageReceived = 0;

	return _uint();
}


void CHP::Get_Damage(_int damage)
{
	m_CurrentHP -= max(5, _int(damage * m_DamageGetPercent));

	m_GetDamageEvent = true;

	m_DamageReceived = damage;

	if (m_CurrentHP <= 0)
	{
		if (m_Desc.bDeadAfterOwnerDead)
		{
			if (m_pOwner)
				m_pOwner->Set_Dead();
		}
		m_CurrentHP = 0;
	}
}

void CHP::Set_DamageGetPercent(_float percent)
{
	if (percent < 0.2f || percent > 1.8f)
		return;

	m_DamageGetPercent = percent;
}

_float CHP::Add_DamageGetPercent(_float percent)
{
	_float returnValue = percent;
	_float preDamageGetPercent = m_DamageGetPercent;
	m_DamageGetPercent += percent;

	if (m_DamageGetPercent < 0.2f)
	{
		m_DamageGetPercent = 0.2f;
		returnValue = preDamageGetPercent - m_DamageGetPercent;
	}

	if (m_DamageGetPercent > 1.8f)
	{
		m_DamageGetPercent = 1.8f;
		returnValue = m_DamageGetPercent - preDamageGetPercent;

	}

	return returnValue;
}

CHP* CHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHP* pInstance = new CHP(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CHP::Clone(CGameObject* pOwner, void* pArg)
{
	CHP* pInstance = new CHP(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHP::Free()
{
	__super::Free();
}
