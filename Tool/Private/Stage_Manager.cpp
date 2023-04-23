#include "stdafx.h"
#include "Stage_Manager.h"

#include "GameInstance.h"

#include "StageCamera.h"
#include "Monster.h"
#include "PokemonSkillButton.h"
#include "Client_Utility.h"
#include "Effect_Manager.h"
#include "Pokering.h"
#include "EnemySpawnPoint.h"
#include "EnemyPack.h"
#include "StageMessageInfo.h"
#include "StageClearUI.h"

#include "Level_Loading.h"

CStage_Manager::CStage_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CStage_Manager::CStage_Manager(const CStage_Manager& rhs)
	: CGameObject(rhs)
{

}

HRESULT CStage_Manager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStage_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Init_ManagerInfo()))
		return E_FAIL;

	m_enemySpawnPoints.reserve(10);

	return S_OK;
}

HRESULT CStage_Manager::Initialize(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	if (FAILED(__super::Initialize(pLayerTag, iLevelIndex, filePath)))
		return E_FAIL;

	if (filePath)
	{
		Load_By_JsonFile(filePath);
		m_strSaveJsonPath = filePath;
	}

	if (FAILED(Add_Components_By_File()))
		return E_FAIL;

	if (FAILED(Init_ManagerInfo()))
		return E_FAIL;

	m_enemySpawnPoints.reserve(10);

	return S_OK;
}

_uint CStage_Manager::Tick(_double TimeDelta)
{
	if (m_returnToWorldMap)
	{
		CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_WORLDMAP));

		return OBJ_SCENE_CHNAGE;
	}

	State_Tick(TimeDelta);
	Change_State();

	return _uint();
}

_uint CStage_Manager::LateTick(_double TimeDelta)
{
	m_pRendererCom->Add_RenderGroup(m_eRenderId, this);

	if (nullptr != m_pEnemyPack && m_pEnemyPack->Is_CanNextSpawn())
	{
		_int nextSpawnIndex = m_pEnemyPack->Get_NextEnemyPack();

		if (nextSpawnIndex >= m_enemySpawnPoints.size())
		{
			return 0; 
		}

		if (nextSpawnIndex == 0)
		{
			if (m_pStageMessageInfo->Is_NoMessageState())
			{
				CEnemySpawnPoint* pEnemySpawnPoint = m_enemySpawnPoints.at(nextSpawnIndex);
				CEnemySpawnPoint::ENEMY_SPAWN_POINT_DESC desc = move(pEnemySpawnPoint->Get_EnemySpawnPointDesc());
				if (nullptr != pEnemySpawnPoint)
				{
					m_pEnemyPack->Next_Spawn(desc.m_position, desc.m_spawnRadius);
					m_pStageMessageInfo->Open_Message(L"야생 포켓몬이다!", 245.f);
				}
			}

		}
		else
		{
			CEnemySpawnPoint* pEnemySpawnPoint = m_enemySpawnPoints.at(nextSpawnIndex);
			CEnemySpawnPoint::ENEMY_SPAWN_POINT_DESC desc = move(pEnemySpawnPoint->Get_EnemySpawnPointDesc());

			if (CEnemySpawnPoint::TYPE::TYPE_NORMAL_APPEARANCE == desc.m_type)
			{
				m_pStageMessageInfo->Open_Message(L"새로운 포켓몬이 나타났다!", 180.f);
				m_pMainCamera->Set_Move_To_Point(pEnemySpawnPoint, false);
			}
			else if (CEnemySpawnPoint::TYPE::TYPE_SURPIRSE_APPEARANCE == desc.m_type)
			{
				CGameObject* pPlayer1 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", L"Player1");
				if (pPlayer1)
				{
					CTransform* pTransform = pPlayer1->Get_As<CTransform>();
					XMStoreFloat3(&desc.m_position, pTransform->Get_State(CTransform::STATE_POSITION));
				}
				m_pStageMessageInfo->Open_Message(L"포위당했다!", 260.f);
			}
			else if (CEnemySpawnPoint::TYPE::TYPE_BOSS_APPEARANCE == desc.m_type)
			{
				m_pStageMessageInfo->Open_Message(L"강해 보이는 포켓몬이 나타났다!", 150.f);
				m_pMainCamera->Set_Move_To_Point(pEnemySpawnPoint, true);
			}

			if (nullptr != pEnemySpawnPoint)
			{
				//m_pStageClearUI->Open_Message(L"WAVE CLEAR!", 330.f);
				m_pEnemyPack->Next_Spawn(desc.m_position, desc.m_spawnRadius);
			}
		}

	}

	return _uint();
}

