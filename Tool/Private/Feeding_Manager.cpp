#include "stdafx.h"
#include "Feeding_Manager.h"

#include "GameInstance.h"

#include "Client_Utility.h"

#include "Monster.h"
#include "Player.h"
#include "StageSupportMonster.h"
#include "PokemonInfoUI.h"
#include "FoodInventory.h"


CFeeding_Manager::CFeeding_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{

}

CFeeding_Manager::CFeeding_Manager(const CFeeding_Manager& rhs)
	: CUI(rhs)
{

}

HRESULT CFeeding_Manager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFeeding_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Init_PickingFood()))
		return E_FAIL;

	if (FAILED(Init_NowMonsters()))
		return E_FAIL;

	if (FAILED(Init_PokemonInfoUI()))
		return E_FAIL;

	if (FAILED(Init_FoodInventory()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFeeding_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (FAILED(Init_PickingFood()))
		return E_FAIL;

	if (FAILED(Init_NowMonsters()))
		return E_FAIL;

	if (FAILED(Init_PokemonInfoUI()))
		return E_FAIL;

	if (FAILED(Init_FoodInventory()))
		return E_FAIL;

	return S_OK;
}

_uint CFeeding_Manager::Tick(_double TimeDelta)
{
	State_Tick(TimeDelta);
	Change_State();

	//if (KEY_TAB(KEY::J))
	//{
	//	for (auto& iter : m_pPokemonInfoUIs)
	//	{
	//		iter->StartDisolve();
	//	}
	//}

	if (m_pPickingFood)
		m_pPickingFood->Tick(TimeDelta);

	return _uint();
}

_uint CFeeding_Manager::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	if (m_pPickingFood)
		m_pPickingFood->LateTick(TimeDelta);

	return _uint();
}

HRESULT CFeeding_Manager::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CFeeding_Manager::Init_NowMonsters()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = nullptr;

	pObject = pGameInstance->Get_Object(LEVEL_FEEDING, L"Layer_Player", L"Player1");
	if (nullptr == pObject)
		return E_FAIL;
	CMonster* pNowMonster1 = dynamic_cast<CPlayer*>(pObject);
	if (nullptr == pNowMonster1)
		return E_FAIL;
	Safe_AddRef(pNowMonster1);
	m_pNowMonsters.push_back(pNowMonster1);

	pObject = pGameInstance->Get_Object(LEVEL_FEEDING, L"Layer_Player", L"Player2");
	if (nullptr == pObject)
		return E_FAIL;
	CMonster* pNowMonster2 = dynamic_cast<CStageSupportMonster*>(pObject);
	if (nullptr == pNowMonster2)
		return E_FAIL;
	Safe_AddRef(pNowMonster2);
	m_pNowMonsters.push_back(pNowMonster2);

	pObject = pGameInstance->Get_Object(LEVEL_FEEDING, L"Layer_Player", L"Player3");
	if (nullptr == pObject)
		return E_FAIL;
	CMonster* pNowMonster3 = dynamic_cast<CStageSupportMonster*>(pObject);
	if (nullptr == pNowMonster3)
		return E_FAIL;
	Safe_AddRef(pNowMonster3);
	m_pNowMonsters.push_back(pNowMonster3);

	return S_OK;
}

HRESULT CFeeding_Manager::Init_PickingFood()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CFood::FOOD_DESC foodDesc{};

	foodDesc.m_eCurState = CFood::STATE_PICKING;
	foodDesc.m_foodType = CFood::TYPE_RED;

	foodDesc.m_UIDesc.m_fSizeX = 45.f;
	foodDesc.m_UIDesc.m_fSizeY = 45.f;
	foodDesc.m_UIDesc.m_fX = 0.f;
	foodDesc.m_UIDesc.m_fY = 0.f;
	foodDesc.m_UIDesc.m_ShaderPass = 0;
	foodDesc.m_UIDesc.m_TextureProtoTypeLevel = 0;
	foodDesc.m_UIDesc.m_UIType = 0;
	lstrcpy(foodDesc.m_UIDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_Item_Red_UC");

	pGameInstance->Clone_GameObject(L"Layer_UI", m_iLevelindex, TEXT("Prototype_GameObject_Food"), (CGameObject**)&m_pPickingFood, &foodDesc);
	if (nullptr == m_pPickingFood)
		return E_FAIL;

	m_pPickingFood->Set_RenderId(RENDER_END);

	return S_OK;
}


