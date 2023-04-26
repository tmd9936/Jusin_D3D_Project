#include "stdafx.h"
#include "StageSupportMonster.h"

#include "GameInstance.h"

#include "Searcher.h"
#include "Utility.h"

/*
파티몬스터 포메이션 테스트하기
이후 버프 디버프 테스트 하기
*/

CStageSupportMonster::CStageSupportMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CStageSupportMonster::CStageSupportMonster(const CMonster& rhs)
	: CMonster(rhs)
{
}

HRESULT CStageSupportMonster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageSupportMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	/* For.Com_Formation */
	if (FAILED(pGameInstance->Add_Component(CFormation::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Formation"),
		(CComponent**)&m_pFormationCom, nullptr)))
		return E_FAIL;

	if (FAILED(Init_MainPlayer()))
		return E_FAIL;

	m_bBattle = false;
	m_pMonFSM->Transit_MotionState(CMonFSM::FORMATION_NORMAL, m_pModelCom);

	return S_OK;
}

HRESULT CStageSupportMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (FAILED(Init_MainPlayer()))
		return E_FAIL;

	m_bBattle = false;
	m_pMonFSM->Transit_MotionState(CMonFSM::FORMATION_NORMAL, m_pModelCom);

	return S_OK;
}

_uint CStageSupportMonster::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

_uint CStageSupportMonster::LateTick(_double TimeDelta)
{
 	switch (m_pMonFSM->Get_MotionState())
	{
	case CMonFSM::FORMATION_NORMAL:
		if (Search_Target())
		{
			m_pTarget = m_pSearcher->Get_Target();
		}
		break;
	case CMonFSM::FORMATION_RUN:
		if (Search_Target())
		{
			m_pTarget = m_pSearcher->Get_Target();
		}
		break;
	case CMonFSM::IDLE1:
		if (Search_Target())
		{
			m_pTarget = m_pSearcher->Get_Target();
		}
		break;
	}

	if (m_pTarget)
	{
		if (!m_bBattle)
		{
			m_bBattle = true;
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}
	}
	else
	{
		if (m_bBattle)
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::FORMATION_NORMAL, m_pModelCom);
			m_bBattle = false;
		}
	}

	return __super::LateTick(TimeDelta);
}

HRESULT CStageSupportMonster::Render()
{
	return __super::Render();
}

void CStageSupportMonster::On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	CGameObject* pOtherOwner = pOther->Get_Owner();

	if (!pOtherOwner)
		return;

	if (pOtherOwner->Get_LayerTag().compare(L"Layer_Monster") == 0)
	{
		Engine::CUtility::CollisionPushingOut(pOther, m_pAABB, fX, fY, fZ, m_pTransformCom, m_pNavigationCom);
	}

	if (pOtherOwner->Get_LayerTag().compare(L"Layer_Player") == 0)
	{
		Engine::CUtility::CollisionPushingOut(pOther, m_pAABB, fX, fY, fZ, m_pTransformCom, m_pNavigationCom);

		if (fX > 0)
		{
			m_pTransformCom->Go_Left_ByNavigation(0.01666f, m_pNavigationCom);
		}
		else if (fX < 0)
		{
			m_pTransformCom->Go_Right_ByNavigation(0.01666f, m_pNavigationCom);
		}

		if (m_FormationChanger)
		{
			m_FormationFightTimeAcc += 0.01666;
		}
	}
}

void CStageSupportMonster::On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	CGameObject* pOtherOwner = pOther->Get_Owner();

	if (!pOtherOwner)
		return;

	if (pOtherOwner->Get_LayerTag().compare(L"Layer_Monster") == 0)
	{
		Engine::CUtility::CollisionPushingOut(pOther, m_pAABB, fX, fY, fZ, m_pTransformCom, m_pNavigationCom);
	}

	if (pOtherOwner->Get_LayerTag().compare(L"Layer_Player") == 0)
	{
		Engine::CUtility::CollisionPushingOut(pOther, m_pAABB, fX, fY, fZ, m_pTransformCom, m_pNavigationCom);

		if (fY > 0)
		{
			m_pTransformCom->Go_Left_ByNavigation(0.01666f, m_pNavigationCom);
		}
		else if (fY < 0)
		{
			m_pTransformCom->Go_Right_ByNavigation(0.01666f, m_pNavigationCom);
		}

		if (m_FormationChanger)
		{
			if (m_FormationFightTimeAcc >= m_FormationFightTime)
			{
				CFormation* pOtherOwnerFormation = pOtherOwner->Get_As<CFormation>();
				if (pOtherOwnerFormation)
				{
					m_pFormationCom->Swap_RelativePos(pOtherOwnerFormation);
					m_FormationFightTimeAcc = 0.0;
				}
			}
			m_FormationFightTimeAcc += 0.01666;
		}
	}
}

void CStageSupportMonster::On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	m_FormationFightTimeAcc = 0.0;
}

