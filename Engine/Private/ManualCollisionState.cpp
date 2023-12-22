#include "ManualCollisionState.h"
#include "GameObject.h"

CManualCollisionState::CManualCollisionState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CManualCollisionState::CManualCollisionState(const CManualCollisionState& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
{
}


HRESULT CManualCollisionState::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CManualCollisionState::Initialize(void* pArg)
{

	return S_OK;
}

_uint CManualCollisionState::Tick(const _double& Timedelta)
{
	m_eState = COLLISION_STATE_NONE;

	return _uint();
}

CManualCollisionState* CManualCollisionState::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CManualCollisionState* pInstance = new CManualCollisionState(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CManualCollisionState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CManualCollisionState::Clone(CGameObject* pOwner, void* pArg)
{
	CManualCollisionState* pInstance = new CManualCollisionState(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CManualCollisionState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CManualCollisionState::Free()
{
	__super::Free();
}
