#include "stdafx.h"
#include "WorldMap_Manager.h"

#include "GameInstance.h"

#include "Camera_Public.h"

#include "Monster.h"

#include "Client_Utility.h"
#include "StageInfoUI.h"
#include "StagePoint.h"
#include "Level_Loading.h"


CWorldMap_Manager::CWorldMap_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CWorldMap_Manager::CWorldMap_Manager(const CWorldMap_Manager& rhs)
	: CGameObject(rhs)
{

}

HRESULT CWorldMap_Manager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWorldMap_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_BACK_UI;

	m_fSizeX = (_float)g_iWinSizeX * 1.5f;
	m_fSizeY = (_float)g_iWinSizeY * 1.5f;
	m_fX = (_float)(g_iWinSizeX >> 1);
	m_fY = (_float)(g_iWinSizeY >> 1);

	m_pTransformCom->Set_Scaled({ m_fSizeX, m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	p_MainCamera = (CCamera_Public*)CGameInstance::GetInstance()->Get_Object(LEVEL_WORLDMAP, L"Layer_Camera", L"Main_Camera");
	if (nullptr == p_MainCamera)
		return E_FAIL;

	Safe_AddRef(p_MainCamera);

	if (FAILED(Init_PlayerPosition()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWorldMap_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
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

	m_eRenderId = RENDER_BACK_UI;

	m_vCurrentFadeColor = m_Desc.m_FadeStartColor;

	m_fSizeX = (_float)g_iWinSizeX * 1.5f;
	m_fSizeY = (_float)g_iWinSizeY * 1.5f;
	m_fX = (_float)(g_iWinSizeX >> 1);
	m_fY = (_float)(g_iWinSizeY >> 1);

	m_pTransformCom->Set_Scaled({ m_fSizeX, m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	p_MainCamera = (CCamera_Public*)CGameInstance::GetInstance()->Get_Object(LEVEL_WORLDMAP, L"Layer_Camera", L"Main_Camera");
	if (nullptr == p_MainCamera)
		return E_FAIL;

	Safe_AddRef(p_MainCamera);

	if (FAILED(Init_PlayerPosition()))
		return E_FAIL;

	return S_OK;
}

_uint CWorldMap_Manager::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);
	Change_State();

	if (m_eCurState == MANAGET_STAGE_SELECT)
	{
		if (FAILED(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_STAGE))))
			return 0;
		else
			return OBJ_SCENE_CHNAGE;
	}

	return _uint();
}

_uint CWorldMap_Manager::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CWorldMap_Manager::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CWorldMap_Manager::Init_PlayerPosition()
{
	CGameObject* pPlayer1 = CGameInstance::GetInstance()->Get_Object(LEVEL_WORLDMAP, L"Layer_Player", L"Player1");

	if (nullptr == pPlayer1)
		return E_FAIL;

	CTransform* pTransform = pPlayer1->Get_As<CTransform>();
	if (nullptr == pTransform)
		return E_FAIL;

	pTransform->Set_Pos(24.04f, 1.2f, 23.60f);

	CNavigation* pNaviagtion = nullptr;
	pNaviagtion = pPlayer1->Get_As<CNavigation>();
	pNaviagtion->Set_Index_By_Position({ 24.04f, 1.2f, 23.60f });

	return S_OK;
}

void CWorldMap_Manager::Fade_out(const _double& TimeDelta)
{
	if (p_MainCamera->Focus_To_Object(m_FocusPosition, (_float)TimeDelta * m_Desc.m_FadeInCameraSpeed, m_Desc.m_FadeInStopIntervalDistance))
	{
		m_eCurState = MANAGET_STAGE_SELECT;
	}

	m_vCurrentFadeColor.x += (_float)TimeDelta;
	m_vCurrentFadeColor.y += (_float)TimeDelta;
	m_vCurrentFadeColor.z += (_float)TimeDelta;
	m_vCurrentFadeColor.w += (_float)TimeDelta;
	m_fCurrentFadeTIme += TimeDelta;
}


_bool CWorldMap_Manager::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	Value ManagerDesc(kObjectType);
	{
		ManagerDesc.AddMember("m_FadeSecond", m_Desc.m_FadeSecond, allocator);

		Value m_FadeStartColor(kObjectType);
		{
			m_FadeStartColor.AddMember("x", m_Desc.m_FadeStartColor.x, allocator);
			m_FadeStartColor.AddMember("y", m_Desc.m_FadeStartColor.y, allocator);
			m_FadeStartColor.AddMember("z", m_Desc.m_FadeStartColor.z, allocator);
			m_FadeStartColor.AddMember("w", m_Desc.m_FadeStartColor.w, allocator);
		}
		ManagerDesc.AddMember("m_FadeStartColor", m_FadeStartColor, allocator);

		Value m_FadeEndColor(kObjectType);
		{
			m_FadeEndColor.AddMember("x", m_Desc.m_FadeEndColor.x, allocator);
			m_FadeEndColor.AddMember("y", m_Desc.m_FadeEndColor.y, allocator);
			m_FadeEndColor.AddMember("z", m_Desc.m_FadeEndColor.z, allocator);
			m_FadeEndColor.AddMember("w", m_Desc.m_FadeEndColor.w, allocator);
		}
		ManagerDesc.AddMember("m_FadeEndColor", m_FadeEndColor, allocator);

		ManagerDesc.AddMember("m_FadeInStopIntervalDistance", m_Desc.m_FadeInStopIntervalDistance, allocator);
		ManagerDesc.AddMember("m_FadeInCameraSpeed", m_Desc.m_FadeInCameraSpeed, allocator);
	}
	doc.AddMember("ManagerDesc", ManagerDesc, allocator);



	return true;
}

