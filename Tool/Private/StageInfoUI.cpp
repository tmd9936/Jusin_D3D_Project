#include"stdafx.h"

#include "StageInfoUI.h"

CStageInfoUI::CStageInfoUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CStageInfoUI::CStageInfoUI(const CStageInfoUI& rhs)
	: CUI(rhs)
{
}

HRESULT CStageInfoUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageInfoUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	return S_OK;
}

_uint CStageInfoUI::Tick(_double TimeDelta)
{
	return __super::Tick(TimeDelta);
}

_uint CStageInfoUI::LateTick(_double TimeDelta)
{
	return __super::LateTick(TimeDelta);
}

HRESULT CStageInfoUI::Render()
{
	return __super::Render();
}

void CStageInfoUI::Change_Stage_Info(STAGE_INFO_DESC desc)
{
	if (nullptr == m_pTextureCom)
		return;
	
	m_UIDesc.m_vColor = desc.m_vBaseColor;
	m_TextureNumber = desc.m_StageNumber;
	m_TextParts[0]->Set_Text(to_wstring(desc.m_StageNumber) + L". " + desc.m_vStageNameText);

}

void CStageInfoUI::Change_Stage_Info(_float4 vBaseColor, _uint stageNumber, wstring nameText)
{
	if (nullptr == m_pTextureCom)
		return;

	m_UIDesc.m_vColor = vBaseColor;
	m_TextureParts[0]->Set_TextureNumber(stageNumber);
	m_TextParts[0]->Set_Text(to_wstring(stageNumber) + L". " + nameText);
}

CStageInfoUI* CStageInfoUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageInfoUI* pInstance = new CStageInfoUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageInfoUI::Clone(const _tchar * pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageInfoUI* pInstance = new CStageInfoUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStageInfoUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageInfoUI::Free()
{
	__super::Free();
}
