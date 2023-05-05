#include "..\Public\Renderer.h"
#include "GameObject.h"

#include "Target_Manager.h"
#include "Light_Manager.h"
#include "Graphic_Device.h"

#include "Shader.h"
#include "VIBuffer_Rect.h"

#include "PipeLine.h"

_bool		CRenderer::m_bLaplacian = true;
_float		CRenderer::m_laplacianThesholdLow = 0.39f;
_float		CRenderer::m_laplacianThesholdHigh = 0.71f;


CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
	: CComponent(pDevice, pContext)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pLight_Manager);
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
	/* 디퍼드 셰이드를 시작하기전에 버퍼를 클리어 할 때 스카이 박스와 같은 정보를 배제하지 않기위해
	알파를 0.f으로 처리하고 디퍼드 셰이드를 할 때 알파가 0.f인 부분을 discard함, 빛 계산으로 알파가 0.f이 된 부분이 있을 수 있으니
	그때는 알파를 1.f로 살림*/
	/* For.Target_Diffuse */

	m_iOriginCX = ViewportDesc.Width;
	m_iOriginCY = ViewportDesc.Height;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Diffuse"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_NonLightDiffuse"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Gray"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_GrayBlurX"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_GrayBlurY"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Laplacian"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Normal */
	/*
	* float이 4바이트인데 2바이트인데
	* DXGI_FORMAT_R16G16B16A16_UNORM로 데이터의 정보를 저장하는 이유는
	* 4바이트로 노말 정보를 받을 경우 나중에 프레임드랍이 발생할 수 있음
	*/
	/* 던저지는 데이터의 정보가 UNORM이여서 나중에 빛 정보를 이 타겟에 저장할 시 에는 
	0~1값으로 보정해서 데이터를 넘겨야함, 이후에 디퓨즈 정보와 연산을 할 때는 -1 ~ 1사이의 값으로 다시 보정해줘야함 */
	/* For.Target_Normal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Normal"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* For.Target_Depth */
	/* 현재 픽셀의 z정보를 저장해주는 타겟, Z정보에는 투영 기준의 z값과 
	뷰 기준의 z값을 축소시킨 값 (원래 값 / 카메라의 Far)*/
	/* 포멧을 32가 아닌16으로 하면 스펙큘러 계산시 지형같은 부분에 물결치는 현상이 발생할 수 있음 -> 이를 해결하기 위해 뎁스렌더타겟의 기본 포멧 크기를 32로 함  */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Depth"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_Shade"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_BrightColor"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_BlurX"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_BlurY"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	m_iShadowMapCX = (_uint)ViewportDesc.Width * 10;
	m_iShadowMapCY = (_uint)ViewportDesc.Height * 10;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
		TEXT("Target_ShadowDepth"), m_iShadowMapCX, m_iShadowMapCY, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(Ready_DepthStencilRenderTargetView(m_iShadowMapCX, m_iShadowMapCY)))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
	//	TEXT("Target_BloomColor"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
	//	return E_FAIL;

	/* For.Target_Specular */
	/* 스펙큘러 값을 저장해서 받아오는 타겟 */
	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext,
	//	TEXT("Target_Specular"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
	//	return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
	//	return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightDepth"), TEXT("Target_ShadowDepth"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_NonLight_Deferred"), TEXT("Target_NonLightDiffuse"))))
		return E_FAIL;

	// === 이펙트 디퍼드
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Bright"), TEXT("Target_BrightColor"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BlurX"), TEXT("Target_BlurX"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BlurY"), TEXT("Target_BlurY"))))
		return E_FAIL;

	// ==  외곽선 검출

	// === 이펙트 디퍼드
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Gray"), TEXT("Target_Gray"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GrayBlurX"), TEXT("Target_GrayBlurX"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GrayBlurY"), TEXT("Target_GrayBlurY"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Laplacian"), TEXT("Target_Laplacian"))))
		return E_FAIL;
	

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.0f, 1.f));

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_Deferred.hlsl"), 
		VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