_bool CWorldMap_Manager::Load_By_JsonFile_Impl(Document& doc)
{
	const Value& ManagerDesc = doc["ManagerDesc"];

	m_Desc.m_FadeSecond = ManagerDesc["m_FadeSecond"].GetFloat();

	const Value& m_FadeStartColor = ManagerDesc["m_FadeStartColor"];
	m_Desc.m_FadeStartColor = _float4(m_FadeStartColor["x"].GetFloat(), m_FadeStartColor["y"].GetFloat(), m_FadeStartColor["z"].GetFloat(), m_FadeStartColor["w"].GetFloat());

	const Value& m_FadeEndColor = ManagerDesc["m_FadeEndColor"];
	m_Desc.m_FadeEndColor = _float4(m_FadeEndColor["x"].GetFloat(), m_FadeEndColor["y"].GetFloat(), m_FadeEndColor["z"].GetFloat(), m_FadeEndColor["w"].GetFloat());

	m_Desc.m_FadeInStopIntervalDistance = ManagerDesc["m_FadeInStopIntervalDistance"].GetFloat();

	m_Desc.m_FadeInCameraSpeed = ManagerDesc["m_FadeInCameraSpeed"].GetFloat();

	return true;
}

void CWorldMap_Manager::State_Tick(const _double& TimeDelta)
{
	switch (m_eCurState)
	{
	case MANAGER_IDLE:
		Picking();
		break;
	case MANAGER_OPEN_STATE_INFO:
		//Focus_Stay(TimeDelta);
		break;
	case MANAGER_CAMERA_FADE_OUT:
		Fade_out(TimeDelta);
		break;
	}
}

