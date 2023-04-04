#include "stdafx.h"
#include "StageEnemyMonster.h"

#include "GameInstance.h"

#include "Searcher.h"

/*
1 스테이지
니드킹 지진, 메가톤 펀치, 돌진
쥬레곤 얼다바람, 아쿠아링
니드리나 독침, 메가폰

2스테이지 
보스 레어코일
또가스 자폭, 오물폭탄
가디 불꽃세례

기본 공격 
공격 쿨타임
스킬 쿨타임
스킬 쿨타임에 따라서 랜덤 공격

대기
기본공격
공격 쿨타임 시
쿨타임 끝나고 공격
추적 타입 or 거리 조절 타입?

추적이랑 빠지는 행동 시간 조절
멀어지면 추적
of
가까워지면 빠지기
가까운 타겟 검색
공격

적 보스 만들고 아군 몬스터 및 카메라 만들기


*/

CStageEnemyMonster::CStageEnemyMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CStageEnemyMonster::CStageEnemyMonster(const CMonster& rhs)
	: CMonster(rhs)
{
}

HRESULT CStageEnemyMonster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageEnemyMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	Init_RandomMotionChangeDelay();

	return S_OK;
}

HRESULT CStageEnemyMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	Init_RandomMotionChangeDelay();

	return S_OK;
}

_uint CStageEnemyMonster::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

_uint CStageEnemyMonster::LateTick(_double TimeDelta)
{
	switch (m_pMonFSM->Get_MotionState())
	{
	case CMonFSM::IDLE1:
		if (Search_Target())
		{
			//m_pMonFSM->Transit_MotionState(CMonFSM::IDLE_GROUND);
			m_pTarget = m_pSearcher->Get_Target();
		}
		break;
	}

	return __super::LateTick(TimeDelta);
}

HRESULT CStageEnemyMonster::Render()
{
	return __super::Render();
}

void CStageEnemyMonster::Change_State_FSM(_uint eState)
{
	switch (eState)
	{
	case CMonFSM::IDLE1:


	case CMonFSM::ROAR:
		//m_bTurn = true;
		//Set_MovePosition();
		// 카메라 보이면 연출
		break;

	case CMonFSM::IDLE_GROUND:
		break;

	case CMonFSM::ATK_NORMAL:
		//Do_Skill(m_PokemonDesc.m_normalSkillType, L"Layer_MonsterSkill");
		break;

	case CMonFSM::ATK_SLE_NORMAL_START:
		break;

	case CMonFSM::ATK_SLE_NORMAL_LOOP:
		break;
	case CMonFSM::ATK_SLE_NORMAL_END:
		break;

	case CMonFSM::BODYBLOW:
		if (m_pTarget)
		{
			CTransform* pTargetTransform = m_pTarget->Get_As<CTransform>();

			if (pTargetTransform)
			{
				m_vTargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
			}
		}
		break;

	case CMonFSM::VERTICAL_JUMP:
		break;
	case CMonFSM::TREMBLING:
		break;	
	case CMonFSM::JUMPLANDING_SLE_START:
		break;

	default:
		break;
	}
}

void CStageEnemyMonster::Go_To_RandomPosition(const _double& TimeDelta)
{
	if (m_bTurn)
	{
		if (m_pTransformCom->TurnToTarget({ 0.f, 1.f, 0.f, 0.f }, XMLoadFloat4(&m_MovePosition), (_float)TimeDelta))
		{
			m_bTurn = false;
		}
	}
	else
	{
		Check_Do_Change_RandomMotion(TimeDelta);
		if (m_pTransformCom->Chase(XMLoadFloat4(&m_MovePosition), (_float)TimeDelta, 0.2f, m_pNavigationCom))
		{
			MotionChange_Random();
			Init_RandomMotionChangeDelay();
		}
	}

}

void CStageEnemyMonster::Init_RandomMotionChangeDelay()
{
	m_MotionChangeDelay = _float(rand() % 3 + 2);
	m_CurMotionChangeDelayTime = 0.f;
}

HRESULT CStageEnemyMonster::Add_TransitionRandomState()
{
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::ATK_NORMAL);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::ATK_SLE_NORMAL_START);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::BODYBLOW);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::JUMPLANDING_SLE_START);

	return S_OK;
}

