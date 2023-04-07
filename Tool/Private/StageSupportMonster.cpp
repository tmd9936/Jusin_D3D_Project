#include "stdafx.h"
#include "StageSupportMonster.h"

#include "GameInstance.h"

#include "Searcher.h"

/*
파티몬스터 포메이션 테스트하기
이후 버프 테스트 하기
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
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	Init_RandomMotionChangeDelay();

	if (FAILED(Init_MainPlayer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageSupportMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	Init_RandomMotionChangeDelay();

	if (FAILED(Init_MainPlayer()))
		return E_FAIL;

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

HRESULT CStageSupportMonster::Render()
{
	return __super::Render();
}

void CStageSupportMonster::Change_State_FSM(_uint eState)
{
	switch (eState)
	{
	case CMonFSM::IDLE1:
		//m_pNavigationCom->Set_Index_By_Position({ m_PokemonDesc.vPos.x, m_PokemonDesc.vPos.y, m_PokemonDesc.vPos.z });
		break;
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
	case CMonFSM::POKING:
		break;

	default:
		break;
	}
}

void CStageSupportMonster::Go_To_RandomPosition(const _double& TimeDelta)
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

HRESULT CStageSupportMonster::Init_MainPlayer()
{
	CGameInstance* pGameInstance =  CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* m_pMainPlayer = pGameInstance->Get_Object(Get_Levelindex(), Get_LayerTag().c_str(), L"Player");
	if (nullptr == m_pMainPlayer)
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

void CStageSupportMonster::Init_RandomMotionChangeDelay()
{
	m_MotionChangeDelay = _float(rand() % 3 + 2);
	m_CurMotionChangeDelayTime = 0.f;
}

HRESULT CStageSupportMonster::Add_TransitionRandomState()
{
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::ATK_NORMAL);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::ATK_SLE_NORMAL_START);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::BODYBLOW);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::JUMPLANDING_SLE_START);

	return S_OK;
}

_uint CStageSupportMonster::State_Tick(const _double& TimeDelta)
{
	CTransform* pTargetTransform = nullptr;

	if (m_pTarget)
	{
		pTargetTransform = m_pTarget->Get_As<CTransform>();
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
			m_bBattle = true;
		}
	}

	switch (m_pMonFSM->Get_MotionState())
	{
	case CMonFSM::FORMATION_NORMAL:

		break;

	case CMonFSM::FORMATION_RUN:
		break;

	case CMonFSM::IDLE1:
		m_pModelCom->Play_Animation(TimeDelta);

		if (m_pTarget)
		{
			if (pTargetTransform)
			{
				m_pTransformCom->TurnToTarget(XMVectorSet(0.f, 1.f, 0.f, 0.f), pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta * 1.5));

				if (m_PokemonDesc.m_AIType == AI_TYPE_SHORT_DISTACE)
				{
					if (m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta), 2.8f, m_pNavigationCom))
					{
						m_pMonFSM->Transit_MotionState(CMonFSM::IDLE_GROUND, m_pModelCom);
					}
				}
				else
				{
					if (m_bCanAttack)
					{
						Do_RandomSkill();
					}
					else if (!m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta), 2.8f, m_pNavigationCom))
					{
					}
					else
					{
						if (m_pTransformCom->Go_BackWard_Look_Pos(pTargetTransform->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_State(CTransform::STATE_POSITION)
							+ m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -2.f, _float(TimeDelta * 1.5), 0.5f, m_pNavigationCom))
						{
						}
					}
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

			if (m_pTransformCom->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), _float(TimeDelta * 1.5f), 1.5f, m_pNavigationCom))
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
		break;
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

	case CMonFSM::POKING:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_bCanAttack = false;
			m_bCanSkillAttack = false;
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
			m_AttackCoolTimeAcc = m_AttackCoolTime * 1.5f;
			m_SkillCoolTimeAcc = m_SkillCoolTime * 1.5f;
		}

		break;


	case CMonFSM::BODYBLOW:
		//m_pTransformCom->TurnToTarget(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_vTargetPos, TimeDelta * 2.0);
		//m_pTransformCom->ChaseNoLook(m_vTargetPos, _float(TimeDelta * 3.0));
		m_pTransformCom->Go_Straight(_float(TimeDelta * 2.5), m_pNavigationCom);
		if (m_pModelCom->Play_Animation(TimeDelta * 0.7))
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

void CStageSupportMonster::Check_Do_Change_RandomMotion(const _double& TimeDelta)
{
	if (m_MotionChangeDelay < m_CurMotionChangeDelayTime)
	{
		MotionChange_Random();
		Init_RandomMotionChangeDelay();
	}

	m_CurMotionChangeDelayTime += _float(TimeDelta);
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
			PokemonDesc.AddMember("m_visitWeightDefault", m_PokemonDesc.m_visitWeightDefault, allocator);
			PokemonDesc.AddMember("m_visitWeight", m_PokemonDesc.m_visitWeight, allocator);
			PokemonDesc.AddMember("m_cookTableID", m_PokemonDesc.m_cookTableID, allocator);
			PokemonDesc.AddMember("m_color", m_PokemonDesc.m_color, allocator);
			PokemonDesc.AddMember("m_Rate", m_PokemonDesc.m_Rate, allocator);
			PokemonDesc.AddMember("m_isLayer", m_PokemonDesc.m_isLayer, allocator);
			PokemonDesc.AddMember("m_meleePercent", m_PokemonDesc.m_meleePercent, allocator);
			PokemonDesc.AddMember("m_slotTypeWeightHp", m_PokemonDesc.m_slotTypeWeightHp, allocator);
			PokemonDesc.AddMember("m_slotTypeWeightAttack", m_PokemonDesc.m_slotTypeWeightAttack, allocator);
			PokemonDesc.AddMember("m_slotTypeWeightMulti", m_PokemonDesc.m_slotTypeWeightMulti, allocator);
			PokemonDesc.AddMember("m_normalSkillType", m_PokemonDesc.m_normalSkillType, allocator);
			PokemonDesc.AddMember("m_AIType", m_PokemonDesc.m_AIType, allocator);

			Value relativePosition(kObjectType);
			{
				_float4 pos = {};
				XMStoreFloat4(&pos, m_relativePosition);

				relativePosition.AddMember("x", pos.x, allocator);
				relativePosition.AddMember("y", pos.y, allocator);
				relativePosition.AddMember("z", pos.z, allocator);
				relativePosition.AddMember("w", pos.w, allocator);
			}
			PokemonDesc.AddMember("m_relativePosition", relativePosition, allocator);

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

		m_PokemonDesc.m_monsterNo = PokemonDesc["m_monsterNo"].GetUint();
		m_PokemonDesc.m_hpBasis = PokemonDesc["m_hpBasis"].GetUint();
		m_PokemonDesc.m_attackBasis = PokemonDesc["m_attackBasis"].GetUint();
		m_PokemonDesc.m_hpGrow = PokemonDesc["m_hpGrow"].GetUint();
		m_PokemonDesc.m_attackGrow = PokemonDesc["m_attackGrow"].GetUint();
		m_PokemonDesc.m_type1 = PokemonDesc["m_type1"].GetUint();
		m_PokemonDesc.m_type2 = PokemonDesc["m_type2"].GetUint();
		m_PokemonDesc.m_visitWeightDefault = PokemonDesc["m_visitWeightDefault"].GetUint();
		m_PokemonDesc.m_visitWeight = PokemonDesc["m_visitWeight"].GetUint();
		m_PokemonDesc.m_cookTableID = PokemonDesc["m_cookTableID"].GetUint();
		m_PokemonDesc.m_color = PokemonDesc["m_color"].GetUint();
		m_PokemonDesc.m_Rate = PokemonDesc["m_Rate"].GetUint();
		m_PokemonDesc.m_isLayer = PokemonDesc["m_isLayer"].GetUint();
		m_PokemonDesc.m_meleePercent = PokemonDesc["m_meleePercent"].GetUint();
		m_PokemonDesc.m_slotTypeWeightHp = PokemonDesc["m_slotTypeWeightHp"].GetUint();
		m_PokemonDesc.m_slotTypeWeightAttack = PokemonDesc["m_slotTypeWeightAttack"].GetUint();
		m_PokemonDesc.m_slotTypeWeightMulti = PokemonDesc["m_slotTypeWeightMulti"].GetUint();

		m_PokemonDesc.m_normalSkillType = PokemonDesc["m_normalSkillType"].GetUint();
		m_PokemonDesc.m_AIType = PokemonDesc["m_AIType"].GetUint();

		const Value& relativePosition = PokemonDesc["m_relativePosition"];
		m_relativePosition = XMVectorSet(relativePosition["x"].GetFloat(),
			relativePosition["y"].GetFloat(),
			relativePosition["z"].GetFloat(),
			relativePosition["w"].GetFloat()
		);

		const Value& skillIDs = PokemonDesc["m_skillIDs"];
		for (SizeType i = 0; i < skillIDs.Size(); ++i)
		{
			m_PokemonDesc.m_skillIDs.push_back(skillIDs[i].GetInt());
		}
	}

	return true;
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
}
