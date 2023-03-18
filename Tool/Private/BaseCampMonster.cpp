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

	m_MotionChangeDelay = _float(rand() % 3 + 2);

	return S_OK;
}

HRESULT CBaseCampMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	m_MotionChangeDelay = _float(rand() % 3 + 2);

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

HRESULT CBaseCampMonster::Add_TransitionRandomState()
{
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::IDLE1);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::RUN_GOUND2);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::RUN_GOUND4);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::RUN_NO);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::IDLE_GROUND);
	m_pMonFSM->Add_RandomTransitionState(CMonFSM::ROTATE_LOOP);

	return S_OK;
}

HRESULT CBaseCampMonster::Set_ChangeStates()
{

	return S_OK;
}

_uint CBaseCampMonster::State_Tick(const _double& TimeDelta)
{
	switch (m_pMonFSM->Get_MotionState())
	{
	case CMonFSM::IDLE1:
		break;

	case CMonFSM::RUN_GOUND2:
		break;

	case CMonFSM::RUN_GOUND4:
		break;

	case CMonFSM::RUN_NO:
		break;

	case CMonFSM::IDLE_GROUND:
		break;

	case CMonFSM::ROTATE_LOOP:
		break;

	default:
		break;
	}

	return 0;
}

void CBaseCampMonster::MotionChange_Random()
{
	m_pMonFSM->Get_RandomState();
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
