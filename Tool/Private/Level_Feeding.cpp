#include "stdafx.h"
#include "Level_Feeding.h"
#include "GameInstance.h"

#include "UI.h"

CLevel_Feeding::CLevel_Feeding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Feeding::Initialize()
{
	g_BackBufferColor = m_FeedingBackBufferColor;

	CGameInstance::GetInstance()->PlayBGM(TEXT("BGM_BASE.ogg"));

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
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

	if (FAILED(pGameInstance->Add_Layer(LEVEL_FEEDING, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GoToBackLevelButton"), LEVEL_FEEDING, pLayerTag, L"GoToBackLevelButton", "../../Reference/Resources/Data/Scene/Feeding/Button/GoToBackLevelButton.json", CLONE_FILEPATH)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_FEEDING, pLayerTag, L"LevelNameInfo", "../../Reference/Resources/Data/Scene/Feeding/UI/LevelNameInfo.json", CLONE_FILEPATH)))
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
