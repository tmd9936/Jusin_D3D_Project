#include "stdafx.h"
#include "Mouse.h"

#include "GameInstance.h"

#include "Loader.h"

CMouse::CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CMouse::CMouse(const CMouse& rhs)
	: CGameObject(rhs)
{

}

HRESULT CMouse::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMouse::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_BLEND_UI;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_pTransformCom->Set_Scaled({ 4.3f, 3.2f, 1.f  });

	m_pModelCom->Set_Animation(0);

	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_pTransformCom->Set_Pos(_float(ptMouse.x) - g_iWinSizeX * 0.5f, _float(-ptMouse.y) + g_iWinSizeY * 0.5f, 0.1f);

	return S_OK;
}

_uint CMouse::Tick(_double TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Mouse_Set_Position();

	Mouse_Move_Check();

	State_Tick(TimeDelta);

	Hide_TIme_Check(TimeDelta);

	return _uint();
}

_uint CMouse::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CMouse::Render()
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

		m_pShaderCom->Begin(2);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CMouse::State_Tick(const _double& TimeDelta)
{
	switch (m_eState)
	{
	case STATE_IDLE:
		if (MOUSE_TAB(MOUSE::LBTN))
		{
			Hide_State_Init();
			m_eState = STATE_CLICK;
		}
		break;
	case STATE_CLICK:
		if (m_pModelCom->Play_Animation(TimeDelta * 1.4))
		{
			m_eState = STATE_IDLE;
		}
		break;
	}
}

void CMouse::Mouse_Set_Position()
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_pTransformCom->Set_Pos(_float(ptMouse.x) - g_iWinSizeX * 0.5f, _float(-ptMouse.y) + g_iWinSizeY * 0.5f, 0.1f);
}

void CMouse::Hide_TIme_Check(const _double& TimeDelta)
{
	if (!m_Hide)
	{
		if (m_HideTimeAcc >= m_HideTime)
		{
			m_Hide = true;
			m_alpha = 0.f;
		}
		m_HideTimeAcc += TimeDelta;
	}

}

void CMouse::Hide_State_Init()
{
	m_alpha = 1.f;
	m_Hide = false;
	m_HideTimeAcc = 0.0;
}

void CMouse::Mouse_Move_Check()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	_long MouseMove = { 0 };
	if (MouseMove = pGameInstance->Get_MouseMove(CInput_Device::DIMS_X))
	{
		Hide_State_Init();
	}
	if (MouseMove = pGameInstance->Get_MouseMove(CInput_Device::DIMS_Y))
	{
		Hide_State_Init();
	}
}


HRESULT CMouse::Add_Components()
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

	/* For.Com_Model */
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Model_Mouse"),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;


	return S_OK;
}


HRESULT CMouse::SetUp_ShaderResources()
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

	m_pShaderCom->Set_RawValue("g_alpha", &m_alpha, sizeof(_float));

	Safe_Release(pGameInstance);

	return S_OK;
}

CMouse* CMouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMouse* pInstance = new CMouse(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CMouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMouse::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CMouse* pInstance = new CMouse(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CMouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CMouse::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
