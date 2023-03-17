#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CModel;
class CTexture;
class CTransform;
END

BEGIN(Client)

class CButton abstract : public CGameObject
{
public:
	enum BUTTON_STATE
	{
		BUTTON_IDLE,
		BUTTON_PRESS,
		BUTTON_RELEASE,
		BUTTON_SELECT,
		BUTTON_END
	};


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
		_tchar					m_MethodName[MAX_PATH];
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
		_float				m_fX;
		_float				m_fY;
		_float				m_fSizeX;
		_float				m_fSizeY;

		_uint				m_eModelPrototypLevel;
		_uint				m_ShaderLevelIndex;
		char  				m_DiffuseTextureName[MAX_PATH];
		_tchar				m_MaskPrototype[MAX_PATH];
		_tchar				m_BrushPrototype[MAX_PATH];
		_tchar				m_ButtonName[MAX_PATH];


		//BUTTON_COLOR_DESC					m_Colors;
		//_float								m_ColorMultiplier;
		//_float								m_FadeDuration;

		//BUTTON_SPRITE_STATE_DESC			m_SpriteState;
		//BUTTON_ANIMATION_TRIGGERS_DESC		m_AnimationTriggers;
		//
		//_bool								m_Interactable;

		//BUTTON_ONCLICK_DESC					m_OnClick;

	} BUTTON_DESC;

protected:
	CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButton(const CButton& rhs);
	virtual ~CButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체의 초기화작업 */
	virtual HRESULT Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg) override; /* 사본객체의 초기화작업 */
	virtual _uint Tick(_double TimeDelta) override;
	virtual _uint LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

protected:
	void	Button_Motion(_double TimeDelta);
	void	Picking_Button();
	_uint	Change_State();

public:
	virtual _uint	On_Idle() PURE;
	virtual _uint	On_Press() PURE;
	virtual _uint	On_Select() PURE;
	virtual _uint	On_Release() PURE;

protected:
	CTransform*		m_pTransformCom = { nullptr };
	CRenderer*		m_pRendererCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };;
	CTexture*		m_pTextureCom = { nullptr };

protected:
	BUTTON_DESC		m_UIDesc = {};
	_float4x4		m_ViewMatrix = {};
	_float4x4		m_ProjMatrix = {};
	BUTTON_STATE	m_ePreState = { BUTTON_END };
	BUTTON_STATE	m_eCurState = { BUTTON_IDLE };

private:
	_float4x4		m_TransformMatrix = {};
	_float4x4		m_selectTransformMatrix = {};


protected:
	const			LONG		m_mouseInterSize = { 5 };

public:
	virtual void Free() override;
};

END