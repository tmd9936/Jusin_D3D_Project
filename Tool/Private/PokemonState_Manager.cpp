#include "stdafx.h"
#include "PokemonState_Manager.h"

#include "GameInstance.h"

#include "Client_Utility.h"

#include "Monster.h"
#include "Player.h"
#include "StageSupportMonster.h"
#include "PokemonSkillStoneUI.h"
#include "SkillInfoUI.h"
#include "StoneEquipInfoUI.h"
#include "StoneInventory.h"
#include "PokemonInfoUI.h"

#include "Stone.h"

CPokemonState_Manager::CPokemonState_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{

}

CPokemonState_Manager::CPokemonState_Manager(const CPokemonState_Manager& rhs)
	: CUI(rhs)
{

}

HRESULT CPokemonState_Manager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPokemonState_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPokemonState_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (FAILED(Init_PickingStone()))
		return E_FAIL;

	if (FAILED(Init_PokemonInfoUI()))
		return E_FAIL;

	if (FAILED(Init_PokemonSkillStoneUI()))
		return E_FAIL;

	if (FAILED(Init_PokemonSkillInfoUI()))
		return E_FAIL;

	if (FAILED(Init_StoneEquipInfoUI()))
		return E_FAIL;

	if (FAILED(Init_StoneInventory()))
		return E_FAIL;

	return S_OK;
}

_uint CPokemonState_Manager::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);
	Change_State();

	return _uint();
}

_uint CPokemonState_Manager::LateTick(_double TimeDelta)
{
	return _uint();
}

HRESULT CPokemonState_Manager::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPokemonState_Manager::Init_NowMonster(const _uint& nowMonsterNumber)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	m_Desc.m_nowMonsterNumber = nowMonsterNumber;

	if (m_Desc.m_nowMonsterNumber == 1)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_POKEMONSTATE, L"Layer_Player", (CGameObject**)&m_pNowMonster, L"Player1", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster1.json", CLONE_FILEPATH)))
			return E_FAIL;
	}
	else if (m_Desc.m_nowMonsterNumber == 2)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_POKEMONSTATE, L"Layer_Player", L"Player1", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster1.json", CLONE_FILEPATH)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageSupportMonster"), LEVEL_POKEMONSTATE, L"Layer_Player", (CGameObject**)&m_pNowMonster, L"Player2", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster2.json", CLONE_FILEPATH)))
			return E_FAIL;
	}
	else if (m_Desc.m_nowMonsterNumber == 3)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_POKEMONSTATE, L"Layer_Player", L"Player1", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster1.json", CLONE_FILEPATH)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StageSupportMonster"), LEVEL_POKEMONSTATE, L"Layer_Player", (CGameObject**)&m_pNowMonster, L"Player3", "../../Reference/Resources/Data/Database/NowMonster/NowPartyMonster3.json", CLONE_FILEPATH)))
			return E_FAIL;
	}

	pGameInstance->Layer_Tick_State_Change(L"Layer_Player", LEVEL_POKEMONSTATE, false);

	return S_OK;
}

HRESULT CPokemonState_Manager::Init_PickingStone()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CStone::STONE_DESC stoneDesc{};

	stoneDesc.m_eCurState = CStone::STATE_PICKING_FOLLOW_MOUSE;
	stoneDesc.m_stoneType = CStone::TYPE_ATK;
	stoneDesc.value = 100;
	stoneDesc.m_pokemonIconNumber = 25;

	stoneDesc.m_UIDesc.m_fSizeX = 45.f;
	stoneDesc.m_UIDesc.m_fSizeY = 45.f;
	stoneDesc.m_UIDesc.m_fX = 0.f;
	stoneDesc.m_UIDesc.m_fY = 0.f;
	stoneDesc.m_UIDesc.m_ShaderPass = 0;
	stoneDesc.m_UIDesc.m_TextureProtoTypeLevel = 0;
	stoneDesc.m_UIDesc.m_UIType = 0;
	lstrcpy(stoneDesc.m_UIDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_UI_Pstone_attack");

	pGameInstance->Clone_GameObject(L"Layer_UI", m_iLevelindex, TEXT("Prototype_GameObject_Stone"), (CGameObject**)&m_pPickingInfoStone, &stoneDesc);
	if (nullptr == m_pPickingInfoStone)
		return false;

	m_pPickingInfoStone->Set_RenderId(RENDER_END);

	return S_OK;
}

HRESULT CPokemonState_Manager::Init_PokemonInfoUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_POKEMONSTATE, L"Layer_UI", L"PokemonInfoUI");
	if (nullptr == pObject)
		return E_FAIL;

	m_pPokemonInfoUI = dynamic_cast<CPokemonInfoUI*>(pObject);
	if (nullptr == m_pPokemonInfoUI)
		return E_FAIL;

	Safe_AddRef(m_pPokemonInfoUI);

	return S_OK;
}

HRESULT CPokemonState_Manager::Init_PokemonSkillStoneUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_POKEMONSTATE, L"Layer_UI", L"PokemonSkillStoneUI");
	if (nullptr == pObject)
		return E_FAIL;

	m_pPokemonSkillStoneUI = dynamic_cast<CPokemonSkillStoneUI*>(pObject);
	if (nullptr == m_pPokemonSkillStoneUI)
		return E_FAIL;

	Safe_AddRef(m_pPokemonSkillStoneUI);

	return S_OK;
}

