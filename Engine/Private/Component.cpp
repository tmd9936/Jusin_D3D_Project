#include "..\Public\Component.h"
#include "GameObject.h"


CComponent::CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_bClone(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	if (pOwner)
	{
		m_pOwner = pOwner;
		//Safe_AddRef(m_pOwner);
	}
}

CComponent::CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_bClone(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CComponent::CComponent(const CComponent& rhs, CGameObject* pOwner)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
	, m_bClone(true)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	if (pOwner)
	{
		m_pOwner = pOwner;
		//Safe_AddRef(m_pOwner);
	}
}

CComponent::CComponent(const CComponent& rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
	, m_bClone(true)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CComponent::Initialize(void* pArg)
{
	return S_OK;
}

void CComponent::Free()
{
	__super::Free();
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	//Safe_Release(m_pOwner);
}
