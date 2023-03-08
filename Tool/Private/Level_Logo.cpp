#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "Level_Loading.h"
#include "GameInstance.h"
#include "UI.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Logo::Initialize()
{
	CGameInstance::GetInstance()->PlayBGM(TEXT("BGM_BASE.ogg"));
	/* 검색시에 어떤 레벨에 있는 특정 태그에 있는 몇번째 녀석. */
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Logo::Tick(_double TimeDelta)
{
	/* 로고레벨다음은 게임플레이야. */
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;

		Safe_Release(pGameInstance);
	}

#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("로고레벨임"));
#endif

}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_LOGO, pLayerTag)))
		return E_FAIL;

	CUI::UI_DESC desc = {};
	desc.m_fSizeX = g_iWinSizeX;
	desc.m_fSizeY = g_iWinSizeY;
	desc.m_fX = g_iWinSizeX >> 1;
	desc.m_fY = g_iWinSizeY >> 1;
	lstrcpy(desc.m_TextureProtoTypeName, L"Test");

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_LOGO, pLayerTag, nullptr, &desc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_UI(const _tchar* pLayerTag)
{
	return S_OK;
}

CLevel_Logo* CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();

}
