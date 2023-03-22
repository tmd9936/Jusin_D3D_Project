#include "stdafx.h"
#include "HpBar.h"

#include "GameInstance.h"

CHpBar::CHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CHpBar::CHpBar(const CHpBar& rhs)
	: CGameObject(rhs)
{
}

HRESULT CHpBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHpBar::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	memcpy(&m_Desc, pArg, sizeof m_Desc);

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled({ m_Desc.m_fSizeX, m_Desc.m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(0.f, 10.f, 0.0f);

	//m_pTransformCom->Set_Scaled({ m_Desc.m_fSizeX, m_Desc.m_fSizeY, 1.f });

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_eRenderId = RENDER_UI;

	return S_OK;
}

_uint CHpBar::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CHpBar::LateTick(_double TimeDelta)
{
	_matrix		ViewPortMatrix = CGameInstance::GetInstance()->Get_ViewPort_Matrix(0, 0, g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	_matrix viewMatrix = pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW);
	_matrix projMatrix = pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);

	_float4x4 ParentMat{};
	XMStoreFloat4x4(&ParentMat, m_Desc.pParent->Get_WorldMatrix_Matrix());
	XMStoreFloat4x4(&ParentMat, XMLoadFloat4x4(&ParentMat) * viewMatrix * projMatrix * ViewPortMatrix);

	//_float ParentScaleXRatio = XMVectorGetX(XMVector3Length((XMLoadFloat4x4(&ParentMat).r[0]))) / g_iWinSizeX;
	//_float ParentScaleYRatio = XMVectorGetX(XMVector3Length((XMLoadFloat4x4(&ParentMat).r[1]))) / g_iWinSizeY;
	//_float ParentScaleZRatio = XMVectorGetX(XMVector3Length((XMLoadFloat4x4(&ParentMat).r[2])));

	//_float ScaleRatio = ParentMat.m[3][3] / ParentMat.m[3][2];

	_float3 vScale = m_pTransformCom->Get_Scaled();
	_float4 r = { vScale.x, 0.f, 0.f, 0.f };
	_float4 u = { 0.f, vScale.y, 0.f, 0.f };
	_float4 l = { 0.f, 0.f, 1.f, 0.f };
	_float4 p = { ParentMat.m[3][0] / ParentMat.m[3][3], ParentMat.m[3][1] / ParentMat.m[3][3], 0.1f, 1.f };

	memcpy(ParentMat.m[0], &r, sizeof _float4);
	memcpy(ParentMat.m[1], &u, sizeof _float4);
	memcpy(ParentMat.m[2], &l, sizeof _float4);
	memcpy(ParentMat.m[3], &p, sizeof _float4);

	m_FinalWorldMatrix = ParentMat;

	m_FinalWorldMatrix.m[3][0] = m_FinalWorldMatrix.m[3][0] - g_iWinSizeX * 0.5f;
	m_FinalWorldMatrix.m[3][1] = -m_FinalWorldMatrix.m[3][1] + g_iWinSizeY * 0.5f;
	//m_FinalWorldMatrix.m[3][1] *= -1.f;
	m_FinalWorldMatrix.m[3][2] = 0.1f;

	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CHpBar::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CHpBar::Add_Components()
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
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, m_Desc.m_TextureLevelIndex, m_Desc.m_TextureProtoTypeName,
		(CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CHpBar::SetUp_ShaderResources()
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

	//if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition",
	//	&pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

_matrix CHpBar::Remove_Scale(_fmatrix Matrix)
{
	_matrix		Result = Matrix;

	Result.r[0] = XMVector3Normalize(Result.r[0]);
	Result.r[1] = XMVector3Normalize(Result.r[1]);
	Result.r[2] = XMVector3Normalize(Result.r[2]);

	return Result;
}

CHpBar* CHpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHpBar* pInstance = new CHpBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CHpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHpBar::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CHpBar* pInstance = new CHpBar(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CHpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHpBar::Free()
{
	__super::Free();

	Safe_Release(m_Desc.pParent);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);


}
