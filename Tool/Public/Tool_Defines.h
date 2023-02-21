#pragma once


namespace Tool
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };

	enum Tool_ID { ENVIRONMENT, SPAWN, MAP, PICKING_ENVIRONMENT, PICKING_SPAWN, TYPE_END };
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Tool;