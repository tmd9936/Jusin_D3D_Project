#include "stdafx.h"
#include "Stove.h"

#include "GameInstance.h"


CStove::CStove(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CStove::CStove(const CStove& rhs)
	: CGameObject(rhs)
{

}

HRESULT CStove::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStove::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}


_uint CStove::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pAABB->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());

	return _uint();
}

_uint CStove::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	m_pRendererCom->Add_RenderGroup(RENDER_LAPLACIAN, this);

	return _uint();
}

HRESULT CStove::Render()
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

#ifdef _DEBUG
	m_pRendererCom->Add_DebugRenderGroup(m_pAABB);
#endif // _DEBUG

	return S_OK;
}

HRESULT CStove::Render_Laplacian()
{
	return E_NOTIMPL;
}


HRESULT CStove::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_PickingCube */
	if (FAILED(pGameInstance->Add_Component(CPickingCube::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_PickingCube"),
		(CComponent**)&m_pPickingCube, nullptr)))
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
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Model_BaseCamp_Stove"),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_AABB*/
	CCollider::COLLIDER_DESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		(CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}


HRESULT CStove::SetUp_ShaderResources()
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

CStove* CStove::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStove* pInstance = new CStove(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStove");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStove::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStove* pInstance = new CStove(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStove");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CStove::Free()
{
	__super::Free();

	Safe_Release(m_pPickingCube);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pAABB);
}
