#pragma once


namespace Client
{
	const unsigned int g_iWinSizeX = 912;
	const unsigned int g_iWinSizeY = 472;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };

	enum Tool_ID { PICKING, ADD, MAP, SAVE_LOAD, TYPE_END };
}

#define		DEFAULT_TERRAIN_WIDTH	128
#define		DEFAULT_TERRAIN_HEIGHT	128

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;