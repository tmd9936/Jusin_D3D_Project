#include "MonFSM.h"

#include "GameInstance.h"
#include "GameObject.h"

CMonFSM::CMonFSM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext, pOwner)
	, m_eCurrentMotion(IDLE1)
	, m_ePrevMotion(END_MOTION)
{
}

CMonFSM::CMonFSM(const CMonFSM& rhs, CGameObject* pOwner)
	: CComponent(rhs, pOwner)
	, m_eCurrentMotion(rhs.m_eCurrentMotion)
	, m_ePrevMotion(rhs.m_ePrevMotion)
{
}


HRESULT CMonFSM::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonFSM::Initialize(void* pArg)
{
	return S_OK;
}

_int CMonFSM::Update_Component(const _float& fTimeDelta, CModel* pModel)
{
	if (nullptr == pModel)
		return 0;

	pModel->Play_Animation(fTimeDelta);

	if (m_eCurrentMotion == CMonFSM::END_MOTION)
		Get_RandomState();

	return _int();
}

HRESULT CMonFSM::Add_RandomTransitionState(MONSTER_STATE eState)
{
	m_RandomState.push_back(eState);

	return S_OK;
}

/* 상태이상에 따른 모션 각각 설정하기 */
HRESULT CMonFSM::Add_MotionState(MONSTER_STATE eState, _uint index)
{
	if (eState > END_MOTION ) return E_FAIL;

	m_MotionState.insert({ eState, index });

	return S_OK;
}

HRESULT CMonFSM::Transit_MotionState(MONSTER_STATE eState, CModel* pModel)
{
	if (eState < 0 || eState > END_MOTION || m_eCurrentMotion == eState)
		return E_FAIL;

	if (nullptr == pModel)
		return E_FAIL;

	auto iter = m_MotionState.find(eState);
	if (iter == m_MotionState.end())
		return E_FAIL;

	pModel->Set_Animation(iter->second);

	if (nullptr != m_pOwner)
	{
		m_pOwner->Change_State_FSM(eState);
	}

	m_ePrevMotion = m_eCurrentMotion;
	m_eCurrentMotion = eState;

	return S_OK;
}


void CMonFSM::Get_RandomState(CModel* pModel)
{
	_int num = _int(m_RandomState.size());

	_int i = rand() % num;

	Transit_MotionState(CMonFSM::MONSTER_STATE(m_RandomState[i]), pModel);
}

_uint CMonFSM::Find_MotionState(MONSTER_STATE eState)
{
	auto iter = m_MotionState.find(eState);

	if (iter == m_MotionState.end())
		return END_MOTION;

	return iter->second;
}


CMonFSM* CMonFSM::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonFSM* pInstance = new CMonFSM(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonFSM");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMonFSM::Clone(CGameObject* pOwner, void* pArg)
{
	CMonFSM* pInstance = new CMonFSM(*this, pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonFSM");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonFSM::Free()
{
	__super::Free();

	m_MotionState.clear();
}
