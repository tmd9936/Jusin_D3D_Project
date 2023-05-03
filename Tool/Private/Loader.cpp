#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "MainApp.h"

#include "BackGround.h"
#include "UI.h"
#include "FlatTerrain.h"
#include "Camera_Dynamic.h"
#include "Camera_Public.h"

#include "Searcher.h"
#include "Mouse.h"

#include "ModelUI.h"
#include "Map.h"
#include "Stove.h"
#include "Effect_Sakura.h"

#include "WorldMapFlower.h"
#include "WorldMapGrass.h"
#include "WorldMapCloud.h"
#include "WorldMapAnimEnv.h"
#include "WorldMap_Manager.h"
#include "WorldMapBackToIdel.h"
#include "GoToBaseCampButton.h"
#include "GoToMonStateButton.h"
#include "PokemonPowerInfoUI.h"

#include "StageInfoUI.h"
#include "StagePoint.h"
#include "GoToStageButton.h"

#include "AnimEnv.h"

#include "Player.h"
#include "GoToWorldMapButton.h"
#include "BaseCampMonster.h"
#include "BaseCamp_Manager.h"
#include "Cauldron.h"

#include "Food.h"

#include "BuffState.h"
//#include "Weapon.h"
#include "Effect.h"
#include "Effect_Manager.h"
#include "Skill.h"
#include "Skill_Manager.h"
#include "PokemonData.h"

#include "Navigation.h"
#include "PartTexture.h"
#include "PartText.h"

#include "PokemonSkillButton.h"
#include "StageCamera.h"
#include "StageCameraTarget.h"

#include "StageEnemyMonster.h"
#include "StageSupportMonster.h"
#include "EnemySpawnPoint.h"
#include "EnemyPack.h"
#include "StageMessageInfo.h"
#include "StageProgressUI.h"
#include "StageClearUI.h"
#include "StageStoneResult.h"

#include "HP.h"
#include "HpBar.h"
#include "Formation.h"
#include "Trail.h"

#include "DamageText.h"

#include "Attack.h"

#include "SkillEffect.h"
#include "HommingAttackEffect.h"
#include "RushAttackEffect.h"
#include "ChargeEffect.h"
#include "BezierAttackEffect.h"
#include "BumerangAttackEffect.h"

#include "ThreadPool.h"

#include "MiscData.h"
#include "Stage_Manager.h"

#include "ManualCollisionState.h"

#include "ConditionData.h"

#include "Pokering.h"

#include "PokemonInfoUI.h"
#include "PokemonSkillStoneUI.h"
#include "GoToBackLevelButton.h"
#include "GoToFeedingButton.h"
#include "SkillInfoUI.h"
#include "StoneEquipInfoUI.h"
#include "StoneInventory.h"
#include "StoneInfoUI.h"

#include "Stone.h"

#include "PokemonState_Manager.h"
#include "GetItemShowUI.h"

#include "FoodInfoUI.h"
#include "StageFoodResult.h"
#include "Feeding_Manager.h"
#include "FoodInventory.h"

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
	case LEVEL_LOGO: 
		hr = pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_BASECAMP:
		hr = pLoader->Loading_ForBaseCampLevel();
		break;
	case LEVEL_WORLDMAP:
		hr = pLoader->Loading_ForWorldMapLevel();
		break;
	case LEVEL_STAGE:
		hr = pLoader->Loading_ForStageLevel();
		break;
	case LEVEL_POKEMONSTATE:
		hr = pLoader->Loading_ForPokemonStateLevel();
		break;
	case LEVEL_FEEDING:
		hr = pLoader->Loading_ForFeedingLevel();
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

_uint APIENTRY LoadingDenkiEffect()
{	
	while (!CMainApp::Get_MainAppInit()) {}

	CThreadPool::GetInstance()->JobStart();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	HRESULT			hr = { 0 };

	wstring modelName = L"Prototype_Component_Model_Pokemon_PM31";
	_matrix PivotMatrix = XMMatrixIdentity();

	//if (false == pGameInstance->Check_Prototype(modelName.c_str()))
	//{
		PivotMatrix = XMMatrixScaling(0.4f, 0.4f, 0.4f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM31.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;
	//}

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_Charge",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_Charge.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_Negative",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_Negative.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BGB_Denki",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BGB_Denki.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BS_Denki",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BS_Denki.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BM_Denki",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BM_Denki.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BR_Denki",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BR_Denki.fbx", PivotMatrix))))
		return	E_FAIL;

	PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BD_Denki",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BD_Denki.fbx", PivotMatrix))))
		return	E_FAIL;

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BW_Denki",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BW_Denki.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BB_Denki",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BB_Denki.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_SP_Kaminari_Start",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_SP_Kaminari_Start.fbx", PivotMatrix))))
		return	E_FAIL;

	CThreadPool::GetInstance()->JobEnd();

	return 0;
}

_uint APIENTRY LoadingDokuEffect()
{
	while (!CMainApp::Get_MainAppInit()) {}

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	HRESULT			hr = { 0 };

	while (nullptr == CThreadPool::GetInstance())
	{

	}

	CThreadPool::GetInstance()->JobStart();

	_matrix PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BS_Doku",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BS_Doku.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BM_Doku",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BM_Doku.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BR_Doku",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BR_Doku.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_SP_Tsuno",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_SP_Tsuno.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BG_Doku",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BG_Doku.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BBB_Doku",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BBB_Doku.fbx", PivotMatrix))))
		return	E_FAIL;

	PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BD_Doku",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BD_Doku.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BM_Iwa",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BM_Iwa.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BD_Iwa",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BD_Iwa.fbx", PivotMatrix))))
		return	E_FAIL;

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BG_Iwa",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BG_Iwa.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BS_Iwa",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BS_Iwa.fbx", PivotMatrix))))
		return	E_FAIL;


	CThreadPool::GetInstance()->JobEnd();

	return 0;
}