_uint CStageEnemyMonster::State_Tick(const _double& TimeDelta)
{
	CTransform* pTargetTransform = nullptr;
	//_float4x4 mat = {};

	if (m_pTarget)
	{
		pTargetTransform = m_pTarget->Get_As<CTransform>();
	}

	switch (m_pMonFSM->Get_MotionState())
	{
	case CMonFSM::IDLE1:
		m_pModelCom->Play_Animation(TimeDelta);

		if (m_pTarget)
		{

			if (pTargetTransform)
			{
				m_pTransformCom->TurnToTarget(XMVectorSet(0.f, 1.f, 0.f, 0.f), pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta * 1.5));
				if (m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta), 3.3f, m_pNavigationCom))
				{
					m_pMonFSM->Transit_MotionState(CMonFSM::IDLE_GROUND, m_pModelCom);
				}
			}
		}
		break;

	case CMonFSM::RUN_NO:
		m_pModelCom->Play_Animation(TimeDelta);

		if (m_bCanAttack || m_bCanSkillAttack)
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}

		break;

	case CMonFSM::ROAR:
		m_pModelCom->Play_Animation(TimeDelta);
		//m_bTurn = true;
		//Set_MovePosition();
		// 카메라 보이면 연출
		break;

	case CMonFSM::IDLE_GROUND:
		m_pModelCom->Play_Animation(TimeDelta);

		if (nullptr == m_pTarget || m_pTarget->Is_Dead())
		{
			if (Search_Target())
			{
				m_pTarget = m_pSearcher->Get_Target();
				break;
			}
		}

		if (pTargetTransform)
		{
			m_pTransformCom->TurnToTarget(XMVectorSet(0.f, 1.f, 0.f, 0.f), pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta * 1.5));

			if (m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta), 1.3f, m_pNavigationCom))
			{
				if (m_bCanAttack)
				{
					Do_RandomSkill();
				}
				else 
				{
					m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);

				}
			}
		}
		break;

	case CMonFSM::ATK_NORMAL:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_bCanAttack = false;
			m_AttackCoolTimeAcc = m_AttackCoolTime;
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}

	case CMonFSM::ATK_SLE_NORMAL_START:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::ATK_SLE_NORMAL_END, m_pModelCom);
			m_bCanAttack = false;
			m_bCanSkillAttack = false;
			m_AttackCoolTimeAcc = m_AttackCoolTime;
			m_SkillCoolTimeAcc = m_SkillCoolTime;
		}
		break;
	case CMonFSM::ATK_SLE_NORMAL_END:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}

		break;

	case CMonFSM::BODYBLOW:
		//m_pTransformCom->TurnToTarget(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_vTargetPos, TimeDelta * 2.0);
		m_pTransformCom->ChaseNoLook(m_vTargetPos, _float(TimeDelta * 2.0));

		if (m_pModelCom->Play_Animation(TimeDelta * 1.5))
		{
			m_bCanSkillAttack = false;
			m_bCanAttack = false;
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
			m_AttackCoolTimeAcc = m_AttackCoolTime;
			m_SkillCoolTimeAcc = m_SkillCoolTime;
		}

		break;

	case CMonFSM::TREMBLING:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_bCanSkillAttack = false;
			m_bCanAttack = false;
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
			m_AttackCoolTimeAcc = m_AttackCoolTime;
			m_SkillCoolTimeAcc = m_SkillCoolTime;
		}
		break;

	case CMonFSM::VERTICAL_JUMP:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_bCanSkillAttack = false;
			m_bCanAttack = false;
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
			m_AttackCoolTimeAcc = m_AttackCoolTime;
			m_SkillCoolTimeAcc = m_SkillCoolTime;
		}
		break;

	case CMonFSM::JUMPLANDING_SLE_START:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_bCanSkillAttack = false;
			m_bCanAttack = false;
			m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_LOOP, m_pModelCom);
			m_AttackCoolTimeAcc = m_AttackCoolTime;
			m_SkillCoolTimeAcc = m_SkillCoolTime;
			//mat = m_pModelCom->Get_CombinedTransformationMatrix_float4_4(0);
			//m_pTransformCom->Set_PosY(mat.m[3][2]);
		}
		break;

	case CMonFSM::JUMPLANDING_SLE_LOOP:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_END, m_pModelCom);
		}
		break;
	case CMonFSM::JUMPLANDING_SLE_END:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
			//mat = m_pModelCom->Get_CombinedTransformationMatrix_float4_4(0);
			//m_pTransformCom->Set_PosY(mat.m[3][2]);
		}
		break;
	default:
		break;
	}



	return 0;
}

void CStageEnemyMonster::Check_Do_Change_RandomMotion(const _double& TimeDelta)
{
	if (m_MotionChangeDelay < m_CurMotionChangeDelayTime)
	{
		MotionChange_Random();
		Init_RandomMotionChangeDelay();
	}

	m_CurMotionChangeDelayTime += _float(TimeDelta);
}

void CStageEnemyMonster::MotionChange_Random()
{
	m_pMonFSM->Get_RandomState(m_pModelCom);
}

void CStageEnemyMonster::Do_RandomSkill()
{
	if (!m_bCanAttack)
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		return;

	}

	if ( !m_bCanSkillAttack || m_PokemonDesc.m_skillIDs.size() <= 0)
	{
		//m_pMonFSM->Transit_MotionState(CMonFSM::ATK_NORMAL, m_pModelCom);
		Do_Skill_After_Set_Motion(m_PokemonDesc.m_normalSkillType, L"Layer_MonsterSkill");
		return;
	}

	_uint randSKill = rand() % m_PokemonDesc.m_skillIDs.size();

	Do_Skill_After_Set_Motion(m_PokemonDesc.m_skillIDs[randSKill], L"Layer_MonsterSkill");
}

CStageEnemyMonster* CStageEnemyMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageEnemyMonster* pInstance = new CStageEnemyMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageEnemyMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageEnemyMonster::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageEnemyMonster* pInstance = new CStageEnemyMonster(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStageEnemyMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageEnemyMonster::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStageEnemyMonster* pInstance = new CStageEnemyMonster(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStageEnemyMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageEnemyMonster::Free()
{
	__super::Free();
}
