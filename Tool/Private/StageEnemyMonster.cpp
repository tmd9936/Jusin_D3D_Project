#include "stdafx.h"
#include "StageEnemyMonster.h"

#include "GameInstance.h"

#include "Searcher.h"

#include "Stage_Manager.h"

/*
1 스테이지
니드킹 지진, 메가톤 펀치, 돌진
쥬레곤 얼다바람, 하이드럼 펌프
니드리나 독침, 메가폰

2스테이지 
보스 뮤츠
파괴광선
순간이동
미니게임
범위공격 


또가스 자폭, 오물폭탄
가디 불꽃세례

기본 공격 
공격 쿨타임
스킬 쿨타임
스킬 쿨타임에 따라서 랜덤 공격

대기
기본 공격
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

	m_pHPCom->Set_CanGetDamage(false);

	return S_OK;
}

HRESULT CStageEnemyMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	m_pHPCom->Set_CanGetDamage(false);

	return S_OK;
}

_uint CStageEnemyMonster::Tick(_double TimeDelta)
{
	if (false == m_StaySpawn)
	{
		if (m_bDead)
			return OBJ_DEAD;

		Dead_Check();

		return __super::Tick(TimeDelta);
	}
	else
		return 0;
}

_uint CStageEnemyMonster::LateTick(_double TimeDelta)
{
	if (false == m_StaySpawn)
	{
		switch (m_pMonFSM->Get_MotionState())
		{
		case CMonFSM::IDLE1:
			if (Search_Target())
			{
				m_pTarget = m_pSearcher->Get_Target();
			}
			break;
		}

		return __super::LateTick(TimeDelta);
	}
	else
		return 0;
}

HRESULT CStageEnemyMonster::Render()
{
	if (false == m_StaySpawn)
	{
		return __super::Render();
	}
	else
		return S_OK;
}

void CStageEnemyMonster::Change_State_FSM(_uint eState)
{
	switch (eState)
	{
	case CMonFSM::IDLE1:
		break;
	case CMonFSM::ROAR:
		break;

	case CMonFSM::IDLE_GROUND:
		break;
	case CMonFSM::ATK_NORMAL:
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
	case CMonFSM::POKING:
		break;

	case CMonFSM::DEAD_BOSS:
		Boss_DeadEffect(false);
		break;

	default:
		break;
	}
}

void CStageEnemyMonster::On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	CGameObject* pOtherOwner = pOther->Get_Owner();

	if (!pOtherOwner)
		return;

	if (pOtherOwner->Get_LayerTag().compare(L"Layer_Monster") == 0)
	{
		if (false == m_StaySpawn)
		{
			if (fY > 0)
			{
				m_pTransformCom->Go_Left_ByNavigation(0.01666f, m_pNavigationCom);
			}
			else if (fY < 0)
			{
				m_pTransformCom->Go_Right_ByNavigation(0.01666f, m_pNavigationCom);
			}
		}

		Engine::CUtility::CollisionPushingOut(pOther, m_pAABB, fX, fY, fZ, m_pTransformCom, m_pNavigationCom);
	}
}

void CStageEnemyMonster::On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	CGameObject* pOtherOwner = pOther->Get_Owner();

	if (!pOtherOwner)
		return;

	if (pOtherOwner->Get_LayerTag().compare(L"Layer_Monster") == 0)
	{
		if (false == m_StaySpawn)
		{
			if (fY > 0)
			{
				m_pTransformCom->Go_Left_ByNavigation(0.01666f, m_pNavigationCom);
			}
			else if (fY < 0)
			{
				m_pTransformCom->Go_Right_ByNavigation(0.01666f, m_pNavigationCom);
			}
		}
		Engine::CUtility::CollisionPushingOut(pOther, m_pAABB, fX, fY, fZ, m_pTransformCom, m_pNavigationCom);
	}
}

void CStageEnemyMonster::On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
}

void CStageEnemyMonster::AI_Type_Long_Idle_Tick(const _double& TimeDelta, CTransform* pTargetTransform)
{
	if (m_bCanAttack)
	{
		Do_RandomSkill();
	}

	_float targetToDistance = m_pTransformCom->Get_DistanceFromTarget(pTargetTransform->Get_State(CTransform::STATE_POSITION));

	if (m_bChase)
	{
		if (m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta), 2.8f, m_pNavigationCom))
		{
			m_bChase = false;
			m_ChaseCoolTimeAcc = 0.0;
		}
	}
	else
	{
		if (targetToDistance <= 1.5f)
		{
			if (m_pTransformCom->Go_BackWard_Look_Pos(pTargetTransform->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_State(CTransform::STATE_POSITION)
				+ m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.5f, _float(TimeDelta * 1.5), 1.1f, m_pNavigationCom))
			{
			}
		}
	}
	if (!m_bChase)
	{

		m_pTransformCom->TurnToTarget({ 0.f, 1.f, 0.f, 0.f }, pTargetTransform->Get_State(CTransform::STATE_POSITION), (_float)TimeDelta);
		if (targetToDistance >= 3.f)
		{
			m_ChaseCoolTimeAcc += TimeDelta;

			if (m_ChaseCoolTimeAcc > m_ChaseCoolTime)
			{
				m_bChase = true;
			}
		}
	}
}

void CStageEnemyMonster::Dead_Check()
{
	if (nullptr == m_pHPCom)
		return;

	if (m_pHPCom->Get_CurrentHp() <= 0.f
		&& m_pMonFSM->Get_MotionState() != CMonFSM::DEAD_ROTATE 
		&& m_pMonFSM->Get_MotionState() != CMonFSM::DEAD_BOSS)
	{
		if (m_isBoss)
			m_pMonFSM->Transit_MotionState(CMonFSM::DEAD_BOSS, m_pModelCom);
		else
			m_pMonFSM->Transit_MotionState(CMonFSM::DEAD_ROTATE, m_pModelCom);
	}
}

void CStageEnemyMonster::Boss_DeadEffect(_bool isEnd)
{
	CGameObject* pManager = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Manager", L"Stage_Manager");

	if (nullptr == pManager)
		return;

	dynamic_cast<CStage_Manager*>(pManager)->Boss_DeadEffect(isEnd, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
}

void CStageEnemyMonster::TurnToCameraRequest(const _double& TimeDelta)
{
	CGameObject* pManager = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Manager", L"Stage_Manager");

	if (nullptr == pManager)
		return;

	if (dynamic_cast<CStage_Manager*>(pManager)->Request_TurnToCamera(m_pTransformCom, TimeDelta))
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::ROAR, m_pModelCom);
	}
}

_uint CStageEnemyMonster::State_Tick(const _double& TimeDelta)
{
	CTransform* pTargetTransform = nullptr;

	if (nullptr != m_pTarget && !m_pTarget->Is_Dead())
	{
		pTargetTransform = m_pTarget->Get_As<CTransform>();
	}
	else
	{
		m_pTarget = nullptr;
	}

	switch (m_pMonFSM->Get_MotionState())
	{
	case CMonFSM::IDLE1:
		m_pModelCom->Play_Animation(TimeDelta);

		if (m_pTarget)
		{
			if (pTargetTransform)
			{
				//m_pTransformCom->TurnToTarget(XMVectorSet(0.f, 1.f, 0.f, 0.f), pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta * 1.5));
				
				if (m_PokemonDesc.m_AIType == AI_TYPE_SHORT_DISTACE)
				{
					if (m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta), 2.8f, m_pNavigationCom))
					{
						m_pMonFSM->Transit_MotionState(CMonFSM::IDLE_GROUND, m_pModelCom);
					}
				}
				else
				{
					AI_Type_Long_Idle_Tick(TimeDelta, pTargetTransform);
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
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}
		//m_bTurn = true;
		//Set_MovePosition();
		// 카메라 보이면 연출
		break;
	case CMonFSM::ROAR_BEFORE:
		m_pModelCom->Play_Animation(TimeDelta);
		TurnToCameraRequest(TimeDelta);
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
			//m_pTransformCom->TurnToTarget(XMVectorSet(0.f, 1.f, 0.f, 0.f), pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta * 1.5));

			if (m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta * 1.2f), 1.2f, m_pNavigationCom))
			{
				if (m_bCanAttack)
				{
					Do_RandomSkill();
				}
				else 
				{}
			}
		}
		break;

	case CMonFSM::ATK_NORMAL:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_bCanAttack = false;
			m_AttackCoolTimeAcc = 0.0;
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1);
		}
		break;
	case CMonFSM::ATK_SLE_NORMAL_START:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::ATK_SLE_NORMAL_END, m_pModelCom);
			SkillCoolTime_Start();
		}
		break;
	case CMonFSM::ATK_SLE_NORMAL_END:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}
		break;

	case CMonFSM::POKING:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
			SkillCoolTime_Start();
		}
		break;
	case CMonFSM::BODYBLOW:
		//m_pTransformCom->TurnToTarget(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_vTargetPos, TimeDelta * 2.0);
		//m_pTransformCom->ChaseNoLook(m_vTargetPos, _float(TimeDelta * 3.0));
		m_pTransformCom->Go_Straight(_float(TimeDelta * 1.5), m_pNavigationCom);
		if (m_pModelCom->Play_Animation(TimeDelta * 0.7))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
			SkillCoolTime_Start();
		}
		break;
	case CMonFSM::TREMBLING:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
			SkillCoolTime_Start();
		}
		break;
	case CMonFSM::VERTICAL_JUMP:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
			SkillCoolTime_Start();
		}
		break;
	case CMonFSM::JUMPLANDING_SLE_START:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_LOOP, m_pModelCom);
			SkillCoolTime_Start();
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
	case CMonFSM::DEAD_ROTATE:
		m_bHitState = true;
		if (m_pModelCom->Play_Animation(TimeDelta, false))
		{
			Set_Dead();
		}
		break;
	case CMonFSM::DEAD_BOSS:
		m_bHitState = true;
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			Boss_DeadEffect(true);
			Set_Dead();
		}
		break;

	case CMonFSM::IDLE_NO:
		m_pModelCom->Play_Animation(TimeDelta);

		break;

	default:
		break;
	}

	return 0;
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


_bool CStageEnemyMonster::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	if (m_pTransformCom)
	{
		Value PokemonDesc(kObjectType);
		{
			Value ModelPrototypeTag;
			string tag = convert.to_bytes(m_PokemonDesc.ModelPrototypeTag.c_str());
			ModelPrototypeTag.SetString(tag.c_str(), (SizeType)tag.size(), allocator);
			PokemonDesc.AddMember("ModelPrototypeTag", ModelPrototypeTag, allocator);

			PokemonDesc.AddMember("ModelPrototypeLevelIndex", m_PokemonDesc.ModelPrototypeLevelIndex, allocator);

			Value vPos(kObjectType);
			{
				_float4 pos = {};
				XMStoreFloat4(&pos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

				vPos.AddMember("x", pos.x, allocator);
				vPos.AddMember("y", pos.y, allocator);
				vPos.AddMember("z", pos.z, allocator);
				vPos.AddMember("w", pos.w, allocator);
			}
			PokemonDesc.AddMember("vPos", vPos, allocator);

			PokemonDesc.AddMember("moveSpeed", m_PokemonDesc.moveSpeed, allocator);
			PokemonDesc.AddMember("rotateSpeed", m_PokemonDesc.rotateSpeed, allocator);

			PokemonDesc.AddMember("m_monsterNo", m_PokemonDesc.m_monsterNo, allocator);
			PokemonDesc.AddMember("m_attackBasis", m_PokemonDesc.m_attackBasis, allocator);
			PokemonDesc.AddMember("m_hpGrow", m_PokemonDesc.m_hpGrow, allocator);
			PokemonDesc.AddMember("m_normalSkillType", m_PokemonDesc.m_normalSkillType, allocator);
			PokemonDesc.AddMember("m_AIType", m_PokemonDesc.m_AIType, allocator);
			PokemonDesc.AddMember("m_layerType", m_PokemonDesc.m_layerType, allocator);
			PokemonDesc.AddMember("m_isBoss", m_isBoss, allocator);

			Value m_skillIDs(kArrayType);
			{
				for (size_t i = 0; i < m_PokemonDesc.m_skillIDs.size(); ++i)
				{
					m_skillIDs.PushBack(m_PokemonDesc.m_skillIDs[i], allocator);
				}
			}
			PokemonDesc.AddMember("m_skillIDs", m_skillIDs, allocator);

		}
		doc.AddMember("PokemonDesc", PokemonDesc, allocator);
	}

	return true;
}

_bool CStageEnemyMonster::Load_By_JsonFile_Impl(Document& doc)
{
	if (m_pTransformCom)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

		const Value& PokemonDesc = doc["PokemonDesc"];

		string ModelPrototypeTag = PokemonDesc["ModelPrototypeTag"].GetString();
		m_PokemonDesc.ModelPrototypeTag = convert.from_bytes(ModelPrototypeTag);

		m_PokemonDesc.ModelPrototypeLevelIndex = PokemonDesc["ModelPrototypeLevelIndex"].GetUint();

		const Value& vPos = PokemonDesc["vPos"];
		m_pTransformCom->Set_Pos(vPos["x"].GetFloat(), vPos["y"].GetFloat(), vPos["z"].GetFloat());
		m_PokemonDesc.vPos.x = vPos["x"].GetFloat();
		m_PokemonDesc.vPos.y = vPos["y"].GetFloat();
		m_PokemonDesc.vPos.z = vPos["z"].GetFloat();
		m_PokemonDesc.vPos.w = vPos["w"].GetFloat();

		m_PokemonDesc.moveSpeed = PokemonDesc["moveSpeed"].GetFloat();
		m_PokemonDesc.rotateSpeed = PokemonDesc["rotateSpeed"].GetFloat();

		m_PokemonDesc.m_monsterNo = PokemonDesc["m_monsterNo"].GetUint();
		m_PokemonDesc.m_hpBasis = PokemonDesc["m_hpBasis"].GetUint();
		m_PokemonDesc.m_attackBasis = PokemonDesc["m_attackBasis"].GetUint();
		m_PokemonDesc.m_normalSkillType = PokemonDesc["m_normalSkillType"].GetUint();
		m_PokemonDesc.m_AIType = PokemonDesc["m_AIType"].GetUint();
		m_PokemonDesc.m_layerType = PokemonDesc["m_layerType"].GetUint();
		m_isBoss = PokemonDesc["m_isBoss"].GetBool();

		const Value& skillIDs = PokemonDesc["m_skillIDs"];
		for (SizeType i = 0; i < skillIDs.Size(); ++i)
		{
			m_PokemonDesc.m_skillIDs.push_back(skillIDs[i].GetInt());
		}
	}

	return true;
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