_uint APIENTRY LoadingNormalEffect()
{
	while (!CMainApp::Get_MainAppInit()) {}

	CThreadPool::GetInstance()->JobStart();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();


	HRESULT			hr = { 0 };

	_matrix PivotMatrix = XMMatrixIdentity();

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BBB_Normal",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BBB_Normal.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BCB_Normal",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BCB_Normal.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BB_Normal",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BB_Normal.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BS_Normal",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BS_Normal.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BW_Normal",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BW_Normal.fbx", PivotMatrix))))
		return	E_FAIL;

	PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BD_Normal",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BD_Normal.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BPB_Normal_Start",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BPB_Normal_Start.fbx", PivotMatrix))))
		return	E_FAIL;

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BPB_Normal_Loop",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BPB_Normal_Loop.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_damage00",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_damage00.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_damage01",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_damage01.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_Onpabullet",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_Onpabullet.fbx", PivotMatrix))))
		return	E_FAIL;

	CThreadPool::GetInstance()->JobEnd();

	return 0;
}

_uint APIENTRY LoadingKooriEffect()
{
	while (!CMainApp::Get_MainAppInit()) {}

	CThreadPool::GetInstance()->JobStart();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	HRESULT			hr = { 0 };

	_matrix PivotMatrix = XMMatrixIdentity();

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BS_Koori",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BS_Koori.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BL_Koori_Reserve",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BL_Koori_Reserve.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BL_Koori",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BL_Koori.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BM_Koori",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BM_Koori.fbx", PivotMatrix))))
		return	E_FAIL;

	PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BD_Koori",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BD_Koori.fbx", PivotMatrix))))
		return	E_FAIL;

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BR_Koori",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BR_Koori.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BC_Mizu",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BC_Mizu.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BP_Mizu_Start",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BP_Mizu_Start.fbx", PivotMatrix))))
		return	E_FAIL;

	PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BD_Mizu",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BD_Mizu.fbx", PivotMatrix))))
		return	E_FAIL;;

	CThreadPool::GetInstance()->JobEnd();

	return 0;
}

_uint APIENTRY LoadingJimenEffect()
{
	while (!CMainApp::Get_MainAppInit()) {}

	CThreadPool::GetInstance()->JobStart();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	HRESULT			hr = { 0 };

	_matrix PivotMatrix = XMMatrixIdentity();

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BD_Jimen",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BD_Jimen.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BS_Jimen",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BS_Jimen.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BR_Jimen",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BR_Jimen.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_BM_Jimen",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_BM_Jimen.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_SP_Honebuumeran",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_SP_Honebuumeran.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_Positive",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_Positive.fbx", PivotMatrix))))
		return	E_FAIL;

	PivotMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_Flash",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_Flash.fbx", PivotMatrix))))
		return	E_FAIL;

	PivotMatrix = XMMatrixScaling(0.4f, 0.4f, 0.4f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_damage_boss",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_damage_boss.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_damage_boss_end",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_damage_boss_end.fbx", PivotMatrix))))
		return	E_FAIL;


	CThreadPool::GetInstance()->JobEnd();

	return 0;
}


_uint APIENTRY LoadingStaticManagerObject()
{
	while (!CMainApp::Get_MainAppInit()) {}

	CThreadPool::GetInstance()->JobStart();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	HRESULT			hr = { 0 };


#pragma region GAMEOBJECTS
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_LOGO))
	{
		_matrix PivotMatrix = XMMatrixIdentity();
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Mouse"),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_ANIM, "../../Reference/Resources/Mesh/Animation/Logo/touch_marker.fbx", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_GameObject_Effect_Manager */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Manager"),
			CEffect_Manager::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Data/Effect/EffectDataSet.json"))))
			return E_FAIL;

	
		/* For.Prototype_GameObject_Skill_Manager */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_Manager"),
			CSkill_Manager::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Data/Skill/SkillDependDataSet.json", "../../Reference/Resources/Data/Skill/SkillDataResourcesSet.json"))))
			return E_FAIL;

		/* For.Prototype_GameObject_Mouse*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mouse"),
			CMouse::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice()))))
			return E_FAIL;

		/* For.Prototype_GameObject_MiscData */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MiscData"),
			CMiscData::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Data/Misc/Misc_data.json"))))
			return E_FAIL;

		/* For.Prototype_GameObject_MiscData */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ConditionData"),
			CConditionData::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(),
				"../../Reference/Resources/Data/Condition_Buff_DeBuff/ConditionDataSet.json",
				"../../Reference/Resources/Data/Condition_Buff_DeBuff/ConditionTypeDataSet.json",
				"../../Reference/Resources/Data/Condition_Buff_DeBuff/ConditionParameter.json"))))
			return E_FAIL;

		/* For.Prototype_GameObject_PokemonData */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PokemonData"),
			CPokemonData::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Data/Pokemon/PokemonData.json"))))
			return E_FAIL;

	}
#pragma endregion

#pragma region STATIC_GAMEOBJECTS
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_LOGO))
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Effect_Manager"), LEVEL_STATIC, L"Layer_Manager", L"Effect_Manager")))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Skill_Manager"), LEVEL_STATIC, L"Layer_Manager", L"Skill_Manager")))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_Mouse", LEVEL_STATIC, L"Layer_Mouse", L"Mouse")))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MiscData", LEVEL_STATIC, L"Layer_Manager", L"MiscData")))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_ConditionData", LEVEL_STATIC, L"Layer_Manager", L"ConditionData")))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_PokemonData", LEVEL_STATIC, L"Layer_Manager", L"PokemonData")))
			return E_FAIL;
	}
#pragma endregion

	CThreadPool::GetInstance()->JobEnd();

	return 0;
}

_uint APIENTRY LoadingConditionEffect()
{
	while (!CMainApp::Get_MainAppInit()) {}

	CThreadPool::GetInstance()->JobStart();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	HRESULT			hr = { 0 };

	_matrix PivotMatrix = XMMatrixIdentity();

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_Burn",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_Burn.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_Freeze",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_Freeze.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_Paralyse",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_Paralyse.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_Confuse",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_Confuse.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_Poison",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_Poison.fbx", PivotMatrix))))
		return	E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_E_EF_Kanasibari",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Effect/E_EF_Kanasibari.fbx", PivotMatrix))))
		return	E_FAIL;

	CThreadPool::GetInstance()->JobEnd();

	return 0;
}

