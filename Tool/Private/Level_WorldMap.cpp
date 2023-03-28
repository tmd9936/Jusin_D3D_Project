#include "stdafx.h"
#include "Level_WorldMap.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "Utility.h"

#include "Button.h"
#include "WorldMapCloud.h"

#include "WorldMapAnimEnv.h"

#include "StagePoint.h"

CLevel_WorldMap::CLevel_WorldMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_WorldMap::Initialize()
{
	if (FAILED(Ready_LightDesc()))
		return E_FAIL;


	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Env(TEXT("Layer_Env"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Map(TEXT("Layer_Map"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Stage_Point(TEXT("Layer_Stage_Point"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Manager(TEXT("Layer_Manager"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_PlayerSkill(TEXT("Layer_PlayerSkill"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_State_Info_UI(TEXT("Layer_Stage_Info_UI"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_WorldMap::Tick(_double TimeDelta)
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("����� ������"));
#endif
}

HRESULT CLevel_WorldMap::Ready_Layer_Manager(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_Manager"), LEVEL_WORLDMAP, pLayerTag, L"WorldMap_Manager", "../../Reference/Resources/Data/Scene/WorldMap/WorldMap_Manager.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_WorldMap::Ready_Layer_Terrain(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FlatTerrain"), LEVEL_WORLDMAP, pLayerTag, L"Terrain")))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_WorldMap::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, pLayerTag)))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_WORLDMAP, pLayerTag, L"Main_Camera", "../../Reference/Resources/Data/Scene/WorldMap/Camera_data.json", CLONE_FILEPATH)))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Public"), LEVEL_WORLDMAP, pLayerTag, L"Main_Camera", "../../Reference/Resources/Data/Scene/WorldMap/Camera_Public_data.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_WorldMap::Ready_Layer_Env(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_Cloud"), LEVEL_WORLDMAP, pLayerTag, L"Cloud")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_Flower"), LEVEL_WORLDMAP, pLayerTag, L"Flower")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_Grass"), LEVEL_WORLDMAP, pLayerTag, L"Grass")))
		return E_FAIL;

	CWorldMapAnimEnv::WORLDMAP_ANIM_ENV_DESC seawave_desc{};
	seawave_desc.vPos = { 19.0f, 0.25, 21.f };
	lstrcpy(seawave_desc.ModelPrototypeTag, L"Prototype_Component_Model_WorldMap_Seawave");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_AnimEnv"), LEVEL_WORLDMAP, pLayerTag, L"Seawave", &seawave_desc)))
		return E_FAIL;

	CWorldMapAnimEnv::WORLDMAP_ANIM_ENV_DESC ship_desc{};
	ship_desc.vPos = { 19.0f, 0.25, 21.f };
	lstrcpy(ship_desc.ModelPrototypeTag, L"Prototype_Component_Model_WorldMap_Ship");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_AnimEnv"), LEVEL_WORLDMAP, pLayerTag, L"Ship", &ship_desc)))
		return E_FAIL;

	CWorldMapAnimEnv::WORLDMAP_ANIM_ENV_DESC special_desc{};
	special_desc.vPos = { 19.0f, 0.25, 21.f };
	lstrcpy(special_desc.ModelPrototypeTag, L"Prototype_Component_Model_WorldMap_Special_Idle");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_AnimEnv"), LEVEL_WORLDMAP, pLayerTag, L"Special", &special_desc)))
		return E_FAIL;

	CWorldMapAnimEnv::WORLDMAP_ANIM_ENV_DESC water_desc{};
	water_desc.vPos = { 18.9f, 0.25, 21.1f };
	lstrcpy(water_desc.ModelPrototypeTag, L"Prototype_Component_Model_WorldMap_Water");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_AnimEnv"), LEVEL_WORLDMAP, pLayerTag, L"Water", &water_desc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_WorldMap::Ready_Layer_Map(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, pLayerTag)))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_WORLDMAP, pLayerTag)))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_WORLDMAP, pLayerTag, L"Map", "../../Reference/Resources/Data/Scene/WorldMap/Map_data.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_WorldMap::Ready_Layer_Stage_Point(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Stage_Point"), LEVEL_WORLDMAP, pLayerTag, L"Stage_Point1", "../../Reference/Resources/Data/Scene/WorldMap/StagePoint/StagePoint1.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Stage_Point"), LEVEL_WORLDMAP, pLayerTag, L"Stage_Point2", "../../Reference/Resources/Data/Scene/WorldMap/StagePoint/StagePoint2.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Stage_Point"), LEVEL_WORLDMAP, pLayerTag, L"Stage_Point3", "../../Reference/Resources/Data/Scene/WorldMap/StagePoint/StagePoint3.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Stage_Point"), LEVEL_WORLDMAP, pLayerTag, L"Stage_Point4", "../../Reference/Resources/Data/Scene/WorldMap/StagePoint/StagePoint4.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_WorldMap::Ready_Layer_Effect(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_WorldMap::Ready_Layer_PlayerSkill(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_WorldMap::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, L"Layer_BuffState")))
		return E_FAIL;

	CMonster::POKEMON_DESC desc{};
	desc.m_monsterNo = 6;
	desc.vPos = _float4(25.2f, 0.5f, 22.0f, 1.f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_WORLDMAP, pLayerTag, L"Player", &desc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_WorldMap::Ready_Layer_State_Info_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_BackToIdle"), LEVEL_WORLDMAP, pLayerTag, L"BackToIdle", "../../Reference/Resources/Data/Scene/WorldMap/Button/WorldMapBackToIdle.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_StageInfoUI"), LEVEL_WORLDMAP, pLayerTag, L"StageInfo", "../../Reference/Resources/Data/Scene/WorldMap/StageInfoUI/StageInfoUI.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_WorldMap::Ready_LightDesc()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_WorldMap* CLevel_WorldMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_WorldMap* pInstance = new CLevel_WorldMap(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CLevel_WorldMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_WorldMap::Free()
{
	__super::Free();

}
