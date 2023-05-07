#include "stdafx.h"
#include "Cauldron.h"

#include "GameInstance.h"

CCauldron::CCauldron(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CCauldron::CCauldron(const CCauldron& rhs)
	: CGameObject(rhs)
{

}

HRESULT CCauldron::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCauldron::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (nullptr != pArg)
	{
		m_Desc = (*(CAULDRON_DESC*)(pArg));
	}

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eCurState = (STATE)m_Desc.eState;

	m_pTransformCom->Set_Pos(m_Desc.vPos.x, m_Desc.vPos.y, m_Desc.vPos.z);
	m_pTransformCom->Set_Scaled({ 0.2f, 0.2f, 0.2f });

	m_eRenderId = RENDER_NONBLEND;

	if (m_eCurState != STATE_IDLE)
	{
		m_pModelCom->Set_Animation(0);
	}

	return S_OK;
}

_uint CCauldron::Tick(_double TimeDelta)
{
	if (m_eCurState != STATE_IDLE)
	{
		m_pModelCom->Play_Animation(TimeDelta);
	}

	return _uint();
}

_uint CCauldron::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	m_pRendererCom->Add_RenderGroup(RENDER_LAPLACIAN, this);

	return _uint();
}

HRESULT CCauldron::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		/*if (m_eCurState != STATE_IDLE)
		{
			if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
				return E_FAIL;
		}*/

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}


	return S_OK;
}

HRESULT CCauldron::Render_Laplacian()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		/*if (m_eCurState != STATE_IDLE)
		{
			if (FAILED(m_pModelCom->SetUp_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
				return E_FAIL;
		}*/

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}


	return S_OK;
}


HRESULT CCauldron::Add_Components()
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

	if (m_Desc.eState != STATE_IDLE)
	{
		if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
			(CComponent**)&m_pShaderCom, nullptr)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
			(CComponent**)&m_pShaderCom, nullptr)))
			return E_FAIL;
	}

	switch (m_Desc.eState)
	{
	case STATE_BREAK:
		if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_break",
			(CComponent**)&m_pModelCom, nullptr)))
			return E_FAIL;
		break;
	case STATE_CHANGE:
		if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_change",
			(CComponent**)&m_pModelCom, nullptr)))
			return E_FAIL;
		break;
	case STATE_CLOSE:
		if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_close",
			(CComponent**)&m_pModelCom, nullptr)))
			return E_FAIL;
		break;
	case STATE_COOK:
		if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_cook",
			(CComponent**)&m_pModelCom, nullptr)))
			return E_FAIL;
		break;
	case STATE_FIX:
		if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_fix",
			(CComponent**)&m_pModelCom, nullptr)))
			return E_FAIL;
		break;
	case STATE_IDLE:
		if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_idle",
			(CComponent**)&m_pModelCom, nullptr)))
			return E_FAIL;
		break;
	case STATE_PRESS:
		if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_press",
			(CComponent**)&m_pModelCom, nullptr)))
			return E_FAIL;
		break;
	case STATE_SELECT:
		if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_select",
			(CComponent**)&m_pModelCom, nullptr)))
			return E_FAIL;
		break;
	case STATE_SET:
		if (FAILED(pGameInstance->Add_Component(CModel::familyId, this, LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_set",
			(CComponent**)&m_pModelCom, nullptr)))
			return E_FAIL;
		break;
	}

	return S_OK;
}


HRESULT CCauldron::SetUp_ShaderResources()
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

CCauldron* CCauldron::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCauldron* pInstance = new CCauldron(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CCauldron");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCauldron::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CCauldron* pInstance = new CCauldron(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CCauldron");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CCauldron::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
