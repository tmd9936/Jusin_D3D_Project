#pragma once

#pragma warning (disable : 4251)

#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Effects11/d3dx11effect.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/ScreenGrab.h>
#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/Effects.h>

#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>


#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <ctime>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <codecvt>

#include <DirectXMath.h>
#include <Process.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h" 
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
//#include "json/json.h"

using namespace rapidjson;

using namespace std;
using namespace DirectX;

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"


#ifndef _TOOL

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

#endif // !_TOOL


#define		OBJ_NORMAL						0
#define		OBJ_DEAD						1
#define		OBJ_REUSE						2
#define		OBJ_SCENE_CHNAGE				3




// https://docs.unity3d.com/kr/current/Manual/ExecutionOrder.html

#define FAMILY_ID_PHYSICS_STATE_MACHINE		2
#define FAMILY_ID_PHYSICS_ANIMATION			3	
#define FAMILY_ID_PHYSICS					4
#define FAMILY_ID_PHYSICS_CALCULATOR		5   

#define FAMILY_ID_KEYINPUT					7

#define FAMILY_ID_TAG						8
#define FAMILY_ID_TALK						9

#define FAMILY_ID_LOGIC_STATE_MACHINE		10  
#define FAMILY_ID_LOGIC_ANIMATION			11  
#define FAMILY_ID_LOGIC_TRANSFORM			13  

#define FAMILY_ID_ANIMATION					15  

#define FAMILY_ID_RENDER_STATE_MACHINE		18  
#define FAMILY_ID_TRANSFORM					19
#define FAMILY_ID_BILLBOARD					20
#define FAMILY_ID_TEXTURE					21
#define FAMILY_ID_TEXTURE_MASK				22
#define FAMILY_ID_TEXTURE_BRUSH				23
#define FAMILY_ID_MATERIAL					25

#define FAMILY_ID_HP						27
#define FAMILY_ID_ATTACK					28
#define FAMILY_ID_FORMATION					29

#define FAMILY_ID_SHADER					30
#define FAMILY_ID_VIBUFFER					31

#define FAMILY_ID_COLLISION					35
#define FAMILY_ID_COLLISION_AABB			36
#define FAMILY_ID_COLLISION_OBB				37
#define FAMILY_ID_COLLISION_SPHERE			38

#define FAMILY_ID_NAVIGATION				40

#define FAMILY_ID_RENDERER					46

#define FAMILY_ID_PICKING_CUBE				50


#define		KEY_CHECK(key, state)	CGameInstance::GetInstance()->Get_KeyState(key) == state
#define		KEY_HOLD(key)			KEY_CHECK(key, KEY_STATE::HOLD)
#define		KEY_TAB(key)			KEY_CHECK(key, KEY_STATE::TAB)
#define		KEY_AWAY(key)			KEY_CHECK(key, KEY_STATE::AWAY)
#define		KEY_NONE(key)			KEY_CHECK(key, KEY_STATE::NONE)

#define		MOUSE_CHECK(mouse, state)	CGameInstance::GetInstance()->Get_MouseState(mouse) == state
#define		MOUSE_HOLD(mouse)			MOUSE_CHECK(mouse, MOUSE_STATE::HOLD)
#define		MOUSE_TAB(mouse)			MOUSE_CHECK(mouse, MOUSE_STATE::TAB)
#define		MOUSE_AWAY(mouse)			MOUSE_CHECK(mouse, MOUSE_STATE::AWAY)
#define		MOUSE_NONE(mouse)			MOUSE_CHECK(mouse, MOUSE_STATE::NONE)


typedef	int					FamilyId;
typedef	unsigned int		ObjectId;

using namespace Engine;