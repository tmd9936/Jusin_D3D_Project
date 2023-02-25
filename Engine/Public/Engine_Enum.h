#pragma once


namespace Engine
{
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND, RENDER_NONLIGHT, RENDER_BLEND, RENDER_UI, RENDER_END };

	enum CHANNELID {
		SOUND_EFFECT,
		SOUND_BGM,
		SOUND_PLAYER,
		SOUND_PLAYER_WEAPON_CHANGE,
		SOUND_PLAYER_MOVE,
		SOUND_MONSTER,
		SOUND_MONSTER_BULLET,
		SOUND_MONSTER_HIT,
		SOUND_UI,
		SOUND_UI_TEXTBOX,
		SOUND_STAGE_UI,
		SOUND_PLAYER_BULLET,
		SOUND_MONSTER_MOVE,
		MAXCHANNEL
	};
}