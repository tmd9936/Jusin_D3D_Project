#include"stdafx.h"

#include "StageClearUI.h"

#include "GameInstance.h"

#include "Stage_Manager.h"

CStageClearUI::CStageClearUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CStageClearUI::CStageClearUI(const CStageClearUI& rhs)
	: CUI(rhs)
{
}

HRESULT CStageClearUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageClearUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	m_MaxSizeY = m_UIDesc.m_fSizeY;

	if (FAILED(Insert_In_Stage_Manager()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageClearUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	m_MaxSizeY = m_UIDesc.m_fSizeY;

	if (m_TextParts.empty())
		return E_FAIL;

	m_textPositionX = m_TextParts.at(0)->Get_PositionX();
	m_message = m_TextParts.at(0)->Get_Text();

	if (FAILED(Insert_In_Stage_Manager()))
		return E_FAIL;

	return S_OK;
}

_uint CStageClearUI::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);

	Change_State();

	return __super::Tick(TimeDelta);
}

_uint CStageClearUI::LateTick(_double TimeDelta)
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

HRESULT CStageClearUI::Render()
{
	return __super::Render();
}

void CStageClearUI::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case STATE_OPENING:
			m_eRenderId = RENDER_UI;
			Set_TextOnParts(L"");
			break;
		case STATE_STAY:
			m_StayTimeAcc = 0.0;
			Set_TextOnParts(m_message);
			break;
		case STATE_CLOSING:
			Set_TextOnParts(L"");
			break;
		case STATE_NO_RENDER:
			Set_TextOnParts(L"");
			m_eRenderId = RENDER_END;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CStageClearUI::State_Tick(const _double& TimeDelta)
{
	switch (m_eCurState)
	{
	case STATE_OPENING:
		State_Tick_Opening(TimeDelta);
		break;
	case STATE_STAY:
		Stay_Time_Check(TimeDelta);
		break;
	case STATE_CLOSING:
		State_Tick_Closing(TimeDelta);
		break;
	case STATE_NO_RENDER:
		break;
	}
}

void CStageClearUI::Stay_Time_Check(const _double& TimeDelta)
{
	if (m_StayTimeAcc >= m_StayTime)
	{
		m_eCurState = STATE_CLOSING;
	}

	m_StayTimeAcc += TimeDelta;
}

void CStageClearUI::State_Tick_Opening(const _double& TimeDelta)
{
	m_CurSizeY += _float(TimeDelta * m_MaxSizeY * 2.f);
	if (m_CurSizeY >= m_MaxSizeY)
	{
		m_CurSizeY = m_MaxSizeY;
		m_eCurState = STATE_STAY;
	}
	m_pTransformCom->Set_ScaledY(m_CurSizeY);
}

void CStageClearUI::State_Tick_Closing(const _double& TimeDelta)
{
	m_CurSizeY -= _float(TimeDelta * m_MaxSizeY * 2.f);
	if (m_CurSizeY <= 0.03f)
	{
		m_CurSizeY = 0.03f;
		m_eCurState = STATE_NO_RENDER;
	}
	m_pTransformCom->Set_ScaledY(m_CurSizeY);
}

void CStageClearUI::Set_TextOnParts(const wstring& message)
{
	if (m_TextParts.empty())
		return;

	m_TextParts.at(0)->Set_Text(message, m_textPositionX);
}

HRESULT CStageClearUI::Insert_In_Stage_Manager()
{
	CGameObject* pObject = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Manager", L"Stage_Manager");
	if (nullptr == pObject)
		return E_FAIL;

	CStage_Manager* pStageManager = dynamic_cast<CStage_Manager*>(pObject);
	if (nullptr == pStageManager)
		return E_FAIL;

	pStageManager->Set_StageClearUI(this);

	return S_OK;
}

CStageClearUI* CStageClearUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageClearUI* pInstance = new CStageClearUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageClearUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageClearUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageClearUI* pInstance = new CStageClearUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))

	{
		MSG_BOX("Failed to Cloned CStageClearUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageClearUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStageClearUI* pInstance = new CStageClearUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStageClearUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageClearUI::Free()
{
	__super::Free();
}
