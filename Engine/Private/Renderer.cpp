#include "..\Public\Renderer.h"
#include "GameObject.h"

#include "Target_Manager.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);

}

HRESULT CRenderer::Initialize_Prototype()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	_uint				iNumViewports = 1;
	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof ViewportDesc);

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Diffuse"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM)))
		return E_FAIL;

	/* For.Target_Normal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Normal"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM)))
		return E_FAIL;

	/* For.Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Shade"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.0f, 1.f));

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 100.f, 100.f, 200.f, 200.f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 100.f, 300.f, 200.f, 200.f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 300.f, 100.f, 200.f, 200.f)))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	if (eRenderGroup >= RENDER_END ||
		nullptr == pGameObject)
		return E_FAIL;

	m_RenderGroups[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	if (FAILED(Draw_Priority()))
		return E_FAIL;
	if (FAILED(Draw_NonBlend()))
		return E_FAIL;
	if (FAILED(Draw_NonLight()))
		return E_FAIL;
	if (FAILED(Draw_Blend()))
		return E_FAIL;
	if (FAILED(Draw_Back_UI()))
		return E_FAIL;
	if (FAILED(Draw_UI()))
		return E_FAIL;
	if (FAILED(Draw_Blend_UI()))
		return E_FAIL;

#ifdef _DEBUG
	//if (FAILED(Render_Debug()))
	//	return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderer::Draw_Priority()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Draw_NonBlend()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_NONBLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_NONBLEND].clear();

	//m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Deferred"));

	//for (auto& pGameObject : m_RenderGroups[RENDER_NONBLEND])
	//{
	//	if (nullptr != pGameObject)
	//		pGameObject->Render();

	//	Safe_Release(pGameObject);
	//}

	//m_RenderGroups[RENDER_NONBLEND].clear();

	//m_pTarget_Manager->End_MRT(m_pContext);

	return S_OK;
}

HRESULT CRenderer::Draw_NonLight()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_NONLIGHT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Draw_Blend()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_BLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Draw_Back_UI()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_BACK_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_BACK_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Draw_UI()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Draw_Blend_UI()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_BLEND_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_BLEND_UI].clear();

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pTarget_Manager->Render_MRT(TEXT("MRT_Deferred"), m_pShader, m_pVIBuffer);
	m_pTarget_Manager->Render_MRT(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer);

	return S_OK;
}
#endif // _DEBUG

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pContext, nullptr);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRenderer::Clone(CGameObject* pOwner, void* pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

	Safe_Release(m_pTarget_Manager);

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& pGameObject : m_RenderGroups[i])
			Safe_Release(pGameObject);

		m_RenderGroups[i].clear();
	}

	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);

}
