#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	CLoader* pLoader = (CLoader*)pArg;

	EnterCriticalSection(pLoader->Get_CriticalSection());

	HRESULT			hr = { 0 };

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO: /* �ε��� ���������� �ΰ��. �ΰ����� �ʿ��� ���� ����(���ҽ�, ������ü) �۾��� ����. */
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

	/* �����带 �����Ѵ�. */
	/* �����带 �����ϰԵǸ� �������Լ��� �����ؾ���. */

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;



	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Ready_Sound();
	pGameInstance->PlayBGM(TEXT("BGM_Title.ogg"));
	/*  */		  
#pragma region TEXTURES	
	wsprintf(m_szLoadingText, TEXT("�ؽ��ĸ� �ε����Դϴ�."));
	for (_uint i = 0; i < 999999999; ++i)
	{
		int a = 10;
	}
#pragma endregion

#pragma region MODELS
	wsprintf(m_szLoadingText, TEXT("�𵨸� �ε����Դϴ�."));
	for (_uint i = 0; i < 999999999; ++i)
	{
		int a = 10;
	}
#pragma endregion

#pragma region SHADERS
	wsprintf(m_szLoadingText, TEXT("���̴��� �ε����Դϴ�."));
	for (_uint i = 0; i < 999999999; ++i)
	{
		int a = 10;
	}
#pragma endregion

#pragma region GAMEOBJECTS
	wsprintf(m_szLoadingText, TEXT("��ü������ �ε���."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion

	wsprintf(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	/*  */
#pragma region TEXTURES
	wsprintf(m_szLoadingText, TEXT("�ؽ��ĸ� �ε����Դϴ�."));
	for (_uint i = 0; i < 999999999; ++i)
	{
		int a = 10;
	}
#pragma endregion

#pragma region MODELS
	wsprintf(m_szLoadingText, TEXT("�𵨸� �ε����Դϴ�."));
	for (_uint i = 0; i < 999999999; ++i)
	{
		int a = 10;
	}
#pragma endregion

#pragma region SHADERS
	wsprintf(m_szLoadingText, TEXT("���̴��� �ε����Դϴ�."));
	for (_uint i = 0; i < 999999999; ++i)
	{
		int a = 10;
	}
#pragma endregion

	wsprintf(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));
	m_isFinished = true;

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

	Safe_Release(m_pDevice);

	Safe_Release(m_pContext);
}
