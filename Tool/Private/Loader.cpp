#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"

#include "BackGround.h"
#include "UI.h"
#include "FlatTerrain.h"
#include "Camera_Dynamic.h"
#include "Camera_Public.h"

#include "TestModel.h"
#include "ModelUI.h"
#include "Map.h"
#include "Stove.h"

#include "Player.h"
#include "Button.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	if(FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	CLoader* pLoader = (CLoader*)pArg;

	EnterCriticalSection(pLoader->Get_CriticalSection());

	HRESULT			hr = { 0 };

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO: /* 로딩씬 다음레벨이 로고다. 로고레벨에 필요한 사전 생성(리소스, 원형객체) 작업을 하자. */
		hr = pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_BASECAMP:
		hr = pLoader->Loading_ForBaseCampLevel();
		break;
	case LEVEL_WORLDMAP:
		hr = pLoader->Loading_ForWorldMapLevel();
		break;
	}

	if (FAILED(hr))
	{
		LeaveCriticalSection(pLoader->Get_CriticalSection());
		return 1;
	}

	LeaveCriticalSection(pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(m_CriticalSection);

	/* 스레드를 생성한다. */
	/* 스레드를 생성하게되면 진입점함수를 정의해야해. */

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;



	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//pGameInstance->Ready_Sound();
	//pGameInstance->PlayBGM(TEXT("BGM_BASE.ogg"));
	/*  */		  
#pragma region TEXTURES	
	wsprintf(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* For.Prototype_Component_Texture */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/splash/TitleLogo_Korea.png")))))
		return E_FAIL;

#pragma endregion

#pragma region MODELS
	wsprintf(m_szLoadingText, TEXT("모델를 로딩중입니다."));

	_matrix PivotMatrix = XMMatrixIdentity();
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Model_Logo_Scene"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Resources/Mesh/Animation/Logo/Logo_Scene.fbx", PivotMatrix))))
		return E_FAIL;
#pragma endregion

#pragma region SHADERS
	wsprintf(m_szLoadingText, TEXT("셰이더를 로딩중입니다."));
	for (_uint i = 0; i < 999999; ++i)
	{
		int a = 10;
	}
#pragma endregion

#pragma region GAMEOBJECTS
	wsprintf(m_szLoadingText, TEXT("객체원형을 로딩중."));

	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_LOGO))
	{
		/* For.Prototype_GameObject_UI */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI"),
			CUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		///* For.Prototype_GameObject_BackGround */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		//	CBackGround::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;
	}

#pragma endregion

	wsprintf(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForBaseCampLevel()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/*  */
#pragma region TEXTURES
	wsprintf(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TerrainMask */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Texture_TerrainMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Button_Color_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_button.png")))))
		return E_FAIL;
#pragma endregion

#pragma region MODELS
	wsprintf(m_szLoadingText, TEXT("모델를 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_FlatTerrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_VIBuffer_FlatTerrain"),
		CVIBuffer_FlatTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Calculator */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Calculator"),
		CCalculator::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	_matrix		PivotMatrix = XMMatrixIdentity();

	/* For.Prototype_Component_Model_Fiona */
	//PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Model_Fiona"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", PivotMatrix))))
	//	return E_FAIL;

	/* For.Prototype_Component_Model_BaseCamp_Field */
	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Model_BaseCamp_Field"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/BC_field.fbx", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Model_Button_Base"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM_UI, "../../Reference/Resources/Mesh/Animation/UI/Button_Base.fbx", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Model_BaseCamp_Stove"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/NonAnimation/Basecamp/BC_stove.fbx", PivotMatrix))))
		return E_FAIL;


	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Model_WolrdMap_Island"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Resources/Mesh/Animation/WorldMap/W_island.fbx", PivotMatrix))))
	//	return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.4f, 0.4f, 0.4f) * XMMatrixRotationY(XMConvertToRadians(180.f));;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Model_Pokemon_PM6"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Reference/Resources/Mesh/Animation/Pokemon/PM6.fbx", PivotMatrix))))
		return E_FAIL;

#pragma endregion

