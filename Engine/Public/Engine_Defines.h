#pragma once

#pragma warning (disable : 4251)

#include <d3d11.h>
#include <Effects11/d3dx11effect.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
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

#include <DirectXMath.h>
#include <Process.h>

//#include "rapidjson/document.h"     // rapidjson's DOM-style API
//#include "rapidjson/prettywriter.h" // for stringify JSON
#include "json/json.h"

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



// https://docs.unity3d.com/kr/current/Manual/ExecutionOrder.html

#define FAMILY_ID_PHYSICS_STATE_MACHINE		2
#define FAMILY_ID_PHYSICS_ANIMATION			3	
#define FAMILY_ID_PHYSICS					4
#define FAMILY_ID_PHYSICS_TARANSFORM		5   

#define FAMILY_ID_KEYINPUT					7

#define FAMILY_ID_TAG						8
#define FAMILY_ID_TALK						8

#define FAMILY_ID_LOGIC_STATE_MACHINE		10  
#define FAMILY_ID_LOGIC_ANIMATION			11  
#define FAMILY_ID_LOGIC_TRANSFORM			13  

#define FAMILY_ID_ANIMATION					15  

#define FAMILY_ID_RENDER_STATE_MACHINE		18  
#define FAMILY_ID_TRANSFORM					19
#define FAMILY_ID_BILLBOARD					20
#define FAMILY_ID_TEXTURE					21
#define FAMILY_ID_MATERIAL					22
#define FAMILY_ID_VIBUFFER					23

#define FAMILY_ID_SHADER					30
#define FAMILY_ID_COLLISION					35

typedef	int					FamilyId;
typedef	unsigned int		ObjectId;

using namespace Engine;