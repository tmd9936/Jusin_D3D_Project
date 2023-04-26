#include"stdafx.h"

#include "Stone.h"

#include "GameInstance.h"

CStone::CStone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CStone::CStone(const CStone& rhs)
	: CUI(rhs)
{
}

HRESULT CStone::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStone::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStone::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CStone::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);

	Change_State();

	return __super::Tick(TimeDelta);
}

_uint CStone::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	for (auto& part : m_TextureParts)
	{
		part->LateTick(TimeDelta);
	}

	for (auto& part : m_TextParts)
	{
		part->LateTick(TimeDelta);
	}


	return _uint();
}

HRESULT CStone::Render()
{
	return __super::Render();
}

void CStone::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case STATE_IN_INVENTORY_NO_EQUIP:
			break;
		case STATE_IN_INVENTORY_EQUIP:
			break;
		case STATE_PICKING:
			break;
		case STATE_IN_EQUIPINFO:
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CStone::State_Tick(const _double& TimeDelta)
{
	switch (m_eCurState)
	{
	case STATE_IN_INVENTORY_NO_EQUIP:
		break;
	case STATE_IN_INVENTORY_EQUIP:
		break;
	case STATE_PICKING:
		break;
	case STATE_IN_EQUIPINFO:
		break;
	}
}

CStone* CStone::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStone* pInstance = new CStone(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStone::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStone* pInstance = new CStone(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))

	{
		MSG_BOX("Failed to Cloned CStone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStone::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStone* pInstance = new CStone(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStone::Free()
{
	__super::Free();
}
