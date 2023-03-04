#include "Collider.h"

_uint CCollider::g_iColliderID = 0;

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext)
	, m_iID(g_iColliderID++)
{
	m_vMin = { -1.f, -1.f, -1.f };
	m_vMax = { 1.f, 1.f, 1.f };
}

CCollider::CCollider(const CCollider& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
	, m_pOwner(pOwner)
	, m_iID(g_iColliderID++)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwFVF(rhs.m_dwFVF)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_IdxFmt(rhs.m_IdxFmt)
	, m_bCustomScale(rhs.m_bCustomScale)
{
	m_vMin = rhs.m_vMin;
	m_vMax = rhs.m_vMax;

	m_pVB->AddRef();
	m_pIB->AddRef();

	for (_int i = 0; i < 8; ++i)
	{
		m_vVtx[i] = rhs.m_vVtx[i];
		m_vOriginVtx[i] = rhs.m_vOriginVtx[i];
	}

	m_vCenter = rhs.m_vCenter;
}

CCollider::~CCollider()
{
}

HRESULT CCollider::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CCollider::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CCollider::On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
}

void CCollider::On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
}

void CCollider::On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
}

HRESULT CCollider::Ready_Collider()
{
	return E_NOTIMPL;
}

_int CCollider::Update_Component(const _float& fTimeDelta)
{
	return _int();
}

void CCollider::LateUpdate_Component()
{
}

void CCollider::Render_Collider()
{
}

HRESULT CCollider::Ready_Buffer(void)
{
	return E_NOTIMPL;
}

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCollider* pInstance = new CCollider(pDevice, pContext, nullptr);
	
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider::Clone(CGameObject * pOwner, void* pArg)
{
	CCollider* pInstance = new CCollider(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	__super::Free();
}