HRESULT CStage_Manager::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}


void CStage_Manager::Boss_DeadEffect(_bool isEnd, _fvector vPos)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CEffect_Manager* pEffect_Manager = dynamic_cast<CEffect_Manager*>(pGameInstance->Get_Object(LEVEL_STATIC, L"Layer_Manager", L"Effect_Manager"));
	if (nullptr == pEffect_Manager)
		return;

	CSkillEffect* pSkillEffect = nullptr;

	if (false == isEnd)
		pSkillEffect = pEffect_Manager->CreateEffect(CEffect_Manager::m_damageBoss, L"Prototype_GameObject_SkillEffect", Get_LayerTag().c_str(), Get_Levelindex());
	else
	{
		//m_pStageClearUI->Open_Message(L"STAGE CLEAR!", 290.f);
		pSkillEffect = pEffect_Manager->CreateEffect(CEffect_Manager::m_damageBossEnd, L"Prototype_GameObject_SkillEffect", Get_LayerTag().c_str(), Get_Levelindex());
		CGameInstance::GetInstance()->StopAll();
		CGameInstance::GetInstance()->PlayBGM(L"BGM_Stage_Clear.ogg");
		m_eCurState = MANAGER_CLEAR;
	}

	if (nullptr != pSkillEffect)
	{
		_float4 vCreatePos{};
		XMStoreFloat4(&vCreatePos, vPos);
		pSkillEffect->Set_Pos(vCreatePos);
	}

	Safe_Release(pSkillEffect);
}

void CStage_Manager::Add_EnemySpawnPoint(CEnemySpawnPoint* pEnemySpawnPoint)
{
	if (nullptr == pEnemySpawnPoint)
		return;

	m_enemySpawnPoints.push_back(pEnemySpawnPoint);
	Safe_AddRef(pEnemySpawnPoint);
}

void CStage_Manager::Set_EnemyPack(CEnemyPack* pEnemyPack)
{
	if (nullptr == pEnemyPack || nullptr != m_pEnemyPack)
		return;

	m_pEnemyPack = pEnemyPack;
	Safe_AddRef(m_pEnemyPack);
}

void CStage_Manager::Set_StageMessageInfo(CStageMessageInfo* pStageMessageInfo)
{
	if (nullptr == pStageMessageInfo || nullptr != m_pStageMessageInfo)
		return;

	m_pStageMessageInfo = pStageMessageInfo;
	Safe_AddRef(m_pStageMessageInfo);
}

void CStage_Manager::Set_StageClearUI(CStageClearUI* pStageClearUI)
{
	if (nullptr == pStageClearUI || nullptr != m_pStageClearUI)
		return;

	m_pStageClearUI = pStageClearUI;
	Safe_AddRef(m_pStageClearUI);
}

_bool CStage_Manager::Request_TurnToCamera(CTransform* pTransform, const _double& TimeDelta)
{
	if (nullptr == pTransform)
		return true; 

	_float4 vCameraPos = CGameInstance::GetInstance()->Get_CamPosition();

	return pTransform->TurnToTarget(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMLoadFloat4(&vCameraPos), _float(TimeDelta));
}

