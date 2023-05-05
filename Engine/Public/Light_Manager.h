#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight;

class CLight_Manager final :
    public CBase
{
    DECLARE_SINGLETON(CLight_Manager)

private:
    CLight_Manager();
    virtual ~CLight_Manager() = default;

public:
	void	Initialize();

public:
	_matrix	Get_ShadowDepthLightView_Matrix() const;
	_matrix	Get_ShadowDepthLightProj_Matrix() const;
	_float4x4	Get_ShadowDepthLightView() const;
	_float4x4	Get_ShadowDepthLightProj() const;

public:
	const LIGHTDESC* Get_Light(_uint iIndex);

public:
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

private:
	list<class CLight*>			m_Lights;

	_float4x4					m_shadowDepthLightView = {};
	_float4x4					m_shadowDepthLightProj = {};

public:
	virtual void Free() override;

};

END