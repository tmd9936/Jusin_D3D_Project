#include "stdafx.h"
#include "Button.h"

#include "GameInstance.h"

CButton::CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CButton::CButton(const CButton& rhs)
	: CGameObject(rhs)
{
}

HRESULT CButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (pArg != nullptr)
		memcpy(&m_UIDesc, pArg, (sizeof m_UIDesc) + 2);

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_UI;	

 	m_pTransformCom->Set_Scaled({ m_UIDesc.m_fSizeX, m_UIDesc.m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(m_UIDesc.m_fX - g_iWinSizeX * 0.5f, -m_UIDesc.m_fY + g_iWinSizeY * 0.5f, 0.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_pModelCom->Set_Animation(0);

	return S_OK;
}

_uint CButton::Tick(_double TimeDelta)
{
	Button_Motion(TimeDelta);
	Picking_Button();
	Change_State();

	return _uint();
}

_uint CButton::LateTick(_double TimeDelta)
{
	m_TransformMatrix = m_pModelCom->Get_CombinedTransformationMatrix_float4_4(1);
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CButton::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_float viewZ = m_pModelCom->Get_ViewZ(i);
		m_pTransformCom->Set_PosZ(viewZ);

		if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix())))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_ShaderResource(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;


		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);

	}

	return S_OK;
}

HRESULT CButton::Add_Components()
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
	if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, m_UIDesc.m_eModelPrototypLevel, TEXT("Prototype_Component_Model_Button_Base"),
		(CComponent**)&m_pModelCom, nullptr)))
		return E_FAIL;

	if (strlen(m_UIDesc.m_DiffuseTextureName) > 0)
		m_pModelCom->Set_Texture_In_Material(0, 1, m_UIDesc.m_DiffuseTextureName);

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, m_UIDesc.m_ShaderLevelIndex, TEXT("Prototype_Component_Shader_VtxtexButton"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	//if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
	//	(CComponent**)&m_pTextureCom, nullptr)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CButton::SetUp_ShaderResources()
{
	//if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&m_ProjMatrix)))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
	//	return E_FAIL;	

	Safe_Release(pGameInstance);

	return S_OK;
}

void CButton::Button_Motion(_double TimeDelta)
{

	switch (m_eCurState)
	{
	case CButton::BUTTON_IDLE:
		m_pModelCom->Play_Animation(TimeDelta);
		m_pTransformCom->Set_Scaled({ m_UIDesc.m_fSizeX,  m_UIDesc.m_fSizeY, 1.f });
		break;
	case CButton::BUTTON_PRESS:
		m_pTransformCom->Set_Scaled({ m_UIDesc.m_fSizeX * m_TransformMatrix.m[0][0],  m_UIDesc.m_fSizeY * m_TransformMatrix.m[1][1], 1.f });
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_eCurState = BUTTON_SELECT;
		}
		break;
	case CButton::BUTTON_RELEASE:
		m_pTransformCom->Set_Scaled({ m_UIDesc.m_fSizeX * m_TransformMatrix.m[0][0],  m_UIDesc.m_fSizeY * m_TransformMatrix.m[1][1], 1.f });
		if (m_pModelCom->Play_Animation(TimeDelta))
		{
			m_eCurState = BUTTON_IDLE;
		}
		break;
	case CButton::BUTTON_SELECT:
		m_pModelCom->Play_Animation(TimeDelta);
		//m_pTransformCom->Set_Scaled({ m_UIDesc.m_fSizeX * m_TransformMatrix.m[0][0],  m_UIDesc.m_fSizeY * m_TransformMatrix.m[1][1], 1.f });
		break;
	}
}

void CButton::Picking_Button()
{
	if (MOUSE_TAB(MOUSE::LBTN) && m_eCurState == BUTTON_IDLE)
	{
		RECT uiRect{ LONG(m_UIDesc.m_fX - m_UIDesc.m_fSizeX * 0.5f), LONG(m_UIDesc.m_fY - m_UIDesc.m_fSizeY * 0.5f)
		, LONG(m_UIDesc.m_fX + m_UIDesc.m_fSizeX * 0.5f),  LONG(m_UIDesc.m_fY + m_UIDesc.m_fSizeY * 0.5f) };

		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		RECT mouseRect{ pt.x - 5, pt.y - 5, pt.x + 5, pt.y + 5 };

		RECT result{};
		if (IntersectRect(&result, &uiRect, &mouseRect))
		{
			m_eCurState = BUTTON_PRESS;
		}
	}
	else if (MOUSE_HOLD(MOUSE::LBTN) && m_eCurState == BUTTON_SELECT)
	{
		m_eCurState = BUTTON_SELECT;
	}

	else if (MOUSE_NONE(MOUSE::LBTN) && m_eCurState == BUTTON_SELECT)
	{
		m_eCurState = BUTTON_RELEASE;
	}
}

void CButton::Change_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CButton::BUTTON_IDLE:
			m_pModelCom->Set_Animation(BUTTON_IDLE);
			break;
		case CButton::BUTTON_PRESS:
			m_pModelCom->Set_Animation(BUTTON_PRESS);
			break;
		case CButton::BUTTON_RELEASE:
			m_pModelCom->Set_Animation(BUTTON_RELEASE);
			break;
		case CButton::BUTTON_SELECT:
			m_pModelCom->Set_Animation(BUTTON_SELECT);
			break;
		}
		m_ePreState = m_eCurState;

	}
}

CButton* CButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CButton* pInstance = new CButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CButton* pInstance = new CButton(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);

}
