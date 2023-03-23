#include "stdafx.h"
#include "..\Public\Level_BaseCamp.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "Utility.h"

#include "Button.h"

#include "Client_Utility.h"

CLevel_BaseCamp::CLevel_BaseCamp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_BaseCamp::Initialize()
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

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Manager(TEXT("Layer_Manager"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_PlayerSkill(TEXT("Layer_PlayerSkill"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->Add_Check_CollisionGroup(L"Layer_Player", L"Layer_Monster")))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->Add_Check_CollisionGroup(L"Layer_Player", L"Layer_Env")))
		return E_FAIL;

	CClient_Utility::Load_Layer_GameObjects("../../Reference/Resources/Data/Scene/BaseCamp/Stove.json");

	return S_OK;
}

void CLevel_BaseCamp::Tick(_double TimeDelta)
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이레벨임"));
#endif
}

HRESULT CLevel_BaseCamp::Ready_Layer_Terrain(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FlatTerrain"), LEVEL_BASECAMP, pLayerTag, L"Terrain")))
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

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_BASECAMP, pLayerTag, L"Main_Camera", "../../Reference/Resources/Data/Scene/BaseCamp/Camera_data.json", CLONE_FILEPATH)))
	//	return E_FAIL;

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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_BASECAMP, pLayerTag)))
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

	//CButton::BUTTON_DESC desc = {};
	//desc.m_fSizeX = 80.5f;
	//desc.m_fSizeY = 30.f;
	//desc.m_fX = g_iWinSizeX - 100.f;
	//desc.m_fY = g_iWinSizeY - 25.f;
	//desc.m_ShaderLevelIndex = LEVEL_BASECAMP;
	//desc.m_ModelPrototypLevel = LEVEL_BASECAMP;

	//strcpy(desc.m_DiffuseTexturePath, "./BaseCamp_worldmapL_button.png");
	//lstrcpy(desc.m_MaskPrototypeTag, TEXT("Prototype_Component_Button_Color_Mask"));
	//lstrcpy(desc.m_ButtonName, TEXT("GO_TO_WORLDMAP_BUTTON"));

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GoToWorldMapButton"), LEVEL_BASECAMP, pLayerTag, desc.m_ButtonName, &desc)))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GoToWorldMapButton"), LEVEL_BASECAMP, pLayerTag, L"GoToWorldMapButton", "../../Reference/Resources/Data/Scene/BaseCamp/Button/GoToMap_Button_data.json", CLONE_FILEPATH)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_Monster(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	//for (size_t i = 0; i < 5; i++)
	//{
	//	char protoTag[MAX_PATH];
	//	sprintf(protoTag, "../../Reference/Resources/Data/Scene/BaseCamp/FieldPokemon/FieldPokemon_data%d.json", i);

	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseCampMonster"), LEVEL_BASECAMP, pLayerTag, nullptr, protoTag, CLONE_FILEPATH)))
	//		return E_FAIL;
	//}

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseCampMonster"), LEVEL_BASECAMP, pLayerTag, nullptr, "../../Reference/Resources/Data/Scene/BaseCamp/FieldPokemon/FieldPokemon_data0.json", CLONE_FILEPATH)))
		return E_FAIL;


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

HRESULT CLevel_BaseCamp::Ready_LightDesc()
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
	//CGameInstance::GetInstance()->Reset_CollisionGroup();
	__super::Free();
}