HRESULT CStage_Manager::Init_ManagerInfo()
{
	m_eRenderId = RENDER_BLEND_UI;

	m_vCurrentFadeColor = m_Desc.m_FadeInStartColor;

	m_fSizeX = (_float)g_iWinSizeX * 1.5f;
	m_fSizeY = (_float)g_iWinSizeY * 1.5f;
	m_fX = (_float)(g_iWinSizeX >> 1);
	m_fY = (_float)(g_iWinSizeY >> 1);

	m_pTransformCom->Set_Scaled({ m_fSizeX, m_fSizeY, 1.f });
	m_pTransformCom->Set_Pos(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_pMainCamera = (CStageCamera*)CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Camera", L"Main_Camera");
	if (nullptr == m_pMainCamera)
		return E_FAIL;

	Safe_AddRef(m_pMainCamera);

	Init_PlayersPos();

	return S_OK;
}

HRESULT CStage_Manager::Init_PlayersPos()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CTransform* pTransform = nullptr;
	CNavigation* pNaviagtion = nullptr;

	CGameObject* pPlayer1 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", L"Player1");
	if (pPlayer1)
	{
		pTransform = pPlayer1->Get_As<CTransform>();
		pTransform->Set_Pos(30.54f, 0.5f, 19.28f);

		pNaviagtion = pPlayer1->Get_As<CNavigation>();
		pNaviagtion->Set_Index_By_Position({ 30.54f, 0.5f, 19.28f });

		CGameObject* pPlayer1Skill0 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_UI", L"Player1_Skill0");
		if (nullptr != pPlayer1Skill0)
		{
			dynamic_cast<CPokemonSkillButton*>(pPlayer1Skill0)->Set_ParentMonster((CMonster*)pPlayer1);
			dynamic_cast<CPokemonSkillButton*>(pPlayer1Skill0)->Set_SkillNumber(0);

		}

		CGameObject* pPlayer1Skill1 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_UI", L"Player1_Skill1");
		if (nullptr != pPlayer1Skill1)
		{
			dynamic_cast<CPokemonSkillButton*>(pPlayer1Skill1)->Set_ParentMonster((CMonster*)pPlayer1);
			dynamic_cast<CPokemonSkillButton*>(pPlayer1Skill1)->Set_SkillNumber(1);

		}

		CModel* pPlayerModel = pPlayer1->Get_As<CModel>();

		CPokering::POKERING_DESC pokeringDesc{};

		pokeringDesc.pBonePtr = pPlayerModel->Get_BonePtr("rootJT");
		lstrcpy(pokeringDesc.modelPrototypeTag, L"Prototype_Component_Model_PokeringA");
		XMStoreFloat4x4(&pokeringDesc.PivotMatrix, pPlayerModel->Get_PivotMatrix());
		pokeringDesc.pParent = dynamic_cast<CMonster*>(pPlayer1);
		pokeringDesc.pParentTransform = pPlayer1->Get_As<CTransform>();

		pGameInstance->Add_GameObject(L"Prototype_GameObject_Pokering", LEVEL_STAGE, L"Layer_Pokering", L"PokeringA", &pokeringDesc);
	}

	CGameObject* pPlayer2 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", L"Player2");
	if (pPlayer2)
	{
		pTransform = pPlayer2->Get_As<CTransform>();
		pTransform->Set_Pos(29.f, 0.5f, 18.5f);

		pNaviagtion = pPlayer2->Get_As<CNavigation>();
		pNaviagtion->Set_Index_By_Position({ 29.f, 0.5f, 18.5f });

		CGameObject* pPlayer2Skill0 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_UI", L"Player2_Skill0");
		if (nullptr != pPlayer2Skill0)
		{
			dynamic_cast<CPokemonSkillButton*>(pPlayer2Skill0)->Set_ParentMonster((CMonster*)pPlayer2);
			dynamic_cast<CPokemonSkillButton*>(pPlayer2Skill0)->Set_SkillNumber(0);

		}

		CGameObject* pPlayer2Skill1 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_UI", L"Player2_Skill1");
		if (nullptr != pPlayer2Skill1)
		{
			dynamic_cast<CPokemonSkillButton*>(pPlayer2Skill1)->Set_ParentMonster((CMonster*)pPlayer2);
			dynamic_cast<CPokemonSkillButton*>(pPlayer2Skill1)->Set_SkillNumber(1);
		}

		CModel* pPlayerModel = pPlayer2->Get_As<CModel>();

		CPokering::POKERING_DESC pokeringDesc{};

		pokeringDesc.pBonePtr = pPlayerModel->Get_BonePtr("rootJT");
		lstrcpy(pokeringDesc.modelPrototypeTag, L"Prototype_Component_Model_PokeringB");
		XMStoreFloat4x4(&pokeringDesc.PivotMatrix, pPlayerModel->Get_PivotMatrix());
		pokeringDesc.pParent = dynamic_cast<CMonster*>(pPlayer2);
		pokeringDesc.pParentTransform = pPlayer2->Get_As<CTransform>();

		pGameInstance->Add_GameObject(L"Prototype_GameObject_Pokering", LEVEL_STAGE, L"Layer_Pokering", L"PokeringB", &pokeringDesc);
	}

	CGameObject* pPlayer3 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", L"Player3");
	if (pPlayer3)
	{
		pTransform = pPlayer3->Get_As<CTransform>();
		pTransform->Set_Pos(31.f, 0.5f, 18.5f);

		pNaviagtion = pPlayer3->Get_As<CNavigation>();
		pNaviagtion->Set_Index_By_Position({ 31.f, 0.5f, 18.5f });

		CGameObject* pPlayer3Skill0 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_UI", L"Player3_Skill0");
		if (nullptr != pPlayer3Skill0)
		{
			dynamic_cast<CPokemonSkillButton*>(pPlayer3Skill0)->Set_ParentMonster((CMonster*)pPlayer3);
			dynamic_cast<CPokemonSkillButton*>(pPlayer3Skill0)->Set_SkillNumber(0);

		}

		CGameObject* pPlayer3Skill1 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_UI", L"Player3_Skill1");
		if (nullptr != pPlayer3Skill1)
		{
			dynamic_cast<CPokemonSkillButton*>(pPlayer3Skill1)->Set_ParentMonster((CMonster*)pPlayer3);
			dynamic_cast<CPokemonSkillButton*>(pPlayer3Skill1)->Set_SkillNumber(1);
		}


		CModel* pPlayerModel = pPlayer3->Get_As<CModel>();

		CPokering::POKERING_DESC pokeringDesc{};

		pokeringDesc.pBonePtr = pPlayerModel->Get_BonePtr("rootJT");
		lstrcpy(pokeringDesc.modelPrototypeTag, L"Prototype_Component_Model_PokeringC");
		XMStoreFloat4x4(&pokeringDesc.PivotMatrix, pPlayerModel->Get_PivotMatrix());
		pokeringDesc.pParent = dynamic_cast<CMonster*>(pPlayer3);
		pokeringDesc.pParentTransform = pPlayer3->Get_As<CTransform>();

		pGameInstance->Add_GameObject(L"Prototype_GameObject_Pokering", LEVEL_STAGE, L"Layer_Pokering", L"PokeringC", &pokeringDesc);
	}

	CGameObject* pCameraTarget = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_CameraTarget", L"CameraTarget");
	if (pCameraTarget)
	{
		pTransform = pCameraTarget->Get_As<CTransform>();
		pTransform->Set_Pos(30.54f, 0.5f, 19.28f);
	}
	

	return S_OK;
}

