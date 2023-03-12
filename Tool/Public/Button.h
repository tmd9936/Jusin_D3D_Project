#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CRenderer;
class CShader;
class CTexture;
class CTransform;
END

BEGIN(Client)

class CButton final : public CGameObject
{
public:
	typedef struct Button_Arguments_Desc
	{
		_tchar			m_ObjectArgument[MAX_PATH];
		_tchar			m_ObjectArgumentAssemblyTypeNamep[MAX_PATH];
		_int			m_IntArgument;
		_float			m_FloatArgument;
		_tchar			m_StringArgument[MAX_PATH];
		_bool			m_BoolArgument;


	} BUTTON_ARGUMENTS_DESC;

public:
	typedef struct Button_Call_Desc
	{
		_tchar					m_MethdName[MAX_PATH];
		_uint					m_Mode;
		BUTTON_ARGUMENTS_DESC	m_Arguments;
		_uint					m_CallState;
	} BUTTON_CALL_DESC;

public:
	typedef struct Button_OnClick_Desc
	{
		_uint							m_Size;
		vector<BUTTON_CALL_DESC>		m_Calls;

	} BUTTON_ONCLICK_DESC;

public:
	typedef struct Button_Animation_Triggers_Desc
	{
		_tchar			m_NormalTrigger[MAX_PATH];
		_tchar			m_HighlightedTrigger[MAX_PATH];
		_tchar			m_PressedTrigger[MAX_PATH];
		_tchar			m_DisabledTrigger[MAX_PATH];

	} BUTTON_ANIMATION_TRIGGERS_DESC;

public:
	typedef struct Button_Color_Desc
	{
		_float4			m_NormalColor;
		_float4			m_HighlightedColor;
		_float4			m_PressedColor;
		_float4			m_DisabledColor;

	} BUTTON_COLOR_DESC;


public:
	typedef struct Button_Sprite_State_Desc
	{
		_tchar			m_HighlightedSprite[MAX_PATH];
		_tchar			m_PressedSprite[MAX_PATH];
		_tchar			m_DisabledSprite[MAX_PATH];

	} BUTTON_SPRITE_STATE_DESC;

public:
	typedef struct Button_Desc
	{
		_float								m_fX;
		_float								m_fY;
		_float								m_fSizeX;
		_float								m_fSizeY;

		BUTTON_COLOR_DESC					m_Colors;
		_float								m_ColorMultiplier;
		_float								m_FadeDuration;

		BUTTON_SPRITE_STATE_DESC			m_SpriteState;
		BUTTON_ANIMATION_TRIGGERS_DESC		m_AnimationTriggers;
		
		_bool								m_Interactable;

		BUTTON_ONCLICK_DESC					m_OnClick;

	} BUTTON_DESC;

private:
	CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButton(const CButton& rhs);
	virtual ~CButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

private:
	BUTTON_DESC		m_UIDesc = {};
	_float4x4		m_ViewMatrix = {};
	_float4x4		m_ProjMatrix = {};


private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	/* Prototype */
	/* 원형 객체를 생성한다. */
	static CButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본 객체를 생성한다. */
	virtual CGameObject* Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg = nullptr) override;
	virtual void Free() override;
};

END