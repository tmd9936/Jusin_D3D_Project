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

	/* 렌더타겟 용도로 사용하기위해. ID3D11Texture2D */
	ID3D11RenderTargetView* m_pRTV = { nullptr };

	/* 셰이더 전역으로 던질 수 있도록 하기위해. */
	ID3D11ShaderResourceView* m_pSRV = { nullptr };

public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat);
	virtual void Free() override;
};

END