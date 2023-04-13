#include "stdafx.h"
#include "WorldMapCloud.h"

#include "GameInstance.h"


CWorldMapCloud::CWorldMapCloud(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CWorldMapCloud::CWorldMapCloud(const CWorldMapCloud& rhs)
	: CGameObject(rhs)
{

}

HRESULT CWorldMapCloud::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWorldMapCloud::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	m_pTransformCom->Set_Pos(17.9f, 0.25f, 21.f);

	return S_OK;
}


_uint CWorldMapCloud::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	return _uint();
}

_uint CWorldMapCloud::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CWorldMapCloud::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	return S_OK;
}


HRESULT CWorldMapCloud::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_PickingCube */
	if (FAILED(pGameInstance->Add_Component(CPickingCube::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_PickingCube"),
		(CComponent**)&m_pPickingCubCom, nullptr)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_WORLDMAP, TEXT("Prototype_Component_Model_WorldMap_Cloud"),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;


	return S_OK;
}


HRESULT CWorldMapCloud::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float cameraFar = pGameInstance->Get_CameraFar();
	if (FAILED(m_pShaderCom->Set_RawValue("g_CameraFar",
		&cameraFar, sizeof(_float))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CWorldMapCloud* CWorldMapCloud::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorldMapCloud* pInstance = new CWorldMapCloud(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CWorldMapCloud");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWorldMapCloud::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CWorldMapCloud* pInstance = new CWorldMapCloud(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CWorldMapCloud");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CWorldMapCloud::Free()
{
	__super::Free();

	Safe_Release(m_pPickingCubCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
