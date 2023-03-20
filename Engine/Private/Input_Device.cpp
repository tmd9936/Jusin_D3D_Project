#include "Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::MOUSEKEYSTATE g_arrDIM[(_int)MOUSE::LAST] =
{
	CInput_Device::DIM_LB,
	CInput_Device::DIM_RB,
	CInput_Device::DIM_MB,
};

_int g_arrDIK[(_int)KEY::LAST] =
{
	DIK_W,
	DIK_A,
	DIK_S,
	DIK_D,
	DIK_R,
	DIK_F,
	DIK_V,
	DIK_E,
	DIK_B,
	DIK_N,
	DIK_M,


	DIK_G,
	DIK_H,
	DIK_J,
	DIK_K,
	DIK_L,

	DIK_TAB,
	DIK_LALT,
	DIK_LCONTROL,
	DIK_LSHIFT,
	DIK_SPACE,
	DIK_RETURN,
	DIK_ESCAPE,
	DIK_DELETE,

	DIK_F1,
	DIK_F2,
	DIK_F3,
	DIK_F4,
	DIK_F5,
	DIK_F6,
	DIK_F7,
	DIK_F8,
	DIK_F9,
	DIK_F10,
	DIK_F11,
	DIK_F12,

	DIK_UP,
	DIK_DOWN,
	DIK_LEFT,
	DIK_RIGHT

};

CInput_Device::CInput_Device()
	: m_pInputSDK(nullptr)
	, m_pKeyBoard(nullptr)
	, m_pMouse(nullptr)
{
	ZeroMemory(m_byKeyState, sizeof(m_byKeyState));
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));

	for (_uint i = 0; i < (_int)KEY::LAST; ++i)
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });

	for (_uint i = 0; i < (_int)MOUSE::LAST; ++i)
		m_vecMouse.push_back(tMouseInfo{ MOUSE_STATE::NONE, false });
}


CInput_Device::~CInput_Device()
{
	Free();
}

HRESULT CInput_Device::Ready_DInput(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->Acquire();
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->Acquire();

	return S_OK;
}

void CInput_Device::Update_DInput(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

	for (_uint i = 0; i < (_int)MOUSE::LAST; ++i)
	{
		if (Get_DIMouseState(g_arrDIM[i]) & 0x80)
		{
			if (m_vecMouse[i].bPrevPush)
				m_vecMouse[i].eState = MOUSE_STATE::HOLD;

			else
				m_vecMouse[i].eState = MOUSE_STATE::TAB;

			m_vecMouse[i].bPrevPush = true;
		}

		else
		{
			if (m_vecMouse[i].bPrevPush)
				m_vecMouse[i].eState = MOUSE_STATE::AWAY;

			else
				m_vecMouse[i].eState = MOUSE_STATE::NONE;

			m_vecMouse[i].bPrevPush = false;
		}
	}

	for (_uint i = 0; i < (_int)KEY::LAST; ++i)
	{
		if (Get_DIKeyState(g_arrDIK[i]) & 0x80)
		{
			if (m_vecKey[i].bPrevPush)
				m_vecKey[i].eState = KEY_STATE::HOLD;

			else
				m_vecKey[i].eState = KEY_STATE::TAB;

			m_vecKey[i].bPrevPush = true;
		}

		else
		{
			if (m_vecKey[i].bPrevPush)
				m_vecKey[i].eState = KEY_STATE::AWAY;

			else
				m_vecKey[i].eState = KEY_STATE::NONE;

			m_vecKey[i].bPrevPush = false;
		}
	}
}

void Engine::CInput_Device::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}
