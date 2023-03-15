#include "stdafx.h"
#include "..\Public\Level_BaseCamp.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "Utility.h"

#include "Button.h"

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

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Map"), LEVEL_BASECAMP, pLayerTag)))
	//	return E_FAIL;

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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_BaseCamp::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
	//	return E_FAIL;

	//CModelUI::UI_DESC desc = {};
	//desc.m_fSizeX = g_iWinSizeX >> 1;
	//desc.m_fSizeY = g_iWinSizeY >> 1;
	//desc.m_fX = g_iWinSizeX >> 1;
	//desc.m_fY = g_iWinSizeY >> 1;
	//desc.m_eLevel = LEVEL_LOADING;

	//lstrcpy(desc.m_TextureProtoTypeName, TEXT("Prototype_Component_Model_Loading_Scene"));

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ModelUI"), LEVEL_BASECAMP, pLayerTag, nullptr, &desc)))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_BASECAMP, pLayerTag)))
		return E_FAIL;

	CButton::BUTTON_DESC desc = {};
	desc.m_fSizeX = g_iWinSizeX >> 2;
	desc.m_fSizeY = g_iWinSizeY >> 2;
	desc.m_fX = g_iWinSizeX >> 1;
	desc.m_fY = g_iWinSizeY >> 1;
	desc.m_ShaderLevelIndex = LEVEL_BASECAMP;

	strcpy(desc.m_DiffuseTextureName, "window_button_pokemon_blue.png");
	lstrcpy(desc.m_MaskPrototype, TEXT(""));

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ModelUI"), LEVEL_BASECAMP, pLayerTag, nullptr, &desc)))
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
	__super::Free();

}
