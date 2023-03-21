#include "stdafx.h"
#include "BuffState.h"

#include "GameInstance.h"
#include "Bone.h"


CBuffState::CBuffState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CBuffState::CBuffState(const CBuffState& rhs)
	: CGameObject(rhs)
{
}

HRESULT CBuffState::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBuffState::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	memcpy(&m_Desc, pArg, sizeof m_Desc);

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(1.0f, 1.0f, 1.0f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.0f));

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

_uint CBuffState::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CBuffState::LateTick(_double TimeDelta)
{
	_matrix		ParentMatrix = m_Desc.pBonePtr->Get_OffsetMatrix() *
		m_Desc.pBonePtr->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_Desc.PivotMatrix);

	XMStoreFloat4x4(&m_FinalWorldMatrix, m_pTransformCom->Get_WorldMatrix_Matrix() * Remove_Scale(ParentMatrix) * m_Desc.pParent->Get_WorldMatrix_Matrix());

	m_pAABB->Tick(XMLoadFloat4x4(&m_FinalWorldMatrix));

	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);


	return _uint();
}

HRESULT CBuffState::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

#ifdef _DEBUG
	m_pAABB->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CBuffState::Add_Components()
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

	/* For.Com_AABB*/
	CCollider::COLLIDER_DESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		(CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBuffState::SetUp_ShaderResources()
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

	//if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition",
	//	&pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

_matrix CBuffState::Remove_Scale(_fmatrix Matrix)
{
	_matrix		Result = Matrix;

	Result.r[0] = XMVector3Normalize(Result.r[0]);
	Result.r[1] = XMVector3Normalize(Result.r[1]);
	Result.r[2] = XMVector3Normalize(Result.r[2]);

	return Result;
}

CBuffState* CBuffState::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBuffState* pInstance = new CBuffState(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CBuffState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBuffState::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CBuffState* pInstance = new CBuffState(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CBuffState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuffState::Free()
{
	__super::Free();

	Safe_Release(m_Desc.pBonePtr);
	Safe_Release(m_Desc.pParent);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pAABB);

}