void CStage_Manager::Fade_In(const _double& TimeDelta)
{
	if (m_Desc.m_FadeSecond <= m_fCurrentFadeTime)
	{
		m_eCurState = MANAGER_IDLE;
		return;
	}
	m_vCurrentFadeColor.x -= (_float)TimeDelta;
	m_vCurrentFadeColor.y -= (_float)TimeDelta;
	m_vCurrentFadeColor.z -= (_float)TimeDelta;
	m_vCurrentFadeColor.w -= (_float)TimeDelta;

	m_fCurrentFadeTime += TimeDelta;
}


_bool CStage_Manager::Save_By_JsonFile_Impl(Document& doc, Document::AllocatorType& allocator)
{
	Value ManagerDesc(kObjectType);
	{
		ManagerDesc.AddMember("m_FadeSecond", m_Desc.m_FadeSecond, allocator);

		Value m_FadeInStartColor(kObjectType);
		{
			m_FadeInStartColor.AddMember("x", m_Desc.m_FadeInStartColor.x, allocator);
			m_FadeInStartColor.AddMember("y", m_Desc.m_FadeInStartColor.y, allocator);
			m_FadeInStartColor.AddMember("z", m_Desc.m_FadeInStartColor.z, allocator);
			m_FadeInStartColor.AddMember("w", m_Desc.m_FadeInStartColor.w, allocator);
		}
		ManagerDesc.AddMember("m_FadeInStartColor", m_FadeInStartColor, allocator);

		Value m_FadeInEndColor(kObjectType);
		{
			m_FadeInEndColor.AddMember("x", m_Desc.m_FadeInEndColor.x, allocator);
			m_FadeInEndColor.AddMember("y", m_Desc.m_FadeInEndColor.y, allocator);
			m_FadeInEndColor.AddMember("z", m_Desc.m_FadeInEndColor.z, allocator);
			m_FadeInEndColor.AddMember("w", m_Desc.m_FadeInEndColor.w, allocator);
		}
		ManagerDesc.AddMember("m_FadeInEndColor", m_FadeInEndColor, allocator);

	}
	doc.AddMember("ManagerDesc", ManagerDesc, allocator);


	return true;
}

