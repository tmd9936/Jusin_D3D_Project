#include"stdafx.h"

#include "Food.h"

#include "GameInstance.h"

CFood::CFood(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CFood::CFood(const CFood& rhs)
	: CUI(rhs)
{
}

HRESULT CFood::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFood::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_Desc = (*(FOOD_DESC*)pArg);

		switch (m_Desc.m_foodType)
		{
		case TYPE_BLUE:
			lstrcpy(m_Desc.m_UIDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_Item_Blue_UC");
			break;
		case TYPE_GREY:
			lstrcpy(m_Desc.m_UIDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_Item_Grey_UC");
			break;
		case TYPE_RED:
			lstrcpy(m_Desc.m_UIDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_Item_Red_UC");
			break;
		case TYPE_YELLOW:
			lstrcpy(m_Desc.m_UIDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_Item_Yellow_UC");
			break;
		}
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, &m_Desc.m_UIDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFood::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CFood::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);

	Change_State();

	return __super::Tick(TimeDelta);
}

_uint CFood::LateTick(_double TimeDelta)
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

HRESULT CFood::Render()
{
	return __super::Render();
}

void CFood::Change_State()
{
	if (m_Desc.m_ePreState != m_Desc.m_eCurState)
	{
		switch (m_Desc.m_eCurState)
		{
		case STATE_PICKING:
			break;
		case STATE_FOODBOX:
			break;
		case STATE_STAGE_VIEW:
			break;
		case STATE_NO_SHOW:
			break;
		}

		m_Desc.m_ePreState = m_Desc.m_eCurState;
	}
}

void CFood::State_Tick(const _double& TimeDelta)
{
	switch (m_Desc.m_eCurState)
	{
	case STATE_PICKING:
		break;
	case STATE_FOODBOX:
		break;
	case STATE_STAGE_VIEW:
		break;
	case STATE_NO_SHOW:
		break;
	}
}

CFood* CFood::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFood* pInstance = new CFood(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CFood");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFood::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CFood* pInstance = new CFood(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))

	{
		MSG_BOX("Failed to Cloned CFood");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFood::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CFood* pInstance = new CFood(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CFood");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFood::Free()
{
	__super::Free();
}
