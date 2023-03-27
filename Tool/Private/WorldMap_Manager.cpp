#include "stdafx.h"
#include "WorldMap_Manager.h"

#include "GameInstance.h"

#include "Camera_Public.h"

#include "Monster.h"

#include "Client_Utility.h"

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

	m_fSizeX = (_float)g_iWinSizeX;
	m_fSizeY = (_float)g_iWinSizeY;
	m_fX = (_float)(g_iWinSizeX >> 1);
	m_fY = (_float)(g_iWinSizeY >> 1);

	m_pTransformCom->Set_Scaled({ m_fSizeX, m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	p_MainCamera = (CCamera_Public*)CGameInstance::GetInstance()->Get_Object(LEVEL_BASECAMP, L"Layer_Camera", L"Main_Camera");
	if (nullptr == p_MainCamera)
		return E_FAIL;

	Safe_AddRef(p_MainCamera);

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

	m_fSizeX = (_float)g_iWinSizeX;
	m_fSizeY = (_float)g_iWinSizeY;
	m_fX = (_float)(g_iWinSizeX >> 1);
	m_fY = (_float)(g_iWinSizeY >> 1);

	m_pTransformCom->Set_Scaled({ m_fSizeX, m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	p_MainCamera = (CCamera_Public*)CGameInstance::GetInstance()->Get_Object(LEVEL_BASECAMP, L"Layer_Camera", L"Main_Camera");
	if (nullptr == p_MainCamera)
		return E_FAIL;

	Safe_AddRef(p_MainCamera);

	return S_OK;
}

_uint CWorldMap_Manager::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);
	Picking();
	Change_State();

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

void CWorldMap_Manager::Focus_In(const _double& TimeDelta)
{
	if (p_MainCamera->Focus_To_Object(m_FocusPosition, (_float)TimeDelta, m_Desc.pokemonFocusLimitDistance))
	{
		m_eCurState = MANAGER_CAMERA_FOCUS_STAY;
	}
}

void CWorldMap_Manager::Focus_Stay(const _double& TimeDelta)
{
	if (m_CurrentLookTime >= m_Desc.lookTime)
	{
		m_eCurState = MANAGER_CAMERA_FOCUS_OUT;
		return;
	}
	m_CurrentLookTime += (_float)TimeDelta;

}

void CWorldMap_Manager::Focus_Out(const _double& TimeDelta)
{
	if (p_MainCamera->Go_To_DefaultPosition(XMLoadFloat4(&m_FocusPosition), (_float)TimeDelta, 0.2f))
	{
		m_eCurState = MANAGER_IDLE;
	}
}

_bool CWorldMap_Manager::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	Value ManagerDesc(kObjectType);
	{
		Value pokemonFocusOffset(kObjectType);
		{
			pokemonFocusOffset.AddMember("x", m_Desc.pokemonFocusOffset.x, allocator);
			pokemonFocusOffset.AddMember("y", m_Desc.pokemonFocusOffset.y, allocator);
			pokemonFocusOffset.AddMember("z", m_Desc.pokemonFocusOffset.z, allocator);
			pokemonFocusOffset.AddMember("w", m_Desc.pokemonFocusOffset.w, allocator);
		}
		ManagerDesc.AddMember("pokemonFocusOffset", pokemonFocusOffset, allocator);

		ManagerDesc.AddMember("pokemonFocusStayTime", m_Desc.pokemonFocusStayTime, allocator);

		ManagerDesc.AddMember("pokemonFocusLimitDistance", m_Desc.pokemonFocusLimitDistance, allocator);
		ManagerDesc.AddMember("hideCommonUiTime", m_Desc.hideCommonUiTime, allocator);
		ManagerDesc.AddMember("visitInterval", m_Desc.visitInterval, allocator);
		ManagerDesc.AddMember("lookTime", m_Desc.lookTime, allocator);

	}
	doc.AddMember("ManagerDesc", ManagerDesc, allocator);


	return true;
}

_bool CWorldMap_Manager::Load_By_JsonFile_Impl(Document& doc)
{
	const Value& ManagerDesc = doc["ManagerDesc"];

	const Value& pokemonFocusOffset = ManagerDesc["pokemonFocusOffset"];
	m_Desc.pokemonFocusOffset.x = pokemonFocusOffset["x"].GetFloat();
	m_Desc.pokemonFocusOffset.y = pokemonFocusOffset["y"].GetFloat();
	m_Desc.pokemonFocusOffset.z = pokemonFocusOffset["z"].GetFloat();
	m_Desc.pokemonFocusOffset.w = pokemonFocusOffset["w"].GetFloat();

	m_Desc.pokemonFocusLimitDistance = ManagerDesc["pokemonFocusLimitDistance"].GetFloat();
	m_Desc.pokemonFocusStayTime = ManagerDesc["pokemonFocusStayTime"].GetFloat();

	m_Desc.hideCommonUiTime = ManagerDesc["hideCommonUiTime"].GetFloat();
	m_Desc.lookTime = ManagerDesc["lookTime"].GetFloat();
	m_Desc.visitInterval = ManagerDesc["visitInterval"].GetFloat();

	return true;
}

void CWorldMap_Manager::State_Tick(const _double& TimeDelta)
{
	switch (m_eCurState)
	{
	case MANAGER_IDLE:
		break;
	case MANAGER_CAMERA_FOCUS_IN:
		Focus_In(TimeDelta);
		break;
	case MANAGER_CAMERA_FOCUS_STAY:
		Focus_Stay(TimeDelta);
		break;
	case MANAGER_CAMERA_FOCUS_OUT:
		Focus_Out(TimeDelta);
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
			p_MainCamera->Control_On();
			break;
		case MANAGER_CAMERA_FOCUS_IN:
			p_MainCamera->Control_Off();
			break;
		case MANAGER_CAMERA_FOCUS_STAY:
			m_CurrentLookTime = 0.f;
			break;
		case MANAGER_CAMERA_FOCUS_OUT:
			p_MainCamera->CurrentMoveValut_Init();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CWorldMap_Manager::Picking()
{
	if (MOUSE_TAB(MOUSE::LBTN) && CClient_Utility::Mouse_Pos_In_Platform() && m_eCurState == MANAGER_IDLE)
	{
		m_pPickingObject = m_pCalculator->Picking_Environment_Object(g_hWnd, L"Layer_Monster", LEVEL_BASECAMP);
		if (nullptr == m_pPickingObject)
			return;

		CTransform* pTransform = m_pPickingObject->Get_As<CTransform>();
		if (nullptr == pTransform)
			return;

		XMStoreFloat4(&m_FocusPosition, pTransform->Get_State(CTransform::STATE_POSITION));

		m_eCurState = MANAGER_CAMERA_FOCUS_IN;
	}
}

HRESULT CWorldMap_Manager::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Calculator */
	if (FAILED(pGameInstance->Add_Component(CCalculator::familyId, this, LEVEL_BASECAMP, TEXT("Prototype_Component_Calculator"),
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
	if (FAILED(pGameInstance->Add_Component(CCalculator::familyId, this, LEVEL_BASECAMP, TEXT("Prototype_Component_Calculator"),
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