#pragma region SHADERS
	wsprintf(m_szLoadingText, TEXT("셰이더를 로딩중입니다."));
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex_HeightTerrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Shader_VtxNorTex_HeightTerrain"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxNorTex_HeightTerrain.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModelColor */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxAnimModelColor.hlsl"), VTXCOLORANIMMODEL_DECLARATION::Elements, VTXCOLORANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModelColor */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Shader_VtxtexButton"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_Vtxtex_Button.hlsl"), VTXUIANIMMODEL_DECLARATION::Elements, VTXUIANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

#pragma endregion


#pragma region GAMEOBJECTS
	wsprintf(m_szLoadingText, TEXT("객체원형을 로딩중."));

	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_BASECAMP))
	{
		/* For.Prototype_GameObject_Button */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
			CButton::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		/* For.Prototype_GameObject_FlatTerrain */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FlatTerrain"),
			CFlatTerrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Camera_Dynamic */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
			CCamera_Dynamic::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Public"),
			CCamera_Public::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		///* For.Prototype_GameObject_TestModel */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TestModel"),
		//	CTestModel::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;

		/* For.Prototype_GameObject_Player */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
			CPlayer::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Map"),
			CMap::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stove"),
			CStove::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}
#pragma endregion

	wsprintf(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForWorldMapLevel()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/*  */
#pragma region TEXTURES
	wsprintf(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TerrainMask */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Texture_TerrainMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Button_Color_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_button.png")))))
		return E_FAIL;
#pragma endregion

#pragma region MODELS
	wsprintf(m_szLoadingText, TEXT("모델를 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_FlatTerrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_VIBuffer_FlatTerrain"),
		CVIBuffer_FlatTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Calculator */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Calculator"),
		CCalculator::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	_matrix		PivotMatrix = XMMatrixIdentity();

	/* For.Prototype_Component_Model_Fiona */
	//PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Model_Fiona"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", PivotMatrix))))
	//	return E_FAIL;


	/* For.Prototype_Component_Model_BaseCamp_Field */
	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Model_WorldMap_island"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/Animation/WorldMap/W_island.fbx", PivotMatrix))))
		return E_FAIL;


	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Model_WolrdMap_Island"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Resources/Mesh/Animation/WorldMap/W_island.fbx", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Model_Pokemon_PM1"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Reference/Resources/Mesh/Animation/Pokemon/PM1.fbx", PivotMatrix))))
	//	return E_FAIL;

#pragma endregion

#pragma region SHADERS
	wsprintf(m_szLoadingText, TEXT("셰이더를 로딩중입니다."));
	///* For.Prototype_Component_Shader_VtxNorTex */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Shader_VtxNorTex"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
	//	return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex_HeightTerrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Shader_VtxNorTex_HeightTerrain"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxNorTex_HeightTerrain.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModelColor */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxAnimModelColor.hlsl"), VTXCOLORANIMMODEL_DECLARATION::Elements, VTXCOLORANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModelColor */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Shader_VtxtexButton"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_Vtxtex_Button.hlsl"), VTXUIANIMMODEL_DECLARATION::Elements, VTXUIANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

#pragma endregion


#pragma region GAMEOBJECTS
	wsprintf(m_szLoadingText, TEXT("객체원형을 로딩중."));

	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_WORLDMAP))
	{
		///* For.Prototype_GameObject_Button */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button"),
		//	CButton::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;
		///* For.Prototype_GameObject_FlatTerrain */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FlatTerrain"),
		//	CFlatTerrain::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;

		///* For.Prototype_GameObject_Camera_Dynamic */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		//	CCamera_Dynamic::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;

		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Public"),
		//	CCamera_Public::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;

		///* For.Prototype_GameObject_TestModel */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TestModel"),
		//	CTestModel::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;

		///* For.Prototype_GameObject_Player */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		//	CPlayer::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;

		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Map"),
		//	CMap::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;

		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stove"),
		//	CStove::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;
	}
#pragma endregion

	wsprintf(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(m_CriticalSection);
	DeleteObject(m_hThread);

	Safe_Delete(m_CriticalSection);

	Safe_Release(m_pDevice);

	Safe_Release(m_pContext);
}
