#pragma once

#include "Component.h"

/* 0.ȭ�鿡 �׷������� ��ü���� �׷����ϴ� ���������� ��Ƽ� �����Ѵ�. */
/* 1.�����ϰ� �ִ� ��ü���� ��ο����� �����Ѵ�. */

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
	HRESULT Draw_RenderGroup();

private:
	list<class CGameObject*>					m_RenderGroups[RENDER_END];

private:
	CTarget_Manager* m_pTarget_Manager = { nullptr };
	CLight_Manager* m_pLight_Manager = { nullptr };

private:
	CVIBuffer_Rect* m_pVIBuffer = { nullptr };
	CShader* m_pShader = { nullptr };

private:
	_float4x4							m_WorldMatrix{}, m_ViewMatrix{}, m_ProjMatrix{};


private: /* �׸��� �׷�鿡 ���� ������ �ٲ����� �ʿ䰡 ���� �� �ֱ⶧���� �׷캰�� �Լ��� ����� ó��. */
	HRESULT Draw_Priority();
	HRESULT Draw_NonBlend();
	HRESULT Draw_NonLight();
	HRESULT Draw_Blend();
	HRESULT Draw_Back_UI();
	HRESULT Draw_UI();
	HRESULT Draw_Blend_UI();

private:
	HRESULT Draw_LightAcc();
	HRESULT Draw_DeferredBlend();

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif // _DEBUG

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void Free() override;
};

END