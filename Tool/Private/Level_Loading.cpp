#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#pragma region ENGINE_HEADERS
#include "GameInstance.h"
#pragma endregion

#pragma region CLIENT_HEADERS
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_BaseCamp.h"
#include "Level_WorldMap.h"

#include "ModelUI.h"
#pragma endregion


CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	g_BackBufferColor = m_LoadindBackBufferColor;
	m_eNextLevelID = eNextLevelID;

	/* 로더안에서 스레드를 생성한다. */
	/* 생성한 스레드가 필요한 준비를 한다.  */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (eNextLevelID == LEVEL_LOGO)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (!pGameInstance->Get_Sound_Ready_Finish())
		{
			pGameInstance->Ready_Sound();
		}
		//pGameInstance->PlayBGM(TEXT("BGM_BASE.ogg"));

		Safe_Release(pGameInstance);
	}

	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;


	return S_OK;
}

void CLevel_Loading::Tick(_double TimeDelta)
{
	//if (GetKeyState(VK_RETURN) & 0x8000)
	//{
		if (true == m_pLoader->Get_Finished())
		{
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);

			CLevel* pNewLevel = { nullptr };


			switch (m_eNextLevelID)
			{
			case LEVEL_LOGO:
				if (!pGameInstance->Get_Sound_Ready_Finish())
				{
					Safe_Release(pGameInstance);
					return;
				}
				pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;

			case LEVEL_BASECAMP:
				pNewLevel = CLevel_BaseCamp::Create(m_pDevice, m_pContext);
				break;

			case LEVEL_WORLDMAP:
				pNewLevel = CLevel_WorldMap::Create(m_pDevice, m_pContext);
				break;

			}

			if (nullptr == pNewLevel)
			{
				Safe_Release(pGameInstance);
				return;
			}

			if (FAILED(pGameInstance->Open_Level(m_eNextLevelID, pNewLevel)))
				return;

			Safe_Release(pGameInstance);
		}
	//}

#ifdef _DEBUG
	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());
#endif


}

HRESULT CLevel_Loading::Ready_Layer_BackGround()
{
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_LOADING, pLayerTag)))
		return E_FAIL;

	CModelUI::UI_DESC desc = {};
	desc.m_fSizeX = g_iWinSizeX >> 1;
	desc.m_fSizeY = g_iWinSizeY >> 1;
	desc.m_fX = g_iWinSizeX >> 1;
	desc.m_fY = g_iWinSizeY >> 1;
	desc.m_eModelPrototypLevel = LEVEL_STATIC;
	desc.m_iAnimationIndex = 0;

	lstrcpy(desc.m_ModelProtoTypeName, TEXT("Prototype_Component_Model_Loading_Scene"));

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ModelUI"), LEVEL_LOADING, pLayerTag, nullptr, &desc)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);

}
