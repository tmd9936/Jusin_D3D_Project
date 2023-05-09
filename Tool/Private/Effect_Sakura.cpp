#include "stdafx.h"
#include "Effect_Sakura.h"

#include "GameInstance.h"

CEffect_Sakura::CEffect_Sakura(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CEffect_Sakura::CEffect_Sakura(const CEffect_Sakura& rhs)
	: CGameObject(rhs)
{

}

HRESULT CEffect_Sakura::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Sakura::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_NONLIGHT;

	m_pTransformCom->Set_Scaled({ 0.3f, 0.3f, 1.f });

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(20.f, 1.f, 15.f, 1.f));


	return S_OK;
}

_uint CEffect_Sakura::Tick(_double TimeDelta)
{
	m_pVIBufferCom->Update(TimeDelta);

	return _uint();
}

_uint CEffect_Sakura::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CEffect_Sakura::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Sakura::Add_Components()
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
	CVIBuffer_Point_Instance::POINT_INSTANCE_DESC		BufferDesc{};

	BufferDesc.vPosition = _float3(0.f, 0.f, 0.f);
	BufferDesc.vSize = _float2(50.f, 50.f);
	BufferDesc.fLifeTime = 15.f;
	BufferDesc.fMinSpeed = 3.f;
	BufferDesc.fMaxSpeed = 10.f;

	/* For.Com_VIBuffer */
	if (FAILED(pGameInstance->Add_Component(CVIBuffer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
		(CComponent**)&m_pVIBufferCom, &BufferDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, LEVEL_STATIC, L"Prototype_Component_Texture_Effect_Sakura",
		(CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Sakura::SetUp_ShaderResources()
{
	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CEffect_Sakura* CEffect_Sakura::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Sakura* pInstance = new CEffect_Sakura(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CEffect_Sakura");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Sakura::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CEffect_Sakura* pInstance = new CEffect_Sakura(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CEffect_Sakura");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Sakura::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
