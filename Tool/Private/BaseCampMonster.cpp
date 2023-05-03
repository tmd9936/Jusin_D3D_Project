#include "stdafx.h"
#include "BaseCampMonster.h"

#include "GameInstance.h"

#include "Utility.h"

#include "Searcher.h"

#include "HpBar.h"


CBaseCampMonster::CBaseCampMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CBaseCampMonster::CBaseCampMonster(const CMonster& rhs)
	: CMonster(rhs)
{
}

HRESULT CBaseCampMonster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBaseCampMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	Init_RandomMotionChangeDelay();

	//m_pTransformCom->Set_PositionY(0.5f);

	//m_pTransformCom->Set_PositinoX(_float(rand() % 5 + 17.5f));
	//m_pTransformCom->Set_PositionZ(_float(rand() % 5 + 17.5f));

	m_pHpBar->Set_RenderId(RENDER_END);
	return S_OK;
}

HRESULT CBaseCampMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	Init_RandomMotionChangeDelay();

	/*m_pTransformCom->Set_PositionY(0.5f);
	m_pTransformCom->Set_PositinoX(_float(rand() % 5 + 20.5f));
	m_pTransformCom->Set_PositionZ(_float(rand() % 5 + 20.5f));*/

	m_pHpBar->Set_RenderId(RENDER_END);

	return S_OK;
}

_uint CBaseCampMonster::Tick(_double TimeDelta)
{
	//if (m_bDead)
	//{
	//	m_pSearcher->Set_Dead();
	//	return OBJ_DEAD;
	//}

	//if (m_pHPCom->Get_CurrentHp() <= 0.f 
	//	&& m_pMonFSM->Get_MotionState() != CMonFSM::DEAD_ROTATE)
	//{
	//	m_pMonFSM->Transit_MotionState(CMonFSM::DEAD_ROTATE, m_pModelCom);
	//}

	//if (m_pMonFSM->Get_MotionState() != CMonFSM::DEAD_ROTATE)
	//{
	//	m_pMonFSM->Update_Component((_float)TimeDelta, m_pModelCom);
	//}
	return __super::Tick(TimeDelta);
}

_uint CBaseCampMonster::LateTick(_double TimeDelta)
{
	return __super::LateTick(TimeDelta);
}

HRESULT CBaseCampMonster::Render()
{
	return __super::Render();
}

void CBaseCampMonster::Change_State_FSM(_uint eState)
{
	switch (eState)
	{
	case CMonFSM::IDLE1:
		break;

	case CMonFSM::IDLE_FLOAT:
		m_bTurn = true;
		Set_MovePosition();
		break;

	case CMonFSM::RUN_FLOAT:
		m_bTurn = true;
		Set_MovePosition();
		break;

	case CMonFSM::IDLE_GROUND:
		m_bTurn = true;
		Set_MovePosition();
		break;

	//case CMonFSM::ROTATE_LOOP:
	//	m_bTurn = true;
	//	Set_MovePosition();
	//	break;

	default:
		break;
	}
}

void CBaseCampMonster::Go_To_RandomPosition(const _double& TimeDelta)
{
	//if (m_bTurn)
	//{
	//	if (m_pTransformCom->TurnToTarget({ 0.f, 1.f, 0.f, 0.f }, XMLoadFloat4(&m_MovePosition), (_float)TimeDelta))
	//	{
	//		m_bTurn = false;
	//	}
	//}
	//else
	//{
		Check_Do_Change_RandomMotion(TimeDelta);
		if (m_pTransformCom->Chase(XMLoadFloat4(&m_MovePosition), (_float)TimeDelta, 0.2f, m_pNavigationCom))
		{
			MotionChange_Random();
			Init_RandomMotionChangeDelay();
		}
	//}
	
}

_bool CBaseCampMonster::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	return _bool();
}

_bool CBaseCampMonster::Load_By_JsonFile_Impl(Document& doc)
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
		m_PokemonDesc.m_maxExp = PokemonDesc["m_maxExp"].GetInt();

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
			desc.m_value = m_stones[i]["m_value"].GetUint();
			desc.m_stoneType = (STONE_TYPE)m_stones[i]["m_stoneType"].GetUint();

			m_PokemonDesc.m_stones.push_back(move(desc));
		}
	}

	return true;
}

void CBaseCampMonster::Init_RandomMotionChangeDelay()
{
	m_MotionChangeDelay = _float(rand() % 3 + 2);
	m_CurMotionChangeDelayTime = 0.f;
}

HRESULT CBaseCampMonster::Add_TransitionRandomState()
{
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::IDLE1);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::IDLE_FLOAT);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::RUN_FLOAT);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::IDLE_GROUND);
	//m_pMonFSM->Add_RandomTransitionState(CMonFSM::ROTATE_LOOP);

	return S_OK;
}

_uint CBaseCampMonster::State_Tick(const _double& TimeDelta)
{
	switch (m_pMonFSM->Get_MotionState())
	{
	case CMonFSM::IDLE1:
		m_pModelCom->Play_Animation(TimeDelta);
		Check_Do_Change_RandomMotion(TimeDelta);
		break;

	case CMonFSM::IDLE_FLOAT:
		m_pModelCom->Play_Animation(TimeDelta);
		Go_To_RandomPosition(TimeDelta);
		break;

	case CMonFSM::RUN_FLOAT:
		m_pModelCom->Play_Animation(TimeDelta);
		Go_To_RandomPosition(TimeDelta);
		break;

	case CMonFSM::IDLE_GROUND:
		m_pModelCom->Play_Animation(TimeDelta);
		Go_To_RandomPosition(TimeDelta);
		break;

	case CMonFSM::DEAD_ROTATE:
		m_bHitState = true;
		if (m_pModelCom->Play_Animation(TimeDelta, false))
		{
			Set_Dead();
		}
		break;

	//case CMonFSM::ROTATE_LOOP:
	//	Go_To_RandomPosition(TimeDelta);
	//	Check_Do_Change_RandomMotion(TimeDelta);
	//	break;

	default:
		break;
	}

	return 0;
}

void CBaseCampMonster::Check_Do_Change_RandomMotion(const _double& TimeDelta)
{
	if (m_MotionChangeDelay < m_CurMotionChangeDelayTime)
	{
		MotionChange_Random();
		Init_RandomMotionChangeDelay();
	}

	m_CurMotionChangeDelayTime += _float(TimeDelta);
}

void CBaseCampMonster::MotionChange_Random()
{
	m_pMonFSM->Get_RandomState(m_pModelCom);
}

CBaseCampMonster* CBaseCampMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBaseCampMonster* pInstance = new CBaseCampMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CBaseCampMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBaseCampMonster::Clone(const _tchar * pLayerTag, _uint iLevelIndex, void* pArg)
{
	CBaseCampMonster* pInstance = new CBaseCampMonster(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CBaseCampMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBaseCampMonster::Clone(const _tchar * pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CBaseCampMonster* pInstance = new CBaseCampMonster(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CBaseCampMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaseCampMonster::Free()
{
	__super::Free();
}
