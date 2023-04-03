#include "stdafx.h"
#include "StageEnemyMonster.h"

#include "GameInstance.h"

/*
1 ��������
�ϵ�ŷ ����, �ް��� ��ġ
���� ��ٹٶ�, ����Ƹ�
�ϵ帮�� ��ħ, �ް���

2�������� 
���� ��������
�ǰ��� ����, ������ź
���� �Ҳɼ���

�⺻ ���� 
���� ��Ÿ��
��ų ��Ÿ��
��ų ��Ÿ�ӿ� ���� ���� ����

���
�⺻����
���� ��Ÿ�� ��
��Ÿ�� ������ ����
���� Ÿ�� or �Ÿ� ���� Ÿ��?

�����̶� ������ �ൿ �ð� ����
�־����� ����
of
��������� ������

�� ���� ����� �Ʊ� ���� �� ī�޶� �����


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
		break;

	case CMonFSM::ROAR:
		//m_bTurn = true;
		//Set_MovePosition();
		// ī�޶� ���̸� ����
		break;

	case CMonFSM::RUN_GOUND2:
		m_bTurn = true;
		Set_MovePosition();
		break;

	case CMonFSM::IDLE_GROUND:
		m_bTurn = true;
		Set_MovePosition();
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
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::IDLE1);
	//m_pMonFSM->Add_RandomTransitionState(CMonFSM::ROAR);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::ATK_NORMAL);
	//m_pMonFSM->Add_RandomTransitionState(CMonFSM::);
	//m_pMonFSM->Add_RandomTransitionState(CMonFSM::ROTATE_LOOP);

	
	// m_pMonFSM->Add_MotionState(CMonFSM::MONSTER_STATE(i), i);

	return S_OK;
}

_uint CStageEnemyMonster::State_Tick(const _double& TimeDelta)
{
	switch (m_pMonFSM->Get_MotionState())
	{
	case CMonFSM::IDLE1:
		Check_Do_Change_RandomMotion(TimeDelta);
		break;

	case CMonFSM::ROAR:
		Go_To_RandomPosition(TimeDelta);
		break;

	case CMonFSM::ATK_NORMAL:
		Go_To_RandomPosition(TimeDelta);
		break;

	case CMonFSM::RUN_GOUND2:
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