_uint APIENTRY Loading_BC_cauldron01()
{
	while (!CMainApp::Get_MainAppInit()) {}

	CThreadPool::GetInstance()->JobStart();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	HRESULT			hr = { 0 };

	_matrix PivotMatrix = XMMatrixIdentity();

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_break",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/BC_cauldron01_break.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_change",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/BC_cauldron01_change.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_close",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/BC_cauldron01_close.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_cook",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/BC_cauldron01_cook.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_fix",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/BC_cauldron01_fix.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_idle",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/BC_cauldron01_idle.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_press",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/BC_cauldron01_press.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_select",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/BC_cauldron01_select.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_BC_cauldron01_set",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/BC_cauldron01_set.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Fire",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/Fire.fbx", PivotMatrix))))
		return	E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Model_Cook_effect",
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/Cook_effect.fbx", PivotMatrix))))
		return	E_FAIL;

	CThreadPool::GetInstance()->JobEnd();

	return 0;
}


_uint APIENTRY LoadingMap()
{
	while (!CMainApp::Get_MainAppInit()) {}

	CThreadPool::GetInstance()->JobStart();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	HRESULT			hr = { 0 };

	_matrix		PivotMatrix = XMMatrixIdentity();

	/* For.Prototype_Component_Model_Stage_Map */
	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_Map1"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_01.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_Map2"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_02.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_Map3"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_03.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_Map4"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_04.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_Map5"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_05.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_Map6"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_06.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_Map7"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_07.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_Map8"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_08.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_Map9"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_09.fbx", PivotMatrix))))
		return E_FAIL;

	CThreadPool::GetInstance()->JobEnd();

	return hr;
}

_uint APIENTRY LoadingStaticShader()
{
	while (!CMainApp::Get_MainAppInit()) {}

	CThreadPool::GetInstance()->JobStart();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	HRESULT			hr = { 0 };

	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_LOGO))
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
			CShader::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxModel */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
			CShader::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxModelColor */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
			CShader::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxModelColor.hlsl"), VTXNORCOLOR_DECLARATION::Elements, VTXNORCOLOR_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxAnimModelColor */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
			CShader::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxAnimModelColor.hlsl"), VTXCOLORANIMMODEL_DECLARATION::Elements, VTXCOLORANIMMODEL_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxTexColor */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexColor"),
			CShader::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxTexColor.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxNorTex_HeightTerrain */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex_HeightTerrain"),
			CShader::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxNorTex_HeightTerrain.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxNorTex */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
			CShader::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxInstance */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxInstance"),
			CShader::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxInstance.hlsl"), VTXINSTANCE_DECLARATION::Elements, VTXINSTANCE_DECLARATION::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxInstance */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
			CShader::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxPointInstance.hlsl"), VTXPOINTINSTANCE_DECLARATION::Elements, VTXPOINTINSTANCE_DECLARATION::iNumElements))))
			return E_FAIL;
	}
	
	CThreadPool::GetInstance()->JobEnd();

	return hr;
}

_uint APIENTRY LoadingWorldMapMesh()
{
	while (!CMainApp::Get_MainAppInit()) {}

	CThreadPool::GetInstance()->JobStart();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	HRESULT			hr = { 0 };

	_matrix		PivotMatrix = XMMatrixIdentity();
	/* For.Prototype_Component_Model_BaseCamp_Field */
	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Model_WorldMap_island"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/Animation/WorldMap/W_island.fbx", PivotMatrix, true))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Model_WorldMap_Cloud"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/Animation/WorldMap/W_cloud.fbx", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Model_WorldMap_Flower"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/Animation/WorldMap/W_flower.fbx", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Model_WorldMap_Grass"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/Animation/WorldMap/W_grass.fbx", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Model_WorldMap_Seawave"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/WorldMap/W_seawave.fbx", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Model_WorldMap_Ship"),
		CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/WorldMap/W_ship.fbx", PivotMatrix))))
		return E_FAIL;

	CThreadPool::GetInstance()->JobEnd();

	return hr;
}

