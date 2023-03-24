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
	if (pArg)
		memcpy(&m_desc, pArg, sizeof FLATTERRAIN_DESC);

	m_desc.m_Level = iLevelIndex;

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_BLEND;
	
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
	if (m_bRender)
	{
		if (FAILED(SetUp_ShaderResources()))
			return E_FAIL;

		if (m_bWire)
		{
			m_pShaderCom->Begin(1);
		}
		else
		{
			m_pShaderCom->Begin(0);
		}
		m_pVIBufferCom->Render();

	}
#ifdef _DEBUG
	m_pNavigationCom->Render();
#endif // _DEBUG

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
	CVIBuffer_FlatTerrain::VIBUFFER_FLAT_TERRAIN_DESC TerrainDesc = { DEFAULT_TERRAIN_WIDTH, DEFAULT_TERRAIN_HEIGHT };
	if (FAILED(pGameInstance->Add_Component(CVIBuffer_FlatTerrain::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_FlatTerrain"),
		(CComponent**)&m_pVIBufferCom, &TerrainDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, m_desc.m_Level, TEXT("Prototype_Component_Shader_VtxNorTex_HeightTerrain"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
		(CComponent**)&m_pTextureCom[TEXTURETYPE_DIFFUSE], nullptr)))
		return E_FAIL;

	/* For.Com_Mask */
	if (FAILED(pGameInstance->Add_Component(FAMILY_ID_TEXTURE_MASK, this, LEVEL_STATIC, TEXT("Prototype_Component_Texture_TerrainMask"),
		(CComponent**)&m_pTextureCom[TEXTURETYPE_MASK], nullptr)))
		return E_FAIL;

	/* For.Com_Brush */
	if (FAILED(pGameInstance->Add_Component(FAMILY_ID_TEXTURE_BRUSH, this, LEVEL_STATIC, TEXT("Prototype_Component_Texture_Brush"),
		(CComponent**)&m_pTextureCom[TEXTURETYPE_BRUSH], nullptr)))
		return E_FAIL;

	/* For.Com_Navigation */
	if (FAILED(pGameInstance->Add_Component(CNavigation::familyId, this, m_desc.m_Level, TEXT("Prototype_Component_Navigation"),
		(CComponent**)&m_pNavigationCom, nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CFlatTerrain::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float4 data = pGameInstance->Get_CamPosition();
	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition",
		&data, sizeof(_float4))))
		return E_FAIL;

	const LIGHTDESC* pLightDesc = pGameInstance->Get_Light(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir",
		&pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse",
		&pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient",
		&pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular",
		&pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vBrushPos",
		&m_pTextureCom[TEXTURETYPE_BRUSH]->Get_BrushPos(), sizeof(_float4))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	if (FAILED(m_pTextureCom[TEXTURETYPE_DIFFUSE]->Set_ShaderResourceArray(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEXTURETYPE_MASK]->Set_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEXTURETYPE_BRUSH]->Set_ShaderResource(m_pShaderCom, "g_BrushTexture", 0)))
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

	Safe_Release(m_pNavigationCom);

	Safe_Release(m_pTransformCom);

	for (_uint i = 0; i < TYPE_END; ++i)
		Safe_Release(m_pTextureCom[i]);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);


}
