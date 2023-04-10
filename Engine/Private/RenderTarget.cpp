#include "..\Public\RenderTarget.h"


CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderTarget::Ready_RenderTarget(_uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat)
{
	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iSizeX;
	TextureDesc.Height = iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	/* 랜더타겟을 그리기 필요한 텍스쳐 컴객체 생성 */
	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	/* 생성한 텍스처를 베이스로한 랜더타겟 생성 2번째 메게변수에 nullptr을 넣으면 
	텍스처의 DESC 속성(버퍼 가로 세로 크기 및 포멧) 그대로 랜더타겟이 만들어짐*/
	if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
		return E_FAIL;

	/* 2차원 버퍼인 랜더 타겟을 정겨변수로 던져서 그리기 위한 셰이더 리소스 생성 */
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
		return E_FAIL;

	return S_OK;
}

CRenderTarget* CRenderTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat)
{
	CRenderTarget* pInstance = new CRenderTarget(pDevice, pContext);

	if (FAILED(pInstance->Ready_RenderTarget(iSizeX, iSizeY, eFormat)))
	{
		MSG_BOX("Failed to Created CRenderTarget");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderTarget::Free()
{
	// SaveDDSTextureToFile(m_pContext, m_pTexture2D, TEXT("../Bin/Test.dds"));

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTexture2D);
	Safe_Release(m_pRTV);
	Safe_Release(m_pSRV);
}
