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
	if (m_bDead)
		return OBJ_DEAD;

	return _uint();
}

_uint CAttackEffect::LateTick(_double TimeDelta)
{
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