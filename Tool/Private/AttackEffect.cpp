#include "AttackEffect.h"

#include "Skill_Manager.h";

#include "GameInstance.h"

CAttackEffect::CAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillEffect(pDevice, pContext)
{
}

CAttackEffect::CAttackEffect(const CAttackEffect& rhs)
	: CSkillEffect(rhs)
{
}

HRESULT CAttackEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAttackEffect::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_AttackEffectDesc.m_bKnockBack = (*(ATTACK_EFFECT_DESC*)(pArg)).m_bKnockBack;
		m_AttackEffectDesc.m_AttackTime = (*(ATTACK_EFFECT_DESC*)(pArg)).m_AttackTime;
		m_AttackEffectDesc.m_bContinue = (*(ATTACK_EFFECT_DESC*)(pArg)).m_bContinue;
		m_AttackEffectDesc.m_CollisionEffectType = (*(ATTACK_EFFECT_DESC*)(pArg)).m_CollisionEffectType;

		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &(*(ATTACK_EFFECT_DESC*)(pArg)).effectDesc)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
			return E_FAIL;
	}

	return S_OK;
}

_uint CAttackEffect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	Attack_Time_Check(TimeDelta);

	return _uint();
}

_uint CAttackEffect::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_EffectDesc.pBonePtr && nullptr != m_EffectDesc.pParent)
	{
		if (m_pColliderCom)
			m_pColliderCom->Tick(XMLoadFloat4x4(&m_EffectDesc.m_FinalWorldMatrix));
	}
	else
	{
		if (m_pColliderCom)
			m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());
	}

#ifdef _DEBUG
	if (m_pColliderCom)
		m_pColliderCom->Render();

	m_pNavigationCom->Render();

#endif // _DEBUG

	return _uint();
}

void CAttackEffect::On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	Collision(pOther, fX, fY, fZ);
}

void CAttackEffect::On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	Collision(pOther, fX, fY, fZ);
}


void CAttackEffect::On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
}

HRESULT CAttackEffect::Add_Components()
{
	__super::Add_Components();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CCollider::COLLIDER_DESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(2.2f, 2.0f, 2.2f);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		(CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	CAttack::ATTACK_DESC attackDesc = {};
	attackDesc.m_AttackPower = 0;

	if (FAILED(pGameInstance->Add_Component(CAttack::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Attack"),
		(CComponent**)&m_pAttackCom, &attackDesc)))
		return E_FAIL;

	return S_OK;
}

void CAttackEffect::Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	if (m_pColliderCom)
	{
		_vector vDestCenter = m_pColliderCom->Get_Center();
		_vector vSourCenter = pOther->Get_Center();
		CGameObject* pOtherOwner = pOther->Get_Owner();
		if (!pOtherOwner)
			return;

		if (m_pAttackCom)
		{
			CHP* pOtherHpCom = pOtherOwner->Get_As<CHP>();

			if (nullptr != pOtherHpCom)
			{
				if (m_AttackTimeAcc <= 0.0)
				{
					pOtherHpCom->Get_Damage(m_pAttackCom->Get_AttackPower());

					if (false == m_AttackEffectDesc.m_bContinue)
					{
						Set_Dead();
					}

					// TODO:: 콜리전 이펙트 생성

					m_AttackTimeAcc = m_AttackEffectDesc.m_AttackTime;
				}
			}
		}

		if (m_AttackEffectDesc.m_bKnockBack)
		{
			CTransform* pOtherTransform = pOtherOwner->Get_As<CTransform>();
			if (!pOtherTransform)
				return;

			CNavigation* pNavigationCom = pOtherOwner->Get_As<CNavigation>();

			if (fX > fZ)
			{

				if (XMVectorGetZ(vDestCenter) < XMVectorGetZ(vSourCenter))
				{
					pOtherTransform->Move(0.f, 0.f, fZ, pNavigationCom);
				}
				else
				{
					pOtherTransform->Move(0.f, 0.f, -fZ, pNavigationCom);
				}

			}
			else if (fX == fZ) {}
			else
			{
				if (XMVectorGetX(vDestCenter) < XMVectorGetX(vSourCenter))
				{
					pOtherTransform->Move(fX, 0.f, 0.f, pNavigationCom);

				}
				else
				{
					pOtherTransform->Move(-fX, 0.f, 0.f, pNavigationCom);
				}
			}
		}
	}
}

void CAttackEffect::Attack_Time_Check(const _double& TimeDelta)
{
	if (m_AttackTimeAcc > 0.0)
		m_AttackTimeAcc -= TimeDelta;
}

CAttackEffect* CAttackEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAttackEffect* pInstance = new CAttackEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAttackEffect::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CAttackEffect* pInstance = new CAttackEffect(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CAttackEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAttackEffect::Free()
{
	__super::Free();

	Safe_Release(m_pAttackCom);
	Safe_Release(m_pColliderCom);

}