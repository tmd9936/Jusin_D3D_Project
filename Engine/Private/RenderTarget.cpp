#include "..\Public\RenderTarget.h"

#ifdef RENDERTARGET_DEBUG
#include "Shader.h"
#include "VIBuffer_Rect.h"
#endif // _DEBUG

CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderTarget::Ready_RenderTarget(_uint iSizeX, _uint iSizeY, 
	DXGI_FORMAT eFormat, const _float4& vClearColor)
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

	/* ����Ÿ���� �׸��� �ʿ��� �ؽ��� �İ�ü ���� */
	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	/* ������ �ؽ�ó�� ���̽����� ����Ÿ�� ���� 2��° �ްԺ����� nullptr�� ������ 
	�ؽ�ó�� DESC �Ӽ�(���� ���� ���� ũ�� �� ����) �״�� ����Ÿ���� �������*/
	if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
		return E_FAIL;

	/* 2���� ������ ���� Ÿ���� ���������� ������ �׸��� ���� ���̴� ���ҽ� ���� */
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
		return E_FAIL;

	m_vClearColor = vClearColor;

	return S_OK;
}

HRESULT CRenderTarget::Clear()
{
	if (nullptr == m_pContext ||
		nullptr == m_pRTV)
		return E_FAIL;

	m_pContext->ClearRenderTargetView(m_pRTV, (_float*)&m_vClearColor);

	return S_OK;
}

HRESULT CRenderTarget::Set_ShaderResourceView(CShader* pShader, const char* pConstantName)
{
	if (nullptr == m_pSRV)
		return E_FAIL;

	return pShader->Set_ShaderResourceView(pConstantName, m_pSRV);
}

#ifdef RENDERTARGET_DEBUG
HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	_uint				iNumViewports = 1;
	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof ViewportDesc);

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_WorldMatrix._11 = fSizeX;
	m_WorldMatrix._22 = fSizeY;

	m_WorldMatrix._41 = fX - ViewportDesc.Width * 0.5f;
	m_WorldMatrix._42 = -fY + ViewportDesc.Height * 0.5f;

	return S_OK;
}

HRESULT CRenderTarget::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	if (FAILED(pShader->Set_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(pShader->Set_ShaderResourceView("g_Texture", m_pSRV)))
		return E_FAIL;

	pShader->Begin(0);

	pVIBuffer->Render();

	return S_OK;
}
#endif // _DEBUG

CRenderTarget* CRenderTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
	_uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, const _float4& vClearColor)
{
	CRenderTarget* pInstance = new CRenderTarget(pDevice, pContext);

	if (FAILED(pInstance->Ready_RenderTarget(iSizeX, iSizeY, eFormat, vClearColor)))
	{
		MSG_BOX("Failed to Created CRenderTarget");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderTarget::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTexture2D);
	Safe_Release(m_pRTV);
	Safe_Release(m_pSRV);
}
