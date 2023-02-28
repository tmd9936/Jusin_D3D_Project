#pragma once


namespace Client
{
	const unsigned int g_iWinSizeX = 912;
	const unsigned int g_iWinSizeY = 472;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };

	enum Tool_ID { ENVIRONMENT, SPAWN, MAP, PICKING_ENVIRONMENT, PICKING_SPAWN, TYPE_END };
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;