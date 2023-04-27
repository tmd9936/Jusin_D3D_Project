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

	if (m_pPickingInfoStone)
		m_pPickingInfoStone->Tick(TimeDelta);

	return _uint();
}

_uint CPokemonState_Manager::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	if (m_pPickingInfoStone)
		m_pPickingInfoStone->LateTick(TimeDelta);

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
	case MANAGER_INVENTORY_STONE_PICKING:
		Inventory_Stone_Picking_Tick();
		break;
	case MANAGER_EQUIPINFO_STONE_PICKING:
		Equip_Info_Stone_Picking_Tick();
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
			m_pPickingInfoStone->Set_State(CStone::STATE_NO_SHOW);
			m_eRenderId = RENDER_END;
			break;
		case MANAGER_INVENTORY_STONE_PICKING:
			m_eRenderId = RENDER_UI;
			break;
		case MANAGER_EQUIPINFO_STONE_PICKING:
			m_eRenderId = RENDER_UI;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CPokemonState_Manager::Picking()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_pPickingInfoStone->Set_Pos({ (_float)pt.x - g_iWinSizeX * 0.5f, -(_float)pt.y - 20.f + g_iWinSizeY * 0.5f, 0.f });
	Set_Pos({ (_float)pt.x - g_iWinSizeX * 0.5f, -(_float)pt.y - 20.f + g_iWinSizeY * 0.5f, 0.f });

	if (MOUSE_TAB(MOUSE::LBTN) && CClient_Utility::Mouse_Pos_In_Platform() && m_eCurState == MANAGER_IDLE)
	{
		if (m_pStoneInventory->Check_Is_In(pt))
		{
			CStone::STONE_DESC stoneDesc{};
			
			if (m_pStoneInventory->Check_Exist_Stone_Is_In(stoneDesc, m_pickingStoneIndex, pt))
			{
				m_pPickingInfoStone->Change_StoneType(stoneDesc.m_stoneType);
				m_pPickingInfoStone->Change_Value(to_wstring(stoneDesc.value));
				m_pPickingInfoStone->Set_State(CStone::STATE_PICKING_FOLLOW_MOUSE);
				m_pPickingInfoStone->Set_InventoryIndex(stoneDesc.m_inventoyIndex);
				m_eCurState = MANAGER_INVENTORY_STONE_PICKING;
			}
		}
		else if (m_pStoneEquipInfoUI->Check_Is_In(pt))
		{
			CStone::STONE_DESC stoneDesc{};
			if (m_pStoneEquipInfoUI->Check_Exist_Stone_Is_In(pt, m_pickingStoneIndex, stoneDesc))
			{
				m_pPickingInfoStone->Change_StoneType(stoneDesc.m_stoneType);
				m_pPickingInfoStone->Change_Value(to_wstring(stoneDesc.value));
				m_pPickingInfoStone->Set_State(CStone::STATE_PICKING_FOLLOW_MOUSE);
				m_pPickingInfoStone->Set_InventoryIndex(stoneDesc.m_inventoyIndex);
				m_eCurState = MANAGER_EQUIPINFO_STONE_PICKING;
			}
		}
	}
}

void CPokemonState_Manager::Inventory_Stone_Picking_Tick()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (MOUSE_HOLD(MOUSE::LBTN))
	{
		m_pPickingInfoStone->Set_Pos({ (_float)pt.x- g_iWinSizeX * 0.5f, -(_float)pt.y - 20.f + g_iWinSizeY * 0.5f, 0.f });
		Set_Pos({ (_float)pt.x - g_iWinSizeX * 0.5f, -(_float)pt.y - 20.f + g_iWinSizeY * 0.5f, 0.f });
	}
	else if (MOUSE_AWAY(MOUSE::LBTN))
	{
		if (m_pStoneInventory->Check_Is_In(pt))
		{
			m_pStoneInventory->Change_StoneIndex(m_pickingStoneIndex, pt);
		}
		else if (m_pStoneEquipInfoUI->Check_Is_In(pt))
		{
			CStone::STONE_DESC pickingStoneDesc = m_pPickingInfoStone->Get_StoneDesc();

			UnEquip(pt);

			if (m_pStoneEquipInfoUI->Equip(pt, pickingStoneDesc))
			{
				_uint pokemonNo = m_pPokemonInfoUI->Get_PokemonNo();
				m_pStoneInventory->Change_StoneState_To_Equip(m_pickingStoneIndex, pokemonNo);
				if (pickingStoneDesc.m_stoneType == CStone::TYPE_ATK)
				{
					m_pPokemonInfoUI->Add_ATK(pickingStoneDesc.value);
				}
				else if (pickingStoneDesc.m_stoneType == CStone::TYPE_HP)
				{
					m_pPokemonInfoUI->Add_HP(pickingStoneDesc.value);
				}
			}
		}

		m_pPickingInfoStone->Set_State(CStone::STATE_NO_SHOW);
		m_eCurState = MANAGER_IDLE;
	}
}

void CPokemonState_Manager::Equip_Info_Stone_Picking_Tick()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	if (MOUSE_HOLD(MOUSE::LBTN))
	{
		m_pPickingInfoStone->Set_Pos({ (_float)pt.x - g_iWinSizeX * 0.5f, -(_float)pt.y - 20.f + g_iWinSizeY * 0.5f, 0.f });
		Set_Pos({ (_float)pt.x - g_iWinSizeX * 0.5f, -(_float)pt.y - 20.f + g_iWinSizeY * 0.5f, 0.f });
	}
	else if (MOUSE_AWAY(MOUSE::LBTN))
	{
		if (m_pStoneInventory->Check_Is_In(pt))
		{
			CStone::STONE_DESC pickingStoneDesc = m_pPickingInfoStone->Get_StoneDesc();

			m_pStoneInventory->Change_StoneState_To_UnEquip(pickingStoneDesc.m_inventoyIndex);
			if (pickingStoneDesc.m_stoneType == CStone::TYPE_ATK)
			{
				m_pPokemonInfoUI->Add_ATK(-(_int)pickingStoneDesc.value);
			}
			else if (pickingStoneDesc.m_stoneType == CStone::TYPE_HP)
			{
				m_pPokemonInfoUI->Add_HP(-(_int)pickingStoneDesc.value);
			}
		}
		else if (m_pStoneEquipInfoUI->Check_Is_In(pt))
		{
			CStone::STONE_DESC pickingStoneDesc = m_pPickingInfoStone->Get_StoneDesc();

			m_pStoneEquipInfoUI->Change_StoneIndex(pt, m_pickingStoneIndex, pickingStoneDesc);
		}

		m_pPickingInfoStone->Set_State(CStone::STATE_NO_SHOW);
		m_eCurState = MANAGER_IDLE;
	}
}

_bool CPokemonState_Manager::UnEquip(const POINT& pt)
{
	CStone::STONE_DESC outUnEqiupstoneDesc{};
	if (m_pStoneEquipInfoUI->UnEquip(pt, outUnEqiupstoneDesc))
	{
		m_pStoneInventory->Change_StoneState_To_UnEquip(outUnEqiupstoneDesc.m_inventoyIndex);
		if (outUnEqiupstoneDesc.m_stoneType == CStone::TYPE_ATK)
		{
			m_pPokemonInfoUI->Add_ATK(-(_int)outUnEqiupstoneDesc.value);
		}
		else if (outUnEqiupstoneDesc.m_stoneType == CStone::TYPE_HP)
		{
			m_pPokemonInfoUI->Add_HP(-(_int)outUnEqiupstoneDesc.value);
		}

		return true;
	}

	return false;
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
