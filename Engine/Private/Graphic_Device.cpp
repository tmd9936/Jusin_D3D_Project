#include "..\public\Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)

//_bool		CGraphic_Device::g_bRelease = false;
//
//ID3D11DepthStencilView* CGraphic_Device::g_pDepthStencilView = nullptr;
//ID3D11RenderTargetView* CGraphic_Device::g_pBackBufferRTV = nullptr;
//IDXGISwapChain* CGraphic_Device::g_pSwapChain = nullptr;
//ID3D11Device* CGraphic_Device::g_pDevice = nullptr;

CGraphic_Device::CGraphic_Device()
	: m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
{


}

//CGraphic_Device::~CGraphic_Device()
//{
//	g_bRelease = true;
//	if (!g_bRelease)
//	{
//		Safe_Release(g_pSwapChain);
//		Safe_Release(g_pDepthStencilView);
//		Safe_Release(g_pBackBufferRTV);
//		Safe_Release(g_pDevice);
//	}
//
//}

HRESULT CGraphic_Device::Ready_Graphic_Device(HWND hWnd, GRAPHIC_DESC::WINMODE eWinMode, _uint iWinCX, _uint iWinCY, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL			FeatureLV;

	/* 그래픽 장치를 초기화한다. */
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, iFlag, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &FeatureLV, &m_pDeviceContext)))
		return E_FAIL;

	/* SwapChain 전면과 후면버퍼를 번갈아가며 화면에 보여준다.(Present) */

	/* 백버퍼는만든거야. */
	if (FAILED(Ready_SwapChain(hWnd, eWinMode, iWinCX, iWinCY)))
		return E_FAIL;

	/* 백버퍼 뷰 를 만든다.*/
	if (FAILED(Ready_BackBufferRenderTargetView()))
		return E_FAIL;

	if (FAILED(Ready_DepthStencilRenderTargetView(iWinCX, iWinCY)))
		return E_FAIL;

	/* 장치에 바인드해놓을 렌더타겟들과 뎁스스텐실뷰를 셋팅한다. */
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBufferRTV,
		m_pDepthStencilView);

	//D3D11_VIEWPORT*			ViewPortDesc = new D3D11_VIEWPORT[2];
	//ZeroMemory(&ViewPortDesc[0], sizeof(D3D11_VIEWPORT));
	//ViewPortDesc[0].TopLeftX = 0;
	//ViewPortDesc[0].TopLeftY = 0;
	//ViewPortDesc[0].Width = (_float)iWinCX;
	//ViewPortDesc[0].Height = (_float)iWinCY;
	//ViewPortDesc[0].MinDepth = 0.f;
	//ViewPortDesc[0].MaxDepth = 1.f;

	//ZeroMemory(&ViewPortDesc[1], sizeof(D3D11_VIEWPORT));
	//ViewPortDesc[1].TopLeftX = 0;
	//ViewPortDesc[1].TopLeftY = 0;
	//ViewPortDesc[1].Width = (_float)300.f;
	//ViewPortDesc[1].Height = (_float)300.f;
	//ViewPortDesc[1].MinDepth = 0.f;
	//ViewPortDesc[1].MaxDepth = 1.f;

	D3D11_VIEWPORT ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)iWinCX;
	ViewPortDesc.Height = (_float)iWinCY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;


	m_pDeviceContext->RSSetViewports(1, &ViewPortDesc);

	*ppDeviceOut = m_pDevice;
	*ppDeviceContextOut = m_pDeviceContext;

	/*g_pDevice = m_pDevice;
	Safe_AddRef(g_pDevice);*/

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

HRESULT CGraphic_Device::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	//m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
	//	vClearColor, 1.f, 0)
	//

	/* 백버퍼를 초기화한다.  */
	m_pDeviceContext->ClearRenderTargetView(m_pBackBufferRTV, (_float*)&vClearColor);
	//m_pDeviceContext->ClearRenderTargetView(m_pBackBufferRTV[1], (_float*)&vClearColor);



	return S_OK;
}

HRESULT CGraphic_Device::Clear_DepthStencil_View()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	return S_OK;
}

HRESULT CGraphic_Device::Present(UINT interval, UINT flags)
{
	if (nullptr == m_pSwapChain)
		return E_FAIL;

	/* 전면 버퍼와 후면버퍼를 교체하여 후면버퍼를 전면으로 보여주는 역할을 한다. */
	/* 후면버퍼를 직접화면에 보여줄께. */
	return m_pSwapChain->Present(interval, flags);
}

