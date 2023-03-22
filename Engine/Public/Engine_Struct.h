#pragma once

namespace Engine
{
	typedef struct tagKeyFrame
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
		double			Time;
	}KEYFRAME;

	/* 메시에게 적용하기위한 머테리얼텍스쳐들(Diffuse, AMbient, Specular, Normal) */
	typedef struct tagMeshMaterial
	{
		class CTexture* pMtrlTexture[AI_TEXTURE_TYPE_MAX] = { nullptr };
		string szModelPath;

	}MESH_MATERIAL;

	typedef struct tagLightDesc
	{
		XMFLOAT4		vDirection;
		XMFLOAT4		vPosition;
		float			fRange;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular; /* 정반사 */
	}LIGHTDESC;


	typedef struct tagGraphicDesc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		HWND	hWnd;
		WINMODE	eWinMode;
		unsigned int iViewSizeX;
		unsigned int iViewSizeY;
	}GRAPHIC_DESC;

	typedef struct tagVertex_Position
	{
		XMFLOAT3			vPosition;
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_Declaration
	{
		const static unsigned int				iNumElements = 1;
		static D3D11_INPUT_ELEMENT_DESC			Elements[1];
	} VTXPOS_DECLARATION;

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

	typedef struct tagVertex_Model
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
	}VTXMODEL;

	typedef struct ENGINE_DLL tagVertex_Model_Declaration
	{
		const static unsigned int				iNumElements = 3;
		static D3D11_INPUT_ELEMENT_DESC			Elements[3];
	} VTXMODEL_DECLARATION;

	typedef struct tagVertex_AnimModel
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
		XMUINT4				vBlendIndex; /* 이 정점이 따라다녀야할 뼈의인덱스들(뼈의 인덱스 : 모델전체의 뼈 중 특정 인덱스(x) 메시에 영향을 주는 뼈들 중 특정 인덱스(0), 최대 뼈의 갯수는 : 4개) */
		XMFLOAT4			vBlendWeight; /* 이 정점이 영향을 받고 있는 뼈들의 상태를 몇퍼센트(0 ~ 1)나 이용할지에 대한 값. */
	}VTXANIMMODEL;

	typedef struct ENGINE_DLL tagVertex_AnimModel_Declaration
	{
		const static unsigned int				iNumElements = 5;
		static D3D11_INPUT_ELEMENT_DESC			Elements[5];
	} VTXANIMMODEL_DECLARATION;

	typedef struct tagVertex_Wire
	{
		XMFLOAT3	vPosition;
		XMFLOAT2	vTexUV;

	}VTXWIRE;

	typedef struct ENGINE_DLL tagVertex_Wire_Declaration
	{
		const static unsigned int				iNumElements = 2;
		static D3D11_INPUT_ELEMENT_DESC			Elements[2];
	} VTXWIRE_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_Color_Declaration
	{
		const static unsigned int				iNumElements = 3;
		static D3D11_INPUT_ELEMENT_DESC			Elements[3];
	} VTXNORCOLOR_DECLARATION;

	typedef struct tagVertex_Color_Model
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT4			vColor;
	}VTXCOLORMODEL;

	typedef struct tagVertex_Color_AnimModel
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT4			vColor;
		XMUINT4				vBlendIndex; /* 이 정점이 따라다녀야할 뼈의인덱스들(뼈의 인덱스 : 모델전체의 뼈 중 특정 인덱스(x) 메시에 영향을 주는 뼈들 중 특정 인덱스(0), 최대 뼈의 갯수는 : 4개) */
		XMFLOAT4			vBlendWeight; /* 이 정점이 영향을 받고 있는 뼈들의 상태를 몇퍼센트(0 ~ 1)나 이용할지에 대한 값. */
	}VTXCOLORANIMMODEL;

	typedef struct ENGINE_DLL tagVertex_Color_AnimModel_Declaration
	{
		const static unsigned int				iNumElements = 5;
		static D3D11_INPUT_ELEMENT_DESC			Elements[5];
	} VTXCOLORANIMMODEL_DECLARATION;

	typedef struct tagVertex_UU_AnimModel
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexUV;
		XMUINT4				vBlendIndex; /* 이 정점이 따라다녀야할 뼈의인덱스들(뼈의 인덱스 : 모델전체의 뼈 중 특정 인덱스(x) 메시에 영향을 주는 뼈들 중 특정 인덱스(0), 최대 뼈의 갯수는 : 4개) */
		XMFLOAT4			vBlendWeight; /* 이 정점이 영향을 받고 있는 뼈들의 상태를 몇퍼센트(0 ~ 1)나 이용할지에 대한 값. */
	}VTXUIANIMMODEL;

	typedef struct ENGINE_DLL tagVertex_UI_AnimModel_Declaration
	{
		const static unsigned int				iNumElements = 4;
		static D3D11_INPUT_ELEMENT_DESC			Elements[4];
	} VTXUIANIMMODEL_DECLARATION;

	typedef struct tagVertex_ModelAllData
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
		XMFLOAT4			vColor;
		XMUINT4				vBlendIndex; /* 이 정점이 따라다녀야할 뼈의인덱스들(뼈의 인덱스 : 모델전체의 뼈 중 특정 인덱스(x) 메시에 영향을 주는 뼈들 중 특정 인덱스(0), 최대 뼈의 갯수는 : 4개) */
		XMFLOAT4			vBlendWeight; /* 이 정점이 영향을 받고 있는 뼈들의 상태를 몇퍼센트(0 ~ 1)나 이용할지에 대한 값. */
	}VTXMODEL_ALL_DATA;


	typedef struct tagIndices16
	{
		unsigned short			_0, _1, _2;
	}FACEINDICES16;

	typedef struct tagIndices32
	{
		unsigned long			_0, _1, _2;
	}FACEINDICES32;

}