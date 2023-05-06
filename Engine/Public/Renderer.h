#pragma once

#include "Component.h"

/* 0.화면에 그려져야할 객체들을 그려야하는 순서에따라 모아서 보관한다. */
/* 1.보관하고 있는 객체들의 드로우콜을 수행한다. */

BEGIN(Engine)

class CTarget_Manager;
class CVIBuffer_Rect;
class CShader;
class CLight_Manager;
class CPipeLine;

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	static const FamilyId familyId = FAMILY_ID_RENDERER;

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Add_DebugRenderGroup(CComponent* pComponent);
	HRESULT Draw_RenderGroup();

private:
	list<class CGameObject*>			m_RenderGroups[RENDER_END];

private:
	list<class CComponent*>				m_DebugGroup;

private:
	CTarget_Manager*					m_pTarget_Manager = { nullptr };
	CLight_Manager*						m_pLight_Manager = { nullptr };

private:
	CVIBuffer_Rect*						m_pVIBuffer = { nullptr };
	CShader*							m_pShader = { nullptr };

	ID3D11DepthStencilView*				m_pShadow_DS_Surface = { nullptr };
	ID3D11DepthStencilView*				m_pPreLaplacian_Surface = { nullptr };


private:
	_float4x4							m_WorldMatrix{}, m_ViewMatrix{}, m_ProjMatrix{};


private: /* 그리는 그룹들에 따라 셋팅이 바뀌어야할 필요가 생길 수 있기때문에 그룹별로 함수를 만들어 처리. */
	HRESULT Draw_Priority();

	HRESULT Draw_ShadowDepth();
	HRESULT Draw_NonBlend();
	HRESULT Draw_NonLight();

	HRESULT Draw_Blend();

	HRESULT Draw_World_UI();

	HRESULT Draw_Back_UI();
	HRESULT Draw_UI();
	HRESULT Draw_Blend_UI();

	HRESULT Draw_DebugComponent();

private:
	HRESULT Draw_LightAcc();
	HRESULT Draw_DeferredBlend();

private:
	HRESULT	Draw_DefferdBright();
	HRESULT	Draw_BlurX();
	HRESULT	Draw_BlurY();
	HRESULT	Draw_DeferredNonLightBlend();

private:
	HRESULT	Draw_Gray();
	HRESULT	Draw_GrayBlurX();
	HRESULT	Draw_GrayBlurY();

	HRESULT	Draw_PreLaplacian();

	HRESULT	Draw_Laplacian();

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif // _DEBUG

private:
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY, ID3D11DepthStencilView** ppDepthSencilView);

public:
	static _bool				m_bLaplacian;
	static _float				m_laplacianThesholdLow;
	static _float				m_laplacianThesholdHigh;

private:
	_uint						m_iOriginCX = { 0 };
	_uint						m_iOriginCY = { 0 };

	_uint						m_iShadowMapCX = { 0 };
	_uint						m_iShadowMapCY = { 0 };

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END