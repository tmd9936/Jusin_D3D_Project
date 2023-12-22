#include "stdafx.h"
#include "Searcher.h"

#include "GameInstance.h"


CSearcher::CSearcher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CSearcher::CSearcher(const CSearcher& rhs)
	: CGameObject(rhs)
{

}

HRESULT CSearcher::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSearcher::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_Desc.pParentTransformCom = (*(SEARCHER_DESC*)(pArg)).pParentTransformCom;
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}


_uint CSearcher::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_eCollisionState = COLLISION_STATE_NO;

	if (m_Desc.pParentTransformCom)
		m_pSphereCom->Tick(m_Desc.pParentTransformCom->Get_WorldMatrix_Matrix());

	return _uint();
}

_uint CSearcher::LateTick(_double TimeDelta)
{

	return _uint();
}


void CSearcher::On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	if (nullptr != pOther)
	{
		m_pTarget = pOther->Get_Owner();
		m_eCollisionState = COLLISION_STATE_ON;
	}
}

void CSearcher::On_CollisionEnter(CCollider* pOther,  const _float& fX, const _float& fY, const _float& fZ)
{
	if (nullptr != pOther)
	{
		m_pTarget = pOther->Get_Owner();
		m_eCollisionState = COLLISION_STATE_ENTER;
	}
}

void CSearcher::On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
		m_eCollisionState = COLLISION_STATE_EXIT;
}


HRESULT CSearcher::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_AABB*/
	CCollider::COLLIDER_DESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(7.f, 7.f, 7.f);
	ColliderDesc.vPosition = _float3(0.0f, 1.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		(CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSearcher::SetUp_ShaderResources()
{

	return S_OK;
}

CSearcher* CSearcher::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSearcher* pInstance = new CSearcher(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CSearcher");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSearcher::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CSearcher* pInstance = new CSearcher(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CSearcher");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CSearcher::Free()
{
	__super::Free();

	Safe_Release(m_Desc.pParentTransformCom);
	Safe_Release(m_pSphereCom);
}
