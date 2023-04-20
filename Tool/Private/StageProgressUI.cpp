#include"stdafx.h"

#include "StageProgressUI.h"

#include "GameInstance.h"

#include "Stage_Manager.h"

CStageProgressUI::CStageProgressUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CStageProgressUI::CStageProgressUI(const CStageProgressUI& rhs)
	: CUI(rhs)
{
}

HRESULT CStageProgressUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageProgressUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_desc = (*(STAGE_PROGRESS_UI_DESC*)pArg);
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	//m_waveInfo.resize(m_desc.m_waveCount);

	//if (FAILED(Insert_In_Stage_Manager()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CStageProgressUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	m_waveInfo.resize(m_desc.m_waveCount);

	//if (FAILED(Insert_In_Stage_Manager()))
	//	return E_FAIL;

	return S_OK;
}

_uint CStageProgressUI::Tick(_double TimeDelta)
{

	return __super::Tick(TimeDelta);
}

_uint CStageProgressUI::LateTick(_double TimeDelta)
{
	//m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

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


HRESULT CStageProgressUI::Insert_In_Stage_Manager()
{
	CGameObject* pObject = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Manager", L"Stage_Manager");
	if (nullptr == pObject)
		return E_FAIL;

	CStage_Manager* pStageManager = dynamic_cast<CStage_Manager*>(pObject);
	if (nullptr == pStageManager)
		return E_FAIL;

	//pStageManager->Set_StageMessageInfo(this);

	return S_OK;
}

CStageProgressUI* CStageProgressUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageProgressUI* pInstance = new CStageProgressUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageProgressUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageProgressUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageProgressUI* pInstance = new CStageProgressUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStageProgressUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageProgressUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStageProgressUI* pInstance = new CStageProgressUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStageProgressUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageProgressUI::Free()
{
	__super::Free();
}