_uint APIENTRY LoadingPokemonModel()
{
	while (!CMainApp::Get_MainAppInit()) {}

	CThreadPool::GetInstance()->JobStart();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	HRESULT			hr = { 0 };

	_matrix		PivotMatrix = XMMatrixIdentity();

	wstring modelName = L"Prototype_Component_Model_Pokemon_PM6";
	//if (false == pGameInstance->Check_Prototype(modelName.c_str()))
	//{
	//	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
	//		CModel::Create(m_pDevice, m_pContext, "../../Reference/Resources/Mesh/Animation/Pokemon/PM6.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
	//		return E_FAIL;

	//}
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_BASECAMP))
	{
		// 어니부기
		modelName = L"Prototype_Component_Model_Pokemon_PM8";
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM8.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;

		// 거북왕
		modelName = L"Prototype_Component_Model_Pokemon_PM9";
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM9.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;

		// 피카츄
		modelName = L"Prototype_Component_Model_Pokemon_PM25";
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM25.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;

		// 라이츄
		modelName = L"Prototype_Component_Model_Pokemon_PM26";
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM26.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;

		// 니드런
		modelName = L"Prototype_Component_Model_Pokemon_PM32";
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM32.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;

		// 니드리노
		modelName = L"Prototype_Component_Model_Pokemon_PM33";
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM33.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;

		// 데구리
		modelName = L"Prototype_Component_Model_Pokemon_PM75";
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM75.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;

		// 딱구리
		modelName = L"Prototype_Component_Model_Pokemon_PM76";
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM76.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;

		// 쥬레곤
		modelName = L"Prototype_Component_Model_Pokemon_PM87";
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM87.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;

		// 롱스톤
		modelName = L"Prototype_Component_Model_Pokemon_PM95";
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM95.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;

		// 탕구리
		modelName = L"Prototype_Component_Model_Pokemon_PM104";
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM104.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;

		// 텅구리
		modelName = L"Prototype_Component_Model_Pokemon_PM105";
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, modelName.c_str(),
			CModel::Create(pGameInstance->Get_Device(), pGameInstance->Get_ContextDevice(), "../../Reference/Resources/Mesh/Animation/Pokemon/PM105.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
			return E_FAIL;
	}

	CThreadPool::GetInstance()->JobEnd();

	return hr;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	m_eNextLevelID = eNextLevelID;
	if (eNextLevelID == LEVEL_LOGO)
	{
		std::function<_uint()> fun1 = std::function<_uint()>(LoadingDenkiEffect);
		CThreadPool::GetInstance()->QueueJob(fun1);
		CThreadPool::GetInstance()->QueueJob(std::function<_uint()>(LoadingNormalEffect));
		CThreadPool::GetInstance()->QueueJob(std::function<_uint()>(LoadingDokuEffect));
		CThreadPool::GetInstance()->QueueJob(std::function<_uint()>(LoadingKooriEffect));
		CThreadPool::GetInstance()->QueueJob(std::function<_uint()>(LoadingStaticShader));
		CThreadPool::GetInstance()->QueueJob(std::function<_uint()>(LoadingJimenEffect));
		CThreadPool::GetInstance()->QueueJob(std::function<_uint()>(LoadingStaticManagerObject));
	}
	else if (eNextLevelID == LEVEL_BASECAMP)
	{
		if (false == pGameInstance->Get_LevelFirstInit(LEVEL_BASECAMP))
		{
			CThreadPool::GetInstance()->QueueJob(std::function<_uint()>(LoadingPokemonModel));
			CThreadPool::GetInstance()->QueueJob(std::function<_uint()>(Loading_BC_cauldron01));
			CThreadPool::GetInstance()->QueueJob(std::function<_uint()>(LoadingConditionEffect));
		}
	}
	else if (eNextLevelID == LEVEL_WORLDMAP)
	{
		CThreadPool::GetInstance()->QueueJob(std::function<_uint()>(LoadingWorldMapMesh));
	}
	else if (eNextLevelID == LEVEL_STAGE)
	{
		CThreadPool::GetInstance()->QueueJob(std::function<_uint()>(LoadingMap));
	}

	

	InitializeCriticalSection(m_CriticalSection);

	/* 스레드를 생성한다. */
	/* 스레드를 생성 후 진입점함수를 정의. */
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
	//pGameInstance->PlayBGM(TEXT("BGM_Title.ogg"));
	/*  */		  
#pragma region TEXTURES	
	wsprintf(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* For.Prototype_Component_Texture */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/splash/TitleLogo_Korea.dds")))))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/splash/title/title%d.dds"), 31))))
		return E_FAIL;

	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_LOGO))
	{
		/* For.Prototype_Component_Texture_Terrain */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_TerrainMask */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TerrainMask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.dds")))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Brush */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Brush"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png")))))
			return E_FAIL;

		/* For.Prototype_Component_Button_Color_Mask */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Button_Color_Mask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_button.png")))))
			return E_FAIL;

		/* For.Prototype_Component_Window_Plane_Corner */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Window_Plane_Corner"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_plane_corner.dds")))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Window_Plane_Corner_Waku */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Window_Plane_Corner_Waku"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_plane_cornerwaku.dds")))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Window_Plane_Corner_Waku */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Window_Plane_Corner_Waku_Mini"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_plane_cornerwaku_mini.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Drop_Result_Base"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/drop/Drop_Result_Base.dds")))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Window_Plane_Bar */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Window_Plane_Corner_Bar"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_plane_corner_bar.dds")))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Button_Allround_Exit */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Button_Allround_Exit"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/UI/button_allround_exit.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Rectangle_Base"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/Rectangle_Base.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Sakura"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Effect/sakura.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UIMask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_State/UIMask.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LevelNameBase"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/UI/LevelNameBase.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BaseCamp_Icon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/UI/BaseCamp_Icon.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Worldmap_Icon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/UI/Worldmap_Icon.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Stage_result_Icon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/UI/Stage_result_Icon.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Trail/effecttrail.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Check_mark"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/UI/Check_mark.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_dent"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/dent.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_Blue_UC"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Item/Item_Blue_UC.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_Grey_UC"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Item/Item_Grey_UC.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_Red_UC"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Item/Item_Red_UC.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_Yellow_UC"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Item/Item_Yellow_UC.dds")))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_DropUI_Base */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DropUI_Base"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/drop/DropUI_Base.dds")))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_drop_icon_stone */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_drop_icon_stone"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/drop/drop_icon_stone.dds")))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_drop_icon_kinomi */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_drop_icon_kinomi"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/drop/drop_icon_kinomi.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_ATK_icon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_ATK_icon.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_HP_icon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_HP_icon.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Feeding_Icon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/UI/Feeding_Icon.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BC_cauldron_base2"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/UI/BC_cauldron_base2.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BC_cauldron_futa2"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/UI/BC_cauldron_futa2.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_disolveMask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Effect/disolveMask.dds")))))
			return E_FAIL;
	}

#pragma endregion

#pragma region MODELS
	wsprintf(m_szLoadingText, TEXT("STATIC 컴포넌트를 로딩중입니다."));

	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_LOGO))
	{
		/* For.Prototype_Component_Billboard */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Billboard"),
			CBillboard::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		
		/* For.Prototype_Component_MonFSM */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_MonFSM"),
			CMonFSM::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_PickingCube */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_PickingCube"),
			CPickingCube::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_AABB */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
			CAABB::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_OBB */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
			COBB::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_Sphere */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			CSphere::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_HP */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_HP"),
			CHP::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_Attack */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Attack"),
			CAttack::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_Formation */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Formation"),
			CFormation::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_VIBuffer_Rect_Instance */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Instance"),
			CVIBuffer_Rect_Instance::Create(m_pDevice, m_pContext, 100))))
			return E_FAIL;


		/* For.Prototype_Component_VIBuffer_Point_Instance */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
			CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, 300))))
			return E_FAIL;

		/* For.Prototype_Component_VIBuffer_Trail */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"),
			CVIBuffer_Trail::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/*이펙트에서 몬스터의 이 컴포넌트 가져와서 수동으로 히트상태로 만들고 몬스터에서 이거로 히트 판단*/
		/* For.Prototype_Component_ManualCollisionState */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ManualCollisionState"),
			CManualCollisionState::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StageStoneResult"),
			CStageStoneResult::Create(m_pDevice, m_pContext))))
			return E_FAIL;

	}

	wsprintf(m_szLoadingText, TEXT("모델을 로딩중입니다."));
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_LOGO))
	{

	}

