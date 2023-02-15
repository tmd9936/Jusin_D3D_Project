#pragma once

#include "Component.h"

/* 0.ȭ�鿡 �׷������� ��ü���� �׷����ϴ� ���������� ��Ƽ� �����Ѵ�. */
/* 1.�����ϰ� �ִ� ��ü���� ��ο����� �����Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND, RENDER_NONLIGHT, RENDER_BLEND, RENDER_UI, RENDER_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Draw_RenderGroup();

private:
	list<class CGameObject*>					m_RenderGroups[RENDER_END];

private: /* �׸��� �׷�鿡 ���� ������ �ٲ����� �ʿ䰡 ���� �� �ֱ⶧���� �׷캰�� �Լ��� ����� ó��. */
	HRESULT Draw_Priority();
	HRESULT Draw_NonBlend();
	HRESULT Draw_NonLight();
	HRESULT Draw_Blend();
	HRESULT Draw_UI();

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END