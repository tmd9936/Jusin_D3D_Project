#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderTarget final : public CBase
{
private:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget() = default;

public:
	ID3D11RenderTargetView* Get_RTV() {
		return m_pRTV;
	}

public:
	HRESULT Ready_RenderTarget(_uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat);

public:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	ID3D11Texture2D* m_pTexture2D = { nullptr };

	/* ����Ÿ�� �뵵�� ����ϱ�����. ID3D11Texture2D */
	ID3D11RenderTargetView* m_pRTV = { nullptr };

	/* ���̴� �������� ���� �� �ֵ��� �ϱ�����. */
	ID3D11ShaderResourceView* m_pSRV = { nullptr };

public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat);
	virtual void Free() override;
};

END