#pragma endregion

#pragma region SHADERS
	wsprintf(m_szLoadingText, TEXT("셰이더를 로딩중입니다."));
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_LOGO))
	{
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		//	return E_FAIL;

		///* For.Prototype_Component_Shader_VtxModel */
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
		//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		//	return E_FAIL;

		///* For.Prototype_Component_Shader_VtxModelColor */
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelColor"),
		//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxModelColor.hlsl"), VTXNORCOLOR_DECLARATION::Elements, VTXNORCOLOR_DECLARATION::iNumElements))))
		//	return E_FAIL;

		///* For.Prototype_Component_Shader_VtxAnimModelColor */
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModelColor"),
		//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxAnimModelColor.hlsl"), VTXCOLORANIMMODEL_DECLARATION::Elements, VTXCOLORANIMMODEL_DECLARATION::iNumElements))))
		//	return E_FAIL;

		///* For.Prototype_Component_Shader_VtxTexColor */
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexColor"),
		//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxTexColor.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		//	return E_FAIL;

		///* For.Prototype_Component_Shader_VtxNorTex_HeightTerrain */
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex_HeightTerrain"),
		//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxNorTex_HeightTerrain.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		//	return E_FAIL;

		///* For.Prototype_Component_Shader_VtxNorTex */
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		//	return E_FAIL;

		///* For.Prototype_Component_Shader_VtxInstance */
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxInstance"),
		//	CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_VtxInstance.hlsl"), VTXINSTANCE_DECLARATION::Elements, VTXINSTANCE_DECLARATION::iNumElements))))
		//	return E_FAIL;
	}
#pragma endregion

#pragma region GAMEOBJECTS
	wsprintf(m_szLoadingText, TEXT("LEVEL_STATIC 객체원형을 로딩중."));
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_LOGO))
	{
		/* For.Prototype_GameObject_Effect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
			CEffect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Skill */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill"),
			CSkill::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StageMessageInfo"),
			CStageMessageInfo::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Trail */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Trail"),
			CTrail::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StageClearUI"),
			CStageClearUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AnimEnv"),
			CAnimEnv::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StoneInfoUI"),
			CStoneInfoUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FoodInfoUI"),
			CFoodInfoUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StageFoodResult"),
			CStageFoodResult::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}
#pragma endregion

#pragma region STATIC_GAMEOBJECTS
	wsprintf(m_szLoadingText, TEXT("LEVEL_STATIC 객체 로딩중."));
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_LOGO))
	{
		//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Effect_Manager"), LEVEL_STATIC, L"Layer_Manager", L"Effect_Manager")))
		//	return E_FAIL;

		//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Skill_Manager"), LEVEL_STATIC, L"Layer_Manager", L"Skill_Manager")))
		//	return E_FAIL;

		//if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_Mouse", LEVEL_STATIC, L"Layer_Mouse", L"Mouse")))
		//	return E_FAIL;

		//if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MiscData", LEVEL_STATIC, L"Layer_Manager", L"MiscData")))
		//	return E_FAIL;

		//if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_ConditionData", LEVEL_STATIC, L"Layer_Manager", L"ConditionData")))
		//	return E_FAIL;

		//if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_PokemonData", LEVEL_STATIC, L"Layer_Manager", L"PokemonData")))
		//	return E_FAIL;
	}
#pragma endregion


