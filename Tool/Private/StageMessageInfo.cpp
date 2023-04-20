#include"stdafx.h"

#include "StageMessageInfo.h"

#include "GameInstance.h"

#include "Stage_Manager.h"

CStageMessageInfo::CStageMessageInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CStageMessageInfo::CStageMessageInfo(const CStageMessageInfo& rhs)
	: CUI(rhs)
{
}

HRESULT CStageMessageInfo::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageMessageInfo::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	m_MaxSizeY = m_UIDesc.m_fSizeY;

	if (FAILED(Insert_In_Stage_Manager()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageMessageInfo::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	m_MaxSizeY = m_UIDesc.m_fSizeY;

	if (m_TextParts.empty())
		return E_FAIL;
	m_message = m_TextParts.at(0)->Get_Text();

	if (FAILED(Insert_In_Stage_Manager()))
		return E_FAIL;

	return S_OK;
}

_uint CStageMessageInfo::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);

	Change_State();

	return __super::Tick(TimeDelta);
}

_uint CStageMessageInfo::LateTick(_double TimeDelta)
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

HRESULT CStageMessageInfo::Render()
{
	return __super::Render();
}

void CStageMessageInfo::Change_State()
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
			m_eRenderId = RENDER_END;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CStageMessageInfo::State_Tick(const _double& TimeDelta)
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

void CStageMessageInfo::Stay_Time_Check(const _double& TimeDelta)
{
	if (m_StayTimeAcc >= m_StayTime)
	{
		m_eCurState = STATE_CLOSING;
	}

	m_StayTimeAcc += TimeDelta;
}

void CStageMessageInfo::State_Tick_Opening(const _double& TimeDelta)
{
	m_CurSizeY += _float(TimeDelta * 95.0);
	if (m_CurSizeY >= m_MaxSizeY)
	{
		m_CurSizeY = m_MaxSizeY;
		m_eCurState = STATE_STAY;
	}
	m_pTransformCom->Set_ScaledY(m_CurSizeY);
}

void CStageMessageInfo::State_Tick_Closing(const _double& TimeDelta)
{
	m_CurSizeY -= _float(TimeDelta * 95.0);
	if (m_CurSizeY <= 5.f)
	{
		m_CurSizeY = 5.f;
		m_eCurState = STATE_NO_RENDER;
	}
	m_pTransformCom->Set_ScaledY(m_CurSizeY);
}

void CStageMessageInfo::Set_TextOnParts(const wstring& message)
{
	if (m_TextParts.empty())
		return;
	m_TextParts.at(0)->Set_Text(message);
}

HRESULT CStageMessageInfo::Insert_In_Stage_Manager()
{
	CGameObject* pObject = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Manager", L"Stage_Manager");
	if (nullptr == pObject)
		return E_FAIL;

	CStage_Manager* pStageManager = dynamic_cast<CStage_Manager*>(pObject);
	if (nullptr == pStageManager)
		return E_FAIL;

	pStageManager->Set_StageMessageInfo(this);

	return S_OK;
}

CStageMessageInfo* CStageMessageInfo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageMessageInfo* pInstance = new CStageMessageInfo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageMessageInfo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageMessageInfo::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageMessageInfo* pInstance = new CStageMessageInfo(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStageMessageInfo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageMessageInfo::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStageMessageInfo* pInstance = new CStageMessageInfo(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStageMessageInfo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageMessageInfo::Free()
{
	__super::Free();
}
