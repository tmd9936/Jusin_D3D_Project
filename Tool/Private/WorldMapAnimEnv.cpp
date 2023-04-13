#include "stdafx.h"
#include "WorldMapAnimEnv.h"

#include "GameInstance.h"

CWorldMapAnimEnv::CWorldMapAnimEnv(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CWorldMapAnimEnv::CWorldMapAnimEnv(const CWorldMapAnimEnv& rhs)
	: CGameObject(rhs)
{

}

HRESULT CWorldMapAnimEnv::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWorldMapAnimEnv::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_Desc.vPos = (*(WORLDMAP_ANIM_ENV_DESC*)(pArg)).vPos;
		lstrcpy(m_Desc.ModelPrototypeTag, (*(WORLDMAP_ANIM_ENV_DESC*)(pArg)).ModelPrototypeTag);

		//memcpy(&m_Desc, pArg, sizeof m_Desc + 2);
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(m_Desc.vPos.x, m_Desc.vPos.y, m_Desc.vPos.z);

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

_uint CWorldMapAnimEnv::Tick(_double TimeDelta)
{
	m_pModelCom->Play_Animation(TimeDelta);

	return _uint();
}

_uint CWorldMapAnimEnv::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CWorldMapAnimEnv::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}


	return S_OK;
}


HRESULT CWorldMapAnimEnv::Add_Components()
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

	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_WORLDMAP, m_Desc.ModelPrototypeTag,
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;


	return S_OK;
}


HRESULT CWorldMapAnimEnv::SetUp_ShaderResources()
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

CWorldMapAnimEnv* CWorldMapAnimEnv::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorldMapAnimEnv* pInstance = new CWorldMapAnimEnv(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CWorldMapAnimEnv");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWorldMapAnimEnv::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CWorldMapAnimEnv* pInstance = new CWorldMapAnimEnv(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CWorldMapAnimEnv");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CWorldMapAnimEnv::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
