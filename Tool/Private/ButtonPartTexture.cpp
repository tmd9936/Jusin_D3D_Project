#include "stdafx.h"
#include "ButtonPartTexture.h"

#include "GameInstance.h"

CButtonPartTexture::CButtonPartTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CButtonPartTexture::CButtonPartTexture(const CButtonPartTexture& rhs)
	: CGameObject(rhs)
{
}

HRESULT CButtonPartTexture::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButtonPartTexture::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (pArg != nullptr)
		memcpy(&m_UIDesc, pArg, (sizeof m_UIDesc) + 2);

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_UI;

	m_pTransformCom->Set_Scaled({ m_UIDesc.m_fSizeX, m_UIDesc.m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(m_UIDesc.m_fX, m_UIDesc.m_fY, 0.01f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

_uint CButtonPartTexture::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CButtonPartTexture::LateTick(_double TimeDelta)
{
	if (m_UIDesc.pParent && m_UIDesc.pParentModel)
	{
		XMStoreFloat4x4(&m_FinalWorldMatrix, XMMatrixIdentity());

		// 부모의 스케일을 제거하고 부모의 변환 행렬 가져와서 얘의 부모기준 위치를 같이 변경시켜주면 될듯 한데 .. 
		_float4x4 parentMat{};
		XMStoreFloat4x4(&parentMat, m_UIDesc.pParent->Get_WorldMatrix_Matrix());

		_matrix parent = m_UIDesc.pParent->Get_WorldMatrix_Matrix();

		parent.r[0] = XMVector3Normalize(parent.r[0]);
		parent.r[1] = XMVector3Normalize(parent.r[1]);
		parent.r[2] = XMVector3Normalize(parent.r[2]);

		_float4x4 vParentCombinedMatrix = m_UIDesc.pParentModel->Get_CombinedTransformationMatrix_float4_4(1);

		_float3 vScale = m_pTransformCom->Get_Scaled();
		_float4 r = { vScale.x, 0.f, 0.f, 0.f };
		_float4 u = { 0.f, vScale.y, 0.f, 0.f };
		_float4 l = { 0.f, 0.f, 1.f, 0.f };
		_float4 p = { m_UIDesc.m_fX , m_UIDesc.m_fY , 0.f, 1.f };

		memcpy(parentMat.m[0], &r, sizeof _float4);
		memcpy(parentMat.m[1], &u, sizeof _float4);
		memcpy(parentMat.m[2], &l, sizeof _float4);
		memcpy(parentMat.m[3], &p, sizeof _float4);

		XMStoreFloat4x4(&m_FinalWorldMatrix, XMLoadFloat4x4(&parentMat) * XMMatrixScaling(vParentCombinedMatrix.m[0][0], vParentCombinedMatrix.m[1][1], 1.f) * parent);

	}

	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CButtonPartTexture::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CButtonPartTexture::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(pGameInstance->Add_Component(CVIBuffer_Rect::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		(CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, m_UIDesc.m_TextureProtoTypeLevel, m_UIDesc.m_TextureProtoTypeName,
		(CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CButtonPartTexture::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_FinalWorldMatrix)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CButtonPartTexture* CButtonPartTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CButtonPartTexture* pInstance = new CButtonPartTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CButtonPartTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButtonPartTexture::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CButtonPartTexture* pInstance = new CButtonPartTexture(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CButtonPartTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButtonPartTexture::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);

}
