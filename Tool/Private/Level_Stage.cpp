#include "stdafx.h"
#include "Level_Stage.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "Utility.h"

#include "Button.h"

#include "FlatTerrain.h"

CLevel_Stage::CLevel_Stage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Stage::Initialize()
{
	CGameInstance::GetInstance()->StopSound(SOUND_BGM);

	CGameInstance::GetInstance()->PlayBGM(TEXT("BGM_Waterside_2.ogg"));

	if (FAILED(Ready_LightDesc()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Env(TEXT("Layer_Env"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Inventory(TEXT("Layer_Inventory"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_StageResultUI(TEXT("Layer_StageResultUI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_PlayerSearcher(TEXT("Layer_PlayerSearcher"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MonsterSearcher(TEXT("Layer_MonsterSearcher"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Map(TEXT("Layer_Map"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_CameraTarget(TEXT("Layer_CameraTarget"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Manager(TEXT("Layer_Manager"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_StageMessageInfo(TEXT("Layer_StageMessageInfo"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_EnemySpawnPoint(TEXT("Layer_EnemySpawnPoint"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_EnemyPack(TEXT("Layer_EnemyPack"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_PlayerSkill(TEXT("Layer_PlayerSkill"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MonsterSkill(TEXT("Layer_MonsterSkill"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;


	return S_OK;
}

void CLevel_Stage::Tick(_double TimeDelta)
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("포켓몬 퀘스트"));
#endif
}

HRESULT CLevel_Stage::Ready_Layer_Manager(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Stage_Manager"), LEVEL_STAGE, pLayerTag, L"Stage_Manager", "../../Reference/Resources/Data/Scene/Stage/Stage_Manager.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_Terrain(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	CFlatTerrain::FLATTERRAIN_DESC desc{};
	desc.m_Level = LEVEL_STAGE;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FlatTerrain"), LEVEL_STAGE, pLayerTag, L"Terrain", &desc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_STAGE, pLayerTag, L"Main_Camera", "../../Reference/Resources/Data/Scene/Stage/Camera_data.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageCamera"), LEVEL_STAGE, pLayerTag, L"Main_Camera", "../../Reference/Resources/Data/Scene/Stage/Stage_Camera.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_Env(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_Map(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag)))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"Map1", "../../Reference/Resources/Data/Scene/Stage/Map/C_water1_01.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"Map2", "../../Reference/Resources/Data/Scene/Stage/Map/C_water1_02.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"Map3", "../../Reference/Resources/Data/Scene/Stage/Map/C_water1_03.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"Map4", "../../Reference/Resources/Data/Scene/Stage/Map/C_water1_04.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"Map5", "../../Reference/Resources/Data/Scene/Stage/Map/C_water1_05.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"Map6", "../../Reference/Resources/Data/Scene/Stage/Map/C_water1_06.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"Map7", "../../Reference/Resources/Data/Scene/Stage/Map/C_water1_07.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"Map8", "../../Reference/Resources/Data/Scene/Stage/Map/C_water1_08.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"Map9", "../../Reference/Resources/Data/Scene/Stage/Map/C_water1_09.json", CLONE_FILEPATH)))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"LD", "../../Reference/Resources/Data/Scene/Stage/Map/C_water_LD.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"LU", "../../Reference/Resources/Data/Scene/Stage/Map/C_water_LU.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"RD", "../../Reference/Resources/Data/Scene/Stage/Map/C_water_RD.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_STAGE, pLayerTag, L"RU", "../../Reference/Resources/Data/Scene/Stage/Map/C_water_RU.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}


HRESULT CLevel_Stage::Ready_Layer_Effect(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_PlayerSkill(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_PlayerSearcher(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_MonsterSearcher(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_Monster(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageEnemyMonster"), LEVEL_STAGE, pLayerTag, nullptr, "../../Reference/Resources/Data/Scene/Stage/MonsterData/EnemyData_031_boss.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageEnemyMonster"), LEVEL_STAGE, pLayerTag, nullptr, "../../Reference/Resources/Data/Scene/Stage/MonsterData/EnemyData_087_warter01.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageEnemyMonster"), LEVEL_STAGE, pLayerTag, nullptr, "../../Reference/Resources/Data/Scene/Stage/MonsterData/EnemyData_104_warter01.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageEnemyMonster"), LEVEL_STAGE, pLayerTag, nullptr, "../../Reference/Resources/Data/Scene/Stage/MonsterData/EnemyData_105_warter01.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageEnemyMonster"), LEVEL_STAGE, pLayerTag, nullptr, "../../Reference/Resources/Data/Scene/Stage/MonsterData/EnemyData_032_warter01.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageEnemyMonster"), LEVEL_STAGE, pLayerTag, nullptr, "../../Reference/Resources/Data/Scene/Stage/MonsterData/EnemyData_033_warter01.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_MonsterSkill(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, L"Layer_BuffState")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, L"Layer_Pokering")))
		return E_FAIL;

	//CMonster::POKEMON_DESC desc{};
	//desc.m_monsterNo = 25;
	//desc.vPos = _float4(19.2f, 0.5f, 22.0f, 1.f);
	//desc.m_skillIDs.push_back(24);
	//desc.m_skillIDs.push_back(58);
	//desc.m_skillIDs.push_back(57);
	//desc.m_skillIDs.push_back(25);
	//desc.m_attackBasis = 50;
	//desc.m_hpBasis = 2500;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_STAGE, pLayerTag, L"Player1", &desc)))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_STAGE, pLayerTag, L"Player1", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster1.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageSupportMonster"), LEVEL_STAGE, pLayerTag, L"Player2", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster2.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageSupportMonster"), LEVEL_STAGE, pLayerTag, L"Player3", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster3.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_STAGE, pLayerTag, L"SkillBase1", "../../Reference/Resources/Data/Scene/Stage/UI/Skill_BackGround_UI1.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_STAGE, pLayerTag, L"SkillBase2", "../../Reference/Resources/Data/Scene/Stage/UI/Skill_BackGround_UI2.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_STAGE, pLayerTag, L"SkillBase3", "../../Reference/Resources/Data/Scene/Stage/UI/Skill_BackGround_UI3.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_STAGE, pLayerTag, L"PokemonIcon1", "../../Reference/Resources/Data/Scene/Stage/UI/Pokemon_Icon1.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_STAGE, pLayerTag, L"PokemonIcon2", "../../Reference/Resources/Data/Scene/Stage/UI/Pokemon_Icon2.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_STAGE, pLayerTag, L"PokemonIcon3", "../../Reference/Resources/Data/Scene/Stage/UI/Pokemon_Icon3.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonSkillButton"), LEVEL_STAGE, pLayerTag, L"Player1_Skill0", "../../Reference/Resources/Data/Scene/Stage/Button/Player1_Skill0.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonSkillButton"), LEVEL_STAGE, pLayerTag, L"Player1_Skill1", "../../Reference/Resources/Data/Scene/Stage/Button/Player1_Skill1.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonSkillButton"), LEVEL_STAGE, pLayerTag, L"Player2_Skill0", "../../Reference/Resources/Data/Scene/Stage/Button/Player2_Skill0.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonSkillButton"), LEVEL_STAGE, pLayerTag, L"Player2_Skill1", "../../Reference/Resources/Data/Scene/Stage/Button/Player2_Skill1.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonSkillButton"), LEVEL_STAGE, pLayerTag, L"Player3_Skill0", "../../Reference/Resources/Data/Scene/Stage/Button/Player3_Skill0.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonSkillButton"), LEVEL_STAGE, pLayerTag, L"Player3_Skill1", "../../Reference/Resources/Data/Scene/Stage/Button/Player3_Skill1.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageProgressUI"), LEVEL_STAGE, pLayerTag, L"StageProgressUI", "../../Reference/Resources/Data/Scene/Stage/UI/StageProgressUI.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GetItemShowUI"), LEVEL_STAGE, pLayerTag, L"GetItemShowUI", "../../Reference/Resources/Data/Scene/Stage/UI/GetItemShowUI.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StoneInventory"), LEVEL_STAGE, pLayerTag,
		L"StoneInvetory", "../../Reference/Resources/Data/Scene/PokemonInfo/SkillStoneUI/StoneInvetory.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FoodInfoUI"), LEVEL_STAGE, pLayerTag, L"FoodInfoUI01", "../../Reference/Resources/Data/Scene/Feeding/UI/FoodInfoUI01.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FoodInfoUI"), LEVEL_STAGE, pLayerTag, L"FoodInfoUI02", "../../Reference/Resources/Data/Scene/Feeding/UI/FoodInfoUI02.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FoodInfoUI"), LEVEL_STAGE, pLayerTag, L"FoodInfoUI03", "../../Reference/Resources/Data/Scene/Feeding/UI/FoodInfoUI03.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FoodInfoUI"), LEVEL_STAGE, pLayerTag, L"FoodInfoUI04", "../../Reference/Resources/Data/Scene/Feeding/UI/FoodInfoUI04.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FoodInventory"), LEVEL_STAGE, pLayerTag,
		L"FoodInventory", "../../Reference/Resources/Data/Scene/Feeding/FoodInventory.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_StageResultUI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_STAGE, pLayerTag, L"LevelNameInfo", "../../Reference/Resources/Data/Scene/Stage/StageResultUI/StageResultNameInfo.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StoneInfoUI"), LEVEL_STAGE, pLayerTag, L"StoneInfoUI01", "../../Reference/Resources/Data/Scene/Stage/StageResultUI/StoneInfoUI01.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StoneInfoUI"), LEVEL_STAGE, pLayerTag, L"StoneInfoUI02", "../../Reference/Resources/Data/Scene/Stage/StageResultUI/StoneInfoUI02.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StoneInfoUI"), LEVEL_STAGE, pLayerTag, L"StoneInfoUI03", "../../Reference/Resources/Data/Scene/Stage/StageResultUI/StoneInfoUI03.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageStoneResult"), LEVEL_STAGE, pLayerTag, L"StageStoneResult", "../../Reference/Resources/Data/Scene/Stage/StageResultUI/StageStoneResult.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FoodInfoUI"), LEVEL_STAGE, pLayerTag, L"FoodInfoUI01", "../../Reference/Resources/Data/Scene/Stage/StageResultUI/FoodInfoUI01.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FoodInfoUI"), LEVEL_STAGE, pLayerTag, L"FoodInfoUI02", "../../Reference/Resources/Data/Scene/Stage/StageResultUI/FoodInfoUI02.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FoodInfoUI"), LEVEL_STAGE, pLayerTag, L"FoodInfoUI03", "../../Reference/Resources/Data/Scene/Stage/StageResultUI/FoodInfoUI03.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FoodInfoUI"), LEVEL_STAGE, pLayerTag, L"FoodInfoUI04", "../../Reference/Resources/Data/Scene/Stage/StageResultUI/FoodInfoUI04.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageFoodResult"), LEVEL_STAGE, pLayerTag, L"StageFoodResult", "../../Reference/Resources/Data/Scene/Stage/StageResultUI/StageFoodResult.json", CLONE_FILEPATH)))
		return E_FAIL;

	pGameInstance->Layer_Tick_State_Change(pLayerTag, LEVEL_STAGE, false);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_Inventory(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	//pGameInstance->Layer_Tick_State_Change(pLayerTag, LEVEL_STAGE, false);
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_CameraTarget(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageCameraTarget"), LEVEL_STAGE, pLayerTag, L"CameraTarget")))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_EnemySpawnPoint(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EnemySpawnPoint"), LEVEL_STAGE, pLayerTag, L"SpawnPoint1", "../../Reference/Resources/Data/Scene/Stage/EnemySpawnPoint/EnemySpwanPoint0_warter01.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EnemySpawnPoint"), LEVEL_STAGE, pLayerTag, L"SpawnPoint2", "../../Reference/Resources/Data/Scene/Stage/EnemySpawnPoint/EnemySpwanPoint1_warter01.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EnemySpawnPoint"), LEVEL_STAGE, pLayerTag, L"SpawnPoint3", "../../Reference/Resources/Data/Scene/Stage/EnemySpawnPoint/EnemySpwanPoint2_warter01.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EnemySpawnPoint"), LEVEL_STAGE, pLayerTag, L"SpawnPoint4", "../../Reference/Resources/Data/Scene/Stage/EnemySpawnPoint/EnemySpwanPoint3_warter01.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_EnemyPack(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EnemyPack"), LEVEL_STAGE, pLayerTag, L"EnemyPack", "../../Reference/Resources/Data/Scene/Stage/EnemyPack/EnemyPack_water1_001.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage::Ready_Layer_StageMessageInfo(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageMessageInfo"), LEVEL_STAGE, pLayerTag, L"StageMessgeInfo", "../../Reference/Resources/Data/Scene/Stage/UI/StageMessageInfo.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageClearUI"), LEVEL_STAGE, pLayerTag, L"StageClearUI", "../../Reference/Resources/Data/Scene/Stage/UI/StageClearUI.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage::Ready_LightDesc()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.45f, 0.45f, 0.45f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Stage* CLevel_Stage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Stage* pInstance = new CLevel_Stage(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CLevel_Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage::Free()
{
	__super::Free();

}
