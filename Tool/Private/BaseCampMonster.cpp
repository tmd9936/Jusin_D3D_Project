#include "stdafx.h"
#include "BaseCampMonster.h"

#include "GameInstance.h"


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

	return S_OK;
}

HRESULT CBaseCampMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	Init_RandomMotionChangeDelay();

	return S_OK;
}

_uint CBaseCampMonster::Tick(_double TimeDelta)
{
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
		Check_Do_Change_RandomMotion(TimeDelta);
		break;

	case CMonFSM::IDLE_FLOAT:
		Go_To_RandomPosition(TimeDelta);
		break;

	case CMonFSM::RUN_FLOAT:
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
