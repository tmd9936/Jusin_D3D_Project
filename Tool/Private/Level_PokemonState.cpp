#include "stdafx.h"
#include "Level_PokemonState.h"

#include "Level_Loading.h"
#include "GameInstance.h"

#include "UI.h"


// ���ϸ� ��ư ������ �̰� ��ȣ �����Ű��
_uint CLevel_PokemonState::m_PokemonNumber = 1;

CLevel_PokemonState::CLevel_PokemonState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_PokemonState::Initialize()
{
	g_BackBufferColor = m_PokemonStateLevelBackColor;

	// ���ϸ� ���� json ���Ͽ� ������ ���ϸ��� ��ȣ �ٲٱ�

	CGameInstance::GetInstance()->PlayBGM(TEXT("BGM_BASE.ogg"));

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_PokemonState::Tick(_double TimeDelta)
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("���ϸ� ���� ����"));
#endif

}

void CLevel_PokemonState::Set_PokemonNumber(const _uint& number)
{
	m_PokemonNumber = number;
}

HRESULT CLevel_PokemonState::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_POKEMONSTATE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BackGround"), LEVEL_POKEMONSTATE, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_PokemonState::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_POKEMONSTATE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ModelUI"), LEVEL_POKEMONSTATE, pLayerTag)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_PokemonState* CLevel_PokemonState::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_PokemonState* pInstance = new CLevel_PokemonState(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CLevel_PokemonState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_PokemonState::Free()
{
	__super::Free();

}