#pragma region GAMEOBJECTS
	wsprintf(m_szLoadingText, TEXT("객체원형을 로딩중."));

	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_LOGO))
	{
		//Prototype_GameObject_Effect_Sakura

		/* For.Prototype_GameObject_UI */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Sakura"),
			CEffect_Sakura::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_UI */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI"),
			CUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_BuffState*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BuffState"),
			CBuffState::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_PartTexture*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PartTexture"),
			CPartTexture::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_PartText*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PartText"),
			CPartText::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_HpBar*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HpBar"),
			CHpBar::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Searcher*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Searcher"),
			CSearcher::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_StageEnemyMonster*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StageEnemyMonster"),
			CStageEnemyMonster::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_StageSupportMonster*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StageSupportMonster"),
			CStageSupportMonster::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_DamageText*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DamageText"),
			CDamageText::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_BackGround */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
			CBackGround::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_SkillEffect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkillEffect"),
			CSkillEffect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_ChargeEffect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChargeEffect"),
			CChargeEffect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_AttackEffect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AttackEffect"),
			CAttackEffect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_RushAttackEffect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RushAttackEffect"),
			CRushAttackEffect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_BezierAttackEffect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BezierAttackEffect"),
			CBezierAttackEffect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_BumerangAttackEffect */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BumerangAttackEffect"),
			CBumerangAttackEffect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_SP_Denki_Zyuumanboruto */
		CHommingAttackEffect::HOMMING_ATTACK_EFFECT_DESC ZyuumanborutoDesc{};
		ZyuumanborutoDesc.m_bArriveHomeDead = false;
		ZyuumanborutoDesc.m_SmallRotationSpeed = XMConvertToRadians(60.f);
		ZyuumanborutoDesc.m_BigRotationSpeed = XMConvertToRadians(180.f);
		ZyuumanborutoDesc.m_BigRotationRadius = 2.f;
		ZyuumanborutoDesc.m_eHommingState = CHommingAttackEffect::HOMMING_OUT;
		ZyuumanborutoDesc.attackEffectDesc.effectDesc.m_bParentRotateApply = false;
		ZyuumanborutoDesc.attackEffectDesc.effectDesc.m_CurrentLoopCount = 7;
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SP_Denki_Zyuumanboruto"),
			CHommingAttackEffect::Create(m_pDevice, m_pContext, ZyuumanborutoDesc))))
			return E_FAIL;


		/* For.Prototype_GameObject_SP_Koori_Kogoerukaze */
		CRushAttackEffect::RUSH_ATTACK_EFFECT_DESC KogoerukazeDesc{};
		KogoerukazeDesc.attackEffectDesc.effectDesc.m_bParentRotateApply = false;
		KogoerukazeDesc.attackEffectDesc.effectDesc.m_CurrentLoopCount = 0;
		KogoerukazeDesc.attackEffectDesc.effectDesc.m_IsParts = false;
		KogoerukazeDesc.attackEffectDesc.effectDesc.m_AnimationSpeed = 0.5;
		KogoerukazeDesc.attackEffectDesc.effectDesc.m_AnimationStartAcc = 0.0;
		KogoerukazeDesc.attackEffectDesc.m_bContinue = true;
		KogoerukazeDesc.m_RushSpeed = 0.7;
		KogoerukazeDesc.attackEffectDesc.effectDesc.m_AnimationLoop = false;
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SP_Koori_Kogoerukaze"),
			CRushAttackEffect::Create(m_pDevice, m_pContext, KogoerukazeDesc))))
			return E_FAIL;


		/* For.Prototype_GameObject_SP_Mizu_Haidoroponpu */
		CRushAttackEffect::RUSH_ATTACK_EFFECT_DESC HaidoroponpuDesc{};
		HaidoroponpuDesc.attackEffectDesc.effectDesc.m_bParentRotateApply = false;
		HaidoroponpuDesc.attackEffectDesc.effectDesc.m_CurrentLoopCount = 1;
		HaidoroponpuDesc.attackEffectDesc.effectDesc.m_IsParts = false;
		HaidoroponpuDesc.attackEffectDesc.effectDesc.m_AnimationSpeed = 1.0;
		HaidoroponpuDesc.attackEffectDesc.effectDesc.m_AnimationStartAcc = 0.0;
		HaidoroponpuDesc.attackEffectDesc.m_bContinue = true;
		HaidoroponpuDesc.m_RushSpeed = 0.5;
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SP_Mizu_Haidoroponpu"),
			CRushAttackEffect::Create(m_pDevice, m_pContext, HaidoroponpuDesc))))
			return E_FAIL;

		/* For.Prototype_GameObject_SP_Koori_Reitoubiimu */
		CRushAttackEffect::RUSH_ATTACK_EFFECT_DESC ReitoubiimuDesc{};
		ReitoubiimuDesc.attackEffectDesc.effectDesc.m_bParentRotateApply = false;
		ReitoubiimuDesc.attackEffectDesc.effectDesc.m_CurrentLoopCount = 4;
		ReitoubiimuDesc.attackEffectDesc.effectDesc.m_IsParts = false;
		ReitoubiimuDesc.attackEffectDesc.effectDesc.m_AnimationSpeed = 1.0;
		ReitoubiimuDesc.attackEffectDesc.effectDesc.m_AnimationStartAcc = 0.0;
		ReitoubiimuDesc.attackEffectDesc.m_bContinue = true;
		ReitoubiimuDesc.m_RushSpeed = 1.0;
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SP_Koori_Reitoubiimu"),
			CRushAttackEffect::Create(m_pDevice, m_pContext, ReitoubiimuDesc))))
			return E_FAIL;

		/* For.Prototype_GameObject_SP_Jimen_Jisin */
		CAttackEffect::ATTACK_EFFECT_DESC JisinDesc{};
		JisinDesc.effectDesc.m_bParentRotateApply = false;
		JisinDesc.effectDesc.m_CurrentLoopCount = 0;
		JisinDesc.effectDesc.m_IsParts = true;
		JisinDesc.effectDesc.m_AnimationSpeed = 1.0;
		JisinDesc.effectDesc.m_AnimationStartAcc = 0.0;
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SP_Jimen_Jisin"),
			CAttackEffect::Create(m_pDevice, m_pContext, JisinDesc))))
			return E_FAIL;

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


	wsprintf(m_szLoadingText, TEXT("네비게이션을 로딩중."));
	///* For.Prototype_Component_Navigation */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Navigation"),
	//	CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/NavigationData.dat")))))
	//	return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, "../../Reference/Resources/Data/NavMask/BaseCamp/nav.json"))))
		return E_FAIL;

	wsprintf(m_szLoadingText, TEXT("pcharm 텍스쳐 로딩중입니다."));
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_BASECAMP))
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_pcharm_bingo"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_pcharm_bingo.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_pcharm_blank_ATK"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_pcharm_blank_ATK.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_pcharm_blank_ATKHP"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_pcharm_blank_ATKHP.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_pcharm_blank_HP"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_pcharm_blank_HP.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_pcharm_blank1"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_pcharm_blank1.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_pcharm_blank2"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_pcharm_blank2.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_pcharm_blank3"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_pcharm_blank3.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_pcharm_hibi"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_pcharm_hibi.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_pcharm_hibi_ATK"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_pcharm_hibi_ATK.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_pcharm_hibi_ATKHP"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_pcharm_hibi_ATKHP.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_pcharm_hibi_HP"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_pcharm_hibi_HP.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_pcharm_pipe_skill"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/window_pcharm_pipe_skill.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Pstone_attack"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pstone/UI_Pstone_attack_3_4.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Pstone_defense"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pstone/UI_Pstone_defense_3_4.dds")))))
			return E_FAIL;

	}


	/*  */
