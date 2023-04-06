#include "Attack.h"
#include "GameObject.h"

CAttack::CAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CAttack::CAttack(const CAttack& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
{
}


HRESULT CAttack::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAttack::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof m_Desc);
	}
	else
	{
		m_Desc.m_AttackPower = 0;
	}

	return S_OK;
}

CAttack* CAttack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAttack* pInstance = new CAttack(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CAttack::Clone(CGameObject* pOwner, void* pArg)
{
	CAttack* pInstance = new CAttack(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAttack::Free()
{
	__super::Free();
}
