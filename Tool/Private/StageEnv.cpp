#include "stdafx.h"
#include "StageEnv.h"

#include "GameInstance.h"

#include "Utility.h"

CStageEnv::CStageEnv(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CStageEnv::CStageEnv(const CStageEnv& rhs)
	: CGameObject(rhs)
{

}

HRESULT CStageEnv::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStageEnv::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_StageEnvDesc.vPos = (*(STAGE_ENV_DESC*)(pArg)).vPos;
		m_StageEnvDesc.vScale = (*(STAGE_ENV_DESC*)(pArg)).vScale;
		m_StageEnvDesc.vRotate = (*(STAGE_ENV_DESC*)(pArg)).vRotate;

		m_StageEnvDesc.ModelPrototypeTag = (*(STAGE_ENV_DESC*)(pArg)).ModelPrototypeTag;
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(m_StageEnvDesc.vPos.x, m_StageEnvDesc.vPos.y, m_StageEnvDesc.vPos.z);

	m_pTransformCom->Set_Rotation(m_StageEnvDesc.vRotate);

	m_pTransformCom->Set_Scaled(m_StageEnvDesc.vScale);

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

HRESULT CStageEnv::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(Add_Components_By_File()))
		return E_FAIL;

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

_uint CStageEnv::Tick(_double TimeDelta)
{
	//m_pModelCom->Play_Animation(TimeDelta);

	m_pAABB->Tick(m_pTransformCom->Get_WorldMatrix_Matrix());

	return _uint();
}

_uint CStageEnv::LateTick(_double TimeDelta)
{
	if (true == CGameInstance::GetInstance()->Is_In_Frustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.5f))
	{
		m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

		m_pRendererCom->Add_RenderGroup(RENDER_LAPLACIAN, this);

		m_pRendererCom->Add_RenderGroup(RENDER_SHADOWDEPTH, this);

	}
	return _uint();
}

HRESULT CStageEnv::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		//if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
		//	return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}


	return S_OK;
}

HRESULT CStageEnv::Render_Laplacian()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		//if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
		//	return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CStageEnv::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_Shadow_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		//if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
		//	return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CStageEnv::On_CollisionEnter(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	CGameObject* pOtherOwner = pOther->Get_Owner();
	if (!pOtherOwner)
		return;

	CTransform* pOtherTransform = pOtherOwner->Get_As<CTransform>();

	CNavigation* pOtherNavigationCom = pOtherOwner->Get_As<CNavigation>();

	Engine::CUtility::CollisionPushingOutNormal(m_pAABB, pOther, fX, fY, fZ, pOtherTransform, pOtherNavigationCom);
}

void CStageEnv::On_Collision(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
	CGameObject* pOtherOwner = pOther->Get_Owner();
	if (!pOtherOwner)
		return;

	CTransform* pOtherTransform = pOtherOwner->Get_As<CTransform>();

	CNavigation* pOtherNavigationCom = pOtherOwner->Get_As<CNavigation>();

	Engine::CUtility::CollisionPushingOutNormal(m_pAABB, pOther, fX, fY, fZ, pOtherTransform, pOtherNavigationCom);
}

void CStageEnv::On_CollisionExit(CCollider* pOther, const _float& fX, const _float& fY, const _float& fZ)
{
}

_bool CStageEnv::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	return _bool();
}

_bool CStageEnv::Load_By_JsonFile_Impl(Document& doc)
{
	if (m_pTransformCom)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

		const Value& StageEnvDesc = doc["StageEnvDesc"];

		const Value& vPos = StageEnvDesc["vPos"];
		m_pTransformCom->Set_Pos(vPos["x"].GetFloat(), vPos["y"].GetFloat(), vPos["z"].GetFloat());

		const Value& vScale = StageEnvDesc["vScale"];
		m_pTransformCom->Set_Scaled({ vScale["x"].GetFloat(), vScale["y"].GetFloat(), vScale["z"].GetFloat() });

		const Value& vRotate = StageEnvDesc["vRotate"];
		m_pTransformCom->Set_Rotation({ vRotate["x"].GetFloat(), vRotate["y"].GetFloat(), vRotate["z"].GetFloat() });

		string ModelPrototypeTag = StageEnvDesc["ModelPrototypeTag"].GetString();
		m_StageEnvDesc.ModelPrototypeTag = convert.from_bytes(ModelPrototypeTag);

	}

	return _bool();
}


HRESULT CStageEnv::Add_Components()
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

	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, m_StageEnvDesc.ModelPrototypeTag.c_str(),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_AABB*/
	CCollider::COLLIDER_DESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(0.8f, 1.0f, 0.8f);
	ColliderDesc.vPosition = _float3(0.0f, 0.f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		(CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageEnv::Add_Components_By_File()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, m_StageEnvDesc.ModelPrototypeTag.c_str(),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_AABB*/
	CCollider::COLLIDER_DESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(0.8f, 1.0f, 0.8f);
	ColliderDesc.vPosition = _float3(0.0f, 0.f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		(CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CStageEnv::SetUp_ShaderResources()
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

HRESULT CStageEnv::SetUp_Shadow_ShaderResources()
{
	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&pGameInstance->Get_LightViewMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&pGameInstance->Get_LightProjMatrix())))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

CStageEnv* CStageEnv::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStageEnv* pInstance = new CStageEnv(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStageEnv");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageEnv::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStageEnv* pInstance = new CStageEnv(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStageEnv");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageEnv::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStageEnv* pInstance = new CStageEnv(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStageEnv");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CStageEnv::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pAABB);
}
