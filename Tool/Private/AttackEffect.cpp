#include "AttackEffect.h"

#include "Skill_Manager.h"
#include "Effect_Manager.h"

#include "StageCamera.h"
#include "GameInstance.h"

#include "BuffState.h"
#include "ConditionData.h"

#include "Monster.h"

#define	NO_DEBUFF_CONDITION 0

CAttackEffect::CAttackEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillEffect(pDevice, pContext)
{
}

CAttackEffect::CAttackEffect(const CAttackEffect& rhs)
	: CSkillEffect(rhs)
	, m_AttackEffectDesc(rhs.m_AttackEffectDesc)
{
}

HRESULT CAttackEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAttackEffect::Initialize_Prototype(ATTACK_EFFECT_DESC& attackEffectDesc)
{
	m_AttackEffectDesc = attackEffectDesc;

	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAttackEffect::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
			return E_FAIL;

	}
	m_EffectDesc.m_bParentRotateApply = m_AttackEffectDesc.effectDesc.m_bParentRotateApply;
	m_EffectDesc.m_CurrentLoopCount = m_AttackEffectDesc.effectDesc.m_CurrentLoopCount;

	m_EffectDesc.m_AnimationSpeed = m_AttackEffectDesc.effectDesc.m_AnimationSpeed;
	m_EffectDesc.m_AnimationStartAcc = m_AttackEffectDesc.effectDesc.m_AnimationStartAcc;

	return S_OK;
}

_uint CAttackEffect::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_EffectDesc.m_CurrentLoopCount < 0)
		return OBJ_DEAD;

	Loop_Count_Check(TimeDelta);

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
	Collision(pOther, fX, fY, fZ, true);
}

void CAttackEffect::On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	Collision(pOther, fX, fY, fZ, false);
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

void CAttackEffect::Collision(CCollider* pOther, 
	const _float& fX, const _float& fY, const _float& fZ, _bool collisionEffect)
{
	if (m_pColliderCom)
	{
		_vector vDestCenter = m_pColliderCom->Get_Center();
		_vector vSourCenter = pOther->Get_Center();
		CGameObject* pOtherOwner = pOther->Get_Owner();
		if (!pOtherOwner)
			return;

		CTransform* pOtherTransform = pOtherOwner->Get_As<CTransform>();
		if (!pOtherTransform)
			return;

		if (m_pAttackCom)
		{
			CHP* pOtherHpCom = pOtherOwner->Get_As<CHP>();

			if (nullptr != pOtherHpCom)
			{
				if (pOtherHpCom->Get_CurrentHp() <= 0)
					return;

				if (m_AttackTimeAcc <= 0.0)
				{
					pOtherHpCom->Get_Damage(m_pAttackCom->Get_AttackPower());

					if (collisionEffect)
					{
						Camera_Shake_Request();
						Set_ManualCollisionState(pOtherOwner, CManualCollisionState::COLLISION_STATE::COLLISION_STATE_ENTER);
					}
					Create_Collision_Effect(pOtherTransform);
					Do_DebuffCondition(pOtherOwner);

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

void CAttackEffect::Create_Collision_Effect(CTransform* hitObjectTransform)
{
	if (nullptr == hitObjectTransform)
		return;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CEffect_Manager* pEffect_Manager = dynamic_cast<CEffect_Manager*>(pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Effect_Manager"));
	if (nullptr == pEffect_Manager)
		return;

	CSkillEffect* pSkillEffect = nullptr;

	int randType = rand() % 7;
	if (randType == 0)
	{
		pSkillEffect = pEffect_Manager->CreateEffect(CEffect_Manager::m_damageEffect00, L"Prototype_GameObject_SkillEffect", Get_LayerTag().c_str(), Get_Levelindex());
	}
	else if (randType == 1)
	{
		pSkillEffect = pEffect_Manager->CreateEffect(CEffect_Manager::m_damageEffect01, L"Prototype_GameObject_SkillEffect", Get_LayerTag().c_str(), Get_Levelindex());
	}
	else
	{
		pSkillEffect = pEffect_Manager->CreateEffect(m_AttackEffectDesc.m_CollisionEffectType, L"Prototype_GameObject_SkillEffect", Get_LayerTag().c_str(), Get_Levelindex());
	}

	if (nullptr != pSkillEffect)
	{
		_float4 vPos{};
		XMStoreFloat4(&vPos, hitObjectTransform->Get_State(CTransform::STATE_POSITION));
		pSkillEffect->Set_Pos(vPos);
	}

	Safe_Release(pSkillEffect);
}

void CAttackEffect::Camera_Shake_Request()
{
	CGameObject* pStageCamera = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Camera", L"Main_Camera");

	if (nullptr == pStageCamera)
		return;

	dynamic_cast<CStageCamera*>(pStageCamera)->Do_Shake();
}

void CAttackEffect::Set_ManualCollisionState(CGameObject* pOtherOwner, CManualCollisionState::COLLISION_STATE eState)
{
	CManualCollisionState* pMCS = pOtherOwner->Get_As<CManualCollisionState>();
	pMCS->Set_State(eState);
}

void CAttackEffect::Do_DebuffCondition(CGameObject* pOtherOwner)
{
	int randomValue = rand() % 8;

	if (randomValue != 0)
		return;

	if (m_AttackEffectDesc.m_ConditionDataID == NO_DEBUFF_CONDITION)
		return;

	CMonster* pOtherMonster = dynamic_cast<CMonster*>(pOtherOwner);
	if (nullptr == pOtherMonster)
		return;

	CGameObject* pObject = CGameInstance::GetInstance()->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"ConditionData");
	CConditionData* pConditionData = dynamic_cast<CConditionData*>(pObject);
	if (nullptr == pConditionData)
		return;

	CConditionData::CONDITIONDATA_DESC conditionDataDesc{};
	pConditionData->Get_ConditonData(conditionDataDesc, m_AttackEffectDesc.m_ConditionDataID);
	_uint conditinoTypeID = conditionDataDesc.m_type;

	CBuffState* pBuffState = pOtherMonster->Search_NoAction_DeBuffState(conditinoTypeID);
	if (nullptr == pBuffState)
		return;

	CConditionData::CONDITIONTYPEDATA_DESC conditionTypeDataDesc = pConditionData->Get_ConditonTypeData(conditinoTypeID);
	pBuffState->Set_BuffState(conditinoTypeID, 500, (CBuffState::BUFF_STATE)conditionTypeDataDesc.m_id,
		conditionTypeDataDesc.m_iconPath.c_str(), conditionDataDesc.m_Value_A, conditionDataDesc.m_Value_B,
		conditionDataDesc.m_time, conditionDataDesc.m_ratio);
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

CAttackEffect* CAttackEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ATTACK_EFFECT_DESC& m_AttackEffectDesc)
{
	CAttackEffect* pInstance = new CAttackEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(m_AttackEffectDesc)))
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