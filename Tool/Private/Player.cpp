#include "stdafx.h"
#include "Player.h"

#include "GameInstance.h"

#include "Skill_Manager.h"
#include "Skill.h"

#include "Utility.h"

#include "HpBar.h"
#include "DamageText.h"
#include "BuffState.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{

}

CPlayer::CPlayer(const CPlayer& rhs)
	: CMonster(rhs)
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	m_pModelCom->Set_Animation(0);
	m_SkillLoopDesc.m_eLoopState = CMonFSM::END_MOTION;

	return S_OK;
}

HRESULT CPlayer::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	m_pModelCom->Set_Animation(0);
	m_SkillLoopDesc.m_eLoopState = CMonFSM::END_MOTION;

	return S_OK;
}

_uint CPlayer::Tick(_double TimeDelta)
{
	for (auto& pBuffStates : m_buffStates)
		pBuffStates->Tick(TimeDelta);

	if (m_pHpBar)
		m_pHpBar->Tick(TimeDelta);

	if (m_pHPCom)
		m_pHPCom->Tick(TimeDelta);

	if (m_pDamageText)
		m_pDamageText->Tick(TimeDelta);

	m_pManualCollisionState->Tick(TimeDelta);

	CoolTimeCheck(TimeDelta);

	HitTimeCheck(TimeDelta);

	m_pAABB->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());

	_float4x4 mat = {};
	switch (m_pMonFSM->Get_MotionState())
	{
	case CMonFSM::IDLE1:
		m_pModelCom->Play_Animation(TimeDelta);
		if (m_SkillLoopDesc.m_eLoopState != CMonFSM::END_MOTION)
		{
			if (m_SkillLoopDelay < 0.f)
			{
				Do_Skill(m_PokemonDesc.m_skillIDs[m_SkillLoopDesc.m_CurskillIndex], m_SkillLoopDesc.m_eLoopState, L"Layer_PlayerSkill");
				//m_pMonFSM->Transit_MotionState(m_SkillLoopDesc.m_eLoopState, m_pModelCom);
			}
			m_SkillLoopDelay -= (_float)TimeDelta;
		}
		break;
	case CMonFSM::ATK_NORMAL:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}
		break;
	case CMonFSM::JUMPLANDING_SLE_START:
		if (m_pModelCom->Play_Animation(TimeDelta, false))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_END, m_pModelCom);
			//mat = m_pModelCom->Get_CombinedTransformationMatrix_float4_4(0);
			//m_pTransformCom->Set_PosY(mat.m[3][2]);
		}
		break;
	case CMonFSM::JUMPLANDING_SLE_LOOP:
		if (m_pModelCom->Play_Animation(TimeDelta, false))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_END, m_pModelCom);
		}
		break;
	case CMonFSM::ROTATE_LOOP:
		if (m_pModelCom->Play_Animation(TimeDelta, false))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_END, m_pModelCom);
		}
		break;
	//case CMonFSM::JUMPLANDING_SLE_LOOP:
	//	m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_END, m_pModelCom);
	//	break;
	case CMonFSM::JUMPLANDING_SLE_END:
		if (m_pModelCom->Play_Animation(TimeDelta, false))
		{
			//mat = m_pModelCom->Get_CombinedTransformationMatrix_float4_4(0);
			//m_pTransformCom->Set_PosY(mat.m[3][2]);
			//m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
			Do_Skill(220, CMonFSM::IDLE1, L"Layer_PlayerSkill");

		}
		break;
	case CMonFSM::HAPPY:
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}
		break;
	case CMonFSM::DASH_SLE_START:
		m_pTransformCom->Go_Straight(_float(TimeDelta * m_fAccel), m_pNavigationCom);
		m_fAccel += TimeDelta * 1.75f;
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::DASH_SLE_LOOP, m_pModelCom);
		}
		break;
	case  CMonFSM::DASH_SLE_LOOP:
		m_pTransformCom->Go_Straight(_float(TimeDelta * m_fAccel), m_pNavigationCom);
		m_fAccel += TimeDelta * 2.25f;
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::DASH_SLE_END, m_pModelCom);
		}
		break;
	case CMonFSM::DASH_SLE_END:
		m_fAccel -= TimeDelta * 2.5f;
		m_pTransformCom->Go_Straight(_float(TimeDelta * m_fAccel), m_pNavigationCom);
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			if (m_SkillLoopCount > 0)
			{
				--m_SkillLoopCount;
				m_SkillLoopDesc.m_eLoopState = CMonFSM::DASH_SLE_START;
				m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom); 
				m_SkillLoopDesc.m_CurskillIndex = 0;
				m_fAccel = 1.f;

				break;
			}
			else
			{
				m_pMonFSM->Transit_MotionState(CMonFSM::JUMPLANDING_SLE_START, m_pModelCom);
				m_SkillLoopDesc.m_eLoopState = CMonFSM::END_MOTION;
			}
			m_fAccel = 1.f;
		}
		break;
	case CMonFSM::IDLE_NO:
		m_pModelCom->Play_Animation(TimeDelta);
		break;
	default:
		break;
	}

	if (CMonFSM::IDLE_NO != m_pMonFSM->Get_MotionState())
		Key_Input(TimeDelta);

	if (KEY_TAB(KEY::SPACE))
	{
 		Do_TestSkill();
	}

	if (KEY_TAB(KEY::K))
	{
		Do_Skill(m_TestSkillindex, CMonFSM::ATK_NORMAL, L"Layer_PlayerSkill");
	}

	return _uint();
}