HRESULT CFeeding_Manager::Init_PokemonInfoUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_FEEDING, L"Layer_UI", L"PokemonInfoUI01");
	if (nullptr == pObject)
		return E_FAIL;
	CPokemonInfoUI* pPokemonInfoUI01 = dynamic_cast<CPokemonInfoUI*>(pObject);
	if (nullptr == pPokemonInfoUI01)
		return E_FAIL;
	Safe_AddRef(pPokemonInfoUI01);
	m_pPokemonInfoUIs.push_back(pPokemonInfoUI01);

	pObject = pGameInstance->Get_Object(LEVEL_FEEDING, L"Layer_UI", L"PokemonInfoUI02");
	if (nullptr == pObject)
		return E_FAIL;
	CPokemonInfoUI* pPokemonInfoUI02 = dynamic_cast<CPokemonInfoUI*>(pObject);
	if (nullptr == pPokemonInfoUI02)
		return E_FAIL;
	Safe_AddRef(pPokemonInfoUI02);
	m_pPokemonInfoUIs.push_back(pPokemonInfoUI02);

	pObject = pGameInstance->Get_Object(LEVEL_FEEDING, L"Layer_UI", L"PokemonInfoUI03");
	if (nullptr == pObject)
		return E_FAIL;
	CPokemonInfoUI* pPokemonInfoUI03 = dynamic_cast<CPokemonInfoUI*>(pObject);
	if (nullptr == pPokemonInfoUI03)
		return E_FAIL;
	Safe_AddRef(pPokemonInfoUI03);
	m_pPokemonInfoUIs.push_back(pPokemonInfoUI03);

	return S_OK;
}

HRESULT CFeeding_Manager::Init_FoodInventory()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Get_Object(LEVEL_FEEDING, L"Layer_UI", L"FoodInventory");
	if (nullptr == pObject)
		return E_FAIL;
	m_pFoodInventory = dynamic_cast<CFoodInventory*>(pObject);
	if (nullptr == m_pFoodInventory)
		return E_FAIL;
	Safe_AddRef(m_pFoodInventory);

	return S_OK;
}

void CFeeding_Manager::State_Tick(const _double& TimeDelta)
{
	switch (m_eCurState)
	{
	case MANAGER_IDLE:
		Picking();
		break;
	case MANAGER_FOOD_PICKING:
		Inventory_Food_Picking_Tick();
		break;
	case MANAGER_EVOLUTION:
		Evolution_Tick(TimeDelta);
		break;
	}

}

void CFeeding_Manager::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case MANAGER_IDLE:
			if (m_ePreState == MANAGER_EVOLUTION)
			{
				CGameInstance::GetInstance()->PlaySoundW(L"JGL_SHINKA_After.ogg", Engine::SOUND_EFFECT);
			}
			m_eRenderId = RENDER_END;
			m_pPickingFood->Set_RenderId(RENDER_END);
			break;
		case MANAGER_FOOD_PICKING:
			m_eRenderId = RENDER_UI;
			m_pPickingFood->Set_RenderId(RENDER_UI);
			break;
		case MANAGER_EVOLUTION:
			Evolution_Change_State();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CFeeding_Manager::Picking()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_pPickingFood->Set_Pos({ (_float)pt.x - g_iWinSizeX * 0.5f, -(_float)pt.y - 20.f + g_iWinSizeY * 0.5f, 0.f });
	Set_Pos({ (_float)pt.x - g_iWinSizeX * 0.5f, -(_float)pt.y - 20.f + g_iWinSizeY * 0.5f, 0.f });

	if (MOUSE_TAB(MOUSE::LBTN) && CClient_Utility::Mouse_Pos_In_Platform() && m_eCurState == MANAGER_IDLE)
	{
		if (m_pFoodInventory->Check_Is_In(pt))
		{
			if (m_pFoodInventory->Check_Exist_Food_Is_In(m_ePickingFoodType, pt))
			{
				CGameInstance::GetInstance()->PlaySoundW(L"SE_SYS_TOUCH_2.ogg", SOUND_EFFECT);

				m_pPickingFood->Change_FoodType(m_ePickingFoodType);
				m_eCurState = MANAGER_FOOD_PICKING;
			}
		}
	}
}

