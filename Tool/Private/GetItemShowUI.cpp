#include"stdafx.h"

#include "GetItemShowUI.h"

#include "GameInstance.h"

#include "Stage_Manager.h"

CGetItemShowUI::CGetItemShowUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CGetItemShowUI::CGetItemShowUI(const CGetItemShowUI& rhs)
	: CUI(rhs)
{
}

HRESULT CGetItemShowUI::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGetItemShowUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	m_foods.resize(CFood::TYPE_END, 0);

	return S_OK;
}

HRESULT CGetItemShowUI::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	m_foods.resize(CFood::TYPE_END, 0);

	return S_OK;
}

_uint CGetItemShowUI::Tick(_double TimeDelta)
{
	if (KEY_TAB(KEY::O))
	{
		CGameObject* pPlayer1 = CGameInstance::GetInstance()->Get_Object(m_iLevelindex, L"Layer_Player", L"Player1");
		if (nullptr != pPlayer1)
		{
			CTransform* pTransform = pPlayer1->Get_As<CTransform>();
			_matrix vMatrix = pTransform->Get_WorldMatrix_Matrix();

			CStone::STONE_DESC stoneDesc{};

			stoneDesc.m_stoneType = CStone::TYPE_ATK;
			stoneDesc.value = 123;
			stoneDesc.m_pokemonIconNumber = 25;
			stoneDesc.m_UIDesc.m_fSizeX = 40.f;
			stoneDesc.m_UIDesc.m_fSizeY = 40.f;
			stoneDesc.m_UIDesc.m_TextureProtoTypeLevel = LEVEL_STATIC;
			stoneDesc.m_UIDesc.m_UIType = 0;
			stoneDesc.m_UIDesc.m_ShaderPass = 0;
			lstrcpy(stoneDesc.m_UIDesc.m_TextureProtoTypeName, L"Prototype_Component_Texture_window_ATK_icon");

			Add_Stone(stoneDesc, vMatrix);
		}

	}
	else if (KEY_TAB(KEY::P))
	{
		CGameObject* pPlayer1 = CGameInstance::GetInstance()->Get_Object(m_iLevelindex, L"Layer_Player", L"Player1");
		if (nullptr != pPlayer1)
		{
			CTransform* pTransform = pPlayer1->Get_As<CTransform>();
			_matrix vMatrix = pTransform->Get_WorldMatrix_Matrix();

			CFood::Food_Desc foodDesc{};

			_int randValue = rand() % 4;
			foodDesc.m_foodType = CFood::TYPE(randValue);
			foodDesc.m_UIDesc.m_fSizeX = 40.f;
			foodDesc.m_UIDesc.m_fSizeY = 40.f;
			foodDesc.m_UIDesc.m_TextureProtoTypeLevel = LEVEL_STATIC;
			foodDesc.m_UIDesc.m_UIType = 0;
			foodDesc.m_UIDesc.m_ShaderPass = 0;

			Add_Food(foodDesc, vMatrix);
		}
	}

	GetStonesTick(TimeDelta);
	GetFoodsTick(TimeDelta);

	return __super::Tick(TimeDelta);
}

_uint CGetItemShowUI::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	for (auto& part : m_TextureParts)
	{
		part->LateTick(TimeDelta);
	}

	for (auto& part : m_TextParts)
	{
		part->LateTick(TimeDelta);
	}

	for (auto& iter : m_GetStones)
	{
		iter->LateTick(TimeDelta);
	}

	for (auto& iter : m_GetFoods)
	{
		iter->LateTick(TimeDelta);
	}

	return 0;
}

HRESULT CGetItemShowUI::Render()
{
	return __super::Render();
}

HRESULT CGetItemShowUI::Add_Stone(const CStone::STONE_DESC& stoneDesc, _fmatrix vStartWorldMatrix)
{
	//if (m_StoneNum >= m_canMaxGetStoneNum)
	//	return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CStone::STONE_DESC addStoneDesc = stoneDesc;
	addStoneDesc.m_eCurState = CStone::STATE_STAGE_VIEW;

	CStone* pStone = nullptr;

	pGameInstance->Clone_GameObject(L"Layer_UI", m_iLevelindex, TEXT("Prototype_GameObject_Stone"), (CGameObject**)&pStone, &addStoneDesc);
	if (nullptr == pStone)
		return E_FAIL;

	_matrix		ViewPortMatrix = CGameInstance::GetInstance()->Get_ViewPort_Matrix(0.f, -30.f, g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);

	_matrix viewMatrix = pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW);
	_matrix projMatrix = pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);

	_float4x4 ParentMat{};
	XMStoreFloat4x4(&ParentMat, vStartWorldMatrix);
	XMStoreFloat4x4(&ParentMat, XMLoadFloat4x4(&ParentMat) * viewMatrix * projMatrix * ViewPortMatrix);

	_float4x4 mat = {};
	XMStoreFloat4x4(&mat, XMMatrixTranslation((ParentMat.m[3][0]) / ParentMat.m[3][2] - g_iWinSizeX * 0.5f,
		-((ParentMat.m[3][1]) / ParentMat.m[3][2]) + g_iWinSizeY * 0.5f, 0.1f));

	pStone->Set_Pos({ mat.m[3][0], mat.m[3][1], 0.1f });
	m_GetStones.push_back(pStone);

	++m_StoneNum;
	m_stones.push_back(stoneDesc);

	return S_OK;
}