void CStageSupportMonster::Change_State_FSM(_uint eState)
{
	switch (eState)
	{
	case CMonFSM::IDLE1:
		//m_pNavigationCom->Set_Index_By_Position({ m_PokemonDesc.vPos.x, m_PokemonDesc.vPos.y, m_PokemonDesc.vPos.z });
		m_bBattle = true;
		break;
	case CMonFSM::IDLE_GROUND:
		m_bBattle = true;
		break;
	case CMonFSM::ATK_NORMAL:
		m_bBattle = true;
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
	case CMonFSM::FORMATION_NORMAL:
		m_bBattle = false;
		break;
	case CMonFSM::FORMATION_RUN:
		m_bBattle = false;
		break;

	default:
		break;
	}
}


HRESULT CStageSupportMonster::Init_MainPlayer()
{
	CGameInstance* pGameInstance =  CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pMainPlayer = pGameInstance->Get_Object(Get_Levelindex(), Get_LayerTag().c_str(), L"Player1");
	if (nullptr == m_pMainPlayer)
		return E_FAIL;
	Safe_AddRef(m_pMainPlayer);

	m_pMainPlayerTransform = m_pMainPlayer->Get_As<CTransform>();
	Safe_AddRef(m_pMainPlayerTransform);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CStageSupportMonster::Add_TransitionRandomState()
{
	return S_OK;
}

_uint CStageSupportMonster::State_Tick(const _double& TimeDelta)
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
	case CMonFSM::FORMATION_NORMAL:
		m_pModelCom->Play_Animation(TimeDelta);
		if (2.0f <= m_pTransformCom->Get_DistanceFromTarget(m_pMainPlayerTransform->Get_State(CTransform::STATE_POSITION) + m_pFormationCom->Get_RelativePos()))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::FORMATION_RUN, m_pModelCom);
			break;
		}
		m_pMainPlayerTransform->Get_NoScaleState(CTransform::STATE_LOOK) * XMVectorGetZ(m_pFormationCom->Get_RelativePos());
		m_pTransformCom->Chase(m_pMainPlayerTransform->Get_State(CTransform::STATE_POSITION) + 
			m_pMainPlayerTransform->Get_NoScaleState(CTransform::STATE_LOOK) * XMVectorGetZ(m_pFormationCom->Get_RelativePos()) +
			m_pMainPlayerTransform->Get_NoScaleState(CTransform::STATE_RIGHT) * XMVectorGetX(m_pFormationCom->Get_RelativePos()),
			_float(TimeDelta), 0.3f, m_pNavigationCom);
		break;

	case CMonFSM::FORMATION_RUN:
		m_pModelCom->Play_Animation(TimeDelta);
		if (m_pTransformCom->Chase(m_pMainPlayerTransform->Get_State(CTransform::STATE_POSITION) + m_pFormationCom->Get_RelativePos(), _float(TimeDelta), 1.0f, m_pNavigationCom))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::FORMATION_NORMAL, m_pModelCom);
		}
		break;

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
		else
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::FORMATION_NORMAL, m_pModelCom);
		}
		break;

	case CMonFSM::RUN_NO:
		m_pModelCom->Play_Animation(TimeDelta);

		if (m_bCanAttack || m_bCanSkillAttack)
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}

		break;

	case CMonFSM::IDLE_GROUND:
		m_pModelCom->Play_Animation(TimeDelta);


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
				{
				}
			}
		}
		break;

	case CMonFSM::ATK_NORMAL:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_bCanAttack = false;
			m_AttackCoolTimeAcc = 0.0;
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
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
		m_pTransformCom->Go_Straight(_float(TimeDelta * 2.5), m_pNavigationCom);
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

	case CMonFSM::IDLE_NO:
		m_pModelCom->Play_Animation(TimeDelta);
		break;

	case CMonFSM::STAGE_CLEAR:
		m_pModelCom->Play_Animation(TimeDelta);
		break;

	default:
		break;
	}

	return 0;
}


void CStageSupportMonster::MotionChange_Random()
{
	m_pMonFSM->Get_RandomState(m_pModelCom);
}

void CStageSupportMonster::Do_RandomSkill()
{
	if (!m_bCanAttack)
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		return;
	}

	if (!m_bCanSkillAttack || m_PokemonDesc.m_skillIDs.size() <= 0)
	{
		//m_pMonFSM->Transit_MotionState(CMonFSM::ATK_NORMAL, m_pModelCom);
		Do_Skill_After_Set_Motion(m_PokemonDesc.m_normalSkillType, L"Layer_PlayerSkill");
		return;
	}

	_uint randSKill = rand() % m_PokemonDesc.m_skillIDs.size();

	Do_Skill_After_Set_Motion(m_PokemonDesc.m_skillIDs[randSKill], L"Layer_PlayerSkill");
}