void CFeeding_Manager::Inventory_Food_Picking_Tick()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (MOUSE_HOLD(MOUSE::LBTN))
	{
		m_pPickingFood->Set_Pos({ (_float)pt.x - g_iWinSizeX * 0.5f, -(_float)pt.y - 20.f + g_iWinSizeY * 0.5f, 0.f });
		Set_Pos({ (_float)pt.x - g_iWinSizeX * 0.5f, -(_float)pt.y - 20.f + g_iWinSizeY * 0.5f, 0.f });
	}
	else if (MOUSE_AWAY(MOUSE::LBTN))
	{
		for (_uint i = 0; i < m_pPokemonInfoUIs.size(); ++i)
		{
			if (m_pPokemonInfoUIs.at(i)->Check_Is_In(pt))
			{
				m_pPokemonInfoUIs.at(i)->Add_Exp((_int)1);

				m_pFoodInventory->Add_FoodNums(m_ePickingFoodType, -1);

				CGameInstance::GetInstance()->PlaySoundW(L"SE_SYS_FEED.ogg", SOUND_UI);
				CGameInstance::GetInstance()->PlaySoundW(L"SE_SYS_EAT.ogg", SOUND_STAGE_UI);

				if (m_pPokemonInfoUIs.at(i)->Check_CanEvolution())
				{
					m_evolutionPokemonIndex = i;
					m_eCurState = MANAGER_EVOLUTION;
					return;
				}
				break;
			}
		}

		m_eCurState = MANAGER_IDLE;
	}
}

void CFeeding_Manager::Evolution_Change_State()
{
	CGameInstance::GetInstance()->PlaySoundW(L"JGL_SHINKA_SE.ogg", Engine::SOUND_UI);
	m_eRenderId = RENDER_END;
	m_pPickingFood->Set_RenderId(RENDER_END);
	m_evolutionTickAcc = 0.0;

	_uint pokemonNo = m_pPokemonInfoUIs.at(m_evolutionPokemonIndex)->Get_PokemonNo();

	m_pNowMonsters.at(m_evolutionPokemonIndex)->Set_PokemonNo(pokemonNo + 1);
	m_pNowMonsters.at(m_evolutionPokemonIndex)->Add_AttackBasis(_int(61 + rand() % 50));
	m_pNowMonsters.at(m_evolutionPokemonIndex)->Add_HpBasis(_int(88 + rand() % 150));
	m_pNowMonsters.at(m_evolutionPokemonIndex)->LevelUp();

	for (size_t i = 0; i < m_pNowMonsters.size(); ++i)
	{
		m_pNowMonsters.at(i)->Save_By_JsonFile(m_pNowMonsters.at(i)->Get_JsonPath().c_str());
	}

	m_pPokemonInfoUIs.at(m_evolutionPokemonIndex)->StartDisolve();
	m_pPokemonInfoUIs.at(m_evolutionPokemonIndex)->Init_PokemonData(m_evolutionPokemonIndex + 1);
}

void CFeeding_Manager::Evolution_Tick(const _double& TimeDelta)
{
	if (m_evolutionTickAcc >= m_evolutionTick)
	{
		m_eCurState = MANAGER_IDLE;

	}

	m_evolutionTickAcc += TimeDelta;
}


HRESULT CFeeding_Manager::SetUp_ShaderResources()
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

CFeeding_Manager* CFeeding_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFeeding_Manager* pInstance = new CFeeding_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CFeeding_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFeeding_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CFeeding_Manager* pInstance = new CFeeding_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CFeeding_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFeeding_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CFeeding_Manager* pInstance = new CFeeding_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CFeeding_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFeeding_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pFoodInventory);

	for (auto& iter : m_pNowMonsters)
	{
		Safe_Release(iter);
	}

	for (auto& iter : m_pPokemonInfoUIs)
	{
		Safe_Release(iter);
	}

	Safe_Release(m_pPickingFood);
}
