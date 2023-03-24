#include "stdafx.h"
#include "Level_WorldMap.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "Utility.h"

#include "Button.h"
#include "WorldMapCloud.h"

#include "WorldMapAnimEnv.h"

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

	//if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Map(TEXT("Layer_Map"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_WorldMap::Tick(_double TimeDelta)
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이레벨임"));
#endif
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

	//CCamera_Dynamic::CAMERA_DYNAMIC_DESC		CameraDynamicDesc;
	//ZeroMemory(&CameraDynamicDesc, sizeof CameraDynamicDesc);

	//if (FAILED(CUtility::Load_Args_Data(L"../../Reference/Resources/Data/Scene/GamePlay/Main_Camera.dat", &CameraDynamicDesc, sizeof CCamera_Dynamic::CAMERA_DYNAMIC_DESC)))
	//	return E_FAIL;

	//CameraDynamicDesc.CameraDesc.vEye = _float4(0.f, 10.f, -7.f, 1.f);
	//CameraDynamicDesc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	//CameraDynamicDesc.CameraDesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.f);

	//CameraDynamicDesc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	//CameraDynamicDesc.CameraDesc.fAspect = _float(g_iWinSizeX) / g_iWinSizeY;
	//CameraDynamicDesc.CameraDesc.fNear = 0.2f;
	//CameraDynamicDesc.CameraDesc.fFar = 300;

	//CameraDynamicDesc.CameraDesc.TransformDesc.SpeedPerSec = 20.f;
	//CameraDynamicDesc.CameraDesc.TransformDesc.RotationPerSec = XMConvertToRadians(180.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_WORLDMAP, pLayerTag, L"Main_Camera", "../../Reference/Resources/Data/Scene/WorldMap/Camera_data.json", CLONE_FILEPATH)))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Public"), LEVEL_WORLDMAP, pLayerTag, L"Main_Camera", "../../Reference/Resources/Data/Scene/WorldMap/Camera_Public_data.json", CLONE_FILEPATH)))
		return E_FAIL;*/

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
	seawave_desc.vPos = { 19.1f, 0.25, 21.f };
	lstrcpy(seawave_desc.ModelPrototypeTag, L"Prototype_Component_Model_WorldMap_Seawave");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_AnimEnv"), LEVEL_WORLDMAP, pLayerTag, L"Seawave", &seawave_desc)))
		return E_FAIL;

	CWorldMapAnimEnv::WORLDMAP_ANIM_ENV_DESC ship_desc{};
	ship_desc.vPos = { 19.1f, 0.25, 21.f };
	lstrcpy(ship_desc.ModelPrototypeTag, L"Prototype_Component_Model_WorldMap_Ship");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_AnimEnv"), LEVEL_WORLDMAP, pLayerTag, L"Ship", &ship_desc)))
		return E_FAIL;

	CWorldMapAnimEnv::WORLDMAP_ANIM_ENV_DESC special_desc{};
	special_desc.vPos = { 19.1f, 0.25, 21.f };
	lstrcpy(special_desc.ModelPrototypeTag, L"Prototype_Component_Model_WorldMap_Special_Idle");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldMap_AnimEnv"), LEVEL_WORLDMAP, pLayerTag, L"Special", &special_desc)))
		return E_FAIL;

	CWorldMapAnimEnv::WORLDMAP_ANIM_ENV_DESC water_desc{};
	water_desc.vPos = { 19.f, 0.25, 21.1f };
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

HRESULT CLevel_WorldMap::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_WORLDMAP, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_WorldMap::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_WORLDMAP, pLayerTag)))
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
