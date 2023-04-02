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


void CHP::Get_Damage(_uint damage)
{
	m_CurrentHP -= damage;

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
