#include "Formation.h"
#include "GameObject.h"

CFormation::CFormation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
{
}

CFormation::CFormation(const CFormation& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
{
}


HRESULT CFormation::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFormation::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof m_Desc);
	}

	return S_OK;
}

void CFormation::Swap_RelativePos(CFormation* pTarget)
{
	_vector tempPos = m_Desc.m_RelativePos;
	m_Desc.m_RelativePos = pTarget->Get_RelativePos();
	pTarget->Set_RelativePos(tempPos);
}

CFormation* CFormation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFormation* pInstance = new CFormation(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFormation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CFormation::Clone(CGameObject* pOwner, void* pArg)
{
	CFormation* pInstance = new CFormation(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFormation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFormation::Free()
{
	__super::Free();
}
