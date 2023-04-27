#pragma once


namespace Client
{
	const unsigned int g_iWinSizeX = 930;
	const unsigned int g_iWinSizeY = 432;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_BASECAMP, LEVEL_WORLDMAP, LEVEL_STAGE, LEVEL_POKEMONSTATE, LEVEL_END };

	enum Tool_ID { PICKING, ADD, MAP, SAVE_LOAD, TYPE_END };
	
	enum TEXTURETYPE { TEXTURETYPE_DIFFUSE, TEXTURETYPE_MASK, TEXTURETYPE_BRUSH, TEXTURETYPE_END };

	enum EFFECT_TYPE
	{
		EFFECT_TYPE_ATTACK,
		EFFECT_TYPE_BUFF,
		EFFECT_TYPE_END,
	};

	enum class STONE_EQUIP_TYPE
	{
		TYPE_ATK,
		TYPE_HP,
		TYPE_TWINS,
		TYPE_END
	};

	enum class STONE_EQUIP_STATE
	{
		STATE_EQUIP,
		STATE_NO_EQUIP,
		STATE_BLOCK,
		STATE_END
	};

	typedef struct Stone_Equip_Desc
	{
		bool					m_isOpen;
		STONE_EQUIP_TYPE		m_type;
		int						m_equip_stoneID;
		int						m_value;
		STONE_EQUIP_STATE		m_state;
	} STONE_EQUIP_DESC;
}


typedef struct tag_My_float4
{
	float x;
	float y;
	float z;
	float w;

}MYFLOAT4;

#define _IMGUITOOL

#define		DEFAULT_TERRAIN_WIDTH	64
#define		DEFAULT_TERRAIN_HEIGHT	64

extern HINSTANCE g_hInst;
extern HWND g_hWnd;
extern MYFLOAT4 g_BackBufferColor;

using namespace Client;