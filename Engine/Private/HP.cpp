#include "HP.h"
#include "GameObject.h"
#include "PipeLine.h"

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
		memcpy(&m_Desc, pArg, sizeof m_Desc);
	else
	{
		m_Desc.m_MaxHp = 100;
		m_Desc.m_HpChangeTick = 1;
	}
	m_CurrentHP = m_Desc.m_MaxHp;

	return S_OK;
}

_uint CHP::Tick()
{
	if (m_TargetHP == m_CurrentHP)
		return 0;

	if (m_CurrentHP < m_TargetHP)
	{
		m_CurrentHP += m_Desc.m_HpChangeTick;
	}
	else
	{
		m_CurrentHP -= m_Desc.m_HpChangeTick;
	}

	return _uint();
}

_bool CHP::Set_TargetHP(_uint targetHP)
{
	if (m_MaxHP < targetHP || 0 > targetHP)
		return false;

	m_TargetHP = targetHP;

	return true;
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
