#include "stdafx.h"
#include "StagePoint.h"

#include "GameInstance.h"

CStagePoint::CStagePoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CStagePoint::CStagePoint(const CStagePoint& rhs)
	: CGameObject(rhs)
{

}

HRESULT CStagePoint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStagePoint::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_Desc.vPos = (*(STAGE_POINT_DESC*)(pArg)).vPos;
		m_Desc.eState = (STAGE_POINT_STATE)(*(STAGE_POINT_DESC*)(pArg)).eState;
		m_eState = (STAGE_POINT_STATE)(*(STAGE_POINT_DESC*)(pArg)).eState;
		m_Desc.stageNumber = (STAGE_POINT_STATE)(*(STAGE_POINT_DESC*)(pArg)).stageNumber;
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(m_Desc.vPos.x, m_Desc.vPos.y, m_Desc.vPos.z);

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

HRESULT CStagePoint::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(m_Desc.vPos.x, m_Desc.vPos.y, m_Desc.vPos.z);

	m_eRenderId = RENDER_NONBLEND;

	return S_OK;
}

_uint CStagePoint::Tick(_double TimeDelta)
{
	switch (m_eState)
	{
	case STAGE_POINT_STATE_NO_VIEW:
		break;
	case STAGE_POINT_STATE_STANDARD:
		break;
	case STAGE_POINT_STATE_CLEAR:
		break;
	case STAGE_POINT_STATE_BOSS:
		break;
	case STAGE_POINT_STATE_EFFECT_CLEAR:
		m_pModelCom->Play_Animation(TimeDelta);
		break;
	case STAGE_POINT_STATE_EFFECT_APPEAR:
		m_pModelCom->Play_Animation(TimeDelta);
		break;
	case STAGE_POINT_STATE_EFFECT_APPEAR_LAST:
		m_pModelCom->Play_Animation(TimeDelta);
		break;
	case STAGE_POINT_STATE_END:
		break;
	default:
		break;
	}

	return _uint();
}

_uint CStagePoint::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CStagePoint::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		//if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		//	return E_FAIL;

		switch (m_eState)
		{
		case STAGE_POINT_STATE_NO_VIEW:
			break;
		case STAGE_POINT_STATE_STANDARD:
			break;
		case STAGE_POINT_STATE_CLEAR:
			break;
		case STAGE_POINT_STATE_BOSS:
			break;
		case STAGE_POINT_STATE_EFFECT_CLEAR:
			if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
				return E_FAIL;
			break;
		case STAGE_POINT_STATE_EFFECT_APPEAR:
			if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
				return E_FAIL;
			break;
		case STAGE_POINT_STATE_EFFECT_APPEAR_LAST:
			if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
				return E_FAIL;
			break;
		case STAGE_POINT_STATE_END:
			break;
		default:
			break;
		}

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	m_pAABB->Render();
#endif // _DEBUG


	return S_OK;
}

_bool CStagePoint::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	Value StagePointDesc(kObjectType);
	{
		Value vPos(kObjectType);
		{
			vPos.AddMember("x", m_Desc.vPos.x, allocator);
			vPos.AddMember("y", m_Desc.vPos.y, allocator);
			vPos.AddMember("z", m_Desc.vPos.z, allocator);
		}
		StagePointDesc.AddMember("vPos", vPos, allocator);

		Value vColor(kObjectType);
		{
			vColor.AddMember("x", m_Desc.vColor.x, allocator);
			vColor.AddMember("y", m_Desc.vColor.y, allocator);
			vColor.AddMember("z", m_Desc.vColor.z, allocator);
			vColor.AddMember("w", m_Desc.vColor.w, allocator);
		}
		StagePointDesc.AddMember("vColor", vColor, allocator);

		StagePointDesc.AddMember("eState", m_Desc.eState, allocator);
		StagePointDesc.AddMember("stageNumber", m_Desc.stageNumber, allocator);

		Value levelTitleTextValue;
		string levelTitleText = convert.to_bytes(m_Desc.levelTitleText);
		levelTitleTextValue.SetString(levelTitleText.c_str(), (SizeType)levelTitleText.size(), allocator);
		StagePointDesc.AddMember("levelTitleText", levelTitleTextValue, allocator);

	}
	doc.AddMember("StagePointDesc", StagePointDesc, allocator);

	return true;
}

_bool CStagePoint::Load_By_JsonFile_Impl(Document& doc)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	const Value& StagePointDesc = doc["StagePointDesc"];

	const Value& vPos = StagePointDesc["vPos"];
	m_Desc.vPos = _float3(vPos["x"].GetFloat(), vPos["y"].GetFloat(), vPos["z"].GetFloat());

	const Value& vColor = StagePointDesc["vColor"];
	m_Desc.vColor = _float4(vColor["x"].GetFloat(), vColor["y"].GetFloat(), vColor["z"].GetFloat(), vColor["w"].GetFloat());

	m_Desc.eState = StagePointDesc["eState"].GetUint();
	m_Desc.stageNumber = StagePointDesc["stageNumber"].GetUint();

	string levelTitleText = StagePointDesc["levelTitleText"].GetString();
	m_Desc.levelTitleText = convert.from_bytes(levelTitleText);

	return true;
}


HRESULT CStagePoint::Add_Components()
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

	/* For.Com_AABB*/
	CCollider::COLLIDER_DESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
	ColliderDesc.vPosition = _float3(0.0f, ColliderDesc.vScale.y * 0.5f, 0.f);
	if (FAILED(pGameInstance->Add_Component(CCollider::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		(CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Model */
	switch (m_eState)
	{
	case STAGE_POINT_STATE_NO_VIEW:
		break;
	case STAGE_POINT_STATE_STANDARD:
		if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
			(CComponent**)&m_pShaderCom, nullptr)))
			return E_FAIL;
		break;
	case STAGE_POINT_STATE_CLEAR:
		if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
			(CComponent**)&m_pShaderCom, nullptr)))
			return E_FAIL;
		break;
	case STAGE_POINT_STATE_BOSS:
		if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
			(CComponent**)&m_pShaderCom, nullptr)))
			return E_FAIL;
		break;
	case STAGE_POINT_STATE_EFFECT_CLEAR:
		if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
			(CComponent**)&m_pShaderCom, nullptr)))
			return E_FAIL;
		break;
	case STAGE_POINT_STATE_EFFECT_APPEAR:
		if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
			(CComponent**)&m_pShaderCom, nullptr)))
			return E_FAIL;
		break;
	case STAGE_POINT_STATE_EFFECT_APPEAR_LAST:
		if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
			(CComponent**)&m_pShaderCom, nullptr)))
			return E_FAIL;
		break;
	case STAGE_POINT_STATE_END:
		break;
	default:
		break;
	}	

	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_WORLDMAP, TEXT("Prototype_Component_Model_Stage_Point_Standard"),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	return S_OK;
}


HRESULT CStagePoint::SetUp_ShaderResources()
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

CStagePoint* CStagePoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStagePoint* pInstance = new CStagePoint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStagePoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStagePoint::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStagePoint* pInstance = new CStagePoint(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStagePoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStagePoint::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStagePoint* pInstance = new CStagePoint(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStagePoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CStagePoint::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pPickingCube);
	Safe_Release(m_pAABB);
}