#pragma region TEXTURES
	wsprintf(m_szLoadingText, TEXT("포켓몬 상태 텍스쳐를 로딩중입니다."));
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_BASECAMP))
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ss_p_speedup"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_State/UI_ss_p_speedup.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ss_n_speeddown"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_State/UI_ss_n_speeddown.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ss_p_defenseup"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_State/UI_ss_p_defenseup.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ss_n_defensdown"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_State/UI_ss_n_defensdown.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ss_n_damagedown"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_State/UI_ss_n_damagedown.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ss_n_koori"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_State/UI_ss_n_koori.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ss_n_kooriL"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_State/UI_ss_n_kooriL.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ss_n_mahi"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_State/UI_ss_n_mahi.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ss_n_doku"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_State/UI_ss_n_doku.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ss_n_konran"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_State/UI_ss_n_konran.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Pokemon_Icon_M8"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_Icon/button_icon_M8.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Pokemon_Icon_M9"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_Icon/button_icon_M9.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Pokemon_Icon_M25"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_Icon/button_icon_M25.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Pokemon_Icon_M26"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_Icon/button_icon_M26.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Pokemon_Icon_M75"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_Icon/button_icon_M75.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Pokemon_Icon_M76"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_Icon/button_icon_M76.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Pokemon_Icon_M91"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_Icon/button_icon_M91.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Pokemon_Icon_M95"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Pokemon_Icon/button_icon_M95.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_window_power_pokemon"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/Window/Window_Pokemon_Power.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_button_skill_denki_borutekkaa"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/skill_Icon/button_skill_denki_borutekkaa.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_button_skill_denki_supaaku"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/skill_Icon/button_skill_denki_supaaku.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_button_skill_denki_zyuumanboruto"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/skill_Icon/button_skill_denki_zyuumanboruto.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_button_skill_esper_bariaa"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/skill_Icon/button_skill_esper_bariaa.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_button_skill_koori_reitoubiimu"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/skill_Icon/button_skill_koori_reitoubiimu.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_button_skill_iwa_iwaotosi"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/skill_Icon/button_skill_iwa_iwaotosi.dds")))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_button_skill_iwa_rokkukatto"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/skill_Icon/button_skill_iwa_rokkukatto.dds")))))
			return E_FAIL;

	}
	wsprintf(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Texture_BaseCamp_GoToWorldMap_Button"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/UI/BaseCamp_worldmapL_button.dds")))))
	//	return E_FAIL;


#pragma endregion

#pragma region MODELS
	wsprintf(m_szLoadingText, TEXT("모델를 로딩중입니다."));
	_matrix		PivotMatrix = XMMatrixIdentity();
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_BASECAMP))
	{
		/* For.Prototype_Component_VIBuffer_Terrain */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
			return E_FAIL;

		/* For.Prototype_Component_VIBuffer_FlatTerrain */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_FlatTerrain"),
			CVIBuffer_FlatTerrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		PivotMatrix = XMMatrixIdentity();
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Button_Base"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM_UI, "../../Reference/Resources/Mesh/Animation/UI/Button_Base.fbx", PivotMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxtexButton */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxtexButton"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/ShaderFiles/Shader_Vtxtex_Button.hlsl"), VTXUIANIMMODEL_DECLARATION::Elements, VTXUIANIMMODEL_DECLARATION::iNumElements))))
			return E_FAIL;

		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_BaseCamp_Stove"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/NonAnimation/Basecamp/BC_stove.fbx", PivotMatrix))))
			return E_FAIL;
	}

	/* For.Prototype_Component_Calculator */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Calculator"),
		CCalculator::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_BaseCamp_Field */
	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Model_BaseCamp_Field"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Basecamp/BC_field.fbx", PivotMatrix))))
		return E_FAIL;


	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));;
	//for (int i = 1; i <= 151; ++i)
	//{
	// 	_tchar* pokemonPrototype = new _tchar[MAX_PATH];
	//	char* pokemonFilePath = new char[MAX_PATH];

	//	wsprintf(pokemonPrototype, TEXT("Prototype_Component_Model_Pokemon_PM%d"), i);
	//	sprintf(pokemonFilePath, "../../Reference/Resources/Mesh/Animation/Pokemon/PM%d.fbx", i);

	//	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, pokemonPrototype,
	//		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_ANIM, pokemonFilePath, PivotMatrix, true))))
	//		return E_FAIL;
	//}

	//PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BASECAMP, TEXT("Prototype_Component_Model_Pokemon_PM6"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Pokemon/PM6.fbx", PivotMatrix, true))))
	//	return E_FAIL;


#pragma endregion

#pragma region SHADERS
	wsprintf(m_szLoadingText, TEXT("셰이더를 로딩중입니다."));

#pragma endregion


#pragma region GAMEOBJECTS
	wsprintf(m_szLoadingText, TEXT("객체원형을 로딩중."));

	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_BASECAMP))
	{
		/* For.Prototype_GameObject_GoToWorldMapButton */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GoToWorldMapButton"),
			CGoToWorldMapButton::Create(m_pDevice, m_pContext))))
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

		/* For.Prototype_GameObject_Weapon */
		//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon"),
		//	CWeapon::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;

		/* For.Prototype_GameObject_Map */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Map"),
			CMap::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Stove */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stove"),
			CStove::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_BaseCampMonster */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BaseCampMonster"),
			CBaseCampMonster::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_BaseCamp_Manager*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BaseCamp_Manager"),
			CBaseCamp_Manager::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_Cauldron*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cauldron"),
			CCauldron::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_GoToMonStateButton*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GoToMonStateButton"),
			CGoToMonStateButton::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_PokemonPowerInfoUI*/
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PokemonPowerInfoUI"),
			CPokemonPowerInfoUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PokemonInfoUI"),
			CPokemonInfoUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PokemonSkillStoneUI"),
			CPokemonSkillStoneUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkillInfoUI"),
			CSkillInfoUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StoneEquipInfoUI"),
			CStoneEquipInfoUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StoneInventory"),
			CStoneInventory::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stone"),
			CStone::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Food"),
			CFood::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GetItemShowUI"),
			CGetItemShowUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GoToBackLevelButton"),
			CGoToBackLevelButton::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GoToFeedingButton"),
			CGoToFeedingButton::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FoodInventory"),
			CFoodInventory::Create(m_pDevice, m_pContext))))
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

	wsprintf(m_szLoadingText, TEXT("네비게이션 데이터 로딩중."));
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, "../../Reference/Resources/Data/NavMask/WorldMap/nav.json"))))
		return E_FAIL;

	/*  */
