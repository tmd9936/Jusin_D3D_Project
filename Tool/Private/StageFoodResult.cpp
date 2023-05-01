#include"stdafx.h"

#include "StageFoodResult.h"

#include "GameInstance.h"

#include "Stage_Manager.h"

#include "FoodInfoUI.h"

CStageFoodResult::CStageFoodResult(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CStageFoodResult::CStageFoodResult(const CStageFoodResult& rhs)
	: CUI(rhs)
{
}

HRESULT CStageFoodResult::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageFoodResult::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Init_FoodInfoUIs()))
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

	m_FoodInfoUIs.reserve(4);

	return S_OK;
}

HRESULT CStageFoodResult::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (FAILED(Init_FoodInfoUIs()))
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

	m_FoodInfoUIs.reserve(4);

	return S_OK;
}

_uint CStageFoodResult::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);

	Change_State();

	return __super::Tick(TimeDelta);
}

_uint CStageFoodResult::LateTick(_double TimeDelta)
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

HRESULT CStageFoodResult::Render()
{
	return __super::Render();
}

void CStageFoodResult::OpenUI(const vector<_uint>& foods)
{
	for (size_t i = 0; i < CFood::TYPE_END; ++i)
	{

	}
	m_eCurState = STATE_OPEN;
}

HRESULT CStageFoodResult::Init_FoodInfoUIs()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_STAGE, L"Layer_StageResultUI", L"FoodInfoUI01");
	if (nullptr == pObject)
		return E_FAIL;
	CFoodInfoUI* m_pFoodInfoUI01 = dynamic_cast<CFoodInfoUI*>(pObject);
	if (nullptr == m_pFoodInfoUI01)
		return E_FAIL;
	Safe_AddRef(m_pFoodInfoUI01);
	m_pFoodInfoUI01->Set_RenderId(RENDER_END);
	m_FoodInfoUIs.push_back(m_pFoodInfoUI01);

	pObject = pGameInstance->Get_Object(LEVEL_STAGE, L"Layer_StageResultUI", L"FoodInfoUI02");
	if (nullptr == pObject)
		return E_FAIL;
	CFoodInfoUI* m_pFoodInfoUI02 = dynamic_cast<CFoodInfoUI*>(pObject);
	if (nullptr == m_pFoodInfoUI02)
		return E_FAIL;
	Safe_AddRef(m_pFoodInfoUI02);
	m_pFoodInfoUI02->Set_RenderId(RENDER_END);
	m_FoodInfoUIs.push_back(m_pFoodInfoUI02);

	pObject = pGameInstance->Get_Object(LEVEL_STAGE, L"Layer_StageResultUI", L"FoodInfoUI03");
	if (nullptr == pObject)
		return E_FAIL;
	CFoodInfoUI* m_pFoodInfoUI03 = dynamic_cast<CFoodInfoUI*>(pObject);
	if (nullptr == m_pFoodInfoUI03)
		return E_FAIL;
	Safe_AddRef(m_pFoodInfoUI03);
	m_pFoodInfoUI03->Set_RenderId(RENDER_END);
	m_FoodInfoUIs.push_back(m_pFoodInfoUI03);

	pObject = pGameInstance->Get_Object(LEVEL_STAGE, L"Layer_StageResultUI", L"FoodInfoUI04");
	if (nullptr == pObject)
		return E_FAIL;
	CFoodInfoUI* m_pFoodInfoUI04 = dynamic_cast<CFoodInfoUI*>(pObject);
	if (nullptr == m_pFoodInfoUI04)
		return E_FAIL;
	Safe_AddRef(m_pFoodInfoUI04);
	m_pFoodInfoUI04->Set_RenderId(RENDER_END);
	m_FoodInfoUIs.push_back(m_pFoodInfoUI04);

	return S_OK;
}

void CStageFoodResult::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case STATE_OPEN:
			m_showTimeAcc = 0.0;
			break;
		case STATE_STAY:
			m_stayTimeAcc = 0.0;
			break;
		case STATE_CLOSE:
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CStageFoodResult::State_Tick(const _double& TimeDelta)
{
	switch (m_eCurState)
	{
	case STATE_OPEN:
		Open_Tick(TimeDelta);
		break;
	case STATE_STAY:
		Stay_Tick(TimeDelta);
		break;
	case STATE_CLOSE:
		break;
	}
}

void CStageFoodResult::Open_Tick(const _double& TimeDelta)
{
	if (m_curShowFoodInfoUI >= m_maxShowFoodInfoUI)
	{
		m_eCurState = STATE_STAY;
		return;
	}
	if (m_showTime <= m_showTimeAcc)
	{
		m_FoodInfoUIs.at(m_curShowFoodInfoUI++)->Set_RenderId(RENDER_UI);
		m_showTimeAcc = 0.0;
	}

	m_showTimeAcc += TimeDelta;
}

void CStageFoodResult::Stay_Tick(const _double& TimeDelta)
{
	if (m_stayTime <= m_stayTimeAcc)
	{
		m_eCurState = STATE_CLOSE;
	}

	m_stayTimeAcc += TimeDelta;
}

CStageFoodResult* CStageFoodResult::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageFoodResult* pInstance = new CStageFoodResult(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageFoodResult");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageFoodResult::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageFoodResult* pInstance = new CStageFoodResult(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStageFoodResult");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageFoodResult::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStageFoodResult* pInstance = new CStageFoodResult(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStageFoodResult");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageFoodResult::Free()
{
	__super::Free();

	for (auto& iter : m_FoodInfoUIs)
	{
		Safe_Release(iter);
	}

}