#ifdef RENDERTARGET_DEBUG
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 50.f, 50.f, 100.f, 100.f)))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 50.f, 150.f, 100.f, 100.f)))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), 50.f, 250.f, 100.f, 100.f)))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 150.f, 50.f, 100.f, 100.f)))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BrightColor"), 250.f, 50.f, 100.f, 100.f)))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BlurX"), 250.f, 150.f, 100.f, 100.f)))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BlurY"), 250.f, 250.f, 100.f, 100.f)))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"), 150.f, 150.f, 100.f, 100.f)))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Gray"), 50.f, 150.f, 100.f, 100.f)))
	//	return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_GrayBlurX"), 50.f, 250.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_GrayBlurY"), 50.f, 350.f, 100.f, 100.f)))
		return E_FAIL;*/

	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Laplacian"), 150.f, 50.f, 100.f, 100.f)))
	//	return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ShadowDepth"), 150.f, 50.f, 100.f, 100.f)))
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
	if (eRenderGroup >= RENDER_END || eRenderGroup < 0 ||
		nullptr == pGameObject)
		return E_FAIL;

	m_RenderGroups[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_DebugRenderGroup(CComponent* pComponent)
{
#ifdef DEBUG_COMPONENT_RENDER

	if (nullptr == pComponent)
		return E_FAIL;

	m_DebugGroup.push_back(pComponent);

	Safe_AddRef(pComponent);

#endif

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	if (FAILED(Draw_Priority()))
		return E_FAIL;

	if (FAILED(Draw_ShadowDepth()))
		return E_FAIL;

	if (FAILED(Draw_NonBlend()))
		return E_FAIL;

	if (FAILED(Draw_LightAcc()))
		return E_FAIL;

	if (FAILED(Draw_Gray()))
		return E_FAIL;

	//if (FAILED(Draw_GrayBlurX()))
	//	return E_FAIL;

	//if (FAILED(Draw_GrayBlurY()))
	//	return E_FAIL;

	if (FAILED(Draw_Laplacian()))
		return E_FAIL;
	
	if (FAILED(Draw_NonLight()))
		return E_FAIL;

	if (FAILED(Draw_DefferdBright()))
		return E_FAIL;

	if (FAILED(Draw_BlurX()))
		return E_FAIL;

	if (FAILED(Draw_BlurY()))
		return E_FAIL;

	if (FAILED(Draw_DeferredNonLightBlend()))
		return E_FAIL;

	if (FAILED(Draw_Blend()))
		return E_FAIL;

#ifdef DEBUG_COMPONENT_RENDER
	//if (FAILED(Draw_DebugComponent()))
	//	return E_FAIL;
#endif

	if (FAILED(Draw_World_UI()))
		return E_FAIL;

	if (FAILED(Draw_Back_UI()))
		return E_FAIL;

	if (FAILED(Draw_UI()))
		return E_FAIL;

	if (FAILED(Draw_Blend_UI()))
		return E_FAIL;

#ifdef RENDERTARGET_DEBUG
	if (FAILED(Render_Debug()))
		return E_FAIL;
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

HRESULT CRenderer::Draw_ShadowDepth()
{
	m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_LightDepth"), m_pShadow_DS_Surface);

	D3D11_VIEWPORT ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)m_iShadowMapCX;
	ViewPortDesc.Height = (_float)m_iShadowMapCY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);

	m_pContext->ClearDepthStencilView(m_pShadow_DS_Surface, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	for (auto& pGameObject : m_RenderGroups[RENDER_SHADOWDEPTH])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_ShadowDepth();
		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_SHADOWDEPTH].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}


HRESULT CRenderer::Draw_NonBlend()
{
	/*
		알파가 들어간 지형은 여기에 그려주면 안됨
	*/

	/*
	기존의 그려졌던 Priority에 그려진 스카이박스 부분은 조명연산을 해주면 안되어서 나중에 배제 하고 
	후처리 하려했던 Nonblend에 그려질 객체들의 정보만 가져와서 후처리를 해줘야함 
	*/

	//for (auto& pGameObject : m_RenderGroups[RENDER_NONBLEND])
	//{
	//	if (nullptr != pGameObject)
	//		pGameObject->Render();

	//	Safe_Release(pGameObject);
	//}

	//m_RenderGroups[RENDER_NONBLEND].clear();

	m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Deferred"));


	D3D11_VIEWPORT ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)m_iOriginCX;
	ViewPortDesc.Height = (_float)m_iOriginCY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);


	for (auto& pGameObject : m_RenderGroups[RENDER_NONBLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_NONBLEND].clear();

	m_pTarget_Manager->End_MRT(m_pContext);

	return S_OK;
}

