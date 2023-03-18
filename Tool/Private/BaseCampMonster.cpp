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
	return S_OK;
}

HRESULT CBaseCampMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	return S_OK;
}

HRESULT CBaseCampMonster::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	return S_OK;
}

_uint CBaseCampMonster::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CBaseCampMonster::LateTick(_double TimeDelta)
{
	return _uint();
}

HRESULT CBaseCampMonster::Render()
{
	return S_OK;
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
