#include "stdafx.h"
#include "PartText.h"

#include "GameInstance.h"

CPartText::CPartText(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CPartText::CPartText(const CPartText& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPartText::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPartText::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (pArg != nullptr)
	{
		m_Text_Desc.pParent = (*(TEXT_DESC*)(pArg)).pParent;
		m_Text_Desc.pParentModel = (*(TEXT_DESC*)(pArg)).pParentModel;
		m_Text_Desc.m_FontTag = (*(TEXT_DESC*)(pArg)).m_FontTag;
		m_Text_Desc.m_Text = (*(TEXT_DESC*)(pArg)).m_Text;
		m_Text_Desc.m_vColor = (*(TEXT_DESC*)(pArg)).m_vColor;
		m_Text_Desc.m_Rotation = (*(TEXT_DESC*)(pArg)).m_Rotation;
		m_Text_Desc.m_vRotationOrigin = (*(TEXT_DESC*)(pArg)).m_vRotationOrigin;
		m_Text_Desc.m_vScale = (*(TEXT_DESC*)(pArg)).m_vScale;
		m_Text_Desc.m_fX = (*(TEXT_DESC*)(pArg)).m_fX;
		m_Text_Desc.m_fY = (*(TEXT_DESC*)(pArg)).m_fY;
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_UI;

	return S_OK;
}

_uint CPartText::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CPartText::LateTick(_double TimeDelta)
{
	if (m_Text_Desc.pParent && m_Text_Desc.pParentModel)
	{
		_matrix parent = m_Text_Desc.pParent->Get_WorldMatrix_Matrix();
		REMOVE_SCALE(parent);

		_float4x4 vParentCombinedMatrix = m_Text_Desc.pParentModel->Get_CombinedTransformationMatrix_float4_4(1);

		XMStoreFloat4x4(&m_FinalWorldMatrix, XMMatrixSet(
			m_Text_Desc.m_vScale.x, 0.f, 0.f, 0.f,
			0.f, m_Text_Desc.m_vScale.y, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			m_Text_Desc.m_fX, -m_Text_Desc.m_fY, 0.f, 1.f
		));

		XMStoreFloat4x4(&m_FinalWorldMatrix, XMLoadFloat4x4(&m_FinalWorldMatrix) *
			XMMatrixScaling(vParentCombinedMatrix.m[0][0], vParentCombinedMatrix.m[1][1], 1.f) * parent);
	}

	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CPartText::Render()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	if (FAILED(pGameInstance->Render_Font(m_Text_Desc.m_FontTag.c_str(),
		m_Text_Desc.m_Text.c_str(),
		_float2(m_FinalWorldMatrix.m[3][0], m_FinalWorldMatrix.m[3][1]),
		XMLoadFloat4(&m_Text_Desc.m_vColor),
		m_Text_Desc.m_Rotation, m_Text_Desc.m_vRotationOrigin, _float2(m_FinalWorldMatrix.m[0][0], m_FinalWorldMatrix.m[1][1]))))
			return E_FAIL;

	return S_OK;
}

HRESULT CPartText::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	return S_OK;
}

HRESULT CPartText::SetUp_ShaderResources()
{

	return S_OK;
}

CPartText* CPartText::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPartText* pInstance = new CPartText(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CPartText");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPartText::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CPartText* pInstance = new CPartText(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CPartText");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPartText::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);

}