_bool CStage_Manager::Load_By_JsonFile_Impl(Document& doc)
{
	const Value& ManagerDesc = doc["ManagerDesc"];


	const Value& m_FadeInStartColor = ManagerDesc["m_FadeInStartColor"];
	m_Desc.m_FadeInStartColor = _float4(m_FadeInStartColor["x"].GetFloat(), m_FadeInStartColor["y"].GetFloat(), 
		m_FadeInStartColor["z"].GetFloat(), m_FadeInStartColor["w"].GetFloat());

	const Value& m_FadeInEndColor = ManagerDesc["m_FadeInEndColor"];
	m_Desc.m_FadeInEndColor = _float4(m_FadeInEndColor["x"].GetFloat(), m_FadeInEndColor["y"].GetFloat(),
		m_FadeInEndColor["z"].GetFloat(), m_FadeInEndColor["w"].GetFloat());

	m_Desc.m_FadeSecond = ManagerDesc["m_FadeSecond"].GetFloat();
	return true;
}

void CStage_Manager::State_Tick(const _double& TimeDelta)
{
	switch (m_eCurState)
	{
	case MANAGER_FADE_IN:
		Fade_In(TimeDelta);
		break;

	case MANAGER_IDLE:
		break;

	case MANAGER_CLEAR:
		Stage_Clear_Tick(TimeDelta);
		break;

	case MANAGER_OPEN_STATE_INFO:
		Open_StageInfo_Tick(TimeDelta);
		break;

	case MANAGER_FADE_OUT:
		Fade_Out(TimeDelta);
		break;
	}
}

void CStage_Manager::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case MANAGER_FADE_IN:
			m_eRenderId = RENDER_BLEND_UI;
			m_vCurrentFadeColor = m_Desc.m_FadeInStartColor;
			break;

		case MANAGER_IDLE:
			m_eRenderId = RENDER_END;
			break;

		case MANAGER_CLEAR:
			m_stageClearDelayTimeAcc = 0.0;
			m_vCurrentFadeColor = _float4(0.2f, 0.2f, 0.2f, 0.5f);
			Stage_Clear();
			break;

		case MANAGER_OPEN_STATE_INFO:
			m_eRenderId = RENDER_BLEND_UI;
			break;

		case MANAGER_FADE_OUT:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CStage_Manager::Stage_Clear()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CMonFSM* pMonFSM = nullptr;
	CModel* pModel = nullptr;

	CGameObject* pPlayer1 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", L"Player1");

	if (nullptr != pPlayer1)
	{
		pMonFSM = pPlayer1->Get_As<CMonFSM>();
		pModel = pPlayer1->Get_As<CModel>();

		if (nullptr != pMonFSM && nullptr != pModel)
		{
			pMonFSM->Transit_MotionState(CMonFSM::STAGE_CLEAR, pModel);
		}
	}

	CGameObject* pPlayer2 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", L"Player2");

	if (nullptr != pPlayer2)
	{
		pMonFSM = pPlayer2->Get_As<CMonFSM>();
		pModel = pPlayer2->Get_As<CModel>();

		if (nullptr != pMonFSM && nullptr != pModel)
		{
			pMonFSM->Transit_MotionState(CMonFSM::STAGE_CLEAR, pModel);
		}
	}

	CGameObject* pPlayer3 = CGameInstance::GetInstance()->Get_Object(LEVEL_STAGE, L"Layer_Player", L"Player3");

	if (nullptr != pPlayer3)
	{
		pMonFSM = pPlayer3->Get_As<CMonFSM>();
		pModel = pPlayer3->Get_As<CModel>();

		if (nullptr != pMonFSM && nullptr != pModel)
		{
			pMonFSM->Transit_MotionState(CMonFSM::STAGE_CLEAR, pModel);
		}
	}
}

