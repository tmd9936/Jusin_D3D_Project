#include "stdafx.h"
#include "StageMonster.h"

#include "GameInstance.h"

/*
1 스테이지
니드킹 지진, 메가톤 펀치
쥬쥬 얼다바람, 아쿠아링
니드리나 독침, 메가폰

2스테이지 
보스 레어코일
또가스 자폭, 오물폭탄
가디 불꽃세례

기본 공격 
공격 쿨타임
스킬 쿨타임
스킬 쿨타임에 따라서 랜덤 공격


*/

CStageMonster::CStageMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CStageMonster::CStageMonster(const CMonster& rhs)
	: CMonster(rhs)
{
}

HRESULT CStageMonster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	Init_RandomMotionChangeDelay();

	return S_OK;
}

HRESULT CStageMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	Init_RandomMotionChangeDelay();

	return S_OK;
}

_uint CStageMonster::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

_uint CStageMonster::LateTick(_double TimeDelta)
{
	return __super::LateTick(TimeDelta);
}

HRESULT CStageMonster::Render()
{
	return __super::Render();
}

void CStageMonster::Change_State_FSM(_uint eState)
{
	switch (eState)
	{
	case CMonFSM::IDLE1:
		break;

	case CMonFSM::RUN_GOUND2:
		m_bTurn = true;
		Set_MovePosition();
		break;

	case CMonFSM::RUN_GOUND4:
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

void CStageMonster::Go_To_RandomPosition(const _double& TimeDelta)
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

void CStageMonster::Init_RandomMotionChangeDelay()
{
	m_MotionChangeDelay = _float(rand() % 3 + 2);
	m_CurMotionChangeDelayTime = 0.f;
}

HRESULT CStageMonster::Add_TransitionRandomState()
{
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::IDLE1);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::RUN_GOUND2);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::RUN_GOUND4);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::IDLE_GROUND);
	//m_pMonFSM->Add_RandomTransitionState(CMonFSM::ROTATE_LOOP);

	return S_OK;
}

_uint CStageMonster::State_Tick(const _double& TimeDelta)
{
	switch (m_pMonFSM->Get_MotionState())
	{
	case CMonFSM::IDLE1:
		Check_Do_Change_RandomMotion(TimeDelta);
		break;

	case CMonFSM::RUN_GOUND2:
		Go_To_RandomPosition(TimeDelta);
		break;

	case CMonFSM::RUN_GOUND4:
		Go_To_RandomPosition(TimeDelta);
		break;

	case CMonFSM::IDLE_GROUND:
		Go_To_RandomPosition(TimeDelta);
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

void CStageMonster::Check_Do_Change_RandomMotion(const _double& TimeDelta)
{
	if (m_MotionChangeDelay < m_CurMotionChangeDelayTime)
	{
		MotionChange_Random();
		Init_RandomMotionChangeDelay();
	}

	m_CurMotionChangeDelayTime += _float(TimeDelta);
}

void CStageMonster::MotionChange_Random()
{
	m_pMonFSM->Get_RandomState(m_pModelCom);
}

CStageMonster* CStageMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageMonster* pInstance = new CStageMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageMonster::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageMonster* pInstance = new CStageMonster(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStageMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageMonster::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStageMonster* pInstance = new CStageMonster(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStageMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageMonster::Free()
{
	__super::Free();
}
