#pragma once

namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		HWND	hWnd;
		WINMODE	eWinMode;
		unsigned int iViewSizeX;
		unsigned int iViewSizeY;
	}GRAPHIC_DESC;


	typedef struct tagVertex_Position_Texcoord
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexUV;
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Texcoord_Declaration
	{
		const static unsigned int				iNumElements = 2;
		static D3D11_INPUT_ELEMENT_DESC			Elements[2];
	} VTXTEX_DECLARATION;


	typedef struct tagVertex_Position_Normal_Texcoord
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_Texcoord_Declaration
	{
		const static unsigned int				iNumElements = 3;
		static D3D11_INPUT_ELEMENT_DESC			Elements[3];
	} VTXNORTEX_DECLARATION;


	typedef struct tagIndices16
	{
		unsigned short			_0, _1, _2;
	}FACEINDICES16;

	typedef struct tagIndices32
	{
		unsigned long			_0, _1, _2;
	}FACEINDICES32;

}