HRESULT CRenderer::Draw_NonLight()
{
	m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_NonLight_Deferred"));

	for (auto& pGameObject : m_RenderGroups[RENDER_NONLIGHT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroups[RENDER_NONLIGHT].clear();

	m_pTarget_Manager->End_MRT(m_pContext);

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

HRESULT CRenderer::Draw_World_UI()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_WORLD_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroups[RENDER_WORLD_UI].clear();

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

HRESULT CRenderer::Draw_DebugComponent()
{
#ifdef DEBUG_COMPONENT_RENDER


	for (auto& pDebugCom : m_DebugGroup)
	{
		pDebugCom->Render();
		Safe_Release(pDebugCom);
	}
	m_DebugGroup.clear();
#endif

	return S_OK;
}

//HRESULT CRenderer::Draw_ShadowDepth()
//{
//	m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_LightDepth"), m_pShadow_DS_Surface);
//
//	m_pContext->ClearDepthStencilView(m_pShadow_DS_Surface, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
//
//	//_float4x4 lightView = m_pLight_Manager->Get_ShadowDepthLightProj();
//	//_float4x4 lightProj = m_pLight_Manager->Get_ShadowDepthLightProj();
//
//	/* Shade 타겟에 그리는 작업을 수행한다. */
//	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
//		return E_FAIL;
//	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix)))
//		return E_FAIL;
//	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix)))
//		return E_FAIL;
//
//	CPipeLine* pPipeLine = CPipeLine::GetInstance();
//
//	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrixInv", &pPipeLine->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW))))
//		return E_FAIL;
//
//	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrixInv", &pPipeLine->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_PROJ))))
//		return E_FAIL;
//
//	//if (FAILED(m_pShader->Set_Matrix("g_matLightView", &lightView)))
//	//	return E_FAIL;
//
//	//if (FAILED(m_pShader->Set_Matrix("g_matLightProj", &lightProj)))
//	//	return E_FAIL;
//
//	_float cameraFar = pPipeLine->Get_CameraFar();
//	if (FAILED(m_pShader->Set_RawValue("g_CameraFar", &cameraFar, sizeof(_float))))
//		return E_FAIL;
//
//	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
//		return E_FAIL;
//
//	m_pShader->Begin(10);
//
//	m_pVIBuffer->Render();
//
//	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
//		return E_FAIL;
//
//	return S_OK;
//}


HRESULT CRenderer::Draw_LightAcc()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* Shade */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_LightAcc"))))
		return E_FAIL;

	/* Shade 타겟에 그리는 작업을 수행한다. */
	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrixInv", &pPipeLine->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrixInv", &pPipeLine->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_vCamPosition", &pPipeLine->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	_float cameraFar = pPipeLine->Get_CameraFar();
	if (FAILED(m_pShader->Set_RawValue("g_CameraFar", &cameraFar, sizeof(_float))))
		return E_FAIL;

	Safe_Release(pPipeLine);

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;

	m_pLight_Manager->Render(m_pShader, m_pVIBuffer);

	/* 따로 빼놨던 백버퍼를 장치에 복구한다. */
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::Draw_DeferredBlend()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	/* 백버퍼에 그린다. */
	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
	//	return E_FAIL;

	m_pShader->Begin(3);

	m_pVIBuffer->Render();

	return S_OK;
}

//HRESULT CRenderer::Draw_Blur()
//{
//	if (nullptr == m_pTarget_Manager)
//		return E_FAIL;
//
//	/* Blur */
//	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blur"))))
//		return E_FAIL;
//
//	/* Shade 타겟에 그리는 작업을 수행한다. */
//	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
//		return E_FAIL;
//	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix)))
//		return E_FAIL;
//	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix)))
//		return E_FAIL;
//
//	_uint				iNumViewports = 1;
//	D3D11_VIEWPORT		ViewportDesc;
//	ZeroMemory(&ViewportDesc, sizeof ViewportDesc);
//
//	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
//
//	_float2 g_TexSize = { ViewportDesc.Width, ViewportDesc.Height };
//	if (FAILED(m_pShader->Set_RawValue("g_TexSize", &g_TexSize, sizeof(_float2))))
//		return E_FAIL;
//
//	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_BloomColor"), m_pShader, "g_BloomTexture")))
//		return E_FAIL;
//
//	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_BloomOrigin"), m_pShader, "g_BloomOriginTexture")))
//		return E_FAIL;
//
//	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
//		return E_FAIL;
//
//	return S_OK;
//}


HRESULT CRenderer::Draw_DefferdBright()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* Blur */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Bright"))))
		return E_FAIL;

	/* Shade 타겟에 그리는 작업을 수행한다. */
	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	//g_NonLightDiffuseTexture
	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_NonLightDiffuse"), m_pShader, "g_NonLightDiffuseTexture")))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
	//	return E_FAIL;

	m_pShader->Begin(4);

	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Draw_BlurX()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* Blur */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BlurX"))))
		return E_FAIL;

	/* Shade 타겟에 그리는 작업을 수행한다. */
	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_uint				iNumViewports = 1;
	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof ViewportDesc);

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	_float2 g_TexSize = { ViewportDesc.Width, ViewportDesc.Height };
	if (FAILED(m_pShader->Set_RawValue("g_TexSize", &g_TexSize, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_BrightColor"), m_pShader, "g_BrightTexture")))
		return E_FAIL;

	m_pShader->Begin(5);

	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Draw_BlurY()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* Blur */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BlurY"))))
		return E_FAIL;

	/* Shade 타겟에 그리는 작업을 수행한다. */
	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_uint				iNumViewports = 1;
	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof ViewportDesc);

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	_float2 g_TexSize = { ViewportDesc.Width, ViewportDesc.Height };
	if (FAILED(m_pShader->Set_RawValue("g_TexSize", &g_TexSize, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_BlurX"), m_pShader, "g_BlurXTexture")))
		return E_FAIL;

	m_pShader->Begin(6);

	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Draw_DeferredNonLightBlend()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	_uint				iNumViewports = 1;
	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof ViewportDesc);

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.0f, 1.f));

	/* 백버퍼에 그린다. */
	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_NonLightDiffuse"), m_pShader, "g_NonLightDiffuseTexture")))
	//	return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_BlurY"), m_pShader, "g_BloomTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_BrightColor"), m_pShader, "g_BloomOriginTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Laplacian"), m_pShader, "g_LaplacianTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_ShadowDepth"), m_pShader, "g_ShadowDepthTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrixInv", &pPipeLine->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrixInv", &pPipeLine->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_vCamPosition", &pPipeLine->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	_float cameraFar = pPipeLine->Get_CameraFar();
	if (FAILED(m_pShader->Set_RawValue("g_CameraFar", &cameraFar, sizeof(_float))))
		return E_FAIL;

	Safe_Release(pPipeLine);


	_float4x4 lightView = m_pLight_Manager->Get_ShadowDepthLightView();
	_float4x4 lightProj = m_pLight_Manager->Get_ShadowDepthLightProj();

	if (FAILED(m_pShader->Set_Matrix("g_matLightView", &lightView)))
		return E_FAIL;

	if (FAILED(m_pShader->Set_Matrix("g_matLightProj", &lightProj)))
		return E_FAIL;


	m_pShader->Begin(7);

	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CRenderer::Draw_Gray()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* Blur */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Gray"))))
		return E_FAIL;

	/* Shade 타겟에 그리는 작업을 수행한다. */
	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return E_FAIL;

	m_pShader->Begin(8);

	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Draw_GrayBlurX()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* Blur */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GrayBlurX"))))
		return E_FAIL;

	/* Shade 타겟에 그리는 작업을 수행한다. */
	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_uint				iNumViewports = 1;
	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof ViewportDesc);

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	_float2 g_TexSize = { ViewportDesc.Width, ViewportDesc.Height };
	if (FAILED(m_pShader->Set_RawValue("g_TexSize", &g_TexSize, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Gray"), m_pShader, "g_BrightTexture")))
		return E_FAIL;

	m_pShader->Begin(5);

	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Draw_GrayBlurY()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* Blur */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GrayBlurY"))))
		return E_FAIL;

	/* Shade 타겟에 그리는 작업을 수행한다. */
	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_uint				iNumViewports = 1;
	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof ViewportDesc);

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	_float2 g_TexSize = { ViewportDesc.Width, ViewportDesc.Height };
	if (FAILED(m_pShader->Set_RawValue("g_TexSize", &g_TexSize, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_GrayBlurX"), m_pShader, "g_BlurXTexture")))
		return E_FAIL;

	m_pShader->Begin(6);

	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}


HRESULT CRenderer::Draw_Laplacian()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	_uint				iNumViewports = 1;
	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof ViewportDesc);

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width * 3.f, ViewportDesc.Height * 3.f, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width * 3.f, ViewportDesc.Height * 3.f, 0.0f, 1.f));

	/* Blur */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Laplacian"))))
		return E_FAIL;

	/* Shade 타겟에 그리는 작업을 수행한다. */
	if (FAILED(m_pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float2 g_TexSize = { ViewportDesc.Width * 3.f, ViewportDesc.Height * 3.f };
	if (FAILED(m_pShader->Set_RawValue("g_TexSize", &g_TexSize, sizeof(_float2))))
		return E_FAIL;

	_float4 g_Laplacian = { 0.f, 0.f, 0.f, _float(m_bLaplacian) };
	if (FAILED(m_pShader->Set_RawValue("g_bLaplacian", &g_Laplacian, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_laplacianThesholdLow", &m_laplacianThesholdLow, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_laplacianThesholdHigh", &m_laplacianThesholdHigh, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Gray"), m_pShader, "g_BlurYTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Set_ShaderResourceView(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return E_FAIL;

	m_pShader->Begin(9);

	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

#ifdef RENDERTARGET_DEBUG
HRESULT CRenderer::Render_Debug()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	m_pShader->Set_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Set_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pTarget_Manager->Render_MRT(TEXT("MRT_Deferred"), m_pShader, m_pVIBuffer);
	//m_pTarget_Manager->Render_MRT(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer);
	//m_pTarget_Manager->Render_MRT(TEXT("MRT_Bright"), m_pShader, m_pVIBuffer);
	//m_pTarget_Manager->Render_MRT(TEXT("MRT_BlurX"), m_pShader, m_pVIBuffer);
	//m_pTarget_Manager->Render_MRT(TEXT("MRT_BlurY"), m_pShader, m_pVIBuffer);

	//m_pTarget_Manager->Render_MRT(TEXT("MRT_Gray"), m_pShader, m_pVIBuffer);
	//m_pTarget_Manager->Render_MRT(TEXT("MRT_Laplacian"), m_pShader, m_pVIBuffer);
	//m_pTarget_Manager->Render_MRT(TEXT("MRT_GrayBlurX"), m_pShader, m_pVIBuffer);
	//m_pTarget_Manager->Render_MRT(TEXT("MRT_GrayBlurY"), m_pShader, m_pVIBuffer);

	m_pTarget_Manager->Render_MRT(TEXT("MRT_LightDepth"), m_pShader, m_pVIBuffer);

	return S_OK;
}
#endif // _DEBUG


HRESULT CRenderer::Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWinCX;
	TextureDesc.Height = iWinCY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL /*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;
	

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pShadow_DS_Surface)))
		return E_FAIL;

	//g_pDepthStencilView = m_pDepthStencilView;
	//Safe_AddRef(g_pDepthStencilView);

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

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

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);

	for (auto& pComponent : m_DebugGroup)
	{
		Safe_Release(pComponent);
	}
	m_DebugGroup.clear();

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& pGameObject : m_RenderGroups[i])
			Safe_Release(pGameObject);

		m_RenderGroups[i].clear();
	}

;	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShadow_DS_Surface);

}