_bool CStageSupportMonster::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
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
			PokemonDesc.AddMember("m_attackGrow", m_PokemonDesc.m_attackGrow, allocator);
			PokemonDesc.AddMember("m_type1", m_PokemonDesc.m_type1, allocator);
			PokemonDesc.AddMember("m_type2", m_PokemonDesc.m_type2, allocator);
			PokemonDesc.AddMember("m_normalSkillType", m_PokemonDesc.m_normalSkillType, allocator);
			PokemonDesc.AddMember("m_AIType", m_PokemonDesc.m_AIType, allocator);
			PokemonDesc.AddMember("m_layerType", m_PokemonDesc.m_layerType, allocator);

			Value relativePosition(kObjectType);
			{
				_float4 pos = {};
				XMStoreFloat4(&pos, m_pFormationCom->Get_RelativePos());

				relativePosition.AddMember("x", pos.x, allocator);
				relativePosition.AddMember("y", pos.y, allocator);
				relativePosition.AddMember("z", pos.z, allocator);
				relativePosition.AddMember("w", pos.w, allocator);
			}
			PokemonDesc.AddMember("m_relativePosition", relativePosition, allocator);

			PokemonDesc.AddMember("m_FormationChanger", m_FormationChanger, allocator);

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

_bool CStageSupportMonster::Load_By_JsonFile_Impl(Document& doc)
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

		m_PokemonDesc.m_level = PokemonDesc["m_level"].GetUint();
		m_PokemonDesc.m_exp = PokemonDesc["m_exp"].GetInt();

		m_PokemonDesc.m_monsterNo = PokemonDesc["m_monsterNo"].GetUint();
		m_PokemonDesc.m_hpBasis = PokemonDesc["m_hpBasis"].GetUint();
		m_PokemonDesc.m_attackBasis = PokemonDesc["m_attackBasis"].GetUint();
		m_PokemonDesc.m_hpGrow = PokemonDesc["m_hpGrow"].GetUint();
		m_PokemonDesc.m_attackGrow = PokemonDesc["m_attackGrow"].GetUint();
		m_PokemonDesc.m_type1 = PokemonDesc["m_type1"].GetUint();
		m_PokemonDesc.m_type2 = PokemonDesc["m_type2"].GetUint();

		m_PokemonDesc.m_normalSkillType = PokemonDesc["m_normalSkillType"].GetUint();
		m_PokemonDesc.m_AIType = PokemonDesc["m_AIType"].GetUint();
		m_PokemonDesc.m_layerType = PokemonDesc["m_layerType"].GetUint();

		const Value& relativePosition = PokemonDesc["m_relativePosition"];
		_vector vRelativePos = XMVectorSet(relativePosition["x"].GetFloat(),
			relativePosition["y"].GetFloat(),
			relativePosition["z"].GetFloat(),
			relativePosition["w"].GetFloat()
		);
		m_pFormationCom->Set_RelativePos(vRelativePos);

		m_FormationChanger = PokemonDesc["m_FormationChanger"].GetBool();

		const Value& skillIDs = PokemonDesc["m_skillIDs"];
		for (SizeType i = 0; i < skillIDs.Size(); ++i)
		{
			m_PokemonDesc.m_skillIDs.push_back(skillIDs[i].GetInt());
		}

		const Value& m_stones = PokemonDesc["m_stones"].GetArray();
		for (SizeType i = 0; i < m_stones.Size(); ++i)
		{
			STONE_EQUIP_DESC desc{};

			desc.m_isOpen = m_stones[i]["m_isOpen"].GetBool();
			desc.m_type = (STONE_EQUIP_TYPE)m_stones[i]["m_type"].GetUint();
			desc.m_equip_stoneID = m_stones[i]["m_equip_stoneID"].GetUint();
			desc.m_state = (STONE_EQUIP_STATE)m_stones[i]["m_state"].GetUint();

			m_PokemonDesc.m_stones.push_back(move(desc));
		}
	}

	return true;
}

HRESULT CStageSupportMonster::Load_By_Json_PreAddComponents()
{
	if (FAILED(__super::Load_By_Json_PreAddComponents()))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Formation */
	if (FAILED(pGameInstance->Add_Component(CFormation::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Formation"),
		(CComponent**)&m_pFormationCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

void CStageSupportMonster::AI_Type_Long_Idle_Tick(const _double& TimeDelta, CTransform* pTargetTransform)
{
	if (m_bCanAttack)
	{
		Do_RandomSkill();
	}

	_float targetToDistance = m_pTransformCom->Get_DistanceFromTarget(pTargetTransform->Get_State(CTransform::STATE_POSITION));

	if (m_bChase)
	{
		if (m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta), 2.0f, m_pNavigationCom))
		{
			m_bChase = false;
			m_ChaseCoolTimeAcc = 0.0;
		}
	}
	else
	{
		if (targetToDistance <= 0.8f)
		{
			if (m_pTransformCom->Go_BackWard_Look_Pos(pTargetTransform->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_State(CTransform::STATE_POSITION)
				+ m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.5f, _float(TimeDelta * 1.5), 1.2f, m_pNavigationCom))
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


CStageSupportMonster* CStageSupportMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageSupportMonster* pInstance = new CStageSupportMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageSupportMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageSupportMonster::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageSupportMonster* pInstance = new CStageSupportMonster(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStageSupportMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageSupportMonster::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStageSupportMonster* pInstance = new CStageSupportMonster(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStageSupportMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageSupportMonster::Free()
{
	__super::Free();

	Safe_Release(m_pMainPlayer);
	Safe_Release(m_pMainPlayerTransform);
	Safe_Release(m_pFormationCom);

}
