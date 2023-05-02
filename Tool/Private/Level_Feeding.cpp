#include "stdafx.h"
#include "Level_Feeding.h"
#include "GameInstance.h"

#include "UI.h"
#include "PokemonInfoUI.h"

CLevel_Feeding::CLevel_Feeding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Feeding::Initialize()
{
	g_BackBufferColor = m_FeedingBackBufferColor;

	CGameInstance::GetInstance()->PlayBGM(TEXT("BGM_BASE.ogg"));

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Manager(TEXT("Layer_Manager"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Feeding::Tick(_double TimeDelta)
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("포켓몬 퀘스트"));
#endif

}

HRESULT CLevel_Feeding::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_FEEDING, pLayerTag)))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BackGround"), LEVEL_FEEDING, pLayerTag)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Feeding::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GoToBackLevelButton"), LEVEL_FEEDING, pLayerTag, L"GoToBackLevelButton", "../../Reference/Resources/Data/Scene/Feeding/Button/GoToBackLevelButton.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_FEEDING, pLayerTag, L"LevelNameInfo", "../../Reference/Resources/Data/Scene/Feeding/UI/LevelNameInfo.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FoodInventory"), LEVEL_FEEDING, pLayerTag, L"LevelNameInfo", "../../Reference/Resources/Data/Scene/Feeding/Feeding_Inventory.json", CLONE_FILEPATH)))
		return E_FAIL;

	CPokemonInfoUI* pPokemonInfoUI01 = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonInfoUI"), LEVEL_FEEDING, pLayerTag, (CGameObject**)&pPokemonInfoUI01,
		L"PokemonInfoUI01", "../../Reference/Resources/Data/Scene/Feeding/UI/PokemonInfoUI01.json", CLONE_FILEPATH)))
		return E_FAIL;
	if (FAILED(pPokemonInfoUI01->Init_PokemonData(1)))
		return E_FAIL;
	Safe_Release(pPokemonInfoUI01);

	CPokemonInfoUI* pPokemonInfoUI02 = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonInfoUI"), LEVEL_FEEDING, pLayerTag, (CGameObject**)&pPokemonInfoUI02,
		L"PokemonInfoUI02", "../../Reference/Resources/Data/Scene/Feeding/UI/PokemonInfoUI02.json", CLONE_FILEPATH)))
		return E_FAIL;
	if (FAILED(pPokemonInfoUI02->Init_PokemonData(2)))
		return E_FAIL;
	Safe_Release(pPokemonInfoUI02);

	CPokemonInfoUI* pPokemonInfoUI03 = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonInfoUI"), LEVEL_FEEDING, pLayerTag, (CGameObject**)&pPokemonInfoUI03,
		L"PokemonInfoUI03", "../../Reference/Resources/Data/Scene/Feeding/UI/PokemonInfoUI03.json", CLONE_FILEPATH)))
		return E_FAIL;
	if (FAILED(pPokemonInfoUI03->Init_PokemonData(3)))
		return E_FAIL;
	Safe_Release(pPokemonInfoUI03);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Feeding::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_FEEDING, L"Layer_UI")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_FEEDING, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_FEEDING, L"Layer_BuffState")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_FEEDING, L"Layer_PlayerSearcher")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_FEEDING, 
		pLayerTag, L"Player1", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster1.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageSupportMonster"), LEVEL_FEEDING, 
		pLayerTag, L"Player2", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster2.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageSupportMonster"), LEVEL_FEEDING, 
		pLayerTag, L"Player3", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster3.json", CLONE_FILEPATH)))
		return E_FAIL;

	pGameInstance->Layer_Tick_State_Change(L"Layer_Player", LEVEL_FEEDING, false);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Feeding::Ready_Layer_Manager(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_FEEDING, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Feeding_Manager"), 
		LEVEL_FEEDING, pLayerTag, L"Feeding_Manager", "../../Reference/Resources/Data/Scene/Feeding/Feeding_Manager.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Feeding* CLevel_Feeding::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Feeding* pInstance = new CLevel_Feeding(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CLevel_Feeding");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Feeding::Free()
{
	__super::Free();

}
