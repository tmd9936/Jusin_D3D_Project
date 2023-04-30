#include"stdafx.h"

#include "StageStoneResult.h"

#include "GameInstance.h"

#include "Stage_Manager.h"

#include "StoneInfoUI.h"

CStageStoneResult::CStageStoneResult(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CStageStoneResult::CStageStoneResult(const CStageStoneResult& rhs)
	: CUI(rhs)
{
}

HRESULT CStageStoneResult::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageStoneResult::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Init_StoneInfoUIs()))
		return E_FAIL;

	m_eRenderId = RENDER_BACK_UI;

	for (auto& part : m_TextureParts)
	{
		part->Set_RenderId(RENDER_BACK_UI);
	}

	for (auto& part : m_TextParts)
	{
		part->Set_RenderId(RENDER_BACK_UI);
	}

	return S_OK;
}

HRESULT CStageStoneResult::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (FAILED(Init_StoneInfoUIs()))
		return E_FAIL;

	m_eRenderId = RENDER_BACK_UI;

	for (auto& part : m_TextureParts)
	{
		part->Set_RenderId(RENDER_BACK_UI);
	}

	for (auto& part : m_TextParts)
	{
		part->Set_RenderId(RENDER_BACK_UI);
	}

	return S_OK;
}

_uint CStageStoneResult::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);

	Change_State();

	return __super::Tick(TimeDelta);
}

_uint CStageStoneResult::LateTick(_double TimeDelta)
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

HRESULT CStageStoneResult::Render()
{
	return __super::Render();
}

void CStageStoneResult::OpenUI()
{
	m_eCurState = STATE_OPEN;
}

HRESULT CStageStoneResult::Init_StoneInfoUIs()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_STAGE, L"Layer_StageResultUI", L"StoneInfoUI01");
	if (nullptr == pObject)
		return E_FAIL;
	m_pStoneInfoUI01 = dynamic_cast<CStoneInfoUI*>(pObject);
	if (nullptr == m_pStoneInfoUI01)
		return E_FAIL;
	
	CStone::STONE_DESC stoneDesc{};

	_int stoneTypeRandomValue = rand() % 2;
	if (stoneTypeRandomValue == 0)
	{
		stoneDesc.m_stoneType = CStone::TYPE_ATK;
		lstrcpy(stoneDesc.m_UIDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_window_ATK_icon");
	}
	else
	{
		stoneDesc.m_stoneType = CStone::TYPE_HP;
		lstrcpy(stoneDesc.m_UIDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_window_HP_icon");
	}

	_int stonePowerRandomValue = rand() % 150;
	stoneDesc.value = 150 + stonePowerRandomValue;
	stoneDesc.m_pokemonIconNumber = 25;
	stoneDesc.m_UIDesc.m_fSizeX = 30.f;
	stoneDesc.m_UIDesc.m_fSizeY = 30.f;
	stoneDesc.m_UIDesc.m_TextureProtoTypeLevel = LEVEL_STATIC;
	stoneDesc.m_UIDesc.m_UIType = 0;
	stoneDesc.m_UIDesc.m_ShaderPass = 0;

	m_pStoneInfoUI01->Change_StoneInfo(stoneDesc);

	Safe_AddRef(m_pStoneInfoUI01);

	//pObject = pGameInstance->Get_Object(LEVEL_STAGE, L"Layer_StageResultUI", L"StoneInfoUI02");
	//if (nullptr == pObject)
	//	return E_FAIL;
	//m_pStoneInfoUI02 = dynamic_cast<CStoneInfoUI*>(pObject);
	//if (nullptr == m_pStoneInfoUI02)
	//	return E_FAIL;
	//Safe_AddRef(m_pStoneInfoUI02);

	//pObject = pGameInstance->Get_Object(LEVEL_STAGE, L"Layer_StageResultUI", L"StoneInfoUI03");
	//if (nullptr == pObject)
	//	return E_FAIL;
	//m_pStoneInfoUI03 = dynamic_cast<CStoneInfoUI*>(pObject);
	//if (nullptr == m_pStoneInfoUI03)
	//	return E_FAIL;
	//Safe_AddRef(m_pStoneInfoUI03);

	return S_OK;
}

void CStageStoneResult::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case STATE_OPEN:
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CStageStoneResult::State_Tick(const _double& TimeDelta)
{
	switch (m_eCurState)
	{
	case STATE_OPEN:
		break;
	}
}

CStageStoneResult* CStageStoneResult::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageStoneResult* pInstance = new CStageStoneResult(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageStoneResult");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageStoneResult::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageStoneResult* pInstance = new CStageStoneResult(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStageStoneResult");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageStoneResult::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStageStoneResult* pInstance = new CStageStoneResult(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStageStoneResult");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageStoneResult::Free()
{
	__super::Free();

	Safe_Release(m_pStoneInfoUI01);
	Safe_Release(m_pStoneInfoUI02);
	Safe_Release(m_pStoneInfoUI03);
}
