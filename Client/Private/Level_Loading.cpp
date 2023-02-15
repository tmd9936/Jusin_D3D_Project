#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#pragma region ENGINE_HEADERS
#include "GameInstance.h"
#pragma endregion

#pragma region CLIENT_HEADERS
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#pragma endregion

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	/* �δ��ȿ��� �����带 �����Ѵ�. */
	/* ������ �����尡 �ʿ��� �غ� �Ѵ�.  */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(_double TimeDelta)
{
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (true == m_pLoader->Get_Finished())
		{
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);

			CLevel* pNewLevel = { nullptr };

			switch (m_eNextLevelID)
			{
			case LEVEL_LOGO:
				pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;

			case LEVEL_GAMEPLAY:
				pNewLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;

			}

			if (nullptr == pNewLevel)
			{
				Safe_Release(pGameInstance);
				return;
			}

			if (FAILED(pGameInstance->Open_Level(m_eNextLevelID, pNewLevel)))
				return;

			Safe_Release(pGameInstance);
		}
	}

#ifdef _DEBUG
	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());
#endif


}

HRESULT CLevel_Loading::Ready_Layer_BackGround()
{
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_UI()
{
	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);

}