void CWorldMap_Manager::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case MANAGER_IDLE:
			m_eRenderId = RENDER_END;
			p_MainCamera->Control_On();
			CGameInstance::GetInstance()->Layer_Tick_State_Change(L"Layer_Stage_Info_UI", LEVEL_WORLDMAP, false);
			CGameInstance::GetInstance()->Layer_Tick_State_Change(L"Layer_UI", LEVEL_WORLDMAP, true);
			break;
		case MANAGER_OPEN_STATE_INFO:
			m_eRenderId = RENDER_BACK_UI;
			m_vCurrentFadeColor = m_Desc.m_FadeStartColor;
			p_MainCamera->Control_Off();
			CGameInstance::GetInstance()->Layer_Tick_State_Change(L"Layer_Stage_Info_UI", LEVEL_WORLDMAP, true);
			CGameInstance::GetInstance()->Layer_Tick_State_Change(L"Layer_UI", LEVEL_WORLDMAP, false);

			break;
		case MANAGER_CAMERA_FADE_OUT:
			m_eRenderId = RENDER_BACK_UI;
			m_vCurrentFadeColor = m_Desc.m_FadeStartColor;
			p_MainCamera->Control_Off();
			CGameInstance::GetInstance()->Layer_Tick_State_Change(L"Layer_Stage_Info_UI", LEVEL_WORLDMAP, false);
			CGameInstance::GetInstance()->Layer_Tick_State_Change(L"Layer_UI", LEVEL_WORLDMAP, false);

			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CWorldMap_Manager::Picking()
{
	if (MOUSE_TAB(MOUSE::LBTN) && CClient_Utility::Mouse_Pos_In_Platform() && m_eCurState == MANAGER_IDLE)
	{
		CGameObject* pStagePoint = m_pCalculator->Picking_Environment_Object(g_hWnd, L"Layer_Stage_Point", LEVEL_WORLDMAP);
		if (nullptr == pStagePoint)
			return;

		CGameObject* pStageInfo = CGameInstance::GetInstance()->Get_Object(LEVEL_WORLDMAP, L"Layer_Stage_Info_UI", L"StageInfo");
		if (nullptr == pStageInfo)
			return;

		CStagePoint::Stage_Point_Desc stagePointDesc =  dynamic_cast<CStagePoint*>(pStagePoint)->Get_Stage_Point_Desc();

		CStageInfoUI::STAGE_INFO_DESC stageInfoDesc{};
		stageInfoDesc.m_StageNumber = stagePointDesc.stageNumber;
		stageInfoDesc.m_vBaseColor = stagePointDesc.vColor;
		lstrcpy(stageInfoDesc.m_vStageNameText, stagePointDesc.levelTitleText.c_str());

		((CStageInfoUI*)pStageInfo)->Change_Stage_Info(stagePointDesc.vColor, stagePointDesc.stageNumber, stagePointDesc.levelTitleText);

		m_FocusPosition = _float4(stagePointDesc.vPos.x, stagePointDesc.vPos.y, stagePointDesc.vPos.z, 1.f);

		m_eCurState = MANAGER_OPEN_STATE_INFO;

		CGameInstance::GetInstance()->PlaySoundW(L"SE_SYS_TOUCH_1.ogg", SOUND_UI);
	}
}

HRESULT CWorldMap_Manager::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Calculator */
	if (FAILED(pGameInstance->Add_Component(CCalculator::familyId, this, LEVEL_WORLDMAP, TEXT("Prototype_Component_Calculator"),
		(CComponent**)&m_pCalculator, nullptr)))
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

HRESULT CWorldMap_Manager::Add_Components_By_File()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Calculator */
	if (FAILED(pGameInstance->Add_Component(CCalculator::familyId, this, LEVEL_WORLDMAP, TEXT("Prototype_Component_Calculator"),
		(CComponent**)&m_pCalculator, nullptr)))
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

HRESULT CWorldMap_Manager::SetUp_ShaderResources()
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

	if (m_eCurState == MANAGER_CAMERA_FADE_OUT)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_vCurrentFadeColor, (sizeof m_vCurrentFadeColor))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_Desc.m_FadeStartColor, (sizeof m_Desc.m_FadeStartColor))))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

CWorldMap_Manager* CWorldMap_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorldMap_Manager* pInstance = new CWorldMap_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CWorldMap_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWorldMap_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CWorldMap_Manager* pInstance = new CWorldMap_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CWorldMap_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWorldMap_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CWorldMap_Manager* pInstance = new CWorldMap_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CWorldMap_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWorldMap_Manager::Free()
{
	__super::Free();

	Safe_Release(p_MainCamera);
	Safe_Release(m_pCalculator);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);

}
