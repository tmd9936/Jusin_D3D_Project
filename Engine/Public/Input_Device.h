#pragma once

#include "Base.h"
#include "Engine_Defines.h"

enum class KEY_STATE
{
	NONE,
	TAB,
	HOLD,
	AWAY
};

enum class MOUSE_STATE
{
	NONE,
	TAB,
	HOLD,
	AWAY,
};

enum class MOUSE
{
	LBTN,
	RBTN,
	MBTN,
	LAST,
};

enum class KEY
{
	W, A, S, D, R, F, V, E, B, N, M,

	G, H, J, K, L,

	TAB,
	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	DEL,

	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

	UP, DOWN, LEFT, RIGHT,

	LAST,
};

struct tKeyInfo
{
	KEY_STATE eState;
	_bool bPrevPush;
};

struct tMouseInfo
{
	MOUSE_STATE eState;
	_bool bPrevPush;
};

BEGIN(Engine)

class CInput_Device : public CBase
{
	DECLARE_SINGLETON(CInput_Device)

public:
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

private:
	explicit CInput_Device();
	virtual ~CInput_Device();

public:
	const KEY_STATE Get_KeyState(KEY eKey)const
	{
		return m_vecKey[(_int)eKey].eState;
	}

	const MOUSE_STATE Get_MouseState(MOUSE eMouse)const
	{
		return m_vecMouse[(_int)eMouse].eState;
	}

	const _long Get_MouseMove(MOUSEMOVESTATE eMouseMoveID)
	{
		return *(((_long*)&m_MouseState) + eMouseMoveID);
	}

	_byte		Get_DIKeyState(_ubyte ubyKeyID)
	{
		return m_byKeyState[ubyKeyID];
	}

	_byte		Get_DIMouseState(MOUSEKEYSTATE eMouseID)
	{
		return m_MouseState.rgbButtons[eMouseID];
	}

	_long		Get_DIMouseMove(MOUSEMOVESTATE eMouseMoveID)
	{
		return *(((_long*)&m_MouseState) + eMouseMoveID);
	}


public:
	HRESULT			Ready_DInput(HINSTANCE hInst, HWND hWnd);
	void			Update_DInput(void);

private:
	LPDIRECTINPUT8				m_pInputSDK;

	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;

private:
	vector<tKeyInfo>		m_vecKey;
	vector<tMouseInfo>		m_vecMouse;
	_byte					m_byKeyState[256];
	DIMOUSESTATE			m_MouseState;

public:
	virtual void		Free(void);

};

END