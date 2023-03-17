#include "MonFSM.h"

#include "GameInstance.h"

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

_int CMonFSM::Update_Component(const _float& fTimeDelta)
{
	return _int();
}

HRESULT CMonFSM::Add_TransitionState(MOTION_STATE eState)
{
	m_vecState.push_back(eState);

	return S_OK;
}

HRESULT CMonFSM::Add_MotionState(MOTION_STATE eState, _uint index)
{
	if (eState > END_MOTION ) return E_FAIL;

	m_mapMotionState.insert({ eState, index });

	return S_OK;
}

HRESULT CMonFSM::Transit_MotionState(MOTION_STATE eState)
{
	if (eState < 0 || eState > END_MOTION || m_eCurrentMotion == eState)
		return E_FAIL;

	m_ePrevMotion = m_eCurrentMotion;
	m_eCurrentMotion = eState;

	return S_OK;
}


void CMonFSM::Get_RandomState(void)
{
	_int num = _int(m_vecState.size());

	_int i = rand() % num;

	Transit_MotionState(CMonFSM::MOTION_STATE(m_vecState[i]));
}

_uint CMonFSM::Find_MotionState(MOTION_STATE eState)
{
	auto iter = m_mapMotionState.find(eState);

	if (iter == m_mapMotionState.end())
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

	m_mapMotionState.clear();
}