HRESULT CPokemonState_Manager::Init_PokemonSkillInfoUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject1 = pGameInstance->Get_Object(LEVEL_POKEMONSTATE, L"Layer_UI", L"SkillInfoUI01");
	if (nullptr == pObject1)
		return E_FAIL;

	m_pSkillInfoUI01 = dynamic_cast<CSkillInfoUI*>(pObject1);
	if (nullptr == m_pSkillInfoUI01)
		return E_FAIL;
	Safe_AddRef(m_pSkillInfoUI01);

	CGameObject* pObject2 = pGameInstance->Get_Object(LEVEL_POKEMONSTATE, L"Layer_UI", L"SkillInfoUI02");
	if (nullptr == pObject2)
		return E_FAIL;

	m_pSkillInfoUI02 = dynamic_cast<CSkillInfoUI*>(pObject2);
	if (nullptr == m_pSkillInfoUI02)
		return E_FAIL;
	Safe_AddRef(m_pSkillInfoUI02);

	return S_OK;
}

HRESULT CPokemonState_Manager::Init_StoneEquipInfoUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_POKEMONSTATE, L"Layer_UI", L"StoneEquipInfoUI");
	if (nullptr == pObject)
		return E_FAIL;

	m_pStoneEquipInfoUI = dynamic_cast<CStoneEquipInfoUI*>(pObject);
	if (nullptr == m_pStoneEquipInfoUI)
		return E_FAIL;
	Safe_AddRef(m_pStoneEquipInfoUI);

	return S_OK;
}

HRESULT CPokemonState_Manager::Init_StoneInventory()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_POKEMONSTATE, L"Layer_UI", L"StoneInvetory");
	if (nullptr == pObject)
		return E_FAIL;

	m_pStoneInventory = dynamic_cast<CStoneInventory*>(pObject);
	if (nullptr == m_pStoneInventory)
		return E_FAIL;
	Safe_AddRef(m_pStoneInventory);

	return S_OK;
}

void CPokemonState_Manager::State_Tick(const _double& TimeDelta)
{
	switch (m_eCurState)
	{
	case MANAGER_IDLE:
		Picking();
		break;
	case MANAGER_PIKING_STONE:
		ShowPickingStone();
		break;
	}

}

void CPokemonState_Manager::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case MANAGER_IDLE:
			break;
		case MANAGER_PIKING_STONE:
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CPokemonState_Manager::Picking()
{
	if (MOUSE_TAB(MOUSE::LBTN) && CClient_Utility::Mouse_Pos_In_Platform() && m_eCurState == MANAGER_IDLE)
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		if (m_pStoneInventory->Check_Is_In(pt))
		{
			// 마우스가 인벤토리 안에 있는지 확인
			// 존재하는 스톤칸에 있는지 확인
			// 맞으면 피킹상태로 변환
			// 스톤이랑 마우스 따라다니는 텍스쳐 만들기
			// 텍스쳐 바꾸기

			CStone::STONE_DESC stoneDesc{};
			if (m_pStoneInventory->Check_Exist_Stone_Is_In(stoneDesc, pt))
			{
				m_pPickingInfoStone->Change_StoneType(stoneDesc.m_stoneType);
				m_pPickingInfoStone->Change_Value(to_wstring(stoneDesc.value));
				m_pPickingInfoStone->Set_RenderId(RENDER_UI);
				m_eCurState = MANAGER_PIKING_STONE;

				// 픽킹된곳 색상변경
			}
		}
	}
}

void CPokemonState_Manager::ShowPickingStone()
{
	if (MOUSE_HOLD(MOUSE::LBTN))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		CTransform* pTransform = m_pPickingInfoStone->Get_As<CTransform>();
		pTransform->Set_PositinoX((_float)pt.x);
		pTransform->Set_PositionY((_float)pt.y);
	}
	else if (MOUSE_AWAY(MOUSE::LBTN))
	{
		// 인벤토리안에 비어있는 다른 칸 인지 확인 (좌표 보내주고 확인)
		// 스톤 장착칸인지?

		m_pPickingInfoStone->Set_RenderId(RENDER_END);
		m_eCurState = MANAGER_IDLE;
	}
}

HRESULT CPokemonState_Manager::SetUp_ShaderResources()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CPokemonState_Manager* CPokemonState_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPokemonState_Manager* pInstance = new CPokemonState_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CPokemonState_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokemonState_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CPokemonState_Manager* pInstance = new CPokemonState_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CPokemonState_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPokemonState_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CPokemonState_Manager* pInstance = new CPokemonState_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CPokemonState_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPokemonState_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pPickingInfoStone);

	Safe_Release(m_pNowMonster);
	Safe_Release(m_pPokemonSkillStoneUI);
	Safe_Release(m_pSkillInfoUI01);
	Safe_Release(m_pSkillInfoUI02);
	Safe_Release(m_pStoneEquipInfoUI);
	Safe_Release(m_pStoneInventory);
	Safe_Release(m_pPokemonInfoUI);
}