#pragma region TEXTURES
	wsprintf(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* For.Prototype_Component_Texture_Stage_Icon */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Texture_Stage_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/World/world_dungeonicon%d.dds"), 5))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Texture_Window_Power_Enemy"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/World/window_power_enemy.dds")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Texture_Window_Clearstamp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Reference/Resources/Texture/World/window_clearstamp.dds")))))
		return E_FAIL;

#pragma endregion

#pragma region MODELS
	wsprintf(m_szLoadingText, TEXT("모델를 로딩중입니다."));

	/* For.Prototype_Component_Calculator */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Calculator"),
		CCalculator::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	_matrix		PivotMatrix = XMMatrixIdentity();

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Model_WorldMap_Special_Idle"),
		CModel::Create(m_pDevice, m_pContext, "../../Reference/Resources/Mesh/Animation/WorldMap/W_special1_idle.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Model_WorldMap_Water"),
		CModel::Create(m_pDevice, m_pContext, "../../Reference/Resources/Mesh/Animation/WorldMap/W_water.json", CModel::TYPE_MESH_COLOR_ANIM, PivotMatrix))))
		return E_FAIL;

	wsprintf(m_szLoadingText, TEXT("스테이지 포인트 모델를 로딩중입니다."));
	PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORLDMAP, TEXT("Prototype_Component_Model_Stage_Point_Standard"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/Animation/WorldMap/W_dpoint_stage_standard.fbx", PivotMatrix))))
		return E_FAIL;
	

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

#pragma endregion


#pragma region GAMEOBJECTS
	wsprintf(m_szLoadingText, TEXT("객체원형을 로딩중."));

	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_WORLDMAP))
	{
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WorldMap_Cloud"),
			CWorldMapCloud::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WorldMap_Flower"),
			CWorldMapFlower::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WorldMap_Grass"),
			CWorldMapGrass::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WorldMap_AnimEnv"),
			CWorldMapAnimEnv::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stage_Point"),
			CStagePoint::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WorldMap_Manager"),
			CWorldMap_Manager::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WorldMap_BackToIdle"),
			CWorldMapBackToIdel::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WorldMap_StageInfoUI"),
			CStageInfoUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WorldMap_GoToStageButton"),
			CGoToStageButton::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_GameObject_GoToBaseCampButton */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GoToBaseCampButton"),
			CGoToBaseCampButton::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}
#pragma endregion

	wsprintf(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLoader::Loading_ForStageLevel()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	wsprintf(m_szLoadingText, TEXT("네비게이션 데이터 로딩중."));
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, "../../Reference/Resources/Data/NavMask/Stage/C_water1_01.json"))))
		return E_FAIL;

	wsprintf(m_szLoadingText, TEXT("모델를 로딩중입니다."));

	// TODO 나중에 선택한 스테이지 마다 다르게 나와야함
	_matrix		PivotMatrix = XMMatrixIdentity();

	/* For.Prototype_Component_Calculator */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Calculator"),
		CCalculator::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_STAGE))
	{
		PivotMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_PokeringA"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Resident/PokeringA.fbx", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_PokeringB"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Resident/PokeringB.fbx", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_PokeringC"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_ANIM, "../../Reference/Resources/Mesh/Animation/Resident/PokeringC.fbx", PivotMatrix))))
			return E_FAIL;
	}

	wsprintf(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_STAGE))
	{

	}

	///* For.Prototype_Component_Model_Stage_LD */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_LD"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_out_LD.fbx", PivotMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Stage_LU */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_LU"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_out_LU.fbx", PivotMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Stage_RD */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_RD"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_out_RD.fbx", PivotMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Stage_RU */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE, TEXT("Prototype_Component_Model_Stage_RU"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_MESH_COLOR_NONANIM, "../../Reference/Resources/Mesh/Animation/Map/C_water1_out_RU.fbx", PivotMatrix))))
	//	return E_FAIL;

	wsprintf(m_szLoadingText, TEXT("객체원형을 로딩중."));
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_STAGE))
	{
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PokemonSkillButton"),
			CPokemonSkillButton::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StageCamera"),
			CStageCamera::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StageCameraTarget"),
			CStageCameraTarget::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stage_Manager"),
			CStage_Manager::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Pokering"),
			CPokering::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EnemySpawnPoint"),
			CEnemySpawnPoint::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EnemyPack"),
			CEnemyPack::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StageProgressUI"),
			CStageProgressUI::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}

	wsprintf(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForPokemonStateLevel()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 데이터 저장용으로 몬스터 생성시 안 넣으면 오류 출력이 되어서 어쩔수 없이 넣음*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_POKEMONSTATE, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, "../../Reference/Resources/Data/NavMask/BaseCamp/nav.json"))))
		return E_FAIL;

	wsprintf(m_szLoadingText, TEXT("텍스쳐 로딩중."));
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_POKEMONSTATE))
	{

	}

	wsprintf(m_szLoadingText, TEXT("객체 원형 로딩중."));
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_POKEMONSTATE))
	{
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PokemonState_Manager"),
			CPokemonState_Manager::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}

	Sleep(250);

	wsprintf(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForFeedingLevel()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 데이터 저장용으로 몬스터 생성시 안 넣으면 오류 출력이 되어서 어쩔수 없이 넣음*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FEEDING, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, "../../Reference/Resources/Data/NavMask/BaseCamp/nav.json"))))
		return E_FAIL;

	wsprintf(m_szLoadingText, TEXT("텍스쳐 로딩중."));
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_FEEDING))
	{
	}

	wsprintf(m_szLoadingText, TEXT("객체 원형 로딩중."));
	if (false == pGameInstance->Get_LevelFirstInit(LEVEL_FEEDING))
	{
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Feeding_Manager"),
			CFeeding_Manager::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}

	Sleep(250);

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
