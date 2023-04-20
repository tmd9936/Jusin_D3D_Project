#include "stdafx.h"
#include "Trail.h"

#include "GameInstance.h"

#include "Bone.h"

CTrail::CTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CTrail::CTrail(const CTrail& rhs)
	: CGameObject(rhs)
{

}

HRESULT CTrail::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrail::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof m_Desc);
	}
	else
	{
		m_Desc.trailVertexNum = 36;
	}

	m_initTick = m_Desc.trailVertexNum;

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.0f));

	m_eRenderId = RENDER_NONLIGHT;

	return S_OK;
}

_uint CTrail::Tick(_double TimeDelta)
{

	return _uint();
}

_uint CTrail::LateTick(_double TimeDelta)
{
	_matrix		ParentMatrix = m_Desc.pBonePtr->Get_OffsetMatrix() *
		m_Desc.pBonePtr->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_Desc.PivotMatrix);

	XMStoreFloat4x4(&m_FinalWorldMatrix, m_pTransformCom->Get_WorldMatrix_Matrix() * Remove_Scale(ParentMatrix) * m_Desc.pParent->Get_WorldMatrix_Matrix());

	m_pVIBufferCom->Tick(TimeDelta, XMLoadFloat4x4(&m_FinalWorldMatrix));
	if (m_initTick >= 0)
	{
		m_initTick--;
	}
	else
	{
		m_pRendererCom->Add_RenderGroup(m_eRenderId, this);
	}


	return _uint();
}

HRESULT CTrail::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(4);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTrail::Add_Components()
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

	CVIBuffer_Trail::VIBUFFER_TRAIL_DESC desc{};
	desc.m_iNumVertices = m_Desc.trailVertexNum;
	/* For.Com_VIBuffer */
	if (FAILED(pGameInstance->Add_Component(CVIBuffer_Trail::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"),
		(CComponent**)&m_pVIBufferCom, &desc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, LEVEL_STATIC, L"Prototype_Component_Texture_Trail",
		(CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

_matrix CTrail::Remove_Scale(_fmatrix Matrix)
{
	_matrix		Result = Matrix;

	Result.r[0] = XMVector3Normalize(Result.r[0]);
	Result.r[1] = XMVector3Normalize(Result.r[1]);
	Result.r[2] = XMVector3Normalize(Result.r[2]);

	return Result;
}

HRESULT CTrail::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_FinalWorldMatrix)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CTrail* CTrail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTrail* pInstance = new CTrail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrail::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CTrail* pInstance = new CTrail(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrail::Free()
{
	__super::Free();

	Safe_Release(m_Desc.pBonePtr);
	Safe_Release(m_Desc.pParent);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
