#include "stdafx.h"
#include "..\Public\Level_BaseCamp.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "Utility.h"

#include "Button.h"

#include "Client_Utility.h"

#include "FlatTerrain.h"

#include "Cauldron.h"

#include "AnimEnv.h"

#include "GoToBackLevelButton.h"

CLevel_BaseCamp::CLevel_BaseCamp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_BaseCamp::Initialize()
{
	CGoToBackLevelButton::Set_BackLevel(LEVEL_BASECAMP);

	CGameInstance::GetInstance()->StopSound(SOUND_BGM);
	CGameInstance::GetInstance()->PlayBGM(TEXT("BGM_BaseCamp.ogg"));

	if (FAILED(Ready_LightDesc()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_PlayerSearcher(TEXT("Layer_PlayerSearcher"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MonsterSearcher(TEXT("Layer_MonsterSearcher"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Env(TEXT("Layer_Env"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Cauldron(TEXT("Layer_Cauldron"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Map(TEXT("Layer_Map"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Manager(TEXT("Layer_Manager"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_PlayerSkill(TEXT("Layer_PlayerSkill"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MonsterSkill(TEXT("Layer_MonsterSkill"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_InstanceEffect(TEXT("Layer_InstanceEffect"))))
	//	return E_FAIL;


	//if (FAILED(CGameInstance::GetInstance()->Add_Check_CollisionGroup(L"Layer_Player", L"Layer_Monster")))
	//	return E_FAIL;

	//if (FAILED(CGameInstance::GetInstance()->Add_Check_CollisionGroup(L"Layer_Player", L"Layer_Env")))
	//	return E_FAIL;

	CGameInstance::GetInstance()->Add_Check_CollisionGroup(L"Layer_Player", L"Layer_Monster");

	CGameInstance::GetInstance()->Add_Check_CollisionGroup(L"Layer_Monster", L"Layer_Monster");

	CGameInstance::GetInstance()->Add_Check_CollisionGroup(L"Layer_Player", L"Layer_Player");

	//CGameInstance::GetInstance()->Add_Check_CollisionGroup(L"Layer_Player", L"Layer_Env");

	CGameInstance::GetInstance()->Add_Check_CollisionGroup(L"Layer_PlayerSkill", L"Layer_Monster");

	CGameInstance::GetInstance()->Add_Check_CollisionGroup(L"Layer_MonsterSkill", L"Layer_Player");

	CGameInstance::GetInstance()->Add_Check_CollisionGroup(L"Layer_PlayerSearcher", L"Layer_Monster");

	CGameInstance::GetInstance()->Add_Check_CollisionGroup(L"Layer_MonsterSearcher", L"Layer_Player");

	CClient_Utility::Load_Layer_GameObjects("../../Reference/Resources/Data/Scene/BaseCamp/Stove.json");

	return S_OK;
}

void CLevel_BaseCamp::Tick(_double TimeDelta)
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("포켓몬 퀘스트"));
#endif
}

HRESULT CLevel_BaseCamp::Ready_Layer_Terrain(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	CFlatTerrain::FLATTERRAIN_DESC desc{};
	desc.m_Level = LEVEL_BASECAMP;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FlatTerrain"), LEVEL_BASECAMP, pLayerTag, L"Terrain", &desc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Public"), LEVEL_BASECAMP, pLayerTag, L"Main_Camera", "../../Reference/Resources/Data/Scene/BaseCamp/Camera_Public_data.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_Env(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	CAnimEnv::ANIM_ENV_DESC desc{};
	desc.ModelPrototypeTag = L"Prototype_Component_Model_Fire";
	desc.vScale = { 1.1f, 3.0f, 1.1f };
	desc.vPos = { 20.2f, -0.55f, 21.f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AnimEnv"), LEVEL_BASECAMP, pLayerTag, L"Fire1", &desc)))
		return E_FAIL;

	desc.vPos = { 19.0f, -0.55f, 19.8f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AnimEnv"), LEVEL_BASECAMP, pLayerTag, L"Fire2", &desc)))
		return E_FAIL;

	CAnimEnv::ANIM_ENV_DESC desc2{};
	desc2.ModelPrototypeTag = L"Prototype_Component_Model_Cook_effect";
	desc2.vScale = { 1.f, 1.f, 1.f };
	desc2.vPos = { 20.2f, 0.5f, 21.f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AnimEnv"), LEVEL_BASECAMP, pLayerTag, L"Cook_effect1", &desc2)))
		return E_FAIL;

	desc2.vPos = { 19.0f, 0.5f, 19.8f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AnimEnv"), LEVEL_BASECAMP, pLayerTag, L"Cook_effect2", &desc2)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AnimEnv"), LEVEL_BASECAMP, pLayerTag, L"arch", "../../Reference/Resources/Data/Scene/BaseCamp/Env/arch.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_Cauldron(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	CCauldron::CAULDRON_DESC desc1{};
	desc1.eState = CCauldron::STATE_COOK;
	desc1.vPos = { 20.2f, 0.5f, 21.f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Cauldron"), LEVEL_BASECAMP, pLayerTag, L"Cauldron1", &desc1)))
		return E_FAIL;

	desc1.vPos = { 19.0f, 0.5f, 19.77f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Cauldron"), LEVEL_BASECAMP, pLayerTag, L"Cauldron2", &desc1)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_Map(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_BASECAMP, pLayerTag, L"Map", "../../Reference/Resources/Data/Scene/BaseCamp/Map_data.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, L"Layer_BuffState")))
		return E_FAIL;

	//CMonster::POKEMON_DESC desc{};
	//desc.m_monsterNo = 25;
	//desc.vPos = _float4(22.f, 0.5f, 13.5f, 1.f);
	//desc.m_skillIDs.push_back(72);
	//desc.m_skillIDs.push_back(58);
	//desc.m_skillIDs.push_back(57);
	//desc.m_skillIDs.push_back(188);
	//desc.m_attackBasis = 50;
	//desc.m_hpBasis = 2500;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_BASECAMP, pLayerTag, L"Player1", &desc)))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_BASECAMP, pLayerTag, L"Player1", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster1.json", CLONE_FILEPATH)))
		return E_FAIL;

	////Prototype_GameObject_StageSupportMonster
	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageSupportMonster"), LEVEL_BASECAMP, pLayerTag, L"Player2", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster2.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageSupportMonster"), LEVEL_BASECAMP, pLayerTag, L"Player3", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster3.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseCampMonster"), LEVEL_BASECAMP, pLayerTag, L"NowMonster1", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster2.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseCampMonster"), LEVEL_BASECAMP, pLayerTag, L"NowMonster2", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster3.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_BASECAMP, pLayerTag, L"BaseUI01", "../../Reference/Resources/Data/Scene/BaseCamp/UI/BaseUI01.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_BASECAMP, pLayerTag, L"BaseUI02", "../../Reference/Resources/Data/Scene/BaseCamp/UI/BaseUI02.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_BASECAMP, pLayerTag, L"BaseUI03", "../../Reference/Resources/Data/Scene/BaseCamp/UI/BaseUI03.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GoToWorldMapButton"), LEVEL_BASECAMP, pLayerTag, L"GoToWorldMapButton", "../../Reference/Resources/Data/Scene/BaseCamp/Button/GoToMap_Button_data.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GoToFeedingButton"), LEVEL_BASECAMP, pLayerTag, L"GoToFeedingButton", "../../Reference/Resources/Data/Scene/BaseCamp/Button/GoToFeedingButton.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_BASECAMP, pLayerTag, L"LevelNameInfo", "../../Reference/Resources/Data/Scene/BaseCamp/UI/LevelNameInfo.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GoToMonStateButton"), LEVEL_BASECAMP, pLayerTag, L"GoToMonStateButton1", "../../Reference/Resources/Data/Scene/BaseCamp/Button/GoToMonsterStateButton01.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GoToMonStateButton"), LEVEL_BASECAMP, pLayerTag, L"GoToMonStateButton2", "../../Reference/Resources/Data/Scene/BaseCamp/Button/GoToMonsterStateButton02.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GoToMonStateButton"), LEVEL_BASECAMP, pLayerTag, L"GoToMonStateButton3", "../../Reference/Resources/Data/Scene/BaseCamp/Button/GoToMonsterStateButton03.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PokemonPowerInfoUI"), LEVEL_BASECAMP, pLayerTag, L"PokemonPowerInfoUI", "../../Reference/Resources/Data/Scene/BaseCamp/UI/PokemonPowerInfo.json", CLONE_FILEPATH)))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GetItemShowUI"), LEVEL_BASECAMP, pLayerTag, L"GetItemShowUI", "../../Reference/Resources/Data/Scene/Stage/UI/GetItemShowUI.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_Monster(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseCampMonster"), LEVEL_BASECAMP, pLayerTag, L"NowMonster1", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster2.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseCampMonster"), LEVEL_BASECAMP, pLayerTag, L"NowMonster2", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster3.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageEnemyMonster"), LEVEL_BASECAMP, pLayerTag, nullptr, "../../Reference/Resources/Data/Scene/Stage/MonsterData/EnemyData_031_boss.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageEnemyMonster"), LEVEL_BASECAMP, pLayerTag, nullptr, "../../Reference/Resources/Data/Scene/Stage/MonsterData/EnemyData_087_warter01.json", CLONE_FILEPATH)))
	//	return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_Manager(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseCamp_Manager"), LEVEL_BASECAMP, pLayerTag, L"BaseCamp_Manager", "../../Reference/Resources/Data/Scene/BaseCamp/BaseCamp_Manager_data.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_Effect(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Effect_Sakura"), LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_PlayerSkill(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_MonsterSkill(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_PlayerSearcher(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_MonsterSearcher(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_InstanceEffect(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Effect_Sakura"), LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_LightDesc()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.8f, 0.8f, 0.8f, 0.8f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	LightDesc.eType = LIGHTDESC::TYPE_POINT;
	LightDesc.vPosition = _float4(20.2f, 0.f, 21.2f, 1.f);
	LightDesc.fRange = 5.f;
	LightDesc.vDiffuse = _float4(1.f, 0.5f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.2f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	LightDesc.eType = LIGHTDESC::TYPE_POINT;
	LightDesc.vPosition = _float4(19.0f, 0.f, 19.8f, 1.f);
	LightDesc.fRange = 5.f;
	LightDesc.vDiffuse = _float4(1.f, 0.5f, 0.0f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.2f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_BaseCamp* CLevel_BaseCamp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_BaseCamp* pInstance = new CLevel_BaseCamp(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CLevel_BaseCamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_BaseCamp::Free()
{
	__super::Free();
	//CGameInstance::GetInstance()->Reset_CollisionGroup();
}
