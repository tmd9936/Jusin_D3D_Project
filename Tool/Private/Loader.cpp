#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Terrain.h"
#include "FlatTerrain.h"
#include "Camera_Dynamic.h"
#include "TestModel.h"

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

	case LEVEL_GAMEPLAY:
		hr = pLoader->Loading_ForGamePlayLevel();
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
	for (_uint i = 0; i < 99999999; ++i)
	{
		int a = 10;
	}

	/* For.Prototype_Component_Texture */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.dds"), 2))))
		return E_FAIL;

#pragma endregion

#pragma region MODELS
	wsprintf(m_szLoadingText, TEXT("모델를 로딩중입니다."));
	for (_uint i = 0; i < 9999999; ++i)
	{
		int a = 10;
	}
#pragma endregion

#pragma region SHADERS
	wsprintf(m_szLoadingText, TEXT("셰이더를 로딩중입니다."));
	for (_uint i = 0; i < 9999999; ++i)
	{
		int a = 10;
	}
#pragma endregion

#pragma region GAMEOBJECTS
	wsprintf(m_szLoadingText, TEXT("객체원형을 로딩중."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion

	wsprintf(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/*  */
#pragma region TEXTURES
	wsprintf(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.dds")))))
		return E_FAIL;
#pragma endregion

#pragma region MODELS
	wsprintf(m_szLoadingText, TEXT("모델를 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_FlatTerrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_FlatTerrain"),
		CVIBuffer_FlatTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region SHADERS
	wsprintf(m_szLoadingText, TEXT("셰이더를 로딩중입니다."));
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;
#pragma endregion


#pragma region GAMEOBJECTS
	wsprintf(m_szLoadingText, TEXT("객체원형을 로딩중."));

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FlatTerrain */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FlatTerrain"),
		CFlatTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Dynamic */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		CCamera_Dynamic::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TestModel */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TestModel"),
		CTestModel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

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
