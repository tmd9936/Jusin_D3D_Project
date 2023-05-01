#include "stdafx.h"
#include "Level_PokemonState.h"

#include "Level_Loading.h"
#include "GameInstance.h"

#include "UI.h"

#include "PokemonInfoUI.h"
#include "PokemonSkillStoneUI.h"

#include "GoToMonStateButton.h"
#include "StoneInventory.h"
#include "PokemonState_Manager.h"

// 포켓몬 버튼 누르면 이거 번호 변경시키기
_uint CLevel_PokemonState::m_PokemonNumber = 1;

CLevel_PokemonState::CLevel_PokemonState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_PokemonState::Initialize()
{
	g_BackBufferColor = m_PokemonStateLevelBackColor;

	CGameInstance::GetInstance()->PlayBGM(TEXT("BGM_BASE.ogg"));

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Manager(TEXT("Layer_Manager"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_PokemonState::Tick(_double TimeDelta)
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("포켓몬 퀘스트"));
#endif
}

void CLevel_PokemonState::Set_PokemonNumber(const _uint& number)
{
	m_PokemonNumber = number;
}


HRESULT CLevel_PokemonState::Ready_Layer_Manager(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_POKEMONSTATE, pLayerTag)))
		return E_FAIL;

	CPokemonState_Manager* pPokemonState_Manager = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonState_Manager"), LEVEL_POKEMONSTATE, pLayerTag, (CGameObject**)&pPokemonState_Manager,
		L"PokemonState_Manager", "../../Reference/Resources/Data/Scene/PokemonInfo/PokemonState_Manager.json", CLONE_FILEPATH)))
		return E_FAIL;
	if (FAILED(pPokemonState_Manager->Init_NowMonster(m_PokemonNumber)))
		return E_FAIL;
	Safe_Release(pPokemonState_Manager);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_PokemonState::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GoToBackLevelButton"), LEVEL_POKEMONSTATE, pLayerTag, L"GoToBackLevelButton", "../../Reference/Resources/Data/Scene/PokemonInfo/Button/GoToBackLevelButton.json", CLONE_FILEPATH)))
		return E_FAIL;

	CPokemonInfoUI* pPokemonInfoUI = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonInfoUI"), LEVEL_POKEMONSTATE, pLayerTag, (CGameObject**)&pPokemonInfoUI, L"PokemonInfoUI", "../../Reference/Resources/Data/Scene/PokemonInfo/PokemonInfoUI.json", CLONE_FILEPATH)))
		return E_FAIL;
	if (FAILED(pPokemonInfoUI->Init_PokemonData(m_PokemonNumber)))
		return E_FAIL;
	Safe_Release(pPokemonInfoUI);

	CPokemonSkillStoneUI* pPokemonSkillStoneUI = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonSkillStoneUI"), LEVEL_POKEMONSTATE, pLayerTag, (CGameObject**)&pPokemonSkillStoneUI, 
		L"PokemonSkillStoneUI", "../../Reference/Resources/Data/Scene/PokemonInfo/SkillStoneUI/PokemonSkillStoneUI.json", CLONE_FILEPATH)))
		return E_FAIL;
	if (FAILED(pPokemonSkillStoneUI->Init_PokemonData(m_PokemonNumber)))
		return E_FAIL;
	Safe_Release(pPokemonSkillStoneUI);
	
	// === 다른 포켓몬 정보로 이동 버튼 시작
	_uint anotherPokemonNumber01 = 0;
	_uint anotherPokemonNumber02 = 0;

	if (m_PokemonNumber == 1)
	{
		anotherPokemonNumber01 = 2;
		anotherPokemonNumber02 = 3;
	}
	else if (m_PokemonNumber == 2)
	{
		anotherPokemonNumber01 = 1;
		anotherPokemonNumber02 = 3;
	}
	else
	{
		anotherPokemonNumber01 = 1;
		anotherPokemonNumber02 = 2;
	}

	CGoToMonStateButton* pGoToMonStateButton01 = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GoToMonStateButton"), LEVEL_POKEMONSTATE, pLayerTag, (CGameObject**)&pGoToMonStateButton01, L"GoToMonStateButton01", "../../Reference/Resources/Data/Scene/PokemonInfo/Button/GoToMonsterStateButton01.json", CLONE_FILEPATH)))
		return E_FAIL;
	if (FAILED(pGoToMonStateButton01->Change_MosnterNumber(anotherPokemonNumber01, 1)))
		return E_FAIL;
	Safe_Release(pGoToMonStateButton01);

	CGoToMonStateButton* pGoToMonStateButton02 = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GoToMonStateButton"), LEVEL_POKEMONSTATE, pLayerTag, (CGameObject**)&pGoToMonStateButton02, L"GoToMonStateButton02", "../../Reference/Resources/Data/Scene/PokemonInfo/Button/GoToMonsterStateButton02.json", CLONE_FILEPATH)))
		return E_FAIL;
	if (FAILED(pGoToMonStateButton02->Change_MosnterNumber(anotherPokemonNumber02, 1)))
		return E_FAIL;
	Safe_Release(pGoToMonStateButton02);
	// === 다른 포켓몬 정보로 이동 버튼 끝

	CStoneInventory* pStoneInventory = nullptr;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StoneInventory"), LEVEL_POKEMONSTATE, pLayerTag, (CGameObject**)&pStoneInventory, L"StoneInvetory", "../../Reference/Resources/Data/Scene/PokemonInfo/SkillStoneUI/StoneInvetory.json", CLONE_FILEPATH)))
		return E_FAIL;
	Safe_Release(pStoneInventory);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StoneInfoUI"), LEVEL_POKEMONSTATE, pLayerTag,  L"StoneInfoUI", "../../Reference/Resources/Data/Scene/PokemonInfo/StoneInfoUI/StoneInfoUI.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_PokemonState::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_POKEMONSTATE, L"Layer_UI")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_POKEMONSTATE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_POKEMONSTATE, L"Layer_BuffState")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_POKEMONSTATE, L"Layer_PlayerSearcher")))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_POKEMONSTATE, L"Layer_Player",  L"Player1", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster1.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (m_PokemonNumber == 2)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageSupportMonster"), LEVEL_POKEMONSTATE, L"Layer_Player",  L"Player2", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster2.json", CLONE_FILEPATH)))
			return E_FAIL;
	}
	else if (m_PokemonNumber == 3)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageSupportMonster"), LEVEL_POKEMONSTATE, L"Layer_Player", L"Player3", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster3.json", CLONE_FILEPATH)))
			return E_FAIL;
	}

	pGameInstance->Layer_Tick_State_Change(L"Layer_Player", LEVEL_POKEMONSTATE, false);

	Safe_Release(pGameInstance);
	return S_OK;
}

CLevel_PokemonState* CLevel_PokemonState::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_PokemonState* pInstance = new CLevel_PokemonState(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CLevel_PokemonState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_PokemonState::Free()
{
	__super::Free();

}
