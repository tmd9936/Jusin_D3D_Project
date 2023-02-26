#include "stdafx.h"
#include "..\Public\FlatTerrain.h"

#include "GameInstance.h"

CFlatTerrain::CFlatTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CFlatTerrain::CFlatTerrain(const CFlatTerrain& rhs)
	: CGameObject(rhs)
{
}

HRESULT CFlatTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFlatTerrain::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

_uint CFlatTerrain::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CFlatTerrain::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CFlatTerrain::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	ID3D11RasterizerState* WireFrame = nullptr;
	ID3D11RasterizerState* FillFrame = nullptr;
	D3D11_RASTERIZER_DESC wfdesc{};
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	m_pDevice->CreateRasterizerState(&wfdesc, &WireFrame);
	m_pContext->RSSetState(WireFrame);

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_BACK;                    // �ٽ� back culling���� �ٲ���� �Ѵ�. 
	m_pDevice->CreateRasterizerState(&wfdesc, &FillFrame);
	m_pContext->RSSetState(FillFrame);
	Safe_Release(WireFrame);
	Safe_Release(FillFrame);

	return S_OK;
}

HRESULT CFlatTerrain::Add_Components()
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
	CVIBuffer_FlatTerrain::VIBUFFER_FLAT_TERRAIN_DESC TerrainDesc = { 200, 200 };
	if (FAILED(pGameInstance->Add_Component(CVIBuffer_FlatTerrain::familyId, this, LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_FlatTerrain"),
		(CComponent**)&m_pVIBufferCom, &TerrainDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		(CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CFlatTerrain::SetUp_ShaderResources()
{
	_float4x4		WorldMatrix;

	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &WorldMatrix)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CFlatTerrain* CFlatTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFlatTerrain* pInstance = new CFlatTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CFlatTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFlatTerrain::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CFlatTerrain* pInstance = new CFlatTerrain(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CFlatTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlatTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);


}