//_uint CPlayer::LateTick(_double TimeDelta)
//{
//	return __super::LateTick(TimeDelta);
//}
//
//HRESULT CPlayer::Render()
//{
//	return __super::Render();
//}

void CPlayer::Do_TestSkill()
{
	if (m_pMonFSM->Get_MotionState() != CMonFSM::ATK_NORMAL)
	{
		m_pMonFSM->Transit_MotionState(CMonFSM::ATK_NORMAL, m_pModelCom);

		CGameObject* pSkill_Mananger = CGameInstance::GetInstance()->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Skill_Manager");
		if (nullptr != pSkill_Mananger)
		{
			CSkill* pSkill = dynamic_cast<CSkill_Manager*>(pSkill_Mananger)->Create_Test_Skill(L"Layer_PlayerSkill", m_iLevelindex, m_TestSkillindex,
				m_pTransformCom->Get_WorldMatrix_Matrix());

			Safe_Release(pSkill);
		}
	}
}

void CPlayer::Jump_Rotate()
{
	m_pMonFSM->Transit_MotionState(CMonFSM::HAPPY, m_pModelCom);

}

void CPlayer::On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
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
	}
}

void CPlayer::On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
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
	}
}

void CPlayer::On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
}

_bool CPlayer::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
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
			PokemonDesc.AddMember("m_normalSkillType2", m_normalSkillType2, allocator);

			PokemonDesc.AddMember("m_layerType", m_PokemonDesc.m_layerType, allocator);

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

_bool CPlayer::Load_By_JsonFile_Impl(Document& doc)
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
		m_normalSkillType2 = PokemonDesc["m_normalSkillType2"].GetUint();

		m_PokemonDesc.m_layerType = PokemonDesc["m_layerType"].GetUint();

		const Value& skillIDs = PokemonDesc["m_skillIDs"];
		for (SizeType i = 0; i < skillIDs.Size(); ++i)
		{
			m_PokemonDesc.m_skillIDs.push_back(skillIDs[i].GetInt());
		}
	}

	return true;
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

}

void CPlayer::Key_Input(const _double& TimeDelta)
{
	if (KEY_TAB(KEY::DOWN))
	{
		if (!m_bAttack)
		{
			if (m_pMonFSM->Get_MotionState() != CMonFSM::RUN_GOUND2)
			{
				m_pMonFSM->Transit_MotionState(CMonFSM::RUN_GOUND2, m_pModelCom);
			}
		}
		m_pTransformCom->Go_Backward((_float)TimeDelta, m_pNavigationCom);
	}
	else if (KEY_HOLD(KEY::DOWN))
	{
		if (!m_bAttack)
		{
			m_pTransformCom->Go_Backward((_float)TimeDelta, m_pNavigationCom);
		}
	}
	else  if (KEY_AWAY(KEY::DOWN))
	{
		if (!m_bAttack)
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (_float)TimeDelta * -1.f);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (_float)TimeDelta);
	}

	if (KEY_TAB(KEY::UP))
	{
		if (!m_bAttack)
		{
			if (m_pMonFSM->Get_MotionState() != CMonFSM::RUN_GOUND2)
			{
				m_pMonFSM->Transit_MotionState(CMonFSM::RUN_GOUND2, m_pModelCom);
			}
		}

		m_pTransformCom->Go_Straight((_float)TimeDelta, m_pNavigationCom);
	}
	else if (KEY_HOLD(KEY::UP))
	{
		if (!m_bAttack)
		{
			m_pTransformCom->Go_Straight((_float)TimeDelta, m_pNavigationCom);
		}
	}
	else  if (KEY_AWAY(KEY::UP))
	{
		if (!m_bAttack)
		{
			m_pMonFSM->Transit_MotionState(CMonFSM::IDLE1, m_pModelCom);
		}
	}

	else  if (KEY_TAB(KEY::A))
	{
		if (m_bCanAttack)
		{
			if (true == Do_Skill(m_normalSkillType2, CMonFSM::ATK_NORMAL, L"Layer_PlayerSkill"))
			{
				m_bCanAttack = false;
				m_AttackCoolTimeAcc = 0.0;
			}
		}
	}

	else  if (KEY_TAB(KEY::W))
	{
		if (m_bCanAttack)
		{
			if (Do_Skill(m_PokemonDesc.m_normalSkillType, CMonFSM::ATK_NORMAL, L"Layer_PlayerSkill"))
			{
				m_bCanAttack = false;
				m_AttackCoolTimeAcc = 0.0;
			}
		}
	}

	else  if (KEY_TAB(KEY::S))
	{
		if (m_bCanSkillAttack)
		{
			if (Do_Skill(m_PokemonDesc.m_skillIDs[0], CMonFSM::DASH_SLE_START, L"Layer_PlayerSkill"))
			{
				m_SkillLoopCount = 1;
				m_SkillLoopDelay = 1.f;
				m_fAccel = 1.f;
				m_SkillLoopDesc.m_CurskillIndex = 0;
				SkillCoolTime_Start();
			}
		}
	}

	else  if (KEY_TAB(KEY::D))
	{
		if (m_bCanSkillAttack)
		{
			if (Do_Skill(m_PokemonDesc.m_skillIDs[1], CMonFSM::HAPPY, L"Layer_PlayerSkill"))
			{
				m_SkillLoopDesc.m_CurskillIndex = 1;
				SkillCoolTime_Start();
			}
		}
	}
}

HRESULT CPlayer::Add_TransitionRandomState()
{
	return S_OK;
}

_uint CPlayer::State_Tick(const _double& TimeDelta)
{
	return _uint();
}

