#include "stdafx.h"
#include "PartTexture.h"

#include "GameInstance.h"

CPartTexture::CPartTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CPartTexture::CPartTexture(const CPartTexture& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPartTexture::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPartTexture::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (pArg != nullptr)
		memcpy(&m_UIDesc, pArg, (sizeof m_UIDesc) + 2);

	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_eRenderId = RENDER_UI;

	m_pTransformCom->Set_Scaled({ m_UIDesc.m_fSizeX, m_UIDesc.m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(m_UIDesc.m_fX, m_UIDesc.m_fY, 0.01f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

_uint CPartTexture::Tick(_double TimeDelta)
{
	return _uint();
}

_uint CPartTexture::LateTick(_double TimeDelta)
{
	if (m_UIDesc.pParent && m_UIDesc.pParentModel)
	{
		_matrix parent = m_UIDesc.pParent->Get_WorldMatrix_Matrix();
		REMOVE_SCALE(parent);

		_float4x4 vParentCombinedMatrix = m_UIDesc.pParentModel->Get_CombinedTransformationMatrix_float4_4(1);

		_float3 vScale = m_pTransformCom->Get_Scaled();
		XMStoreFloat4x4(&m_FinalWorldMatrix, XMMatrixSet(
			vScale.x, 0.f, 0.f, 0.f,
			0.f, vScale.y, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			m_UIDesc.m_fX, -m_UIDesc.m_fY, 0.f, 1.f
		));

		XMStoreFloat4x4(&m_FinalWorldMatrix, XMLoadFloat4x4(&m_FinalWorldMatrix) * 
			XMMatrixScaling(vParentCombinedMatrix.m[0][0], vParentCombinedMatrix.m[1][1], 1.f) * parent);
	}

	else if (m_UIDesc.pParent && !m_UIDesc.pParentModel)
	{
		_matrix parent = m_UIDesc.pParent->Get_WorldMatrix_Matrix();
		REMOVE_SCALE(parent);

		_float3 vScale = m_pTransformCom->Get_Scaled();
		XMStoreFloat4x4(&m_FinalWorldMatrix, XMMatrixSet(
			vScale.x, 0.f, 0.f, 0.f,
			0.f, vScale.y, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			m_UIDesc.m_fX, -m_UIDesc.m_fY, 0.f, 1.f
		));

		XMStoreFloat4x4(&m_FinalWorldMatrix, XMLoadFloat4x4(&m_FinalWorldMatrix) *
			  parent);
	}

	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	return _uint();
}

HRESULT CPartTexture::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPartTexture::Add_Components()
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


	if (m_UIDesc.m_eType == TYPE_TEXTURE)
	{
		/* For.Com_Shader */
		if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
			(CComponent**)&m_pShaderCom, nullptr)))
			return E_FAIL;
	}
	else if (m_UIDesc.m_eType == TYPE_COLOR_TEXTURE)
	{
		/* For.Com_Shader */
		if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexColor"),
			(CComponent**)&m_pShaderCom, nullptr)))
			return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, m_UIDesc.m_TextureProtoTypeLevel, m_UIDesc.m_TextureProtoTypeName,
		(CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPartTexture::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_FinalWorldMatrix)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", m_TextureNumber)))
		return E_FAIL;

	else if (m_UIDesc.m_eType == TYPE_COLOR_TEXTURE)
	{
		m_pShaderCom->Set_RawValue("g_vColor", &m_UIDesc.m_vColor, sizeof(_float4));
	}


	Safe_Release(pGameInstance);

	return S_OK;
}

CPartTexture* CPartTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPartTexture* pInstance = new CPartTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CPartTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPartTexture::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CPartTexture* pInstance = new CPartTexture(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CPartTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPartTexture::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);

}
