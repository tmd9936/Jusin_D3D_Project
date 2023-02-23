#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

public:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	/* �׷��� ����̽��� �ʱ�ȭ. */
	HRESULT Ready_Graphic_Device(HWND hWnd, GRAPHIC_DESC::WINMODE eWinMode, _uint iWinCX, _uint iWinCY, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);

	/* ����۸� �����. */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);

	/* ���̹��� + ���ٽǹ��۸� �����. */
	HRESULT Clear_DepthStencil_View();



	/* �ĸ� ���۸� ������۷� ��ü�Ѵ�.(����۸� ȭ�鿡 ���� �����ش�.) */
	HRESULT Present(UINT interval, UINT flags);

private:

	// IDirect3DDevice9* == LPDIRECT3DDEVICE9 == ID3D11Device + ID3D11DeviceContext 

	/* �޸� �Ҵ�. (��������, �ε�������, �ؽ��ķε�) */
	ID3D11Device* m_pDevice = nullptr;

	/* ���ε��۾�. ��ɽ��� (�������۸� SetStreamSource(), SetIndices(), SetTransform(), SetTexture() */
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

	/* �ĸ���ۿ� ������۸� ��ü�ذ��鼭 ȭ�鿡 �����ִ� ��Ȱ. */
	IDXGISwapChain* m_pSwapChain = nullptr;


	/* IDirect3DTexture9 */
	/* ID3D11Texture2D : �ؽ��ĸ� ǥ���ϴ� ������ü Ÿ���̴�.
	why? �뵵�� �´� ���������� ����ϱ����� �ؽ��İ�ü�� ����������.  */

	/* ID3D11ShaderResourceView : ���̴��� ���޵� �� �ִ� �ױ���Ÿ��. */
	/* ID3D11RenderTargetView : ����Ÿ�ٿ����� ���� �� �ִ� �ؽ��� Ÿ��. */
	/* ID3D11DepthStencilView : ���̽��ٽǹ��۷μ� ���� �� �ִ� Ÿ��.  */
	ID3D11RenderTargetView* m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
public:
	static ID3D11Device* g_pDevice;
	static ID3D11DepthStencilView* g_pDepthStencilView;
	static ID3D11RenderTargetView* g_pBackBufferRTV;
	static IDXGISwapChain* g_pSwapChain;


private:
	HRESULT Ready_SwapChain(HWND hWnd, GRAPHIC_DESC::WINMODE eWinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

public:
	virtual void Free() override;
};

END