HRESULT CGraphic_Device::SetRenderTargets(_bool bDepthSet)
{
	if (bDepthSet)
		m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBufferRTV, m_pDepthStencilView);
	else
		m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBufferRTV, nullptr);
	return S_OK;
}


HRESULT CGraphic_Device::Ready_SwapChain(HWND hWnd, GRAPHIC_DESC::WINMODE eWinMode, _uint iWinCX, _uint iWinCY)
{
	IDXGIDevice* pDevice = nullptr;
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice);

	IDXGIAdapter* pAdapter = nullptr;
	pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);

	IDXGIFactory* pFactory = nullptr;
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	/* 스왑체인을 생성한다. = 텍스쳐를 생성하는 행위 + 스왑하는 형태  */
	DXGI_SWAP_CHAIN_DESC		SwapChain;
	ZeroMemory(&SwapChain, sizeof(DXGI_SWAP_CHAIN_DESC));

	/*텍스쳐(백버퍼)를 생성하는 행위*/
	SwapChain.BufferDesc.Width = iWinCX;
	SwapChain.BufferDesc.Height = iWinCY;
	SwapChain.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChain.BufferCount = 1;

	/*스왑하는 형태*/
	SwapChain.BufferDesc.RefreshRate.Numerator = 60;
	SwapChain.BufferDesc.RefreshRate.Denominator = 1;
	SwapChain.SampleDesc.Quality = 0;
	SwapChain.SampleDesc.Count = 1;
	SwapChain.OutputWindow = hWnd;
	SwapChain.Windowed = eWinMode;
	SwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChain.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	/* 백버퍼라는 텍스쳐를 생성했다. */
	if (FAILED(pFactory->CreateSwapChain(m_pDevice, &SwapChain, &m_pSwapChain)))
		return E_FAIL;

	//Safe_AddRef(g_pSwapChain);
	//g_pSwapChain = m_pSwapChain;

	Safe_Release(pFactory);
	Safe_Release(pAdapter);
	Safe_Release(pDevice);

	return S_OK;
}


HRESULT CGraphic_Device::Ready_BackBufferRenderTargetView()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	/* 내가 앞으로 사용하기위한 용도의 객체를 생성하기위한 베이스 데이터를 가지고 있는 객체이다. */
	/* 내가 앞으로 사용하기위한 용도의 객체 : ID3D11RenderTargetView, ID3D11ShaderResoureView, ID3D11DepthStencilView */
	ID3D11Texture2D* pBackBufferTexture = nullptr;

	/* 스왑체인이 들고있던 텍스처를 가져와봐. */
	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(pBackBufferTexture, nullptr, &m_pBackBufferRTV)))
		return E_FAIL;

	//ID3D11Texture2D* pBackBuffer = nullptr;
	//m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	//m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pBackBufferRTV[1]);

	//g_pBackBufferRTV[0] = m_pBackBufferRTV[0];
	//g_pBackBufferRTV = m_pBackBufferRTV;

	//Safe_AddRef(g_pBackBufferRTV);
	//Safe_Release(pBackBuffer);

	Safe_Release(pBackBufferTexture);

	return S_OK;
}

HRESULT CGraphic_Device::Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY)
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
	TextureDesc.Format = DXGI_FORMAT_R16_TYPELESS;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; /*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc{};

	DSVDesc.Format = DXGI_FORMAT_D16_UNORM;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Flags = 0;
	DSVDesc.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, &DSVDesc, &m_pDepthStencilView)))
		return E_FAIL;

	//g_pDepthStencilView = m_pDepthStencilView;
	//Safe_AddRef(g_pDepthStencilView);
	
	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

void CGraphic_Device::Free()
{
	Safe_Release(m_pSwapChain);
	Safe_Release(m_pDepthStencilView);
	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pDeviceContext);

	//#if defined(DEBUG) || defined(_DEBUG)
	//	ID3D11Debug* d3dDebug;
	//	HRESULT hr = m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
	//	if (SUCCEEDED(hr))
	//	{
	//		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
	//		OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker \r ");
	//		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
	//
	//		hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	//
	//		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
	//		OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker END \r ");
	//		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
	//	}
	//	if (d3dDebug != nullptr)            d3dDebug->Release();
	//#endif


	Safe_Release(m_pDevice);

}