void CStage_Manager::Stage_Clear_Tick(const _double& TimeDelta)
{
	if (m_stageClearDelayTimeAcc >= m_stageClearDelayTime)
	{
		m_eCurState = MANAGER_OPEN_STATE_INFO;
	}

	m_stageClearDelayTimeAcc += TimeDelta;
}

void CStage_Manager::Open_StageInfo_Tick(const _double& TimeDelta)
{
	if (m_openStateTimeAcc >= m_openStateTime)
	{
		m_eCurState = MANAGER_FADE_OUT;
	}

	m_openStateTimeAcc += TimeDelta;
}

void CStage_Manager::Fade_Out(const _double& TimeDelta)
{
	if (m_vCurrentFadeColor.x >= 1.f)
	{
		m_returnToWorldMap = true;
	}

	m_vCurrentFadeColor.x += (_float)TimeDelta;
	m_vCurrentFadeColor.y += (_float)TimeDelta;
	m_vCurrentFadeColor.z += (_float)TimeDelta;
	m_vCurrentFadeColor.w += (_float)TimeDelta;
}

HRESULT CStage_Manager::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(pGameInstance->Add_Component(CVIBuffer_Rect::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		(CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, LEVEL_STATIC, L"Prototype_Component_Texture_Window_Plane_Corner",
		(CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage_Manager::Add_Components_By_File()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc = { 10.f, XMConvertToRadians(90.0f) };
	if (FAILED(pGameInstance->Add_Component(CTransform::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		(CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(pGameInstance->Add_Component(CRenderer::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		(CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(pGameInstance->Add_Component(CVIBuffer_Rect::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		(CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(pGameInstance->Add_Component(CShader::familyId, this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexColor"),
		(CComponent**)&m_pShaderCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(pGameInstance->Add_Component(CTexture::familyId, this, LEVEL_STATIC, L"Prototype_Component_Texture_Window_Plane_Corner",
		(CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage_Manager::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Set_Matrix("g_ViewMatrix",
		&m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_Matrix("g_ProjMatrix",
		&m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_vCurrentFadeColor, (sizeof m_vCurrentFadeColor))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CStage_Manager* CStage_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStage_Manager* pInstance = new CStage_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CStage_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStage_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, void* pArg)
{
	CStage_Manager* pInstance = new CStage_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, pArg)))
	{
		MSG_BOX("Failed to Cloned CStage_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStage_Manager::Clone(const _tchar* pLayerTag, _uint iLevelIndex, const char* filePath)
{
	CStage_Manager* pInstance = new CStage_Manager(*this);

	if (FAILED(pInstance->Initialize(pLayerTag, iLevelIndex, filePath)))
	{
		MSG_BOX("Failed to Cloned CStage_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStage_Manager::Free()
{
	__super::Free();

	for (auto& point : m_enemySpawnPoints)
	{
		Safe_Release(point);
	}

	Safe_Release(m_pStageMessageInfo);

	Safe_Release(m_pEnemyPack);

	Safe_Release(m_pMainCamera);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pStageClearUI);

}
