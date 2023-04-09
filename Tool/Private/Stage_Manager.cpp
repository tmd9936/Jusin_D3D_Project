#include "stdafx.h"
#include "Stage_Manager.h"

#include "GameInstance.h"

#include "StageCamera.h"

#include "Monster.h"

#include "Client_Utility.h"


CStage_Manager::CStage_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CStage_Manager::CStage_Manager(const CStage_Manager& rhs)
	: CGameObject(rhs)
{

}

HRESULT CStage_Manager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStage_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Init_ManagerInfo()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(Add_Components_By_File()))
		return E_FAIL;

	if (FAILED(Init_ManagerInfo()))
		return E_FAIL;

	return S_OK;
}

_uint CStage_Manager::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);
	Change_State();

	return _uint();
}

_uint CStage_Manager::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CStage_Manager::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}


HRESULT CStage_Manager::Init_ManagerInfo()
{
	m_eRenderId = RENDER_BLEND_UI;

	m_vCurrentFadeColor = m_Desc.m_FadeInStartColor;

	m_fSizeX = (_float)g_iWinSizeX * 1.5f;
	m_fSizeY = (_float)g_iWinSizeY * 1.5f;
	m_fX = (_float)(g_iWinSizeX >> 1);
	m_fY = (_float)(g_iWinSizeY >> 1);

	m_pTransformCom->Set_Scaled({ m_fSizeX, m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	p_MainCamera = (CStageCamera*)CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Camera", L"Main_Camera");
	if (nullptr == p_MainCamera)
		return E_FAIL;

	Safe_AddRef(p_MainCamera);

	Init_PlayersPos();

	return S_OK;
}

void CStage_Manager::Init_PlayersPos()
{
	CTransform* pTransform = nullptr;
	CNavigation* pNaviagtion = nullptr;

	CGameObject* pPlayer1 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", L"Player1");
	if (pPlayer1)
	{
		pTransform = pPlayer1->Get_As<CTransform>();
		pTransform->Set_Pos(30.54f, 0.5f, 19.28f);

		pNaviagtion = pPlayer1->Get_As<CNavigation>();
		pNaviagtion->Set_Index_By_Position({ 30.54f, 0.5f, 19.28f });
	}

	CGameObject* pPlayer2 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", L"Player2");
	if (pPlayer2)
	{
		pTransform = pPlayer2->Get_As<CTransform>();
		pTransform->Set_Pos(29.f, 0.5f, 18.5f);

		pNaviagtion = pPlayer2->Get_As<CNavigation>();
		pNaviagtion->Set_Index_By_Position({ 29.f, 0.5f, 18.5f });
	}

	CGameObject* pPlayer3 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", L"Player3");
	if (pPlayer3)
	{
		pTransform = pPlayer3->Get_As<CTransform>();
		pTransform->Set_Pos(31.f, 0.5f, 18.5f);

		pNaviagtion = pPlayer3->Get_As<CNavigation>();
		pNaviagtion->Set_Index_By_Position({ 31.f, 0.5f, 18.5f });
	}

	CGameObject* pCameraTarget = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_CameraTarget", L"CameraTarget");
	if (pCameraTarget)
	{
		pTransform = pCameraTarget->Get_As<CTransform>();
		pTransform->Set_Pos(30.54f, 0.5f, 19.28f);
	}
}

void CStage_Manager::Fade_In(const _double& TimeDelta)
{
	if (m_Desc.m_FadeSecond <= m_fCurrentFadeTime)
	{
		m_eCurState = MANAGER_IDLE;
		return;
	}
	m_vCurrentFadeColor.x -= (_float)TimeDelta;
	m_vCurrentFadeColor.y -= (_float)TimeDelta;
	m_vCurrentFadeColor.z -= (_float)TimeDelta;
	m_vCurrentFadeColor.w -= (_float)TimeDelta;

	m_fCurrentFadeTime += TimeDelta;
}


_bool CStage_Manager::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	Value ManagerDesc(kObjectType);
	{
		ManagerDesc.AddMember("m_FadeSecond", m_Desc.m_FadeSecond, allocator);

		Value m_FadeInStartColor(kObjectType);
		{
			m_FadeInStartColor.AddMember("x", m_Desc.m_FadeInStartColor.x, allocator);
			m_FadeInStartColor.AddMember("y", m_Desc.m_FadeInStartColor.y, allocator);
			m_FadeInStartColor.AddMember("z", m_Desc.m_FadeInStartColor.z, allocator);
			m_FadeInStartColor.AddMember("w", m_Desc.m_FadeInStartColor.w, allocator);
		}
		ManagerDesc.AddMember("m_FadeInStartColor", m_FadeInStartColor, allocator);

		Value m_FadeInEndColor(kObjectType);
		{
			m_FadeInEndColor.AddMember("x", m_Desc.m_FadeInEndColor.x, allocator);
			m_FadeInEndColor.AddMember("y", m_Desc.m_FadeInEndColor.y, allocator);
			m_FadeInEndColor.AddMember("z", m_Desc.m_FadeInEndColor.z, allocator);
			m_FadeInEndColor.AddMember("w", m_Desc.m_FadeInEndColor.w, allocator);
		}
		ManagerDesc.AddMember("m_FadeInEndColor", m_FadeInEndColor, allocator);

	}
	doc.AddMember("ManagerDesc", ManagerDesc, allocator);


	return true;
}

_bool CStage_Manager::Load_By_JsonFile_Impl(Document& doc)
{
	const Value& ManagerDesc = doc["ManagerDesc"];


	const Value& m_FadeInStartColor = ManagerDesc["m_FadeInStartColor"];
	m_Desc.m_FadeInStartColor = _float4(m_FadeInStartColor["x"].GetFloat(), m_FadeInStartColor["y"].GetFloat(), 
		m_FadeInStartColor["z"].GetFloat(), m_FadeInStartColor["w"].GetFloat());

	const Value& m_FadeInEndColor = ManagerDesc["m_FadeInEndColor"];
	m_Desc.m_FadeInEndColor = _float4(m_FadeInEndColor["x"].GetFloat(), m_FadeInEndColor["y"].GetFloat(),
		m_FadeInEndColor["z"].GetFloat(), m_FadeInEndColor["w"].GetFloat());

	m_Desc.m_FadeSecond = ManagerDesc["m_FadeSecond"].GetFloat();
	return true;
}

void CStage_Manager::State_Tick(const _double& TimeDelta)
{
	switch (m_eCurState)
	{
	case MANAGER_IDLE:
		break;
	case MANAGER_OPEN_STATE_INFO:
		//Focus_Stay(TimeDelta);
		break;
	case MANAGER_FADE_IN:
		Fade_In(TimeDelta);
		break;
	}
}

void CStage_Manager::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case MANAGER_IDLE:
			m_eRenderId = RENDER_END;
			break;
		case MANAGER_OPEN_STATE_INFO:
			m_eRenderId = RENDER_BLEND_UI;
			m_vCurrentFadeColor = m_Desc.m_FadeInStartColor;

			break;
		case MANAGER_FADE_IN:
			m_eRenderId = RENDER_BLEND_UI;
			m_vCurrentFadeColor = m_Desc.m_FadeInStartColor;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

HRESULT CStage_Manager::Add_Components()
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
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, LEVEL_STATIC, L"Prototype_Component_Texture_Window_Plane_Corner",
		(CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage_Manager::Add_Components_By_File()
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
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, LEVEL_STATIC, L"Prototype_Component_Texture_Window_Plane_Corner",
		(CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage_Manager::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_vCurrentFadeColor, (sizeof m_vCurrentFadeColor))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CStage_Manager* CStage_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStage_Manager* pInstance = new CStage_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStage_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStage_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStage_Manager* pInstance = new CStage_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStage_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStage_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStage_Manager* pInstance = new CStage_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStage_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStage_Manager::Free()
{
	__super::Free();

	Safe_Release(p_MainCamera);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);

}
