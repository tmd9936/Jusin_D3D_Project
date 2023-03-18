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

	return S_OK;
}

HRESULT CBaseCampMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

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

HRESULT CBaseCampMonster::Add_TransitionState()
{
	return E_NOTIMPL;
}

_uint CBaseCampMonster::Change_State()
{
	return _uint();
}

_uint CBaseCampMonster::State_Tick(const _double& TimeDelta)
{
	return _uint();
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