HRESULT CGetItemShowUI::Add_Food(const CFood::FOOD_DESC& foodDesc, _fmatrix vStartWorldMatrix)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CFood::FOOD_DESC addFoodDesc = foodDesc;
	addFoodDesc.m_eCurState = CFood::STATE_STAGE_VIEW;

	CFood* pFood = nullptr;

	pGameInstance->Clone_GameObject(L"Layer_UI", m_iLevelindex, TEXT("Prototype_GameObject_Food"), (CGameObject**)&pFood, &addFoodDesc);
	if (nullptr == pFood)
		return E_FAIL;

	_matrix		ViewPortMatrix = CGameInstance::GetInstance()->Get_ViewPort_Matrix(0.f, -30.f, g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);

	_matrix viewMatrix = pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW);
	_matrix projMatrix = pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);

	_float4x4 ParentMat{};
	XMStoreFloat4x4(&ParentMat, vStartWorldMatrix);
	XMStoreFloat4x4(&ParentMat, XMLoadFloat4x4(&ParentMat) * viewMatrix * projMatrix * ViewPortMatrix);

	_float4x4 mat = {};
	XMStoreFloat4x4(&mat, XMMatrixTranslation((ParentMat.m[3][0]) / ParentMat.m[3][2] - g_iWinSizeX * 0.5f,
		-((ParentMat.m[3][1]) / ParentMat.m[3][2]) + g_iWinSizeY * 0.5f, 0.1f));

	pFood->Set_Pos({ mat.m[3][0], mat.m[3][1], 0.1f});
	m_GetFoods.push_back(pFood);

	++m_FoodNum;
	++m_foods.at((size_t)foodDesc.m_foodType);

	return S_OK;
}

_uint CGetItemShowUI::GetStonesTick(const _double& TimeDelta)
{
	_float3 alivePos = m_TextureParts.at(m_StoneIconBaseTextureIndex)->Get_FinalWorldMatrixPosition();
	_float2	vUISize = m_TextureParts.at(m_StoneIconBaseTextureIndex)->Get_UISize();

	for (auto iter = m_GetStones.begin(); iter != m_GetStones.end();)
	{
		if (nullptr != (*iter))
		{
			(*iter)->Tick(TimeDelta);

			if ((*iter)->Move_To_ViewPortPositoin(TimeDelta, XMLoadFloat3(&alivePos), vUISize))
			{
				m_TextParts.at(m_StoneNumTextIndex)->Set_Text(to_wstring(m_StoneNum));

				//Safe_Release(*iter);
				//iter = m_GetStones.erase(iter);
				//continue;
			}
			++iter;
		}
	}

	return _uint();
}

_uint CGetItemShowUI::GetFoodsTick(const _double& TimeDelta)
{
	_float3 alivePos = m_TextureParts.at(m_FoodIconBaseTextureIndex)->Get_FinalWorldMatrixPosition();
	_float2	vUISize = m_TextureParts.at(m_FoodIconBaseTextureIndex)->Get_UISize();

	for (auto iter = m_GetFoods.begin(); iter != m_GetFoods.end();)
	{
		if (nullptr != (*iter))
		{
			(*iter)->Tick(TimeDelta);

			if ((*iter)->Move_To_ViewPortPositoin(TimeDelta, XMLoadFloat3(&alivePos), vUISize))
			{
				m_TextParts.at(m_FoodNumTextIndex)->Set_Text(to_wstring(m_FoodNum));

				//Safe_Release(*iter);
				//iter = m_GetFoods.erase(iter);
				//continue;
			}
			++iter;
		}
	}

	return _uint();
}

CGetItemShowUI* CGetItemShowUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGetItemShowUI* pInstance = new CGetItemShowUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CGetItemShowUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGetItemShowUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CGetItemShowUI* pInstance = new CGetItemShowUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))

	{
		MSG_BOX("Failed to Cloned CGetItemShowUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGetItemShowUI::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CGetItemShowUI* pInstance = new CGetItemShowUI(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CGetItemShowUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGetItemShowUI::Free()
{
	__super::Free();

	for (auto& iter : m_GetStones)
	{
		Safe_Release(iter);
	}

	for (auto& iter : m_GetFoods)
	{
		Safe_Release(iter);
	}
}
